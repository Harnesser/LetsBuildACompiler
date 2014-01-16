#! /usr/bin/env python
import subprocess

C_RED = "\033[31m"
C_GREEN = "\033[32m"
C_YELLOW = "\033[33m"
C_OFF = "\033[0m"

tests = [
	( "123", 123),
	( "-3", -3),
	( "123 + 345", 468),
	( "10 * 5", 50),
	( "10 + 4*2", 18),
	( "3 + 1 * 2 * 4 + 5", 16),
	( "10 - 2 - 1", 7),
	( "-3 - -6", 3),
	( "3 -2 + 4 * -5", -19),
	( "2^3", 8 ),
	( "2^3^2", 512),
	( "( 10 + 4) *2", 28),
	( "( (3 + 1) * 2 * (4 + 5) )", 72),
#	( "( (4+2) )))", 34),
]	

#tests = tests[0:-1]

def check_expression(program, expected):
	""" Run and check the result. """
	has_error = 1
	hPROG = open('expression.txt', 'w')
	hPROG.write(program)
	hPROG.close()
	subprocess.call(['$HOME/Projects/LetsBuildACompiler/TDOP/C/main < expression.txt > result.log'], shell=True)

	hRES = open('result.log')
	res = None
	for line in hRES:
		if not line.startswith('RESULT:'):
			continue
		a,b = line.split()
		res = int(b)
		break
	hRES.close()

	print "PROGRAM:", program
	if res == expected:
		has_error = 0
		print C_GREEN, "  WANTED:" , expected, " GOT ", res, C_OFF
	else:
		print C_RED, "  WANTED:" , expected, " GOT ", res, C_OFF
		print C_YELLOW
		subprocess.call(['cat result.log'], shell=True)
		print C_OFF
	print "-" * 80

	return has_error 
	
if __name__ == '__main__':
	ctests = 0
	cfails = 0
	for program, expected in tests:
		ctests += 1
		cfails += check_expression(program, expected)
	print "%d tests, %d fails" % (ctests, cfails)
