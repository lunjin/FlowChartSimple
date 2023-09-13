#include "imp.h"
#include "string_table.h"
#include <sstream> //for by-passing to_string
#include <cmath>
#include<regex>
#include<string>

using namespace std;



string_table nametable(64); 
map<unsigned, value> state;


bool isTooSmall(double value)
{ 
  const double epsilon = 5.4E-079;
  return std::abs(value) <= epsilon;
}

bool is_integer(const string & s){ 
  size_t first, last;
  bool flag = true;

  last = s.find_last_not_of(' ');
  first = s.find_first_not_of(" -");

  if (!isdigit(s[first]) && (s[first]!='-')) flag = false;  
  for(size_t i =first+1; flag && (i <=last); i++)
    if (!isdigit(s[i])) flag = false;

  //  cout << "The string " << s << (flag ? " is ":" is not ") << "an integer" << endl; 
   
  return flag;
}


bool is_float(const string & s){
  size_t first, last, i;
  bool flag = true;

  last = s.find_last_not_of(' ');
  first = s.find_first_not_of(' ');

  if (!isdigit(s[first]) && (s[first]!='-') &&(s[first]!='.')) 
    { 
      //      cout<< "The 1st character is not part of integer\n";
      flag = false;  // check first char
    }

  if (s[first] !='.')
    {
      i =first+1;
      while((i <=last) && isdigit(s[i]))  i++; //
    }
  else i=first; 


  if ( (i>=last) || (s[i] != '.') ) 
    { 
      //      cout << "First non-digit must be dot which cannot be the last char\n"; 
      flag = false; // first non-digit  must be dot which cannot be last 
    }
  
  i++; 

  while (flag && (i <=last)) 
    {      
      if (!isdigit(s[i])) 
	{
	  //	  cout << "There is a  non-digit after  dot\n"; 
	  flag = false;
	}
      else  i++;
    }

  //  cout << "The string " << s << (flag ? " is ":" is not ") << "a float" << endl; 
   
  return flag;
}


void init_func(string name, func_t address)
{ unsigned index;
   index = nametable.put(name);
   state[index].rep.fnctptr = address; 
   state[index].type = FVALUE; 
}

void  init_state ()
     {
       init_func("sin", sin);
       init_func("cos",cos);
       init_func("atan",atan);
       init_func("log",log);
       init_func("exp",exp);
       init_func("sqrt",sqrt);
     }
     

// preconditon: n>=0
void output_tabs(int n) 
{ 
  while (n>0) 
    { cout << "\t"; 
       n--;
    }
}



string cond_node::toString()
{ return toString(0); 
}

or_node::or_node(cond_node *L, cond_node *R):
  left(L), right(R)
{
}


string or_node::toString(unsigned pp)
{ 
  string text =  left->toString(1) + "||" +  right->toString(1); 

  if (pp > 1) text = "("+text+")"; 

  return  text; 
}

value or_node::evaluate()
{ value opd1, opd2, result;  
   
  opd1 = left->evaluate(); 
  opd2 = right->evaluate(); 

  if ( (opd1.type != BVALUE) ||  (opd2.type != BVALUE) ) 
    {
      cout << " Operator || is applied to non-Boolean operands" << endl;
      exit(1);
    };

  result.rep.truth = opd1.rep.truth || opd2.rep.truth; 
  result.type = BVALUE; 
  return result;
 
} 


and_node::and_node(cond_node *L, cond_node *R):
  left(L), right(R)
{
}

string and_node::toString(unsigned pp)
{ 
  string text =  left->toString(2) + "&&" +  right->toString(2); 

  if (pp > 2) text = "("+text+")"; 

  return  text; 

}

value and_node::evaluate()
{ value opd1, opd2; 
   
  opd1 = left->evaluate(); 
  opd2 = right->evaluate(); 

  if ( (opd1.type != BVALUE) || (opd2.type != BVALUE) ) 
    { 
      cout << " Operator && is applied to non-Boolean operands" << endl;
      exit(1);
    };

  opd1.rep.truth = opd1.rep.truth && opd2.rep.truth; 
  return opd1;

 } 


