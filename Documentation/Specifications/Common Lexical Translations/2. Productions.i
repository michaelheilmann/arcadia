<h2 id="productions">2 Productions</h2>
<p>This section lists reusable lexical productions and their translation rules. A consuming
language selects and combines these productions with its own additions to define its lexical
grammar, word alternatives, and start symbol.</p>

<h3 id="productions-whitespace">2.1 Whitespace</h3>
<p>The word <code>Lexical.Whitespace</code> is defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Whitespace productions"><code>
/* #9 is also known as "CHARACTER TABULATION" */
Lexical.Whitespace : #9
/* #20 is also known as "SPACE" */
Lexical.Whitespace : #20
/* Legacy misspelled label for the space alternative, retained for compatibility. */
Lexial.Whitespace : #20
</code></pre>
</div>
</div>

<h3 id="productions-end-of-line">2.2 End of line</h3>
<p>The word <code>Lexical.EndOfLine</code> is defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="End-of-line productions"><code>
/* #a is also known as "LINEFEED (LF)" */
/* #d is also known as "CARRIAGE RETURN (CR)" */
Lexical.EndOfLine : #a {#d}
Lexical.EndOfLine : #d {#a}
</code></pre>
</div>
</div>

<h3 id="productions-comment">2.3 Comment</h3>
<p>
A consuming language may select both single-line and multi-line comments.
A <code>Lexical.Comment</code> is either a <code>Lexical.SingleLineComment</code> or a <code>Lexical.MultiLineComment</code>.
<code>Lexical.Comment</code> is defined by
</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Comment productions"><code>
Lexical.Comment : Lexical.SingleLineComment
Lexical.Comment : Lexical.MultiLineComment
</code></pre>
</div>
</div>

