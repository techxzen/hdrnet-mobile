
import sys
import os
import re

import generate_rgb_raw
import show_rgb


def main():
    img_file = sys.argv[1]

    # create input rgb
    h, w, rgb_file = generate_rgb_raw.generate_rgb_raw(img_file)
    out_file = rgb_file + '.output'

    # run main
    cmd = ''
    cmd += './bin/main '
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