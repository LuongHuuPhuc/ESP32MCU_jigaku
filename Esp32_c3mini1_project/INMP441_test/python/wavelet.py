import numpy as np
import pandas as pd
import pywt
import matplotlib.pyplot as plt

# 🔹 Đọc file CSV
def read_csv(file_path):
    df = pd.read_csv(file_path)
    signal = df.iloc[:, 0].values  # Giả sử cột thứ 2 chứa dữ liệu tín hiệu
    return signal

# 🔹 Thực hiện DWT với họ Daubechies (db4)
def wavelet_transform(signal, wavelet_name='db4', level=4):
    coeffs = pywt.wavedec(signal, wavelet_name, level=level)  # Phân rã tín hiệu
    cA = coeffs[0]  # Thành phần tần số thấp nhất
    cD = coeffs[1:] # Thành phần tần số cao (detail coefficients)
    return cA, cD

# 🔹 Hiển thị kết quả DWT
def plot_wavelet(signal, cA, cD, wavelet_name='db4'):
    plt.figure(figsize=(12, 6))
    
    # Biểu đồ tín hiệu gốc
    plt.subplot(len(cD) + 2, 1, 1)
    plt.plot(signal, label="Tín hiệu gốc", color='black')
    plt.legend()

    # Biểu đồ tần số thấp (Approximation Coefficients)
    plt.subplot(len(cD) + 2, 1, 2)
    plt.plot(cA, label=f"Thành phần tần số thấp ({wavelet_name})", color='blue')
    plt.legend()

    # Biểu đồ các thành phần tần số cao (Detail Coefficients)
    for i, detail in enumerate(cD):
        plt.subplot(len(cD) + 2, 1, i + 3)
        plt.plot(detail, label=f"Tần số cao - Bậc {i+1}", color='red')
        plt.legend()

    plt.tight_layout()
    plt.show()

# 🔹 Chạy chương trình
file_path = "D:\Esp-idf\Mysource\inmp441_test\data_text\Freq_test/HeartBeat_raw/2000Hz/test1.csv"  # Thay bằng đường dẫn file CSV của bạn
signal = read_csv(file_path)  # Đọc tín hiệu từ file CSV
cA, cD = wavelet_transform(signal, wavelet_name='db4', level=4)  # Biến đổi wavelet
plot_wavelet(signal, cA, cD, wavelet_name='db4')  # Vẽ biểu đồ
