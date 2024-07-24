#!/bin/bash

while true; do
    start_time=$(date +%s.%N)  # Record the start time
    response=$(curl -s -o /dev/null -w "%{time_total}" http://192.168.4.1/api/available-networks)  # Replace with your desired URL
    end_time=$(date +%s.%N)  # Record the end time
    duration=$(echo "$end_time - $start_time" | bc)  # Calculate the duration

    echo "Request duration: $duration seconds"
    #sleep 1  # Adjust the sleep duration as needed
done