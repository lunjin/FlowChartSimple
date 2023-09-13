#include "Register.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std; 

Register::Register():
  size(1), used(0)
{ 
  values = new int[1];
}

Register::~Register()
{
  delete values; 
}

unsigned Register::put(int  content)
{ 
  if (used == size) 
    { 
      int * newvalues = new int [size*2+1];
	std::copy(values, values+size,newvalues);
        delete values;
        values = newvalues;
        size = size*2+1;
    };
  values[used]= content;
  return used ++;  
}

int Register::get(unsigned key)
{ 
  if (key<0 || key>=used) cout<< "Key out of bound\n\n";
 
  return values[key]; 
}


Register &  Register::print() const
{
  for(int r =0; r<used; r++) 
    {  cout << std::setw(10) << values[r]; 
    }
    cout << endl; 
}



int main() 
{ Register m;

  m.put(1);
  m.put(2);
  m.put(3);

  m.print();

  cout << endl << endl;

  m.put(4);
  m.put(5);
  m.put(6);

  m.print();

}
