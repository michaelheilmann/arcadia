<h2 id="introduction">1. Introduction</h2>
@{include("./0. Abstract.i")}

<p>The rules layed out here are described in terms of the concepts and notations of the <em>Context-Free Grammars</em>
specification (see <a href="@{siteAddress}/specifications/context-free-grammars">@{siteAddress}/specifications/context-free-grammars</a> for more information).</p>

<p>A grammar incorporating these rules must have the set of all Unicode code points as its input alphabet.
Furthermore, the grammar must ensure that ambiguities are resolved.</p>

<p>A <em>word</em> is a general term for an item recognized by a lexical translation. The common
categories do not determine a universal set of words for all languages. Each consuming language
shall define its own word production and lexical start symbol, selecting the common categories
it uses and adding language-specific categories such as keywords or register notation.</p>
<p>The consuming language shall resolve ambiguities among its selected alternatives and specify
which recognized items are emitted, ignored, or significant to its syntax. It shall state any
refinement of a common rule explicitly. Reusing a common category does not require accepting
every other category listed in this specification.</p>
