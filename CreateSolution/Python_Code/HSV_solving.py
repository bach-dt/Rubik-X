import glob

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
from tqdm.auto import tqdm


class RUBIK_SOLVING:
    def __init__(self):
        self.COLORs = ["green", "yellow", "red", "white", "orange", "blue"]

        self.COLORtoSYMBOL = {
            "green": "g",
            "yellow": "y",
            "red": "r",
            "white": "w",
            "orange": "o",
            "blue": "b",
        }

        self.DRAW_COLOR = {
            "green": [0, 200, 0],
            "yellow": [200, 200, 0],
            "red": [200, 0, 0],
            "white": [200, 200, 200],
            "orange": [200, 150, 0],
            "blue": [0, 0, 200],
        }

        self.SequenceCenter = {
            'y': 0,
            'b': 1,
            'r': 2,
            'g': 3,
            'o': 4,
            'w': 5
        }

    def drawLine(self, image, box):
        cv2.rectangle(image,
                      (box[0], box[1]),
                      (box[2], box[3]),
                      (220, 0, 0), 2)

        #     ---x1---x2---
        #     |   |   |   |
        #     y1---------y1
        #     |   |   |   |
        #     y2---------y2
        #     |   |   |   |
        #     ---x1---x2---

        x1 = int((box[0] * 2 + box[2]) / 3)
        x2 = int((box[0] + box[2] * 2) / 3)
        y1 = int((box[1] * 2 + box[3]) / 3)
        y2 = int((box[1] + box[3] * 2) / 3)
        cv2.line(image, (x1, box[1]), (x1, box[3]), (0, 255, 0), 2)
        cv2.line(image, (x2, box[1]), (x2, box[3]), (0, 255, 0), 2)
        cv2.line(image, (box[0], y1), (box[2], y1), (0, 255, 0), 2)
        cv2.line(image, (box[0], y2), (box[2], y2), (0, 255, 0), 2)

    def mainBox(self, image, boxes):
        max_area = 1
        main_box = []
        for box in boxes:
            if (box[2] - box[0]) * (box[3] - box[1]) > max_area:
                main_box = box
                max_area = (box[2] - box[0]) * (box[3] - box[1])

        box = main_box
        self.drawLine(image, box)
        return box

    def centerColor(self, frame, l_b, u_b):
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, l_b, u_b)
        cnts, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        center = []
        for c in cnts:
            area = cv2.contourArea(c)
            if area > 7000:
                M = cv2.moments(c)
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])
                center.append([cx, cy])
        return center

    def formatFace(self, frame, centers):
        face = np.zeros((3, 3), str)
        for color in self.COLORs:
            for center in centers[color]:
                segment_y = frame.shape[1] / 3
                segment_x = frame.shape[0] / 3
                face[int(center[1] / segment_x)][int(center[0] / segment_y)] = self.COLORtoSYMBOL[color]
        return face

    def solving(self, DIR):
        solution = ''
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

        FACE = np.zeros((6, 3, 3), str)

        dir = glob.glob(DIR + '/*.jpg')
        for i, image_id in tqdm(enumerate(dir), total=len(dir)):
        # for image_id in DIR.glob('*.jpg'):
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

            box = self.mainBox(image_show, boxes)

            frame = image_process[box[1]:box[3], box[0]:box[2]]
            frame = cv2.resize(frame, (512, 512))
            frame = cv2.blur(frame, (5, 5))

            l_b_green = np.array([45, 100, 80])
            u_b_green = np.array([90, 255, 255])

            l_b_yellow = np.array([25, 40, 125])
            u_b_yellow = np.array([50, 255, 255])

            l_b_red_1 = np.array([0, 100, 150])
            u_b_red_1 = np.array([7, 255, 255])
            l_b_red_2 = np.array([165, 100, 150])
            u_b_red_2 = np.array([180, 255, 255])

            l_b_white = np.array([0, 0, 100])
            u_b_white = np.array([180, 65, 255])

            l_b_orange = np.array([7, 80, 150])
            u_b_orange = np.array([25, 255, 255])

            l_b_blue = np.array([90, 90, 75])
            u_b_blue = np.array([125, 255, 255])

            centers = {}
            centers["green"] = self.centerColor(frame, l_b_green, u_b_green)
            centers["yellow"] = self.centerColor(frame, l_b_yellow, u_b_yellow)
            centers["red"] = self.centerColor(frame, l_b_red_1, u_b_red_1)
            for center_red_2 in self.centerColor(frame, l_b_red_2, u_b_red_2):
                centers["red"].append(center_red_2)
            centers["white"] = self.centerColor(frame, l_b_white, u_b_white)
            centers["orange"] = self.centerColor(frame, l_b_orange, u_b_orange)
            centers["blue"] = self.centerColor(frame, l_b_blue, u_b_blue)

            this_face = self.formatFace(frame, centers)
            this_face[1, 1] = str(image_id).split('\\')[1].split(".")[0]
            try:
                # if self.SequenceCenter[this_face[1, 1]] == 0:
                #     cv2.imshow("", frame)
                #     cv2.waitKey(0)
                FACE[self.SequenceCenter[this_face[1, 1]]] = this_face
            except:
                cv2.imshow("ERROR!", frame)
                cv2.waitKey(0)

        PROBLEM_STRING = ''

        for face_num in range(0, 6):
            for x in range(0, 3):
                for y in range(0, 3):
                    PROBLEM_STRING += FACE[face_num, x, y]

        if len(PROBLEM_STRING) == 54:
            print("Done!")
            print("Solving ...")
            try:
                print(FACE)
                solution = str(utils.solve(PROBLEM_STRING, 'Kociemba'))
                print(f'Solved: {solution}')
                print("DONE!")
            except:
                print("CANNOT SOLVE")
        else:
            print("FAILED DETECTION!")
            print(FACE)
        return solution

        # cv2.imshow("image", image_show)
        # cv2.waitKey(0)
