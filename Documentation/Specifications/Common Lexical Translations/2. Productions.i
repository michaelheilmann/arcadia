<h2 id="productions">2 Productions</h2>
<p>This section lists reusable lexical productions and their translation rules. A consuming
language selects and combines these productions with its own additions to define its lexical
grammar, word alternatives, and start symbol.</p>

<h3 id="productions-whitespace">2.1 whitespace</h3>
<p>The word <code>whitespace</code> is defined by</p>

<div class="box">
<div class="body">
<p><code>
/* #9 is also known as "CHARACTER TABULATION" */<br>
Lexical.Whitespace : #9<br>
/* #20 is also known as "SPACE" */<br>
Lexial.Whitespace : #20
</code></p>
</div>
</div>

<h3 id="productions-end-of-line">2.2 end of line</h3>
<p>The word <code>Lexical.EndOfLine</code> is defined by</p>

<div class="box">
<div class="body">
<p><code>
/* #a is also known as "LINEFEED (LF)" */<br>
/* #d is also known as "CARRIAGE RETURN (CR)" */<br>
Lexical.EndOfLine : #a {#d}<br>
Lexical.EndOfLine : #d {#a}
</code></p>
</div>
</div>

<h3 id="productions-comment">2.3 comment</h3>
<p>
The language using the Common Lexical Specification may use both single-line comments and multi-line comments.
A <code>Lexical.Comment</code> is either a <code>single_line_comment</code> or a <code>Lexical.MultiLineComment</code>.
<code>Lexical.MultiLineComment</code> is defined by
</p>

<div class="box">
<div class="body">
<p><code>
Lexical.Comment : Lexical.SingleLineComment
Lexical.Comment : Lexical.MultiLineComment
</code></p>
</div>
</div>

<p>
A <code>Lexical.SingleLineComment</code> starts with two solidi.
It extends to the end of the line.
<code>Lexical.SingleLinecomment</code> is defined by</p>

<div class="box">
<div class="body">
<p><code>
/* #2f is also known as SOLIDUS */
Lexical.SingleLineComment :
#2f #2f
/* any sequence of characters except for line_terminator */
</code></p>
</div>
</div>

<p>The <code>Lexical.LineTerminator</code> is not considered as part of the comment text.</p>

<p>
A <code>Lexical.MultiLineComment</code> is opened by a solidus and an asterisk and closed by an asterisk and a solidus.
<code>Lexical.MultiLineComment</code> is defined by
</p>

<div class="box">
<div class="body">
<p><code>
/* #2f is also known as SOLIDUS */<br>
/* #2a is also known as ASTERISK */<br>
Lexical.MultiLineComment :<br>
#2f #2a<br>
/* any sequence of characters except except for #2a #2f */<br>
#2a #2f
</code></p>
</div>
</div>

<p>The <code>#2f #2a</code> and <code>#2a #2f</code> sequences are not considered as part of the comment text.</p>

<p>This implies:</p>

<ul>
  <li><code>#2f #2f</code> has no special meaning either comment.</li>
  <li><code>#2f #2a</code> and <code>#2a #2f</code> have no special meaning in single-line comments.</li>
  <li>Multi-line comments do not nest.</li>
</ul>

<h3 id="productions-parentheses">2.4 parentheses</h3>
<p>The words <code>Lexical.LeftParenthesis</code> and <code>Lexical.RightParenthesis</code>, respectively, are defined by</p>

<div class="box">
<div class="body">
<p><code>
/* #28 is also known as "LEFT PARENTHESIS" */<br>
Lexical.LeftParenthesis : #28<br>
/* #29 is also known as "RIGHT PARENTHESIS" */<br>
Lexical.RightParenthesis : #29
</code></p>
</div>
</div>

<h3 id="productions-curly-brackets">2.5 curly brackets</h3>
<p>The words <code>Lexical.LeftCurlyBracket</code> and <code>Lexical.RightCurlyBracket</code>, respectively, are defined by</p>