<p>
A <code>Lexical.SingleLineComment</code> begins with two solidus characters (<code>//</code>)
and extends to the end of the line.
<code>Lexical.SingleLineComment</code> is defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Single-line comment production"><code>
/* #2f is also known as SOLIDUS */
Lexical.SingleLineComment :
#2f #2f
/* any sequence of characters except for line_terminator */
</code></pre>
</div>
</div>

<p>The <code>Lexical.LineTerminator</code> is not considered as part of the comment text.</p>

<p>
A <code>Lexical.MultiLineComment</code> begins with a solidus and an asterisk (<code>/*</code>)
and ends with an asterisk and a solidus (<code>*/</code>).
<code>Lexical.MultiLineComment</code> is defined by
</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Multi-line comment production"><code>
/* #2f is also known as SOLIDUS */
/* #2a is also known as ASTERISK */
Lexical.MultiLineComment :
#2f #2a
/* any sequence of characters except except for #2a #2f */
#2a #2f
</code></pre>
</div>
</div>

<p>The opening <code>#2f #2a</code> and closing <code>#2a #2f</code> sequences are not part of the comment text.</p>

<p>Within comment text:</p>

<ul>
  <li><code>#2f #2f</code> has no special meaning in either kind of comment.</li>
  <li><code>#2f #2a</code> and <code>#2a #2f</code> have no special meaning in single-line comments.</li>
  <li>Multi-line comments do not nest.</li>
</ul>

<h3 id="productions-parentheses">2.4 Parentheses</h3>
<p>The words <code>Lexical.LeftParenthesis</code> and <code>Lexical.RightParenthesis</code>, respectively, are defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Parenthesis productions"><code>
/* #28 is also known as "LEFT PARENTHESIS" */
Lexical.LeftParenthesis : #28
/* #29 is also known as "RIGHT PARENTHESIS" */
Lexical.RightParenthesis : #29
</code></pre>
</div>
</div>

<h3 id="productions-curly-brackets">2.5 Curly brackets</h3>
<p>The words <code>Lexical.LeftCurlyBracket</code> and <code>Lexical.RightCurlyBracket</code>, respectively, are defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Curly-bracket productions"><code>
/* #7b is also known as "LEFT CURLY BRACKET" */
Lexical.LeftCurlyBracket : #7b
/* #7d is also known as "RIGHT CURLY BRACKET" */
Lexical.RightCurlyBracket : #7d
</code></pre>
</div>
</div>

<h3 id="productions-colon">2.6 Colon</h3>
<p>The word <code>Lexical.Colon</code> is defined by</p>

<div class="box">
<pre tabindex="0" aria-label="Colon production"><code>
/* #3a is also known as "COLON" */
Lexical.Colon : #3a
</code></pre>
</div>

<h3 id="productions-square-brackets">2.7 Square brackets</h3>
<p>The words <code>Lexical.LeftSquareBracket</code> and <code>Lexical.RightSquareBracket</code>, respectively, are defined by</p>

<div class="box">
<pre tabindex="0" aria-label="Square-bracket productions"><code>
/* #5b is also known as "LEFT SQUARE BRACKET" */
Lexical.LeftSquareBracket : #5b
/* #5d is also known as "RIGHT SQUARE BRACKET" */
Lexical.RightSquareBracket : #5d
</code></pre>
</div>

<h3 id="productions-comma">2.8 Comma</h3>
<p>The word <code>Lexical.Comma</code> is defined by</p>

<div class="box">
<pre tabindex="0" aria-label="Comma production"><code>
/* #2c is also known as "COMMA" */
Lexical.Comma : #2c
</code></pre>
</div>

<h3 id="productions-name">2.9 Name</h3>
<p>The word <code>Lexical.Name</code> is defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Name productions"><code>
Lexical.Name : {Lexical.Underscore} Lexical.Alphabetic {Lexical.NameSuffixCharacter}
Lexical.Underscore : #5f
Lexical.Alphabetic : /* one ASCII letter from #41 to #5a or #61 to #7a */

/* #41 is also known as "LATIN CAPITAL LETTER A" */
/* #5a is also known as "LATIN CAPITAL LETTER Z" */
/* #61 is also known as "LATIN SMALL LETTER A" */
/* #7a is also known as "LATIN SMALL LETTER Z" */
Lexical.NameSuffixCharacter : /* The unicode characters from #41 to #5a and from #61 to #7a. */

/* #30 is also known as "DIGIT ZERO" */
/* #39 is also known as "DIGIT NINE" */
Lexical.NameSuffixCharacter : /* The unicode characters from #30 to #39. */

/* #5f is also known as "LOW LINE" */
Lexical.NameSuffixCharacter : #5f
</code></pre>
</div>
</div>

<p>The shared name production requires an ASCII letter after any leading underscores.
DDL's <a href="@{siteAddress}/specifications/data-definition-language/index.html#lexical-translation">name profile</a>
also permits underscore-only names. That language-specific refinement does not replace the shared production above.</p>

<h3 id="productions-number-literal">2.10 Number literal</h3>
<p>The word <code>Lexical.Number</code> is defined by</p>

<div class="box">
<pre tabindex="0" aria-label="Number productions"><code>
Lexical.Number : Lexical.IntegerNumber
Lexical.Number : Lexical.RealNumber
Lexical.Digit : Lexical.DecimalDigit
Lexical.IntegerNumber : [Lexical.Sign] Lexical.Digit {Lexical.Digit}
Lexical.RealNumber : [Lexical.Sign] Lexical.Period Lexical.Digit {Lexical.Digit} [Lexical.Exponent]
Lexical.RealNumber : [Lexical.Sign] Lexical.Digit {Lexical.Digit} [Lexical.Period {Lexical.Digit}] [Lexical.Exponent]
Lexical.Exponent : Lexical.ExponentPrefix [Lexical.Sign] Lexical.Digit {Lexical.Digit}

/* #2b is also known as "PLUS SIGN" */
Lexical.Sign : #2b
/* #2d is also known as "MINUS SIGN" */
Lexical.Sign : #2d
/* #65 is also known as "LATIN SMALL LETTER E" */
Lexical.ExponentPrefix : #65
/* #45 is also known as "LATIN CAPITAL LETTER E" */
Lexical.ExponentPrefix : #45
</code></pre>
</div>

<h3 id="productions-string-literal">2.11 String literal</h3>
<p>The word <code>Lexical.String</code> is defined by</p>
<p>These reusable productions offer both quote styles. A consuming language may restrict the delimiters or explicitly refine the escape set;
DDL selects double-quoted strings only, while other languages specify their own profiles.</p>

<div class="box">
<pre tabindex="0" aria-label="String productions"><code>Lexical.String : Lexical.SingleQuotedString
Lexical.String : Lexical.DoubleQuotedString

Lexical.DoubleQuotedString : Lexical.DoubleQuote {Lexical.DoubleQuotedStringCharacter} Lexical.DoubleQuote
Lexical.DoubleQuotedStringCharacter : /* any Unicode scalar value except #22, #5c and [#00,#1f] */
Lexical.DoubleQuotedStringCharacter : Lexical.EscapeSequence
Lexical.DoubleQuotedStringCharacter : #5c Lexical.DoubleQuote
/* #22 is QUOTATION MARK. */
Lexical.DoubleQuote : #22

Lexical.SingleQuotedString : Lexical.SingleQuote {Lexical.SingleQuotedStringCharacter} Lexical.SingleQuote
Lexical.SingleQuotedStringCharacter : /* any Unicode scalar value except #27, #5c and [#00,#1f] */
Lexical.SingleQuotedStringCharacter : Lexical.EscapeSequence
Lexical.SingleQuotedStringCharacter : #5c Lexical.SingleQuote
/* #27 is APOSTROPHE. */
Lexical.SingleQuote : #27

/* #5c is REVERSE SOLIDUS; #75 is LATIN SMALL LETTER U. */
Lexical.EscapeSequence : #5c 'u' #7b Lexical.HexadecimalDigit [Lexical.HexadecimalDigit] [Lexical.HexadecimalDigit] [Lexical.HexadecimalDigit] [Lexical.HexadecimalDigit] [Lexical.HexadecimalDigit] #7d
Lexical.EscapeSequence : #5c #5c /* reverse solidus */
Lexical.EscapeSequence : #5c #62 /* b */
Lexical.EscapeSequence : #5c #66 /* f */
Lexical.EscapeSequence : #5c #6e /* n */
Lexical.EscapeSequence : #5c #72 /* r */
Lexical.EscapeSequence : #5c #74 /* t */
Lexical.EscapeSequence : #5c #76 /* v */
Lexical.EscapeSequence : #5c #30 /* 0 */</code></pre>
</div>

<p>The reverse solidus must introduce a listed escape; it is not an ordinary unescaped string character.
Raw U+0000 through U+001F, including line endings, are forbidden inside strings. The opposite quote character may appear literally:
for example, <code>"It's ready"</code> needs no escape for its apostrophe.</p>
<p>Lexical translation removes the delimiters and decodes the contents as follows:</p>

<ul>
  <li><code>Lexical.DoubleQuote {Lexical.DoubleQuotedStringCharacter} Lexical.DoubleQuote</code> has the leading double quote and the trailing double quote removed.</li>
  <li><code>Lexical.SingleQuote {Lexical.SingleQuotedStringCharacter} Lexical.SingleQuote</code> has the leading single quote and the trailing single quote removed.</li>
  <li><code>\"</code> (<code>#5c #22</code>) in a double-quoted string denotes U+0022 (quotation mark).</li>
  <li><code>\'</code> (<code>#5c #27</code>) in a single-quoted string denotes U+0027 (apostrophe). This production does not enable single-quoted DDL strings.</li>
  <li><code>\\</code> (<code>#5c #5c</code>) denotes U+005C (reverse solidus).</li>
  <li><code>\b</code> (<code>#5c #62</code>) denotes U+0008 (backspace), not U+0007 (bell).</li>
  <li><code>\f</code> (<code>#5c #66</code>) denotes U+000C (form feed).</li>
  <li><code>\n</code> (<code>#5c #6e</code>) denotes U+000A (line feed).</li>
  <li><code>\r</code> (<code>#5c #72</code>) denotes U+000D (carriage return).</li>
  <li><code>\t</code> (<code>#5c #74</code>) denotes U+0009 (character tabulation).</li>
  <li><code>\v</code> (<code>#5c #76</code>) denotes U+000B (line tabulation / vertical tab).</li>
  <li><code>\0</code> (<code>#5c #30</code>) denotes U+0000 (NUL). It is part of the decoded text, not a string terminator.</li>
  <li><code>\u{...}</code> contains one to six hexadecimal digits between <code>{</code> (#7b) and <code>}</code> (#7d),
  and is replaced by the corresponding Unicode scalar value. Let \(d_1,\ldots,d_n\) be the digit values in source order,
  with \(1 \leq n \leq 6\). The code point is
  \[
  c := \sum_{i=1}^{n} d_i \cdot 16^{n-i}.
  \]
  Lexical translation shall fail if braces are missing, the digit count is outside this range, a digit is not hexadecimal,
  \(c &gt; \texttt{10FFFF}_{16}\), or \(\texttt{D800}_{16} \leq c \leq \texttt{DFFF}_{16}\).
  Leading zeros count toward the six-digit limit. Spaces, signs and underscores are not allowed inside the braces.
  The unbraced <code>\uXXXX</code> form is not supported. For example, <code>\u{41}</code> denotes A and <code>\u{1F3EE}</code> denotes U+1F3EE.
  </li>
</ul>
<p>For example, the DDL string <code>"A\tB\n"</code> decodes to A, U+0009, B, U+000A;
<code>"\0"</code> has one decoded scalar value, U+0000. Unknown escapes such as <code>\q</code> or <code>\d</code> fail lexical translation.</p>



<h3 id="productions-boolean-literal">2.12 Boolean literal</h3>
<p>The word <code>Lexical.Boolean</code> is defined by</p>

<div class="box">
<pre tabindex="0" aria-label="Boolean productions"><code>
Lexical.Boolean : Lexical.True
Lexical.Boolean : Lexical.False
Lexical.True : true
Lexical.False : false
true : #74 #72 #75 #65
false : #66 #61 #6c #73 #65
</code></pre>
</div>

<p>When a consuming language selects these alternatives, <code>true</code> and <code>false</code>
are keywords: <code>Lexical.Boolean</code> takes priority over <code>Lexical.Name</code>
for those spellings.</p>

<h3 id="productions-void-literal">2.13 Void literal</h3>
<p>The word <code>Lexical.Void</code> is defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Void production"><code>
Lexical.Void : #76 #6f #69 #64
</code></pre>
</div>
</div>

<p>When a consuming language selects this alternative, <code>void</code> is a keyword:
<code>Lexical.Void</code> takes priority over <code>Lexical.Name</code> for that spelling.</p>

<h3 id="productions-decimal-digit">2.14 Decimal digit</h3>
<p>The word <code>Lexical.DecimalDigit</code> is defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Decimal-digit production"><code>
Lexical.DecimalDigit : /* A single Unicode character from the code point range +U0030 to +U0039. */
</code></pre>
</div>
</div>

<h3 id="productions-hexadecimal-digit">2.15 Hexadecimal digit</h3>
<p>The word <code>Lexical.HexadecimalDigit</code> is defined by</p>

<div class="box">
<div class="body">
<pre tabindex="0" aria-label="Hexadecimal-digit production"><code>
Lexical.HexadecimalDigit : /* A single Unicode character from U+0030 to U+0039, U+0061 to U+0066, or U+0041 to U+0046. */
</code></pre>
</div>
</div>

<h3 id="productions-alphanumeric">2.16 Alphanumeric</h3>
<p>The word <code>Lexical.Alphanumeric</code> is reserved for future use.</p>

<h3 id="productions-period">2.17 Period</h3>
<p>The word <code>Lexical.Period</code> is defined by</p>

<div class="box">
<pre tabindex="0" aria-label="Period production"><code>
/* #2e is also known as "FULL STOP" */
Lexical.Period : #2e
</code></pre>
</div>

<h3 id="productions-end-of-input">2.18 End of input</h3>
<p>The word <code>Lexical.EndOfInput</code> is a zero-width condition
that is satisfied at the position immediately following the last Unicode code point of the input.
It does not consume or represent a Unicode code point.</p>

<h3 id="productions-start-of-input">2.19 Start of input</h3>
<p>The word <code>Lexical.StartOfInput</code> is a zero-width condition
that is satisfied at the position immediately preceding the first Unicode code point of the input.
It does not consume or represent a Unicode code point.</p>

<h3 id="productions-line-continuation">2.20 Line continuation</h3>
<p>The word <code>Lexical.LineContinuation</code> is defined by</p>
<div class="box">
<pre tabindex="0" aria-label="Line-continuation production"><code>
/* #5c is also known as "REVERSE SOLIDUS" */
Lexical.LineContinuation : #5c Lexical.EndOfLine
</code></pre>
</div>
