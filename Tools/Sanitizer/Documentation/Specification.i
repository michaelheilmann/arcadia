<section id="Specification">
  <h2>Specification</h2>
  <h3>Command line and exit statuses</h3>
  <pre><code>Arcadia.Sanitizer.Tool (--check|--fix) [file-or-directory]</code></pre>
  <p>Exactly one mode is required. The optional path is interpreted as a
  native filesystem path and defaults to <code>.</code>. Invalid arguments,
  a missing or inaccessible root, and filesystem/runtime errors produce
  exit status 2. The following exit statuses are part of the CLI contract:</p>
  <table>
    <thead><tr><th>Status</th><th>Meaning</th></tr></thead>
    <tbody>
      <tr><td><code>0</code></td><td>Check found no whitespace differences, or fix completed successfully.</td></tr>
      <tr><td><code>1</code></td><td>Check found at least one whitespace difference.</td></tr>
      <tr><td><code>2</code></td><td>Argument, filesystem, or runtime error.</td></tr>
    </tbody>
  </table>
  <p>Skipped binary and linked files are not counted as formatting violations;
  a check can therefore return zero even when such files were skipped.</p>

  <h3>Normalization</h3>
  <p>The transform is byte-oriented and performs these operations:</p>
  <ol>
    <li>Replace each CRLF pair with one LF byte.</li>
    <li>Replace each remaining CR byte with one LF byte.</li>
    <li>Remove each run of ASCII space or tab bytes immediately before a line
    ending or at the end of the file.</li>
  </ol>
  <p>All other bytes are copied unchanged. In particular, interior spaces,
  byte order marks, encoded non-ASCII bytes, and the presence or absence of
  a final line ending are preserved. The transform is idempotent. It does not
  add a final LF, decode encodings, reorder text, or format C, PDL, or ADL
  syntax.</p>

  <h3>File selection</h3>
  <p>Directory traversal processes regular files with these case-sensitive
  suffixes:</p>
  <p><code>.txt</code>, <code>.c</code>, <code>.h</code>, <code>.i</code>,
  <code>.tl</code>, <code>.te</code>, <code>.pdl</code>,
  <code>.adl</code>, <code>.ddl</code>, <code>.md</code>,
  <code>.cmake</code>, <code>.c.g</code>, <code>.h.g</code>.</p>
  <p>Directory scans exclude child directories named <code>.</code>,
  <code>..</code>, <code>.git</code>, <code>.vs</code>,
  <code>.venv</code>, <code>.Dependencies</code>, <code>build</code>,
  <code>.build</code>, <code>CMakeFiles</code>,
  <code>node_modules</code>, and <code>__pycache__</code>. These
  exclusions apply to encountered child directories, not to a root directory
  explicitly selected by the caller. Hidden files are not otherwise ignored.
  This is not a Git tracked-file-only scan.</p>

  <h3>Symbolic links and special files</h3>
  <p>The sanitizer skips discovered symbolic links and never follows them
  when descending directories. The filesystem test also treats Windows
  reparse points, including junctions, as links. Explicitly selecting a
  symbolic-link or reparse-point root is rejected with status 2. Special
  file types not recognized as regular files or directories are not
  processed. See <a href="@{siteAddress}/Arcadia/FileSystem/">Arcadia
  FileSystem</a> for the underlying API.</p>

  <h3>Reporting and writes</h3>
  <p>A changed file produces one report for its first affected line, for
  example:</p>
  <pre><code>Runtime/FileSystem/Example.c:17: whitespace differs</code></pre>
  <p>In fix mode the suffix is <code>fixed whitespace</code> instead. Binary
  inputs are reported as <code>skipped binary: path</code>; nested links
  as <code>skipped link: path</code>. File-level reporting is not a report
  of every offending line, nor a JSON or machine-readable diagnostics format.
  Directory enumeration order is filesystem-dependent.</p>
  <p>Fix mode writes affected files in place using
  <code>Arcadia_FileSystem_setFileContents</code>. This is not an atomic
  replacement and does not guarantee preservation of modification times
  for changed files. Interrupted writes can leave partial output. Use
  version control or backups, and review the diff before committing.</p>

  <h3>Non-goals and future checks</h3>
  <p>This release checks whitespace only. It does not check header guards,
  C prototypes, C function layouts, PDL/ADL/DDL grammar, documentation
  structure, or generated-code ownership. Future validators should expose
  distinct named checks, deterministic diagnostic locations, independent
  regression tests, and a documented severity/exit-status policy rather
  than silently changing the meaning of this whitespace pass.</p>
</section>
