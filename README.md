
# 🤖 SELF-BALANCING ROBOT WITH SLAM

A fully autonomous, self-balancing robot built using Raspberry Pi and ESP32, equipped with SLAM (Simultaneous Localization and Mapping), IoT connectivity, and robust obstacle avoidance. This robot is capable of real-time balance, environment mapping, and remote telemetry, making it ideal for applications like smart delivery and surveillance.

![Project Demo](https://youtube.com/shorts/E-i_8Q7rbdQ)

## 🔧 Features

- 🔁 **Self-Balancing** using cascaded PID control and adaptive gain tuning.
- 🌐 **2D LiDAR-based SLAM** using GMapping for real-time map generation.
- 👁️ **Monocular Visual SLAM (ORB-SLAM2)** for enhanced localization in textured environments.
- 🧠 **Sensor Fusion** with Extended Kalman Filter (EKF) for precise pose estimation.
- 📡 **IoT Dashboard** via ESP32 and MQTT for remote monitoring and control.
- 🚧 **Obstacle Avoidance** with 98% success rate in dynamic indoor and semi-structured settings.
- 🔌 Modular hardware for future enhancements (gesture control, 3D SLAM, etc.)

---

## 📦 Hardware Used

| Component            | Purpose                                       |
|----------------------|-----------------------------------------------|
| Raspberry Pi 4       | Central processor for SLAM & control logic   |
| MPU6050              | IMU for tilt and acceleration data           |
| Stepper Motors (NEMA 17) | Motion control                          |
| DRV8255 Motor Drivers| High-precision stepper control               |
| RPLIDAR A1M8         | 2D LiDAR sensor for environment mapping      |
| ESP32                | MQTT-based telemetry and dashboard           |
| Monocular USB Camera | Visual SLAM and obstacle detection           |
| Bluetooth Module     | (Optional) manual remote control             |
| CNC Shield + Arduino Uno | (Legacy) modular testing platform       |
| 12V Li-ion Battery   | Portable power supply                        |

---

## 🧠 Software Stack

| Layer              | Tools/Technologies         |
|--------------------|----------------------------|
| OS & Middleware    | Raspbian OS, ROS Noetic    |
| SLAM               | GMapping (2D), ORB-SLAM2 (VSLAM) |
| Control            | Custom C++ ROS Nodes (PID) |
| Sensor Fusion      | Extended Kalman Filter (EKF) |
| IoT Communication  | MQTT via ESP32             |
| Frontend (Dashboard)| Web-based MQTT monitor    |

---

## 🛠️ Setup Instructions

### 1. Clone the Repository
```bash
git clone https://github.com/Sanyam753/SELF-BALANCING-BOT.git
cd SELF-BALANCING-BOT
````

### 2. Hardware Assembly

Refer to the `Hardware_Schematics/` folder for wiring diagrams and chassis design.

### 3. Install ROS Dependencies

```bash
sudo apt update
sudo apt install ros-noetic-gmapping ros-noetic-robot-state-publisher ros-noetic-tf2-ros
```

### 4. Run SLAM Nodes

Start `roscore`, LiDAR, and camera nodes:

```bash
roscore
rosrun rplidar_ros rplidarNode
rosrun usb_cam usb_cam_node
roslaunch slam_gmapping slam.launch
```

### 5. Run PID Control

Launch the self-balancing PID node:

```bash
rosrun balance_control pid_controller
```

### 6. IoT MQTT Setup

Upload `esp32_mqtt.ino` to your ESP32. Configure WiFi credentials and MQTT broker IP.

---

## 📈 Results

| Metric                     | Value                 |
| -------------------------- | --------------------- |
| Tilt Stability             | ±2° under disturbance |
| LiDAR SLAM Accuracy (RMSE) | 4.7 cm                |
| VSLAM Drift (10m)          | 6.2 cm                |
| Obstacle Avoidance Rate    | 98% (50 trials)       |
| MQTT Latency               | \~120 ms              |

---

## 🎯 Future Enhancements

* 👋 Gesture-based control using computer vision
* 🧠 Reinforcement learning for adaptive balancing
* 🌍 3D SLAM using depth or stereo cameras
* 🤖 Swarm coordination and distributed mapping

---

## 📹 Demo & Links

* 📽️ **Video Demo**: [YouTube Short](https://youtube.com/shorts/E-i_8Q7rbdQ)


---

## 📜 License

This project is open-source and available under the MIT License.

---

