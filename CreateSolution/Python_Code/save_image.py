import cv2
import urllib.request
import numpy as np
from pathlib import Path

path = 'dataset'
DIR = Path(path)

# # define a video capture object
# vid = cv2.VideoCapture(0)                                             # uncomment this for webcam

url = 'http://172.20.10.7/cam-hi.jpg'  # uncomment this for espcam

count_image = 0

while True:
    # # Capture the video frame
    # # by frame
    # ret, frame = vid.read()                                           # uncomment this for webcam

    img_resp = urllib.request.urlopen(url)  # uncomment this for espcam
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)  # uncomment this for espcam
    frame = cv2.imdecode(imgnp, -1)  # uncomment this for espcam

    # Display the resulting frame
    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        count_image += 1
        path_this = f"{path}/{count_image}.jpg"
        cv2.imwrite(path_this, frame)
        print(f"saved {count_image}")

    if cv2.waitKey(1) & 0xFF == ord('w'):
        break