neg_node::neg_node(cond_node *child)
{ this -> child = child; 
}

string neg_node::toString(unsigned pp)
{   
  string text = "!(" + child->toString(3) +")"; 

  if (pp > 3) text = "("+text+")"; 

  return  text; 

}

value neg_node::evaluate()
{  value opd; 
   
  opd = child->evaluate(); 

  if  (opd.type != BVALUE)  
    { 
      cout << " Operator ! is applied to non-Boolean operand" << endl;
      exit(1);
    };

  opd.rep.truth = ! opd.rep.truth;
  return opd;

} 

string exp_node::toString()
{ return toString(0); 
}

compare_node::compare_node(operation op, exp_node *left, exp_node *right)
{
  this -> op = op;
  this -> left = left;
  this -> right = right;
}


string  compare_node::toString(unsigned pp)
{ 
  string opName; 
  switch(op)
    { case GE: opName =  ">"; break;
       case EQ: opName =  "="; break;
    } 

  string text =  left->toString(4) + opName +  right->toString(4); 

  if (pp > 4) text = "("+text+")"; 

  return  text; 
}

bool isNumeric(kind type) 
{ 
  return (type==iNVALUE) || (type==fNVALUE) ;
}

value compare_node::evaluate()
{
  value opd1, opd2, result; 
   
  opd1 = left->evaluate(); 
  opd2 = right->evaluate(); 

  if ( ! isNumeric(opd1.type)  || ! isNumeric(opd2.type) ) 
    { 
      cout << "Comparison  operator is applied to non-numeric operands" << endl;
      exit(1);
    };

  result.type = BVALUE;

  switch(op)
    { case GE: result.rep.truth = (  ((opd1.type==iNVALUE)? opd1.rep.iNumber:opd1.rep.fNumber)  > 
				     ((opd2.type==iNVALUE) ? opd2.rep.iNumber : opd2.rep.fNumber) ); break;
         case EQ: result.rep.truth = (  ((opd1.type==iNVALUE)? opd1.rep.iNumber:opd1.rep.fNumber)  == 
				     ((opd2.type==iNVALUE) ? opd2.rep.iNumber : opd2.rep.fNumber) ); break;
      } 

  return result;
} 

  
fNumber_node::fNumber_node(double value) {
    num = value;
   }
  
string fNumber_node:: toString(unsigned pp) {
  ostringstream os; 
  os << num;
  return os.str(); 
  //  return to_string(num);
}

value fNumber_node::evaluate() { 
  value result;

  result.type = fNVALUE;
  result.rep.fNumber = num;

  return result; 

}


iNumber_node::iNumber_node(int value) {
    num = value;
   }
  
string iNumber_node:: toString(unsigned pp) {
  ostringstream os; 
  os << num;
  return os.str(); 
  //  return to_string(num);
}

value iNumber_node::evaluate() { 
  value result;

  result.type = iNVALUE;
  result.rep.iNumber = num;

  return result; 

}

id_node::id_node(unsigned myid) : id(myid) {}

string id_node:: toString(unsigned pp) {
  return nametable.get(id);
}

value id_node::evaluate() { 
    return state[id]; 
  }


call_node::call_node(unsigned myfid, exp_node *myargexp) : fid(myfid), argexp(myargexp) {
  }

string call_node:: toString(unsigned pp) {
  return  nametable.get(fid) + "(" +  argexp->toString() + ")";
}

value call_node::evaluate() {
  value  argvalue, result;

  argvalue = argexp -> evaluate(); 

  if (state[fid].type != FVALUE) // type checking
      { cout << nametable.get(fid) << " is not a primitive function!"<< endl;
	exit(1);
      };

  if (! isNumeric(argvalue.type) ) // type checking
      { cout << nametable.get(fid) << " is applied to a non-numeric argument!"<< endl;
	exit(1);
      };
    
  result.type = fNVALUE; // function calls always return a float 
  result.rep.fNumber = (*(state[fid].rep.fnctptr))((argvalue.type==iNVALUE) ? argvalue.rep.iNumber : argvalue.rep.fNumber);

  return result;

  }


