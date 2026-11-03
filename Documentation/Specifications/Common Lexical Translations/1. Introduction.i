<h2 id="introduction" tabindex="-1">1 Introduction</h2>
@{include("./0. Abstract.i")}

<p>This specification is for readers defining or using a language's lexical grammar.
The productions use the concepts and notation in the
<a href="@{siteAddress}/specifications/mathematical-definitions-notations-and-proofs/index.html#context-free-grammars">Context-Free Grammars</a>
section of <em>Mathematical Definitions, Notations and Proofs</em>.
Read that section for production notation, repetition, optional elements and hexadecimal code-point literals.</p>

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

<p>To use this catalog, select the required categories from <a href="#productions">Section 2</a>,
then consult the consuming language's specification for its complete word grammar and refinements.
For example, the <a href="@{siteAddress}/specifications/data-definition-language/index.html#lexical-translation">Data Definition Language (DDL) lexical translation</a>
selects double-quoted strings and defines its own ASCII name profile. Those choices do not restrict other consumers.</p>
<p>The Unicode input alphabet does not mean that every category accepts every Unicode code point.
Each production states its own character ranges; the string rules distinguish Unicode scalar values
from the broader set of Unicode code points.</p>
