#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include "Matrix/FCD.h" 
#include "Matrix/CFA.h" 
#include "Matrix/AST.h" 


enum operation {EQ,GE,ADD,SUB,MUL,DIV,MOD};

typedef double (*func_t) (double); // type of primitive functions

enum kind {iNVALUE,fNVALUE, BVALUE,FVALUE};      

typedef struct
{ kind type; 
   union {
     bool truth;
     int iNumber;
     double fNumber;     
     func_t fnctptr;        /* address of a primitive function */
   } rep;
} value; 

void init_state(); 

class exp_node {
  public:

    // evaluation function for a leaf, replaced for interior nodes
    virtual value evaluate() = 0;

    std::string toString();

    /*
      operator priority levels: 
     
     8.   - (unary )
     7. ** (not implemented)
     6.  *, / 
     5.  +,  -
     4.  >, = 
     3.  !,  
     2. &&
     1. - ||             
     
     */

    virtual std::string toString(unsigned  parent_priority) = 0;
     
};


class call_node : public exp_node {
private:
    unsigned fid;
    exp_node *argexp;

 public: 
    call_node(unsigned myfid, exp_node *myargexp);
    value evaluate();
    std::string toString(unsigned);
};


class operator_node : public exp_node {
private:
    operation op; 
    exp_node *left;
    exp_node *right;

 public: 
  // the constructor for node links the node to its children,
  // and stores the character representation of the operator.
    operator_node(operation myop, exp_node *L, exp_node *R);
    value evaluate();
    std::string toString(unsigned);
};

class fNumber_node : public exp_node {
 private:
    double num;

 public:
  fNumber_node(double value);
  value evaluate();
  std::string toString(unsigned);
};


class iNumber_node : public exp_node {
 private:
    int num;

 public:
  iNumber_node(int value);
  value evaluate();
  std::string toString(unsigned);
};


class unary_minus_node : public exp_node {
 protected:
  exp_node *exp;
 public:
  unary_minus_node(exp_node *exp);
  value evaluate();
  std::string toString(unsigned);
};

class id_node : public exp_node {
protected:
  unsigned id;

public:
  id_node(unsigned myid);
  value evaluate();
  std::string toString(unsigned);
};


class cond_node {
  public:

    // evaluation function for a leaf, replaced for interior nodes
    virtual value evaluate() = 0;
    
    std::string toString();

    virtual std::string toString(unsigned)=0;
};


class or_node : public cond_node {
private:
  cond_node *left, *right;

 public:
    or_node(cond_node *L, cond_node *R);
    value evaluate(); 
    std::string toString(unsigned);
};

class and_node : public cond_node {
private:
  cond_node *left, *right;

public:
    and_node(cond_node *L, cond_node *R);
    value evaluate(); 
    std::string toString(unsigned);
};


class neg_node : public cond_node {
 private:
    cond_node *child;

 public:
    neg_node(cond_node *child);
    value evaluate(); 
    std::string toString(unsigned);
};


class compare_node : public cond_node {
private:
  operation op; 
  exp_node *left, *right;

public:
  compare_node(operation op, exp_node *L, exp_node *R);
  value evaluate(); 
  std::string toString(unsigned);
};




class statement {
 public:
  virtual void print(int) {}
  virtual void evaluate() = 0;
  virtual bool isEmpty() =0;
  virtual FCD FCDgen() = 0; 
  virtual CFA CFAgen() = 0; 
  virtual AST ASTgen() = 0; 
};

class ife_stmt : public statement {
 protected:
  cond_node *condition;
  statement *thenbranch, *elsebranch;
 public:
  ife_stmt(cond_node  *condition, statement *thenbranch, statement *elsebranch);
  void print(int);
  void evaluate();
  bool isEmpty();
  FCD FCDgen(); 
  CFA CFAgen(); 
  AST ASTgen(); 
};

class while_stmt : public statement {
 protected:
  cond_node *condition;
  statement *bodystmt;
 public:
  while_stmt(cond_node  *condition, statement *bodystmt);
  void print(int);
  void evaluate();
 bool isEmpty(); 
  FCD FCDgen(); 
  CFA CFAgen(); 
  AST ASTgen(); 
};

class input_stmt : public statement {
 protected:
  unsigned id;
 public:
  input_stmt(unsigned myid);
  void print(int);
  void evaluate();
 bool isEmpty(); 
  FCD FCDgen(); 
  CFA CFAgen(); 
  AST ASTgen(); 
};



class assignment_stmt : public statement {
 protected:
  unsigned id;
  exp_node *exp;
 public:
  assignment_stmt(unsigned myid, exp_node *expression);
  void print(int);
  void evaluate();
 bool isEmpty(); 
  FCD FCDgen(); 
  CFA CFAgen();
  AST ASTgen();  
};

class print_stmt: public statement {
 protected:
  exp_node *exp;
 public:
  print_stmt(exp_node *myexp);
  void print(int);
  void evaluate();
 bool isEmpty(); 
  FCD FCDgen(); 
  CFA CFAgen(); 
  AST ASTgen(); 
};

class skip_stmt: public statement {
 public:
  skip_stmt();
  void print(int);
  void evaluate();
  bool isEmpty();
  FCD FCDgen();   
  CFA CFAgen(); 
  AST ASTgen(); 
};


class sequence_stmt: public statement {
 protected:
  statement *stmt1, *stmt2;
 public:
  sequence_stmt(statement *mystmt1, statement *mystmt2);
  void print(int);
  void evaluate();
  bool isEmpty(); 
  FCD FCDgen(); 
  CFA CFAgen(); 
  AST ASTgen(); 
};



