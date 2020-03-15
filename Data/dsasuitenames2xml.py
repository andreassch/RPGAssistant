#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Convert name lists in DSASuite's ASCII format (with some extensions) to RPGAssistant's XML format.

Copyright (c) 2019, 2020 Andreas Schneider (andreas underscore schn at web dot de)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
"""

from lxml import etree as ET
from sys import argv
import os
from glob import glob


def dsasuitename2xml(xmlroot, filename, encoding='iso8859-1'):
    region = os.path.splitext(os.path.basename(filename))[0]
    namelist = ET.SubElement(xmlroot, 'namelist', attrib={'region': region})
    cat = None
    with open(filename, 'r') as fd:
        for line in fd:
            line = line.decode(encoding).rstrip()
            if line.startswith('---'): # start of new category
                category = line[3:-3]
                if category == 'Nachnamen':
                    cat = ET.SubElement(namelist, 'surnames', attrib={'type': 'none'})
                elif category == 'MVornamen':
                    cat = ET.SubElement(namelist, 'firstnames', attrib={'gender': 'male'})
                elif category == 'WVornamen':
                    cat = ET.SubElement(namelist, 'firstnames', attrib={'gender': 'female'})
                else:
                    cat = ET.SubElement(namelist, category)
                subcat = cat
            elif line.startswith('--'): # start of new subcategory
                subcategory = line[2:-2]
                if subcategory == 'Startteil':
                    subcat = ET.SubElement(cat, 'namePart', attrib={'place': 'beginning'})
                    cat.attrib['type'] = 'parts'
                elif subcategory == 'Endteil':
                    subcat = ET.SubElement(cat, 'namePart', attrib={'place': 'end'})
                    cat.attrib['type'] = 'parts'
                else:
                    subcat = ET.SubElement(namelist, subcategory)
            elif line.startswith('*prefix'):
                elem = ET.SubElement(cat, 'prefix', attrib={'gender': 'male' if line[7]=='M' else 'female'})
                elem.text = line[9:]
                cat.attrib['type'] = 'parentname'
            elif line.startswith('*postfix'):
                elem = ET.SubElement(cat, 'postfix', attrib={'gender': 'male' if line[8]=='M' else 'female'})
                elem.text = line[10:]
                cat.attrib['type'] = 'parentname'
            else: # it's a name
                if cat is None:
                    return False
                if subcat.tag == 'surnames': # if no subcategory is present
                    cat.attrib['type'] = 'list'
                if ', ' in line:
                    names = line.split(', ')
                else:
                    names = [line]
                for name in names:
                    if ('place' in subcat.attrib) and (subcat.attrib['place'] == 'end') and name.startswith('-'):
                        name = name[1:] # remove leading dash in end parts
                    elem = ET.SubElement(subcat, 'n')
                    elem.text = name
    return True


if __name__ == '__main__':
    if len(argv) < 2:
        print('Usage: {} file|dir'.format(os.path.basename(__file__)))
        exit()

    root = ET.Element('namelists')
    for path in argv[1:]:
        if os.path.isdir(path):
            filelist = sorted(glob(os.path.join(path,'*.dat')))
            for filename in filelist:
                success = dsasuitename2xml(root, filename)
        else:
            dsasuitename2xml(root, path)
    ET.dump(root)
