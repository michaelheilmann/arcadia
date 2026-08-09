<h3 id="list-type">3.4 List type</h3>
<p>Values of type \(\ddls{\textit{List}}\) are represented by a \(\textit{Map}\) values.
   The map contains two entries.</p>

<p>The translation function is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      &\left[ \left(\texttt{kind},\texttt{List}\right), \left(\texttt{elements}, w@@2 \right) \right]@@1\\
      &\wedge\;\texttt{kind},\texttt{elements} \in \ddl{\textit{Name}}\\
      &\wedge\;\texttt{List} \in \ddl{\textit{String}}\\
      &\wedge\;w \in \ddl{\textit{Map}}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} =
      \begin{cases}
      \left(\textit{ListTag}, 2.\text{value}\right) & \text{if }2.\text{value} \in \ddls{\textit{Value}}\text{,}\\
      \InvalidDefinitionError                       & \text{otherwise.}
      \end{cases}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
