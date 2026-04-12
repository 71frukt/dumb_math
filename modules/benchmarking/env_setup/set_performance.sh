#!/bin/bash

# файл, куда мы запишем исходное состояние (performance / power safe)
STATE_FILE="/tmp/dumb_math_cpu_governor.state"
SYSFS_PATH="/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"

# проверяем, существует ли интерфейс cpufreq
if [ -f "$SYSFS_PATH" ]; then
    # читаем текущий governor ("powersave", "schedutil", "ondemand")
    CURRENT_GOVERNOR=$(cat "$SYSFS_PATH")
    
    # сохраняем состояние во временный файл
    echo "$CURRENT_GOVERNOR" > "$STATE_FILE"
else
    # eсли sysfs недоступен (например, виртуалка без прямого доступа к CPU),
    # записываем fallback-значение
    echo "powersave" > "$STATE_FILE"
fi

# переводим систему в режим performance
cpupower frequency-set -g performance > /dev/null 2>&1

# Возвращаем 0 (успех), даже если cpupower ругнулся, 
# чтобы не прерывать CTest из-за отсутствия прав или отсутствия утилиты на CI-сервере
exit 0