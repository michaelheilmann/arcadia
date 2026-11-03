/* Legacy consumers are identified by their existing canonical metadata until
 * their page-owned migration opts into body.specification. :where keeps the
 * compatibility rules' specificity unchanged and excludes non-spec pages. */
:where(html:has(> head > link[rel="canonical"][href*="/specifications/"])) {
  & my-lhs { display: block; }
  & my-rhs { display: block; margin-left: 1rem; }

  & div.box {
    --base-color: rgb(219, 212, 205);
    --background-color: color-mix(in srgb, var(--base-color) 20%, white 100%);
    --border-color: color-mix(in srgb, var(--base-color) 100%, white 20%);
    padding: 8px;
    margin: 1rem 0;
    border: 1.75px solid var(--border-color);
    border-radius: 4px;
    background-color: var(--background-color);
  }
  & div.box.todo {
    --todo-base-color: rgb(234, 136, 145);
    --todo-background-color: color-mix(in srgb, var(--todo-base-color) 20%, white 100%);
    --todo-border-color: color-mix(in srgb, var(--todo-base-color) 100%, white 20%);
    border-color: var(--todo-border-color);
    background-color: var(--todo-background-color);
  }
  & div.abstract { font-style: italic; }

  & my-formula-box.align-left > mjx-container { text-align: left!important; }
  & my-formula-box.align-right > mjx-container { text-align: right!important; }

  & table.syntax-directed-translation {
    width: 80%;
    margin-left: 10%;
    margin-right: 10%;
  }
  & table.syntax-directed-translation,
  & :where(table.syntax-directed-translation) tr,
  & :where(table.syntax-directed-translation) td {
    border: 1px solid black;
    border-collapse: collapse;
  }
  & table.syntax-directed-translation td {
    padding: 16px;
    overflow-x: auto;
    overflow-y: hidden;
  }
  & table.syntax-directed-translation td:nth-child(2) {
    width: 50%;
    text-align: right;
  }
}

/* Approved specification-family components. Page migrations own the opt-in marker
 * and semantic markup; unrelated shared-style consumers keep the legacy shell. */
body.my-content-page-1.specification {
  --specification-width: 78rem;
  --specification-navigation-width: 17rem;
  --specification-surface: #f5f7f8;
  --specification-border: #ccd4da;
  --specification-divider: #d8e1e2;
  --specification-focus: #875100;

  min-width: 0;
  width: 100%;
  height: auto;
  line-height: 1.65;

  & main {
    display: grid;
    grid-template-columns: minmax(0, 1fr) var(--specification-navigation-width);
    gap: 3rem;
    max-width: var(--specification-width);
    margin-inline: auto;
    padding: 2rem;
    box-sizing: border-box;
    width: 100%;
  }
  & main > .left-column { display: none; }
  & main > .center-column { grid-column: 1; grid-row: 1; min-width: 0; width: auto; }
  & main > .right-column { grid-column: 2; grid-row: 1; min-width: 0; width: auto; }

  & h1 { font-size: clamp(2rem, 3.5vw, 2.75rem); line-height: 1.2; margin: 0 0 1.5rem; }
  & h2 { font-size: 1.6rem; line-height: 1.3; margin: 2.5rem 0 1rem; padding-top: 1rem; border-top: 1px solid var(--specification-divider); }
  & h3 { font-size: 1.25rem; line-height: 1.4; margin: 2rem 0 0.75rem; }
  & h4 { font-size: 1.125rem; line-height: 1.4; }
  & main p { overflow-wrap: anywhere; }
  & a { text-decoration: underline; text-underline-offset: 0.15em; }

  & .box {
    padding: 1rem;
    margin: 1rem 0;
    background: var(--specification-surface);
    border: 1px solid var(--specification-border);
    border-radius: 4px;
    min-width: 0;
  }
  & .box.todo { border-style: dashed; }
  & .box:has(> .body > pre), & .box:has(> pre) { padding: 0; overflow: hidden; }
  & pre {
    max-width: 100%;
    box-sizing: border-box;
    background: var(--specification-surface);
    padding: 1rem;
    overflow-x: auto;
    line-height: 1.6;
  }
  & .box pre { margin: 0; }
  /* Keep native mathematical layout and fonts; constrain only display boxes. */
  & mjx-container[display="true"] { max-width: 100%; overflow-x: auto; overflow-y: hidden; }
  & my-formula-box { display: block; min-width: 0; }

  /* Scroll the enclosing region, never detach a table's rows/cells from its grid. */
  & .specification-scroll { max-width: 100%; overflow-x: auto; margin: 1rem 0; }
  & table { border-collapse: collapse; margin: 1rem 0; width: 100%; }
  & .specification-scroll > table { margin: 0; }
  & table.syntax-directed-translation { width: 100%; margin: 0; }
  & th, & td { border: 1px solid var(--specification-border); padding: 0.75rem; text-align: left; vertical-align: top; }
  & th { background: var(--specification-surface); }
  & caption { font-weight: bold; text-align: left; margin-bottom: 0.5rem; }
  & .specification-scroll-hint { display: block; font-size: 0.875rem; font-weight: normal; color: #465a65; }
  & table.syntax-directed-translation td:nth-child(2) { width: auto; text-align: left; }

  & main > .right-column nav { padding: 0; }
  & .specification-contents { border: 1px solid var(--specification-border); border-radius: 4px; padding: 1rem; background: var(--specification-surface); }
  & .specification-contents summary { cursor: pointer; font-weight: bold; }
  & .specification-contents nav ul { list-style: none; margin: 1rem 0 0; padding: 0; }
  & .specification-contents nav ul > li { margin: 0 0 0.5rem; }
  & .specification-contents nav ul ul { padding-left: 1rem; }
  & nav a { text-decoration: none; }
  & nav a:hover { text-decoration: underline; }
  & :focus-visible { outline: 3px solid var(--specification-focus); outline-offset: 3px; }
  & .specification-skip { position: absolute; top: -5rem; left: 1rem; padding: 0.5rem; background: var(--white); }
  & .specification-skip:focus { top: 0.5rem; }
  & footer { flex-wrap: wrap; }
}

@@media (max-width: 70rem) {
  body.my-content-page-1.specification {
    & main { display: flex; flex-direction: column; gap: 1.5rem; padding: 1rem; }
    & main > .right-column, & main > .center-column { width: 100%; }
  }
}
