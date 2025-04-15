# 🚗 Rental Vehicle System

<!-- CI status badge -->
![CI](https://github.com/minh-9999/Rental-Vehicle/actions/workflows/cpp-multi-platform.yml/badge.svg)

<!-- Code coverage badge -->
[![codecov](https://codecov.io/gh/minh-9999/Rental-Vehicle/branch/main/graph/badge.svg)](https://codecov.io/gh/minh-9999/Rental-Vehicle)

<!-- Latest release badge -->
![GitHub release (latest by date)](https://img.shields.io/github/v/release/minh-9999/Rental-Vehicle)

<!-- License badge -->
![GitHub](https://img.shields.io/github/license/minh-9999/Rental-Vehicle)

<!-- Platform support -->
![Platform](https://img.shields.io/badge/platform-linux--windows--macos-blue)

<!-- CMake badge -->
![CMake](https://img.shields.io/badge/build%20system-CMake-informational)

<!-- C++ version -->
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)

<!-- Lines of code (optional but cool) -->
![LOC](https://tokei.rs/b1/github/minh-9999/Rental-Vehicle)


A cross-platform C++ project for managing vehicle rentals, designed with clean architecture, JSON data persistence, unit/integration testing, and CI/CD using GitHub Actions.

---

## ✨ Features

- Manage vehicle data: license plate, manufacturer, year, type, rental price
- Rent/return vehicle and save records in JSON format
- Real-time rental status tracking
- Unit & Integration testing with GoogleTest
- GitHub Actions CI for Ubuntu, Windows, and macOS

---

## ⚙️ Requirements

- CMake ≥ 3.20
- Ninja or Make
- C++20 compatible compiler

---

## 🚀 Build Instructions

### Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```


## ✅ Run Tests

cd script  
./run_tests.sh

Logs will be saved in build/logs/test.log.


## 🗂 Project Structure

.
├── src/                  		# Core source code  
├── template-test/        		# YAML files for test config  
├── test/                 		# Unit & Integration tests  
├── script/               		# Test & build scripts  
├── third_party/          		# External libraries (e.g., fmt, gtest)  
├── build/                		# CMake build output  
├── resource/                	# Assets (icons, etc.)  
└── CMakeLists.txt  


## 📫 Contact

📧 ngodinhhai.hcm@gmail.com  
🌐 https://github.com/minh-9999
