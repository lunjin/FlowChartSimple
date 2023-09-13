class myMatrix
{ 
 private: 
     int *values; 
     int height;
     int width;  

 public:

     myMatrix(int, int);
     myMatrix();
     myMatrix(const myMatrix &); 

     myMatrix  joinBottom(const myMatrix &) const ;
     myMatrix  joinRight(const myMatrix &) const ;

     myMatrix  joinTop(const myMatrix &) const ;
     myMatrix  joinLeft(const myMatrix &) const ;
     
    myMatrix  growLeft(int) const ;
    myMatrix  growRight(int) const ;
    myMatrix  growBottom(int) const ;
    myMatrix  growTop(int) const ;



     ~myMatrix();

     int & operator()(int, int); 

     myMatrix & operator=(const myMatrix &); 

     myMatrix & print() const; 

     int getHeight() const; 
     int getWidth() const; 
     int getElement(int, int) const; 
     void setElement(int, int,int); 
     
  
} ;

