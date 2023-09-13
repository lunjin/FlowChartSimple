CCC = g++
CCFLAGS=  -std=c++11
LEX = flex
LFLAGS= -8     
YACC= bison 
YFLAGS= -d -t -y
COMPONENTS =  y.tab.o lex.yy.o imp.o string_table.o Matrix/CFA.o Matrix/FCD.o Matrix/myMatrix.o Matrix/Register.o Matrix/Link.o Matrix/AST.o


RM = /bin/rm -f

imp: ${COMPONENTS}
	${CCC} ${CCFLAGS} ${COMPONENTS} -o imp

Matrix/Register.o: Matrix/Register.cpp Matrix/Register.h
	${CCC} ${CCFLAGS} -c Matrix/Register.cpp -o Matrix/Register.o


Matrix/Link.o: Matrix/Link.cpp Matrix/Link.h
	${CCC} ${CCFLAGS} -c Matrix/Link.cpp -o Matrix/Link.o

Matrix/myMatrix.o: Matrix/myMatrix.cpp Matrix/myMatrix.h
	${CCC} ${CCFLAGS} -c Matrix/myMatrix.cpp -o Matrix/myMatrix.o

Matrix/FCD.o: Matrix/FCD.cpp Matrix/FCD.h
	${CCC} ${CCFLAGS} -c Matrix/FCD.cpp -o Matrix/FCD.o

Matrix/AST.o: Matrix/AST.cpp Matrix/AST.h
	${CCC} ${CCFLAGS} -c Matrix/AST.cpp -o Matrix/AST.o

Matrix/CFA.o: Matrix/CFA.cpp Matrix/CFA.h
	${CCC} ${CCFLAGS} -c Matrix/CFA.cpp -o Matrix/CFA.o

string_table.o: string_table.cpp string_table.h
	${CCC} ${CCFLAGS} -c string_table.cpp
imp.o: imp.cpp imp.h
	${CCC}  ${CCFLAGS} -c imp.cpp
y.tab.o: imp.yacc
	${YACC} ${YFLAGS} imp.yacc
	${CCC} ${CCFLAGS} y.tab.c -c 

lex.yy.o: imp.lex
	${LEX} $(LFLAGS) imp.lex
	${CCC} ${CCFLAGS} lex.yy.c -c 

clean:
	/bin/rm -f lex.yy.* y.tab.* *.o imp *~ *.AST.* *.FCD.* *.CFA.*
