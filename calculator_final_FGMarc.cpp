
/*
    Marc Flavius-Gabriel - Medical Imaging Master's Student
    September 30, 2021
    Solutions for exercises 6.1-3 and 7.1-9
    I use the technique from PPP Style Guide
*/

//
// This is example code from Chapter 6.7 "Trying the second version" of
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

/*
    Simple calculator

    Revision history:

		Revised by Bjarne Stroustrup November 2013
        Revised by Bjarne Stroustrup May 2007
        Revised by Bjarne Stroustrup August 2006
        Revised by Bjarne Stroustrup August 2004
        Originally written by Bjarne Stroustrup
            (bs@cs.tamu.edu) Spring 2004.

    This program implements a basic expression calculator.
    Input from cin; output to cout.

    The grammar for input is:

    Calculation:
        Statement
        Print
        Quit
        Help
        Calculation Statement
    
    Statement:
        Declaration
        Expression
    
    Declaration:
        "let" Name "=" Expression
        "const" Name "=" Expression

    Name:
        letter
        letter Sequence
        Sequence:
        letter
        digit
        "_"
        letter Sequence
        digit Sequence
        "_" Sequence

    Print:
        ";" or "\n"

    Quit:
        "quit"/"Quit"

    Help:
        "help"/"Help"

    Expression:
        Term
        Expression + Term
        Expression - Term

    Term:
        fact_term
        Term * fact_term
        Term / fact_term
        Term % fact_term
        Case k (it only works with + -)

    fact_term
        Primary
        Factorial: tgamma

    Primary:
        "{" "(" Expression ")" "}"
        Name
        Name "=" Expression
        Number
        Factorial
        sqrt_t function:  "sqrt"(" Expression ")"
        power_t function: "power"("Expression "," Integer ")"
        "-" Primary
        "+" Primary
        
    Number:
        floating-point-literal

     Input comes from cin through the Token_stream called ts.
*/

#include "../std_lib_facilities.h"

// declaration for tokens/magic constants
const char number = '8';        
const char quit = 'q';          
const char print = ';';         
const char name = 'a';          
const char let = 'L';           
const char help = 'h';
const char constanta = 'C';
const char sqrt_t = 's';
const char power_t = 'p';
const string power_s = "power";
const string sqrt_s = "sqrt";
const string low_quit = "quit";
const string upp_quit = "Quit";
const string upp_help = "Help"; 
const string low_help = "help";
const string prompt = "> ";
const string result = "= ";     
const string declkey = "let";       // let declaration keyword
const string constantkey = "const";	// constant declaration keyword

//------------------------------------------------------------------------------

class Token {
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value 
    string name;
    Token(char ch)              :kind(ch), value(0) {}      // make a Token from a char      
    Token(char ch, double val)  :kind(ch), value(val) {}    // make a Token from a char and a double
    Token(char ch, string n)    :kind(ch), name(n) {}       // initialize kind and name
};

//------------------------------------------------------------------------------

class Token_stream {
public: 
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
    void ignore(char c); // discard characters up to and including a c
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
:full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

// read characters from cin and compose a Token
Token Token_stream::get()   
{
    if (full) {          // do we already have a Token ready?              
        full = false;    // remove token from buffer
        return buffer;
    }

    char ch;
    cin.get(ch);         // get a character 

    while (isspace(ch)) {
        if (ch=='\n')
            return Token(print);   //if new line is detected, return the print token
        cin.get(ch);
    }

    switch (ch) {
    case print:
    case '(':
    case ')':
    case '{':
    case '}':
    case '!':
    case '%':
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case ',':
    case 'k':
        return Token(ch);        // let each character represent itself
    case '.':                    // a floating-point-literal can start with a dot
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':   // numeric literal
    {
        cin.putback(ch);         // put digit back into the input stream
        double val;
        cin >> val;              // read a floating-point number
        return Token(number, val);
    }
    default:
        if (isalpha(ch) || ch == '_') {     
            string s;
            s += ch;
            while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch=='_')) s+=ch;	// reads letters digits or underscores
            cin.putback(ch);                                                        
            if (s == low_quit || s == upp_quit) return Token(quit);                 // returning token for quit and Quit
            if (s == low_help || s == upp_help) return Token(help);                 // returning token for help and Help
            if (s == declkey) return Token(let);                                    // returning token for let
            if (s == constantkey) return Token(constanta);                          // returning token for const
            if (s == sqrt_s) return Token(sqrt_t);
            if (s == power_s) return Token(power_t);
            return Token(name,s);
        }
        error("Bad token");
    }
}

//------------------------------------------------------------------------------

