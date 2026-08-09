# The tiny interpreter specification
Specification of a tiny interpreter.

## The values
The machine supports *string values* (UTF8 strings), *boolean values* (two values, indicating logically true or logically false),
*integer values* (8, 16, 32, or 64 bit representing a two's complement binary number) and *natural values* (8, 16, 32, or 64 bit
representing unsigned binary numbers), *real values* (32 or 64 bit IEEE754 floating point numbers). Lastly, it supports
*procedure values* (C functions which follow conventions to be invocable by the interpreter).

## The stack
The interpreter provides a stack.
The stack is a list of values.
This list is initially empty.

The stack supports three operations:

- get size operation: Get the number of elements on the stack.
Getting the size of the stack is implemented in terms of getting the size of the list..

- push operation: Pushes a value on the stack. 
Pushing a value on the top of the stack is implemented in terms of inserting a value at the end of the list.

- pop operation: Pops a value from the top of the stack.
Popping a value from the top of the stack is implemented in terms of removing the value at the end of the list.
The behavior of pop is undefined if the stack is empty.

## The status variable
The interpreter provides an error variable with the status *Success* or *Failure*.
The intiial state is *Success*.

## Instructions and Values
The interpreter receives instructions parameterized by values which guide its operations.

Example:
A potential C interface of the interpreter allowing for executing the instruction for pushing a *string value* on the stack.
```
Result Interpreter_pushString(Interpreter*, String*);
```

Example:
A potential  C interface of the interpreter allowing for executing the concatentation instruction.
This instruction concatenates the topmost two value on the stack.
```
Result Interpreter_concatenate(Interpreter*);
```

Example:
A potential  C interface of the interpreter allowing for executing the invoke instruction.
This instruction invokes a procedure with arguments.
```
Result Interpreter_invoke(Interpreter*, Procedure* procdure, Value const* value, size_t numberOfValues)
```

## The stack calling convention non-exceptional case
The stack calling convention defines how the stack is used to call procedures.

- The caller pushes the arguments from left to right on the stack.
- The caller pushes number of arguments on the stack.
- The procedure to invoke is pushed on the stack.

- The *invoke* instruction is sent.
  The procedure pushes zero or more return values from left to right on the stack.
  It then returns with success (setting the status variable to the success state) or failure (setting the status variable to the error state).

- The caller is responsible for removing the return values, the procedure, the number of arguments, and the arguments.

## Operations

Given an integer x reduced by modulo y means.

Solve the equation
 x = n * q + r
such that 
r| < |n|, n != 0.

Furthermore, sgn(x) = sgn(n).

### Arithmetic Operations over Natural types
The following binary arithmetic operations are known: *Add*, *Subtract*, *Multiply*, *Divide*.

For *Add*, *Subtract, and *Multiply*:
Let the two arguments `x` of type `Natural`*n*  and `y` of type `Natural`*m* be the first and the second operand of a binary
arithmetic operation where *n*,*m* in *{8,16,32,64}*. Let *k = max(n,m)*. Perform the mathematical operation as if they were
mathematical integers and reduce the sum or difference by *module 2^k*. This mathematical result is represented by a
`Natural`*k* constituting the result of the operation.

The following unary arithmetic operations are known: *Affirm* and *Negate*.
The result of the *Affirm* operation for any `Natural`*n* type value is that value.

Let `x` be the single argument of the *Negate* operation of type `Natural`*n* where *n* in *{8,16,32,64}*.
The result of the operation is the result of the *Subtract* operation of `c` and `x` where `c` is the zero
value of type `Natural`*n*. 

The following binary arithmetic operation is known: *Divide*.
Let the two arguments `x` of type `Natural`*n*  and `y` of type `Natural`*m* be the first and the second operand of the
*Divide* operation where *n*,*m* in *{8,16,32,64}*. Let *k = max(n,m)*. If `y` is zero, then raise an arithmetic error.
Otherwise perform the mathematical operation of floored division as if they were mathematical integers and reduce the 
quotient by *module 2^k*. This mathematical result is represented by a `Natural`*k* constituting the result of the operation.
 
### Arithemtic Operation over Integer types

following binary arithmetic operations are known: *Add*, *Subtract*, *Multiply*, *Divide*, *Affirm*, and *Negate*
and 

