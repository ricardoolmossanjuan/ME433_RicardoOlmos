import numpy as np
import matplotlib.pyplot as plt
import csv

def get_sampling_rate(time_vector):
    return (len(time_vector) - 1) / (time_vector[-1] - time_vector[0])

def compute_fft(signal, time_vector):
    N = len(signal)
    T = time_vector[-1] - time_vector[0]
    freq = np.fft.fftfreq(N, d=T/N)[:N//2]
    fft_values = np.fft.fft(signal) / N
    return freq, np.abs(fft_values[:N//2])

def plot_filtered_vs_raw(ax_signal, ax_fft, time_vector, raw_data, filtered_data, label):
    ax_signal.plot(time_vector, raw_data[:len(time_vector)], 'gray', label='Raw')
    ax_signal.plot(time_vector, filtered_data, 'blue', label='Filtered')
    ax_signal.set_title(f'{label} Signal')
    ax_signal.set_xlabel('Time (s)')
    ax_signal.set_ylabel('Amplitude')
    ax_signal.legend()

    freq_raw, fft_raw = compute_fft(raw_data, time_vector)
    freq_filt, fft_filt = compute_fft(filtered_data, time_vector)

    ax_fft.loglog(freq_raw, fft_raw, 'gray', label='Raw FFT')
    ax_fft.loglog(freq_filt, fft_filt, 'blue', label='Filtered FFT')
    ax_fft.set_title(f'{label} FFT')
    ax_fft.set_xlabel('Frequency (Hz)')
    ax_fft.set_ylabel('Magnitude')
    ax_fft.legend()

def apply_fir_filter(signal, cutoff_hz, num_taps, sample_rate):
    # Design low-pass FIR filter using windowed sinc
    t = np.arange(num_taps) - (num_taps - 1) / 2
    h = np.sinc(2 * cutoff_hz / sample_rate * t)
    h *= np.blackman(num_taps)
    h /= np.sum(h)
    return np.convolve(signal, h, mode='valid')

# Filenames to process
csv_files = ['sigA.csv', 'sigB.csv', 'sigC.csv', 'sigD.csv']
cutoffs = [100, 50, 5000, 15]
taps = [40, 60, 1, 250]

fig, axes = plt.subplots(len(csv_files), 2, figsize=(14, 18))

for idx, file in enumerate(csv_files):
    time_vals = []
    signal_vals = []
    
    with open(file) as f:
        reader = csv.reader(f)
        for row in reader:
            time_vals.append(float(row[0]))
            signal_vals.append(float(row[1]))

    time_vals = np.array(time_vals)
    signal_vals = np.array(signal_vals)
    
    fs = get_sampling_rate(time_vals)
    filtered_signal = apply_fir_filter(signal_vals, cutoffs[idx], taps[idx], fs)
    time_filtered = time_vals[:len(filtered_signal)]
    
    plot_filtered_vs_raw(
        axes[idx][0], axes[idx][1],
        time_filtered,
        signal_vals[:len(time_filtered)],
        filtered_signal,
        label=f'Signal {chr(65 + idx)}'
    )

plt.tight_layout()
plt.show()