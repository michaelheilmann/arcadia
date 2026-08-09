<h3 id="boolean-type">3.2 Boolean types</h3>
<p>The values of type \(\ddls{\textit{Boolean}}\) are represented by a \(\textit{Map}\) values.
   The map contains a single map entry of name \(\texttt{kind}\) with the value \(\texttt{Boolean}\) of type
   \(\textit{String}\).</p>

<p>The translation function is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \left[\left( \texttt{kind}, \texttt{Boolean} \right) \right]@@1
      \;\wedge\;\texttt{kind} \in \ddl{\textit{Name}}
      \;\wedge\;\texttt{Any} \in \ddl{\textit{String}}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = \ddls{\textit{boolean}}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
<p>where \(\ddls{\textit{boolean}}\) is the single value of type \(\ddls{\textit{Boolean}}\).</p>
