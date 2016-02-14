# Copyright 2016-2016 the openage authors. See copying.md for legal info.

import argparse
import logging
from PIL import Image
from collections import namedtuple

tool_description = """Python translation of the visgrep v1.09
visual grep, greps for images in another image
Author of the original C version: Steve Slaven - http://hoopajoo.net"""

Point = namedtuple('Point', 'x y')
Pixel = namedtuple('Pixel', 'r g b a')

class FoundResult:
    badness = 0
    point = Point

def img_pixel_get(img, p):
    if p.x < img.size[0] and p.y < img.size[1]:
        return Pixel(*img.getpixel((p.x, p.y)))
    else:
        return Pixel(0, 0, 0, 0)

def img_subimage_find(master, find, start_from, tolerance, find_next):
    px = FoundResult

    x_end = master.size[0] - find.size[0]
    y_end = master.size[1] - find.size[1]

    if find_next:
        start_from = Point(start_from.x + 1, start_from.y)

    logging.debug("Starting from {}".format(start_from))
    logging.debug("End {},{}".format(x_end, y_end))

    ''' Loop the whole freakin image looking for this sub image, but not past edges '''
    for y in range(start_from.y, y_end + 1):
        logging.debug("Begin subimg find loop for y {} of {}".format(y, y_end))
        for x in range(start_from.x, x_end + 1):
            px.point = Point(x, y)
            logging.debug("Begin subimg find loop for x: {}".format(px.point))
            px.badness = img_subimage_cmp(master, find, px.point, tolerance)
            if px.badness <= tolerance:
                logging.debug("Found subimage at {}".format(px.point))
                return px

    ''' No match '''
    px.point = Point(-1, -1)
    px.badness = -1

    return px

''' o is the compare from pixel, assumed to be from a pattern.  It's transparency
      is the transparency used to modify the tolerance value '''
def img_pixel_cmp(p, o):
    '''  return( memcmp( &p, &o, sizeof( PIXEL ) ) ); '''
    ''' make tolerance mean something '''
    logging.debug("{}".format(p, o))
    difference = abs(p.r - o.r) + abs(p.g - o.g) + abs(p.b - o.b)
    transparentness = o.a;
    difference = difference * (transparentness / 255)
    logging.debug("Difference: {}".format(difference))
    return difference

''' Returns 0 if subimage is inside master at where, like *cmp usually does for other stuff
      otherwise returns an integer of how different the match is, for each color component
      value off.  tolerance is how high to go before bailing.  set lower to avoid processing
      lots of extra pixels, it will just ret when tolerance is met '''
def img_subimage_cmp(master, subimage, where, tolerance):
    mpx = spx = Pixel
    mpt = Point(0, 0)

    ''' DMSG( "Comparing images where={},{}\n", where.x, where.y ); '''

    logging.debug("Comparing subimage where={},{}".format(where.x, where.y))

    ''' Check if subimage even fits in masterimage at POINT '''
    if (where.x + subimage.size[0]) > master.size[0] or (where.y + subimage.size[1]) > master.size[1]:
        ''' Superbad '''
        logging.debug("Subimage would not fit here")
        return 1000

    badness = 0
    for sptx in range(subimage.size[0]):
        for spty in range(subimage.size[1]):
            ''' Map U/V to X/Y '''
            mpt = Point(sptx + where.x, spty + where.y)

            logging.debug("Pixel cmp @ main: {},{}, subimage: {},{}".format(mpt.x, mpt.y, sptx, spty))

            ''' Grab pels and see if they match '''
            mpx = img_pixel_get(master, mpt)
            spx = img_pixel_get(subimage, Point(sptx, spty))

            badness += abs(img_pixel_cmp(mpx, spx))
            ''' DMSG( "Badness {}\n", badness ); '''

            if badness > tolerance:
                logging.debug("Bail out early, badness > tolerance {} > {}".format(badness, tolerance))
                ''' No match here, bail early '''
                return badness

    ''' Matched all of subimage '''
    logging.debug("Image match ok, badness = {}".format(badness))
    return badness

def do_output(show_badness, badness, x, y, idx):
    if show_badness:
        print("{} {},{} {}".format(badness, x, y, idx))
    else:
        print("{},{} {}".format(x, y, idx))

parser = argparse.ArgumentParser(description=tool_description,
epilog = """The image.png is
scanned for detect.png starting from X,Y specified above. When detect.png
is found, then all the match.png files are scanned at an offset of x,y as
specified above.  If a match is found, then visgrep prints the x,y and
index of the item.

For example, image.png is a screenshot and match1.png .. to match5.png are
images of letters a to e.  Each of these letters is enclosed in a blue box,
so detect.png is an image of the upper left corner of the box.  This box is
not included in the match*.png files, so they are actually offset 5 pixels
down and 4 pixels to the left.  You might run it like this then:

  visgrep -b -t50 -x-4 -y5 image.png match_corner.png match_a.png match_b.png ...

Etc, with all matches listed.  Now suppose the screen showed 'ace' so
visgrep might output:

0 10,10 0
12 50,10 2
7 90,10 4

Showing that match_a.png (index 0) is at 10,10 on the screen.  If no match
is found even though the detection image is found, the index will be -1.

The first match was 100%% accurate, while the second and third were very slightly
inaccurate, probably due to anti-aliasing on the fonts.

Exit status is 0 for successful match, 1 for no match, and 2 for error.

See the examples page for use cases for different flags""", formatter_class=argparse.RawDescriptionHelpFormatter)

