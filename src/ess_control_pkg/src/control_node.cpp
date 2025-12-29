#include <chrono>
#include <cmath>
#include <memory>
#include <vector>
#include <string>
#include <atomic>
#include <thread>

// 필요한 타입 발행하는 publisher 추가들
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/int32.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "geometry_msgs/msg/pose.hpp"

#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

#include <gpiod.h>
#include <poll.h>

using namespace std::chrono_literals;

// 리프터 제어 비트 플래그
#define STOP 0
#define TOFLOOR1 (1 << 0)
#define TOFLOOR2 (1 << 1)
#define TOFLOOR3 (1 << 2)
#define GOUP     (1 << 3)
#define GODOWN   (1 << 4)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define PUBLISH_INTERVAL_RATIO 4  // 50ms 타이머 중 4번째마다 좌표 발행

class ControlNode : public rclcpp::Node {
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNav = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    ControlNode() 
    : Node("control_node"), 
      tf_buffer_(this->get_clock()), 
      tf_listener_(tf_buffer_) 
    {
        // 1. ROS 통신 설정
        sub_home_ = create_subscription<std_msgs::msg::Bool>(
            "/ess/home", 10, std::bind(&ControlNode::on_home, this, std::placeholders::_1));
        sub_thermal = create_subscription<std_msgs::msg::Int32>(
            "/ess/thermal/ack", 10, std::bind(&ControlNode::thermal_callback, this, std::placeholders::_1));
        
        pub_cmd_vel_ = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        pub_capture = create_publisher<std_msgs::msg::Int32>("/ess/request/id", 10);
        
        pub_pose_mqtt_ = create_publisher<geometry_msgs::msg::Pose>("/ess/robot_pose", 10);

        // 50ms 마다 실행되는 tick 타이머에서 좌표를 계속 발행할거임
        nav_client_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pose");
            
        // 2. GPIO 초기화
        setup_gpio();
        
        lift_inflight_ = true;
        std::thread(&ControlNode::move_to_floor_blocking, this, TOFLOOR1 | GODOWN).detach();
        // 3. 메인 루프 타이머 (FSM)
        timer_ = create_wall_timer(50ms, std::bind(&ControlNode::tick, this));
        current_row_bit = TOFLOOR1;
        target_floor_bit_ = TOFLOOR1; // 초기화 안해줘서 죽을뻔함
        RCLCPP_INFO(get_logger(), "Integrated Control Node Started. Waiting for /ess/home signal...");
    }

    ~ControlNode() {
        cleanup_gpio();
    }

private:
    enum class State { IDLE, NAV_HOME, NAV_FIRST_POS, NAV_SECOND_POS, DRIVE_STRAIGHT, START_LIFT, WAIT_LIFT, WAIT_THERMAL, NAV_BACK_HOME };
    State state_{State::NAV_HOME};

    // --- 콜백 함수 ---
    void on_home(const std_msgs::msg::Bool::SharedPtr msg) {
         if (msg->data) 
            home_requested_ = true; 
        
        RCLCPP_INFO(get_logger(), "on_home()");
    }
    void thermal_callback(const std_msgs::msg::Int32::SharedPtr msg) { 
        if (msg->data == 1) 
            thermal_done_ = true; 
        
        RCLCPP_INFO(get_logger(), "thermal_callback()");
    }

