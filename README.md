# Real-Time Industrial IoT Edge Monitoring & Fault Detection System

## 📌 Project Overview

The **Real-Time Industrial IoT Edge Monitoring & Fault Detection System** is a C-based Industrial IoT application designed to monitor industrial machines and detect abnormal operating conditions in real time.

The system continuously generates sensor readings for:

- 🌡️ Temperature
- 📳 Vibration
- 🔧 Pressure

The readings are processed using a fault-detection mechanism and classified into:

- NORMAL
- WARNING
- CRITICAL

Sensor data is stored in an SQLite database, abnormal events are recorded in log files, and current machine information can be transmitted through TCP/IP.

A Flask-based web dashboard is also included for monitoring stored sensor data.

---

## 🎯 Objectives

The main objectives of this project are:

1. Monitor multiple industrial machines in real time.
2. Simulate industrial sensor readings.
3. Detect abnormal machine conditions.
4. Generate warning and critical alerts.
5. Store sensor readings in SQLite.
6. Maintain event logs.
7. Implement multithreaded monitoring.
8. Implement TCP/IP communication.
9. Demonstrate MQTT-based IoT communication.
10. Provide a web-based monitoring dashboard.

---

## 🏭 Machines Monitored

The system currently simulates three industrial machines:

| Machine | Type |
|---|---|
| Motor-A | Industrial Motor |
| Pump-B | Industrial Pump |
| Compressor-C | Industrial Compressor |

---

## ⚙️ System Architecture

```text
                INDUSTRIAL MACHINES
                       │
        ┌──────────────┼──────────────┐
        ↓              ↓              ↓
   Temperature     Vibration       Pressure
      Sensor          Sensor         Sensor
        │              │              │
        └──────────────┼──────────────┘
                       ↓
                  C PROGRAM
                       ↓
              REAL-TIME PROCESSING
                       ↓
                 FAULT DETECTION
                       │
          ┌────────────┼────────────┐
          ↓            ↓            ↓
       NORMAL       WARNING      CRITICAL
          │            │            │
          └────────────┼────────────┘
                       ↓
                DATA LOGGING
                       │
              ┌────────┴────────┐
              ↓                 ↓
          SQLite DB         Event Log
              │
              ↓
        Flask Dashboard