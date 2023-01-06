import glob
from pathlib import Path
DIR = 'wifi_image'
dir = glob.glob(DIR + '/*.jpg')
print(len(dir))