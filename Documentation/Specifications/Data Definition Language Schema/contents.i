<h2 id="introduction">1 Introduction</h2>

\(
\def\ddl#1{{#1_{\textit{DDL}}}}
\def\ddls#1{{#1_{\textit{DDLS}}}}

\def\NumberLit#1{{\texttt{#1}}}
\def\StringLit#1{{\texttt{#1}}}

\def\true{{\text{true}}}
\def\false{{\text{false}}}

\def\CyclicDefinitionError{{\ddls{\textit{CyclicDefinitionError}}}}
\def\SchemaAlreadySpecifiedError{{\ddls{\textit{SchemaAlreadySpecifiedError}}}}
\def\SchemaNotFoundError{{\ddls{\textit{SchemaNotFoundError}}}}
\def\InvalidDefinitionError{{\ddls{\textit{InvalidDefinitionError}}}}
\def\ValidationError{{\ddls{\textit{ValidationError}}}}
\)

<p>
This document is the specification of the <em>Data Definition Language Schema</em>, or <em>DDLS</em> for short.
DDLS is a declarative language for defining the structure and constraints for data described by a <a href="@{siteAddress}/specifications/data-definition-language">DDL</a>  programs.
</p>

<p>
<em>DDLS</em> programs and <em>DDL</em> programs are inputs to <em>DDLS validators</em>.
<em>DDLS validators</em> are tools which validate an input DDL program against an input DDLS Schema program.
These tools can be standalone or integrated into libraries and executables.
</p>

<h2 id="notations">Notations</h2>

<p>This specification uses the semantical form of the DDL as defined in <em>Data Definition Language</em>
   specification (see <a href="@{siteAddress}/specifications/data-definition-language/#semantical-specification">
   Data Definition Language</a> for more information).</p>

<p>Given a DDL value \(x\), we denote the type of that value by \(\textit{type}(x)\).</p>

<p>
The following DDLS types exist:

The \(\ddls{\textit{Any}}\) type,
the scalar types
\(\ddls{\textit{Boolean}}\),
\(\ddls{\textit{Number}}\),
\(\ddls{\textit{String}}\), and
\(\ddls{\textit{Void}}\),
the aggregate types
\(\ddls{\textit{List}}\), and
\(\ddls{\textit{Map}}\),
the choice type
\(\ddls{\textit{Choice}}\),
as well as the schema reference type
\(\ddls{\textit{SchemaReference}}\).
</p>

The \(\textit{Value}_{\textit{DDLS}}\) type is defined as the union of the above types
\[\begin{aligned}
\textit{Value}_{\textit{DDLS}} =&\;\ddls{\textit{Any}}\\
                            \cup&\;\ddls{\textit{Boolean}}\\
                            \cup&\;\ddls{\textit{Choice}}\\
                            \cup&\;\ddls{\textit{List}}\\
                            \cup&\;\ddls{\textit{Map}}\\
                            \cup&\;\ddls{\textit{Number}}\\

                            \cup&\;\ddls{\textit{SchemaReference}}\\
                            \cup&\;\ddls{\textit{Void}}\\
\end{aligned}\]

<p>
Furthermore, there exist a schema type \(\ddls{\textit{Schema}}\) and a map entry type \(\ddls{\textit{MapEntry}}\).
</p>


<p>Given a DDLS value \(x\), we denote the type of that value by \(\textit{type}(x)\).</p>

<h2 id="semantics">3 Semantics</h2>

<p>
The translation of a DDL value into DDLS values is described by syntax-directed translations
(see Aho, Seti, Ullman: Compilers, Principles, Techniques, and Tools; 1st; pp. 305 for more information).
At the end of a translation, the input DDL value \(x\) has a variable \(x.\text{value}\) which is either
a value of type \(\ddls{\textit{Value}}\), if the translation was successful, or an value of type
\(\ddls{\textit{Error}}\), if the translation failed.</p>

<p>
The translation from DDL value into DDLS value can fail as not every DDL program is a DDLS program.
To indicate failures, values of type \(\ddls{\textit{Error}}\) are used. The following values,
called errors, of that type are defined:</p>

<ul>
  <li>\(\CyclicDefinitionError\)</li>
  <li>\(\SchemaAlreadySpecifiedError\)</li>
  <li>\(\SchemaNotFoundError\)</li>
  <li>\(\InvalidDefinitionError\)</li>
  <li>\(\ValidationError\)</li>
</ul>

<p>
We denote the set of lists by \(\mathbb{L}\).
The empty list is denoted by \(\left[\right]\).
We denote the concatenation of two lists \(a\) and \(b\) by \(a \circ b\).
The set of Unicode code point sequences is denoted by \(\mathbb{U}\).
</p>

