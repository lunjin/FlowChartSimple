#include <string> 

class Link { 
   protected :
      unsigned source, sink;
      std::string arrowtip; 
      std::string desc; // link description including label 
    public:

      Link(unsigned, unsigned, std::string, std::string);
      unsigned getSource() const;
      unsigned getSink() const;
      std::string getDesc() const;       
      std::string getArrowTip() const;       

      std::string toString() const;

}  ;


