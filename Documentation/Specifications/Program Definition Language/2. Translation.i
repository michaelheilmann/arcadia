      <h2>Translation</h2>
      <p>A program of the Arcadia PDL language is translated into declarations. This translation happens in three phases: The <em>lexical translation</em> translates Unicode code points to words. The <em>syntactical translation</em> filters the resulting sequence of words and then translates these words into a sentence. The <em>semantical translation</em> translates sentences into typed declarations.</p>

      <h3>Module Directories</h3>
      <p>A compiler invocation supplies one or more <em>module directories</em>. A module directory is the root directory of one Arcadia PDL module. If a supplied module directory path is relative, it is interpreted relative to the compiler invocation's working directory.</p>
      <p>Each module directory contains a required compilation unit named <code>Module.mil</code>. The compiler parses this file before the other compilation units of the module. If <code>Module.mil</code> is not a regular file, the compiler reports an error.</p>
      <p>The compiler discovers the remaining compilation units of a module by recursively enumerating the <code>Library/Sources</code> directory below the module directory and selecting regular files with the <code>.mil</code> extension. These files are parsed as compilation units of the same module.</p>
      <p>The current implementation therefore uses the following directory shape:</p>
      <pre><code>&lt;module-directory&gt;/
  Module.mil
  Library/
    Sources/
      ... *.mil</code></pre>
      <p><code>Module.mil</code> must contain exactly one module definition. A missing module definition is an error. More than one module definition is an error. A module definition in any other compilation unit of the module is an error. The name of the module definition does not have to match the names of declarations in <code>Library/Sources</code>.</p>
