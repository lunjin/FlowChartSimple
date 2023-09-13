#include "Register.h"
#include <iostream>
#include <iomanip>


using namespace std; 

Register::Register():
  size(2), used(1)
{ // first location 0  is not used since 0 is not a valid node number 
  values = new string *[2];
  values[0] = new string(" "); 
}

Register::~Register()
{
  for(int r =0; r<used; r++) 
    {  delete values[r]; 
    }
  delete values; 
}

unsigned Register::put(string  content)
{ 
  if (used == size) 
    { 
        string ** newvalues = new string * [size*2+1];
	std::copy(values, values+size,newvalues);
        delete values;
        values = newvalues;
        size = size*2+1;
    };
  values[used]= new string(content);
  return used ++;  
}

string Register::get(unsigned key)
{ 
  if (key<0 || key>=used) cout<< "Key out of bound\n\n";
 
  if (key == 0) return " "; 
  else  return "\\node(vert"+to_string(key) + ")" + *(values[key]); 
}


Register &  Register::print() const
{
  for(int r =0; r<used; r++) 
    {  cout << std::setw(10) << *(values[r]); 
    }
    cout << endl; 
}



/*
int main() 
{ Register m;

  m.put("1");
  m.put("2");
  m.put("3");

  m.print();

  cout << endl << endl;

  m.put("4");
  m.put("5");
  m.put("6");

  m.print();

}
*/
