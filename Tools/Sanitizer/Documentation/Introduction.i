<section id="Introduction">
  <h2>Introduction</h2>
  <p>Use <code>--check</code> before committing to detect whitespace differences
  without modifying files. Use <code>--fix</code> to apply the supported
  normalization. Both modes accept a file or a directory; the path defaults
  to the current working directory.</p>

  <h3>Build</h3>
  <p>Configure and build Arcadia with the usual CMake workflow, then build
  the sanitizer executable and its regression tests:</p>
  <pre><code>cmake -S . -B build
cmake --build build --target Arcadia.Sanitizer.Tool
cmake --build build --target Arcadia.Sanitizer.Tests
ctest --test-dir build -R Sanitizer --output-on-failure</code></pre>
  <p>The executable location depends on the selected CMake generator and
  configuration. For Visual Studio or other multi-configuration generators,
  select the appropriate build/test configuration (for example, with
  <code>--config Debug</code> and <code>ctest -C Debug</code>).</p>

  <h3>Examples</h3>
  <pre><code>Arcadia.Sanitizer.Tool --check .
Arcadia.Sanitizer.Tool --check Runtime/FileSystem
Arcadia.Sanitizer.Tool --fix Runtime/FileSystem
Arcadia.Sanitizer.Tool --check CMakeLists.txt</code></pre>
  <p>Run the executable by its actual filesystem path, or place its directory
  on <code>PATH</code>. The examples assume it is available on <code>PATH</code>.
  After a fix, inspect the changes using <code>git diff</code> and
  <code>git diff --check</code>. Do not use <code>--fix</code> on files that
  cannot be restored if an interruption occurs.</p>

  <h3>Typical workflow</h3>
  <ol>
    <li>Run <code>--check</code> for the module being changed.</li>
    <li>If differences are expected, run <code>--fix</code> on that module.</li>
    <li>Review the changed files and run <code>--check</code> again.</li>
    <li>Run the native sanitizer CTest regression suite.</li>
  </ol>
</section>
