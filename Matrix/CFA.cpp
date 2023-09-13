#include "CFA.h"

using namespace std;

Register CFA::registrar = Register(); 


CFA::CFA()
{ nodeMatrix = myMatrix(1,1); 
  axis = 0; 
  links = list<Link>(); 
  hungLinks = list<HungLink>();
}

CFA::CFA(string Label)
{ 
  unsigned sn = CFA::registrar.put("s"); // a state

  axis = 0; 
  nodeMatrix=myMatrix(1,1);
  nodeMatrix(0,0) = sn;

  links = list<Link>();
  hungLinks = list<HungLink>();
  hungLinks.push_back(HungLink(sn, 0, " node[sloped, near start,above]{" + Label + "}"));
  // a hunglink <source node number, column number in node matrix, label>
  
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
    "\\tikzstyle{state} = [circle, inner sep=2pt, radius =20pt, text centered, draw=black,  fill=blue!50]\n"
    "\\tikzstyle{point} = [circle, inner sep=0pt, minimum size =1pt,fill]\n"
    "\\tikzstyle{myarrow} = [->, >=latex]\n"
   "\\begin{tikzpicture}[node distance = 2cm, >=latex]\n\n" 
 
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
    string edgeLabel; 
   
    result.links = links;
    for(Link l:lower.links) result.links.push_back(l);  

    if(axis == lower.axis) 
      { result.nodeMatrix = nodeMatrix.joinBottom(lower.nodeMatrix);
         result.axis = axis;  

	 result.hungLinks = lower.hungLinks;

 	 for (HungLink e: hungLinks) 
	   { 

	     if (e.col==lower.axis) edgeLabel = " -- " +e.label; 
	     else edgeLabel = " |- " +e.label; 

	     result.links.push_back(Link(e.node_n, lower.getEntryNode(), "myarrow", edgeLabel));
	   }

      }
    else if (axis > lower.axis) 
      { int diff = axis-lower.axis;
	 
	result.nodeMatrix = nodeMatrix.joinBottom(lower.nodeMatrix.growLeft(diff));       
	result.axis = axis;  

	for (HungLink e: lower.hungLinks) // adjust column number for each hungLinks
	  result.hungLinks.push_back(HungLink(e.node_n, e.col+diff, e.label));

	for (HungLink e: hungLinks) 
	  {	
	     if (e.col==diff+lower.axis) edgeLabel = " -- " +e.label; 
	     else edgeLabel= " |- " +e.label; 

	     result.links.push_back(Link(e.node_n, lower.getEntryNode(),  "myarrow", edgeLabel));
	  }
       }
    else /* axis < lower.axis */
      { int diff = lower.axis-axis;
	
	result.nodeMatrix = nodeMatrix.growLeft(diff).joinBottom(lower.nodeMatrix);
	result.axis = lower.axis;  
	
	result.hungLinks = lower.hungLinks;

	for (HungLink e: hungLinks) 
	  {
	    if (e.col+diff == lower.axis) edgeLabel = " -- " +e.label; 
	    else edgeLabel= " |- " +e.label; 
	    result.links.push_back(Link(e.node_n, lower.getEntryNode(), "myarrow", edgeLabel));
	  }
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
  
  result.links.push_back(Link(entry, lentry, "myarrow", " -- node[sloped,above]{$" + cond +"$}"));
  result.links.push_back(Link(entry, rentry, "myarrow", " -- node[sloped,above]{$ !(" +cond +")$}"));

  result.axis = ( nodeMatrix.getWidth() + right.nodeMatrix.getWidth()-1) /2 ;  

  result.nodeMatrix = nodeMatrix.joinRight(right.nodeMatrix).growTop(1);//.growBottom(1); 
  result.nodeMatrix.setElement(0,result.axis, entry); 

  result.hungLinks = hungLinks; 
  for (HungLink e: right.hungLinks) // adjust column number for each exit in the else branch
    result.hungLinks.push_back(HungLink(e.node_n, e.col + nodeMatrix.getWidth(), e.label));

  return result;
}
 

CFA CFA::whileComp(string cond) const
{
  CFA result;

  unsigned entry  = CFA::registrar.put("s"); // create entry state
  unsigned bentry = getEntryNode(); // entry state of the then branch
  unsigned lpoint  = CFA::registrar.put(""); // loop reentry
  unsigned exit  = CFA::registrar.put(""); // loop exit

 
  result.links = links; 
  
  result.links.push_back(Link(entry, bentry, "myarrow",  " -- node[sloped,above]{$" + cond +"$} "));
  result.links.push_back(Link(entry, exit, "myarrow", " -- node[sloped,above]{$!(" +cond + ")$} "));


  for (HungLink e: hungLinks) 
    result.links.push_back(Link(e.node_n, lpoint, "myarrow", "|- " + e.label)); //

  result.links.push_back(Link(lpoint, entry, "myarrow", " |- "));

  result.axis = axis+1;

  result.nodeMatrix = nodeMatrix.growLeft(1).growRight(1).growTop(1).growBottom(1);
  result.nodeMatrix.setElement(0,result.axis, entry); 
  result.nodeMatrix.setElement(result.nodeMatrix.getHeight()-1,0,lpoint); // TBD
  result.nodeMatrix.setElement(0, result.nodeMatrix.getWidth()-1, exit); 

  result.hungLinks.push_back(HungLink(exit,result.nodeMatrix.getWidth()-1,"")); // one exit point

  return result;

}