@{include("./any-type.i")}
@{include("./boolean-type.i")}
@{include("./choice-type.i")}
@{include("./list-type.i")}
@{include("./map-type.i")}
@{include("./number-type.i")}
@{include("./schema-type.i")}
@{include("./schema-reference-type.i")}
@{include("./string-type.i")}
@{include("./void-type.i")}

\(
\def\ValidationContext{{\ddls{\textit{Context}}}}
\)

<h2 id="validation">4 Validation</h2>
<p>
Given a value \(x \in \ddls{\textit{Schema}}\),
we denote the 2nd element of the tuple, the name of the schema, by \(x.\text{name}\).

We denote the 3rd element of the tuple, the definition, by \(x.\text{definition}\).

Given a value \(x \in \ddls{\textit{SchemaReference}}\),
we denote the 2nd element of the tuple, the name of the schema, by \(x.\text{name}\).

Given a value \(x \in \ddls{\textit{List}}\),
we denote the 2nd element of the tuple, the element, by \(x.\text{element}\),

Given a value \(x \in \ddls{\textit{Map}}\),
we denote the 2nd element of the tuple, the entries, by \(x.\text{entries}\),

Given a value \(x \in \ddls{\textit{MapEntry}}\),
we denote the 2nd element of the tuple, the name, by \(x.\text{name}\) and
the 3d element, the value, by \(x.\text{value}\).
<p>

<p>A validation context \(c \in \ValidationContext\) is a list of \(\ddls{\textit{Schema}}\) values.</p>

<h3>4.1 DDL value validation</h3>
<p>The DDL value validation function
\[
\text{validate} : \ValidationContext \times \mathbb{U} \times \ddl{\textit{Value}} \rightarrow \mathbb{B}
\]
maps from a validation context \(c\), the name of a starting schema \(s\), and a DDL value \(v\) to boolean values:
It evaluates to \(\text{true}\) if the DDL value was accepted by the validation context with the given starting schema name.
Otherwise it returns \(\text{false}\).
</p>

<p>The function is defined as follows:</p>

