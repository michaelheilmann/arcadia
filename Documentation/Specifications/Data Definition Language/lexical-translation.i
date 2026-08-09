<h2 id="lexical-translation">3 Lexical Translation</h2>
<p>The lexical translation translates a sequence of Unicode code points provided as input a sequence of words.</p>
<p>The lexical translation of the <em>Data Definition Language</em> is based on the <em>Common Lexical Translations</em>
(see <a href="@{siteAddress}/specifications/common-lexical-translations">@{siteAddress}/specifications/common-lexical-translations</a>
for more information).</p>

<p>The lexical grammar consists of</p>
<ul>
  <li>a set of non-terminals \(\textit{DDL.Lexical.NonTerminals}\) and the set of terminals \(\textit{DDL.Lexical.Terminals}\) which are disjoint</li>
  <li>a set of production rules \(\textit{DDL.Lexical.ProductionRules}\), which are layed down in this section, and</li>
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
                        &|\;\text{Lexical.Whitespace}\\
                        &|\;\text{Lexical.Newline}\\
                        &|\;\text{Lexical.Comment}\\
\end{array}\]</my-formula-box>
</div>

<p>The lexical translation translates a sequence of Unicode code points into words.
   This resulting sequence of words is then consumed by the syntactical translation.</p>
