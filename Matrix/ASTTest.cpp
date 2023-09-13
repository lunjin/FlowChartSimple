#include "AST.h"
#include <iostream>

using namespace std;


int main()
{
  
  AST c1("{y:=0}"); 

  AST c2("{z:=0}"); 

  AST c3=c1.seqComp(c2); 

  AST c4("{x:=0}");

  AST c5=c3.ifeComp("x>0", c4).whileComp("k=0");

  AST c6("skip"); 
  
  cout << c5.ifeComp("z<0",c6).TikZCode();

  return 0;
} 
