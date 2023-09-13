CCC = g++
CCFLAGS= -std=c++11
LEX = flex
LFLAGS= -8     
YACC= bison 
YFLAGS= -d -t -y

RM = /bin/rm -f

cfa: myMatrix.o Register.o CFA.o CFATest.o Link.o
	${CCC} ${CCFLAGS} myMatrix.o Register.o CFA.o Link.o CFATest.o -o cfa

fcd: myMatrix.o Register.o FlowChart.o FlowChartTest.o Link.o
	${CCC} ${CCFLAGS} myMatrix.o Register.o FlowChart.o Link.o FlowChartTest.o -o fcd

Register.o: Register.cpp Register.h
	${CCC} ${CCFLAGS} -c Register.cpp

Link.o: Link.cpp Link.h
	${CCC} ${CCFLAGS} -c Link.cpp

myMatrix.o: myMatrix.cpp myMatrix.h
	${CCC} ${CCFLAGS} -c myMatrix.cpp

FlowChart.o: FlowChart.cpp FlowChart.h
	${CCC} ${CCFLAGS} -c FlowChart.cpp

FlowChartTest.o: FlowChartTest.cpp
	${CCC} ${CCFLAGS} -c FlowChartTest.cpp

CFA.o: CFA.cpp CFA.h
	${CCC} ${CCFLAGS} -c CFA.cpp

CFATest.o: CFATest.cpp
	${CCC} ${CCFLAGS} -c CFATest.cpp

clean:
	${RM}  *.o fcd *~
