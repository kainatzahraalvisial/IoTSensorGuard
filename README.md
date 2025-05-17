# IoTSensorGuard

IoTSensorGuard is a C++ console application developed for an Information Security course to demonstrate secure data transfer in IoT systems. It simulates a sensor sending encrypted temperature and location data to a server, protecting against cyberattacks like data theft and tampering using XOR encryption, key rotation, and checksums.

## Project Overview

In IoT systems, sensors (e.g., temperature sensors) send sensitive data to servers, which attackers may target to steal or alter data. IoTSensorGuard addresses this by:
- **Encrypting** data with a simple XOR algorithm and an alphanumeric key.
- **Rotating keys** every 60 seconds to limit key exposure.
- **Verifying integrity** with a checksum to detect tampering.
- Providing a **user-friendly console interface** with a menu for encryption and decryption.

This project is educational, using basic security concepts suitable for a course, not production-grade encryption.

## Features
- **Encryption**: Scrambles sensor data (e.g., "Temp: 25.5C, Loc: NewYork") using XOR with a 4-character alphanumeric key.
- **Key Rotation**: Generates new keys every 60 seconds based on sensor ID, location, temperature, and time.
- **Integrity Check**: Uses a checksum to ensure data isn’t altered.
- **Menu-Driven**: Interactive console with options to encrypt, decrypt, or exit.
- **Security**: Hides sensitive data (keys, original message) from console output.

## Prerequisites
- **C++ Compiler**: g++ (MinGW-w64 on Windows, Xcode on macOS, g++ on Linux).
- **Visual Studio Code** (recommended) or any C++ IDE.
- No external libraries required.

## Setup Instructions
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/kainatzahraalvisial/IoTSensorGuard.git
   cd IoTSensorGuard