<h2 id="semantical-translation">Semantical translation</h2>
<p>Semantical translation maps a sentence to a typed value. The <em>Data Definition Language</em> has six basic types:
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
(see <a href="#references">Aho, Sethi and Ullman</a> for background).
</p>

<p>At the end of translation, a value form \(x\) has an attribute \(x.\text{value}\) of type \(\textit{Value}\).
In each table, numbered occurrences associate a grammar production with its semantic action.
The operator \(:=\) assigns an attribute; \(\circ\) concatenates lists, \([a]\) is a singleton list and \([]\) is an empty list.
The attribute \(\textit{codePoints}\) means the word's associated text: the original numeric spelling for Number,
the name spelling for Name, but the decoded contents (without quotes) for String. It does not always mean the raw source lexeme.</p>
<p>The current reader builds DDL nodes. Boolean and Void nodes store keyword spellings, Number nodes store numeric text,
and String nodes store decoded UTF-8 text. The rules below describe the denoted values, not a promise that every node is
converted to a native numeric or Boolean runtime type. Semantic analysis is an explicit operation;
<code>DefaultReader_run</code> parses but does not itself invoke duplicate-name checking.</p>

<h3 id="semantical-translation-scalar-types">Scalar types</h3>
<p>The scalar rules translate a single word into its denoted value.</p>

<h4 id="type-boolean">Boolean type</h4>
<p>The type \(\textit{Boolean}\) has two values, \(\textit{true}\) and \(\textit{false}\), expressed by
<code>true</code> and <code>false</code> (\(\text{Lexical.True}\) and \(\text{Lexical.False}\)).
The older names <code>DDL.Lexical.true</code> and <code>DDL.Lexical.false</code> refer to these same keywords.</p>

<p>The semantical translation is defined as follows:</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="type-boolean">
<table class="syntax-directed-translation">
  <caption>Boolean translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Production</th><th scope="col">Semantic action</th></tr></thead>
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
</div>

<h4 id="type-number">Number type</h4>
<p>
The type \(\textit{Number}\) retains a sequence of code points satisfying \(\text{DDL.Syntactical.Number}\).
Its representation is textual: for example, <code>01</code> and <code>+1</code> remain distinct spellings.
Decimal notation allows an optional sign, an optional decimal point (with digits on at least one side), and an optional exponent
introduced by <code>e</code> or <code>E</code> with an optional sign and at least one digit.</p>

<p>The semantical translation is defined as follows:</p>

<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="type-number">
<table class="syntax-directed-translation">
  <caption>Number translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Production</th><th scope="col">Semantic action</th></tr></thead>
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
</div>

<p><b>Representation and precision:</b> Reading a Number does not perform arithmetic or convert it to <code>int</code> or <code>float</code>.
The grammar does not impose a fixed machine-number range, but storage is bounded by implementation resources.
Conversions performed by consumers have their own range, precision and error behavior; this specification does not promise arbitrary-precision numeric operations.</p>

<h4 id="type-string">String type</h4>
<p>
The \(\textit{String}\) type represents decoded text, not its quoted source spelling.
For example, <code>"\u{41}"</code> denotes the same text as <code>"A"</code>.
DDL strings are delimited only by double quotes; single-quoted strings and character literals are not supported.
The reader decodes <code>\"</code>, <code>\\</code>, <code>\b</code>, <code>\f</code>,
<code>\n</code>, <code>\r</code>, <code>\t</code>, <code>\v</code>, <code>\0</code> and braced <code>\u{...}</code> escapes.
Each Unicode escape requires one to six digits from <code>0-9</code>, <code>a-f</code> or <code>A-F</code> inside braces.
The decoded value must be at most U+10FFFF and outside U+D800 through U+DFFF (the surrogate range).
Malformed escapes and non-scalar values cause a lexical error; <code>\uXXXX</code> is no longer accepted.
Supplementary scalar values are supported directly as UTF-8 or through braced escapes, with identical decoded text.
Unescaped code points U+0000 through U+001F are rejected. Embedded NUL is part of the text, not a terminator.
No Unicode normalization is performed. Surrogate-pair escapes are not supported: use the scalar value itself, for example <code>\u{1F3EE}</code>.</p>

<p>The semantical translation is defined as follows:</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="type-string">
<table class="syntax-directed-translation">
  <caption>String translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Production</th><th scope="col">Semantic action</th></tr></thead>
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
</div>

