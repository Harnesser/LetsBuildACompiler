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
		elif operator == '*':
			yield operator_mul_token()
		else:
			raise SyntaxError('uknown operator %s' % operator)
	yield end_token()

def parse(program):
	global token, next
	next = tokenize(program).next
	token = next()
	return expression()


""" Main parser bit """
def expression(rbp=0):
	global token
	t = token
	token = next()
	left = t.nud()
	while rbp < token.lbp:
		t = token
		token = next()
		left = t.led(left)
	return left

class literal_token(object):
	def __init__(self, value):
		self.value = int(value)
	def nud(self):
		return self.value

class operator_add_token(object):
	lbp = 10
	def led(self, left):
		right = expression(10)
		return left + right

class operator_mul_token(object):
	lbp = 20
	def led(self, left):
		return left * expression(20)

class end_token(object):
	lbp = 0


if __name__ == '__main__':
	print parse("10 + 4*2")
	print parse("3 + 1 * 2 * 4 + 5")
	
