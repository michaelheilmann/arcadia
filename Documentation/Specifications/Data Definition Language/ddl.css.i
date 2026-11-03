/* Only DDL's reader-guide accents and diagnosed formal-layout exceptions remain.
 * Layout, light code panels, navigation, tables and focus use specification.css.i. */
.ddl-spec .ddl-lead { font-size: 1.2rem; color: #465a65; }
.ddl-spec .ddl-label { color: #235f6b; font-weight: bold; letter-spacing: 0.08em; text-transform: uppercase; }
/* Match display semantic actions to inline actions; do not alter math fonts. */
.ddl-spec table.syntax-directed-translation my-formula-box > mjx-container { text-align: left!important; }
/* Preserve the accepted DDL display-formula glyph clearance during migration. */
.ddl-spec mjx-container[display="true"] { padding-bottom: 0.5rem; }
