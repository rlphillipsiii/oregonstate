#!/usr/bin/python

from lib import *

testers = open('verify.txt', 'r').read().rstrip().split('\n')

print 'BRUTE FORCE (expected, actual):'
for test in testers:
    values = test.split(',')
    answer = int(values[-1])

    calc = brute_force(values[:-1])
    if answer != calc:
        print 'TEST FAIL (%d, %d)' % (answer, calc)
    else:
        print 'TEST PASS (%d, %d)' % (answer, calc)

print '\nDIVIDE AND CONQUER (expected, actual):'
for test in testers:
    values = test.split(',')
    answer = int(values[-1])

    calc = divide_and_conquer(values[:-1])
    if answer != calc:
        print 'TEST FAIL (%d, %d)' % (answer, calc)
    else:
        print 'TEST PASS (%d, %d)' % (answer, calc)

print '\nMERGE (expected, actual):'
for test in testers:
    values = test.split(',')
    answer = int(values[-1])

    calc, arr = merge_count(values[:-1])
    if answer != calc:
        print 'TEST FAIL (%d, %d)' % (answer, calc)
    else:
        print 'TEST PASS (%d, %d)' % (answer, calc)
