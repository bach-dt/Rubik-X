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

DIR = Path("dataset")


def mainBox(boxes):
    max_area = 1
    main_box = []
    for box in boxes:
        if (box[2] - box[0]) * (box[3] - box[1]) > max_area:
            main_box = box
            max_area = (box[2] - box[0]) * (box[3] - box[1])
    box = main_box
    return box

count_image = 0
for image_id in DIR.glob('*.jpg'):
    image = cv2.imread(str(image_id), 1)  # read image
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
    frame = cv2.resize(frame, (600, 600))

    count_image = count_image + 1

    for i in range(0, 3):
        for j in range(0, 3):
            sub_image = frame[i * 200:(i + 1) * 200, j * 200:(j + 1) * 200]
            cv2.imwrite(f"color/sub_image_3/sub_image_3_{count_image}_{i}_{j}.jpg", sub_image)

    print(f"saved image {count_image} ...")

print("DONE!")
