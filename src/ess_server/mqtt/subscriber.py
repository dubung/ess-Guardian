import paho.mqtt.client as mqtt
import json
import time
from db_service import save_environment, save_alert, get_admin_by_code, log_access_result

BROKER = "10.10.14.109"
PORT = 1883
RECONNECT_DELAY = 5  # 초 단위

# ================================
# MQTT Connect / Subscribe
# ================================
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(f"[MQTT] Connected successfully")
        client.subscribe("ess/env")
        client.subscribe("ess/alert")
        client.subscribe("ess/access/request")
        print("[MQTT] Subscribed: ess/env, ess/alert, ess/access/request")
    else:
        print(f"[MQTT ERROR] Connection failed with code {rc}")

# ================================
# MQTT Message Handler
# ================================
def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode("utf-8")

    try:
        data = json.loads(payload)
    except json.JSONDecodeError:
        print(f"[ERROR] Invalid JSON: {payload}")
        return

    print(f"[MQTT] Received ({topic}): {data}")

    if topic == "ess/env":
        handle_environment(data)
    elif topic == "ess/alert":
        handle_alert(data)
    elif topic == "ess/access/request":
        handle_access_request(client, data)

# ================================
# Environment / Alert Handlers
# ================================
def handle_environment(data):
    save_environment(data)

def handle_alert(data):
    save_alert(data)

# ================================
# Access Request Handler
# ================================
def verify_access_request(data):
    admin_code = data.get("admin_id")
    access_point = data.get("access_point")
    return {
        "admin_code": admin_code,
        "access_point": access_point,
        "verified": False,
        "reason": "Auth method not defined yet"
    }

def handle_access_request(client, data):
    result = verify_access_request(data)
    log_access_result(
        admin_id=None,  # 아직 인증 방식 미정
        access_point=result["access_point"],
        result="pending"
    )
    response = {
        "admin_id": result["admin_code"],
        "access_point": result["access_point"],
        "result": "pending",
        "message": result["reason"]
    }
    client.publish("ess/access/response", json.dumps(response))
    print("[ACCESS] response:", response)

# ================================
# Subscriber 실행
# ================================
def run_subscriber():
    while True:
        try:
            client = mqtt.Client(protocol=mqtt.MQTTv311)
            client.on_connect = on_connect
            client.on_message = on_message
            client.connect(BROKER, PORT)
            print("[MQTT] Subscriber started. Waiting for messages...")
            client.loop_forever()
        except Exception as e:
            print(f"[MQTT ERROR] Connection lost: {e}")
            print(f"[MQTT] Reconnecting in {RECONNECT_DELAY} seconds...")
            time.sleep(RECONNECT_DELAY)

# ================================
# Entry Point
# ================================
if __name__ == "__main__":
    run_subscriber()

