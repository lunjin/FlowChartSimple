#include <string>



class Register
{ 
 private: 

     std::string *values; 
     unsigned size;
     unsigned used;  

 public:

     Register();

     ~Register();

     std::string get(unsigned); 

     unsigned put(std::string); 

     Register & print() const; 
  
} ;

