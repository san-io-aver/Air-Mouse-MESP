## Air Mouse Using ESP-32
The mouse consists of MPU6050 (accelerometer + gyroscope) and an ESP32.
We make use of MPU6050_light and BleMouse library which provides pitch, yaw, roll 
angles using a complementary filter and mapsit to the number of pixels to be moved.

BleMouse library is old and some minor changes need to be made.
Replace BleMouse.cpp and Connection status with the given code
