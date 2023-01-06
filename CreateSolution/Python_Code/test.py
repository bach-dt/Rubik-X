import serial
from HSV_solving import RUBIK_SOLVING
from pathlib import Path
import cv2
import urllib.request
import numpy as np

ser = serial.Serial('COM4', 9600)
url = 'http://172.20.10.7/cam-hi.jpg'

# ------------------------------------- DETECTION and PROCESS ------------------------------------- #
# Sequence of Center:       Yellow - Blue - Red - Green - Orange - White
# Top: Yellow, Left: Blue

DIR = 'wifi_image'
# define a video capture object
vid = cv2.VideoCapture(1)
count_image = -1
print('RECOGNIZING')
ser.write(b'recgn_Yellow')
while True:
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    frame = cv2.imdecode(imgnp, -1)

    # Display the resulting frame
    cv2.imshow('frame', frame)
    name_image = ["y", "b", "r", "g", "o", "w"]
    signal = ["recgn_Blue", "recgn_Red", "recgn_Green",
              "recgn_Orange", "recgn_White", "retn_Face"]
    if cv2.waitKey(1) & 0xFF == ord('q'):
        count_image += 1
        path_this = f"{DIR}/{name_image[count_image]}.jpg"
        cv2.imwrite(path_this, frame)
        print(f"saved {name_image[count_image]} face")
        ser.write(bytes(signal[count_image], 'utf-8'))

    if count_image == 5:
        break

# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()

solution = RUBIK_SOLVING().solving(DIR)
sol = solution.replace('[', '').replace(']', '').replace(', ', '')
signal = 'S'
sol = signal + sol
ser.write(bytes(sol, 'utf-8'))
