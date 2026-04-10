#!/bin/bash

STATE_FILE="/tmp/dumb_math_cpu_governor.state"

# проверяем, существует ли файл с сохраненным состоянием
if [ -f "$STATE_FILE" ]; then
    # читаем исходное состояние
    RESTORE_GOVERNOR=$(cat "$STATE_FILE")
    
    # восстанавливаем состояние
    cpupower frequency-set -g "$RESTORE_GOVERNOR" > /dev/null 2>&1
    
    # удаляем временный файл, убирая за собой
    rm -f "$STATE_FILE"
else
    # Fallback на случай, если Setup-скрипт не отработал, 
    # а Teardown почему-то вызвался, или файл удалили извне.
    cpupower frequency-set -g schedutil > /dev/null 2>&1
fi

exit 0