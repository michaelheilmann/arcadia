<h3 id="ddls-schema-reference-type">3.7 Schema Reference type</h3>
<p>A value of type \(\ddls{\textit{SchemaReference}}\) are represented by a \(\textit{Map}\) value.
   The map contains two entries.</p>

<p>The translation function is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      &\left[ \left(\texttt{kind},\texttt{SchemaReference}\right), \left(\texttt{name}, w@@2 \right) \right]@@1\\
      &\wedge\;\texttt{kind} \in \ddl{\textit{Name}}\\
      &\wedge\;\texttt{SchemaReference} \in \ddl{\textit{String}}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = \left(\text{SchemaReferenceTag}, 2.\text{value}\right)
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
