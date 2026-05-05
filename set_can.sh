#!/bin/bash

INTERFACE="can0"
BITRATE=1000000

echo "--- Initializing $INTERFACE ---"
sudo ip link set $INTERFACE down 2>/dev/null
sudo ip link set $INTERFACE type can bitrate $BITRATE
sudo ip link set $INTERFACE txqueuelen 1000
sudo ip link set $INTERFACE up

if ! ip link show $INTERFACE | grep -q "state UP"; then
    echo "Error: $INTERFACE is not active."
    exit 1
fi

# Create a temporary pipe in RAM for the response
PIPE="/dev/shm/can_pipe"
[ -p "$PIPE" ] || mkfifo "$PIPE"

echo "--- Starting Batch Motor Test ---"

for i in {1..1}
do
    HEX_ID=$(printf '%X' $i)
    CMD_ID="14${HEX_ID}#0000000000000000"
    EXPECTED_ID="24${HEX_ID}"

    echo -n "Testing Motor ID 0x14$HEX_ID... "

    # 1. Start candump in background, looking specifically for the response ID
    # 'stdbuf -oL' ensures the output is sent to the pipe instantly
    stdbuf -oL candump $INTERFACE | grep --line-buffered "$EXPECTED_ID" > "$PIPE" &
    DUMP_PID=$!

    # 2. Short wait to ensure candump is attached to the kernel
    sleep 0.2

    # 3. Send the command
    cansend $INTERFACE $CMD_ID

    # 4. Use 'read' with a timeout to grab the response from the pipe
    if read -t 0.8 RESPONSE < "$PIPE"; then
        echo "OK"
    else
        echo "FAILED (No response)"
        # Kill the hung candump process if it didn't find anything
        kill $DUMP_PID 2>/dev/null
    fi
done

# Clean up
rm "$PIPE"
echo "--- Test Complete ---"