<div class="box">
<div class="body">
<p><code>
/* #7b is also known as "LEFT CURLY BRACKET" */<br>
Lexical.LeftCurlyBracket : #7b<br>
/* #7d is also known as "RIGHT CURLY BRACKET" */<br>
Lexical.RightCurlyBracket : #7d
</code></p>
</div>
</div>

<h3 id="productions-colon">2.6 colon</h3>
<p>The word <code>Lexical.Colon</code> is defined by</p>

<div class="box">
<p><code>
/* #3a is also known as "COLON" */<br>
Lexical.Colon : #3a
</code></p>
</div>

<h3 id="productions-square-brackets">2.7 square brackets</h3>
<p>The words <code>Lexical.LeftSquareBracket</code> and <code>Lexica.RightSquareBracket</code>, respectively, are defined by</p>

<div class="box">
<p><code>
/* #5b is also known as "LEFT SQUARE BRACKET" */<br>
Lexical.LeftSquareBracket : #5b<br>
/* #5d is also known as "RIGHT SQUARE BRACKET" */<br>
Lexical.RightSquareBracket : #5d
</code></p>
</div>

<h3 id="productions-comma">2.8 comma</h3>
<p>The word <code>Lexical.Comma</code> is defined by</p>

<div class="box">
<p><code>
/* #2c is also known as "COMMA" */<br>
Lexical.Comma : #2c
</code></p>
</div>

<h3 id="productions-name">2.9 name</h3>
<p>The word <code>Lexical.Name</code> is defined by</p>

<div class="box">
<div class="body">
<p><code>
Lexical.Name : {Lexical.Underscore} Lexical.Alphabetic {Lexical.NameSuffixCharacter}<br>
<br>
/* #41 is also known as "LATIN CAPITAL LETTER A" */<br>
/* #5a is also known as "LATIN CAPITAL LETTER Z" */<br>
/* #61 is also known as "LATIN SMALL LETTER A" */<br>
/* #7a is also known as "LATIN SMALLER LETTER Z" */<br>
Lexical.NameSuffixCharacter : /* The unicode characters from #41 to #5a and from #61 to #7a. */<br>
<br>
/* #30 is also known as "DIGIT ZERO" */<br>
/* #39 is also known as "DIGIT NINE" */<br>
Lexical.NameSuffixCharacter : /* The unicode characters from #30 to #39. */<br>
<br>
/* #5f is also known as "LOW LINE" */<br>
Lexical.NameSuffixCharacter : #5f<br>
</code></p>
</div>
</div>

<h3 id="productions-number-literal">2.10 number literal</h3>
<p>The word <code>Lexical.Number</code> is defined by</p>

<div class="box">
<p><code>
Lexical.Number : Lexical.IntegerNumber<br>
Lexical.Number : Lexical.RealNumber<br>
Lexical.IntegerNumber : [Lexical.Sign] Lexical.Digit {Lexical.Digit}<br>
Lexical.RealNumber : [Lexical.Sign] Lexical.Period Lexical.Digit {Lexical.Digit} [Lexical.Exponent]<br>
Lexical.RealNumber : [Lexical.Sign] Lexical.Digit {Lexical.Digit} [Lexical.Period {Lexical.Digit}] [Lexical.Exponent]<br>
Lexical.Exponent : Lexical.ExponentPrefix [Lexical.Sign] Lexical.Digit {Lexical.Digit}<br>
<br>
/* #2b is also known as "PLUS SIGN" */<br>
Lexical.Sign : #2b<br>
/* #2d is also known as "MINUS SIGN" */<br>
Lexical.Sign : #2d<br>
/* #65 is also known as "LATIN SMALL LETTER E" */<br>
Lexical.ExponentPrefix : #65<br>
/* #45 is also known as "LATIN CAPITAL LETTER E" */<br>
Lexical.ExponentPrefix : #45<br>
</code></p>
</div>

