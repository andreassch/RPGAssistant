#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Convert holiday list in DSASuite's ASCII format to RPGAssistant's XML format.

Created on Sun Jan 26 20:44:23 2020
@author: Andreas Schneider <andreas underscore schn at web dot de>

This script is licensed under the GNU General Public License (GPL) version 3
as published by the Free Software Foundation.
"""

from lxml import etree as ET
from sys import argv
import os
from glob import glob
from enum import IntEnum

class HolidayReference(IntEnum):
    REF_MONTH = 0
    REF_WEEKDAY_FIRST = 1
    REF_WEEKDAY_LAST = 2

class HolidayDurationReference(IntEnum):
    END_DATE = 0
    DURATION_BACKWARD = 1
    DURATION_FORWARD = 2

def dsasuiteholiday2xml(xmlroot, filename, encoding='iso8859-1'):
    references = ['month', 'firstWeekday', 'lastWeekday']
    duration_types = ['endDate','durationBack', 'duration']
    with open(filename, 'r') as fd:
        while True:
            name = fd.readline().decode(encoding).rstrip()
            if not name:
                break
            day_reference = HolidayReference(int(fd.readline()))
            end_type = HolidayDurationReference(int(fd.readline()))
            day_start = int(fd.readline())
            day_end = int(fd.readline())
            month_start = int(fd.readline())
            month_end = int(fd.readline())
            fd.readline()
            fd.readline()
            elem_holiday = ET.SubElement(xmlroot, 'holiday', attrib={'name': name, 'durationType': 'backward' if end_type==HolidayDurationReference.DURATION_BACKWARD else 'forward'})
            day_attrib = {'reference': references[day_reference]}
            if day_reference > 0:
                day_attrib.update({'number': str(1)})
            elem_day = ET.SubElement(elem_holiday, 'day', attrib=day_attrib)
            elem_day.text = str(day_start if day_reference==0 else (day_start+2)%7+1) # DSASuite has different first day of the week than RPGAssistant (Praiosday vs. Windsday).
            elem_month = ET.SubElement(elem_holiday, 'month')
            elem_month.text = str(month_start)
            if end_type==HolidayDurationReference.END_DATE:
                duration = day_end - day_start + 1 + 30*(month_end - month_start) # Compute duration, assuming that holidays do not extend over the nameless days.
                elem_end = ET.SubElement(elem_holiday, 'duration')
                elem_end.text = str(duration)
            else:
                elem_end = ET.SubElement(elem_holiday, 'duration')
                elem_end.text = str(day_end)
    return

if __name__ == '__main__':
    root = ET.Element('holidays')
    dsasuiteholiday2xml(root, argv[1])
    ET.dump(root)
