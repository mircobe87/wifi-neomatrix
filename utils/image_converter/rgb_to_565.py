#!/usr/bin/env python3

import sys
import cv2 as cv

def pixel_to_str(pixel):
    blue  = pixel[0] << 16
    green = pixel[1] <<  8
    red   = pixel[2]
    return f"{(red|green|blue):06x}"


def bgr_to_565 (pixel):
    blue  = pixel[0] >> 3
    green = pixel[1] >> 2
    red   = pixel[2] >> 3
    return f"{(blue | green << 5 | red << 11):04x}"

im = cv.imread(sys.argv[1], cv.IMREAD_COLOR)

print('{')
for line in im:
    print('    ', end="")
    for pixel in line:
        print('0x' + bgr_to_565(pixel) + ', ', end="")
    print()
print('}')
