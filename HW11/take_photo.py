from time import sleep
from picamera import PiCamera

camera = PiCamera()
camera.resolution = (1024, 768)  # or (1280, 720)
sleep(2)  # allow sensor to warm up
camera.capture('line_4.png')  # capture photo