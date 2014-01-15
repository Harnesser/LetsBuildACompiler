#! /usr/bin/env python
import subprocess

tests = [
	( "10 + 4*2", 18),
	( "3 + 1 * 2 * 4 + 5", 16),
	( "10 - 2 - 1", 7),
	( "-3 - -6", 3),
	( "3 -2 + 4 * -5", -19),
	( "2^3", 8 ),
	( "2^3^2", 512),
	( "( 10 + 4) *2", 28),
	( "( (3 + 1) * 2 * (4 + 5) )", 72),
	( "123 + 345", 468),
]	

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
	print "  WANTED:" , expected, " GOT ", res
	if res == expected:
		has_error = 0
	else:
		subprocess.call(['cat result.log'], shell=True)
	print "-" * 80

	return has_error 
	
if __name__ == '__main__':
	ctests = 0
	cfails = 0
	for program, expected in tests:
		ctests += 1
		cfails += check_expression(program, expected)
	print "%d tests, %d fails" % (ctests, cfails)
