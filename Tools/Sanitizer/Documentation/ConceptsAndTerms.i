<section id="ConceptsAndTerms">
  <h2>Concepts and Terms</h2>
  <dl>
    <dt>Check mode</dt>
    <dd>Read-only traversal which reports differing whitespace and exits with
    status 1 when at least one inspected text file is nonconforming.</dd>
    <dt>Fix mode</dt>
    <dd>Traversal which writes normalized bytes back to each affected text
    file. Files whose content is already conforming are not rewritten.</dd>
    <dt>Root</dt>
    <dd>The explicitly supplied file or directory; if omitted, the working
    directory. Root directories are traversed recursively.</dd>
    <dt>Eligible source file</dt>
    <dd>A regular file with an allowed extension discovered during directory
    traversal. An explicitly provided regular file is eligible regardless of
    its filename extension.</dd>
    <dt>Normalized bytes</dt>
    <dd>The original byte sequence after converting CRLF and lone CR line
    endings to LF and removing spaces/tabs at the ends of lines.</dd>
    <dt>Binary input</dt>
    <dd>Any inspected file containing a NUL byte. Such files are reported and
    skipped without modification.</dd>
  </dl>
  <p>The scanner operates on bytes rather than decoding text. It does not
  infer a character encoding or interpret language syntax. See the
  <a href="#Specification">Specification</a> for its exact boundaries.</p>
</section>
