<h3 id="choice-type">3.3 Choice type</h3>
<p>The values of type \(\ddls{\textit{Choice}}\) are represented by \(\textit{Map}\) values.
   The map contains two entries.</p>

   The map contains a single map entry of name \(\texttt{kind}\) with the value \(\texttt{Choice}\) of type
   \(\textit{String}\).</p>

<p>The translation function is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      &\left[ \left( \texttt{kind},\texttt{Choice} \right), \left(\texttt{choices}, w@@2\right)\right]@@1\\
      &\;\wedge\;\texttt{kind} \in \ddl{\textit{Name}} \;\wedge\;\texttt{Choice} \in \ddl{\textit{String}}\\
      &\;\wedge\;\texttt{choices} \in \ddl{\textit{Name}} \;\wedge w \in \ddl{\textit{List}}\\
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} = \left(\textit{ChoiceTag},\text{toChoices}(2)\right)
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
<p>where \(\ddls{\textit{any}}\) is the single value of type \(\ddls{\textit{Any}}\).</p>

<h4>Auxiliary definition: toChoices</h4>
<p>\(\text{toMapEntries} : \ddl{\textit{List}} \rightarrow \mathbb{L} \cup \ddls{\textit{Error}}\) is defined as</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{ if }w = [x@@1] \circ v@@2 \wedge w \in \textit{List}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      \begin{cases}
        \left[1.\text{value}\right] \circ \text{toChoices}\left(2\right)
          &\text{if } 1.\text{value} \not\in \ddls{\textit{Error}} \wedge \text{toChoices}\left(2\right) \not\in \ddls{\textit{Error}}\text{,}\\
        \ddls{\textit{Error.InvalidDefinition}}
          &\text{otherwise.}
      \end{cases}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{if }w = \left[\right] \wedge w \in \textit{List}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{Otherwise}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      \textit{Error.InvalidDefinition}_{\textit{DDLS}}
    \end{array}\]</my-formula-box>
    </td>
  </tr>

</table>
