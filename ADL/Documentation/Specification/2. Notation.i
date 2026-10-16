      <h2 id="notation">2 Notation</h2>
      <p>
      Adl assets are written in the
      <a href="@{siteAddress}/specifications/data-definition-language/">Arcadia Data Definition Language</a>
      (Arcadia DDL). The grammar productions used below are given in the notation of
      <a href="@{siteAddress}/specifications/common-lexical-translations/">Arcadia Common Lexical Translations</a>.
      A map entry line is written as <code>name : value</code> followed by a comma.
      The name is an identifier, the value a literal or a list of literals.
      </p>
      <p>
      The following types of values occur:
      </p>
      <ul>
        <li><b>String</b>, for example <code>"Colors.Red"</code>,</li>
        <li><b>Number</b>, an integer literal such as <code>255</code>,</li>
        <li><b>Boolean</b>, written as the String <code>"true"</code> or as the String <code>"false"</code>, and</li>
        <li><b>List</b>, a comma-separated list of values in square brackets, such as <code>[ 1.0, 0.0 ]</code>.</li>
      </ul>
      <p>
      Entry lists below are annotated with the precision the reader consumes a number
      with: <code>Natural8</code>, <code>Natural32</code>, <code>Integer32</code>,
      <code>Real32</code>, or <code>Real64</code>. An entry annotated
      <i>optional</i> may be omitted, and the annotation records the value the reader
      substitutes when it is omitted. An entry not annotated <i>optional</i> is
      mandatory; an omitted mandatory entry, an entry of the wrong value kind, and an
      entry unknown to the schema are all semantical errors.
      </p>
