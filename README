# Linux Embedded Automotive Platform

A C++17 Linux embedded automotive platform simulating multiple electronic control units (ECUs) communicating over a CAN bus.

The project focuses on embedded software architecture, Linux system programming, CAN communication, and the development of modular ECU simulations.

---

## 🚧 Project Status

**Current milestone:** V0.3 — Multi-ECU Platform

The project is under active development.

### V0.1 — Initial ECU Simulation

* [x] Project structure
* [x] Engine ECU
* [x] Engine simulation
* [x] Periodic simulation loop

### V0.2 — CAN Communication

* [x] SocketCAN integration
* [x] Virtual CAN interface (`vcan0`)
* [x] CAN socket abstraction
* [x] CAN frame transmission
* [x] Engine ECU CAN integration
* [x] CAN message encoding

### V0.3 — Multi-ECU Platform

* [x] Battery ECU model
* [x] Battery ECU simulation
* [ ] Battery ECU CAN integration
* [ ] Multiple ECUs communicating on the same CAN bus
* [ ] Gateway ECU
* [ ] Unit tests

### Future Work

* [ ] Inter-ECU message handling
* [ ] Fault and error simulation
* [ ] Watchdog supervision
* [ ] systemd service integration
* [ ] Configuration management
* [ ] Continuous integration
* [ ] Yocto-based embedded Linux image

---

## 🏗️ Architecture

The platform is designed around independent ECU components communicating through a Linux CAN interface.

```text
                         ┌──────────────────┐
                         │   Engine ECU     │
                         │                  │
                         │  Engine Model    │
                         └────────┬─────────┘
                                  │
                                  │ CAN
                                  │
                         ┌────────▼─────────┐
                         │     vcan0        │
                         │                  │
                         │    SocketCAN     │
                         └────────┬─────────┘
                                  │
                                  │ CAN
                                  │
                         ┌────────▼─────────┐
                         │   Battery ECU    │
                         │                  │
                         │  Battery Model   │
                         └──────────────────┘
```

The architecture will evolve as additional ECUs and a gateway are introduced.

---

## 📁 Project Structure

```text
linux-embedded-automotive-platform/
├── apps/
│   └── gateway/
├── can/
│   ├── CanSocket.*
│   ├── EngineCanMessage.*
│   └── test_can.cpp
├── common/
├── docs/
├── ecu/
│   ├── engine/
│   │   ├── Engine.*
│   │   ├── EngineEcu.*
│   │   ├── main.cpp
│   │   └── CMakeLists.txt
│   ├── battery/
│   │   ├── Battery.*
│   │   ├── BatteryEcu.*
│   │   ├── main.cpp
│   │   └── CMakeLists.txt
│   └── abs/
├── scripts/
├── tests/
├── CMakeLists.txt
└── README.md
```

---

## 🛠️ Technologies

* **C++17**
* **Linux**
* **CMake**
* **SocketCAN**
* **GDB**
* **Doxygen**
* **systemd**
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

### Engine ECU

```bash
./build/ecu/engine/engine_ecu
```

### Battery ECU

```bash
./build/ecu/battery/battery_ecu
```

---

## 🔌 CAN Communication

The project currently uses a Linux virtual CAN interface through SocketCAN.

Create and configure `vcan0`:

```bash
sudo modprobe vcan

sudo ip link add dev vcan0 type vcan

sudo ip link set up vcan0
```

Monitor CAN traffic with:

```bash
candump vcan0
```

The Engine ECU currently publishes engine status messages using CAN identifier `0x100`.

Example:

```text
vcan0  100   [8]  20 03 F5 00 00 00 00 00
```

The payload contains the simulated engine speed and temperature.

---

## 🧪 Testing

The project currently includes a basic CAN transmission test.

Build the project:

```bash
cmake --build build
```

Run the CAN test:

```bash
./build/can/test_can
```

Monitor the transmitted frame from another terminal:

```bash
candump vcan0
```

Unit testing will be expanded as the multi-ECU architecture develops.

---

## 📚 Documentation

API documentation is generated using Doxygen.

The project documentation will progressively cover:

* Software architecture
* ECU interfaces
* CAN message formats
* Linux interfaces
* Build and deployment procedures

---

## 🎯 Project Goals

This project is designed as a practical exploration of embedded Linux software development, with a focus on:

* Modular C++ architecture
* Linux system programming
* CAN communication
* ECU-oriented software design
* Periodic and real-time-oriented processing
* Debugging and testing
* Embedded Linux deployment
* Automotive-oriented system architecture

The long-term goal is to build a small but realistic embedded platform that can be compiled, tested, monitored, and eventually deployed as part of a custom embedded Linux image.

---

## 📌 Versioning

The project uses Git tags to mark stable milestones.

Current release:

**v0.2.0 — CAN communication**

Development continues on the `main` branch toward the next milestone.
