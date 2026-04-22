#!/bin/bash
# Использование: ./run_max_priority.sh ./exec_bin [cpu_id] [exec_args]

BIN=$1
CPU_ID=${2:-1} # по умолчанию ядро 1

if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <executable> <cpu_id> [args...]"
    exit 1
fi

if [ ! -f "$BIN" ]; then
    echo "Error: Binary not found."
    exit 1
fi

# Сдвигаем позиционные аргументы на 2 влево. 
# Теперь $1 и $2 удалены из списка параметров, 
# а $3 становится $1.
shift 2

# задает максимальный RT приоритет с жесткой привязкой к ядру через taskset
chrt -f 99 taskset -c "${CPU_ID}" "${TARGET_EXE}" "$@"