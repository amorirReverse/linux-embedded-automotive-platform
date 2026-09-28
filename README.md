# Linux Embedded Automotive Platform

A C++17 Linux embedded automotive platform simulating multiple electronic control units (ECUs) communicating over a CAN bus.

The project focuses on embedded software architecture, Linux system programming, CAN communication, testing, logging, and modular ECU simulations.

---

## 🚧 Project Status

**Current milestone:** V0.3 — Multi-ECU Platform

The project is under active development.

### V0.1 — Initial ECU Simulation

* [x] Project structure
* [x] Engine ECU
* [x] Engine simulation
* [x] Periodic simulation loop
* [x] Engine state management

### V0.2 — CAN Communication

* [x] SocketCAN integration
* [x] Virtual CAN interface (`vcan0`)
* [x] CAN socket abstraction
* [x] CAN frame transmission
* [x] CAN frame reception
* [x] Engine ECU CAN integration
* [x] Battery ECU CAN integration
* [x] CAN message encoding and decoding
* [x] Gateway application
* [x] Multi-ECU CAN communication

### V0.3 — Testing and Reliability

* [x] Battery ECU model
* [x] Shared logging component
* [x] Engine CAN message unit tests
* [x] Battery CAN message unit tests
* [x] Engine model unit tests
* [x] CTest integration
* [x] Platform startup script
* [ ] Improved error handling
* [ ] Multithreaded ECU execution
* [ ] Configuration management
* [ ] Watchdog supervision

### Future Work

* [ ] systemd service integration
* [ ] Continuous integration
* [ ] Doxygen documentation generation
* [ ] Fault and error simulation
* [ ] Yocto-based embedded Linux image
* [ ] Embedded Linux deployment

---

## 🏗️ Architecture

The platform is designed around independent ECU components communicating through a Linux CAN interface.

```text
                         ┌──────────────────┐
                         │    Engine ECU    │
                         │                  │
                         │   Engine Model   │
                         └────────┬─────────┘
                                  │
                                  │ CAN
                                  │
                    ┌─────────────▼─────────────┐
                    │           vcan0           │
                    │         SocketCAN         │
                    └─────────────┬─────────────┘
                                  │
                         ┌────────▼─────────┐
                         │   Battery ECU    │
                         │                  │
                         │  Battery Model   │
                         └────────┬─────────┘
                                  │
                                  │ CAN
                                  │
                         ┌────────▼─────────┐
                         │     Gateway      │
                         │                  │
                         │ CAN message      │
                         │ decoding         │
                         └──────────────────┘
```

The architecture is progressively evolving toward a more complete embedded automotive platform.

---

## 📁 Project Structure

```text
linux-embedded-automotive-platform/

├── apps/
│   └── gateway/
│       ├── Gateway.*
│       ├── main.cpp
│       └── CMakeLists.txt
│
├── can/
│   ├── CanSocket.*
│   ├── EngineCanMessage.*
│   ├── BatteryCanMessage.*
│   ├── test_can.cpp
│   └── CMakeLists.txt
│
├── common/
│   ├── Logger.*
│   └── CMakeLists.txt
│
├── docs/
│
├── ecu/
│   ├── engine/
│   │   ├── Engine.*
│   │   ├── EngineEcu.*
│   │   ├── main.cpp
│   │   └── CMakeLists.txt
│   │
│   ├── battery/
│   │   ├── Battery.*
│   │   ├── BatteryEcu.*
│   │   ├── main.cpp
│   │   └── CMakeLists.txt
│   │
│   └── abs/
│
├── scripts/
│   └── run_platform.sh
│
├── tests/
│   ├── test_engine.cpp
│   ├── test_engine_can_message.cpp
│   ├── test_battery_can_message.cpp
│   └── test_logger.cpp
│
├── CMakeLists.txt
└── README.md
```

---

## 🛠️ Technologies

* **C++17**
* **Linux**
* **CMake**
* **SocketCAN**
* **Virtual CAN (`vcan0`)**
* **GDB**
* **Doxygen**
* **CTest**
* **systemd** *(planned)*
* **Yocto Project** *(planned)*

---

## 🚀 Build

Clone the repository and build the project with CMake:

```bash
git clone https://github.com/amorirReverse/linux-embedded-automotive-platform.git

cd linux-embedded-automotive-platform

cmake -S . -B build

cmake --build build
```

---

## ▶️ Run

Before starting the platform, create the virtual CAN interface:

```bash
sudo modprobe vcan

sudo ip link add dev vcan0 type vcan

sudo ip link set up vcan0
```

### Engine ECU

```bash
./build/ecu/engine/engine_ecu
```

### Battery ECU

```bash
./build/ecu/battery/battery_ecu
```

### Gateway

```bash
./build/apps/gateway/gateway
```

### Full platform

The complete platform can be started with:

```bash
./scripts/run_platform.sh
```

The startup script launches the Gateway, Engine ECU, and Battery ECU together.

---

## 🔌 CAN Communication

The project currently uses a Linux virtual CAN interface through SocketCAN.

Monitor CAN traffic with:

```bash
candump vcan0
```

### Engine CAN message

The Engine ECU publishes engine status messages using CAN identifier `0x100`.

The payload contains:

* Engine speed in RPM
* Engine temperature in degrees Celsius

Example payload for 800 RPM and 90 °C:

```text
20 03 84 03 00 00 00 00
```

### Battery CAN message

The Battery ECU publishes battery status messages using CAN identifier `0x200`.

The payload contains:

* Battery voltage
* Battery current
* Battery state of charge

The Gateway receives and decodes both message types.

---

## 🧪 Testing

The project uses standalone test executables integrated with **CTest**.

Run the complete test suite with:

```bash
ctest --test-dir build
```

Current tests cover:

* Engine CAN message encoding and decoding
* Battery CAN message encoding and decoding
* Engine model behavior
* Shared Logger component

Example:

```text
Test project .../build
    Start 1: logger_test
    Start 2: engine_can_message_test
    Start 3: battery_can_message_test
    Start 4: engine_test

100% tests passed
```

Individual tests can also be executed directly from the build directory.

---

## 📚 Documentation

API documentation is planned using Doxygen.

The documentation will progressively cover:

* Software architecture
* ECU interfaces
* CAN message formats
* Linux interfaces
* Build and deployment procedures
* Testing strategy

---

## 🎯 Project Goals

This project is designed as a practical exploration of embedded Linux software development, with a focus on:

* Modular C++ architecture
* Linux system programming
* CAN communication
* ECU-oriented software design
* Periodic processing
* Inter-process and inter-component communication
* Logging and error handling
* Debugging and testing
* Embedded Linux deployment
* Automotive-oriented system architecture

The long-term goal is to build a small but realistic embedded platform that can be compiled, tested, monitored, and eventually deployed as part of a custom embedded Linux image.

---

## 📌 Versioning

The project uses Git tags to mark stable milestones.

Current stable release:

**v0.2.0 — CAN communication**

Development continues on the `main` branch toward **V0.3**.

A new release tag will be created once the V0.3 milestone is complete.
