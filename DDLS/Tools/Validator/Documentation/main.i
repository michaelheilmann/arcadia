<main>
<div class="left-column"></div>
<div class="center-column">
  <h1>Michael Heilmann's Arcadia Template Engine</h1>
  <p>
  This is the documentation for Michael Heilmann's Arcadia Template Engine.
  The Template Engine is a template engine primarily used to generate websites.
  </p>

  <h2>Invoking the Template Engine from the command line</h2>
  <p>
  The Template Engine can be invoked by the following command (assuming the executable <code>Arcadia.TemplateEngine</code> is in your path).
  </p>
  <p><code>
  Arcadia.TemplateEngine <my-mv>arguments</my-mv>
  </code></p>

  <p><code><my-mv>arguments</my-mv></code></p>
  <table>
    <tr><td><code>--source=<my-mv>source file</my-mv></code></td>          <td><my-mv>source file</my-mv> is the path to the source file (relative to the working directory)</td></tr>
    <tr><td><code>--target=<my-mv>target file</my-mv></code></td>          <td><my-mv>target file</my-mv> is the path to the target file (relative to the working directory)</td></tr>
    <tr><td><code>--environment=<my-mv>environment file</my-mv></code></td><td><my-mv>environment file</my-mv> is the path to the environment file (relative to the working directory).</td></tr>
  </table>

  <p>
  The <my-mv>source file</my-mv> is an UTF-8 encoded text file which is an input to this template engine invocation.
  The contents of this file are evaluated by the template engine and the evaluation result is written to the <my-mv>target file</my-mv>.
  </p>

  <p>
  The <my-mv>target file</my-mv> is an UTF-8 encoded text file which is an output of this template engine invocation.
  If the file exists and is a regular file, its contents are overwritten.
  If the file exists and is not a regular file or the path to the file does not exist, then this template engine invocation fails.
  </p>

  <p>
  The <my-mv>environment file</my-mv> is an <a href="@{siteAddress}/specifications/data-definition-language">Data Definition Language</a> file
  which must consist of a single value of type <em>Map</em>. The values of the map element values of this map value must be of type <em>String</em>.
  These map values represent variables which can be used in the template directives (see <a href="@{siteAddress}/Arcadia/TemplateEngine/#TemplateLanguage">Template Language</a> of this template engine invocation.
  Note: Definitions in this environment file shadow definitions of default variables and default procedures.
  </p>

  <h2>The Template Language</h2>
  <p>
  A <em>program</em> of the <em>Template Language</em> consists of <em>texts</em> in which <em>directives</em> are embedded into.
  <em>text</em> is a sequence of UTF8 symbols excluding the <em>ampersand</em> symbol. An <em>directive</em> begins with an ampersand.
  The extend (in UTF8 symbols) of an <em>directive</em> is determined by the directive itself. <em>text</em> is written directly to
  the output. <em>directives</em> are not written to the output. Instead, they are <em>evaluated</em> and the evaluation result
  is written to the output. There are two major categories of directives, the <em>ampersand directive</em> and the <em>expression directives</em>.
  </p>

  <p><b>Remark for this version</b>:
  In future version of the Template Language, there will be directives (e.g., loop operators) working on <em>text</em>.
  Consequently, the statement that <em>text</em> is written directly to the output will be revised.
  </p>

  <p><b>Remark for this version</b>:
  There will be a new category of directovies in future versions of the Template Language called <em>statement directives</em>.
  They will allow for defining new variables and (bounded) loops over text and directives.
  Consequently, the statement that there are two major categories of directives will be revised.
  </p>

  <h3>Ampersand Directive</h3>
  <p>The <em>ampersand directive</em> is of the form <code>@@@@</code> and is used output a single ampersand.</p>

  <h3>Expression Directive</h3>
  <p>Expression directives start with an ampersand <code>@@</code> followed by curly bracket <code>{</code> followed
  by a name. Optionally, the name is followed by an argument list which is opened by a left parenthesis <code>(</code>
  and closed by a right parenthesis <code>)</code>. The parentheses enclosed zero or more literals separated by commas
  <code>,</code>. Expressions without an argument list are <em>variable load</em> expression, expressions with an argument
  list are <em>invocation expressions</em>.
  </p>

  <p>
  <em>Variable load</em> expression search first the environment defined by the environment file and then the default environment for definitions of the name.
  If it discovers are a variable definition, it will emit the value of the variable to the output.
  The processing stops with an error if a definition for a procedure was found or no definition was found at all.
  </p>

  <p>
  <em>Invocation expression</em> search first the environment defined by the environment file and then the default environment for definitions of the name.
  If it discovers a procedure definition, it will invoke that procedure with the specified arguments and write the return value of the procedure to the output.
  The processing stops with an error if a definition for a variable procedure was found or no definition was found at all.
  </p>

  <p><b>Remark for this version</b>:
  Currently, it is not possible to specify procedures.
  Consequently, all procedures available are defined in the default environment.
  However, if a variable is defined in the environment file with a name used by a procedure in the default environment,
  then an invocation expression of that name will discover the variable and not the procedure causing the processing to stop with an error.
  </p>

  The following <em>invocation expression directives</em> are known:
  </p>
  <table class="invocation-directives">
    <tr>
      <th>Name</th>
      <th>Description</th>
    </tr>
    <tr>
      <td>
      include
      </td>
      <td>
      Defined in the default environment.
      Expects a string as its single argument which denotes the path to a file relative to the working directory.
      The file contents are processed by and the result is written to the output.
      Recursive includes (direct or indirect) are an error.
      </td>
    </tr>
    <tr>
      <td>
      <code>dayOfMonth</code>
      </td>
      <td>
      Defined in the default environment.
      Expects zero arguments.
      Outputs the day of the month of "now", that is, an integer in the interval [1,31] without leading zeroes.
      </td>
    </tr>
    <tr>
      <td>
      <code>month</code>
      </td>
      <td>
      Defined in the default environment.
      Expects zero arguments.
      Outputs the month of the year of "now", that is, an integer in the interval [1,12] without leading zeroes.
      </td>
    </tr>
      <td>
      <code>monthName</code>
      </td>
      <td>
      Defined in the default environment.
      Expects zero arguments.
      Outputs the English name of the month of the year of "now" (e.g., "January" or "October" without the quotes).
      </td>
    </tr>
    <tr>
      <td>
      <code>year</code>
      </td>
      <td>
      Defined in the default environ,ent.
      Expects zero arguments.
      Outputs the year of "now" (e.g., 2025).
      </td>
    </tr>
  </table>

</div>
<div class="right-column"></div>
</main>
