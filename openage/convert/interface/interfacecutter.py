# Copyright 2016-2016 the openage authors. See copying.md for legal info.

""" Cutting some user interface assets into subtextures """

import os
import re
import tempfile
from subprocess import Popen, PIPE

from ..texture import TextureImage

from .interfacehardcoded import *

class InterfaceCutter(object):
    """
    Callable object that cuts interface textures.
    """

    def __init__(self, filename):
        self.filename = filename

    def __call__(self, image):
        if is_ingame_hud_background(self.filename):
            pil_image = image.get_pil_image()

            yield self.__cut_top_strip(pil_image)

            for coords in KNOWN_SUBTEXTURE_CORNER_COORDS:
                yield TextureImage(pil_image.crop(coords))
        else:
            yield image

    def __cut_top_strip(self, pil_image):
        """
        Finds a horisontally tilable piece of the strip that's at the top of the HUD.
        """

        try:
            uncut = tempfile.NamedTemporaryFile(delete=False)
            uncut.close()

            pil_image.save(uncut.name, "PNG")

            seamless_top_strip_tile_search = Popen([
                'python',
                os.path.join(os.path.dirname(os.path.realpath(__file__)), 'visgrep.py'),
                '-J', str(TOP_STRIP_PATTERN_XYWH[1] + TOP_STRIP_PATTERN_XYWH[3]),
                '-u', str(TOP_STRIP_PATTERN_XYWH[0]),
                '-v', str(TOP_STRIP_PATTERN_XYWH[1]),
                '-M', str(TOP_STRIP_PATTERN_XYWH[2]),
                '-N', str(TOP_STRIP_PATTERN_XYWH[3]),
                '-W', str(TOP_STRIP_PATTERN_XYWH[2]),
                '-H', str(TOP_STRIP_PATTERN_XYWH[3]),
                '-t', '100000',
                uncut.name, uncut.name, uncut.name], stdout=PIPE)

            search_res = seamless_top_strip_tile_search.communicate()[0]

            if seamless_top_strip_tile_search.returncode != 0:
                raise Exception("visgrep failed to start analyzing '%s'\n" % self.filename)

            search_res_list = search_res.splitlines()

            if len(search_res_list) <= 1:
                raise Exception("visgrep failed to find repeating pattern (%s)\n" % self.filename)

            xy_repeat_starts = [int(x.group()) for x in re.finditer(br"\d+", search_res_list[-1])]

            if len(xy_repeat_starts) < 2 or xy_repeat_starts[1] != TOP_STRIP_PATTERN_XYWH[1]:
                raise Exception("visgrep failed to find repeating pattern (%s)\n" % self.filename)

            return TextureImage(pil_image.crop((
                TOP_STRIP_PATTERN_XYWH[0],
                TOP_STRIP_PATTERN_XYWH[1],
                xy_repeat_starts[0],
                TOP_STRIP_PATTERN_XYWH[1] + TOP_STRIP_PATTERN_XYWH[3]
            )))

        finally:
            os.remove(uncut.name)
