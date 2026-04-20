import sys
import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import make_interp_spline
import itertools

def main():
    if len(sys.argv) < 3:
        print("Usage: python spline.py <output_dir> <input1.csv> [input2.csv ...]")
        sys.exit(1)

    out_dir = sys.argv[1]
    input_files = sys.argv[2:]

    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    datasets = []
    global_max_x = 0
    global_max_y = 0

    # 1. Чтение данных
    for filename in input_files:
        try:
            with open(filename, 'r') as f:
                func_name = f.readline().strip()

            data = np.loadtxt(filename, delimiter=',', skiprows=2)
            
            if data.ndim == 1:
                data = data.reshape(1, -1)
                
            data = data[data[:, 0].argsort()]
            
            x = data[:, 0]
            average = data[:, 1]
            stddev = data[:, 2]

            if len(x) > 0:
                global_max_x = max(global_max_x, max(x))
                global_max_y = max(global_max_y, max(average))
            
            datasets.append({
                'filename': filename,
                'func_name': func_name,
                'x': x,
                'average': average,
                'stddev': stddev
            })
        except Exception as e:
            print(f"Error reading {filename}: {e}")

    if not datasets:
        print("No valid data found.")
        sys.exit(1)

    # 2. Вычисление интервалов по геометрической сетке N
    ref_x = sorted(list(set(datasets[0]['x'])))
    intervals = []
    
    if len(ref_x) < 2:
        intervals = [(ref_x[0] if len(ref_x)>0 else 0, ref_x[0]*1.1 if len(ref_x)>0 else 1)]
    else:
        start_idx = 0
        while start_idx < len(ref_x) - 1:
            min_step = ref_x[start_idx+1] - ref_x[start_idx]
            end_idx = start_idx + 1
            
            for j in range(start_idx + 2, len(ref_x)):
                current_step = ref_x[j] - ref_x[j-1]
                
                if current_step > 10 * min_step:
                    break
                end_idx = j
                
            intervals.append((ref_x[start_idx], ref_x[end_idx]))
            start_idx = end_idx

    # 3. Отрисовка всех данных на одном холсте в памяти (БЕЗ ЭКСТРАПОЛЯЦИИ)
    plt.figure(figsize=(10, 6))
    colors = plt.cm.tab10.colors
    color_cycle = itertools.cycle(colors)

    plt.errorbar([np.nan], [np.nan], yerr=[np.nan], fmt='o', color='black', ecolor='black', 
                 capsize=4, markersize=5, label='$\mu \pm 1\sigma$')

    for ds in datasets:
        color = next(color_cycle)
        x = ds['x']
        average = ds['average']
        stddev = ds['stddev']
        func_name = ds['func_name']

        plt.errorbar(x, average, yerr=stddev, fmt='o', 
                     color=color, ecolor=color, alpha=0.4, capsize=4, markersize=5, zorder=3)

        if len(x) >= 4:
            try:
                # Строим сплайн строго до последней реальной точки
                spline = make_interp_spline(x, average, k=3)
                N_smooth = np.linspace(min(x), max(x), 500)
                average_smooth = spline(N_smooth)
                
                plt.plot(N_smooth, average_smooth, '-', color=color, label=func_name, zorder=2)
            except Exception as e:
                plt.plot(x, average, '-', color=color, label=f"{func_name} (linear)", zorder=2)
        else:
            plt.plot(x, average, '-', color=color, label=f"{func_name} (< 4 points)", zorder=2)

    plt.xlabel('N')
    plt.ylabel('')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()

    # 4. Сохранение основного (полного) графика
    plt.title('Full testing interval')
    if global_max_x > 0:
        plt.xlim(left=0, right=global_max_x * 1.05)
        plt.ylim(bottom=0, top=global_max_y * 1.05)
    else:
        plt.xlim(left=0)
        plt.ylim(bottom=0)
        
    plt.savefig(os.path.join(out_dir, "plot_0_main.png"), dpi=300)

    # 5. Нарезка кусков
    for i, (x_start, x_end) in enumerate(intervals):
        local_max_y = float('-inf')
        local_min_y = float('inf')
        has_points = False
        
        for ds in datasets:
            x = ds['x']
            avg = ds['average']
            mask = (x >= x_start) & (x <= x_end)
            if np.any(mask):
                local_max_y = max(local_max_y, max(avg[mask]))
                local_min_y = min(local_min_y, min(avg[mask]))
                has_points = True
        
        if not has_points:
            continue
            
        margin_x = (x_end - x_start) * 0.02
        if margin_x == 0: margin_x = 1
        
        margin_y = (local_max_y - local_min_y) * 0.05
        if margin_y == 0: 
            margin_y = local_max_y * 0.05 if local_max_y != 0 else 1
            
        plt.title(f'Testing interval N: {int(x_start)} - {int(x_end)}')
        
        plt.xlim(left=x_start - margin_x, right=x_end + margin_x)
        plt.ylim(bottom=local_min_y - margin_y, top=local_max_y + margin_y)
        
        plt.savefig(os.path.join(out_dir, f"plot_{i+1}_part.png"), dpi=300)

if __name__ == "__main__":
    main()