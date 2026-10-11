<h2>Starting up and shutting down</h2>
<p>To use the services of Arcadia ARMS, a user must acquire an handle to Arcadia ARMS. The user acquires a handle by a succesfull call to <code>Arcadia_ARMS_Status Arcadia_ARMS_startup()</code>.
This function returns <code>Arcadia_ARMS_Status_Success</code> to indicate success and returns status code different from <code>Arcadia_ARMS_Status_Success</code> on failure.
The following table lists the possible values returned in case of failure</p>
<table>
  <tr><td>Value                                    </td><td>Description               </td></tr>
  <tr><td><code>Arcadia_ARMS_Status_AllocationFailed</code></td><td>an allocation failed      </td></tr>
  <tr><td><code>Arcadia_ARMS_Status_OperationInvalid</code></td><td>there are too many handles</td></tr>
</table>
<p>A user may acquire more than one handle.</p>
<p>
If the user no longer requires the services of Arcadia ARMS, a user must relinquish the acquired handles.
The user relinquishes a handle by a successful call to <code>Arcadia_ARMS_Status Arms_shutdown()</code>.
This function returns <code>Arcadia_ARMS_Status_Success</code> to indicate success and returns status code different from <code>Arcadia_ARMS_Status_Success</code> on failure.
The following table lists the possible values returned in case of failure</p>
<table>
  <tr><td>Value                                    </td><td>Description               </td></tr>
  <tr><td><code>Arcadia_ARMS_Status_OperationInvalid</code></td><td>if there are objects in \(U\). This is usually the result of illicit usage of Arcadia ARMS.</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_OperationInvalid</code></td><td>if the last handle was already relinquished. This is usually the result of illicit usage of Arcadia ARMS</td></tr>
</table>

<p><b>Warning:</b>
Using the services of Arcadia ARMS without a handle is undefined behavior.
</p>
