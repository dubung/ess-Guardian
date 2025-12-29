#include <memory>
#include <string>
#include <stdexcept>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

#include <gpiod.h>
#include <poll.h>

#include <atomic>
#include <vector>
#include <thread>


#define  STOP 0

// lifter만 하는 flag
#define  TOFLOOR1 1 << 0
#define  TOFLOOR2 1 << 1
#define  TOFLOOR3 1 << 2
#define  GOUP     1 << 3
#define  GODOWN   1 << 4



class ControlNode : public rclcpp::Node {
public:
  ControlNode()
  : Node("control_node"),
    chip_name_("gpiochip0"),
    lines_{17, 27, 22},
    ena_line_num_(13),
    in1_line_num_(5),
    in2_line_num_(6),
    last_cmd_(999),
    flag(0),
    running_(true)
  {
     // subscription은 대기 열화상 노드 구동해야함
     sub_thermal = create_subscription<std_msgs::msg::Int32>(
      "/ess/thermal/ack", 10,
      std::bind(&ControlNode::thermal_callback, this, std::placeholders::_1)
    );

    // 열화상 카메라에거 할 것임
    pub_capture = create_publisher<std_msgs::msg::Int32>("/ess/request/id", 10);

    // FSM 을 쓰기위해서 타이머 설정
    timer_ = create_wall_timer(50ms, std::bind(&ControlNode::tick, this));


    // GPIO chip open
    chip_ = gpiod_chip_open_by_name(chip_name_.c_str());
    if (!chip_) throw std::runtime_error("Failed to open gpio chip: " + chip_name_);

    // lines get
    line_ena_ = gpiod_chip_get_line(chip_, ena_line_num_);
    line_in1_ = gpiod_chip_get_line(chip_, in1_line_num_);
    line_in2_ = gpiod_chip_get_line(chip_, in2_line_num_);
    if (!line_ena_ || !line_in1_ || !line_in2_) {
      cleanup();
      throw std::runtime_error("Failed to get one or more gpio lines");
    }

    // request output lines, default 0
    if (gpiod_line_request_output(line_ena_, "lift_motor_ENA", 0) < 0 ||
        gpiod_line_request_output(line_in1_, "lift_motor_IN1", 0) < 0 ||
        gpiod_line_request_output(line_in2_, "lift_motor_IN2", 0) < 0) {
      cleanup();
      throw std::runtime_error("Failed to request gpio output lines");
    }

    // 포토센서들 gpio 세팅
    for (int ln : lines_) {
      gpiod_line* line = gpiod_chip_get_line(chip_, ln);
      if (!line) throw std::runtime_error("Failed to get line");

      // both edges events (v1 API)
      if (gpiod_line_request_both_edges_events(line, "photo_sensor_3level") < 0) {
        throw std::runtime_error("Failed to request edge events");
      }


      lines_handle_.push_back(line);

      pollfd pfd{};
      pfd.fd = gpiod_line_event_get_fd(line);
      pfd.events = POLLIN;
      pfds_.push_back(pfd);
    }
    
    int initCmd = GODOWN | TOFLOOR1 ; 
    worker_lift_control = std::thread([this, initCmd]() {
      this->move_to_floor_blocking(initCmd);
    });

    RCLCPP_INFO(get_logger(), "Photo Sensor Thread Done (GPIO %d,%d,%d)",
                lines_[0], lines_[1], lines_[2]);

    // 안전 정지 정지가 아니라 무조건 밑으로 내려야지 수정해야함
    apply_motor_state(0);

    RCLCPP_INFO(get_logger(), "ControlNode started (ENA=%d, IN1=%d, IN2=%d)",
                ena_line_num_, in1_line_num_, in2_line_num_);
  }

  ~ControlNode() override {
    // 종료시 포토센서 GPIO 관련 해제
    running_.store(false);

    for (auto* line : lines_handle_) {
      if (line) gpiod_line_release(line);
    }
    if (chip_) gpiod_chip_close(chip_);
    // 종료 시 정지 + 해제
    try { apply_motor_state(0); } catch (...) {}
    cleanup();
  }

private:


  // WAITING : 초기상태
  // FLOWNAV : 초기 위치로 (로봇의 home)
  // POSALIGN : 현재위치 보정
  // 
  enum class State {WAITING, FLOWNAV, POSALIGN, START_LIFT, WAIT_LIFT, WAIT_THERMAL };
  State state_{State::WAITING};

  void tick() {
  // 상태머신 한 스텝

    int level; 
    int cmd_;
    
     switch (state) {
        case WAITING:
        // 목표 설정
        
        state = State::START_LIFT;
        break;
        case State::FLOWNAV:
        // 초기위치로 NAV를 통해서 혹은 wayPoint로 갈거임

        break;
        case State::POSALIGN:
        // ArUco 마크야 힘내~~
        if (alignment_done_) { // 완료 신호를 받아야만 움직일것임 
                RCLCPP_INFO(get_logger(), "Alignment Complete. Starting Lift...");
                state_ = State::START_LIFT;
        }
        break;
        case State::START_LIFT:
        if (!lift_inflight) {
            lift_inflight = true;
            lift_done_ok = false;

            level = check_current_floor();

            if(level & TOFLOOR3)
            {
              cmd_ = TOFLOOR1 | GODOWN;
            }
            else{
              cmd_ = (level << 1) | GOUP;
            }

            lift_thread = std::thread([this]{
            bool ok = move_to_floor_blocking(cmd_);
            lift_done_ok = ok;
            lift_inflight = false;
            });

        }
        state = State::WAIT_LIFT;
        break;

        case State::WAIT_LIFT:
        if (!lift_inflight) {
          level = check_current_floor();
          switch(level)
          {
            case TOFLOOR1:
              level = 1;
            break;
            case TOFLOOR2:
              level = 2;
            break;
            case TOFLOOR3:
              level = 3;
            break;
          }

          // 여기서 현재 위치 기준으로 몇번째 열에 있는지 확인하고 열 * 3 + level을 해야 번호가 나옴
          send_capture_cmd(+level);
          thermal_done = false;
          state = State::WAIT_THERMAL; //  capture 끝날때까지 잠시 대기
        }
        break;

        case State::WAIT_THERMAL:
          if(thermal_done)
          {
              state = State::START_LIFT;
          }

        break;
        case State::NEXT:
        // 여기서 캡처 요청/검사/MQTT 등
        break;
  }

  }

