

import cv2
import skimage
import skimage.transform
import numpy as np


def main():
    ''' read image '''
    input_file = "/home/chen/myworkspace/projects/hdrnet-mobile/sample_data/input.png"
    input_img = cv2.imread(input_file, -1)
    if input_img.shape[2] == 4:
        log.info("Input {} has 4 channels, dropping alpha".format(input_path))
        input_img = input_img[:, :, :3]

    input_img = np.flip(input_img, 2)  # OpenCV reads BGR, convert back to RGB.

    ''' hack for hdr+, convert to [0, 1], similar to y = x/255.0 '''
    input_img = skimage.img_as_float(input_img)
    print(input_img)

    ''' low resolution image '''
    lowres_input = skimage.transform.resize(input_img, [256, 256], order = 0)
    print(lowres_input.shape)

    ''' set output name '''
    output_path = input_file + '-output.png'
    print(output_path)

    input_img = input_img[np.newaxis, :, :, :]
    lowres_input = lowres_input[np.newaxis, :, :, :]

    print(input_img.shape)
    print(lowres_input.shape)

    # out_ =  sess.run(output, feed_dict=feed_dict)


if __name__ == "__main__":
    main()
