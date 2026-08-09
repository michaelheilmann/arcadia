<h2 id="semantical-translation">5 Semantical Translation</h2>
<p>The semantical translation a sentence provided by the syntactical translation into a typed value. The <em>Data Definition Language</em> knows six basic types
   \(\textit{List}\) and \(\textit{Map}\), which are the so called aggregate types, and \(\textit{Boolean}\), \(\textit{Number}\), \(\textit{String}\),      and
   \(\textit{Void}\), which are the so called scalar types.</p>

<p>The type \(\textit{Value}\) is defined as the union of all the types above.
\[\begin{aligned}
\textit{Value} =&\;\textit{List}\\
            \cup&\;\textit{Map}\\
            \cup&\;\textit{Boolean}\\
            \cup&\;\textit{Number}\\
            \cup&\;\textit{String}\\
            \cup&\;\textit{Void}
\end{aligned}\]
</p>

<p>
The translation of a sentence into values is described by syntax-directed translations
(see Aho, Seti, Ullman: Compilers, Principles, Techniques, and Tools; 1st; pp. 305 for more information).
</p>

<p>At the end of a translation, the input syntactic form \(x\) has a variable \(x.\text{value}\) which is either
a value of type \textit{Value}. Furthermore, for each syntactic form, we define two attributes: \(\text{value}\)
is the computed value of the syntactic form. \(\textit{codePoints}\) is the sequence of code points associated
with the syntactic form.</p>

<h3>4.1 Scalar Types</h3>
<p>This section defines \(\sigma\) for the translation of scalar types.

<h4>4.1.1 Boolean Type</h4>
<p>The type \(\textit{Boolean}\) type has two values \(\textit{true}\) and \(\textit{false}\) which are expressed in the language by the
   words \(\texttt{DDL.Lexical.true}\) and \(\texttt{DDL.Lexical.false}\), respectively (as defined in the syntactical grammar).</p>

<p>The semantical translation is defined as follows:</p>
<table class="syntax-directed-translation">
  <tr>
    <td>\(\text{DDL.Syntactical.Boolean}@@1 : \text{Lexical.Boolean}@@2\)</td>
    <td>\(\{ 1.\text{value} := 2.\text{value} \}\)</td>
  </tr>
  <tr>
    <td>\(\text{Lexical.Boolean}@@1 : \text{Lexical.True}\)</td>
    <td>\(\{ 1.\text{value} := \textit{true} \}\)</td>
  </tr>
  <tr>
    <td>\(\text{Lexical.Boolean}@@1 : \text{Lexical.False}\)</td>
    <td>\(\{ 1.\text{value} := \textit{false} \}\)</td>
  </tr>
</table>

<h4>4.1.2 Number Type</h4>
<p>
The type \(\textit{Number}\) represent sequences of Unicode code points that adhere to the constraints defined by \(\textit{DDL.Syntactical.Number}\).
These Unicode code point sequences represent arbitrary precision integer and arbitrary precision floating point numbers.</p>

<p>The semantical translation is defined as follows:</p>

<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
    \text{DDL.Syntactical.Number}@@1 : \text{Lexical.Number}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
    \{ 1.\text{value} := 2.\textit{codePoints} \}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>

<p><b>Important:</b> Implementations may want to use typical data types like \(\texttt{int}\) or \(\texttt{float}\). The <em>Data Definition Language</em> does not        impose
restrictions on the value ranges represented by these number literals. Consequently, the numeric value represented by value of type \(\textit{Number}\) might not be reprentable
by typical data types like \(\texttt{int}\) or \(\texttt{float}\).</p>

<h4>4.1.3 String Type</h4>
<p>
The \(\textit{String}\) type represents sequences of Unicode code points that have the properties defined by \(\text{DDL.Syntactical.String}\).</p>

<p>The semantical translation is defined as follows:</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
    \text{DDL.Syntactical.String}@@1 : \text{Lexical.String}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
    \{ 1.\text{value} := 2.\textit{codePoints} \}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>

