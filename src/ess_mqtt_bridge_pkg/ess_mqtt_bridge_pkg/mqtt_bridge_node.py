import json
import time
import threading

import rclpy
from rclpy.node import Node

from paho.mqtt import client as mqtt

# 예: 너가 만든 인터페이스를 쓴다고 가정
from ess_interfaces.msg import ThermalEvent  # 예시
from std_msgs.msg import Float32            # center를 std_msgs로 쓰는 경우 예시


class MqttBridgeNode(Node):
    def __init__(self):
        super().__init__('ess_mqtt_bridge')

        # --- ROS 파라미터로 MQTT 설정 받기 ---
        self.declare_parameter('broker_host', '10.10.14.109')
        self.declare_parameter('broker_port', 1883)
        self.declare_parameter('base_topic', 'ess')
        self.declare_parameter('qos', 0)
        self.declare_parameter('retain', False)
        self.declare_parameter('client_id', 'ess-bridge')
        self.declare_parameter('username', '')
        self.declare_parameter('password', '')

        self.host = self.get_parameter('broker_host').value
        self.port = int(self.get_parameter('broker_port').value)
        self.base = self.get_parameter('base_topic').value.rstrip('/')
        self.qos = int(self.get_parameter('qos').value)
        self.retain = bool(self.get_parameter('retain').value)
        self.client_id = self.get_parameter('client_id').value
        self.username = self.get_parameter('username').value
        self.password = self.get_parameter('password').value

        # --- MQTT 클라이언트 ---
        self.mqtt = mqtt.Client(client_id=self.client_id, clean_session=True)
        if self.username:
            self.mqtt.username_pw_set(self.username, self.password)

        self.mqtt.on_connect = self._on_connect
        self.mqtt.on_disconnect = self._on_disconnect

        # 네트워크 loop는 별도 스레드로
        self._mqtt_thread = threading.Thread(target=self._mqtt_loop, daemon=True)
        self._mqtt_thread.start()

        # --- ROS 구독 설정 ---
        # center를 따로 토픽으로 쓰는 경우
        self.sub_center = self.create_subscription(
            Float32, '/thermal/center', self.on_center, 10
        )

        # 이벤트 메시지(이상/최대/최소 등)
        self.sub_event = self.create_subscription(
            ThermalEvent, '/thermal/event', self.on_event, 10
        )

        self.get_logger().info('MQTT bridge node started.')

    def _mqtt_loop(self):
        # 재연결 루프
        while rclpy.ok():
            try:
                self.mqtt.connect(self.host, self.port, keepalive=30)
                self.mqtt.loop_forever(retry_first_connection=True)
            except Exception as e:
                # 연결 실패하면 잠깐 쉬고 재시도
                time.sleep(1.0)

    def _on_connect(self, client, userdata, flags, rc):
        self.get_logger().info(f'MQTT connected rc={rc}')

    def _on_disconnect(self, client, userdata, rc):
        self.get_logger().warn(f'MQTT disconnected rc={rc}')

    def _pub_json(self, topic_suffix: str, payload: dict):
        topic = f'{self.base}/{topic_suffix}'.replace('//', '/')
        data = json.dumps(payload, ensure_ascii=False)
        self.mqtt.publish(topic, data, qos=self.qos, retain=self.retain)

    def on_center(self, msg: Float32):
        self._pub_json('thermal/center', {
            'center': float(msg.data),
            'ts': time.time(),
        })

    def on_event(self, msg: ThermalEvent):
        self._pub_json('thermal/event', {
            'abnormal': bool(msg.abnormal),
            'center': float(msg.center),
            'min': float(msg.min),
            'max': float(msg.max),
            'hot_x': int(msg.hot_x),
            'hot_y': int(msg.hot_y),
            'stamp': {
                'sec': int(msg.stamp.sec),
                'nanosec': int(msg.stamp.nanosec),
            }
        })


def main():
    rclpy.init()
    node = MqttBridgeNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

