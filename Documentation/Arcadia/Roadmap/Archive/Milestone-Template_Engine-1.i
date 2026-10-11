<h2>Milestone "Template Engine" 1: Template Engine: Ampersand- and Include-directives</h2>
<p>Percentage done: 100&percnt;, Due date: Week 2 September 2024</p>

<p>
The <em>Template Engine</em> is a tool which takes the filename of a (source) file and the filename of a (target) file.
It will <i>parse</i> the source file to create the target file.
<i>parsing</i> means that the contents of the source file are written to the target file unlesss directives are encountered.
The directives are not written to the target file. Instead, they are interpreted. There are two kinds of directives:
</p>
<ul>
  <li><code>@@@@</code> directive results in <code>@@</code> in being written to the target file.</li>
  <li><code>@@include(&lt;filename&gt;)</code> directive *parses* the file specified by <code>&lt;filename&gt;</code>.
</ul>
<p>
Recursive include directives yield an error.
The parser must support UTF-8-NO-BOM encoding.
</p>
