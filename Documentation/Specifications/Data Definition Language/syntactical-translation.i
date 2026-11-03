<h2 id="syntactical-translation">6 Syntactical translation</h2>
<p>The syntactical translation translates a sequence of words provided by the lexical translation into a sentence.</p>

<p>The syntactical grammar consists of</p>

<ul>
  <li>a set of non-terminals \(\textit{DDL.Syntactical.NonTerminals}\) and the set of terminals \(\textit{DDL.Syntactical.Terminals}\) which are disjoint</li>
  <li>a set of production rules \(\textit{DDL.Syntactical.ProductionRules}\), specified in this section, and</li>
  <li>a starting symbol \(\textit{DDL.Syntactical.Sentence}\) which is an element of \(\textit{DDL.Syntactical.NonTerminals}\).</li>
</ul>

<p><b>Important:</b> The following words are removed from the sequence of words before its translation
into a sentence:</p>
<ul>
  <li>\(\text{Lexical.Whitespace}\) (also called \(\text{DDL.Lexical.Whitespace}\)),</li>
  <li>\(\text{Lexical.Newline}\) (\(\text{DDL.Lexical.Newline}\)), and</li>
  <li>\(\text{Lexical.Comment}\) (\(\text{DDL.Lexical.Comment}\)).</li>
</ul>

<div class="box">
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Sentence} : \text{DDL.Syntactical.Value}
\end{aligned}\]</my-formula-box>
</div>

<p>The sentence consumes the entire filtered input: exactly one value, followed by end-of-input.
A comma after the top-level value is not allowed. Whitespace and comments may follow that value before end-of-input.
In the rules below, \(\epsilon\) is the empty sequence.</p>

<h3 id="syntax-value">6.1 DDL.Syntactical.Value</h3>
<p>The non-terminal \(\text{DDL.Syntactical.Value}\) is defined by</p>

<div class="box">
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Value} : \text{DDL.Syntactical.Map}\\
&\text{DDL.Syntactical.Value} : \text{DDL.Syntactical.List}\\
&\text{DDL.Syntactical.Value} : \text{DDL.Syntactical.String}\\
&\text{DDL.Syntactical.Value} : \text{DDL.Syntactical.Number}\\
&\text{DDL.Syntactical.Value} : \text{DDL.Syntactical.Boolean}\\
&\text{DDL.Syntactical.Value} : \text{DDL.Syntactical.Void}
\end{aligned}\]</my-formula-box>
</div>

<h3 id="syntax-string">6.2 DDL.Syntactical.String</h3>
<p>The non-terminal \(\text{DDL.Syntactical.String}\) is defined by</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.String} : \text{Lexical.String}
\end{aligned}\]</my-formula-box>

<h3 id="syntax-number">6.3 DDL.Syntactical.Number</h3>
<p>The non-terminal \(\text{DDL.Syntactical.Number}\) is</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Number} : \text{Lexical.Number}
\end{aligned}\]</my-formula-box>

<h3 id="syntax-boolean">6.4 DDL.Syntactical.Boolean</h3>
<p>The non-terminal \(\text{DDL.Syntactical.Boolean}\) is</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Boolean} : \text{Lexical.Boolean}
\end{aligned}\]</my-formula-box>

<h3 id="syntax-void">6.5 DDL.Syntactical.Void</h3>
<p>The non-terminal \(\text{DDL.Syntactical.Void}\) is</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Void} : \text{Lexical.Void}
\end{aligned}\]</my-formula-box>

<h3 id="syntax-map">6.6 DDL.Syntactical.Map</h3>
<p>A map may be empty. Each nonempty entry has a name, a colon and a value; commas separate entries,
and one trailing comma is allowed. Quoted strings and reserved keywords are not map names.</p>
<p>The non-terminal \(\text{DDL.Syntactical.Map}\) is</p>

<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Map} :\text{Lexical.LeftCurlyBracket}\;\text{DDL.Syntactical.MapBody}\;\text{Lexical.RightCurlyBracket}\\
\\
&\text{DDL.Syntactical.MapBody} : \text{DDL.Syntactical.MapBodyElement}\;\text{DDL.Syntactical.MapBodyRest}\\
&\text{DDL.Syntactical.MapBody} : \epsilon\\
\\
&\text{DDL.Syntactical.MapBodyRest} : \text{Lexical.Comma}\;\text{DDL.Syntactical.MapBodyElement}\;\text{DDL.Syntactical.MapBodyRest}\\
&\text{DDL.Syntactical.MapBodyRest} : \text{Lexical.Comma}\\
&\text{DDL.Syntactical.MapBodyRest} : \epsilon\\
&\text{DDL.Syntactical.MapBodyElement} : \text{Lexical.Name}\;\text{Lexical.Colon}\;\text{DDL.Syntactical.Value}
\end{aligned}\]</my-formula-box>

<h3 id="syntax-list">6.7 DDL.Syntactical.List</h3>
<p>A list may be empty. Otherwise commas separate values, with one optional trailing comma.
A comma alone does not form an empty collection.</p>
<p>The non-terminal \(\text{DDL.Syntactical.List}\) is</p>

<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.List} : \text{Lexical.LeftSquareBracket}\; \text{DDL.Syntactical.ListBody}\; \text{Lexical.RightSquareBracket}\\
\\
&\text{DDL.Syntactical.ListBody} : \text{DDL.Syntactical.ListBodyElement}\; \text{DDL.Syntactical.ListBodyRest}\\
&\text{DDL.Syntactical.ListBody} : \epsilon\\
\\
&\text{DDL.Syntactical.ListBodyRest} : \text{Lexical.Comma}\; \text{DDL.Syntactical.ListBodyElement}\; \text{DDL.Syntactical.ListBodyRest}\\
&\text{DDL.Syntactical.ListBodyRest} : \text{Lexical.Comma}\\
&\text{DDL.Syntactical.ListBodyRest} : \epsilon\\
\\
&\text{DDL.Syntactical.ListBodyElement} : \text{DDL.Syntactical.Value}
\end{aligned}\]</my-formula-box>

<p>The resulting sentence is consumed by the semantical translation.</p>
