#! /usr/bin/env python
from subprocess import check_call
from subprocess import CalledProcessError

KTRUE = -1
KFALSE = 0

tests = {
	'program': 0,
	'topdecls': 0,
	'vars_initialised': 0,
	'var_list': 0,
	'var_negatives': 0,
	'var_double_decl': 127,
	'assignments': 3,
	'assignments_and_expressions': 2,
    'assignments_boolean_01': KFALSE, # -100 > 2
    'assignments_boolean_02': KFALSE, # -100 == 2
    'assignments_boolean_03': KTRUE,  # -100 < 2
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
	print "%-30s %s" % (test, results[test])


