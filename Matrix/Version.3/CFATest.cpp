#include "CFA.h"
#include <iostream>

using namespace std;


int main()
{
  
  CFA start("");
  CFA end("");

  CFA c1("{y=0}"); 

  CFA c2("{z=0}"); 

  CFA c3=c1.seqComp(c2); 

  CFA c4("{x=0}");

  CFA c5=c3.ifeComp("x>0", c4).whileComp("k==0");

  CFA c6("{skip}"); 
  
  cout << start.seqComp(c5.ifeComp("z<0",c6)).seqComp(end).TikZCode();

  return 0;
} 
