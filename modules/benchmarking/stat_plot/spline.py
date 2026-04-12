import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import make_interp_spline

def main():
    if len(sys.argv) < 2:
        print("Usage: python plot_latency.py <data.csv>")
        sys.exit(1)

    filename = sys.argv[1]

    # Загрузка данных
    try:
        data = np.loadtxt(filename, delimiter=',', skiprows=1)
        
        # Сортировка данных по x (обязательно для сплайна)
        data = data[data[:, 0].argsort()]
        
        x = data[:, 0]
        average = data[:, 1]
        stddev = data[:, 2]
    except Exception as e:
        print(f"Error reading {filename}: {e}")
        sys.exit(1)

    plt.figure(figsize=(10, 6))
    
    # Построение точек с погрешностью
    plt.errorbar(x, average, yerr=stddev, fmt='o', label='Измерения ($\mu \pm 1\sigma$)', 
                 color='black', ecolor='gray', capsize=4, markersize=5, zorder=3)

    # Построение гладкой кривой (кубический сплайн)
    if len(x) >= 4:
        try:
            # k=3 означает кубическую интерполяцию
            spline = make_interp_spline(x, average, k=3)
            
            # Генерация 500 точек для плавности
            N_smooth = np.linspace(min(x), max(x), 500)
            average_smooth = spline(N_smooth)
            
            plt.plot(N_smooth, average_smooth, 'r-', label='Гладкая кривая (кубический сплайн)', zorder=2)
        except Exception as e:
            print(f"Error in plotting spline: {e}")
            plt.plot(x, average, 'r-', label='Linear interpolation', zorder=2)
    else:
        # Fallback для малого количества точек
        plt.plot(x, average, 'r-', label='Linear interpolation (< 4 points)', zorder=2)

    # Оформление графика
    plt.xlabel('x')
    plt.ylabel('Latency')
    plt.title('Зависимость времени выполнения от x')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    
    # Ограничение оси Y снизу нулем
    plt.ylim(bottom=0)

    plt.savefig('latency_plot.png', dpi=300)
    plt.show()

if __name__ == "__main__":
    main()