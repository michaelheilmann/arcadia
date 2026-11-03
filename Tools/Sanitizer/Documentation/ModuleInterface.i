<section id="ModuleInterface">
  <h2>Module Interface</h2>
  <p>The native public library interface is defined in
  <code>Tools/Sanitizer/Library/Sources/Arcadia/Sanitizer/Sanitizer.h</code>.
  It depends on Arcadia FileSystem and Ring2. The executable is implemented
  separately in <code>Tools/Sanitizer/Tool</code>.</p>

  <h3>Arcadia_Sanitizer_normalize</h3>
  <pre><code>Arcadia_BooleanValue
Arcadia_Sanitizer_normalize
  (
    Arcadia_Thread* thread,
    Arcadia_ByteArrayBuilder* input,
    Arcadia_ByteArrayBuilder* output,
    Arcadia_SizeValue* firstAffectedLine
  );</code></pre>
  <p>Normalizes the input byte sequence into a distinct output builder.
  Returns true if any byte would change; otherwise returns false.
  <code>firstAffectedLine</code> receives the 1-based first affected line
  number or zero when unchanged. The input and output builders must be
  different objects. The caller owns the supplied builders and output
  parameter.</p>

  <h3>Arcadia_Sanitizer_Statistics</h3>
  <pre><code>typedef struct Arcadia_Sanitizer_Statistics {
  Arcadia_SizeValue inspected;
  Arcadia_SizeValue changed;
  Arcadia_SizeValue skippedBinary;
  Arcadia_SizeValue skippedLinks;
} Arcadia_Sanitizer_Statistics;</code></pre>
  <p><code>inspected</code> counts regular files read for normalization,
  including binary files. <code>changed</code> counts files whose
  normalized bytes differ, including files fixed in fix mode.
  <code>skippedBinary</code> and <code>skippedLinks</code> count
  ignored binary inputs and links respectively. Initialize this structure
  to zero before a scan.</p>

  <h3>Arcadia_Sanitizer_scan</h3>
  <pre><code>void
Arcadia_Sanitizer_scan
  (
    Arcadia_Thread* thread,
    Arcadia_FileSystem* fileSystem,
    Arcadia_FilePath* path,
    Arcadia_FileHandle* output,
    Arcadia_BooleanValue fix,
    Arcadia_Sanitizer_Statistics* statistics
  );</code></pre>
  <p>Scans a file or directory recursively, writes diagnostics to the
  supplied open file handle, and updates the provided statistics. When
  <code>fix</code> is false, no files are rewritten. When true, only
  files with differing normalized bytes are rewritten. This library
  function does not return a CLI exit status; the command-line wrapper
  maps the statistics and failures to statuses 0, 1, and 2.</p>
  <p>The library uses Arcadia's normal thread error/status and jump-target
  conventions. Its caller must establish the required Arcadia process and
  thread environment, and manage errors according to those conventions.</p>

  <h3>Build targets and tests</h3>
  <table>
    <thead><tr><th>Target</th><th>Purpose</th></tr></thead>
    <tbody>
      <tr><td><code>Arcadia.Sanitizer</code></td><td>Reusable native C library.</td></tr>
      <tr><td><code>Arcadia.Sanitizer.Tool</code></td><td>Command-line executable.</td></tr>
      <tr><td><code>Arcadia.Sanitizer.Tests</code></td><td>Native regression suite, registered with CTest.</td></tr>
      <tr><td><code>Arcadia.Sanitizer.BuildDocs</code></td><td>Generate this documentation with the Arcadia Template Engine.</td></tr>
    </tbody>
  </table>
  <p>The documentation source resides in <code>Tools/Sanitizer/Documentation</code>.
  The generated site is <code>.Website/Arcadia/Sanitizer/index.html</code>.
  Treat generated files as output, not as the primary editing surface.</p>
</section>
