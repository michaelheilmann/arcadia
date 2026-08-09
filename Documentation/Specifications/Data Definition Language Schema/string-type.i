<h3 id="string-type">3.9 String type</h3>
<p>The values of type \(\ddls{\textit{String}}\) are represented by a \(\textit{Map}\) values.
   The map contains a single map entry of name \(\texttt{kind}\) with the value \(\texttt{String}\) of type
   \(\textit{String}\).</p>

<p>The translation function is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \left[\left( \texttt{kind}, \texttt{String} \right) \right]@@1
      \;\wedge\;\texttt{kind} \in \ddl{\textit{Name}}
      \;\wedge\;\texttt{String} \in \ddl{\textit{String}}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = \textit{string}_{\textit{DDLS}}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
<p>where \(\ddls{\textit{string}}\) is the single value of type \(\ddls{\textit{String}}\).</p>