operator_node::operator_node(operation myop, exp_node *L, exp_node *R) {
  op = myop; 
  left    = L;
  right   = R;
}


string operator_node:: toString(unsigned pp) {
  string text; 

  switch (op) 
    {
    case ADD: 
       text =  left->toString(5) + "+" +  right->toString(5); 
       if (pp > 5) text = "("+text+")"; 
       break; 

    case SUB:
      text =  left->toString(5) + "-" +  right->toString(5); 
      if (pp > 5) text = "("+text+")"; 
      break;

    case MUL:
      text =  left->toString(6) + "*" +  right->toString(6); 
      if (pp > 6) text = "("+text+")";  
      break;

    case DIV: 
      text =  left->toString(6) + "/" +  right->toString(6); 
      if (pp > 6) text = "("+text+")"; 
      break;

    case MOD:
      text =  left->toString(6) + "\%" +  right->toString(6); 
      if (pp > 6) text = "("+text+")"; 
      break;
    }

  return  text; 
}


value operator_node::evaluate() {

  value opd1, opd2, result;

  opd1 = left -> evaluate(); 
  opd2 = right -> evaluate(); 

  if (! isNumeric(opd1.type)  || ! isNumeric(opd2.type) )  // type checking
      { cout << " Numeric operator is applied to non-numeric operands!"<< endl;
	exit(1);
      };
    
  if ( opd1.type == iNVALUE && opd2.type==iNVALUE) result.type = iNVALUE; 
  else result.type=fNVALUE;  

  switch (op)
    {
    case ADD: 
      if ( opd1.type == iNVALUE && opd2.type==iNVALUE) result.rep.iNumber = opd1.rep.iNumber + opd2.rep.iNumber; 
      else result.rep.fNumber = ((opd1.type==iNVALUE) ? opd1.rep.iNumber : opd1.rep.fNumber) + 
	                                       ((opd2.type==iNVALUE) ? opd2.rep.iNumber : opd2.rep.fNumber) ;
      break;

    case SUB: 
      if ( opd1.type == iNVALUE && opd2.type==iNVALUE) result.rep.iNumber = opd1.rep.iNumber - opd2.rep.iNumber; 
      else result.rep.fNumber = ((opd1.type==iNVALUE) ? opd1.rep.iNumber : opd1.rep.fNumber) -
	                                       ((opd2.type==iNVALUE) ? opd2.rep.iNumber : opd2.rep.fNumber) ;
      break;

    case MUL: 
      if ( opd1.type == iNVALUE && opd2.type==iNVALUE) result.rep.iNumber = opd1.rep.iNumber * opd2.rep.iNumber; 
      else result.rep.fNumber = ((opd1.type==iNVALUE) ? opd1.rep.iNumber : opd1.rep.fNumber) * 
	                                       ((opd2.type==iNVALUE) ? opd2.rep.iNumber : opd2.rep.fNumber) ;
      break;

    case DIV: 
      if ( (opd2.type == iNVALUE && opd2.rep.iNumber !=0) || (opd2.type == fNVALUE && isTooSmall(opd2.rep.fNumber) ) )
	{ 
	  if ( opd1.type == iNVALUE && opd2.type==iNVALUE) result.rep.iNumber = opd1.rep.iNumber / opd2.rep.iNumber; 
	  else result.rep.fNumber = ((opd1.type==iNVALUE) ? opd1.rep.iNumber : opd1.rep.fNumber) /
		                                  ((opd2.type==iNVALUE) ? opd2.rep.iNumber : opd2.rep.fNumber) ;
	}
      else
	{
	  cout << "division by zero -> " << ((opd1.type==iNVALUE) ? opd1.rep.iNumber : opd1.rep.fNumber)  << " / " << 0 << endl;
	  //  include stdlib.h for exit
	  exit(1);
	}
      break;

    case MOD: 
      if (opd1.type !=iNVALUE || opd2.type !=iNVALUE )
	{ 
	  cout << "Operator % is applied to non-interger operands" << endl; 
	  exit(1);
	}
      else if (opd2.rep.iNumber!=0) 
	{ result.rep.iNumber = opd1.rep.iNumber % opd2.rep.iNumber; 
	  // needs to refine
	}
      else
	{
	  cout << "division by zero -> " << opd1.rep.iNumber << " / " << 0 << endl;
	  //  include stdlib.h for exit
	  exit(1);
	}
      break;
    }

  return result;

  }




