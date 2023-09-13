CCC = g++
CCFLAGS= -std=c++11
LEX = flex
LFLAGS= -8     
YACC= bison 
YFLAGS= -d -t -y

RM = /bin/rm -f


ast:  AST.o ASTTest.o
	${CCC} ${CCFLAGS} AST.o  ASTTest.o -o ast

cfa: myMatrix.o Register.o CFA.o CFATest.o Link.o
	${CCC} ${CCFLAGS} myMatrix.o Register.o CFA.o Link.o CFATest.o -o cfa

fcd: myMatrix.o Register.o FCD.o FCDTest.o Link.o
	${CCC} ${CCFLAGS} myMatrix.o Register.o FCD.o Link.o FCDTest.o -o fcd

Register.o: Register.cpp Register.h
	${CCC} ${CCFLAGS} -c Register.cpp

Link.o: Link.cpp Link.h
	${CCC} ${CCFLAGS} -c Link.cpp

myMatrix.o: myMatrix.cpp myMatrix.h
	${CCC} ${CCFLAGS} -c myMatrix.cpp

FCD.o: FCD.cpp FCD.h
	${CCC} ${CCFLAGS} -c FCD.cpp

FCDTest.o: FCDTest.cpp
	${CCC} ${CCFLAGS} -c FCDTest.cpp

CFA.o: CFA.cpp CFA.h
	${CCC} ${CCFLAGS} -c CFA.cpp

CFATest.o: CFATest.cpp
	${CCC} ${CCFLAGS} -c CFATest.cpp

AST.o: AST.cpp AST.h
	${CCC} ${CCFLAGS} -c AST.cpp

ASTTest.o: ASTTest.cpp
	${CCC} ${CCFLAGS} -c ASTTest.cpp

clean:
	${RM}  *.o fcd ast cfa *~