parser.add_argument("-x", dest="off_x", default=0, metavar="X_OFF", type=int, help="Set x offset for detection matching")
parser.add_argument("-y", dest="off_y", default=0, metavar="Y_OFF", type=int, help="Set y offset for detection matching")
parser.add_argument("-I", dest="src_width", default=0, metavar="WIDTH", type=int, help="Set width for cropping the source image")
parser.add_argument("-J", dest="src_height", default=0, metavar="HEIGHT", type=int, help="Set height for cropping the source image")
parser.add_argument("-W", dest="sub_width", default=0, metavar="X_OFF_WIDTH", type=int, help="Set x offset width for detection matching")
parser.add_argument("-H", dest="sub_height", default=0, metavar="Y_OFF_HEIGHT", type=int, help="Set y offset height for detection matching")
parser.add_argument("-X", dest="start_x", default=0, metavar="START_X_OFF", type=int, help="Start scanning at X")
parser.add_argument("-Y", dest="start_y", default=0, metavar="START_Y_OFF", type=int, help="Start scanning at Y")
parser.add_argument("-u", dest="off_u", default=0, metavar="U_OFF", type=int, help="Set u offset for cropping patterns before use")
parser.add_argument("-v", dest="off_v", default=0, metavar="V_OFF", type=int, help="Set y offset for cropping patterns before use")
parser.add_argument("-M", dest="pattern_width", metavar="U_WIDTH", type=int, help="Set width of the cropped patterns")
parser.add_argument("-N", dest="pattern_height", metavar="V_HEIGHT", type=int, help="Set height of the cropped patterns")
parser.add_argument("-a", dest="scan_all", help="Scan all patterns, not just after matching the detection pattern note: this method is much slower because we scan for all images at every pixel instead of just at detection points. Also, in this mode the detection image is ignored, there will be no matches for tile -1", action="store_true")
parser.add_argument("-t", dest="tolerance", default=0, metavar="TOLERANCE", type=int, help="Set tolerance for 'fuzzy' matches, higher numbers are more tolerant")
parser.add_argument("-b", dest="show_badness", help="Display 'badness' value, higher numbers mean match is less accurate, a badness value of 0 means the match is pixel-perfect", action="store_true")
parser.add_argument("-d", metavar="DEBUGLEVEL", type=int, help="Print debug messages")
parser.add_argument("image", metavar='image.png', help="Image to search in")
parser.add_argument("detect", metavar='detect.png', help="Pattern to search")
parser.add_argument("match", metavar='match.png', nargs="*", help="Images to compare if detected")

args = parser.parse_args()

logging.basicConfig(level=args.d)

if len([v for v in (args.off_u, args.off_v, args.pattern_width, args.pattern_height) if v is not None]) != 4:
    parser.error('-u, -v, -M, -N must be given together')

pt = FoundResult

gotmatch = find_next = False
tmp_off_x = tmp_off_y = 0
final_exit = 1

img = Image.open(args.image).convert('RGBA')
find = Image.open(args.detect).convert('RGBA')
matches = [Image.open(fname).convert('RGBA') for fname in args.match]

img = img.crop((0, 0, args.src_width or img.size[0], args.src_height  or img.size[1]))

patterns_crop = (args.off_u, args.off_v, args.pattern_width + args.off_u, args.pattern_height + args.off_v)

if args.off_u is not None:
    find = find.crop(patterns_crop)
    matches = [im.crop(patterns_crop) for im in matches]

logging.info("Detecting offsets...")
pt.point = Point(args.start_x, args.start_y)
find_next = False
while pt.point.x != -1:
    if args.scan_all:
        ''' fake match here '''
        if find_next:
            ''' increment counters '''
            pt.point = Point(pt.point.x + 1, pt.point.y)
            if pt.point.x > img.size[0]:
                pt.point = Point(args.start_x, pt.point.y + 1)
                if pt.point.y > img.size[1]:
                    ''' done, bail '''
                    pt.point = Point(-1, -1)
    else:
        pt = img_subimage_find(img, find, pt.point, args.tolerance, find_next)

    ''' Not first time anymore '''
    find_next = 1

    if pt.point.x != -1:
        logging.info("  Found match at {}".format(pt.point))

        ''' Try and identify what thing it is '''
        gotmatch = False
        for tmp_off_x in range(args.sub_width):
            for tmp_off_y in range(args.sub_height):
                if not gotmatch:
                    tmp_pt_x = pt.point.x + args.off_x + tmp_off_x
                    tmp_pt_y = pt.point.y + args.off_y + tmp_off_y
                    for cnt in range(len(matches)):
                        if not gotmatch:
                            logging.info(" Testing for {}  ".format(cnt))
                            logging.info(" {},{} ".format(tmp_pt_x, tmp_pt_y))
                            badness = img_subimage_cmp(img, matches[cnt], Point(tmp_pt_x, tmp_pt_y), args.tolerance)
                            if badness <= args.tolerance:
                                logging.info("  YES")

                                do_output(args.show_badness, badness, tmp_pt_x, tmp_pt_y, cnt)

                                gotmatch = True
                                final_exit = 0

                                ''' Fall out '''
                                break
                            else:
                                logging.info("  NO")

        ''' Notify of no match '''
        if not gotmatch:
            logging.info(" NO ITEMS MATCHED!")
            if not args.scan_all:
                do_output(args.show_badness, pt.badness, pt.point.x, pt.point.y, -1)

exit(final_exit)
