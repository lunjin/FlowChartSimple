#include "CFA.h"

using namespace std;

Register CFA::registrar = Register(); 


CFA::CFA()
{ nodeMatrix = myMatrix(1,1); 
  axis = 0; 
  links = list<Link>(); 
  exits = list<pair<int,int>>();
}

CFA::CFA(string Label)
{ 
  unsigned sn = CFA::registrar.put("s"); // a state
  unsigned pn = CFA::registrar.put(""); // a point

  axis = 0; 
  nodeMatrix=myMatrix(2,1);
  nodeMatrix(0,0) = sn;
  nodeMatrix(1,0) = pn;

  links = list<Link>();
  links.push_back(Link(sn,pn,"->", "-- node[sloped, left]{" + Label + "}"));  
  exits = list<pair<int,int>>();
  exits.push_back(pair<int,int>(pn,0));
  
}

string CFA::TikZCode() const
{   
  int n_rows =   nodeMatrix.getHeight();
  int n_cols =   nodeMatrix.getWidth();
  int nn; 

 string code = 
        "\\documentclass{article}\n" 
    "\\usepackage{tikz}\n"
    "\\usetikzlibrary{trees,shapes.geometric, positioning, arrows}\n\n"
    "\\begin{document}\n\n"
   // "\\tikzset{ every node/.style={rotate=90} }\n\n" 
   //    "\\tikzstyle{startstop} = [rectangle, rounded corners, minimum width=2cm, minimum height =1cm, text centered, draw=black, fill=black!30]\n"
   // "\\tikzstyle{io} = [trapezium, trapezium left angle = 70, trapezium right angle=110, minimum width=2cm, minimum height =1cm, text centered, draw=black, fill=blue!30]\n"
   //   "\\tikzstyle{process} = [rectangle, minimum width=2cm, minimum height =1cm, text centered, draw=black, fill=blue!30]\n"
   //"\\tikzstyle{decision} = [diamond, minimum width=3cm, minimum height =1cm, text centered, draw=black, fill=green!30]\n"
    "\\tikzstyle{state} = [circle, inner sep=2pt, radius =20pt, text centered, draw=black,  fill=blue!50]\n"
    "\\tikzstyle{point} = [circle, inner sep=0pt, minimum size =1pt,  fill=red!30]\n"
   "\\tikzstyle{arrow} = [thick, ->, >=stealth]\n\n" 
   "\\begin{tikzpicture}[node distance = 2cm]\n\n" 
 
    "\\matrix[row sep =5em,column sep=5em]{\n"; 

  for (int r=0; r<n_rows; r++) 
    { 
      nn = nodeMatrix.getElement(r,0);  
      
      if (nn>0) code.append(registrar.getState(nn) +";");

      for (int c=1; c<n_cols; c++) 
	{
	  nn = nodeMatrix.getElement(r,c);  
	  code.append(" &"); 
          if (nn>0) code.append(registrar.getState(nn) +";");
	}
      code.append( "\\\\\n" ); 
    }

  code.append("};\n\n"); 

  for(Link l: links) code.append(l.toString()+"\n");


  code.append("\n\n\\end{tikzpicture}\n\\end{document}\n\n");

  return code;

}

int CFA::getEntryNode() const
{  
  return nodeMatrix.getElement(0,axis); 
}


CFA CFA::seqComp(const CFA & lower) const
{ 
    CFA result; 

    result.links = links;
    for(Link l:lower.links) result.links.push_back(l);  

    if(axis == lower.axis) 
      { result.nodeMatrix = nodeMatrix.joinBottom(lower.nodeMatrix);
         result.axis = axis;  

	 result.exits = lower.exits;

 	 for (pair<int,int> e: exits) 
  	     result.links.push_back(Link(e.first,lower.getEntryNode(), "->", (e.second==result.axis)?"--":"|-" ));
      }
    else if (axis > lower.axis) 
      { int diff = axis-lower.axis;
	 
	result.nodeMatrix = nodeMatrix.joinBottom(lower.nodeMatrix.growLeft(diff));       
	result.axis = axis;  

	for (pair<int,int> e: lower.exits) // adjust column number for each exits
	     result.exits.push_back(pair<int,int>(e.first,e.second+diff));

 	 for (pair<int,int> e: exits) 
  	     result.links.push_back(Link(e.first,lower.getEntryNode(), "->", (e.second==diff+lower.axis)?"--":"|-" ));

       }
    else /* axis < lower.axis */
      { int diff = lower.axis-axis;
	
	result.nodeMatrix = nodeMatrix.growLeft(diff).joinBottom(lower.nodeMatrix);
	result.axis = lower.axis;  
	
	result.exits = lower.exits;

 	 for (pair<int,int> e: exits) 
  	     result.links.push_back(Link(e.first,lower.getEntryNode(), "->", (diff+e.second==lower.axis)? "--" : "|-" ));

      }

    return result; 
}

