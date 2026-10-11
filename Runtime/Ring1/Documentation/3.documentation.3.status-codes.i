  <h3 id="status-codes">Status Codes</h3>
  <p>
  R provides a status type and symbolic constants for its values. That type denotes success or failure of program (or a part of it)
  as well as information on the reason for the failure.</p>

  <p><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_Status
  </code></p>
  <p>
  <code>Arcadia_Status</code> is a natural number type of a width of 32 Bit.</p>

  <p><code>
  #definer Arcadia_Status_Success <my-mv>implementation detail</my-mv>
  </code></p>
  <p>is the symbolic constant of the type <code>Arcadia_Status</code> has the value <code>((Arcadia_Status)0)</code>.
  It denotes success and is guaranteed to be defined to the value <code>0</code> of the underlaying natural number
  type of Arcadia_Status as well as to be the only symbolic constant starting with <code>Arcadia_Status_</code> having
  defined to that value.</p>

  <p>
  Symbolic constants of the form
  </p>
  <p><code>
  #definer Arcadia_Status_<my-mv>Name</my-mv> <my-mv>implementation detail</my-mv>
  </code></p>
  <p>where <my-mv>Name</my-mv> is not <code>Success</code> provide information on failure of a program (or a part of it).
  The following table denotes the value of the constant for the respective <my-mv>Name</my-mv>
  </p>
  <table>
  <tr><td><code><span>Name</span></code></td>                                                      <td>Value</td></tr>
  <tr><td><code id="Arcadia_Status_AccessDenied">AccessDenied</code></td>                          <td>An access was denied.</td></tr>
  <tr><td><code id="Arcadia_Status_AllocationFailed">AllocationFailed</code></td>                  <td>An allocation failed.</td></tr>

  <tr><td><code id="Arcadia_Status_ArgumentTypeInvalid">ArgumentTypeInvalid</code></td>            <td>The type of an argument is invalid.</td></tr>
  <tr><td><code id="Arcadia_Status_ArgumentValueInvalid">ArgumentValueInvalid</code></td>          <td>The value of an argument is invalid.</td></tr>

  <tr><td><code id="Arcadia_Status_ConversionFailed">ConversionFailed</code></td>                  <td>A conversion failed.</td></tr>

  <tr><td><code id="Arcadia_Status_DivisionByZero">DivisionByZero</code></td>                      <td>An divions by zero was detected.</td></tr>

  <tr><td><code id="Arcadia_Status_EncodingInvalid">EncodingInvalid</code></td>                    <td>An encoding is invalid.</td></tr>
  <tr><td><code id="Arcadia_Status_EnvironmentFailed">EnvironmentFailed</code></td>                <td>The environment failed.</td></tr>
  <tr><td><code id="Arcadia_Status_Exists">Exists</code></td>                                      <td>Something exists (but should not).</td></tr>

  <tr><td><code id="Arcadia_Status_Found">Found</code></td>                                        <td>Something was found.</td>

  <tr><td><code id="Arcadia_Status_Initialized">Initialized</code></td>                            <td>Something was initialized (but should not have been initialized).</td></tr>

  <tr><td><code id="Arcadia_Status_LexicalError">LexicalError</code></td>                          <td>A lexical error.</td></tr>

  <tr><td><code id="Arcadia_Status_NotExists">NotExists</code></td>                                <td>Something does not exist.</td></tr>
  <tr><td><code id="Arcadia_Status_NotFound">NotFound</code></td>                                  <td>Something was not found.</td>
  <tr><td><code id="Arcadia_Status_NumberOfArgumentsInvalid">NumberOfArgumentsInvalid</code></td>  <td>The number of arguments is invalid.</td></tr>

  <tr><td><code id="Arcadia_Status_OperationFailed">OperationFailed</code></td>                    <td>An operation failed.</td></tr>
  <tr><td><code id="Arcadia_Status_OperationInvalid">OperationInvalid</code></td>                  <td>An operation is invalid.</td></tr>

  <tr><td><code id="Arcadia_Status_SemanticalError">SemanticalError</code></td>                    <td>A semantical error.</td></tr>
  <tr><td><code id="Arcadia_Status_SyntacticalError">SyntacticalError</code></td>                  <td>A syntactical error.</td></tr>

  <tr><td><code id="Arcadia_Status_TestFailed">TestFailed</code></td>                              <td>A test failed. This is used by the built-in tests that ensure correctness.</td></tr>
  <tr><td><code id="Arcadia_Status_TypeExists">TypeExists</code></td>                              <td>A type exists (but should not).</td></tr>
  <tr><td><code id="Arcadia_Status_TypeNotExists">TypeNotExists</code></td>                        <td>A type does not exist (but should).</td></tr>

  <tr><td><code id="Arcadia_Status_Uninitialized">Uninitialized</code></td>                        <td>Something was uninitialized (but should have been initialized).</td></tr>

  </table>
