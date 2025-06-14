from PIL import Image
import matplotlib.pyplot as plt

# Load and convert image
img = Image.open("line_4.png")
img = img.convert('RGB')
width, height = img.size
print(f"Image size: {width} x {height}")

# Select a row to scan (e.g. y = 45)
target_row = 45

x_vals = []
r_vals = []
g_vals = []
b_vals = []
brightness = []
binary_line = []

total = 0
weighted = 0

for x in range(width):
    r, g, b = img.getpixel((x, target_row))
    brightness_val = r + g + b
    x_vals.append(x)
    r_vals.append(r)
    g_vals.append(g)
    b_vals.append(b)
    brightness.append(brightness_val)
    total += brightness_val
    weighted += brightness_val * x

# Binary threshold
avg_brightness = total / width
for val in brightness:
    binary_line.append(100 if val > avg_brightness else 0)

# Center of mass
center = weighted / total if total > 0 else None
print(f"Estimated center: {center}")

# Plot
plt.plot(x_vals, r_vals, 'r-', label='Red')
plt.plot(x_vals, g_vals, 'g-', label='Green')
plt.plot(x_vals, b_vals, 'b-', label='Blue')
plt.plot(x_vals, brightness, 'k-', label='Brightness Sum')
plt.plot(x_vals, binary_line, 'm--', label='Thresholded Line')
if center:
    plt.axvline(center, color='c', linestyle='--', label='Center of Mass')
plt.legend()
plt.title('RGB Intensities and Estimated Center')
plt.xlabel('X Pixel Position')
plt.ylabel('Intensity')
plt.grid(True)
plt.show()