CFA CFA::ifeComp(string cond, const CFA &right) const // this is the then branch CFA
{ 
  CFA result;

  unsigned entry  = CFA::registrar.put("s"); // create a state
  unsigned lentry = getEntryNode(); // entry state of the then branch
  unsigned rentry = right.getEntryNode(); // entry state of the else branch

  result.links = links; 
  for (Link l:right.links) result.links.push_back(l);
  
  result.links.push_back(Link(entry, lentry, "->", " -- node[sloped,above]{$" + cond +"$}"));
  result.links.push_back(Link(entry, rentry, "->", " -- node[sloped,above]{$ !(" +cond +")$}"));

  result.axis = ( nodeMatrix.getWidth() + right.nodeMatrix.getWidth()-1) /2 ;  

  result.nodeMatrix = nodeMatrix.joinRight(right.nodeMatrix).growTop(1);//.growBottom(1); 
  result.nodeMatrix.setElement(0,result.axis, entry); 

  result.exits = exits; 
  for (pair<int,int> e: right.exits) // adjust column number for each exit in the else branch
    result.exits.push_back(pair<int,int>(e.first,e.second+nodeMatrix.getWidth()));

  return result;
}
 
/*
CFA CFA::whileComp(string cond) const
{
  CFA result;

  unsigned entry  = CFA::registrar.put("s"); // create entry state
  unsigned exit  = CFA::registrar.put(""); // loop exit
  unsigned bentry = getEntryNode(); // entry state of the then branch


 
  result.links = links; 
  
  result.links.push_back(Link(entry, bentry, "->",  " -- node[sloped,above]{$" + cond +"$} "));
  result.links.push_back(Link(entry, exit, "->", " -- node[sloped,above]{$!(" +cond + ")$} "));


  result.axis = axis;

  result.nodeMatrix = nodeMatrix.growRight(1).growTop(1);
  result.nodeMatrix.setElement(0,result.axis, entry); 
  result.nodeMatrix.setElement(0, result.nodeMatrix.getWidth()-1, exit); 

  for (pair<int,int> e:exits)  result.exits.push_back(e); 
  result.exits.push_back(pair<int,int>(exit,result.nodeMatrix.getWidth()-1)); // add exit point

  return result;

}
*/

CFA CFA::whileComp(string cond) const
{
  CFA result;

  unsigned entry  = CFA::registrar.put("s"); // create entry state
  unsigned bentry = getEntryNode(); // entry state of the then branch
  unsigned lpoint  = CFA::registrar.put(""); // loop reentry
  unsigned exit  = CFA::registrar.put(""); // loop exit

 
  result.links = links; 
  
  result.links.push_back(Link(entry, bentry, "->",  " -- node[sloped,above]{$" + cond +"$} "));
  result.links.push_back(Link(entry, exit, "->", " -- node[sloped,above]{$!(" +cond + ")$} "));


  for (pair<int,int> e: exits) 
    result.links.push_back(Link(e.first, lpoint, " ", "|-")); //

  result.links.push_back(Link(lpoint, entry, "->", " |- "));

  result.axis = axis+1;

  result.nodeMatrix = nodeMatrix.growLeft(1).growRight(1).growTop(1).growBottom(1);
  result.nodeMatrix.setElement(0,result.axis, entry); 
  result.nodeMatrix.setElement(result.nodeMatrix.getHeight()-1,0,lpoint); // TBD
  result.nodeMatrix.setElement(0, result.nodeMatrix.getWidth()-1, exit); 

  result.exits.push_back(pair<int,int>(exit,result.nodeMatrix.getWidth()-1)); // one exit point

  return result;

}
