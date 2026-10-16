    <h2>Syntactical Translation</h2>
    <h3>Compilation Units</h3>
    <p>A compilation unit contains zero or more top-level definitions.</p>
    <p><code>
    compilationUnit : definition* endOfInput<br>
    definition : moduleDefinition | classDefinition | enumerationDefinition | interfaceDefinition | procedureDefinition
    </code></p>

    <h3>Modules</h3>
    <p>A module definition names a module. The current parser accepts an optional empty body.</p>
    <p><code>
    moduleDefinition : 'module' identifier moduleBody?<br>
    moduleBody : '{' '}'
    </code></p>

    <h3>Classes</h3>
    <p>A class definition names a class, may name an extended class, and may contain fields, constructors, and methods.</p>
    <p><code>
    classDefinition : 'class' identifier ('extends' identifier)? ('implements' interfaceNames)? classBody?<br>
    classBody : '{' classMemberDefinition* '}'<br>
    classMemberDefinition : fieldDefinition | constructorDefinition | methodDefinition<br>
    fieldDefinition : 'variable' name ':' identifier
    </code></p>
    <p>A class may implement one or more interface types. Every implemented interface type must be an interface type; the semantic analysis rejects any other symbol.</p>
    <p><em>Not yet reflected by the C backend:</em> the <code>implements</code> clause is parsed and semantically checked, but the current backend writer does not yet emit an implemented interface type set or the corresponding interface dispatch initializers. Classes are still written with <code>Arcadia_defineObjectType</code>.</p>

    <h3>Interfaces</h3>
    <p>An interface definition names an interface, may name the interface types it extends, and must contain operation definitions. The body of an interface definition is mandatory.</p>
    <p><code>
    interfaceDefinition : 'interface' identifier ('extends' interfaceNames)? interfaceBody<br>
    interfaceBody : '{' interfaceMemberDefinition* '}'<br>
    interfaceMemberDefinition : operationDefinition<br>
    operationDefinition : 'method' nativeClause? name parameters? ':' identifier methodBody?<br>
    interfaceNames : identifier (',' identifier)*
    </code></p>
    <p>An interface type may extend one or more interface types. Every extended interface type must be an interface type; the semantic analysis rejects any other symbol. The inheritance graph of interface types is acyclic, and an interface type may extend the same interface type at most once, transitively. The semantic analysis rejects a cyclic interface inheritance graph.</p>
    <p>Each interface type has its own operation table. An interface type does not inherit the operations of the interface types it extends.</p>

    <h3>Enumerations</h3>
    <p>An enumeration definition names an enumeration and may contain named constants. A constant may have an integer initializer.</p>
    <p><code>
    enumerationDefinition : 'enumeration' identifier enumerationBody?<br>
    enumerationBody : '{' enumerationConstantDefinition* '}'<br>
    enumerationConstantDefinition : name ('=' integerLiteral)?
    </code></p>

    <h3>Procedures, Constructors, and Methods</h3>
    <p>Procedures and methods declare a return type. Constructors do not. The declaration forms are:</p>
    <p><code>
    procedureDefinition : 'procedure' 'entry'? nativeClause? identifier parameters? ':' identifier procedureBody?<br>
    constructorDefinition : 'constructor' nativeClause? parameters? constructorBody?<br>
    methodDefinition : 'method' nativeClause? name parameters? ':' identifier methodBody?<br>
    nativeClause : 'native' stringLiteral<br>
    parameters : '(' (parameter (',' parameter)*)? ')'<br>
    parameter : 'variable' name ':' identifier<br>
    procedureBody : block<br>
    constructorBody : block<br>
    methodBody : block<br>
    block : '{' statement* '}'
    </code></p>

    <h3>Statements</h3>
    <p>A statement is terminated by a line terminator, a closing curly bracket, or the end of input. Empty lines are ignored.</p>
    <p><code>
    statement : variableDefinitionStatement | labelDefinitionStatement | instructionStatement<br>
    variableDefinitionStatement : 'variable' name<br>
    labelDefinitionStatement : name ':'<br>
    instructionStatement : instruction
    </code></p>

    <h3>Operands</h3>
    <p>Arcadia PDL instruction operands are not general expressions. They are literals, variables, or registers.</p>
    <p><code>
    operand : booleanLiteral | integerLiteral | realLiteral | stringLiteral | voidLiteral | name | register
    </code></p>

    <h3>Instructions</h3>
    <p>The parser recognizes the following instruction forms. Conditional jumps preserve their condition operand in the AST; unconditional jumps have no condition operand.</p>
    <p><code>
    instruction : (binaryInstruction<br>
                | unaryInstruction<br>
                | invokeInstruction<br>
                | constructInstruction<br>
                | jumpInstruction<br>
                | argumentInstruction<br>
                | fieldInstruction<br>
                | 'raise'<br>
                | returnInstruction) instructionTerminator<br>
    instructionTerminator : (Lexical.EndOfInput|Lexical.EndOfLine)<br>
    binaryInstruction : binaryOperator operand operand operand<br>
    binaryOperator : 'add'
                   | 'subtract'
                   | 'multiply'
                   | 'divide'
                   | 'and'
                   | 'or'
                   | 'concatenate'
                   | 'isEqualTo'
                   | 'isNotEqualTo'
                   | 'isLowerThan'
                   | 'isLowerThanOrEqualTo'
                   | 'isGreaterThan'
                   | 'isGreaterThanOrEqualTo'<br>
    unaryInstruction : unaryOperator operand operand<br>
    unaryOperator : 'negate'
                  | 'not'
                  | 'set'<br>
    invokeInstruction : 'invoke' operand name operand operand*<br>
    constructInstruction : 'construct' operand name operand operand*<br>
    jumpInstruction : 'jump' name<br>
                    | 'jumpIfTrue' operand name<br>
                    | 'jumpIfFalse' operand name<br>
    argumentInstruction : 'loadArgument' operand operand<br>
                        | 'storeArgument' operand operand operand<br>
    fieldInstruction : 'loadField' operand operand operand<br>
                     | 'storeField' operand operand operand<br>
    returnInstruction : 'return' operand?<br>
    </code></p>

    <p>An instruction is terminated by a line terminator of the line of its opcode.
    The line continuation symbol may be used to continue an instruction in the next line.</p>

