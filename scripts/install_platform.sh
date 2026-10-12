#!/bin/bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"

echo "Installing Linux Embedded Automotive Platform..."


install -m 0755 "${BUILD_DIR}/ecu/engine/engine_ecu" /usr/bin/engine_ecu
install -m 0755 "${BUILD_DIR}/ecu/battery/battery_ecu" /usr/bin/battery_ecu
install -m 0755 "${BUILD_DIR}/apps/gateway/gateway" /usr/bin/gateway
install -m 0755 \
    "${PROJECT_ROOT}/scripts/setup_vcan0.sh" \
    /usr/bin/setup_vcan0.sh

install -m 0644 \
    "${PROJECT_ROOT}/deploy/systemd/engine-ecu.service" \
    /etc/systemd/system/engine-ecu.service

install -m 0644 \
    "${PROJECT_ROOT}/deploy/systemd/battery-ecu.service" \
    /etc/systemd/system/battery-ecu.service

install -m 0644 \
    "${PROJECT_ROOT}/deploy/systemd/gateway.service" \
    /etc/systemd/system/gateway.service

install -m 0644 \
    "${PROJECT_ROOT}/deploy/systemd/vcan0.service" \
    /etc/systemd/system/vcan0.service

install -d -m 0755 \
    /etc/linux-embedded-automotive

install -m 0644 \
    "${PROJECT_ROOT}/config/platform.conf" \
    /etc/linux-embedded-automotive/platform.conf

echo "Installation completed successfully."