#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec 31 14:08:25 2019
@author: Andreas Schneider
"""

import xml.etree.ElementTree as ET
from sys import argv
import os
from glob import glob


def dsasuitename2xml(xmlroot, filename, encoding='iso8859-1'):
    region = os.path.splitext(os.path.basename(filename))[0]
    namelist = ET.SubElement(xmlroot, 'namelist', attrib={'region': region})
    cat = None
    with open(filename, 'r') as fd:
        for line in fd:
            if line.endswith('\n'):
                line = line[:-1]
            if line.endswith('\r'):
                line = line[:-1]
            if line.startswith('---'): # start of new category
                category = line[3:-3]
                if category == 'Nachnamen':
                    cat = ET.SubElement(namelist, 'surnames')
                elif category == 'MVornamen':
                    cat = ET.SubElement(namelist, 'firstnames', attrib={'gender': 'male'})
                elif category == 'WVornamen':
                    cat = ET.SubElement(namelist, 'firstnames', attrib={'gender': 'female'})
                else:
                    cat = ET.SubElement(namelist, category.decode(encoding))
            else: # it's a name
                if cat is None:
                    return False
                elem = ET.SubElement(cat, 'n')
                elem.text = line.decode(encoding)
    return True


if __name__ == '__main__':
    if len(argv) < 2:
        print('Usage: {} file|dir'.format(os.path.basename(__file__)))
        exit()

    root = ET.Element('namelists')
    if os.path.isdir(argv[1]):
        filelist = sorted(glob(os.path.join(argv[1],'*.dat')))
        for filename in filelist:
            success = dsasuitename2xml(root, filename)
    else:
        dsasuitename2xml(root, argv[1])
    ET.dump(root)
