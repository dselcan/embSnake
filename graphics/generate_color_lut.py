import colorsys as csys

LUT_SIZE = 64

#CALIB = [(223, 100, 100), (219, 73, 73), (151, 65, 59)]
VALS = [(155.0, 151.0), (160.0, 209.0), (140.0, 223.0)]

def rgb_to_16b(pixel):
    r, g, b = int(pixel[0]*255), int(pixel[1]*255), int(pixel[2]*255)
    print r, g, b
    return (((r&0xff)>>3) << 11) | (((g&0xff)>>2) << 5) | (((b&0xff)>>3) << 0)

if __name__ == "__main__":
    #for c in CALIB:
        #hsv = csys.rgb_to_hsv(float(c[0])/255, float(c[1])/255, float(c[2])/255)
        #print hsv[0]*255, hsv[1]*255, hsv[2]*255
        #rgb = csys.hsv_to_rgb(*hsv)
        #print rgb
    #quit()
    
    s = ""
    s += "#define COLORS_AMMOUNT %d\n" % (LUT_SIZE)
    s += "#define COLORS_VALUES %d\n" % (len(VALS))
    s += "static uint16_t const COLORS[%d][%d] = {\\\n" % (LUT_SIZE, len(VALS))
    
    for c in range(LUT_SIZE):
        s += "\t"
        for v in VALS:
            val = rgb_to_16b(csys.hsv_to_rgb(float(c)/LUT_SIZE, v[0]/255, v[1]/255))
            s += "%d," % val
        s += "\\\n"
    
    s = s[:-3] + " \\\n"
    s += "};\n"
            
            
    f = open("embSnake_color_lut.h", "w")
    f.write(s)
    f.close()