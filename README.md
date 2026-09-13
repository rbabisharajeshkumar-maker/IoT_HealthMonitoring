# IoT Health Monitor (WiFi-Connected)

An ESP32-based health monitoring device that tracks body temperature and heart rate, displaying readings locally on an LCD and sending them to a remote server over WiFi for logging.

## Components Used
- ESP32 development board
- DS18B20 Temperature Sensor (OneWire)
- Heart Rate Pulse Sensor (digital)
- 16x2 LCD (parallel interface)
- WiFi connectivity

## How It Works
- On startup, the ESP32 connects to WiFi.
- A DS18B20 sensor reads temperature over the OneWire protocol.
- A pulse sensor detects heartbeats via digital input (rising-edge detection); beats are converted to BPM every 5 seconds.
- Readings are displayed locally on a 16x2 LCD.
- Every 5 seconds, the data (temperature + BPM) is sent to a remote server via an HTTP GET request for logging/monitoring.

## Protocols/Concepts Used
- OneWire communication (DS18B20 temperature sensor)
- WiFi connectivity and HTTP client requests (ESP32)
- Digital pulse detection (rising-edge counting) and BPM calculation
- Parallel LCD interfacing (LiquidCrystal library)
- Timing-based updates using `millis()`

## Code
See `Temp_and_HR.ino` for the full sketch.

