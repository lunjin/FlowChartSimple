#include "AST.h"

using namespace std;

AST::AST(string Label)
{ 
  name = Label; 
  children = ""; 
}

string AST::TikZCode() const
{   

 string code = 
   "\\documentclass{article}\n" 
   "\\usepackage{forest}\n"
   "\\begin{document}\n\n"
   "\\begin{forest}\n";

 code.append("[").append(name).append("\n").append(children).append("]"); 

 code.append("\n\\end{forest}\n\\end{document}\n\n");

 return code;

}

AST AST::seqComp(const AST & right) const
{ 
  AST result("{;}"); 
  result.children.append("[").append(name).append("\n").append(children).append("]\n"); 
  result.children.append("[").append(right.name).append("\n").append(right.children).append("]\n"); 
  return result;

}

AST AST::ifeComp(string cond, const AST &right) const // this is the then branch AST
{ 
  AST result("{ife($"+cond+"$)}"); 
  result.children.append("[").append(name).append(",edge label = {node[midway,green,left]{yes}}\n").append(children).append("]\n"); 
  result.children.append("[").append(right.name).append(",edge label = {node[midway,red,right]{no}}\n").append(right.children).append("]\n"); 
  return result;
}
 

AST AST::whileComp(string cond) const
{
  AST result("{while ($"+cond+"$)}");
  result.children.append("[").append(name).append("\n").append(children).append("]\n"); 
  return result;

}
