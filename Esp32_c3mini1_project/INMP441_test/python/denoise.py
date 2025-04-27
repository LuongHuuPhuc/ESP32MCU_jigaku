import numpy as np
import pywt 
import matplotlib.pyplot as plt
from scipy import signal as sg
 
#Doc tin hieu
def read_signal(file_path):
  # Doc tin hieu tu file csv 
  import pandas as pd
  df = pd.read_csv(file_path, header=None)
  signal_data = df.iloc[:, 1].values
  return signal_data

#Giam nhieu bang Wavelet transform 
def denoise_wavelet(signal_data, wavelet='db4', level=5):
  # Phan ra tin hieu 
  coeffs = pywt.wavedec(signal_data, wavelet, level=level)
  # Giu cac thanh phan tan so thap va loai bo tan so cao (detailed coeffs)
  coeffs[1:] =[np.zeros_like(c) for c in coeffs[1:]] # Loai bo cac chi tiet
  denoised_signal = pywt.waverec(coeffs, wavelet)
  return denoised_signal

# Bandpass filter (Loc tin hieu voi dai tan tu 0.5 - 50Hz)
def bandpass_filter(signal_data, lowcut=20, highcut=120, fs=1000, order=4):
  nyquist = 0.5 * fs  
  low = lowcut / nyquist
  high = highcut / nyquist
  b, a = sg.butter(order, [low, high], btype='band')
  filtered_signal = sg.filtfilt(b, a, signal_data)
  return filtered_signal

#Ve tin hieu goc va tin hieu da giam nhieu 
def plot_signal(original_signal, denoised_signal):
  plt.figure(figsize=(12, 6))

  #Ve tin hieu goc
  plt.subplot(2, 1, 1)
  plt.plot(original_signal, label="Tin hieu goc", color="black")
  plt.title('Tin hieu goc')
  plt.legend()

  #Ve tin hieu da giam nhieu 
  plt.subplot(2, 1, 2)
  plt.plot(denoised_signal, label="Tin hieu da giam nhieu")
  plt.title('Tin hieu da giam nhieu')
  plt.legend()

  plt.tight_layout()
  plt.show()

#Doc tin hieu tu file csv
file_path = "D:/Esp-idf/amplified_pcg.csv"
signal_data = read_signal(file_path)

#Ap dung Wavelet de loc nhieu 
denoised_signal_wavelet = denoise_wavelet(signal_data)

#Ap dung bo loc Bandpass filter
denoised_signal_filter = bandpass_filter(denoised_signal_wavelet)

#Ve tin hieu 
plot_signal(signal_data, denoised_signal_filter)