<h2>Milestone "Website" 2: Tooling for building the website | Tools.TemplateEngine</h2>
<p>Percentage done: 100&percnt;, Due date: Week 2 September 2024</p>

<p>
The source code of the website consists of multiple <code>.t</code> files.
The tool <i>Tools.TemplateEngine</i> reads <code>.t</code> files and creates the output <code>.html</code> files.
</p>

<p>
The tool works as follows: It receives the filename of one <code>.t</code> file and the filename of the target <code>.html</code> file.
It will <i>parse</i> the <code>.t</code> file to create the target <code>.html</code> file.
<i>parsing</i> means that the contents of the <code>.t</code> file are written to the target file unlesss directives are encountered.
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
