ORK (Objects R Kool) language
==============================

The ORK language is a verbose Object Oriented language created by Gregor Richards ([ORK Website](http://codu.org/eso/#ORK)). The last
version seems to be released in 2005.

I only modified the compiler to allows forward references between classes in  function definition and to add some utility sh scripts. Here is the original README.

-------
## Language Spec.

The ORK (Objekts R Kool) language is insanely verbose but not actually very useful.  It's object
oriented beyond all possible usefulness, and makes writing even the most trivial programs long
and complex.  Its divine purpose in existance is to be the polar opposite of BrainFuck.

So let's begin!

Please note that this introduction is really only intended for people who already know C++ or
maybe Java, but, to say the least, are comfortable with object-oriented programming.  A little bit
of masochism doesn't hurt either.

What people who write C++ would know as the **main function** is started with the line:

```
When this program starts:
```

All functions are instantiated this way.  There's a `When` line, then the "code," then a blank
lineto end the function.  Here's an example (you can ignore the code, just pay attention to the
`When` lines):

```
  When this program starts:
  I have a dog called Bob.
  Bob is to bark.

  When a dog is to bark:
  My voicebox is to say "Woof!\n"
```

This translates to something like:
```
  int main() {
          dog Bob;
          Bob.bark();
  }

  void dog::bark() {
          cout << "Woof!\n";
  }
```

Within a function, you can instantiate objects and run objects' functions.  There is no
functionality beyond that - all operations must be done through objects.
**To instantiate an object**, use a line like either:
  ```
  I have a[n] {object} called {name}.
  ```
or
  ```
  There is a[n] {object} called {name}.
  ```
From then on in that function, that object can be referred to by its name.
For example:
  ```
  I have a dog called Bob.`
 ```
or
  ```
  There is a scribe called Will.
  ```

**To call a function from an object**, use
  ```
  {object} is to {function} [{parameter}].
  ```

Here's an example of that, and an introduction to the built-in scribe object.
```
  When this program starts:
  There is a scribe called Will.
  Will is to write "Hello World!\n"
```
This would simply output "Hello World!"

You can also **reference variables within an object**, such as:
  ```
  Fibonacci's first operand
  ```
That would be equivalant to "Fibonacci.first_operand" in C++.  **You can set a variable** (whether
within an object or not) by:
  ```
  [My] {variable} is {variable|value}
  ```
For example:
  ```
  Fibonacci's first operand is 1.
  ```


So now you ought to be able to write a very simple program.  However, you can't do anything very
complex without classes, since ORK is, after all, mindlessly object oriented.
**To declare a class**, use the line:
```
  There is such a thing as a[n] {name}.
```
Between that line and the next blank line, you put all the class' functions, etc.
**To declare a function within a class**:
  ```
  A {class} can {function} [a[n] {parameter type}].
  ```
For example:

```
  There is such a thing as a scribe.
  A scribe can write a phrase.
```


**To declare a class variable**:
  ```
  A {class} has [a[n]] {variable name} which is a {variable type}.
  ```
For example:

```
  There is such a thing as a mathematician.
  A mathematician has a first operand which is a number.
```


**Other than objects, there are only two variable types**:
1. **number**.  This translates to "double," so 1, 2, 3.5, -15, etc.
2. **word**, **phrase** or **sentence**.  All of these translate to "string."


**To instantiate a class' function**, use:
  ```
  When a {class} is to {function} [a[n] {variable type}]:
  ```
You can then refer to the argument variable by:
  ```
  the {variable type}
  ```
For example:

```
  When a man is to speak a phrase:
  There is a scribe called Will.
  Will is to write the phrase.
```


You should now know the entire syntax of the language.  So, now it's time for built-in classes.
Firstly, you need to learn the scribe class.  This one is so easy, I'll just give an example:
```
  There is a scribe called Will.
  I have a phrase called Hamlet.
  Hamlet is "To be, or not to be?\n"
  Will is to write Hamlet.
```

**The scribe class** only has the "write" function, and the "asciiWrite" function.  The asciiWrite
function outputs a character from its ASCII value (a number).


Now let's go on to the mathematician class.  Because you don't have mathematical operators like +
or -, you have to give the values to a mathematician and have him do the calculations for you.  So,
if you wanted to add 5 and 5 and write the result:
```
  There is a scribe called Will.
  There is a mathematician called Fibonacci.
  Fibonacci's first operand is 5.
  Fibonacci's second operand is 5.
  Fibonacci is to add.
  Will is to write Fibonacci's result.
```
**The mathematician class** also has subtract, multiply, divide, modulo and compare.  The compare
function will be explored later.


To receive input, there is a class called "inputter."  Inputter can:
* read a phrase.
* read a number.
* readOne a phrase.
* readOne a number.
* say it's done.
The read functions read a word into a string.  The read a number version translates that string
into a number.  The readOne functions read a single character.  The readOne a number version puts
the ASCII value into the number.  It says it's done when it receives EOF.


**The linguist class** is the string variant of the mathematician class.  It has a first and second
operand, a result, and two functions:
* concatenate.
* compare.
The concatenate functions concatenates the first and second operand into the result.  The compare
function compares the values of the operands and says it's equal if they're equal, and says it's
not equal if they're not equal.


Sorry, I still haven't covered arrays!  **Read libork.h** ;)


Now, the final bit - program logic.
To compare two numbers, use a mathematician and ask him to compare.  For example:
```
  There is a scribe called Will.
  There is a mathematician called Fibonacci.
  Fibonacci's first operand is 5.
  Fibonacci's second operand is 5.
  Fibonacci is to compare.
  If Fibonacci says it's less then Will is to write "Less!\n"
  If Fibonacci says it's equal then Will is to write "Equal!\n"
  If Fibonacci says it's greater then Will is to write "Greater!\n"
```
Naturally, this would output "Equal!"

Loops are a bit more complex.  Only an entire function can loop, so generally you want to create a
loop by:
1. Creating the function you want to loop
1. Calling it with "If {condition} then I am to {function}"
1. Looping at the end with the same condition.

When you write "I am to loop," the current function loops, with the same parameters, in the same
class, etc.
So, for example:
```
  When looper is to loop a number:
  There is a mathematician called Fibonacci.
  I have a number called current.
  Fibonacci's first operand is the number.
  Fibonacci's second operand is 1.
  Fibonacci is to subtract.
  current is Fibonacci's result.
  Fibonacci's first operand is Fibonacci's result.
  Fibonacci's second operand is 0.
  Fibonacci is to compare.
  If Fibonacci says it's greater then I am to loop.
```
This would loop from the value you gave it down to 0.  You would of course want to add some sort of
functionality there, too.


ORK has a minimal level of pointer support.  **To put a pointer to an object in a class**, use the line
  ```
  A {class} can have [a[n]] {variable} which is a {type}.
  ```
for example:
  ```
  A linear linked list can have a next which is a linear linked list.
  ```

This pointer can be instantiated simply by setting it:
  ```
  My next is b's next.
  ```
or by instanciating it by one of several means:
  * I have a next which is a linear linked list.
  * b has a next which is a linear linked list.
  * b's next's next's next's next's next has a next which is a linear linked list.


## ORK Compiler Usage
And now, on the ORK compiler itself:
Use
```
ork {program}
```
to compile a program.  C++ code will be outputted to stdout, so
```
ork {program} > out.cc
```
will give you a proper C++ file.

To compile the C++ file, you need a line similar to this one:
```
g++ out.cc -I/opt/ork /opt/ork/libork.a
```
Where _/opt/ork_ is your ORK directory.

## Addition

By looking at the source code, you can **inherits from another class**:
```
Every {object} is a[n] {Object}.
```
