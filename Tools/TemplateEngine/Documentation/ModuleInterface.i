<section id="ModuleInterface">
  <h2>Module Interface</h2>
  <h3>Command-line interface</h3>
  <pre><code>Arcadia.TemplateEngine.CLI [options]</code></pre>
  <table>
    <thead><tr><th>Option</th><th>Contract</th></tr></thead>
    <tbody>
      <tr><td><code>--source="path"</code></td><td>Required: UTF-8 input template.</td></tr>
      <tr><td><code>--target="path"</code></td><td>Required: rendered output file.</td></tr>
      <tr><td><code>--dependencies="path"</code></td><td>Required: output dependency file.</td></tr>
      <tr><td><code>--environment="path"</code></td><td>Optional: DDL map supplying string variables.</td></tr>
      <tr><td><code>--help</code></td><td>Print CLI usage; currently exits with failure.</td></tr>
    </tbody>
  </table>
  <p>The command-line parser expects double-quoted path values; ensure those
  quotes reach the application when invoking it from a shell. CMake's
  <code>OnTemplateEngine</code> integration handles argument construction.
  The CLI also recognizes <code>--log</code>, but its value is not currently
  used by the render path. It is not a supported logging interface.</p>

  <h3>Library entry points</h3>
  <p>The implementation is provided by the <code>Arcadia.TemplateEngine</code>
  library. These current C entry points are declared under
  <code>Tools/TemplateEngine/Library/Sources/Arcadia.TemplateEngine</code>:</p>
  <pre><code>Context*
Context_create
  (
    Arcadia_Thread* thread
  );

void
Context_onRun
  (
    Arcadia_Thread* thread,
    Context* self
  );

Environment*
Environment_create
  (
    Arcadia_Thread* thread,
    Environment* enclosing
  );

Environment*
Environment_loadString
  (
    Arcadia_Thread* thread,
    Arcadia_String* source
  );

Arcadia_Value
Environment_get
  (
    Arcadia_Thread* thread,
    Environment* self,
    Arcadia_Value key,
    Arcadia_BooleanValue recursive
  );

void
Environment_set
  (
    Arcadia_Thread* thread,
    Environment* self,
    Arcadia_Value key,
    Arcadia_Value value
  );</code></pre>
  <p><code>Context</code> stores source, target, and dependency paths and the
  evaluation environment. <code>Context_onRun</code> renders the configured
  source and writes the dependency description; the CLI writes the final
  target bytes. <code>Environment_loadString</code> parses a DDL map into
  variables. These are implementation-level C interfaces: callers must
  supply the normal Arcadia process/thread environment and error-handling
  discipline. This page does not promise a stable independent C ABI.</p>

  <h3>CMake integration</h3>
  <p>The preferred build-system interface is implemented by
  <code>CMake/tools-template-engine.cmake</code>:</p>
  <pre><code>BeginTemplateEngine(${MyProjectName}.Example.BuildDocs "Example")
  OnTemplateEngine("${CMAKE_CURRENT_SOURCE_DIR}/index.html.te"
                   "${REPOSITORY_SOURCE_DIR}/.Website/Example/index.html"
                   "${CMAKE_CURRENT_SOURCE_DIR}/website.env")
EndTemplateEngine()</code></pre>
  <p><code>OnTemplateEngine</code> takes a source template, an output path,
  and an environment-file path. These arguments must be absolute in this
  CMake integration. Each generated page gets a dependency file containing
  its transitive template includes; the template and environment sources
  trigger rebuilds through the custom command.</p>

  <h3>Build and test targets</h3>
  <table>
    <thead><tr><th>Target</th><th>Purpose</th></tr></thead>
    <tbody>
      <tr><td><code>Arcadia.TemplateEngine</code></td><td>Native C template engine library.</td></tr>
      <tr><td><code>Arcadia.TemplateEngine.CLI</code></td><td>Command-line renderer.</td></tr>
      <tr><td><code>Arcadia.TemplateEngine.BuildDocs</code></td><td>Generate this documentation page.</td></tr>
    </tbody>
  </table>
  <p>The tests under <code>Tools/TemplateEngine/Tests</code> cover literal
  at-signs, built-in procedures, recursive includes, rendering, and nested
  dependency-file behavior. When editing template parsing or dependency
  generation, run the corresponding CTest cases as well as regenerating
  the website.</p>
</section>
