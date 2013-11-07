#! /usr/bin/env python
from subprocess import check_call

tests = {
	'do_loop': 5,
	'do_loop_2': -10,
	}

results = {}

for test in tests.keys():
	expect = tests[test]
	results[test]='Pass'
	
	print "Running %s expecting %d" % (test, expect)
	try:
		check_call(["./run.sh", test, str(expect)])
	except (CalledProcessError):
		results[test]='FAIL'
	print('\n\n\n')

print("========== RESULTS =================================")

for test in tests.keys():
	print test, results[test]