    // --- 상태 머신 (FSM) ---
    void tick() {
        // 매 tick 루프중 4번째마다 현재 좌표를 읽어서 MQTT 전송용 토픽으로 발행 해야징~
        static int publish_count = 0;


        if (++publish_count >= PUBLISH_INTERVAL_RATIO) {
            publish_current_pose();
            publish_count = 0;
        }

        switch (state_) {
            case State::IDLE:
                // 대기 상태, 홈 요청이 오면 시작
                // 1바퀴 돌면 10분간 휴식
                break;
            case State::NAV_HOME:
                if (home_requested_.exchange(false)) {
                    RCLCPP_INFO(get_logger(), "Sequence Start: Moving to Home(0,0)");
                    send_nav_goal(0.0, 0.0, 0.0);
                    state_ = State::NAV_FIRST_POS;
                    start_drive_straight(0.5, 0.1); // 0.5m 거리, 0.1m/s 속도;
                }
                break;

            case State::NAV_FIRST_POS:
                if (nav_done_) {
                    if (!nav_ok_) { fail_and_stop("First Pos Failed"); return; }
                    RCLCPP_INFO(get_logger(), "Arrived First Pos...");
                    send_nav_goal(0.5, 0.2, 0.0);
                    //state_ = State::DRIVE_STRAIGHT; // 이때는 바로 Wait lift로 가는게 맞는거같음
                    state_ = State::WAIT_LIFT;
                }
                break;
                
            case State::NAV_SECOND_POS:
                if (nav_done_) {
                    if (!nav_ok_) { fail_and_stop("Second Pos Failed"); return; }
                    RCLCPP_INFO(get_logger(), "Arrived Second Pos...");
                    send_nav_goal(0.5, 0.2, M_PI);
                    //state_ = State::DRIVE_STRAIGHT; // 이때는 바로 Wait lift로 가는게 맞는거같음
                    state_ = State::NAV_BACK_HOME;
                }
                break;

            case State::DRIVE_STRAIGHT:
            
                if (!lift_inflight_) {// 리프트 이동 중이 아닐 때만 실행
                    if (drive_step()) {
                        stop_cmd_vel();
                        RCLCPP_INFO(get_logger(), "Straight Drive Done. Starting Lift...");
                        state_ = State::WAIT_LIFT;
                    }
                }
                break;

            case State::START_LIFT:
                if (!lift_inflight_) {
                    lift_inflight_ = true;
                    // 예시: 2층으로 올라가기

                    RCLCPP_INFO(get_logger(), "Moving Lift: target %d", target_floor_bit_);
                    std::thread(&ControlNode::move_to_floor_blocking, this, target_floor_bit_ | GOUP).detach();
                    state_ = State::WAIT_LIFT;
                }
                break;
            case State::WAIT_LIFT:
            {
                // 리프트가 이동 중이 아닐 때만 실행 (도착했거나 이미 그 층인 경우)
                if (!lift_inflight_) {
                    RCLCPP_INFO(get_logger(), "Condition met. Starting Thermal Capture...");
                    
                    // 1. 현재 지점(Zone) 자동 계산 (posX 기반 또는 카운터 기반)
                    // 여기서는 안전하게 현재 멤버 변수 current_col_을 사용합니다.
                    
                    // 2. 현재 층(Row) 확인 및 번호 변환
                    current_row_bit = check_current_floor();
                    int floor_num = 1;
                    if (current_row_bit & TOFLOOR2) floor_num = 2;
                    else if (current_row_bit & TOFLOOR3) floor_num = 3;

                    // 3. 촬영 ID 계산 및 전송 (0~2 zone * 3 + 1~3 floor)
                    thermal_done_ = false; // 카메라 응답을 기다리기 위해 리셋
                    send_capture_cmd(current_col_ * 3 + floor_num);
                    
                    RCLCPP_INFO(get_logger(), "Request ID: %d (Zone %d, Floor %d)", 
                                current_col_ * 3 + floor_num, current_col_ + 1, floor_num);

                    state_ = State::WAIT_THERMAL;
                }
                break;
            }

            case State::WAIT_THERMAL:
            {
                if (thermal_done_) {
                    // 촬영이 완료되었으므로 다음 경로를 결정합니다.
                    if (target_floor_bit_ == TOFLOOR3) {
                        // [지점 내 모든 층 완료]
                        if (current_col_ >= 2) {
                            // 모든 구역 순찰 완료 -> 복귀 시퀀스
                            RCLCPP_INFO(get_logger(), "All Zones complete. Moving to Final Position...");
                            send_nav_goal(2.0, 1.2, M_PI); // 예시 좌표
                            state_ = State::NAV_SECOND_POS;
                        } else {
                            // 다음 지점으로 이동 준비
                            current_col_++;
                            target_floor_bit_ = TOFLOOR1; // 층 초기화
                            
                            // 리프트를 1층으로 내리면서 전진 시작
                            lift_inflight_ = true;
                            std::thread(&ControlNode::move_to_floor_blocking, this, TOFLOOR1 | GODOWN).detach();
                            start_drive_straight(0.5, 0.15); 
                            state_ = State::DRIVE_STRAIGHT;
                        }
                    } else {
                        // [다음 층으로 상승]
                        target_floor_bit_ <<= 1;
                        state_ = State::START_LIFT;
                    }
                    thermal_done_ = false; // 플래그 리셋
                }
                break;
            }
            case State::NAV_BACK_HOME:
                if (nav_done_) {
                    RCLCPP_INFO(get_logger(), "All Sequence Complete.");
                    send_nav_goal(0.0, 0.0, 0.0);
                    state_ = State::IDLE;
                }
                break;
        }
    }

