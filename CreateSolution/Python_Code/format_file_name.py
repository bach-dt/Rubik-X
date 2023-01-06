from pathlib import Path
import cv2

DIR = Path("color/g")
count = 0

for image_id in DIR.glob('*.jpg'):
    image = cv2.imread(str(image_id), 1)  # read image
    image = cv2.resize(image, (512, 512))
    cv2.imwrite(f"color/green/green_{count}.jpg", image)
    cv2.imwrite(f"color/sub_image/green_{count}.jpg", image)
    print(f"saved image _{count}_")
    count += 1

print("DONE!")