// to ignore a char. c represents the kind of Token
void Token_stream::ignore(char c)
{
    char ch = 0;
    if (full && c == buffer.kind) {  // first look in buffer:
        full = false;
        return;
    }
    full = false;
    cout << "\nPress ; to continue: ";                        
    while (cin >> ch) {              // now search for input
        if (ch == c) return;
    }
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

//------------------------------------------------------------------------------

class Variable {
public:
    string name;
    double value;
    bool check_var;     // true for variable, false for constant
    Variable(string s, double d, bool var=true) :name(s), value(d), check_var(var) { }
};

//------------------------------------------------------------------------------

// created class Symbol_table (7.4)
class Symbol_table {                                                           
public:
    vector<Variable> var_table;
    double get(string s);
    void set(string s, double d);
    bool is_declared(string var);
    double declare(string var, double val, bool check_var);

};

//------------------------------------------------------------------------------
    
Symbol_table st;      //object of type Symbol_table

//------------------------------------------------------------------------------

// return the value of the Variable names s
double Symbol_table::get(string s) 
{
    for (int i = 0; i<var_table.size(); ++i)    // explore var_table's elements
        if (var_table[i].name == s) return var_table[i].value;
    error("get: undefined variable ", s);
}

//------------------------------------------------------------------------------

// set the Variable named s to d
void Symbol_table::set(string s, double d)
{
    for (int i = 0; i<var_table.size(); ++i)
        if (var_table[i].name == s) {
			if (var_table[i].check_var==false) error(s," is a constant");
            var_table[i].value = d;
            return;
        }
    error("set: undefined variable ", s);
}

//------------------------------------------------------------------------------

// check if var is already in var_table
bool Symbol_table::is_declared(string var)

{
    for (int i = 0; i<var_table.size(); ++i)
        if (var_table[i].name == var) return true;
    return false;
}

//------------------------------------------------------------------------------

// add (var,val,check_var) to var_table
double Symbol_table::declare(string var, double val, bool check_var = true)
{
    if (check_var == true) {
        if (is_declared(var)) { 
            for (int i = 0; i < var_table.size(); ++i)
                if (var_table[i].name == var) {
                    if (var_table[i].check_var == false) error(var, " is a constant");  // check if the variable was declared using let. If not -> call error
                    var_table.erase(var_table.begin() + i);                             // if the variable was declared using let, erase the old value
                }
            var_table.push_back(Variable(var, val, check_var));                         // push the new variable 
            return val;
        }
        else {
            var_table.push_back(Variable(var, val, check_var));                         // if the variable is not declared, push the variable
            return val;
        }
    }
    else {
        if (is_declared(var)) error(var, " declared twice");                           // if the user tries to declare the same constant/let variable -> throw error
        var_table.push_back(Variable(var, val, check_var));                            // if the first declaration is a const, push the variable
        return val;
    }
}

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()
double primary();       // used in sqrt_f()

//------------------------------------------------------------------------------

// deal with power function
double pow_f(Token t) {
    t = ts.get();
    if (t.kind != '(') {
        error("'(‘ expected");
    }
    double d = expression();
    t = ts.get();
    if (t.kind != ',') {
        error("',' expected");
    }
    double n = expression();
    t = ts.get();
    if (t.kind == ')') {
        return pow(d, n);
    }
    else {
        error("Expected ')'");
    }
}

//------------------------------------------------------------------------------

// deal with sqrt function
double sqrt_f(Token t) {
    t = ts.get();
    if (t.kind != '(') {
        error("'(‘ expected");
    }
    ts.putback(t);
    double d = primary();
    if (d < 0.0) {
        error("not possible -> negative square root");
    }
    return sqrt(d);
}

//------------------------------------------------------------------------------

// deal with factorial(!)
double factorial(double value1) {
    //int value_int = static_cast<int>(value1);         // convert double to int as indicated in the Hint from ex6.3
    if (value1 <= 1)                                    // for 0! return 1 
        return 1;
    //return value1 * factorial(value_int - 1);         // compute factorial for int
    return tgamma(value1 + 1);                          // compute factorial for doubles using tgamma function
}

//------------------------------------------------------------------------------

// deal with connecting term and primary 
double fact_term()
{
    double left = primary();
    Token t = ts.get();

    switch (t.kind) {
    case '!':
    {   
       double rez = factorial(left);
       return rez;
    }
    default:
        ts.putback(t);
        return left;
    }
}


//------------------------------------------------------------------------------

// check for factorial 
bool check_factorial() {
    Token t = ts.get();
    if (t.kind == '!') {                                // factorial symbol is found, then put t back in the token stream and return true
        ts.putback(t);
        return true;
    }
    ts.putback(t);                                      // otherwise put t back in the token stream and return false
    return false;
}

//------------------------------------------------------------------------------

// deal with numbers, parentheses, names, factorial
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '{':    // handle '{' expression '}'
    {
        double d = expression();
        t = ts.get();
        if (t.kind != '}') error("'}' expected");
        return d;
    }
    case '(':    // handle '(' expression ')'
        {    
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
    case name: {
        Token next_t = ts.get();
        if (next_t.kind == '=') {	// handle name = expression
            double d = expression();
            st.set(next_t.name, d);
            return d;
        }
        else {
            ts.putback(next_t);		// not an assignment: return the value
            return st.get(t.name);  // return the variable's value
        }

    }
    case number: case '!': {
        // check for factorial
        if (check_factorial()) {
            //Token t = ts.get();
            double d = factorial(t.value); // compute factorial
            t = ts.get();                  // get next token
            return d;                      // return computed factorial
        }
        else {
            return t.value;                // return the number's value
        }
    }
    case sqrt_t:
    {
        return sqrt_f(t);
    }
    case power_t:
    {
        return pow_f(t);
    }
    case '-':
        return -primary();
    case '+':
        return primary();
    
        
    default:
        error("primary expected");
    }
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
    double left = fact_term();
    Token t = ts.get();                   // get the next token from token stream

    while(true) {
        switch (t.kind) {
        case 'k':
        {
            left = left * 1000;
            t = ts.get();
            break;
        }
        case '*':
        {
            left *= fact_term();
            t = ts.get();
            break;
        }
        case '/':
        {    
            double d = fact_term();
            if (d == 0) error("divide by zero");
            left /= d; 
            t = ts.get();
            break;
        }
        case '%':
        { 
            double d = fact_term();
            if (d == 0) error("divide by zero");
            left = fmod(left, d);    // calculate % using fmod
            t = ts.get();
            break;
        }
        default: 
            ts.putback(t);              // put t back into the token stream
            return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while(true) {    
        switch(t.kind) {
        case '+':
            left += term();    // evaluate Term and add
            t = ts.get();
            break;
        case '-':
            left -= term();    // evaluate Term and subtract
            t = ts.get();
            break;
        default: 
            ts.putback(t);     // put t back into the token stream
            return left;       // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------

// handle: name = expression
// declare a variable called "name” with the initial value "expression”
// m will be "let" or "constant"
double declaration(Token m)
{
    Token t = ts.get();
    if (t.kind != name) error("name expected in declaration");
    string var_name = t.name;
    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", var_name);
    double d = expression();
    st.declare(var_name, d, m.kind==let);
    return d;
}

//------------------------------------------------------------------------------

// deal with declaration and expression
double statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case let:
    case constanta:
        return declaration(t.kind);
    default:
        ts.putback(t);
        return expression();
    }
}

//------------------------------------------------------------------------------

// function to ignore print character
void clean_up_mess()           
{
    ts.ignore(print);
}

//------------------------------------------------------------------------------

// expression evaluation loop
void calculate() 
{
    cout << ">>>>>>>>>>>>>>>>>>>>Calculator<<<<<<<<<<<<<<<<<<<< by FG Marc\n"
        << "Press enter or ; at the end of your expression to calculate.\n"
        << "For help, enter Help or help. Enter quit or Quit, for exiting the program.\n";
    while (cin) 
        try {
            cout << prompt;
            Token t = ts.get();

            while (t.kind == print) {        // first discard all "prints"
                t = ts.get();
            }
            if (t.kind == help) {
                cout << "Further information on how to use the calculator: \n"
                    << "-you can introduce expressions separated with `space` or `;` and they will be evaluated after you press `enter` \n"
                    << "-you can use +-*/%=!(){},power('value1','value2'),sqrt('value') in your expressions \n"
                    << "-you can use k in your + and - expressions if you want to multiply a value with 1000 \n"
                    << "-you can declare a variable that you want to change later using `let variable = value` \n"
                    << "-if you want to change a variable declared as let use `let new_variable = new_value` \n"
                    << "-if you want to declare a variable that you don't want to change later use `const variable = value` \n"
                    << "-you cannot change your variable from `let` to `const` and vice versa \n"
                    << "-Press ; if you want to resume from a computation error \n";
                t = ts.get();
            }
            else {
                if (t.kind == quit) return;
                ts.putback(t);
                cout << result << statement() << '\n';
            }
        }
        catch (exception& e) {
            cerr << e.what() << '\n';       // write error message
            clean_up_mess();
        }
}

//------------------------------------------------------------------------------

int main()
try
{
    // predefined names:
    st.declare("pi", 3.1415926535, false); // set to false because they are constants
    st.declare("e", 2.7182818284, false);

    calculate();
    keep_window_open();                    // cope with Windows console mode
    return 0;
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    keep_window_open("~~");
    return 1;
}
catch (...) {
    cerr << "exception\n"; 
    keep_window_open("~~");
    return 2;
}

//------------------------------------------------------------------------------