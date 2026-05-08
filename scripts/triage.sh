#!/bin/bash

name=$1
age=$2
severity=$3

if [ $# -ne 3 ]
then
    echo "Usage: ./triage.sh name age severity"
    exit 1
fi

if ! [[ $age =~ ^[0-9]+$ ]]
then
    echo "Invalid age"
    exit 1
fi

if ! [[ $severity =~ ^[0-9]+$ ]]
then
    echo "Invalid severity"
    exit 1
fi

if [ $severity -ge 9 ]
then
    priority=1
elif [ $severity -ge 7 ]
then
    priority=2
elif [ $severity -ge 5 ]
then
    priority=3
elif [ $severity -ge 3 ]
then
    priority=4
else
    priority=5
fi

echo "$name $age $severity $priority"