<h3 id="productions-string-literal">2.11 string literal</h3>
<p>The word <code>Lexical.String</code> is defined by</p>

<div class="box">
<p><code>
Lexical.String : Lexical.SingleQuotedString<br>
Lexical.String : Lexical.DoubleQuotedString<br>
<br>
Lexical.DoubleQuotedString : Lexical.DoubleQuote {Lexical.DoubleQuotedStringCharacter} Lexical.DoubleQuote<br>
Lexical.DoubleQuotedStringCharacter : /* any character except for Lexical.Newline and Lexical.DoubleQuote and characters in [0,1F]*/<br>
Lexical.DoubleQuotedStringCharacter : Lexical.EscapeSequence<br>
Lexical.DoubleQuotedStringCharacter : #5c Lexical.DoubleQuote<br>
/* #22 is also known as "QUOTATION MARK" */<br>
Lexical.DoubleQuote : #22<br>
<br>
Lexical.SingleQuotedString : Lexical.SingleQuote {Lexical.SingleQuotedStringCharacter} Lexical.SingleQuote<br>
Lexical.SingleQuotedStringCharacter : /* any character except for Lexical.Newline and Lexical.SingleQuote and characters in [0,1F]*/<br>
Lexical.SingleQuotedStringCharacter : Lexical.EscapeSequence<br>
Lexical.SingleQuotedStringCharacter : #5c Lexical.SingleQuote <br>
/* #27 is also known as "APOSTROPHE" */<br>
Lexical.SingleQuote : #27<br>
<br>

/* #5c is also known as "REVERSE SOLIDUS", #75 is also known as 'LATIN SMALL LETTER U*/<br>
Lexical.EscapeSequence : #5c 'u' Lexical.HexadecimalDigit Lexical.HexadecimalDigit Lexical.HexadecimalDigit Lexical.HexadecimalDigit<br>
/* #5c is also known as "REVERSE SOLIDUS" */<br>
Lexical.EscapeSequence : #5c #5c<br>
/* #64 is also known as "LATIN SMALL LETTER B" */<br>
Lexical.EscapeSequence : #5c #64<br>
/* #66 is also known as "LATIN SMALL LETTER F" */<br>
Lexical.EscapeSequence : #5c #66<br>
/* #6e is also known as "LATIN SMALL LETTER N" */<br>
Lexical.EscapeSequence : #5c #6e<br>
/* #72 is also known as "LATIN SMALL LETTER R" */<br>
Lexical.EscapeSequence : #5c #72<br>
/* #74 is also known as "LATIN SMALL LETTER T" */<br>
Lexical.EscapeSequence : #5c #75

</code></p>
</div>

<p>In the lexical translation, several transformations are performed upon the word:</p>

  </ul>
  <li><code>Lexical.DoubleQuote {Lexical.DoubleQuotedStringCharacter} Lexical.DoubleQuote</code> has the leading double quote and the trailing double quote removed.</li>
  <li><code>Lexical.SingleQuote {Lexical.SingleQuotedStringCharacter} Lexical.SingleQuote</code> has the leading single quote and the trailing single quote removed.</li>
  <li><code>#5c #5c</code> (double "REVERSE SOLIDUS") are replaced by <code>#5c</code> ("REVERSE SOLIDUS")</li>
  <li><code>#5c #64</code> ("REVERSE SOLIDUS" followed by "LATIN SMALL LETTER B") are replaced by <code>#7</code> ("BELL")</li>
  <li><code>#5c #66</code> ("REVERSE SOLIDUS" followed by "LATIN SMALL LETTER F") are replaced by <code>#c</code> ("FORM FEED")</li>
  <li><code>#5c #6e</code> ("REVERSE SOLIDUS" followed by "LATIN SMALL LETTER N") are replaced by <code>#a</code> ("LINE FEED")</li>
  <li><code>#5c #72</code> ("REVERSE SOLIDUS" followed by "LATIN SMALL LETTER R") are replaced by <code>#d</code> ("CARRIAGE RETURN")</li>
  <li><code>#5c #72</code> ("REVERSE SOLIDUS" followed by "LATIN SMALL LETTER T") are replaced by <code>#9</code> ("TAB")</li>
  <li><code>#5c 'u' Lexical.HexadecimalDigit Lexical.HexadecimalDigit Lexical.HexadecimalDigit Lexical.HexadecimalDigit</code> is replaced
  by the corresponding Unicode code point. That Unicode code point is computed as follows: Let \(a\), \(b\), \(c\), and \(d\)  denote the
  first, second, third, and fourth hexadecimal digit from left to right following <code>'u'</code>. Then the Unicode code point is given by
  \[
  c := (a \cdot 16^3) + (b*16^2) + (c*16^1) + (d*16^0)
  \]
  If \(c\) is outside of the set of Unicode code points, then the lexical translation shall fail.
  </li>
