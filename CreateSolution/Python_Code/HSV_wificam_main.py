import urllib.request
import numpy as np
import cv2
from pathlib import Path

# change the IP address below according to the
# IP shown in the Serial monitor of Arduino code
from HSV_solving import RUBIK_SOLVING

url = 'http://172.20.10.7/cam-hi.jpg'

# ------------------------------------- DETECTION and PROCESS ------------------------------------- #
# Sequence of Center:       Yellow - Blue - Red - Green - Orange - White
# Top: Yellow, Left: Blue

DIR = 'wifi_image'

# define a video capture object
vid = cv2.VideoCapture(0)
count_image = -1

while True:
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    frame = cv2.imdecode(imgnp, -1)

    # Display the resulting frame
    cv2.imshow('frame', frame)
    name_image = ["y", "b", "r", "g", "o", "w"]
    if cv2.waitKey(1) & 0xFF == ord('q'):
        count_image += 1
        path_this = f"{DIR}/{name_image[count_image]}.jpg"
        cv2.imwrite(path_this, frame)
        print(f"saved {name_image[count_image]} face")

    if count_image == 5:
        break

# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()

RUBIK_SOLVING().solving(DIR)