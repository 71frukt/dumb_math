#!/usr/bin/gnuplot -persist

if (strlen(ARG1) == 0 || strlen(ARG2) == 0) \
    print "Usage: gnuplot -c plot_stat.gp <data_file> <output_png>"; exit

set terminal pngcairo size 1280, 720 font "Arial,10"
set output ARG2

set title "ULP Error Analysis: " . ARG1
set logscale x

# --- Настройка подробной шкалы ---
set mxtics 10            # Добавляет 9 малых делений между основными степенями 10
set mytics 5             # Добавляет малые деления по оси Y
set grid xtics mxtics ytics mytics lc rgb "#E0E0E0" # Сетка для всех делений светло-серым цветом

set xlabel "x (Log Scale)"
set ylabel "ULP Error"
set format x "10^{%L}"
# set xtics 10                                          # 1 декада
set mxtics 10                                         # деления между основными
set grid xtics mxtics lc rgb "#D0D0D0" lw 0.5       # прозрачность сетки

# --- Отрисовка только точек ---
# pt 7 - закрашенный круг
# ps 0.3 - уменьшенный размер точки (чтобы не сливались в кашу)
plot ARG1 using 1:2 with points pt 1 ps 0.3 lc rgb "#006969" title "ULP Error"

set output