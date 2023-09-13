#include <iostream>
#include <string>
#include "string_table.h"

using namespace std;

string_table::string_table(unsigned capacity) 
{ 
  this ->capacity = capacity;
  data = new string [capacity];
  used = 0;
}

string string_table::get(unsigned index)
{ if ((index>=0) && (index<used)) return data[index];
  else return "Invalid identifier number";
}

unsigned string_table::put(string name)
{

  // search table first
  for(unsigned index=0; index<used; index++)
    if (data[index] == name) return  index; // name in table

  // name is not in table  

  if (used == capacity) // table is full
    { // resize the table
	  capacity = 2*capacity +1; 
	  string *newdata = new string[capacity];
          for (unsigned i=0; i<used; i++) newdata[i]=data[i];
	  data = newdata;
    };

   data[used] = name;
    
   return used++; // post increment
}

/**
int main(int argc, char **args){ 

  string_table table(2);

  cout << table.put("one") <<endl;
  cout << table.put("three") <<endl;

  cout << table.put("five") <<endl;
  cout << table.put("four") <<endl;
  cout << table.put("six") <<endl;
  cout << table.put("three") <<endl;

  cout << table.get(2) <<endl;


  cout << table.get(10) <<endl;


}
*/
