      <h2>Semantical Translation</h2>
      <p>The semantical translation resolves the names of a sentence provided by the syntactical translation and binds every declaration to a type.</p>

      <h3>Type Names</h3>
      <p>Type names are fully qualified, and there is no relative resolution of type names. A type name is matched against the fully qualified names of the compilation as it is written, so <code>Arcadia.Natural32</code> denotes a primitive type and <code>MyModule.Greeter</code> denotes the type declared with the qualified name <code>MyModule.Greeter</code>. A type name that does not spell out the fully qualified name of a type is not defined; for example, <code>Greeter</code> does not denote the type <code>MyModule.Greeter</code>. The same rule applies wherever the grammar expects a type name: a class <code>extends</code> or <code>implements</code> clause, an interface <code>extends</code> clause, a <code>variable</code> or parameter type, and the return type of a procedure, constructor, or method.</p>

      <h3>Member Names</h3>
      <p>A member of a class type is looked up by its declared name, unqualified, in the scope of the class type that declares it. A member name, unlike a type name, is never fully qualified.</p>

      <h3>Built-in Types</h3>
      <p>The following types are built in to every compilation. They are always spelled with the fully qualified <code>Arcadia.</code> prefix:</p>
      <ul>
        <li><code>Arcadia.Boolean</code></li>
        <li><code>Arcadia.Integer8</code>, <code>Arcadia.Integer16</code>, <code>Arcadia.Integer32</code>, <code>Arcadia.Integer64</code></li>
        <li><code>Arcadia.Natural8</code>, <code>Arcadia.Natural16</code>, <code>Arcadia.Natural32</code>, <code>Arcadia.Natural64</code></li>
        <li><code>Arcadia.Object</code></li>
        <li><code>Arcadia.Real32</code>, <code>Arcadia.Real64</code></li>
        <li><code>Arcadia.Void</code></li>
      </ul>
      <p><code>Arcadia.Object</code> is a class type; the other built-in types are primitive types.</p>
      <p>A built-in type is provided by the runtime rather than by a module, so it is not generated: the compiler emits no source or header file for a built-in type, and instead binds a reference to it to the corresponding runtime type.</p>

      <h3>Native Definitions</h3>
      <p>A procedure, constructor, or method definition may carry an optional <code>native</code> clause. The clause takes a string literal naming the native target. A definition that carries the clause is a <em>native definition</em>; a definition that omits the clause is not native, even though it declares the same kind of member.</p>
      <p>The string literal names the native target verbatim. It is an ordinary C identifier and is not subject to the qualified-name rules that apply to type names and to defined procedure, constructor, and method names. In particular, the rewriting that derives an implementation name from a fully qualified name by replacing the <code>.</code> separator does not apply to a native target name.</p>
      <p>A native definition has no body to compile, because its body is provided by the named target. A definition without a <code>native</code> clause is provided with a compiler-generated body, which the current implementation leaves unimplemented.</p>
      <p>A built-in type may declare members, and such a member is native without a <code>native</code> clause appearing in any compilation unit: a member supplied by a built-in type is native by being supplied, and not by carrying the optional clause.</p>

      <h3>Interface Operations</h3>
      <p>Interface operations are method declarations. The syntactical translation accepts the same surface method form as class methods, but semantical translation rejects an interface operation that has a method body.</p>

    <h3>Instruction Semantics</h3>
    <p>Arcadia PDL is currently dynamically typed. The semantical translation does not bind an instruction operand to a type, so the descriptions below state which operand forms an instruction accepts and which effect it has on the values of its operands at run time. An instruction raises an error when it cannot be applied to the values of its operands.</p>

    <h4>Arithmetic Instructions</h4>

    <h5>add</h5>
    <p>The <code>add</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation adds the value of the 3rd operand to the value of the 2nd operand and stores the result in the 1st operand.
    If the values cannot be added, then an error is raised.</p>

    <h5>subtract</h5>
    <p>The <code>subtract</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation subtracts the value of the 3rd operand from the value of the 2nd operand and stores the result in the 1st operand.
    If the values cannot be subtracted, then an error is raised.</p>

    <h5>multiply</h5>
    <p>The <code>multiply</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation multiplies the value of the 2nd operand with the value of the 3rd operand and stores the result in the 1st operand.
    If the values cannot be multiplied, then an error is raised.</p>

    <h5>divide</h5>
    <p>The <code>divide</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation divides the value of the 2nd operand by the value of the 3rd operand and stores the result in the 1st operand.
    If the values cannot be divided, or if the value of the 3rd operand is zero, then an error is raised.</p>

    <h5>negate</h5>
    <p>The <code>negate</code> instruction expects two operands.
    The 1st operand must be a register, then 2nd operand must be a register or a literal.
    The operation negates the value of the 2nd operand and stores the result in the 1st operand.
    If the value cannot be negated, then an error is raised.</p>

    <h4>Concatenation Instructions</h4>

    <h5>concatenate</h5>
    <p>The <code>concatenate</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation concatenates the value of the 3rd operand to the value of the 2nd operand and stores the result in the 1st operand.
    If the values cannot be concatenated, then an error is raised.</p>

    <h4>Logic Instructions</h4>

    <h5>and</h5>
    <p>The <code>and</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores the logical conjunction of the values of the 2nd and the 3rd operand in the 1st operand.
    If the values cannot be combined by a logical conjunction, then an error is raised.</p>

    <h5>or</h5>
    <p>The <code>or</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores the logical disjunction of the values of the 2nd and the 3rd operand in the 1st operand.
    If the values cannot be combined by a logical disjunction, then an error is raised.</p>

    <h5>not</h5>
    <p>The <code>not</code> instruction expects two operands.
    The 1st operand must be a register, then 2nd operand must be a register or a literal.
    The operation stores the logical negation of the value of the 2nd operand in the 1st operand.
    If the value cannot be logically negated, then an error is raised.</p>

    <h4>Relational Instructions</h4>

    <h5>isEqualTo</h5>
    <p>The <code>isEqualTo</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores true in the 1st operand if the values of the 2nd and the 3rd operand are equal, and false otherwise.
    If the values cannot be compared, then an error is raised.</p>

    <h5>isNotEqualTo</h5>
    <p>The <code>isNotEqualTo</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores true in the 1st operand if the values of the 2nd and the 3rd operand are not equal, and false otherwise.
    If the values cannot be compared, then an error is raised.</p>

    <h5>isLowerThan</h5>
    <p>The <code>isLowerThan</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores true in the 1st operand if the value of the 2nd operand is lower than the value of the 3rd operand, and false otherwise.
    If the values cannot be compared, then an error is raised.</p>

    <h5>isLowerThanOrEqualTo</h5>
    <p>The <code>isLowerThanOrEqualTo</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores true in the 1st operand if the value of the 2nd operand is lower than or equal to the value of the 3rd operand, and false otherwise.
    If the values cannot be compared, then an error is raised.</p>

    <h5>isGreaterThan</h5>
    <p>The <code>isGreaterThan</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores true in the 1st operand if the value of the 2nd operand is greater than the value of the 3rd operand, and false otherwise.
    If the values cannot be compared, then an error is raised.</p>

    <h5>isGreaterThanOrEqualTo</h5>
    <p>The <code>isGreaterThanOrEqualTo</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd and 3rd operands must be registers or literals.
    The operation stores true in the 1st operand if the value of the 2nd operand is greater than or equal to the value of the 3rd operand, and false otherwise.
    If the values cannot be compared, then an error is raised.</p>

    <h4>Data Movement Instructions</h4>
    <p>Data movement instructions transfer a value into a <em>target</em> operand, which is always given as the first operand. The operand denoted by <code>name</code> is a declared name, not a register.</p>

    <h5>set</h5>
    <p>The <code>set</code> instruction expects two operands.
    The 1st operand must be a register or a variable declared in the enclosing procedure, constructor, or method, then 2nd operand must be a register, such a variable, or a literal.
    The operation stores the value of the 2nd operand in the 1st operand.</p>

    <h5>loadArgument</h5>
    <p>The <code>loadArgument</code> instruction expects two operands.
    The 1st operand must be a register, then 2nd operand must be a name denoting a parameter of the current procedure, constructor, or method.
    The operation loads the value bound to that parameter into the 1st operand.
    If the 2nd operand does not denote a parameter of the current procedure, constructor, or method, then an error is raised.</p>

    <h5>storeArgument</h5>
    <p>The <code>storeArgument</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd operand must be a name denoting a parameter of the current procedure, constructor, or method, and the 3rd operand must be a register or a literal.
    The operation stores the value of the 3rd operand into the argument slot for that parameter of the entity identified by the 1st operand.
    If the 2nd operand does not denote a parameter of the current procedure, constructor, or method, then an error is raised.</p>

    <h5>loadField</h5>
    <p>The <code>loadField</code> instruction expects three operands.
    The 1st operand must be a register, then 2nd operand must be a register holding an object, and the 3rd operand must be a name denoting a field of the class type of that object.
    The operation reads that field into the 1st operand.
    If the 3rd operand does not denote a field of that class type, then an error is raised.</p>

    <h5>storeField</h5>
    <p>The <code>storeField</code> instruction expects three operands.
    The 1st operand must be a register holding an object, then 2nd operand must be a name denoting a field of the class type of that object, and the 3rd operand must be a register or a literal.
    The operation stores the value of the 3rd operand into that field.
    If the 2nd operand does not denote a field of that class type, then an error is raised.</p>

    <h4>Object Instructions</h4>

    <h5>invoke</h5>
    <p>The <code>invoke</code> instruction invokes a method of an object.
    It expects the invoked object in its 1st operand in a register and the method to be invoked as its 2nd operand as name.
    The arguments, which may either be registers or literals, follow.</p>

    <h5>construct</h5>
    <p>The <code>construct</code> instruction invokes a constructor for the named class and stores the constructed object in the target operand.
    The 2nd argument is the name of the class of which the constructor shall be invoked.
    The third argument is a register containing the object upon which the constructor is invoked.
    The object is stored in the register specified by the first argument on success.
    The arguments, which may either be registers or literals, follow.</p>

    <h4>Control Flow Instructions</h4>

    <h5>jump</h5>
    <p>The <code>jump</code> instruction expects one operand, which must be a name denoting a label declared in the body of the current procedure, constructor, or method.
    The operation transfers control to that label.
    If the operand does not denote a label declared in the current procedure, constructor, or method, then an error is raised.</p>

    <h5>jumpIfTrue</h5>
    <p>The <code>jumpIfTrue</code> instruction expects two operands.
    The 1st operand must be a register or a literal, then 2nd operand must be a name denoting a label declared in the body of the current procedure, constructor, or method.
    The operation transfers control to that label if the value of the 1st operand is true, and falls through otherwise.
    If the 2nd operand does not denote a label declared in the current procedure, constructor, or method, then an error is raised.</p>

    <h5>jumpIfFalse</h5>
    <p>The <code>jumpIfFalse</code> instruction expects two operands.
    The 1st operand must be a register or a literal, then 2nd operand must be a name denoting a label declared in the body of the current procedure, constructor, or method.
    The operation transfers control to that label if the value of the 1st operand is false, and falls through otherwise.
    If the 2nd operand does not denote a label declared in the current procedure, constructor, or method, then an error is raised.</p>

    <h5>raise</h5>
    <p>The <code>raise</code> instruction expects no operands.
    The operation raises the current exception.</p>

    <h5>return</h5>
    <p>The <code>return</code> instruction returns from the current method / procedure / constructor.
    It may have zero or one operand. It returns a void value with zero operands.
    If an operand is specified, that operand must be a register or a literal of which the value is returned.</p>

    <p>The target of a jump must be a bare name; a string literal is not accepted. Labels are introduced by a label definition statement, that is by <code>name ':'</code>, in the body of the enclosing procedure, constructor, or method.</p>
