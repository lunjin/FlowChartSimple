#include "myMatrix.h"
#include <iostream>
#include <iomanip>

using namespace std; 

myMatrix::myMatrix(int m, int n):
  height(m), width(n)
{ 
  values = new int[m*n];
  for(int i=0; i<m*n; i++) values[i]=0;
}


myMatrix::myMatrix():
  height(1), width(1)
{ 
  values = new int[1];
  values [0] = 0; 
}


myMatrix::~myMatrix()
{
  delete values; 
}


int myMatrix::getHeight() const
    { return height; 
    }

int myMatrix::getWidth() const
    { return width; 
    }

myMatrix::myMatrix(const myMatrix & m) :
  height(m.height), width(m.width)
{ values = new int[height*width]; 
  std::copy(m.values, m.values+height*width, values);
}


myMatrix & myMatrix::operator=(const myMatrix & m) 
{ 
  height =m.height;
  width = m.width;
  delete values; 
  values = new int[height*width]; 
  std::copy(m.values, m.values+height*width, values);
}

int & myMatrix::operator()(int i, int j) 
{
  if ((i<0)||(i>=height) || (j<0) || (j>=width)) 
    { cout << "Index out of bound" << endl; 
    }
  return values[i*width+j]; 
}


int  myMatrix::getElement(int i, int j) const
{
  if ((i<0)||(i>=height) || (j<0) || (j>=width)) 
    { cout << "Index out of bound" << endl; 
    }
  return values[i*width+j]; 
}

void  myMatrix::setElement(int i, int j,int e)
{
  if ((i<0)||(i>=height) || (j<0) || (j>=width)) 
    { cout << "Index out of bound" << endl; 
    }
  values[i*width+j]=e; 
}

myMatrix &  myMatrix::print() const
{
  for(int r =0; r<height; r++) 
    {  for (int c=0; c<width; c++)
	    cout << std::setw(5) << values[r*width +c]; 
       cout << endl; 
    }
}

myMatrix  myMatrix::joinBottom( const myMatrix & other) const
{ 
  if(width == other.width)
    { myMatrix result(height+other.height, width); 
       result.values = new int[width*(height+other.height)]; 
       std::copy(values, 
		 values+height*width,
		 result.values); // copy rows from this  
       std::copy(other.values, 
		 other.values+other.height*other.width, 
		 result.values+height*width);
       return result;
    }
  else if (width>other.width) 
    {  //pad other with columns of zeros to the right
      myMatrix pad(other.height,width-other.width);
      return joinBottom(other.joinRight(pad));  
    }
 else 
   {  //pad this with columns of zeros to the right
      myMatrix pad(height,other.width-width);
      return joinRight(pad).joinBottom(other); 
   }
}


myMatrix  myMatrix::joinTop( const myMatrix & other) const
{ 
  if(width == other.width)
    { 
       myMatrix result(height+other.height, width); 
       result.values = new int[width*(height+other.height)]; 
       std::copy(other.values, 
		 other.values+other.height*other.width, 
		 result.values);
       std::copy(values, 
		 values+height*width,
		 result.values+other.height*other.width); // copy rows from this  
       return result; 
    }
  else if (width>other.width) 
    {  //pad other with columns of zeros to the right
      myMatrix pad(other.height,width-other.width);
      return joinTop(other.joinRight(pad));  
    }
 else 
   {  //pad this with columns of zeros to the right
      myMatrix pad(height,other.width-width);
      return joinRight(pad).joinTop(other); 
   }
}

myMatrix  myMatrix::joinRight( const myMatrix & other) const
{
  if(height == other.height)
    {   
      int newwidth = width+other.width; 
      myMatrix result(height,newwidth);

      for (int r=0; r<height; r++)
	   {  //cout<< "copying row " << r <<endl;
              std::copy(values+r*width, 
			values+(r+1)*width,
			result.values+r*newwidth); 
	     std::copy(other.values+r*other.width, 
		       other.values+(r+1)*other.width, 
		       result.values+r*newwidth+width);
	   }
       return result;
    }
  else if (height>other.height) 
    { //pad other with rows of zeros to the bottom 
      myMatrix pad(height-other.height,other.width); 
      return joinRight(other.joinBottom(pad));   
    }
 else 
   { //pad this with rows of zeros to the bottom 
      myMatrix pad(other.height-height,width); 
      return joinBottom(pad).joinRight(other);   
   }
}


myMatrix  myMatrix::joinLeft( const myMatrix & other) const
{
  if(height == other.height)
    {   
      int newwidth = width+other.width; 

      myMatrix result(height,newwidth); 

      for (int r=0; r<height; r++)
	   {  //cout<< "copying row " << r <<endl;
       	      std::copy(other.values+r*other.width, 
		       other.values+(r+1)*other.width, 
		       result.values+r*newwidth);
	      std::copy(values+r*width, 
			values+(r+1)*width,
			result.values+r*newwidth+other.width); 
	   }
      return result; 
    }
  else if (height>other.height) 
    { //pad other with rows of zeros to the bottom 
      myMatrix pad(height-other.height,other.width); 

      return joinLeft(other.joinBottom(pad));   
    }
 else 
   { //pad this with rows of zeros to the bottom 
      myMatrix pad(other.height-height,width); 
 
      return joinBottom(pad).joinLeft(other);   
   }
}

myMatrix myMatrix::growLeft(int n) const
{ 

if (n==0) return *this; 
else 
  { myMatrix pad(height,n);
    return joinLeft(pad); 
  }
}


myMatrix myMatrix::growRight(int n) const
{ 

if (n==0) return *this; 
else 
  { myMatrix pad(height,n);
    return joinRight(pad); 
  }
}


myMatrix myMatrix::growTop(int n) const
{ 

if (n==0) return *this; 
else 
  { myMatrix pad(n,width);
    return joinTop(pad); 
  }
}


myMatrix myMatrix::growBottom(int n) const
{ 

if (n==0) return *this; 
else 
  { myMatrix pad(n,width);
    return joinBottom(pad); 
  }
}





/*
int main() 
{ myMatrix m(2,3), b(3,1),c;
  m(0,0)=1; m(0,1)=2; m(0,2)=3;
  m(1,0)=4; m(1,1)=5; m(1,2)=6;

  b(0,0)=9; b(1,0)=8; b(2,0)=7; 

  m.print();

  cout << endl << endl;

  b.print(); 


 cout << endl << endl;

 m.joinTop(b).growRight(5).print(); 

 cout << endl << endl;

 b.joinLeft(m).growTop(5).print();


}
*/
