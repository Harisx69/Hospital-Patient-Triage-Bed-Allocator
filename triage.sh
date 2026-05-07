#!/ bin / bash
# ============================================================
# Project : Hospital Patient Triage & Bed Allocator
# Script : triage .sh
# Group : Group Pta Nahi
# Members : Muhammad Haris (24F-0764) ,Abdul Rehman(24F -0758)
# Date : 2026 -07 -05
# Purpose : Compute triage priority and pipe patient data
# to the admissions manager process .
# Usage : ./ triage .sh [name] [age] [severity 1 -10]
# ============================================================

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
priority=""

if [ $severity -ge 8 ]; then
    priority="CRITICAL"
elif [ $severity -ge 5 ]; then
    priority="URGENT"
else
    priority="NORMAL"
fi
echo "=============================="
echo "PaTient Triage Report"
echo "Name     : $name"
echo "Age      : $age"
echo "Severity : $severity"
echo "Priority : $priority"

