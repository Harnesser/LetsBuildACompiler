#! /usr/bin/env python
from subprocess import check_call
from subprocess import CalledProcessError

tests = {
	'assign': 36,
	'if_else': -5,
	'if_else_newlines': -5,
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
	print "%-25s %s" % (test, results[test])


