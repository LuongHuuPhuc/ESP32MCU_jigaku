import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def amplify_pcg_signal(csv_file, column_index=0, gain=2.0, output_file="amplified_pcg.csv"):
    # Đọc dữ liệu từ file CSV
    df = pd.read_csv(csv_file)
    
    # Kiểm tra xem chỉ mục cột có hợp lệ không
    if column_index >= len(df.columns):
        raise ValueError(f"Column index {column_index} is out of range.")
    
    # Lấy tín hiệu PCG từ cột chỉ định (cột index bắt đầu từ 0)
    pcg_signal = df.iloc[:, column_index].values
    
    # Khuếch đại tín hiệu
    amplified_signal = pcg_signal * gain
    
    # Lưu tín hiệu đã khuếch đại vào DataFrame
    df["Amplified_Signal"] = amplified_signal
    
    # Ghi vào file CSV mới
    df.to_csv(output_file, index=False)
    print(f"Amplified PCG signal saved to {output_file}")
    
    # Hiển thị tín hiệu gốc và tín hiệu đã khuếch đại
    plt.figure(figsize=(12, 6))
    # plt.plot(pcg_signal, label="Original Signal", alpha=0.7, color = 'blue')
    plt.plot(amplified_signal, label="Amplified Signal", alpha=0.7, color = 'red')
    plt.legend()
    plt.xlabel("Samples")
    plt.ylabel("Amplitude")
    plt.title("PCG Signal Amplification")
    plt.show()

# Ví dụ sử dụng
csv_file = "D:\Esp-idf\Mysource\inmp441_test\data_text\Freq_test/HeartBeat_raw/1000Hz/test2.csv" 
column_index = 0         # Chỉ định cột cần lấy (ở đây là cột đầu tiên, index 0)
gain = 10.0              # Hệ số khuếch đại

amplify_pcg_signal(csv_file, column_index, gain)
