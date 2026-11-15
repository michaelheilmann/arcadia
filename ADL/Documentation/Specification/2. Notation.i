      <h2 id="notation">2 Notation</h2>
      <p>
      ADL assets are written in the
      <a href="@{siteAddress}/specifications/data-definition-language/">Arcadia Data Definition Language</a>
      (Arcadia DDL), whose lexical and syntactical grammar uses conventions from
      <a href="@{siteAddress}/specifications/common-lexical-translations/">Arcadia Common Lexical Translations</a>.
      DDL map entry syntax has the form <code>name : value</code>, with entries
      separated by commas according to DDL. Here, the name is a DDL identifier
      and the value is any DDL value kind permitted by the entry's ADL schema.
      These entry examples are DDL syntax, not new ADL grammar productions;
      the ADL specification imposes semantic constraints on DDL maps.
      In particular, a nested DDL map is not necessarily an ADL definition.
      </p>
      <p>
      DDL provides <b>String</b>, <b>Number</b>, <b>Boolean</b>, <b>Void</b>,
      <b>List</b> and <b>Map</b> values. This specification uses strings such as
      <code>"Colors.Red"</code>, numbers such as <code>255</code>, lists such as
      <code>[1.0, 0.0]</code>, and maps such as <code>{red: 255}</code>.
      Some ADL entries encode a Boolean choice with the strings <code>"true"</code>
      and <code>"false"</code>; these strings are not DDL Boolean values.
      </p>
      <p>
      Entry lists below are annotated with the precision the reader consumes a number
      with: <code>Natural8</code>, <code>Natural32</code>, <code>Integer32</code>,
      <code>Real32</code>, or <code>Real64</code>. An entry annotated
      <i>optional</i> may be omitted, and the annotation records the value the reader
      substitutes when it is omitted. An entry not annotated <i>optional</i> is
      mandatory; an omitted mandatory entry, an entry of the wrong value kind, and an
      entry unknown to the schema are all semantical errors.
      </p>
