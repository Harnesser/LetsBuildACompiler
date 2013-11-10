#! /usr/bin/env python
from subprocess import check_call
from subprocess import CalledProcessError

C_RED = "\033[31m"
C_OFF = "\033[0m"

KTRUE = -1
KFALSE = 0

tests = {
	'program': 0,
	}

results = {}

for test in tests.keys():
	expect = tests[test]
	results[test]='Pass'
	
	print "Running %s expecting %d" % (test, expect)
	try:
		check_call(["./run.sh", test, str(expect)])
	except(CalledProcessError):
		results[test]='FAIL'

	print('\n\n\n')

print("========== RESULTS =================================")

for test in tests.keys():
    if results[test] == 'FAIL':
        colour = C_RED
    else:
        colour = ''
    print "%s%-30s %s %s" % (colour, test, results[test], C_OFF)


