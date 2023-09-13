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

     myMatrix & joinToBottom(const myMatrix &) ;
     myMatrix & joinToRight(const myMatrix &) ;

     myMatrix & joinToTop(const myMatrix &) ;
     myMatrix & joinToLeft(const myMatrix &) ;


     ~myMatrix();

     int & operator()(int, int); 

     myMatrix & operator=(const myMatrix &); 

     void print(); 
  
} ;

