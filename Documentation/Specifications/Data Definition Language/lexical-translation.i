<h2 id="lexical-translation">Lexical translation</h2>
<p>Lexical translation turns the input sequence of Unicode code points into words (tokens).</p>
<p>The lexical translation of the <em>Data Definition Language</em> is based on
<a href="@{siteAddress}/specifications/common-lexical-translations/index.html">Common Lexical Translations</a>.</p>
<p>DDL selects only the double-quoted alternative of the shared string category.
Within this specification, \(\text{Lexical.String}\) is restricted by</p>
<my-formula-box class="align-left">\[\text{Lexical.String} : \text{Lexical.DoubleQuotedString}\]</my-formula-box>
<p>Single quotes do not delimit DDL strings or character literals. An apostrophe inside a double-quoted string is ordinary text,
as in <code>"It's ready"</code>; a double quote inside it must be escaped as <code>\"</code>.
The broader shared <code>Lexical.String</code> category remains available to other languages.</p>
<p>DDL also defines its own name profile. Within this specification, <code>Lexical.Name</code> matches
<code>[A-Za-z_][A-Za-z0-9_]*</code>: the first character is an ASCII letter or underscore; subsequent characters
are ASCII letters, digits or underscores. Names are nonempty and case-sensitive. Examples include <code>name</code>,
<code>_</code>, <code>__</code>, <code>_1</code> and <code>layer_2</code>.</p>
<p>The exact lowercase words <code>true</code>, <code>false</code> and <code>void</code> are reserved keywords, not names.
Keyword prefixes such as <code>true_</code> are names. Map keys must be names, not quoted strings or digit-leading forms such as <code>1name</code>.
This DDL profile replaces the shared Name production's requirement for a letter after leading underscores; it does not redefine other languages' name rules.</p>
<p>The DDL label <code>Lexical.Newline</code> denotes a CR, LF or CRLF line ending. CRLF is recognized as one newline word.
It plays the end-of-line role described by the shared lexical categories; the label is retained for existing DDL grammar references.</p>

<p>The lexical grammar consists of</p>
<ul>
  <li>a set of non-terminals \(\textit{DDL.Lexical.NonTerminals}\) and the set of terminals \(\textit{DDL.Lexical.Terminals}\) which are disjoint</li>
  <li>a set of production rules \(\textit{DDL.Lexical.ProductionRules}\), specified in this section, and</li>
  <li>a starting symbol \(\text{DDL.Lexical.Words}\) which is element of \(\textit{DDL.Lexical.NonTerminals}\).</li>
</ul>

<p>The two rules, defined in terms of the <em>Common Lexical translations</em>, are defined as follows:</p>

<div class="box">
<my-formula-box class="align-left">\[\begin{array}{ll}
\text{DDL.Lexical.Words} &: \text{DDL.Lexical.Word}^*\\
\text{DDL.Lexical.Word} &:\;\text{Lexical.Boolean}\\
                        &|\;\text{Lexical.Number}\\
                        &|\;\text{Lexical.String}\\
                        &|\;\text{Lexical.Void}\\
                        &|\;\text{Lexical.Name}\\
                        &|\;\text{Lexical.LeftCurlyBracket}\\
                        &|\;\text{Lexical.RightCurlyBracket}\\
                        &|\;\text{Lexical.LeftSquareBracket}\\
                        &|\;\text{Lexical.RightSquareBracket}\\
                        &|\;\text{Lexical.Comma}\\
                        &|\;\text{Lexical.Colon}\\
                        &|\;\text{Lexical.Whitespace}\\
                        &|\;\text{Lexical.Newline}\\
                        &|\;\text{Lexical.Comment}\\
\end{array}\]</my-formula-box>
</div>

<p>The lexical translation translates a sequence of Unicode code points into words.
    This resulting sequence of words is then consumed by the syntactical translation.</p>
<p><code>Lexical.Colon</code> denotes <code>:</code>, as defined in
<a href="@{siteAddress}/specifications/common-lexical-translations/index.html#productions-colon">Common Lexical Translations</a>.
The current reader recognizes spaces and tabs, CR, LF and CRLF, <code>//</code> line comments and
<code>/* ... */</code> block comments. Comments are not nested. Names and keywords are case-sensitive.
See <a href="#conformance">conformance</a> for DDL-specific selections and reader edge cases.</p>