  void send_capture_cmd(int value)
  {
    std_msgs::msg::Int32 msg;
    msg.data = value;
    pub_capture_->publish(msg);
  }


  void thermal_callback(const std_msgs::msg::Int32::SharedPtr msg){

    int thermal = msg->data;
    if(thermal == 1){
      thermal_done = true;
    }
    RCLCPP_INFO(get_logger(), "Capture Done : %d", thermal);

  }


  bool move_to_floor_blocking(int cmd) {
    apply_motor_state(cmd); // 모터 켬

    while (true) {
        int ret = ::poll(pfds_.data(), pfds_.size(), 10);  // 이벤트 기다림
        if (ret < 0) return false;                          // 에러
        if (ret == 0) continue;                             // 타임아웃 → 계속

        // 이벤트가 온 라인들 이벤트를 읽어서 비움
        for (size_t i = 0; i < pfds_.size(); ++i) {
        if (pfds_[i].revents & POLLIN) {
            gpiod_line_event ev;
            gpiod_line_event_read(lines_handle_[i], &ev);
        }
        }

        int level = check_current_floor();            // check_current_floor 현재층 계산
        if (level & target_level) {
            apply_motor_state(STOP);                          // 모터 끔
            return true;
        }
    }
    return false;
  }


  // 1층 : 1 << 0 2층 : 1 << 1 3층 : 1 << 2
  int check_current_floor() {
    std::vector<int> values;
    values.reserve(lines_handle_.size());

    for (auto* line : lines_handle_) {
      int v = gpiod_line_get_value(line);
      if (v < 0) v = 0;
      values.push_back(v);
    }

    // active-high: 1이면 감지
    std::vector<int> pressed_idx;
    for (size_t i = 0; i < values.size(); ++i) {
      if (values[i] == 0) pressed_idx.push_back((int)i);
    }

    int level = 0;
    if (pressed_idx.size() == 1) level = 1 << pressed_idx[0];
    else if (pressed_idx.empty()) level = 0;
    else level = -1;

    if ( level != last_level_) {
      last_level_ = level;
      RCLCPP_INFO(get_logger(), "감지 상태 변경: %d (raw=%d,%d,%d)",
                  level, values[0], values[1], values[2])지
    }

    return level;
  }


  void apply_motor_state(int cmd) {
    int in1_val = 0, in2_val = 0, ena_val = 0;

    if (cmd & GOUP) {
      // 정방향
      in1_val = 1; in2_val = 0; ena_val = 1;
    } else if (cmd & GODOWN) {
      // 역방향
      in1_val = 0; in2_val = 1; ena_val = 1;
    } else {
      in1_val = 0; in2_val = 0; ena_val = 0;
    }

    if (gpiod_line_set_value(line_in1_, in1_val) < 0 ||
        gpiod_line_set_value(line_in2_, in2_val) < 0 ||
        gpiod_line_set_value(line_ena_, ena_val) < 0) {
      RCLCPP_WARN(get_logger(), "GPIO set_value 실패");
    }
  }

  void cleanup() {
    // 라인 release
    if (line_ena_) { gpiod_line_set_value(line_ena_, 0); gpiod_line_release(line_ena_); line_ena_ = nullptr; }
    if (line_in1_) { gpiod_line_set_value(line_in1_, 0); gpiod_line_release(line_in1_); line_in1_ = nullptr; }
    if (line_in2_) { gpiod_line_set_value(line_in2_, 0); gpiod_line_release(line_in2_); line_in2_ = nullptr; }

    // chip close
    if (chip_) { gpiod_chip_close(chip_); chip_ = nullptr; }
  }

private:
  std::string chip_name_;
  int ena_line_num_, in1_line_num_, in2_line_num_;

  gpiod_chip* chip_{nullptr};
  gpiod_line* line_ena_{nullptr};
  gpiod_line* line_in1_{nullptr};
  gpiod_line* line_in2_{nullptr};


  // 구독
  rclcpp::subscription<std_msgs::msg::Int32>::SharedPtr sub_thermal;
  std::atomic<bool> listen_enabled_{false};


  // 발행
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_capture;

  // lift 제어용 쓰레드 및 돌다리 두들기고 건ㄱ너기
  std::thread worker_lift_control;
  std::atomic<bool> lift_inflight{false};
  std::atomic<bool> lift_done_ok{false};

  // thermal cam done callback시 상태 전이 하기위한 atomic
  std::atomic<bool> thermal_done{false}; 


  int last_cmd_;
  
  std::atomic<bool> running_;
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ControlNode>());
  rclcpp::shutdown();
  return 0;
}