<h4>4.1.4 Void Type</h4>
<p>
The type <my-sem>sem:Void</my-sem> type has a single values \(\textit{void\} which is expressed in the language by the word \(\textit{void\} (as defined in the syntactical grammar).
</p>

<p>The semantical translation is defined as follows:</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
    \text{DDL.Syntactical.Void}@@1 : \text{Lexical.Void}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
    \{ 1.\text{value} = \textit{void} \}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>

<h3>4.2 List Type</h3>
<p>
The type \(\textit{List}\) represents list of elements. Each element is a DDL node of type \(\textit{Value}\).
A DDL node of type \(\textit{List}\) is expressed in the language by the sentence \(\text{DDL.Syntactical.List}\) (as defined in the syntactical grammar).
</p>

<p>The semantical translation is defined as follows:</p>
<table class="syntax-directed-translation">

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.List}@@1 : &\text{Lexical.LeftSquareBracket}\\
                                       &\text{DDL.Syntactical.ListBody}@@2\\
                                       &\text{Lexical.RightSquareBracket}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.ListBody}@@1 : &\text{DDL.Syntactical.ListBodyElement}@@2\\
                                           &\text{DDL.Syntactical.ListBodyRest}@@3
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := \left[ 2.\text{value} \right] \circ 3.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.ListBody}@@1 : \epsilon
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.ListBodyElement}@@1 : \text{DDL.Syntactical.Value}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.ListBodyRest}@@1 :& \text{Lexical.Comma}\\
                                              & \text{DDL.Syntactical.ListBodyElement}@@2\\
                                              & \text{DDL.Syntactical.ListBodyRest}@@3
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = \left[ 2.\text{value} \right] \circ 3.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.ListBodyRest}@@1 : \text{Lexical.Comma}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.ListBodyRest}@@1 : \epsilon
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

</table>

<p><b>Example</b></p>
<div class="box">
<p><code>
// A list with three numbers 1, 2, and 3.<br>
[ 1, 2, 3 ]
</code></p>
</div>

<h3>4.3 Map Type</h3>
<p>
The type \(\textit{Map}\) represents a list of entries. Each entry is a pair. The first element of a pair is a value of type \(\textit{Name}\), the second element is a value of type \(\textit{Value}\).
A value of type \(\textit{Map}\) is expressed in the language by the sentence \(\text{DDL.Syntactical.Map}\) (as defined in the syntactical grammar).
</p>

<p>The semantical translation is defined as follows:</p>
<table class="syntax-directed-translation">

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Map}@@1 : &\text{Lexical.LeftCurlyBracket}\\
                                      &\text{DDL.Syntactical.MapBody}@@2\\
                                      &\text{Lexical.RightCurlyBracket}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.MapBody}@@1 : &\text{DDL.Syntactical.MapBodyElement}@@2\\
                                          &\text{DDL.Syntactical.MapBodyRest}@@3
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := \left[ 2.\text{value} \right] \circ 3.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.MapBody}@@1 : \epsilon
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.MapBodyRest}@@1 : &\text{Lexical.Comma}\\
                                              &\text{DDL.Syntactical.MapBodyElement}@@2\\
                                              &\text{DDL.Syntactical.MapBodyRest}@@3
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = \left[ 2.\text{value} \right] \circ 3.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.MapBodyRest}@@1 : \text{Lexical.Comma}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.MapBodyRest}@@1 : \epsilon
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.MapBodyElement}@@1 = \text{Lexical.Name}@@2;\text{Lexical.Colon};\text{DDL.Syntactical.Value}@@3
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := \left(2.\text{codePoints}, 3.\text{value}\right)
    \end{array}\]</my-formula-box>
    </td>
  </tr>

</table>

<h3>4.4 Value Type</h3>
<p>
The type \(\textit{Value}\) type is the union type of the types
\(\textit{Boolean}\),
\(\textit{List}\),
\(\textit{Map}\),
\(\textit{Number}\),
\(\textit{String}\), and
\(\textit{Void}\).
</p>

<p>The semantical translation is defined as follows:</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Boolean}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.List}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Map}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Number}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.String}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Void}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
