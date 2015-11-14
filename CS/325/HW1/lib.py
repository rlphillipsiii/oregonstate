#!/usr/bin/python

import math

debug = False

def ceil(num):
    return int(math.ceil(num))

def floor(num):
    return int(math.ceil(num))

def brute_force(values):
    count = 0
    
    for i in range(len(values)):
        for j in range(i, len(values)):
            if (int(values[i]) > int(values[j])):
                if debug:
                    print '(%s, %s)' % (values[i], values[j])
                    
                count += 1

    return count

def divide_and_conquer(values):
    return _divide_and_conquer(values, 0)

def _divide_and_conquer(values, count):
    size = len(values)

    if size == 1:
        return count

    count = _divide_and_conquer(values[:ceil(size/2)], count)
    count = _divide_and_conquer(values[floor(size/2):size], count)

    for i in range(floor(size/2)):
        for j in range(ceil(size/2), size):
            if int(values[i]) > int(values[j]):
                count += 1

    return count
    
def merge_count(values):
    if len(values) == 1:
        return (0, values)

    lcount, left = merge_count(values[:len(values)/2])
    rcount, right = merge_count(values[len(values)/2:])

    count = rcount+lcount

    rindex = 0
    lindex = 0

    merged = []
    while len(left) > lindex or len(right) > rindex:
        if len(right) == rindex:
            merged.append(left[lindex])
            lindex += 1
        elif len(left) == lindex:
            merged.append(right[rindex])
            rindex += 1
        elif int(left[lindex]) < int(right[rindex]):
            merged.append(left[lindex])
            lindex += 1
        else:
            count += len(left)-lindex

            merged.append(right[rindex])
            rindex += 1

    return (count, merged)
