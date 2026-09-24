#!/bin/bash

# scripts/run_platform.sh

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

echo "Starting Linux Embedded Automotive Platform..."

"$BUILD_DIR/apps/gateway/gateway" &
GATEWAY_PID=$!

"$BUILD_DIR/ecu/engine/engine_ecu" &
ENGINE_PID=$!

"$BUILD_DIR/ecu/battery/battery_ecu" &
BATTERY_PID=$!

echo "Gateway PID: $GATEWAY_PID"
echo "Engine ECU PID: $ENGINE_PID"
echo "Battery ECU PID: $BATTERY_PID"

wait "$GATEWAY_PID"
wait "$ENGINE_PID"
wait "$BATTERY_PID"