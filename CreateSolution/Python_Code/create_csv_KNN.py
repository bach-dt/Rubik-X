import csv
from pathlib import Path

with open("color/KNN.csv", "w") as file:
    writer = csv.writer(file)
    writer.writerow(["image_id", "label"])
    for image_id in Path("color/yellow").glob('*.jpg'):
        writer.writerow([image_id, "0"])
    print("wrote YELLOW ...")
    for image_id in Path("color/blue").glob('*.jpg'):
        writer.writerow([image_id, "1"])
    print("wrote BLUE ...")
    for image_id in Path("color/red").glob('*.jpg'):
        writer.writerow([image_id, "2"])
    print("wrote RED ...")
    for image_id in Path("color/green").glob('*.jpg'):
        writer.writerow([image_id, "3"])
    print("wrote GREEN ...")
    for image_id in Path("color/orange").glob('*.jpg'):
        writer.writerow([image_id, "4"])
    print("wrote ORANGE ...")
    for image_id in Path("color/white").glob('*.jpg'):
        writer.writerow([image_id, "5"])
    print("wrote WHITE ...")

print("DONE!")
