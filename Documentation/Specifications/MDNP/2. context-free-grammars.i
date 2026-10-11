<h2 id="context-free-grammars">2 Context-Free Grammars</h2>
<p>A context-free grammar \(G\) is a 4-tuple \(G = (V,\Sigma,P,S)\), where</p>
<ol>
  <li>\(V\) is a finite set; each element \(v \in V\) is called a nonterminal element.</li>
  <li>\(\Sigma\) is a finite set of terminals, disjoint vom \(V\).</li>
  <li>\(P\) is a finite relation in \(V \times (V \cup T)^*\), where the asterisk
       represents the Kleene star operation. The members of \(R\) are called
       productions of the grammar.</li>
  <li>\(S\) is the start element. \(S\) must be an element of \(V\).</li>
</ol>

<p>A production rule in \(P\) is an ordered pair \((\alpha,\beta)\in P\), where \(a \in V\)
is a nonterminal and \(\beta \in (V \cup \Sigma)^*\) is a string of nonterminals     and/or
terminals; rather than using an ordered pair notation, production rules are usually written
using an arrow operator with \(\alpha\) as its left hand side and \(\beta\) as its right hand
side \(\alpha \rightarrow \beta\).</p>

<p>It is allow for \(\beta\) to be the empty string and in this case it is denoted by
it by \(\epsilon\). The form \(\alpha \rightarrow \epsilon\) is called             an
\(\epsilon\)-production.</p>

<h3>2.1 Direct derivation</h3>
<p>The operation of a context-free grammar is defined in terms of the relation of two strings.
For any two strings \(u,v \in \left(V \cup \Sigma\right)^*\), we say \(v\) directly    derives
from \(u\), written as \(u \Rightarrow v\) if there exists a production \((\alpha   \rightarrow
\beta) \in P\) with \(\alpha \in V\) and \(u_1, u_2 \in \left(V \cup \Sigma\right)^*\) such that
\(u = u_1 \alpha u_2\) and \(v = u_1 \beta u_2\). Thus, \(v\) is a result of applying the rule
\(\alpha \rightarrow \beta\) to \(u\).</p>

<h3>2.2 Indirect derivation</h2>
<p>For any two string \(u,v\in (V \cup \Sigma)^*\), we say \(v\) indirectly drives from \(u\)   if
there is a positive integer \(k\) and string \(u_1, \ldots, u_k \in \left(V \cup \Sigma\right)^*\)
such that \(u = u_1 \Rightarrow u_2 \Rightarrow \ldots \Rightarrow u_k = v\), that is,         the
$\Rightarrow$ operator is applied \(k-1\) times.</p>

<p>To express that the operator is applied zero or more times to derive \(v\) from \(u\) one
   writes \(u \stackrel{*}{\Rightarrow} v\).</p>
<p>To express that the operator is applied zero or one times  to derive \(v\) from \(u\) one
   writes \(u \stackrel{?}{\Rightarrow} v\).</p>
<p>To express that the operator is applied one or more times  to derive \(v\) from \(u\) one
   writes \(u \stackrel{+}{\Rightarrow} v\).<p>

<p>The language \(L(G)\) of a grammar \(G = \left(V,\Sigma,P,S\right)\) is the set
\[
L(G) = \left\{ w \in \Sigma^* : S \stackrel{*}{\Rightarrow} w \right\}
\]
</p>

<h3>2.3 Notational convenience</h3>
<p>One may use a colon instead of the rightarrows such that \(\alpha \rightarrow \beta\) becomes \(\alpha\) <code>:</code> \(\beta\).</p>

<p>Terminals which are Unicode code points may be written starting with a shebang <code>#</code> followed by a hexadecimal number denoting the code point.<p>

<div class="box">
<div class="header">
Example
</div>
<div class="body">
<p>The following productions define non-terminals for the Unicode code symbols "PLUS SIGN" and "MINUS SIGN":</p>
<my-formula-box class="align-left">
\[\begin{aligned}
&\text{/* \#2b is also known as "PLUS SIGN" */}\\
&\text{PlusSign} : \text{\#2b}\\
&\text{/* \#2d is also known as "MINUS SIGN" */}\\
&\text{MinusSign} : \text{\#2b}\\
\end{aligned}\]
</my-formula-box>
</div>
</div>

<p>The syntax <code>{x}</code> on the right-hand side of a production denotes zero or more occurrences of <code>x</code>.</p>

<div class="box">
<div class="header">
Example
</div>
<div class="body">
<p>The following production defines a possibly empty sequence of digits:</p>
<my-formula-box class="align-left">
\[\begin{aligned}
&\text{ZeroOrMoreDigits}:\left\{\text{Digit}\right\}
\end{aligned}\]
</my-formula-box>
</div>
</div>

<p>The syntax <code>[x]</code> on the right-hand side of a production denotes zero or one occurrences of <code>x</code>.</p>

<div class="box">
<div class="header">Example</div>
<div class="body">
<p>The following productions denotes a possible definition of an integer numeral.
It consists of an optional sign followed by a digit followed by zero or more digits:</p>
<my-formula-box class="align-left">
\[\begin{aligned}
&\text{Integer}:\left[\text{Sign}\right]\;\text{Digit}\;\text{ZeroOrMoreDigits}
\end{aligned}\]
</my-formula-box>
</div>
</div>
