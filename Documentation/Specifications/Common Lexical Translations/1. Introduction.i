<h2 id="introduction">1. Introduction</h2>
@{include("./0. Abstract.i")}

<p>The rules laid out here use the concepts and notation in the
<a href="@{siteAddress}/specifications/mathematical-definitions-notations-and-proofs/index.html#context-free-grammars">Context-Free Grammars</a>
section of <em>Mathematical Definitions, Notations and Proofs</em>.</p>

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
