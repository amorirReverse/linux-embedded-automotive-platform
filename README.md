# Linux Embedded Automotive Platform

A C++17 Linux embedded automotive platform simulating multiple electronic control units (ECUs) communicating over a CAN bus.

The project focuses on embedded software architecture, Linux system programming, CAN communication, testing, logging, and modular ECU simulations.

---

## 🚧 Project Status

**Current milestone:** Multi-ECU Platform — Reliability and Deployment Validation

**Status:** In progress

The platform supports a standalone CMake build and a Buildroot/QEMU integration for running multiple automotive applications on embedded Linux.

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
* [x] Engine and Battery CAN message unit tests
* [x] Engine model unit tests
* [x] Configuration file parsing and validation
* [x] Configuration unit tests
* [x] CTest integration
* [x] Error reporting across CAN communication and ECU applications
* [x] Detection and logging of ECU cycles exceeding the configured timeout
* [x] Buildroot external tree and QEMU integration
* [x] Automatic ECU startup in the Buildroot image
* [ ] Review and improve error recovery
* [ ] Multithreaded ECU execution
* [ ] Validate systemd service behavior at runtime
* [ ] Independent watchdog supervision


### Future Work
* [ ] Continuous integration
* [ ] Doxygen documentation generation
* [ ] Fault injection and error simulation
* [ ] Yocto-based embedded Linux image
* [ ] Further embedded Linux deployment and validation

V0.3 remains under development. A stable release will be tagged after the milestone's remaining tasks and validation are complete.

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
│   └── gateway/                 # CAN gateway application
├── board/
│   └── qemu/
│       └── rootfs-overlay/      # Buildroot root filesystem overlay
├── buildroot-external/          # Buildroot external tree and package
├── can/                         # SocketCAN abstraction and CAN messages
├── common/                      # Shared logger and configuration parser
├── config/
│   └── platform.conf            # Platform configuration
├── deploy/
│   └── systemd/                 # systemd service unit files
├── ecu/
│   ├── engine/                  # Engine model and ECU
│   ├── battery/                 # Battery model and ECU
│   └── abs/                     # Reserved for future ABS development
├── scripts/
│   ├── install_platform.sh
│   ├── run_platform.sh
│   └── setup_vcan0.sh
├── tests/                       # Unit and ECU tests
├── CMakeLists.txt
└── README.md
```
The project also includes component-level CMake files and tests in the relevant source directories.

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

### Buildroot / QEMU
The repository includes a Buildroot external tree and a QEMU root filesystem overlay.

The overlay provides two SysV init scripts:

* `S40vcan` creates and enables the vcan0 virtual CAN interface.
* `S90automotive` starts the Engine ECU, Battery ECU, and Gateway at boot.

The Buildroot package is defined in
`buildroot-external/package/linux-embedded-automotive-platform/`.

#### Runtime verification
After booting the Buildroot image in QEMU, verify the CAN interface:
```bash
ip link show vcan0
```

Check that the three applications are running:
```bash
ps
```

Inspect the application logs:
```bash
cat /tmp/engine.log
cat /tmp/battery.log
cat /tmp/gateway.log
```

If `candump` is available in the image, monitor CAN traffic with:
```bash
candump vcan0
```

**Reproducibility note:** The repository contains the Buildroot external tree and root filesystem overlay, but does not yet include the Buildroot configuration, image-generation commands, or QEMU launch script needed to reproduce the complete workflow from a clean checkout.


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

Build the project and run the complete test suite with:

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
```

The test suite currently contains 10 tests covering:
* Logger and configuration handling
* CAN socket operations
* Engine and Battery CAN message encoding and decoding
* Engine and Battery model behavior
* Engine and Battery ECU behavior
* Gateway behavior

To list the registered tests without executing them:
```bash
ctest --test-dir build --show-only
```


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

Git tags mark project milestones. Available tags include `v0.1.0`, `v0.2.0`, `v0.3.0`, and `v0.4.0`.

Development and integration work continues on the `main` branch. Refer to the corresponding Git tag to inspect a specific milestone.
