#include <string>



class Register
{ 
 private: 

     int *values; 
     unsigned size;
     unsigned used;  

 public:

     Register();

     ~Register();

     int get(unsigned); 

     unsigned put(int); 

     Register & print() const; 
  
} ;

