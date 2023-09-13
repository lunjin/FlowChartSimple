#include <string>
#include <list>
#include <sstream>
#include <iostream>
#include <iomanip>

class AST 
{
 private: 
  std::string name; 
  std::string children; // concatenation of string representations of children

  public: 
   
    AST(std::string Label); // create a simple node chart with a given label 

    std::string TikZCode() const;  // output code for drawing TikZ tree

    AST seqComp(const AST &) const; // this is the AST for left statement

    AST ifeComp(std::string cond,  const AST &right) const; //  this is AST for the then branch statement

    AST whileComp(std::string cond) const;  // this is AST for the body statement

} ; 


