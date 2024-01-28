###  SELF-BALANCING BOT PROJECT ###

Overvie:

Welcome to the Self-Balancing Bot project! This repository contains all the code and information you need to build your own self-balancing robot using two NEMA 17 stepper motors, an Arduino Uno, CNC shield, DRV8255 stepper motor drivers, MPU6050 accelerometer and gyroscope, and a Bluetooth module for convenient control.

Features:
Self-Balancing: The bot utilizes sensor data from the MPU6050 to maintain balance and adjust its position dynamically.
User-Friendly Control: Control the bot wirelessly using the Bluetooth module. Simply connect your smartphone or other Bluetooth-enabled device to the bot and enjoy seamless control.
Customizable: The Arduino Uno and CNC shield allow for easy customization and expansion of the project. Experiment with different sensors, motor configurations, or control algorithms to enhance the bot's capabilities.
Hardware Requirements:

•	Arduino Uno
•	CNC Shield
•	DRV8255 Stepper Motor Drivers
•	NEMA 17 Stepper Motors (x2)
•	MPU6050 Accelerometer and Gyroscope
•	Bluetooth Module
•	Software Requirements
•	Arduino IDE
•	MPU6050 Library
•	Bluetooth Library


Getting Started:
Connections: Ensure all hardware components are correctly connected. Refer to the wiring diagrams provided in the "docs" folder for assistance.

Arduino Code: Upload the provided Arduino sketch (self_balancing_bot.ino) to your Arduino Uno using the Arduino IDE.

Bluetooth Setup: Pair your Bluetooth-enabled device with the bot's Bluetooth module. The default Bluetooth credentials are usually provided in the documentation of the module.

Power Up: Power up the bot and wait for it to initialize. The MPU6050 will calibrate, and the bot will enter the balancing mode. Use 12 volt lipo or li-ion power supply.

Control: Use your Bluetooth-enabled device to wirelessly control the bot. Experiment with different commands and observe how the bot adjusts its balance in response.

License
This project is licensed under the MIT License, making it open and accessible for everyone.

Acknowledgments
Special thanks to the open-source community for their contributions and support in making this project possible.

Happy balancing!
