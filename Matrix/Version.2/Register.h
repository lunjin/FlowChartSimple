#include <string>



class Register
{ 
 private: 

     std::string **values; 
     unsigned size;
     unsigned used;  
     // location 0 is not used since 0 is not a valid node number. 
 public:

     Register();

     ~Register();

     std::string get(unsigned); 

     unsigned put(std::string); 

     Register & print() const; 
  
} ;

