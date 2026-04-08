# 1. Проверяем, переданы ли параметры снаружи. Если нет — ставим дефолтные
if (!exists("filename"))   filename   = '../../../chi_sq_res.txt'
if (!exists("k"))          k          = 5.0
if (!exists("cols_width")) cols_width = 0.5

lines_num = system(sprintf("wc -l < %s", filename)) + 0.0

bin(x, s) = s * floor(x/s) + s/2.0
f(x) = (x <= 0 ? 0 : (x**(k/2.0 - 1.0) * exp(-x/2.0)) / (2.0**(k/2.0) * gamma(k/2.0)))

set style fill solid 0.5
set grid
set xlabel "Value"
set ylabel "Density"
set title sprintf("Chi-Square Distribution (df=%g)", k)

plot filename u (bin($1, cols_width)):(1.0/(lines_num*cols_width)) smooth freq with boxes lc rgb 'blue' title 'Data', \
     f(x) lw 3 lc rgb 'red' title 'Theory'