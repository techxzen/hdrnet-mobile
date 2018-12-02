
import sys
import os
import re

import generate_rgb_raw
import show_rgb


def main():
    img_file = sys.argv[1]

    if (len(sys.argv) > 2):
        model_dir = sys.argv[2]
    else:
        model_dir = "/home/chen/myworkspace/projects/sample_data/pretrained_models/local_laplacian/strong_1024/binaries/"

    project_dir = "/home/chen/myworkspace/projects/hdrnet-mobile/"

    # create input rgb
    h, w, rgb_file = generate_rgb_raw.generate_rgb_raw(img_file)
    out_file = rgb_file + '.output'

    # run main
    cmd = ''
    cmd += project_dir + 'bin/main '
    cmd += project_dir + 'lib/libhdrnet_api.so '
    cmd += '%s ' %(model_dir)
    cmd += '%s ' %(rgb_file)
    cmd += '%s ' %(out_file)
    cmd += '%d ' %(h)
    cmd += '%d ' %(w)
    os.system(cmd)

    # show output rgb
    show_rgb.show_rgb(out_file, h, w)

    # rm rgb
    cmd = "rm %s" %(rgb_file)
    os.system(cmd)
    cmd = "rm %s" %(out_file)
    os.system(cmd)


if __name__ == "__main__":
    main()