#!/bin/bash

AFL_SKIP_CPUFREQ=1 AFL_SKIP_BIN_CHECK=1 ./afl-fuzz -t 30000 -m 6144 -i INPUTS -o OUTPUTS -x DICTIONARY/DICTIONARY.dict -R -- 'renode_mode/example-uart.resc'

sleep 5

xterm -e 'script output.log -c "bash -c \"screen /tmp/uart\""'
