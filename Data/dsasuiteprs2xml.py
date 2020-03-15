#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Convert person data files in DSASuite's ASCII format to RPGAssistant's XML format.

Copyright (c) 2020 Andreas Schneider (andreas underscore schn at web dot de)

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
import numpy as np
from dsasuitenames2xml import dsasuitename2xml


def readPrsFile(xmlroot, filename, namelist_dir=None, encoding='iso8859-1'):
    with open(filename, 'r') as fd:
        namelist_file = fd.readline().rstrip().decode(encoding)
        region = os.path.splitext(namelist_file)[0]
        prsroot = ET.SubElement(xmlroot, 'person', attrib={'region': region})
        if namelist_dir is not None:
            dsasuitename2xml(prsroot, os.path.join(namelist_dir, namelist_file), encoding)
        size_formula = fd.readline().rstrip().decode(encoding)
        size_tag = ET.SubElement(prsroot, 'size')
        size_tag.text = size_formula
        min_age = int(fd.readline().rstrip().decode(encoding))
        max_age = int(fd.readline().rstrip().decode(encoding))
        age_tag = ET.SubElement(prsroot, 'ageRange', attrib={'min': str(min_age), 'max': str(max_age)})
        terms = dict.fromkeys(['adult','child'])
        for age in terms:
            terms[age] = dict.fromkeys(['male', 'female'])
            for gender in terms[age]:
                terms[age][gender] = fd.readline().rstrip().decode(encoding)
        age_limit_child = int(fd.readline().rstrip().decode(encoding))
        age_limit_elderly = int(fd.readline().rstrip().decode(encoding))
        for age in terms:
            for gender in terms[age]:
                tag = ET.SubElement(prsroot, 'term', attrib={'age': age, 'gender': gender, 'grammaticalGender': 'neutral' if terms[age][gender].endswith(u'ädchen') else gender})
                tag.text = terms[age][gender]
            tag = ET.SubElement(prsroot, 'ageLimit', attrib={'age': age})
            tag.text = str(age_limit_elderly if age=='adult' else age_limit_child)
        haircolors_tag = ET.SubElement(prsroot, 'haircolors')
        no_of_haircolors = int(fd.readline().rstrip().decode(encoding))
        for ind in range(no_of_haircolors):
            weight = int(fd.readline().rstrip().decode(encoding))
            name = fd.readline().rstrip().decode(encoding)
            tag = ET.SubElement(haircolors_tag, 'color', attrib={'weight': str(weight)})
            tag.text = name
        appearances_tag = ET.SubElement(prsroot, 'appearances')
        no_of_appearances = int(fd.readline().rstrip().decode(encoding))
        for ind in range(no_of_appearances):
            weight = int(fd.readline().rstrip().decode(encoding))
            name = fd.readline().rstrip().decode(encoding)
            tag = ET.SubElement(appearances_tag, 'appearance', attrib={'weight': str(weight)})
            tag.text = name
        specials_tag = ET.SubElement(prsroot, 'specialties')
        no_of_specials = int(fd.readline().rstrip().decode(encoding))
        for ind in range(no_of_specials):
            weight = int(fd.readline().rstrip().decode(encoding))
            name = fd.readline().rstrip().decode(encoding)
            tag = ET.SubElement(specials_tag, 'specialty', attrib={'weight': str(weight)})
            tag.text = name
        professions_tag = ET.SubElement(prsroot, 'professions')
        no_of_prefessions = int(fd.readline().rstrip().decode(encoding))
        professions = [None]*no_of_prefessions
        profession_weights = np.zeros(no_of_prefessions, dtype=int)
        for ind in range(no_of_prefessions):
            weight = int(fd.readline().rstrip().decode(encoding))
            name = fd.readline().rstrip().decode(encoding)
            professions[ind] = name
            profession_weights[ind] = weight
        for ind in range(no_of_prefessions):
            num1 = int(fd.readline().rstrip().decode(encoding))
            num2 = int(fd.readline().rstrip().decode(encoding))
            tag = ET.SubElement(professions_tag, 'profession', attrib={'weight': str(profession_weights[ind]), 'value': str(num1)})
            tag.text = professions[ind]
        traits_tag = ET.SubElement(prsroot, 'traits')
        no_of_traits = int(fd.readline().rstrip().decode(encoding))
        for ind in range(no_of_traits):
            weight = int(fd.readline().rstrip().decode(encoding))
            name = fd.readline().rstrip().decode(encoding)
            tag = ET.SubElement(traits_tag, 'trait', attrib={'weight': str(weight)})
            tag.text = name
        if region == 'Elfen':
            beard_percentage = 0
        elif region == 'Zwerge':
            beard_percentage = 100
        else:
            beard_percentage = 66
        beards_tag = ET.SubElement(prsroot, 'beards', attrib={'percentage': str(beard_percentage)})
        no_of_beards = int(fd.readline().rstrip().decode(encoding))
        for ind in range(no_of_beards):
            weight = int(fd.readline().rstrip().decode(encoding))
            name = fd.readline().rstrip().decode(encoding)
            tag = ET.SubElement(beards_tag, 'beard', attrib={'weight': str(weight)})
            tag.text = name
        xval1 = int(fd.readline().rstrip().decode(encoding)) # unknown entry
        xval2 = int(fd.readline().rstrip().decode(encoding)) # unknown entry
    return


if __name__ == '__main__':
    if len(argv) < 2:
        print('Usage: {} file|dir'.format(os.path.basename(__file__)))
        exit()

    root = ET.Element('persons')
    path = argv[1]
    if len(argv) >= 3:
        namelist_dir = argv[2]
    else:
        namelist_dir = None
    if os.path.isdir(path):
        filelist = sorted(glob(os.path.join(path,'*.prs')))
        for filename in filelist:
            readPrsFile(root, filename, namelist_dir=namelist_dir)
    else:
        readPrsFile(root, path, namelist_dir=namelist_dir)
    ET.dump(root)