    // 로봇의 현재 좌표 발행 함수
    void publish_current_pose() {
        double x, y;
        if (get_robot_xy(x, y)) {
            geometry_msgs::msg::Pose pose_msg;
            pose_msg.position.x = x;
            pose_msg.position.y = y;
            // 필요하다면 orientation(방향) 정보도 추가 가능
            pub_pose_mqtt_->publish(pose_msg);
        }
    }


    // --- 하드웨어(GPIO) 제어 ---
    void setup_gpio() {
        chip_ = gpiod_chip_open_by_name("gpiochip0");
        line_ena_ = gpiod_chip_get_line(chip_, 13);
        line_in1_ = gpiod_chip_get_line(chip_, 5);
        line_in2_ = gpiod_chip_get_line(chip_, 6);
        gpiod_line_request_output(line_ena_, "ena", 0);
        gpiod_line_request_output(line_in1_, "in1", 0);
        gpiod_line_request_output(line_in2_, "in2", 0);

        std::vector<int> pins = {17, 27, 22};
        for (int p : pins) {
            gpiod_line* l = gpiod_chip_get_line(chip_, p);
            gpiod_line_request_both_edges_events(l, "sensor");
            sensor_lines_.push_back(l);
            pollfd pfd{}; pfd.fd = gpiod_line_event_get_fd(l); pfd.events = POLLIN;
            pfds_.push_back(pfd);
        }
    }

    void move_to_floor_blocking(int cmd) {


        int floor_bit = check_current_floor();
        
        // if((cmd & GODOWN && cmd & TOFLOOR1 && TOFLOOR1 & floor_bit) || 
        // (cmd & GOUP && cmd & TOFLOOR3 && TOFLOOR3 & floor_bit))
        if((cmd & floor_bit))
        {
            lift_inflight_ = false;
            return;
        }

        apply_motor_state(cmd);
        int target_bit = cmd & (TOFLOOR1 | TOFLOOR2 | TOFLOOR3);

        while (rclcpp::ok()) {
            if (::poll(pfds_.data(), pfds_.size(), 100) > 0) {
                for (size_t i = 0; i < pfds_.size(); ++i) {
                    if (pfds_[i].revents & POLLIN) {
                        gpiod_line_event ev; gpiod_line_event_read(sensor_lines_[i], &ev);
                    }
                }
            }
            if (check_current_floor() & target_bit) break;
        }
        apply_motor_state(STOP);
        lift_inflight_ = false;
    }

    int check_current_floor() {
        for (size_t i = 0; i < sensor_lines_.size(); ++i) {
            if (gpiod_line_get_value(sensor_lines_[i]) == 0) return (1 << i);
        }
        return 0;
    }

    void apply_motor_state(int cmd) {
        int in1 = (cmd & GOUP) ? 1 : 0;
        int in2 = (cmd & GODOWN) ? 1 : 0;
        int ena = (in1 || in2) ? 1 : 0;



        gpiod_line_set_value(line_in1_, in1);
        gpiod_line_set_value(line_in2_, in2);
        gpiod_line_set_value(line_ena_, ena);
    }

