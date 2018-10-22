

import sys
import cv2
import numpy as np
import os


def img_to_rgb(input_path):
    input_img = cv2.imread(input_path, -1)
    if input_img.shape[2] == 4:
        log.info("Input {} has 4 channels, dropping alpha".format(input_path))
        input_img = input_img[:, :, :3]

    input_img = np.flip(input_img, 2)  # OpenCV reads BGR, convert back to RGB.

    input_img = np.transpose(input_img, (2,0,1))

    channel, height, width = input_img.shape

    ''' save to binary file '''
    input_path_dirname = os.path.dirname(input_path)
    input_path_basename = os.path.basename(input_path)
    input_path_name = input_path_basename.split('.')[0]
    input_path_type = input_path_basename.split('.')[1]
    output_path = input_path_dirname + '/' + input_path_name + '_%dx%d.rgb' %(height, width)
    print(input_img.dtype)
    input_img.tofile(output_path)
    print(output_path)



def main():
    input_path = sys.argv[1]
    img_to_rgb(input_path)



if __name__ == "__main__":
    main()