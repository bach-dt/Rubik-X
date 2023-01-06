from HSV_solving import RUBIK_SOLVING
from pathlib import Path
import cv2

# ------------------------------------- DETECTION and PROCESS ------------------------------------- #
# Sequence of Center:       Yellow - Blue - Red - Green - Orange - White
# Top: Yellow, Left: Blue

path = 'webcam_image'
DIR = Path(path)

# define a video capture object
vid = cv2.VideoCapture(1)
count_image = -1

while True:
    # Capture the video frame
    # by frame
    ret, frame = vid.read()

    # Display the resulting frame
    cv2.imshow('frame', frame)
    name_image = ["y", "b", "r", "g", "o", "w"]
    if cv2.waitKey(1) & 0xFF == ord('q'):
        count_image += 1
        path_this = f"{path}/{name_image[count_image]}.jpg"
        cv2.imwrite(path_this, frame)
        print(f"saved {name_image[count_image]} face")

    if count_image == 5:
        break

# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()

RUBIK_SOLVING().solving(DIR)