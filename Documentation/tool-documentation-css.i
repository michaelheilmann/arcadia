/*
 * Arcadia tool documentation: shared code typography and vertical rhythm.
 * Opt in using the my-tool-documentation class on the page body.
 * Keep the rules scoped: non-tool documentation has its own layouts.
 */
body.my-tool-documentation {
  --tool-inline-code-background: #e9edf2;
  --tool-code-block-background: #f1f3f6;
  --tool-code-border: #d6dce4;
}

/* Inline code should be distinct even inside prose, lists and table cells. */
body.my-tool-documentation code {
  padding: 0.12em 0.35em;
  border: 1px solid var(--tool-code-border);
  border-radius: 0.25rem;
  background-color: var(--tool-inline-code-background);
  font-family: var(--font-family-code);
  box-decoration-break: clone;
  -webkit-box-decoration-break: clone;
}

/* Block examples get consistent separation from adjacent paragraphs. */
body.my-tool-documentation pre {
  box-sizing: border-box;
  margin: 1rem 0 1.25rem;
  padding: 1rem 1.25rem;
  border: 1px solid var(--tool-code-border);
  border-radius: 0.4rem;
  background-color: var(--tool-code-block-background);
  line-height: 1.5;
  white-space: pre;
  overflow-x: auto;
}

/* Avoid an inline-code pill or double background inside a code block. */
body.my-tool-documentation pre > code {
  display: block;
  padding: 0;
  border: 0;
  border-radius: 0;
  background: transparent;
  white-space: inherit;
}

/* Separate option tables from the paragraphs before and after them. */
body.my-tool-documentation main > .center-column table {
  border-collapse: collapse;
  margin: 0.75rem 0 1.25rem;
}

body.my-tool-documentation main > .center-column table :is(th, td) {
  padding: 0.45rem 0.75rem;
  text-align: left;
  vertical-align: top;
}
