# PMI_Calculator

Solutions for exercises 6.1-3 and 7.1-9 from "Programming: Principles and Practice Using C++ (second edition), Bjarne Stroustrup, ISBN: 9780321992789"

Ch6.1:  
	1. For the first `try this` having the actual knowledge of that point in the chapter. 
	Q: How would we deal with a complicated expression, such as 1+2*3/4%5+(6–7*(8))? And how would we deal with errors, such as 2+*3 and 2&3?
	A: We cannot create a `token` for specific expressions like for example 2+3, because then we will need to create many different `token` combinations of expressions based on the complexity of expression.
	A: Based on the principle "Divide et impera" the only answer I could see is to assign a token for each character of the expression and in this way to "conquer" a future complex expression. This can also treat the exception for when the user inputs 2+*3.
	
	2. Q: The second `try this` of the chapter6 required me to run the calculator00.cpp program and see how it goes. 
	A: I managed to reproduce the behavior of this program by following the steps in the book. The program is not perfect, but I am on the right track. Token_Stream has to be implemented to store an actual token and also, I need a putback() function to put the token in the right place and not allowing `expression` or `term` "to eat" not used tokens.
	
	3. Q: The third `try this` was about getting calculator01.cpp to run, see what it does and say Why it works as it does?
	A: Based on grammar rules, we have 4 functions: get(), expression(), term() and primary(). 
	-get() returns an object of type "Token", it is declared in the Token_stream class as public and its definition is outside the class. (it reads characters and compose tokens)
	-expression() returns a double value and calls get() and term(). If the token is not used, the putback function is called for putting the token back into the token stream (it deals with + and –)
	-term() returns a double value and calls get() and primary(). The same as for expression, If the token is not used, the putback function is called for putting the token back into the token stream. (it deals with * and /)
	-primary() returns a double value and calls get() and expression(). (it deals with numbers and parentheses)
	
	So, 1. in the main function, while all previous operations on cin have succeeded continue to loop (while(cin)), and then, by calling the get() function, each character of the expression is tokenized. If the user enters q, the while loop will break/close the execution of the program. For ;, the actual value is printed on the screen. The value is assigned by calling expression() for each loop.
	2. When the get function gets called the tokenization of all the characters begins by prompting the user to enter a character (cin >> ch). For all the operators including ; and q, by calling the Token(ch) method defined in the Token class as public, we let the operators represent itself.  For the numbers including . , we push back the ch input into cin and then we get the value back as a double. The value is then "reported" as an '8' through Token(char, value) public method. If no token was recognized, throw error "Bad Token". 
	3. Functions expression(), term() and primary() are then called depending on the expression introduced by the user with the conditions that term() cannot call the expression() and primary() cannot call term().
	3. Depending on the operations, the value of the `left` variable which contains the solution, will be returned in the end by the expression() function.
	4. For a successful program some other conditions are fulfilled: (the Token_stream)ts is declared before ts.get() and also expression is declared before primary() is being defined, because it is called in primary(). 
	
	
Ch6.2: Q: Add the ability to use {} as well as () in the program
	   A: added new characters in the get() function, changed Primary() function with adding a '{' case, similar with the '(' case.
	   
Ch6.3: Q: Add a factorial operator: use a suffix ! operator to represent “factorial.”
	   A: as indicated, I changed the grammar to account for a high level operator, so I created another function called factorial(double value1) <it has one input parameter>. Also, I have created a function named check_factorial needed in the primary() function. So, for each number, we check to see if after it there is a ! symbol found. If ! is found, then call factorial() function. Also, added fact_term, to make (2+1)! work, as well as (2*1)! (term and expressions)
	   Solution1: there are 2 commented lines in the factorial function because First time, I have computed the factorial only for integers(so, casting doubles to int), as mentioned in the exercise.
chosen     Solution2: Because it is requested to verify our calculator results with WolframAlpha (which can calculate factorial for doubles), I changed my program to compute double values also. It is possible with the gamma function (tgamma()) from math.h header file, included in std_lib_facilities.h. More details on the gamma function https://math.stackexchange.com/questions/454053/how-do-we-calculate-factorials-for-numbers-with-decimal-places
	   
	   
Ch7.1: Q: Allow underscores in the calculator’s variable names.
	   A: added changes to get() function.
Ch7.2: Q: Provide an assignment operator, =, so that you can change the value of a variable after you introduce it using let.
	   A: added in Primary(), otherwise we would get a "primary expected" error and we cannot compute x+y*2, when let x = value and let y = value were used.
	   A: also, here i implemented in define_name() the possibility for the user to change the variable's value by erasing the actual value in the var_table. So, if you do let x = 2; , after, you can do let x = 3; and the last value x takes will be 3. You can also declare a variable as const, for example const x = 1; and then let x = 2 is not possible anymore (call error-> x is a constant).
	   A: it is useful if you want to change a value assigned by mistake to a variable or maybe later in time, there will be new requirements for having a different value. In this case is good to have access to the variable's value.
	   A: It can be a source of problems when the user defines a variable (z) using other variables(e.g. x,y) and he wants to change the variables x and y for a new result z. For example if the user defines let x = 3 and let y = 4, if he wants to compute let z = x+y, the answer will be z = 7 but if afterwards the user wants to change the value of x or y, he has to compute again z (let z) which doesn't change automatically.
Ch7.3: Q: Provide named constants that you really can’t change the value of of.
	   A: I made this by defining "constant" as a token, as indicated I modified the Variable to hold constant/variable information, and a check in set_value and declare_value for stopping to assign to a constant.
Ch7.4: Q: Define a class called Symbol_table
	   A: Symbol_table created with all members public, also I have created an object `st` of type Symbol_table to access the functions inside the class (E.g. st.get())
	   A: renamed members inside Symbol_table as required, now we have get(), set(), is_declared() and declare() note: define_name became declare().
Ch7.5: Q: Modify Token_stream::get() to return Token(print) when it sees a newline.
	   A: I had to change the cin >> ch because it was skipping new lines with cin.get(ch) which takes each character individually. Then, as recommended,  I created the while loop having isspace(ch) as an argument, which returns true if ch is a whitespace character.
Ch7.6: Q: Have the calculator print out some instructions (H)
	   A: Implemented if condition in calculate() and added token for h and H in token_stream::get() function.
Ch7.7: Q: Change the q and h commands to be quit and help, respectively.
	   A: Created string variables for help and quit and (extra) for Quit and Help. also, changed the token_stream::get function's default case.
Ch7.8: Q: Update Grammar 
	   A: Updated with Calculation  and Sequence formation.
Ch7.9: Q: 3 Improvements
done   A1: added tgamma function for factorial! so now it works for double also like it does with WolframAlpha calculator! see: https://math.stackexchange.com/questions/454053/how-do-we-calculate-factorials-for-numbers-with-decimal-places   . Changed grammar for fact_term , Factorial
done   A2: added 'k' for when you want to multiply a value with 1000, for example if you want 2k, the result will be 2*1000. Help text updated, Grammar updated. It only works with + - expressions. To be further implemented for terms also...
       A3: extending the program to solve equations (for example finding x or y from an expression) 
	   
Other changes done: -implement sqrt function: implemented sqrt_f () which is called in Primary on the specific case for sqrt_t. Updated Grammar.
-implement pow function: implemented pow_f () which is called in Primary on the specific case for power_t. Updated Grammar.
-added Help and Quit commands with capital letters. Updated Grammar.
	  
