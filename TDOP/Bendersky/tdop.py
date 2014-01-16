#! /usr/bin/env python
""" TDOP expression parsing example. """

import re

""" Scanner """
token_pat = re.compile("\s*(?:(\d+)|(.))")

def tokenize(program):
	for number, operator in token_pat.findall(program):
		if number:
			yield literal_token(number)
		elif operator == '+':
			yield operator_add_token()
		elif operator == '-':
			yield operator_sub_token()
		elif operator == '*':
			yield operator_mul_token()
		elif operator == '^':
			yield operator_pow_token()
		elif operator == '(':
			yield operator_lparen_token()
		elif operator == ')':
			yield operator_rparen_token()
		else:
			msg = "unknown operator '%s'" % (operator) 
			raise SyntaxError(msg)
	yield end_token()

def parse(program):
	global token, next, indent
	print("Program: %s" % program)
	next = tokenize(program).next
	token = next()
	indent = 0
	return expression()

def match(tok=None):
	global token
	print type(token)
	if tok and tok != type(token):
		msg = "expected '%s'" % (tok)
		raise SyntaxError(msg)
	token = next()

def mesg(message):
	global indent
	print (' ' * indent ) + '<< ' + message + ' >>'


""" Main parser bit """
def expression(rbp=0):
	global token, indent
	mesg("expression with rbp %d" % rbp)
	indent += 2

	t = token
	token = next()
	left = t.nud()
	while rbp < token.lbp:
		t = token
		token = next()
		left = t.led(left)
	indent -= 2
	return left
	

class literal_token(object):
	def __init__(self, value):
		self.value = int(value)
	def nud(self):
		mesg("literal nud = %d" % self.value)
		return self.value

class operator_add_token(object):
	lbp = 10
	def led(self, left):
		mesg("led of '+'")
		right = expression(10)
		return left + right

class operator_mul_token(object):
	lbp = 20
	def led(self, left):
		return left * expression(20)

class operator_sub_token(object):
	lbp = 10
	def nud(self):
		mesg("nud of '-'")
		return -expression(100)
	def led(self, left):
		mesg("led of '-'")
		return left - expression(10)

class operator_pow_token(object):
	""" Right-associative.
	Want parser to treat subsequent exponential operators as
	sub-expressions of the first one. So call expression() 
	with a rbp lower that the lbp of the exponential.

	When expression() gets to next '^', it finds that still
	rbp < token.lbp and won't return the result right away.
	but collects the value of the sub-expression first.
	"""
	lbp = 30
	def led(self, left):
		mesg("led of '^'")
		return left ** expression(30-1)

class operator_lparen_token(object):
	lbp = 0
	def nud(self):
		mesg("nud of '('")
		expr = expression()
		match(operator_rparen_token)
		return expr

class operator_rparen_token(object):
	lbp = 0

class end_token(object):
	lbp = 0

def check_expression(program, expected):
	print "PROGRAM: %s" % (program)
	value = parse(program)
	print "VALUE: %d" % (value)
	if value != expected:
		print "FAIL!!!"
		return 1
	else:
		return 0

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

if __name__ == '__main__':
	ctests = 0
	cfails = 0
	for program, expected in tests:
		ctests += 1
		cfails += check_expression(program, expected)
	print "%d tests, %d fails" % (ctests, cfails)
