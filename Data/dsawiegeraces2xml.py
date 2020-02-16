#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Extract information from race files in DSAWiege's ASCII format to RPGAssistant's XML format.

Created on Tue Dec 31 15:45:09 2019
@author: Andreas Schneider <andreas underscore schn at web dot de>

This script is licensed under the GNU General Public License (GPL) version 3
as published by the Free Software Foundation.
"""

from lxml import etree as ET
from sys import argv
import os


def dsasuiterace2xml(xmlroot, filename, encoding='iso8859-1'):
    with open(filename, 'r') as fd:
        fd.readline()
        race = fd.readline()[:-2].decode(encoding)
        subrace = fd.readline()[:-2].decode(encoding)
        attrib = {'name': race}
        if subrace:
            attrib.update({'subname': subrace})
        root = ET.SubElement(xmlroot, 'race', attrib=attrib)
        for ind in range(4):
            fd.readline()[:-2]
        size = fd.readline()[:-2].decode(encoding)
        size_tag = ET.SubElement(root, 'size')
        size_tag.text = size
        weight_modifier = fd.readline()[:-2].decode(encoding)
        weight_tag = ET.SubElement(root, 'size2weight')
        weight_tag.text = weight_modifier
        haircolor_tag = ET.SubElement(root, 'haircolor')
        for ind in range(20):
            tag = ET.SubElement(haircolor_tag, 'color')
            tag.text = fd.readline()[:-2].decode(encoding)
        eyecolor_tag = ET.SubElement(root, 'eyecolor')
        for ind in range(20):
            tag = ET.SubElement(eyecolor_tag, 'color')
            tag.text = fd.readline()[:-2].decode(encoding)
    return


if __name__ == '__main__':
    if len(argv) < 2:
        print('Usage: {} infile [racename]'.format(os.path.basename(__file__)))
        exit()

    root = ET.Element('races')
    if os.path.isdir(argv[1]):
        for path, subdirs, files in os.walk(argv[1]):
            subdirs.sort() # trick to alphabetically sort subdirectories
            if len(files) > 0:
                species_name = os.path.basename(path)
                species = ET.SubElement(root, 'species', {'name': species_name})
                for filename in sorted(files):
                    dsasuiterace2xml(species, os.path.join(path,filename))
    else:
        filename = argv[1]
        dsasuiterace2xml(filename)
    ET.dump(root)
