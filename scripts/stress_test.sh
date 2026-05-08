#!/bin/bash

for i in {1..20}
do
    ./scripts/triage.sh Test$i 20 $((RANDOM%10+1)) &

done

wait
