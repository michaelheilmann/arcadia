/* DDL-local additions to the common specification page family. */
body.my-content-page-1.ddl-spec { min-width: 0; width: 100%; height: auto; }
.ddl-spec { line-height: 1.65; }
body.my-content-page-1.ddl-spec main p { margin-bottom: 1rem; }
body.my-content-page-1.ddl-spec main > .center-column { padding: 1.5rem; box-sizing: border-box; }
.ddl-spec h2 { border-top: 1px solid #d8e1e2; padding-top: 1rem; }
.ddl-spec a { text-decoration: underline; text-underline-offset: 0.15em; }
.ddl-spec .ddl-lead { font-size: 1.2rem; color: #465a65; }
.ddl-spec .ddl-label { color: #235f6b; font-weight: bold; letter-spacing: 0.08em; text-transform: uppercase; }
.ddl-spec .center-column { min-width: 0; }
body.my-content-page-1.ddl-spec main { min-width: 0; }
.ddl-spec h1, .ddl-spec h2, .ddl-spec h3, .ddl-spec h4, .ddl-spec p { overflow-wrap: anywhere; }
.ddl-spec pre {
  background: #18242e;
  color: #f1f7fa;
  border-left: 0.25rem solid #235f6b;
  padding: 1rem;
  overflow-x: auto;
  line-height: 1.6;
}
html.my-theme-1 .ddl-spec pre { background: #18242e; }
html.my-theme-1 .ddl-spec pre code { color: #f1f7fa; }
.ddl-spec table { border-collapse: collapse; margin: 1rem 0; width: 100%; }
.ddl-spec th, .ddl-spec td {
  border: 1px solid #777;
  padding: 0.6rem;
  text-align: left;
  vertical-align: top;
}
.ddl-spec th { background: #edf3f3; color: #192634; }
.ddl-spec caption { font-weight: bold; text-align: left; margin-bottom: 0.5rem; }
.ddl-spec .ddl-table { overflow-x: auto; }
.ddl-spec .syntax-directed-translation { display: block; overflow-x: auto; width: 100%; margin: 1rem 0; }
.ddl-spec .syntax-directed-translation td:nth-child(2) { text-align: left; }
.ddl-spec mjx-container[display="true"] { max-width: 100%; overflow-x: auto; overflow-y: hidden; padding-bottom: 0.5rem; }
.ddl-spec :focus-visible { outline: 3px solid #875100; outline-offset: 3px; }
.ddl-spec .ddl-skip { position: absolute; left: 0.5rem; top: -5rem; padding: 0.5rem; background: white; }
.ddl-spec .ddl-skip:focus { top: 0.5rem; }
.ddl-spec nav ul { padding-left: 1rem; }
@@media (max-width: 70rem) {
  body.my-content-page-1.ddl-spec main { display: flex; flex-direction: column; }
  body.my-content-page-1.ddl-spec main > .left-column { display: none; }
  body.my-content-page-1.ddl-spec main > .center-column,
  body.my-content-page-1.ddl-spec main > .right-column { box-sizing: border-box; width: 100%; min-width: 0; margin: 0; padding: 1rem; }
  body.my-content-page-1.ddl-spec main > .right-column { position: static; }
  body.my-content-page-1.ddl-spec footer { flex-wrap: wrap; }
  .ddl-spec .syntax-directed-translation { display: block; overflow-x: auto; }
}
