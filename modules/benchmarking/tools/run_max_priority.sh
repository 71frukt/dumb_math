#!/bin/bash
# Использование: ./run_max_priority.sh ./exec_bin [cpu_id]

BIN=$1
CPU_ID=${2:-1} # по умолчанию ядро 1

if [ ! -f "$BIN" ]; then
    echo "Error: Binary not found."
    exit 1
fi

# задает максимальный RT приоритет с жесткой привязкой к ядру через taskset
sudo chrt -f 99 taskset -c $CPU_ID $BIN