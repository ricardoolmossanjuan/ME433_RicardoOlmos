from PIL import Image

# Load and convert image
img = Image.open("line_low_res.jpg")
img = img.convert('RGB')
width, height = img.size
print(f"Image size: {width} x {height}")

# IIR parameters
alpha = 0.95
beta = 0.05

for y in range(height):
    brightness = []
    smoothed = []
    derivative = []

    # Calculate brightness and smooth it row-wise
    for x in range(width):
        r, g, b = img.getpixel((x, y))
        bright = r + g + b
        brightness.append(bright)
        if x == 0:
            smoothed.append(bright)
            derivative.append(0)
        else:
            s = alpha * smoothed[-1] + beta * bright
            smoothed.append(s)
            derivative.append(s - smoothed[-2])

    # Find edges by derivative extrema
    left = derivative.index(max(derivative))
    right = derivative.index(min(derivative))
    center = int((left + right) / 2)

    # Mark edges and center on the image
    img.putpixel((left, y), (0, 255, 0))    # green
    img.putpixel((right, y), (0, 255, 0))   # green
    img.putpixel((center, y), (255, 0, 0))  # red

# Show result
img.show()