<ul>
  <li>Return a \(\textit{false}\) if there is no \(a \in c\) with \(a.\text{name} = \text{s}\).</li>
  <li>Return a \(\textit{false}\) if there two schemas \(a, b \in c\) such that \(a \neq b\) but \(a.\text{name} = b.\text{name}\).</li>
  <li>Otherwise let \(a \in c\) with \(a.\text{name} = s\).</li>
  <li>Evaluate to \(\text{validate}'(c, a, v)\)</li>
</ul>

<h3>4.2 DDL value validation'</h3>
<p>The DDL value validation' function
\[
\text{validate}' : \ValidationContext \times \ddls{\textit{Value}} \times \ddl{\textit{Value}} \rightarrow \mathbb{B}
\]
maps from a validation context \(c\), a DDLS value \(a\), and a DDL value \(v\) to boolean values:
It evaluates to \(\text{true}\) if the DDL value was accepted by the validation context/DDLS value.
Otherwise it returns \(\text{false}\).
</p>

<p>The function is defined as follows:</p>

<ul>
  <li>
    If \(a \in \ddls{\textit{SchemaReference}}\) let \(a = \text{resolve}(c, a)\).
    if \(a = \false\) return \(\false\).
  </li>

  <li>If \(v\) is a DDL boolean value:
      <ul>
        <li>Return \(\false\) if \(a\) is not a value of type \(\ddls{\textit{Boolean}}\).</li>
        <li>Otherwise return \(\true\).</li>
      </ul>
  </li>
  <li>If \(v\) is a DDL list value:
      <ul>
        <li>Return \(\false\) if \(a\) is not a value of type \(\ddls{\textit{List}}\).</li>
        <li>Otherwise return \(\text{listValidation}(c, a, v)\).</li>
      </ul>
  </li>
  <li>If \(v\) is a DDL map value:
      <ul>
        <li>Return \(\false\) if \(a\) is not a value of type \(\ddls{\textit{Map}}\).</li>
        <li>Otherwise return \(\text{mapValidation}(c, a, v)\).</li>
      </ul>
  </li>
  <li>If \(v\) is a DDL number value:
      <ul>
        <li>Return \(\false\) if \(a\) is not a value of type \(\ddls{\textit{Number}}\).</li>
        <li>Otherwise return \(\true\).</li>
      </ul>
  </li>
  <li>If \(v\) is a DDL string value:
      <ul>
        <li>Return \(\false\) if \(a\) is not a value of type \(\ddls{\textit{String}}\).</li>
        <li>Otherwise return \(\true\).</li>
      </ul>
  </li>
  <li>If \(v\) is a DDL void value:
      <ul>
        <li>Return \(\false\) if \(a\) is not a value of type \(\ddls{\textit{Void}}\).</li>
        <li>Otherwise return \(\true\).</li>
      </ul>
  </li>
</ul>

<h3>4.3 DDL list value validation</h3>
<p>The DDL list value validation function
\[
\text{validateList} : \ValidationContext \times \ddls{\textit{Value}} \times \ddl{\textit{List}} \rightarrow \mathbb{B}
\]
maps from a validation context \(c\), a DDLS value \(a\), and a DDL list value \(v\) to boolean values:
It evaluates to \(\true\) if the DDL list value was accepted by the validation context/DDLS value.
Otherwise it returns \(\false\).</p>

<p>The function is defined as follows:</p>

<ul>
  <li>If \(a \not\in\ddls{\textit{List}}\), return \(\false\).</li>
  <li>
  \(\textit{validate}'\left(c, a.\text{element}, w\right)\) is evaluated for each \(w \in v.\textit{elements}\).
  </li>
  <li>
  If there is an evaluation \(\false = \textit{validate}\left(c, a.\text{element}.\text{name}, w\right)\) then return \(\false\).
  Otherwise return \(\true\).
  </li>
</ul>

<h3>4.4 DDL map value validation</h3>
<p>The DDL map value validation function
\[
\text{validateMap} : \ValidationContext \times \ddls{\textit{Value}} \times \ddl{\textit{Map}} \rightarrow \mathbb{B}
\]
maps from a validation context \(c\), a DDLS value \(s\), and a DDL map value \(v\) to boolean values:
It evaluates to \(\true\) if the DDL map value was accepted by the validation context/DDLS value.
Otherwise it returns \(\false\).</p>

<p>The function is defined as follows:</p>

<ul>
  <li>If \(a \not\in \ddls{\textit{Map}}\), return \(\text{false}\).</li>
  <li>
  <!-- This should be checked during translation. -->
  Return a \(\false\) if there exist \(b', b'' \in a.\textit{entries}\) such that \(b' \neq b''\) and \(b'.\text{name} = b''.\text{name}\).
  That is, the DDLS map value must not have two map entries with the same name.
  </li>
  <li>
  <!-- Also define the "entries" alias for DDL values. -->
  Return a \(\false\) if there exist \(u', u'' \in v\) such that \(u' \neq u''\) and \(u'_0 = u''_0\).
  That is, the DDL map value must not have two map entries with the same name.
  </li>
  <li>
  <!-- Also define the "name" alias for DDL values. -->
  Return \(\false\) if there exists \(u' \in v\) such that no \(b' \in a.\textit{entries}\) with \(u'_0 = b'.\text{name}\).
  That is, if a map entry with a certain name was specified in \(v\) then it must be present in \(a\).
  </li>
  <li>
  Return \(\false\) if there exists \(b' \in a.\text{entries}\) such that no \(u' \in v\) with \(u'_0 = b'.\text{name}\).
  That is, if a map entry with a certain name is present in \(a\) then it must be specified in \(v\).
  </li>

  <li>
  For each \(b' \in a.\textit{entries}\) and for each \(u' \in v\)
  \(\textit{validate}'\left(x, b'_1, u'_1\right)\) is invoked if \(u'_0 = b'.\text{name}\).
  If there is an invocation \(\false = \textit{validate}'\left(c, b'_1, u'_1\right)\) return \(\false\).
  Otherwise return \(\true\).
  </li>
</ul>

<h3>4.4 Schema reference resolution</h3>

<p>The DDL value validation function
\[
\text{resolve} : \ValidationContext \times \ddls{\textit{SchemaReference}} \rightarrow \ddls{\textit{Schema}} \cup \mathbb{B}
\]
maps from a validation context \(c\) and a DDLS schema reference \(s\) to a DDLS schema or to \(\false\):
</p>

<p>The function is defined as follows:</p>

<ul>
  <li>Let \(v = \emptyset\).</li>
  <li>While \(s\) is a \(\ddls{\textit{SchemaReference}}\) type:
    <ul>
      <li>Find \(s' \in c, s' \in \ddls{\textit{Schema}}\) such that \(s'.\text{name} = s.\text{name}\).
          Return a \(\textit{false}\) if no such \(s'\) was found.</li>
      <li>Return a \(\textit{false}\) if \(s' \in v\).</li>
      <li>Let \(s = s'\) and \(v = v \cup \{ s' \}\).</li>
    </ul>
  </li>
  <li>Return \(s\).</li>
</ul>

<h3>4.4 Acceptance and Rejection</h3>
<p>\(\textit{validate}(S, s, x)\) always terminates. \(x\) was accepted by \(S\) with \(s\) if the result is \(\ddls{\textit{success}}\).
Otherwise - if the result is in \(\ddls{\textit{Error}}\) then \(x\) was rejected by \(S\) with \(s\).</p>
