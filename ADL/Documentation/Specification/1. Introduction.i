      <h2 id="introduction">1 Introduction</h2>
      <p>
      This document specifies the Arcadia asset definition language, Arcadia ADL for
      short. It is a draft; it is authoritative over prose found elsewhere in this
      documentation and over the doc-comment examples found in the headers of the
      <code>Arcadia.ADL</code> module, which are known to be stale in places.
      </p>
      <p>
      Sections 1 to 3 describe the language as a whole. Sections 4 to 6 specify one
      group of definition types each. Section 7 records the enumerations whose
      literals string entries are drawn from. Section 8 lists, completely and in one
      place, every type name the current implementation accepts; it is the
      authoritative inventory and supersedes any incomplete enumeration of type names
      in earlier drafts of this document.
      </p>
      <p>
      A definition type is specified here exactly as far as the implementation reads
      it. Where the implementation accepts a string without constraining it, this
      document says so instead of inventing a value set. Where the implementation
      stores a name without resolving it, this document says so instead of claiming a
      link-time constraint that does not exist.
      </p>
