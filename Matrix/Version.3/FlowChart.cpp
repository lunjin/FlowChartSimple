#include "FlowChart.h"

using namespace std;

Register FCD::registrar = Register(); 


FCD::FCD()
{ nodeMatrix = myMatrix(1,1); 
  axis = 0; 
  links = list<Link>(); 
  exits = list<pair<int,int>>();
}

FCD::FCD(string Label)
{ 
  unsigned nn = FCD::registrar.put(Label);

  axis = 0; 
  nodeMatrix=myMatrix(1,1);
  nodeMatrix(0,0) = nn;

  links = list<Link>();  
  exits = list<pair<int,int>>();
  exits.push_back(pair<int,int>(nn,0));
  
}

string FCD::TikZCode() const
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
    "\\tikzstyle{startstop} = [rectangle, rounded corners, minimum width=2cm, minimum height =1cm, text centered, draw=black, fill=black!30]\n"
    "\\tikzstyle{io} = [trapezium, trapezium left angle = 70, trapezium right angle=110, minimum width=2cm, minimum height =1cm, text centered, draw=black, fill=blue!30]\n"
    "\\tikzstyle{process} = [rectangle, minimum width=2cm, minimum height =1cm, text centered, draw=black, fill=blue!30]\n"
   "\\tikzstyle{decision} = [diamond, minimum width=3cm, minimum height =1cm, text centered, draw=black, fill=green!30]\n"
    "\\tikzstyle{point} = [circle, inner sep=0pt, minimum size =2pt,  fill=red!30]\n"
   "\\tikzstyle{arrow} = [thick, ->, >=stealth]\n\n" 
   "\\begin{tikzpicture}[node distance = 2cm]\n\n" 
 
    "\\matrix[row sep =5mm,column sep=5mm]{\n"; 

  for (int r=0; r<n_rows; r++) 
    { 
      nn = nodeMatrix.getElement(r,0);  
      // cout << std::setw(5) << registrar.get(nn) <<";";
      
      if (nn>0) code.append(registrar.get(nn) +";");

      for (int c=1; c<n_cols; c++) 
	{
	  nn = nodeMatrix.getElement(r,c);  
	  //	  cout << " &" << std::setw(5) << registrar.get(nn)<<";";
	  code.append(" &"); 
          if (nn>0) code.append(registrar.get(nn) +";");
	}
      code.append( "\\\\\n" ); 
    }

  code.append("};\n\n"); 

  for(Link l: links) code.append(l.toString()+"\n");


  code.append("\n\n\\end{tikzpicture}\n\\end{document}\n\n");

  return code;

}

int FCD::getExitNode() const
{  
  return nodeMatrix.getElement(nodeMatrix.getHeight()-1,axis); 
}


int FCD::getEntryNode() const
{  
  return nodeMatrix.getElement(0,axis); 
}


FCD FCD::seqComp(const FCD & lower) const
{ 
    FCD result; 

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

FCD FCD::ifeComp(string cond, const FCD &right) const // this is the then branch FCD
{ 
  FCD result;

  unsigned entry  = FCD::registrar.put("[decision]{"+cond+"}"); // create decision node
  unsigned lentry = getEntryNode(); // entry node of the then branch
  unsigned rentry = right.getEntryNode(); // entry node of the else branch

  result.links = links; 
  for (Link l:right.links) result.links.push_back(l);
  
  result.links.push_back(Link(entry, lentry, "->", " -- node[sloped]{yes}"));
  result.links.push_back(Link(entry, rentry, "->", "-- node[sloped]{no}"));

  result.axis = ( nodeMatrix.getWidth() + right.nodeMatrix.getWidth()-1) /2 ;  

  result.nodeMatrix = nodeMatrix.joinRight(right.nodeMatrix).growTop(1);//.growBottom(1); 
  result.nodeMatrix.setElement(0,result.axis, entry); 

  result.exits = exits; 
  for (pair<int,int> e: right.exits) // adjust column number for each exit in the else branch
    result.exits.push_back(pair<int,int>(e.first,e.second+nodeMatrix.getWidth()));

  return result;
}
 

FCD FCD::whileComp(string cond) const
{
  FCD result;

  unsigned entry  = FCD::registrar.put("[decision]{"+cond+"}"); // create decision node
  unsigned bentry = getEntryNode(); // entry node of the then branch
  unsigned bexit = getExitNode(); // exit node of the then branch
  unsigned lpoint  = FCD::registrar.put("[point]{}"); // loop reentry
  unsigned rpoint  = FCD::registrar.put("[point]{}"); // loop exit

 
  result.links = links; 
  
  result.links.push_back(Link(entry, bentry, "->",  " -- node[sloped]{yes} "));
  result.links.push_back(Link(entry, rpoint, "->", " -- node[sloped]{no} "));

  for (pair<int,int> e: exits) 
    result.links.push_back(Link(e.first,lpoint, " ", "|-")); 

  result.links.push_back(Link(lpoint, entry, "->", " |- "));

  result.axis = axis+1;

  result.nodeMatrix = nodeMatrix.growLeft(1).growRight(1).growTop(1).growBottom(1);
  result.nodeMatrix.setElement(0,result.axis, entry); 
  result.nodeMatrix.setElement(result.nodeMatrix.getHeight()-1,0,lpoint); // TBD
  result.nodeMatrix.setElement(0, result.nodeMatrix.getWidth()-1, rpoint); 

  result.exits.push_back(pair<int,int>(rpoint,result.nodeMatrix.getWidth()-1)); // one exit point

  return result;

}