unary_minus_node::unary_minus_node(exp_node *L) : exp(L) {}


string unary_minus_node:: toString(unsigned pp) {
  string text =  "-"+exp->toString(8);

  if (pp > 8) text = "("+text+")"; 

  return  text; 
}

value  unary_minus_node::evaluate() {

  value opd = exp->evaluate();
  
  if (opd.type==iNVALUE)  opd.rep.iNumber = - opd.rep.iNumber;
  else   if (opd.type==fNVALUE)  opd.rep.fNumber = - opd.rep.fNumber;
  else // type checking 
    { 	cout << "- is applied to a non-numeric operand!" << endl;
	exit(1);
    };

  return  opd;
}

ife_stmt::ife_stmt(cond_node *condition, statement *thenbranch, statement *elsebranch)
{ 
  this -> condition = condition;
  this -> thenbranch = thenbranch;
  this -> elsebranch = elsebranch;
}

bool ife_stmt::isEmpty() { return false; }

void ife_stmt::print(int n) {
  output_tabs(n);  cout << "if " + condition -> toString();   cout  << endl;
  output_tabs(n);  cout << "then { " << endl; 
  thenbranch -> print(n+1);  cout <<  "} " << endl;
  output_tabs(n); cout << "else { " << endl;
  elsebranch -> print(n+1);   cout << "} "; 
}

FCD ife_stmt::FCDgen() { 
  return thenbranch ->FCDgen().ifeComp("$"+condition ->toString() +"$" , elsebranch->FCDgen()); 
}

CFA ife_stmt::CFAgen() { 
  return thenbranch ->CFAgen().ifeComp(condition ->toString() , elsebranch->CFAgen()); 
}

AST ife_stmt::ASTgen() { 
  return thenbranch ->ASTgen().ifeComp(condition ->toString() , elsebranch->ASTgen()); 
}

void ife_stmt::evaluate() {
 
  value cval;

  cval = condition -> evaluate(); 

  if (cval.type != BVALUE) {
    cout << "Branching condition is non-Boolean" << endl;
    exit(1);
  };

  if (cval.rep.truth) { thenbranch -> evaluate(); }  else { elsebranch -> evaluate(); }

}


while_stmt::while_stmt(cond_node *condition, statement *bodystmt)
{ 
  this -> condition = condition;
  this -> bodystmt = bodystmt;
}

bool while_stmt::isEmpty() { return false; }

void while_stmt::print(int n) {
  output_tabs(n);  cout << "while " + condition->toString();   cout  << " do {" <<endl;
  bodystmt -> print(n+1);  cout <<  "} ";
}

FCD while_stmt::FCDgen() { 
  return bodystmt->FCDgen().whileComp("$"+condition ->toString() +"$"); 
}

CFA while_stmt::CFAgen() { 
  return bodystmt->CFAgen().whileComp(condition ->toString()); 
}

AST while_stmt::ASTgen() { 
  return bodystmt->ASTgen().whileComp(condition ->toString()); 
}

void while_stmt::evaluate() {

  value cval;

  cval = condition -> evaluate(); 

  if (cval.type != BVALUE) {
    cout << "Looping condition is non-Boolean" << endl;
    exit(1);
  };

  if (cval.rep.truth) { 
    cout << "looping"<<endl;
    bodystmt -> evaluate(); this -> evaluate();  } 

}


input_stmt::input_stmt(unsigned myid)
  : id(myid) {}

bool input_stmt::isEmpty() { return false; }

