#include "FCD.h"
#include <iostream>

using namespace std;


int main()
{
  
  FCD start("[start]{}");
  FCD end("[stop]{}");

  FCD c1("[process]{y=0}"); 

  FCD c2("[process]{z=0}"); 

  FCD c3=c1.seqComp(c2); 

  FCD c4("[process]{x=0}");

  FCD c5=c3.ifeComp("$x>0$", c4).whileComp("k==0");

  FCD c6("[process]{skip}"); 
  
  cout << start.seqComp(c5.ifeComp("$z<0$",c6)).seqComp(end).TikZCode();

  return 0;
} 