</ul>



<h3 id="productions-boolean-literal">2.12 boolean literal</h3>
<p>The word <code>Lexical.Boolean</code> is defined by</p>

<div class="box">
<p><code>
Lexical.Boolean : Lexical.True<br>
Lexical.Boolean : Lexical.False<br>
true : #74 #72 #75 #65<br>
false : #66 #61 #6c #73 #65
</code></p>
</div>

<p>Remark: The word <code>Lexical.Boolean</code> is a so called <em>keyword</em>.
It takes priority over the <code>Lexical.Name</code>.</p>

<h3 id="productions-void-literal">2.13 void literal</h3>
<p>The word <code>Lexical.Void</code> is defined by</p>

<div class="box">
<div class="body">
<p><code>
Lexical.Void : #76 #6f # #69 #64
</code></p>
</div>
</div>

<p>Remark: The word <code>Lexical.Void</code> is a so called <em>keyword</em>.
It takes priority over the <code>Lexical.Name</code>.</p>

<h3 id="productions-decimal-digit">2.14 decimal digit</h3>
<p>The word <code>Lexical.DecimalDigit</code> is defined by</p>

<div class="box">
<div class="body">
<p><code>
Lexical.DecimalDigit : /* A single Unicode character from the code point range +U0030 to +U0039. */
</code></p>
</div>
</div>

<h3 id="productions-hexadecimal-digit">2.15 hexadecimal digit</h3>
<p>The word <code>Lexical.HexadecimalDigit</code> is defined by</p>

<div class="box">
<div class="body">
<p><code>
Lexical.HexadecimalDigit : /* A single Unicode character from the code point range +U0030 to +U0039,  +U0061 to +U007A,  U+0041 to U+005A*/
</code></p>
</div>
</div>

<h3 id="productions-alphanumeric">2.16 alphanumeric</h3>
<p>The word <code>Lexical.Alphanumeric</code> is reserved for future use.</p>

<h3 id="productions-period">2.17 period</h3>
<p>The word <code>Lexical.Period</code> is defined by</p>

<div class="box">
<p><code>
/* #2e is also known as "FULL STOP" */<br>
Lexical.Period : 2e
</code></p>
</div>

<h3 id="productions-end-of-input">2.18 end of input</h3>
<p>The word <code>Lexical.EndOfInput</code> is a zero-width condition
that is satisfied at the position immediately following the last Unicode code point of the input.
It does not consume or represent a Unicode code point.</p>

<h3 id="productions-start-of-input">2.19 start of input</h3>
<p>The word <code>Lexical.StartOfInput</code> is a zero-width condition
that is satisfied at the position immediately preceding the first Unicode code point of the input.
It does not consume or represent a Unicode code point.</p>

<h3 id="productions-line-continuation">2.20 line continuation</h3>
<p>The word <code>Lexical.LineContinuation</code> is defined by</p>
<div class="box">
<p><code>
/* #5c is also known as "REVERSE SOLIDUS" */<br>
Lexical.LineContinuation : #5c Lexical.EndOfLine
</code></p>
</div>
