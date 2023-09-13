#ifndef LINKDEF

#define LINKDEF

#include <string> 

class Link { 
   private :
      unsigned source, sink;
      std::string source_pos, sink_pos; 
      std::string arrowtip; 
      std::string desc; // link description including label 
    public:

      Link(unsigned, unsigned, std::string, std::string);
      Link(unsigned, std::string, unsigned, std::string, std::string, std::string);
      unsigned getSource() const;
      unsigned getSink() const;
      std::string getDesc() const;       
      std::string getArrowTip() const;       

      std::string toString() const;

}  ;


#endif
