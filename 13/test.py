#! /usr/bin/env python
from subprocess import check_call
from subprocess import CalledProcessError

C_RED = "\033[31m"
C_OFF = "\033[0m"

KTRUE = -1
KFALSE = 0

tests = {
	'program_empty_no_procs': 0,
	'var_declarations': 0,
	'assignment_simple': 100,
	'assignment_several': 100,
	'if': 100,
	'if_else': 37,
	'while_accum_whitespace': 45,
	'while_accum_whitespace_optsemi': 45,
	'multichar_symbols': 21,
	'multichar_relops': -45,
	'write_numbers': -8, # not checked properly 
	'write_m15_to_15': 0, # again, need to look at log
	'maths_multiply' : -40,
	'maths_divide' : -5,
	'while_accum_whitespace_comments': 45,
	'assignment_several_comments': 100,

	'program_empty_proc_empty.txt': 0,
	'program_empty_small_proc.txt': 0,
	'procedure_basic.txt': 3,
	'procedure_param_list.txt': 9,
	}

# these tests:
iotests = {
	'read_incr5:23'  : 28,
	'read_incr5:-4'  : 1,
#	'read3sum:1 2 3' : 6,  # can't get 3 inputs going...
}
results = {}

for test in iotests.keys():
	testname,input_ = test.split(':')
	expect = iotests[test]
	results[test]='Pass'

	arg = '%s' % (input_)
	print "Running IO %s %s expecting %d" % (
	    testname, input_, expect)
	try:
		check_call(["./run.sh", testname, str(expect), arg])
	except(CalledProcessError):
		results[test]='FAIL'
	print('\n\n\n')


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

all_tests = iotests.keys()
all_tests.extend(tests.keys())
for test in all_tests:
    if results[test] == 'FAIL':
        colour = C_RED
    else:
        colour = ''
    print "%s%-35s %s %s" % (colour, test, results[test], C_OFF)