    // --- 주행 제어 (Nav2 & TF) ---
    void send_nav_goal(double x, double y, double yaw) {
        if (!nav_client_->wait_for_action_server(1s)) {
            fail_and_stop("Nav2 Server Not Found");
            return;
        }
        auto goal = NavigateToPose::Goal();
        goal.pose.header.frame_id = "map";
        goal.pose.header.stamp = now();

        // [중요 수정] goal.pose(PoseStamped) -> pose(Pose) -> position 순서입니다.
        goal.pose.pose.position.x = x; 
        goal.pose.pose.position.y = y;
        goal.pose.pose.position.z = 0.0;

        tf2::Quaternion q; 
        q.setRPY(0, 0, yaw);
        goal.pose.pose.orientation = tf2::toMsg(q);

        nav_done_ = false;
        auto options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
        options.result_callback = [this](const GoalHandleNav::WrappedResult &res) {
            nav_ok_ = (res.code == rclcpp_action::ResultCode::SUCCEEDED);
            nav_done_ = true;
        };
        nav_client_->async_send_goal(goal, options);
    }

    void start_drive_straight(double dist, double v) {
        drive_target_m_ = dist; drive_v_ = v; drive_started_ = false;
    }

    bool drive_step() {
        double x, y;
        if (!get_robot_xy(x, y)) return false;
        if (!drive_started_) { drive_started_ = true; drive_sx_ = x; drive_sy_ = y; }
        double d = std::sqrt(std::pow(x - drive_sx_, 2) + std::pow(y - drive_sy_, 2));
        if (d >= drive_target_m_) return true;

        geometry_msgs::msg::Twist tw; tw.linear.x = drive_v_;
        pub_cmd_vel_->publish(tw);
        return false;
    }

    // map으로 위치 줌
    bool get_robot_xy(double &x, double &y) {
        try {
            auto t = tf_buffer_.lookupTransform("map", "base_link", tf2::TimePointZero);
            x = t.transform.translation.x; y = t.transform.translation.y;
            return true;
        } catch (...) { return false; }
    }

    // odom 으로 위치좌표를 가져오면
    // 장거리시 오류 누적 가능
    // map이 아닌 바퀴와 imu 센서만 믿고 계산한 위치
    // bool get_robot_xy(double &x, double &y) {
    //     try {
    //         // "map" 대신 "odom"을 사용하여 지도 보정으로 인한 위치 튐 현상 방지
    //         auto t = tf_buffer_.lookupTransform("odom", "base_link", tf2::TimePointZero);
    //         x = t.transform.translation.x; 
    //         y = t.transform.translation.y;
    //         return true;
    //     } catch (tf2::TransformException &ex) {
    //         return false; 
    //     }
    // }
    
    // --- 보조 함수 ---
    void stop_cmd_vel() { pub_cmd_vel_->publish(geometry_msgs::msg::Twist()); }
    void send_capture_cmd(int id) { std_msgs::msg::Int32 m; m.data = id; pub_capture->publish(m); }
    void fail_and_stop(std::string m) { RCLCPP_ERROR(get_logger(), "%s", m.c_str()); state_ = State::IDLE; }
    void cleanup_gpio() { apply_motor_state(STOP); for(auto* l : sensor_lines_) gpiod_line_release(l); gpiod_chip_close(chip_); }

    // 멤버 변수 정의
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_home_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_thermal;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_capture;
    rclcpp_action::Client<NavigateToPose>::SharedPtr nav_client_;
    rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr pub_pose_mqtt_;

    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;

    gpiod_chip* chip_;
    gpiod_line *line_ena_, *line_in1_, *line_in2_;
    std::vector<gpiod_line*> sensor_lines_;
    std::vector<pollfd> pfds_;

    std::atomic<bool> home_requested_{false}, nav_done_{false}, nav_ok_{false}, lift_inflight_{false}, thermal_done_{true};
    bool drive_started_{false};
    double drive_sx_, drive_sy_, drive_target_m_, drive_v_;

    uint8_t target_floor_bit_; // 목표 층 비트 플래그 1, 2, 4 |연산으로 GOUP, GODOWN 을하여 제어함
    int current_col_; // 현재 로봇의 열 위치 0, 1, 2 즉 몇번째 구역인지
    uint8_t current_row_bit; // 현재 로봇의 행 위치 1, 2, 4 즉 몇층인지몇번째 비트인지 
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ControlNode>());
    rclcpp::shutdown();
    return 0;
}