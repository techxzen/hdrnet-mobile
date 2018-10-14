

import sys
import cv2
import numpy as np
import os


def show_rgb(input_path, height, width):
    input_file = np.fromfile(input_path, dtype=np.uint8)
    input_file.shape = (height, width, 3)

    input_file = np.flip(input_file, 2)  # OpenCV reads BGR, convert back to RGB.

    ''' save to binary file '''
    output_path = input_path + '.jpg'
    cv2.imwrite(output_path, input_file)
    print(output_path)




def main():
    if (len(sys.argv[1]) < 4):
        print("Error: use python show_rgb.py xx.rgb h w")
    input_path = sys.argv[1]
    height = np.int(sys.argv[2])
    width  = np.int(sys.argv[3])
    show_rgb(input_path, height, width)



if __name__ == "__main__":
    main()