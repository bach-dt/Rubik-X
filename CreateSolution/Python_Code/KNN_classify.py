import torchvision
import torch
import numpy as np
import cv2
from matplotlib import pyplot as plt
from torchvision.models.detection.faster_rcnn import FastRCNNPredictor
from torchvision.models.detection import FasterRCNN
from torchvision import transforms
from pathlib import Path
from rubik_solver import utils
import pandas as pd
import urllib.request
from sklearn.neighbors import KNeighborsClassifier

image_size = 48
COLORtoINT = {
    'y': 0,
    'b': 1,
    'r': 2,
    'g': 3,
    'o': 4,
    'w': 5
}
INTtoCOLOR = {
    0: 'y',
    1: 'b',
    2: 'r',
    3: 'g',
    4: 'o',
    5: 'w'
}

train_csv = pd.read_csv("color/KNN.csv")
X_train = np.zeros((len(train_csv["image_id"]), image_size, image_size, 3))
Y_train = np.zeros((len(train_csv["image_id"])))

for i in range(0, len(train_csv["image_id"]) - 20):
    image_id = train_csv["image_id"][i]
    label = train_csv["label"][i]

    image = cv2.imread(image_id, 1)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    image = cv2.resize(image, (image_size, image_size))
    image = np.array(image)
    image = image / 255

    X_train[i] = image
    Y_train[i] = label

url = 'http://172.20.10.7/cam-hi.jpg'

# ------------------------------------- DETECTION and PROCESS ------------------------------------- #
# Sequence of Center:       Yellow - Blue - Red - Green - Orange - White
# Top: Yellow, Left: Blue

# define a video capture object
vid = cv2.VideoCapture(0)
count_image = -1

path = 'wifi_image'
DIR = Path(path)

# while True:
#     img_resp = urllib.request.urlopen(url)
#     imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
#     frame = cv2.imdecode(imgnp, -1)
#
#     # Display the resulting frame
#     cv2.imshow('frame', frame)
#     name_image = ["y", "b", "r", "g", "o", "w"]
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         count_image += 1
#         path_this = f"{path}/{name_image[count_image]}.jpg"
#         cv2.imwrite(path_this, frame)
#         print(f"saved {name_image[count_image]} face")
#
#     if count_image == 5:
#         break
#
# # After the loop release the cap object
# vid.release()
# # Destroy all the windows
# cv2.destroyAllWindows()

X_train = X_train.reshape((X_train.shape[0], -1))
knn = KNeighborsClassifier(n_neighbors=30)
knn.fit(X_train, Y_train)

num_classes = 2
device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')

# load a model; pre-trained on COCO
model = torchvision.models.detection.fasterrcnn_resnet50_fpn(pretrained=True)

# get number of input features for the classifier
in_features = model.roi_heads.box_predictor.cls_score.in_features

# replace the pre-trained head with a new one
model.roi_heads.box_predictor = FastRCNNPredictor(in_features, num_classes)

# load weight
model.load_state_dict(torch.load('rubik_weight_best.pth', map_location=torch.device('cpu')))
model.to(device)
model.eval()


def mainBox(boxes):
    max_area = 1
    main_box = []
    for box in boxes:
        if (box[2] - box[0]) * (box[3] - box[1]) > max_area:
            main_box = box
            max_area = (box[2] - box[0]) * (box[3] - box[1])
    box = main_box
    return box


FACE = np.zeros((54, image_size * image_size * 3))
for image_id in DIR.glob('y.jpg'):
    print(image_id)
    image_side = str(image_id).split("\\")[1].split(".")[0]

    image = cv2.imread(str(image_id), 1)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    image = cv2.resize(image, (512, 512))
    image_process = image.copy()
    image_show = image.copy()

    convert_tensor = transforms.ToTensor()
    image = convert_tensor(image)
    image = image.to(device)

    Images = [image]

    output = model(Images)

    detection_threshold = 0.5
    sample = image.permute(1, 2, 0).cpu().numpy()
    boxes = output[0]['boxes'].data.cpu().numpy()
    scores = output[0]['scores'].data.cpu().numpy()
    boxes = boxes[scores >= detection_threshold].astype(np.int32)
    box = mainBox(boxes)

    frame = image_process[box[1]:box[3], box[0]:box[2]]
    frame = cv2.resize(frame, (image_size * 3, image_size * 3))
    frame = np.array(frame)
    frame = frame / 255

    for i in range(0, 3):
        for j in range(0, 3):
            sub_image = frame[i * image_size:(i + 1) * image_size, j * image_size:(j + 1) * image_size]
            sub_image = sub_image.reshape((-1))

            FACE[COLORtoINT[image_side]*9 + i*3 + j] = sub_image
            # FACE[i * 3 + j] = sub_image

PROBLEM_INT = np.array(knn.predict(FACE), dtype=int)

l = knn.kneighbors([FACE[5]], n_neighbors=60, return_distance=False)
print(Y_train[l])

PROBLEM_STRING = ""
for i in range(0, len(PROBLEM_INT)):
    if i % 9 == 4:
        PROBLEM_STRING += INTtoCOLOR[int(i / 9)]
    else:
        PROBLEM_STRING += INTtoCOLOR[PROBLEM_INT[i]]

print(PROBLEM_STRING)

solution = utils.solve(PROBLEM_STRING, 'Kociemba')
print(f'Solved: {solution}')
print("DONE!")