void input_stmt::print(int n) {
  output_tabs(n);
  cout << "read " << nametable.get(id) ;

}

AST input_stmt::ASTgen() { 
  return AST("read " + nametable.get(id));
}

CFA input_stmt::CFAgen() { 
  return CFA("read " + nametable.get(id));
}

FCD input_stmt::FCDgen() { 
  return FCD("[io]{ read " + nametable.get(id) +"}");
}



void input_stmt::evaluate() {
  string result;
  bool flag = true; 

  while(flag) 
    {

    cout << "input " << nametable.get(id) << ":" << endl;
    cin >> result;

    //    cout <<"your input: " << result << endl; 

    if (is_integer(result)) 
      {
	state[id].type = iNVALUE;
	state[id].rep.iNumber = std::stoi(result);
	flag = false;
      }
    else if (is_float(result)) 
      {
	state[id].type = fNVALUE;
	state[id].rep.fNumber = stof(result);
	flag = false;
      } 
  }
}



assignment_stmt::assignment_stmt(unsigned myid, exp_node *expression)
  : id(myid), exp(expression) {}

bool assignment_stmt::isEmpty() { return false; }

void assignment_stmt::print(int n) {

  output_tabs(n);
  cout << nametable.get(id) << " := " + exp->toString();
 
}

FCD assignment_stmt::FCDgen() { 
  return FCD("[process]{" + nametable.get(id) + ":=" + exp->toString() +"}");
}

CFA assignment_stmt::CFAgen() { 
  return CFA( nametable.get(id) + ":=" + exp->toString());
}

AST assignment_stmt::ASTgen() { 
  return AST( "{"+nametable.get(id) + ":=" + exp->toString()+"}");
}


void assignment_stmt::evaluate() {
  state[id] = exp->evaluate();
}

print_stmt::print_stmt (exp_node *myexp) : exp(myexp) {}

bool print_stmt::isEmpty() { return false; }

void print_stmt::print(int n) {
  output_tabs(n);
  cout <<  "print " + exp->toString();
}

FCD print_stmt::FCDgen() { 
  return FCD("[io]{ print " + exp->toString() +"}");
}

CFA print_stmt::CFAgen() { 
  return CFA("print " + exp->toString());
}

AST print_stmt::ASTgen() { 
  return AST("print " + exp->toString());
}



void print_stmt::evaluate() {
  value opd;

  opd = exp -> evaluate();
 
  switch (opd.type) 
    { 	
    case BVALUE:
    case FVALUE: cout << "Trying to print  a non-numeric value!" << endl;
	        exit(1);
    case iNVALUE: 
      cout << "output: " << opd.rep.iNumber << endl << endl; 
      break;
    case fNVALUE: 
      cout << "output: " << opd.rep.fNumber << endl << endl; 
      break;
    };

}

skip_stmt::skip_stmt ()  {}

void skip_stmt::evaluate() {}
void skip_stmt::print(int n) {}

FCD skip_stmt::FCDgen() { 
  return FCD("[process]{skip}");
}

CFA skip_stmt::CFAgen() { 
  return CFA("skip");
}

AST skip_stmt::ASTgen() { 
  return AST("skip");
}



bool skip_stmt::isEmpty() { return true; }

sequence_stmt::sequence_stmt(statement* mystmt1, statement* mystmt2): 
  stmt1(mystmt1), stmt2(mystmt2)
{}

bool sequence_stmt::isEmpty() { return false; }


void sequence_stmt::print(int n) {
  stmt1->print(n);  cout << " ;" << endl;
  stmt2->print(n);
}

FCD sequence_stmt::FCDgen() { 
  return stmt1->FCDgen().seqComp(stmt2->FCDgen()); 
}

CFA sequence_stmt::CFAgen() { 
  return stmt1->CFAgen().seqComp(stmt2->CFAgen()); 
}

AST sequence_stmt::ASTgen() { 
  return stmt1->ASTgen().seqComp(stmt2->ASTgen()); 
}


void sequence_stmt::evaluate() {
  stmt1->evaluate();
  stmt2->evaluate();
}



