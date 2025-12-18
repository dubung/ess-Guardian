import paho.mqtt.client as mqtt
import json
import random
import time

BROKER = "10.10.14.109"
PORT = 1883


# ================================
#  MQTT Client Init
# ================================
client = mqtt.Client(protocol=mqtt.MQTTv311)

try:
    client.connect(BROKER, PORT)
except Exception as e:
    print("[ERROR] MQTT connection failed:", e)
    exit(1)


# ================================
#  Environment (temperature/humidity)
# ================================
def publish_environment():
    """
    온습도 센서 값 발행
    실제 센서 연동 시 여기만 교체하면 됨
    """
    data = {
        "temperature": round(random.uniform(18.0, 30.0), 2),
        "humidity": round(random.uniform(30.0, 70.0), 2)
    }

    client.publish("ess/env", json.dumps(data))
    print("[Publish] ess/env:", data)


# ================================
#  Alert Events (gas, thermal)
# ================================
GAS_WARNING = 300
GAS_CRITICAL = 500

THERMAL_WARNING = 60.0
THERMAL_CRITICAL = 80.0


def publish_alert(event_type: str, level: str, value: float):
    """
    공통 Alert 발행 함수
    event_type: "gas", "thermal"
    level: "warning" 또는 "critical"
    value: 센서 측정값
    """

    alert = {
        "event_type": event_type,
        "level": level,
        "value": value,
        "location": "ess_1",
        "message": f"{event_type.upper()} {level.upper()} - value={value}"
    }

    client.publish("ess/alert", json.dumps(alert))
    print(f"[Publish] ess/alert ({event_type}/{level}):", alert)


def check_and_publish_gas():
    gas_value = random.randint(100, 600)

    if GAS_WARNING <= gas_value < GAS_CRITICAL:
        publish_alert("gas", "warning", gas_value)

    elif gas_value >= GAS_CRITICAL:
        publish_alert("gas", "critical", gas_value)


def check_and_publish_thermal():
    thermal_value = round(random.uniform(40.0, 90.0), 2)

    if THERMAL_WARNING <= thermal_value < THERMAL_CRITICAL:
        publish_alert("thermal", "warning", thermal_value)

    elif thermal_value >= THERMAL_CRITICAL:
        publish_alert("thermal", "critical", thermal_value)


# ================================
#  Access Request (출입 인증 요청)
# ================================
def publish_access_request(admin_code: str, access_point: str):
    """
    출입 인증 요청 발행
    admin_code : 등록된 관리자 코드
    access_point : 출입 지점(ex: main_door, storage_room)
    """
    request = {
        "admin_code": admin_code,
        "access_point": access_point
    }

    client.publish("ess/access/request", json.dumps(request))
    print("[Publish] ess/access/request:", request)


# ================================
#  MAIN LOOP (테스트용)
# ================================
def main():
    print("Publisher started. Press Ctrl+C to stop.\n")

    try:
        while True:
            publish_environment()
            check_and_publish_gas()
            check_and_publish_thermal()

            # 수동 테스트 필요 시 직접 호출
            # publish_access_request("admin001", "main_door")

            time.sleep(5)

    except KeyboardInterrupt:
        print("\nPublisher stopped.")
        client.disconnect()


if __name__ == "__main__":
    main()

