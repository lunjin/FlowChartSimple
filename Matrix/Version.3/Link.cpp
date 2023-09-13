#include "Link.h"

using namespace std;

unsigned Link::getSource() const
{ 
  return source; 
}

unsigned Link::getSink() const
{ 
  return sink; 
}

string Link::getDesc() const
{
  return desc;
}

string Link::getArrowTip() const
{
  return arrowtip;
}

Link::Link(unsigned s, unsigned t, string a, string d): 
  source(s), sink(t), arrowtip(a), desc(d)
{
}


string Link::toString() const
{ 
  return "\\draw ["+arrowtip+"]  (vert"+to_string(source)+") "  + desc + " (vert"+to_string(sink) +");"; 
}
