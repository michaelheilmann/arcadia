<h3 id="map-type">3.5 Map type</h3>
<p>A value of type \(\ddls{\textit{Map}}\) are represented by a \(\ddl{\textit{Map}}\) value.
   The map contains two entries.</p>

<p>The translation function is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \left[ \left(\texttt{kind},\texttt{Map}\right), \left(\texttt{entries}, w@@2 \right) \right]@@1
      \;\wedge\;\texttt{kind},\texttt{entries} \in \ddl{\textit{Name}}
      \;\wedge\;\texttt{Map} \in \ddl{\textit{String}}
      \;\wedge\;w \in \ddl{\textit{List}}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} =
      \left(
      \textit{MapTag},
      \text{toMapEntries}(2)
      \right)
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>

<h4>Auxiliary definition: toMapEntries</h4>
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
        \left[1.\text{value}\right] \circ \text{toMapEntries}\left(2\right)
          &\text{if } 1.\text{value} \in \ddls{\textit{MapEntry}} \wedge \text{toMapEntries}\left(2\right) \not\in \ddls{\textit{Error}}\text{,}\\
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

<p>A value of type \(\ddls{\textit{MapEntry}}\) are represented by a \(\textit{Map}\) value.
   The map contains three entries.</p>

<p>The translation function for a \(\ddls{\textit{MapEntry}}\) is hence given by</p>
<table class="syntax-directed-translation">
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
     &\left[\left(\texttt{kind},\texttt{MapEntry}\right),
           \left(\texttt{name}, a@@2\right),
           \left(\texttt{type}, b@@3\right)\right]@@1\\
     &\wedge\;\texttt{kind},\texttt{name},\texttt{type} \in \ddl{\textit{Name}}\\
     &\wedge\;\texttt{MapEntry} \in \ddl{\textit{String}}\\
     &\wedge\;a \in \ddl{\textit{Name}}\\
     &\wedge\;b \in \ddl{\textit{Map}}
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
    1.\text{value} =
      \begin{cases}
      \left(\textit{MapEntryTag}, 2.\text{value}, 3.\text{value}\right) &\text{if }2.\text{value},3.\text{value}\not\in\ddls{\textit{Error}}\text{,}\\
      \ddls{\textit{Error}} &\text{otherwise.}
      \end{cases}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
