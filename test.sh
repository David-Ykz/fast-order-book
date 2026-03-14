#!/bin/bash

CLIENT_BIN="./bin/client"
SERVER_BIN="./bin/server"
PORTS=(8002 8004)

cleanup() {
    echo "Shutting down clients and server..."
    kill $(jobs -p) 2>/dev/null
    
    echo "Generating charts..."
    python3 dashboard/display.py
    
    exit
}

trap cleanup SIGINT SIGTERM

rm -f logs/*.txt charts/*.png

echo "--- Compiling binaries ---"
make compile-server
make compile-client

if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

echo "--- Starting Clients ---"
for PORT in "${PORTS[@]}"; do
    echo "Starting client on port $PORT..."
    $CLIENT_BIN $PORT & 
    $CLIENT_BIN $PORT & 
done

sleep 1

echo "--- Starting Server ---"
$SERVER_BIN

cleanup