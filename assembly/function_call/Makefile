# ------------------------
runfunction_call: function_call
	./function_call

function_call: function_call.o 
	ld function_call.o -o function_call

function_call.o: function_call.s
	as function_call.s -o function_call.o

# --------------------------
clean:
	\rm -f function_call.o function_call

