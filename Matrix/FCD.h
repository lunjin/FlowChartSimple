#include <string>
#include <list>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "myMatrix.h"
#include "Register.h"
#include "Link.h"


class FCD 
{
 private: 
    myMatrix nodeMatrix; 
    int axis; //entry point is at  row 0 and column axis 
    std::list<Link> links; // links of the FCD 
    std::list<std::pair<int,int> > exits;  // pair of (exit node number, column in nodeMatrix)
    static Register registrar; // global data structure;



  public: 
   
    FCD(std::string Label); // create a simple node chart with a given label 
    FCD(); // create the empty chart 

    std::string TikZCode() const;  // output code for drawing TikZ tree

    FCD seqComp(const FCD &) const; // this is the FCD for left statement

    FCD ifeComp(std::string cond,  const FCD &right) const; //  this is FCD for the then branch statement

    FCD whileComp(std::string cond) const;  // this is FCD for the body statement

    int getEntryNode() const;

} ; 


