from db import get_connection

def save_environment(data):
    db = get_connection()
    cursor = db.cursor()
    try:
        cursor.execute("""
            INSERT INTO environment_data (temperature, humidity)
            VALUES (%s, %s)
        """, (data["temperature"], data["humidity"]))
        db.commit()
        print("[DB] Environment data saved.")
    except Exception as e:
        db.rollback()
        print("[DB ERROR] env save failed:", e)
    finally:
        cursor.close()
        db.close()

def save_alert(data):
    db = get_connection()
    cursor = db.cursor()
    try:
        cursor.execute("""
            INSERT INTO alert_events (event_type, level, value, location, message)
            VALUES (%s, %s, %s, %s, %s)
        """, (
            data["event_type"],
            data["level"],
            data["value"],
            data["location"],
            data["message"]
        ))
        db.commit()
        print("[DB] Alert event saved.")
    except Exception as e:
        db.rollback()
        print("[DB ERROR] alert save failed:", e)
    finally:
        cursor.close()
        db.close()

# ================================
# Admin / Access DB Functions
# ================================
def get_admin_by_code(admin_code: str):
    db = get_connection()
    cursor = db.cursor()
    try:
        cursor.execute("""
            SELECT id FROM admins WHERE admin_code = %s
        """, (admin_code,))
        row = cursor.fetchone()
        return row[0] if row else None
    finally:
        cursor.close()
        db.close()

def log_access_result(admin_id: int, access_point: str, result: str):
    db = get_connection()
    cursor = db.cursor()
    try:
        cursor.execute("""
            INSERT INTO access_logs (admin_id, access_point, result)
            VALUES (%s, %s, %s)
        """, (admin_id, access_point, result))
        db.commit()
    except Exception as e:
        db.rollback()
        print("[DB ERROR] access log save failed:", e)
    finally:
        cursor.close()
        db.close()

