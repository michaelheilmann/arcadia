      <h2 id="structure">3 Structure of an asset</h2>
      <p>
      An ADL asset is a DDL map. Every definition of every type has the entry
      <code>type</code> of type String and the entry <code>name</code> of type String:
      </p>
      <pre>
asset : map
asset -&gt; "type" : &lt;string&gt;
          "name" : &lt;string&gt;
          &lt;type-specific entries&gt;
      </pre>
      <p>
      The <code>type</code> entry selects the reader that processes the asset. It must
      be one of the type names listed in section 8; an unidentified <code>type</code>
      is a semantical error. The <code>name</code> entry is the name of the definition
      and must be unique within the application that loads the assets.
      </p>
      <p>
      String entries that name other definitions are <b>references</b>. A reference is
      stored as a name while reading and is <b>resolved</b> while linking: the named
      definition must exist among the loaded definitions, and for the entries marked
      as such below the resolved definition must be of a required type. An
      unresolvable reference, or a reference to a definition of the wrong type, is an
      error reported while linking.
      </p>
      <p>
      Not every entry that names another definition is resolved while linking. Each
      section below states for its own entries whether linking resolves and
      type-checks them, and the entry lists mark such entries as <i>reference</i>.
      </p>
