test:
	gcc graph.h
	bison -d bison.y
	flex flex.l
	gcc bison.tab.c -lm lex.yy.c -lm -o ac_circuit_solver
