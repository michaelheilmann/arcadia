<h3 id="schema-type">3.8 Schema type</h3>
<p>A value of type \(\ddls{\textit{Schema}}\) are represented by a \(\textit{Map}\) value.
   The map contains three entries.</p>

<p>The translation function is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      &\left[ \left(\texttt{kind},\texttt{Schema}\right), \left(\texttt{name}, a@@2 \right), \left(\texttt{definition}, b@@3 \right)  \right]@@1\\
      &\wedge\;\texttt{kind} \in \ddl{\textit{Name}}\\
      &\wedge\;\texttt{Schema} \in \ddl{\textit{String}}\\
      &\wedge\;a \in \ddl{\textit{String}}\\
      &\wedge\;b \in \ddl{\textit{Map}}\\
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} =
      \begin{cases}
      \ValidationError & \text{if } 2.\text{value} \in \ddls{\textit{Error}} \vee 3.\text{value} \in\ddls{\textit{Error}} \vee 3.\text{value} \not\in\ddls{\textit{Value}}\text{,}\\
      \left(\text{SchemaTag}, 2.\text{value}, 3.\text{value}\right) & \text{otherwise.}
      \end{cases}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
