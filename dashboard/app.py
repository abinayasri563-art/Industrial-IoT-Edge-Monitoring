from flask import Flask, render_template
import sqlite3

app = Flask(__name__)

DATABASE = "../industrial_iot.db"


def get_sensor_data():
    connection = sqlite3.connect(DATABASE)

    connection.row_factory = sqlite3.Row

    cursor = connection.cursor()

    cursor.execute("""
        SELECT *
        FROM sensor_data
        ORDER BY id DESC
        LIMIT 20
    """)

    data = cursor.fetchall()

    connection.close()

    return data


@app.route("/")
def dashboard():

    sensor_data = get_sensor_data()

    return render_template(
        "dashboard.html",
        sensor_data=sensor_data
    )


if __name__ == "__main__":

    print("============================================")
    print("     INDUSTRIAL IoT MONITORING DASHBOARD")
    print("============================================")
    print("Starting Flask dashboard...")
    print("Open: http://127.0.0.1:5000")

    app.run(
        host="127.0.0.1",
        port=5000,
        debug=True
    )