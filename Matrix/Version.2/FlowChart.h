#include <string>
#include <list>
#include "myMatrix.h"
#include "Register.h"
#include <sstream>
#include <iostream>
#include <iomanip>


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



class FCD 
{
 private: 
    myMatrix nodeMatrix; 
    int axis; //entry point is at  row 0 and column axis and exit is at bottom row and colun asix
    std::list<Link> links; // links of the FCD 
    std::list<std::pair<int,int>> exits;  
    static Register registrar; // global data structure;



  public: 
   
    FCD(std::string Label); // create a simple node chart with a given label 
    FCD(); // create the empty chart 

    void TikZCode() const;  // output code for drawing TikZ tree

    FCD seqComp(const FCD &) const; // this is the FCD for left statement

    FCD ifeComp(std::string cond,  const FCD &right) const; //  this is FCD for the then branch statement

    FCD whileComp(std::string cond) const;  // this is FCD for the body statement

    int getExitNode() const;
    int getEntryNode() const;

} ; 


