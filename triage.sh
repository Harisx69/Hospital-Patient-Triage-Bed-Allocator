#!/bin/bash
# ============================================================
# Project : Hospital Patient Triage & Bed Allocator
# Script  : triage.sh
# Group   : Group Pta Nahi
# Members : Muhammad Haris (24F-0764), Abdul Rehman (24F-0758)
# Date    : 2026-07-05
# Purpose : Compute triage priority and pipe patient data
#           to the admissions manager process.
# Usage   : ./triage.sh [name] [age] [severity 1-10]
# ============================================================

# Define the Pipe Name
TRIAGE_FIFO="triage_fifo"

if [ $# -ne 3 ]; then
    echo "Error: Please provide exactly 3 arguments"
    echo "Usage: ./triage.sh <name> <age> <severity 1-10>"
    exit 1
fi

name=$1
age=$2
severity=$3

# Check if age is a number
if ! [[ "$age" =~ ^[0-9]+$ ]]; then
    echo "Error: Age must be a number"
    exit 1
fi

# Check if severity is a number
if ! [[ "$severity" =~ ^[0-9]+$ ]]; then
    echo "Error: Severity must be a number between 1 and 10"
    exit 1
fi

# Check severity range
if [ $severity -lt 1 ] || [ $severity -gt 10 ]; then
    echo "Error: Severity must be between 1 and 10"
    exit 1
fi

# Triage decision logic
priority_label=""
numeric_priority=0

# Mapping severity to numeric priority for the C Scheduler
# 1 = Highest Priority, 5 = Lowest Priority
if [ $severity -ge 8 ]; then
    priority_label="CRITICAL"
    numeric_priority=1
elif [ $severity -ge 5 ]; then
    priority_label="URGENT"
    numeric_priority=2
else
    priority_label="NORMAL"
    numeric_priority=3
fi

echo "=============================="
echo "Patient Triage Report"
echo "Name     : $name"
echo "Age      : $age"
echo "Severity : $severity"
echo "Priority : $priority_label (Numeric: $numeric_priority)"
echo "=============================="

# Generate a random ID for the simulation
patient_id=$((RANDOM % 900 + 100))
# Default care units to 1 for standard bed allocation
care_units=1

# PIPE DATA TO THE C PROGRAM
# Format: ID PRIORITY UNITS
if [[ -p $TRIAGE_FIFO ]]; then
    echo "$patient_id $numeric_priority $care_units" > "$TRIAGE_FIFO"
    echo "SUCCESS: Data sent to Admissions Manager."
else
    echo "ERROR: $TRIAGE_FIFO not found. Is the C simulation running?"
fi
