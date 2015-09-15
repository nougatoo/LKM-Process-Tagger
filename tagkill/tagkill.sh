#!/bin/bash
# kill -9 on a process with a specific tag
# assumes you'll input a that that exits
# usages: ./tagkill.sh tag
# tagkillscript 1

(cat /proc/ptag) | grep $1 | awk '{print $1}' | xargs kill -9
