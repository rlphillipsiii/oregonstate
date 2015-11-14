#!/usr/bin/python

from lib import *
import time
import random

def get_input_array(size):
    values = []

    for i in range(size):
        values.append(random.randint(1, size))

    return values

def do_timings(algorithm):
    for size in [1, 2, 3, 4, 5, 10, 20, 30, 40, 50]:
        results = []

        for j in range(10):
            arr = get_input_array(size*1000)

            start = time.time()

            algorithm(arr)
            results.append(time.time()-start)
            
        print '(%d, %f)' % (size, sum(results)/10)

#----------------------------------------------------------#
for algorithm in [brute_force, divide_and_conquer, merge_count]:
    print str(algorithm)

    do_timings(algorithm)
