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

Link::Link(unsigned s, string sp,  unsigned t, string tp, string a, string d): 
  source(s), source_pos(sp), sink(t), sink_pos(tp),  arrowtip(a), desc(d)
{
}

Link::Link(unsigned s, unsigned t, string a, string d): 
  Link(s, "", t, "", a, d)
{
}


string Link::toString() const
{ 
  return "\\draw ["+arrowtip+"]  (vert"+to_string(source)+source_pos+") "  + desc + " (vert"+to_string(sink) +sink_pos+");"; 
}
