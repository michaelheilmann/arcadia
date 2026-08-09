<h2 id="syntactical-translation">4 Syntactical Translation</h2>
<p>The syntactical translation translates a sequence of words provided by the lexical translation into a sentence.</p>

<p>The syntactical grammar consists of</p>

<ul>
  <li>a set of non-terminals \(\textit{DDL.Syntactical.NonTerminals}\) and the set of terminals \(\textit{DDL.Syntactical.Terminals}\) which are disjoint</li>
  <li>a set of production rules \(\textit{DDL.Syntactical.ProductionRules}\), which are layed down in this section, and</li>
  <li>a starting symbol \(\textit{DDL.Syntactical.Sentence}\)> which is element of \(\textit{DDL.Syntactical.NonTerminals}\).</li>
</ul>

<p><b>Important:</b>The following words are removed from the sequence of words before its translation
into a sentence:</p>
<ul>
  <li>\(\text{DDL.Lexical.Whitespace}\),</li>
  <li>\(\text{DDL.Lexical.Newline}\), and</li>
  <li>\(\text{DDL.Lexical.Comment}\)</li>
</ul>

<div class="box">
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Sentence} : \text{DDL.Syntactical.Value}
\end{aligned}\]</my-formula-box>
</div>

<h3>4.1 DDL.Syntactical.Value</h3>
<p>The sentence \(\text{DDL.Syntactical.Value}\) is defined by</p>

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

<h3>4.2 DDL.Syntactical.String</h3>
<p>The sentence \(\text{DDL.Syntactical.String}\) is defined by</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.String} : \text{Lexical.String}
\end{aligned}\]</my-formula-box>

<h3>4.2 DDL.Syntactical.Number</h3>
<p>The sentence \(\text{DDL.Syntactical.Number}\) is</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Number} : \text{Lexical.Number}
\end{aligned}\]</my-formula-box>

<h3>4.3 DDL.Syntactical.Boolean</h3>
<p>The sentence \(\text{DDL.Syntactical.Boolean}\) is</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Boolean} : \text{Lexical.Boolean}
\end{aligned}\]</my-formula-box>

<h3>4.4 DDL.Syntactical.Void</h3>
<p>The sentence \(\text{DDL.Syntactical.Void}\) is</p>
<my-formula-box class="align-left">\[\begin{aligned}
&\text{DDL.Syntactical.Void} : \text{Lexical.Void}
\end{aligned}\]</my-formula-box>

<h3>4.5 DDL.Syntactical.Map</h3>
<p>The sentence \(\text{DDL.Syntactical.Map}\) is</p>

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

<h3>4.6 DDL.Syntactical.List</h3>
<p>The sentence \(\text{DDL.Syntactical.List}\) is</p>

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

<p>The syntatical translation translates a sequence of words into one sentence.
   This resulting sntence is then consumed by the semantical translation.</p>
