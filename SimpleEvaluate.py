import torch
from cfg import Cfg
from tool.darknet2pytorch import Darknet
from tool.torch_utils import do_detect
from PIL import Image
import cv2
Cfg.cfgfile = '/home/isv/Documents/tensorrt/yolov4/darknet-master/yolov4-tiny.cfg'
Cfg.weights_file = '/home/isv/Documents/tensorrt/yolov4/darknet-master/backup/yolov4-tiny_last.weights'
model = Darknet(Cfg.cfgfile)
model.print_network()
model.load_weights(Cfg.weights_file)
if torch.cuda.is_available():
    use_cuda = 1
else:
    use_cuda = 0

if use_cuda:
    model = model.cuda()
img = cv2.imread('/home/isv/qt_projects/build-final_thread-Desktop_Qt_5_13_2_GCC_64bit-Debug/plateSaved/187.jpg')
img = cv2.resize(img, (320, 320))
# for i in range(10):
boxes = do_detect(model, img, 0.1, 0.4, use_cuda)

numberOfDetection = len(boxes[0])
for i in range(numberOfDetection):
    each_box = boxes[0][i]
    x = int(320 * each_box[0])
    y = int(320 * each_box[1])
    w = int(320 * each_box[2])
    h = int(320 * each_box[3])
    img = cv2.rectangle(img, (x, y), (w, h), (100, 220, 0), 2)
    cv2.putText(img, str(each_box[6]), (x, h), cv2.FONT_ITALIC, 1, (100, 200, 100), 2)
    print(each_box, ' ', x)
import matplotlib.pyplot as plt

plt.imshow(img, 'gray')
plt.show()
