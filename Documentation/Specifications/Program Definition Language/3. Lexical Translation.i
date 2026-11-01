    <h2>Lexical Translation</h2>
    <p>The input is read as Unicode code points from UTF-8 bytes. Arcadia PDL selects reusable productions and translation rules from <a href="@{siteAddress}/specifications/common-lexical-translations/index.html">Common Lexical Translations</a> unless this section defines a more specific Arcadia PDL rule.</p>
    <p>Arcadia PDL defines its own lexical word alternatives and start symbol using the selected common categories and the PDL-specific categories below. The common specification does not supply a universal word production. PDL's reserved-word and other disambiguation rules belong to this consuming language.</p>
    <p>Arcadia PDL uses the common definitions for whitespace, line terminators, line continuations, single-line comments, multi-line comments, periods, parentheses, curly brackets, colons, commas, boolean literals, and the void literal. Whitespace and comments separate words and are otherwise ignored by the parser. Line terminators are significant as statement terminators unless a line continuation joins the statement with the following line.</p>
    <p><code>
    whiteSpace : Lexical.Whitespace<br>
    lineTerminator : Lexical.LineTerminator<br>
    lineContinuation : Lexical.LineContinuation<br>
    singleLineComment : Lexical.SingleLineComment<br>
    multiLineComment : Lexical.MultiLineComment<br>
    period : Lexical.Period<br>
    leftParenthesis : Lexical.LeftParenthesis<br>
    rightParenthesis : Lexical.RightParenthesis<br>
    leftCurlyBracket : Lexical.LeftCurlyBracket<br>
    rightCurlyBracket : Lexical.RightCurlyBracket<br>
    colon : Lexical.Colon<br>
    comma : Lexical.Comma<br>
    booleanLiteral : Lexical.Boolean<br>
    voidLiteral : Lexical.Void
    </code></p>

    <h3>Line Continuations</h3>
    <p>A line continuation is a reverse solidus immediately followed by a line terminator. The scanner produces one line continuation word that spans the reverse solidus and the line terminator, and the parser ignores that word. A reverse solidus that is not followed by a line terminator is a lexical error.</p>
    <p>Statements may therefore be written over several lines by ending every line but the last with a line continuation. Only the line terminator of the last line terminates the statement:</p>
    <p><code>
    invoke $0 print \<br>
    &nbsp;&nbsp;&nbsp;c \<br>
    &nbsp;&nbsp;&nbsp;"done"
    </code></p>

    <h3>Names and Identifiers</h3>
    <p>Arcadia PDL names are similar to <code>Lexical.Name</code>, but the current scanner also accepts an underscore as the first character without requiring a following alphabetic character. A qualified identifier is one or more names separated by periods.</p>
    <p><code>
    name : ('A'..'Z' | 'a'..'z' | '_') ('A'..'Z' | 'a'..'z' | '0'..'9' | '_')*<br>
    identifier : name (period name)*
    </code></p>

    <h3>Registers</h3>
    <p>A register starts with <code>$</code> and is followed by either a decimal register number or a name.</p>
    <p><code>
    register : '$' ('0' | ('1'..'9' '0'..'9'*) | name)
    </code></p>

    <h3>Literals</h3>
    <p>The current scanner recognizes the common boolean and void literals and Arcadia PDL-specific integer, real, and string literal forms. Arcadia PDL integer and real literals are narrower than <code>Lexical.Number</code>: they do not have signs, and a real literal must start with one or more decimal digits followed by a period. Arcadia PDL string literals are narrower than <code>Lexical.String</code>: they use double quotes only and currently recognize only the escapes listed below.</p>
    <p><code>
    integerLiteral : digit+<br>
    realLiteral : digit+ '.' digit* exponent?<br>
    exponent : ('e' | 'E') ('+' | '-')? digit+<br>
    stringLiteral : '"' stringCharacter* '"'<br>
    stringCharacter : anyCodePointExceptBackslashQuoteOrEndOfInput | '\\"' | '\\\\' | '\\n' | '\\r'
    </code></p>

    <h3>Reserved Words</h3>
    <p>The following words are reserved and cannot be used as names unless the scanner is changed:</p>
    <p><code>abstract add and call class concatenate construct constructor divide entry enumeration extends false implements interface invoke isEqualTo isGreaterThan isGreaterThanOrEqualTo isLowerThan isLowerThanOrEqualTo isNotEqualTo jump jumpIfFalse jumpIfTrue loadArgument loadField method module multiply native negate not or procedure raise return set storeArgument storeField subtract true variable void</code></p>
