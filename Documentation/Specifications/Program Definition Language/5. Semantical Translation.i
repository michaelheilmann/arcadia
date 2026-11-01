      <h2>Semantical Translation</h2>
      <p>The semantical translation resolves the names of a sentence provided by the syntactical translation and binds every declaration to a type.</p>

      <h3>Module Names and Declaration Names</h3>
      <p>Module lookup is separate from type and procedure lookup. Module names must be unique among modules in a compilation; type and procedure declarations must be unique among top-level declarations, including built-in types. A module and a type may have the same fully qualified name, for example module <code>Arcadia.Engine</code> and class <code>Arcadia.Engine</code>. A module name alone never denotes a type or a callable, and does not reserve that name against declarations. Duplicate type declarations and duplicate module definitions remain errors in their respective name spaces.</p>
      <p>A declaration belongs to its defining module independently of its name. Sharing a name with a module, or having a module name as a prefix, does not change declaration ownership or imply lexical nesting. Module membership determines generated-file placement and cross-module dependency boundaries; it does not add an implicit prefix during type lookup.</p>

      <h3>Type Names</h3>
      <p>Type names are fully qualified, and there is no relative resolution of type names. A type name is matched against the fully qualified names of the compilation as it is written, so <code>Arcadia.Natural32</code> denotes a primitive type and <code>MyModule.Greeter</code> denotes the type declared with the qualified name <code>MyModule.Greeter</code>. A type name that does not spell out the fully qualified name of a type is not defined; for example, <code>Greeter</code> does not denote the type <code>MyModule.Greeter</code>. The same rule applies wherever the grammar expects a type name: a class <code>extends</code> or <code>implements</code> clause, an interface <code>extends</code> clause, a <code>variable</code> or parameter type, the return type of a procedure or method, and the class type named by <code>construct</code>. Constructors have no return type declaration.</p>

      <h3>Member Names</h3>
      <p>A member of a class type is looked up by its declared name, unqualified, in the scope of the class type that declares it. A member name, unlike a type name, is never fully qualified.</p>

      <h3>Abstract Classes</h3>
      <p>A class declared with <code>abstract</code> cannot be instantiated directly. It may declare fields, constructors, concrete implementations and explicit abstract methods, and may be extended by another abstract class or by a concrete class. A class is concrete unless explicitly declared abstract; class abstractness is not inherited. An abstract class need not contain abstract methods. A concrete non-native subclass must implement or inherit concrete implementations of every abstract class method.</p>
      <p>A <code>construct</code> instruction naming an abstract class is a compilation error unless it is immediate-superclass initialization: the first statement of a derived constructor, naming its immediate parent and supplying <code>self</code> as the receiver. This operation initializes the existing derived object and never allocates an abstract object. Implicit zero-argument superclass initialization is also permitted for an abstract parent. Constructor argument and superclass-forwarding rules otherwise apply unchanged.</p>

      <h3>Built-in Types</h3>
      <p>The following types are built in to every compilation. They are always spelled with the fully qualified <code>Arcadia.</code> prefix:</p>
      <ul>
        <li><code>Arcadia.Boolean</code></li>
        <li><code>Arcadia.Integer8</code>, <code>Arcadia.Integer16</code>, <code>Arcadia.Integer32</code>, <code>Arcadia.Integer64</code></li>
        <li><code>Arcadia.Natural8</code>, <code>Arcadia.Natural16</code>, <code>Arcadia.Natural32</code>, <code>Arcadia.Natural64</code></li>
        <li><code>Arcadia.Object</code></li>
        <li><code>Arcadia.Real32</code>, <code>Arcadia.Real64</code></li>
        <li><code>Arcadia.Size</code></li>
        <li><code>Arcadia.String</code></li>
        <li><code>Arcadia.Value</code></li>
        <li><code>Arcadia.Void</code></li>
      </ul>
      <p><code>Arcadia.Object</code> and <code>Arcadia.String</code> are class types; the other built-in types are primitive types. <code>Arcadia.String</code> denotes the runtime string object and maps to <code>Arcadia_String*</code> in C, distinct from the runtime UTF-8 string values used by string literals.</p>
      <p>A built-in type is provided by the runtime rather than by a module, so it is not generated: the compiler emits no source or header file for a built-in type, and instead binds a reference to it to the corresponding runtime type.</p>
      <p><code>Arcadia.Size</code> is the runtime's unsigned, platform-sized size type. It is distinct from the fixed-width natural types, including on platforms where their widths happen to match. Integer literals retain their <code>Arcadia.Natural64</code> representation; declaring a size parameter does not implicitly retag such a literal.</p>

      <h3>Native Definitions</h3>
      <p>A procedure, constructor, or method definition may carry an optional <code>native</code> clause. The clause takes a string literal naming the native target. A definition that carries the clause is a <em>native definition</em>; a definition that omits the clause is not native, even though it declares the same kind of member.</p>
      <p>A native class may instead carry an optional header path after <code>native</code>. The path is nonempty and relative to the C compiler's include search roots, uses forward slashes, and contains no empty, <code>.</code>, or <code>..</code> components. Control characters, reverse solidus, quotation marks, angle brackets, and colons are rejected with an invalid-path diagnostic. The header mapping does not change the PDL/runtime type name, derived C identifier, inheritance, or native implementation ownership. A mapped header need not reside in the PDL module's source tree; it is resolved by the consumer's C compiler.</p>
      <p>A native enumeration uses the same optional header mapping, for example <code>native "NativeGenerated/Type.h" enumeration Test.NativeDeclared.Kind { First = 0 Second = 7 }</code> (one member per source line). This is a top-level type declaration: another declared type's name may be a prefix of its qualified name without implying lexical nesting. Its externally owned header is not emitted unless it shares a header explicitly owned by a native-implementation class, as described below.</p>
      <p>A plain native class remains externally declared and is not emitted. A <code>native implementation</code> class instead owns its declaration output; its implementation remains native and native construction restrictions still apply. The mapped declaration path must end in <code>.h</code> and cannot be the module aggregate. Two output declarations cannot own the same header, and a generated header cannot replace an opaque native class's definition. Native enums explicitly mapped to the same owned header are emitted there as shared top-level declarations, not as nested types or runtime enum implementations.</p>
      <p>A callable's native-target string names the target verbatim. It must be a nonempty C identifier. The rewriting that derives an implementation name from a fully qualified type name by replacing the <code>.</code> separator does not apply to a native target name.</p>
      <p>A native callable definition must not have a PDL body: its implementation is provided by the named target. An ordinary, nonabstract definition is compiled from its PDL body. An omitted ordinary body is treated as an empty body. An empty void callable returns void; falling through a non-void callable raises a runtime error. An explicitly abstract method instead declares an implementation obligation and has no implementation body.</p>
      <p>A built-in type may declare members, and such a member is native without a <code>native</code> clause appearing in any compilation unit: a member supplied by a built-in type is native by being supplied, and not by carrying the optional clause.</p>

      <h3>Qualified Declarations, Ownership, and Value Records</h3>
      <p>Fields and parameter annotations describe the program independently of a backend. <code>reference T</code> describes a reference to external storage for a scalar <code>T</code>; <code>reference Arcadia.Void</code> describes untyped external storage. <code>readonly</code> grants read-only access through an object view or external reference. <code>output reference T</code> describes caller-provided writable result storage. Output parameters cannot be readonly. These declarations do not prescribe a C pointer, a Java carrier, or any particular runtime value tag.</p>
      <p>A <code>readonly</code> object view is not an immutable object type: it restricts access through that view, without making the object globally immutable or preventing other aliases from modifying it. It is also distinct from a <code>constant</code> declaration, which defines an immutable initialized class value. The language has no separate <code>const</code> keyword or general constant-object declaration; current class constants are Integer32 values.</p>
      <p>Ordinary object fields are GC-managed references. A <code>borrowed</code> field does not acquire ownership; an <code>owned reference</code> field owns external storage; a <code>retained</code> object field follows its type's lifetime protocol. The current resource protocol provides nonvirtual zero-argument <code>ref</code> and <code>unref</code> operations returning void. Retained references are distinct from GC-traced references. Native implementations remain responsible for carrying out these declared contracts.</p>
      <p>Record fields are value aggregates: their fields have declared types and order, and repeated field names are errors. They have no object identity or explicit base member. Class constants have immutable initialized values, not per-instance storage. The current implementation accepts class constants of unqualified <code>Arcadia.Integer32</code> and diagnoses out-of-range initializers.</p>
      <p>A nonvirtual method belongs to its declaring type and does not participate in override dispatch. A method cannot override a nonvirtual method, and a nonvirtual method cannot replace an inherited member. Overrides must preserve parameter types, reference/read-only qualifiers, and output modes. Interface operations are virtual.</p>
      <table>
        <thead><tr><th>Declaration or annotation</th><th>Meaning</th></tr></thead>
        <tbody>
          <tr><td><code>readonly Arcadia.Math.Color4Real32</code></td><td>A read-only object view for a native input, not a distinct immutable color object.</td></tr>
          <tr><td><code>readonly reference Arcadia.Void</code></td><td>A read-only view of borrowed, untyped external storage, as used by byte-buffer inputs.</td></tr>
          <tr><td><code>output reference Arcadia.Integer32</code></td><td>Caller-provided writable scalar result storage, as used by frame-buffer size queries.</td></tr>
          <tr><td><code>borrowed Arcadia.Engine.Visuals.BackendContext</code></td><td>A non-owning object field.</td></tr>
          <tr><td><code>owned reference Arcadia.Void</code></td><td>An owning external-storage field, as used by vertex-buffer bytes.</td></tr>
          <tr><td><code>retained Arcadia.Engine.Visuals.MaterialResource</code></td><td>A field governed by the resource's explicit lifetime protocol.</td></tr>
          <tr><td><code>constant VertexDataDirty : Arcadia.Integer32 = 1</code></td><td>An initialized class constant with no instance storage.</td></tr>
        </tbody>
      </table>
      <p>For example, an aggregate field is written in PDL rather than as a quoted backend declaration:</p>
      <pre><code>variable canvasSize : record {
  variable width : Arcadia.Real32
  variable height : Arcadia.Real32
}</code></pre>

      <h3>Procedures, Methods, and Constructors</h3>
      <p>A procedure belongs to its declaring module and may have a qualified declared identifier. A qualified procedure target is resolved by its exact declared name in the compilation; an unqualified target retains current-module lookup. There is no implicit namespace-prefix search. A method belongs to its declaring class and uses an unqualified member name. A constructor belongs to its declaring class and is not inherited. A class may declare at most one constructor and at most one member with a given declared name; overloading by parameter types or parameter count is not supported.</p>
      <p>Parameters are ordered and passed by value. A callable accepts at most 255 parameters. At invocation, the argument count must match the parameter count and each argument must have the declared type. Primitive types require the corresponding runtime value tag; class and interface types accept instances of those types or <code>void</code> to denote an absent reference. The C backend maps that absent reference to <code>NULL</code>. Non-null class arguments must be instances of the declared class or a derived class; non-null interface arguments must implement the declared interface. No implicit scalar conversion is performed. A returned value is checked against the declared return type, with the same absent-reference rule for class and interface results. Locals and registers remain dynamically typed.</p>
      <p>Parameter and local names are scoped to the complete callable body. Duplicate parameter or local names are errors. A method or constructor also binds <code>self</code> to its receiver; a parameter or local must not redeclare that name. Registers are separate from local names and are initialized to void, as are local variables. Labels are scoped to the complete body; duplicate labels and unresolved jump destinations are errors.</p>
      <p>A virtual class method with the same declared name as an inherited virtual method overrides that method. Its parameter types, reference/read-only qualifiers, output modes, parameter count, and return type must agree with the inherited method. Invocation dispatches to the overriding implementation for the receiver's runtime class, including when invocation uses an ancestor's method entry point. Nonvirtual members cannot participate in this override relation.</p>
      <p>An abstract method must be declared by an abstract class. It cannot be nonvirtual, have a native target, or have a body (including an empty body). Abstract intermediate classes may leave inherited methods unimplemented, or re-declare an inherited concrete method abstract. Every concrete non-native class must provide or inherit a concrete implementation of every abstract class method; implementing only some inherited obligations is an error. A concrete native binding is an implementation. A native class provider is responsible for inherited obligations fulfilled outside PDL, so opaque native and native-implementation classes are not rejected merely because an inherited implementation is absent from their PDL declarations.</p>
      <p>Interface operations are implicitly abstract; explicitly writing <code>method abstract</code> is permitted. An explicitly abstract operation cannot also name a native target or have a body. This does not change the separate, currently limited class-interface implementation support.</p>
      <p>A generated constructor initializes its own fields to their zero values. Unless explicitly forwarded, the immediate superclass constructor is invoked with zero arguments before the body. If that superclass constructor requires arguments, the first body statement must be <code>construct target Fully.Qualified.Superclass self arguments...</code>. This initializes the same object and does not allocate a second one. The superclass must be the immediate parent. A class without a declared constructor has a zero-argument constructor with implicit zero-argument superclass forwarding.</p>
      <p>A field must not shadow an inherited member. A plain value field must not have type <code>Arcadia.Void</code>; <code>reference Arcadia.Void</code> is instead a permitted description of external storage. A class- or interface-valued field is initially null; reading an absent reference produces <code>void</code>, and storing <code>void</code> clears the reference. Non-null field values must match the declared type. Method invocation and field access still require a non-null object receiver; <code>void</code> cannot be used as that receiver. The receiver binding <code>self</code> is not a writable instruction target.</p>

      <h3>Interface Operations</h3>
      <p>Interface operations are method declarations. The syntactical translation accepts the same surface method form as class methods, but semantical translation rejects an interface operation that has a method body.</p>

    <h3>Instruction Semantics</h3>
    <p>Arcadia PDL is currently dynamically typed. The semantical translation does not bind an instruction operand to a type, so the descriptions below state which operand forms an instruction accepts and which effect it has on the values of its operands at run time. An instruction raises an error when it cannot be applied to the values of its operands.</p>
    <p>In the instruction descriptions below, a register value may also be supplied by a declared local variable or parameter. A writable target must be a register or a declared local variable or parameter; a literal is never writable. Integer literal operands are represented as <code>Arcadia.Natural64</code>, real literal operands as <code>Arcadia.Real64</code>, and string literal operands as runtime UTF-8 strings. This operand representation is distinct from the width selection for enumeration initializers.</p>

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
    <p>The <code>storeArgument</code> instruction expects three operands: a writable result target, a name denoting a parameter of the current callable, and a source value. It stores the source value into the current invocation's parameter slot and copies that value to the result target. It does not modify the caller's argument slot. An undefined parameter name is a semantic error.</p>

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
    <p>The <code>invoke</code> instruction writes a returned value to its first operand, a writable target. Its second operand is a callable identifier. A qualified identifier denotes a procedure by its exact declared name; an unqualified name first looks for a procedure in the current module. For a procedure, every following operand is an argument. Otherwise an unqualified name denotes a method: the first following operand is the receiver and the remaining operands are method arguments. The receiver must be an instance of a class declaring or inheriting the named method with the given argument count. Method dispatch uses its runtime class. A void method or procedure stores void in the target. At least one operand must follow the callable identifier.</p>

    <h5>call</h5>
    <p><code>call target identifier arguments...</code> invokes a procedure and stores its return value in the writable target. Qualified identifiers resolve the exact declared procedure across the compilation; unqualified names retain current-module lookup. Unlike <code>invoke</code>, it permits zero arguments and never performs method lookup. Arguments are evaluated from left to right before invocation. Undefined procedure identifiers and statically known argument-count mismatches are semantic errors.</p>

    <h5>construct</h5>
     <p><code>construct target Fully.Qualified.Class receiver arguments...</code> constructs an object and writes it to the writable target on success. A void receiver requests allocation of a new concrete object. An existing receiver is permitted only for immediate-superclass construction in a constructor and must be <code>self</code>. The remaining operands are constructor arguments, evaluated from left to right. A zero-argument allocation is written <code>construct target Fully.Qualified.Class void</code>. The C backend generates allocation entry points only for concrete, non-native classes. It rejects direct abstract-class construction even when the receiver operand is a variable. An immediate native superclass may be initialized by the first constructor statement using its declared constructor signature and native runtime callback; its native provider remains responsible for implementing that signature. Native-class allocation remains unsupported.</p>

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
    If an operand is specified, that operand must be a register, local variable, parameter, or literal whose value is returned. Constructors and void procedures or methods must not return an operand; non-void procedures and methods must return an operand. These form violations are semantic errors.</p>

    <p>The target of a jump must be a bare name; a string literal is not accepted. Labels are introduced by a label definition statement, that is by <code>name ':'</code>, in the body of the enclosing procedure, constructor, or method.</p>
