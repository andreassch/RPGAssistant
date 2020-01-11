#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Dec 31 15:45:09 2019
@author: Andreas Schneider
"""

import xml.etree.ElementTree as ET
from sys import argv
import os


def dsasuiterace2xml(xmlroot, filename, encoding='iso8859-1'):
    with open(filename, 'r') as fd:
        fd.readline()
        race = fd.readline()[:-2].decode(encoding)
        root = ET.SubElement(xmlroot, 'race', attrib={'name': race})
        for ind in range(5):
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
            if len(files) > 0:
                species_name = os.path.basename(path)
                species = ET.SubElement(root, 'species', {'name': species_name})
                for filename in files:
                    dsasuiterace2xml(species, os.path.join(path,filename))
    else:
        filename = argv[1]
        dsasuiterace2xml(filename)
    ET.dump(root)
