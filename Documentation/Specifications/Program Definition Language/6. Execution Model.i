    <h2>Execution Model</h2>
    <p>This section captures the reusable interpreter model from the older material where it still matches Arcadia PDL instructions. It is descriptive until the compiler backend or runtime defines the behavior normatively.</p>
    <p>The abstract machine supports string values, boolean values, signed integer values, unsigned natural values, real values, void, and procedure values. Integer and natural values may use 8, 16, 32, or 64 bit widths. Real values may use 32-bit or 64-bit IEEE 754 formats.</p>
    <p>An interpreter may provide a stack. The stack is initially empty and supports querying its size, pushing a value onto the top of the stack, and popping the top value. Popping an empty stack is undefined.</p>
    <p>An interpreter may provide a status variable with the states success and failure. The initial state is success.</p>

    <h3>Calling Convention</h3>
    <p>The old stack calling convention is retained as an abstract model for procedure invocation:</p>
    <ol>
      <li>The caller pushes arguments from left to right.</li>
      <li>The caller pushes the number of arguments.</li>
      <li>The caller pushes the procedure value.</li>
      <li>The invoke operation calls the procedure.</li>
      <li>The procedure pushes zero or more return values from left to right and returns with success or failure.</li>
      <li>The caller removes return values, the procedure, the number of arguments, and the arguments.</li>
    </ol>

    <h3>Arithmetic</h3>
    <p>For unsigned natural values of width <var>n</var> and <var>m</var>, <code>add</code>, <code>subtract</code>, and <code>multiply</code> compute the mathematical result modulo 2<sup>k</sup>, where <var>k = max(n,m)</var>. The result is represented as an unsigned natural value of width <var>k</var>.</p>
    <p>For signed integer values of width <var>n</var> and <var>m</var>, operands are sign-extended to width <var>k = max(n,m)</var>. Addition, subtraction, and multiplication use two's-complement arithmetic and keep the low-order <var>k</var> bits of the mathematical result.</p>
    <p>For division, division by zero raises an arithmetic error. Otherwise the operation computes the quotient according to the selected integer or natural arithmetic model and represents it at width <var>k = max(n,m)</var>.</p>
    <p>The unary <code>negate</code> operation computes zero minus its operand in the operand's arithmetic domain. The unary <code>not</code> operation is the logical negation operation.</p>