<h4 id="type-void">Void type</h4>
<p>
The type \(\textit{Void}\) has a single value, \(\textit{void}\), expressed by the keyword <code>void</code>.
</p>

<p>The semantical translation is defined as follows:</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="type-void">
<table class="syntax-directed-translation">
  <caption>Void translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Production</th><th scope="col">Semantic action</th></tr></thead>
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
    \text{DDL.Syntactical.Void}@@1 : \text{Lexical.Void}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
    \{ 1.\text{value} := \textit{void} \}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
</div>

<h3 id="semantical-translation-list-type">List type</h3>
<p>
The type \(\textit{List}\) represents an ordered list of values of type \(\textit{Value}\).
A list is expressed by \(\text{DDL.Syntactical.List}\). The rules prepend each element to the translated remainder;
empty bodies and trailing commas contribute an empty list.
</p>

<p>The semantical translation is defined as follows:</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="semantical-translation-list-type">
<table class="syntax-directed-translation">
  <caption>List translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Production</th><th scope="col">Semantic action</th></tr></thead>

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
      1.\text{value} := 2.\text{value}
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
      1.\text{value} := \left[ 2.\text{value} \right] \circ 3.\text{value}
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
      1.\text{value} := []
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
      1.\text{value} := []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

</table>
</div>

<p><b>Example</b></p>
<div class="box">
<pre tabindex="0" aria-label="List example"><code>// A list with three numbers 1, 2, and 3.
[ 1, 2, 3 ]
</code></pre>
</div>

<h3 id="semantical-translation-map-type">Map type</h3>
<p>
The type \(\textit{Map}\) represents an ordered list of entries. Each entry is a pair: a name spelling (\(\textit{Name}\), not a seventh value type)
and a value of type \(\textit{Value}\). Name comparison is case-sensitive. Names must be unique within each map;
duplicate names cause a semantic error, including in nested maps. The same name may occur in different maps.
A value of type \(\textit{Map}\) is expressed in the language by the sentence \(\text{DDL.Syntactical.Map}\) (as defined in the syntactical grammar).
</p>

<p>The semantical translation is defined as follows:</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="semantical-translation-map-type">
<table class="syntax-directed-translation">
  <caption>Map translation (subject to unique names)<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Production</th><th scope="col">Semantic action</th></tr></thead>

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
      1.\text{value} := 2.\text{value}
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
      1.\text{value} := \left[ 2.\text{value} \right] \circ 3.\text{value}
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
      1.\text{value} := []
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
      1.\text{value} := []
    \end{array}\]</my-formula-box>
    </td>
  </tr>

  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.MapBodyElement}@@1 : \text{Lexical.Name}@@2\;\text{Lexical.Colon}\;\text{DDL.Syntactical.Value}@@3
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := \left(2.\text{codePoints}, 3.\text{value}\right)
    \end{array}\]</my-formula-box>
    </td>
  </tr>

</table>
</div>

<h3 id="semantical-translation-value-type">Value type</h3>
<p>
The type \(\textit{Value}\) is the union of the types
\(\textit{Boolean}\),
\(\textit{List}\),
\(\textit{Map}\),
\(\textit{Number}\),
\(\textit{String}\), and
\(\textit{Void}\).
</p>

<p>The semantical translation is defined as follows:</p>
<div class="specification-scroll" tabindex="0" role="region" aria-labelledby="semantical-translation-value-type">
<table class="syntax-directed-translation">
  <caption>Value translation<span class="specification-scroll-hint">Scroll horizontally if needed.</span></caption>
  <thead><tr><th scope="col">Production</th><th scope="col">Semantic action</th></tr></thead>
  <tr>
    <td>
    <my-formula-box class="align-left">\[\begin{array}{ll}
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Boolean}@@2
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
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.List}@@2
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
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Map}@@2
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
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Number}@@2
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
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.String}@@2
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
      \text{DDL.Syntactical.Value}@@1 : \text{DDL.Syntactical.Void}@@2
    \end{array}\]</my-formula-box>
    </td>
    <td>
    <my-formula-box class="align-right">\[\begin{array}{ll}
      1.\text{value} := 2.\text{value}
    \end{array}\]</my-formula-box>
    </td>
  </tr>
</table>
</div>