For *Add*, *Subtract, and *Multiply*:
Let the two arguments `x` of type `Natural`*n*  and `y` of type `Natural`*m* be the first and the second operand of a binary
arithmetic operation where *n*,*m* in *{8,16,32,64}*. Let *k = max(n,m)*. If n < k sign-extend x to k. If m < k sign-extend
y to k. Perform the operation according to the rules of two's complement corresponding operation.
mathematical integers and reduce the sum or difference by *module 2^k*. This mathematical result is represented by a
`Natural`*k* constituting the result of the operation.

The following unary arithmetic operations are known: *Affirm* and *Negate*.
The result of the *Affirm* operation for any `Natural`*n* type value is that value.

Let `x` be the single argument of the *Negate* operation of type `Natural`*n* where *n* in *{8,16,32,64}*.
The result of the operation is the result of the *Subtract* operation of `c` and `x` where `c` is the zero
value of type `Natural`*n*. 

The following binary arithmetic operation is known: *Divide*.
Let the two arguments `x` of type `Natural`*n*  and `y` of type `Natural`*m* be the first and the second operand of the
*Divide* operation where *n*,*m* in *{8,16,32,64}*. Let *k = max(n,m)*. If `y` is zero, then raise an arithmetic error.
Otherwise perform the mathematical operation of floored division as if they were mathematical integers and reduce the 
quotient by *module 2^k*. This mathematical result is represented by a `Natural`*k* constituting the result of the operation.
 
 
### add (integer values)
Let the two arguments x and y be *integer values*.
Sign extend the integer value with the shorter bit width to the bit width of the longer integer value.




If an integer addition overflows, then the result is the low-order bits of the mathematical sum as represented in some sufficiently large two's-complement format.
If overflow occurs, then the sign of the result is not the same as the sign of the mathematical sum of the two operand values. 

### add (natural values)
Let the two arguments x and y be *natural values*.
Extend the natural vaue with the shorter bit width to the bit width of the longer natural value.

If a natural addition overflows, then the result is the lower-order bits mathematical sum as representetd in some sufficiently large format.

### subtract (integer values)
Let the two arguments x and y be *integer values*.
Sign extend the integer value with the shorter bit width to the bit width of the longer integer value.

If an integer addition overflows, then the result is the low-order bits of the mathematical sum as represented in some sufficiently large two's-complement format.
If overflow occurs, then the sign of the result is not the same as the sign of the mathematical sum of the two operand values.

### subtract (natural values) 

### multiply (integers)
Let the two arguments x and y be *integer values*.
Sign extend the integer with the shorter bit width to the bit width of the longer integer.

If an integer multiplication overflows, then the result is the low-order bits of the mathematical product as represented in some sufficiently large two's-complement format.
As a result, if overflow occurs, then the sign of the result may not be the same as the sign of the mathematical product of the two operand values. 


### Mathematical Kernel
The operation `add`, `subtract`, `multiply` are defined for two's complement binary integers and unsigned binary integers:

### Auxilioary Definition: The Modulus
The modulus of an integer a and positive integer b
```
a mod b
```
is computed by
```
a = n * b + r, |r| < |n|, sgn(n) = sgn(y)
```

### Addition
- Let `x` and `y` be the unsigned binary integers of width n and m, respectively, with n,m > 0.
- Let *k = max(n,m)*.
- Compute
  *c := a + b mod 2^k*
  where *a* and *b* is the representation of `x` and `y`, respectively, in *Z*.
- *c* is representable by an unsigned binary integer of width *k*.
- The result of the operation is the representation of *c* as an unsigned binary integer of width *k*.  

### Subtraction
- Let `x` and `y` be the unsigned binary integers of width n and m, respectively, with n,m > 0.
- Let *k = max(n,m)*.
- Compute
  *c := a - b mod 2^k*
  where *a* and *b* is the representation of `x` and `y`, respetively, in *Z*.
- *c* is representable by an unsigned binary integer of width *k*.
- The result of the operation is the representation of *c* as an unsigned binary integer of width *k*.  

### Multiplication  
- Let `x` and `y` be the unsigned binary integers of width n and m, respectively, with n,m > 0.
- Let *k = max(n,m)*.
- Compute
  *c := a \* b mod 2^k*
  where *a* and *b* is the representation of `x` and `y`, respetively, in *Z*.
- *c* is representable by an unsigned binary integer of width *k*.
- The result of the operation is the representation of *c* as an unsigned binary integer of width *k*.  