import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import make_interp_spline
import itertools

def main():
    # Ожидается минимум 3 аргумента: скрипт, выходной файл, один входной файл
    if len(sys.argv) < 3:
        print("Usage: python spline.py <output_plot.png> <input1.csv> [input2.csv ...]")
        sys.exit(1)

    # Индекс 1 - путь для сохранения картинки
    out_filename = sys.argv[1]
    
    # Индекс 2 и далее - пути к входным CSV файлам
    input_files = sys.argv[2:]

    plt.figure(figsize=(10, 6))
    
    # Инициализация цветовой палитры (10 высококонтрастных цветов)
    colors = plt.cm.tab10.colors
    color_cycle = itertools.cycle(colors)

    # пустой (невидимый) элемент черного цвета исключительно для общей легенды
    plt.errorbar([np.nan], [np.nan], yerr=[np.nan], fmt='o', color='black', ecolor='black', 
                 capsize=4, markersize=5, label='$\mu \pm 1\sigma$')
    
    for filename in input_files:
        color = next(color_cycle)
        
        try:
            # Чтение первой строки для получения имени функции
            with open(filename, 'r') as f:
                func_name = f.readline().strip()

            data = np.loadtxt(filename, delimiter=',', skiprows=2)
            
            # Защита от случая, когда в файле только одна точка (строка)
            if data.ndim == 1:
                data = data.reshape(1, -1)
                
            data = data[data[:, 0].argsort()]
            
            x = data[:, 0]
            average = data[:, 1]
            stddev = data[:, 2]
        except Exception as e:
            print(f"Error reading {filename}: {e}")
            continue # Пропуск битого файла и переход к следующему

        # Построение точек с погрешностью
        plt.errorbar(x, average, yerr=stddev, fmt='o', 
                     color=color, ecolor=color, alpha=0.4, capsize=4, markersize=5, zorder=3)

        # Построение гладкой кривой
        if len(x) >= 4:
            try:
                spline = make_interp_spline(x, average, k=3)
                N_smooth = np.linspace(min(x), max(x), 500)
                average_smooth = spline(N_smooth)
                
                plt.plot(N_smooth, average_smooth, '-', color=color, label=func_name, zorder=2)
            except Exception as e:
                print(f"Error in plotting spline for {filename}: {e}")
                plt.plot(x, average, '-', color=color, label=f"{func_name} (linear)", zorder=2)
        else:
            plt.plot(x, average, '-', color=color, label=f"{func_name} (< 4 points)", zorder=2)

    plt.xlabel('N')
    plt.ylabel('Latency')
    plt.title('Matrix mul funcs testing')
    plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.legend()
    plt.ylim(bottom=0)

    plt.savefig(out_filename, dpi=300)

if __name__ == "__main__":
    main()