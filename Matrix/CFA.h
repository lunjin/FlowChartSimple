#include <string>
#include <list>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "myMatrix.h"
#include "Register.h"
#include "Link.h"


typedef struct  HungLink  {
  int node_n, col;
  std::string label; 
HungLink(int nn, int cn, std::string l): node_n(nn), col(cn), label(l) 
  {
  }
} HungLink; 

class CFA 
{
 private: 
    myMatrix nodeMatrix; 
    int axis; //entry point is at  row 0 and column axis and exit is at bottom row and colun asix
    std::list<Link> links; // links of the CFA 
    std::list<HungLink> hungLinks;  
    static Register registrar; // global data structure;



  public: 
   
    CFA(std::string Label); // create a simple CFA for a given action 
    CFA(); // create the empty CFA

    std::string TikZCode() const;  // output code for drawing TikZ tree

    CFA seqComp(const CFA &) const; // this is the CFA for left statement

    CFA ifeComp(std::string cond,  const CFA &right) const; //  this is CFA for the then branch statement

    CFA whileComp(std::string cond) const;  // this is CFA for the body statement

    int getEntryNode() const;

} ; 


