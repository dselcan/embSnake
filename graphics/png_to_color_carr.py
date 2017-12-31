import sys

import matplotlib.image as mpimg
import numpy as np
import os

def rgb_to_cm(pixel):
    r, g, b = pixel[0], pixel[1], pixel[2]
    print r, g, b
    if r == 0:
        return 0
    elif r < 32*3:
        return 1
    elif r < 32*5:
        return 2
    else:
        return 3

if __name__ == "__main__":
    png_file = sys.argv[1]
    
    folder = os.path.split(png_file)[0]
    file = os.path.split(png_file)[1]
    
    carr_file = os.path.join(folder, "embSnake_img_" + file.replace(".png", ".h"))
    name = file.replace(".png", "").upper()
    
    img = mpimg.imread(png_file)
    img = np.array(img*255, dtype=np.uint8)
    
    
    height, width  = img.shape[:2]
    
    s = ""
    s += "#define IMG_%s_WIDTH   %d\n" % (name, width)
    s += "#define IMG_%s_HEIGHT  %d\n" % (name, height)
    s += "static uint8_t const IMG_%s[%d][%d] = {\\\n" % (name, height, width)
    
    for row in img:
        s += "\t"
        for pixel in row:
            val = rgb_to_cm(pixel)
            s += "%d," % val
        s += "\\\n"

    s = s[:-3] + " \\\n"
           
    s += "};\n"
    
    f = open(carr_file, "w")
    f.write(s)
    f.close()