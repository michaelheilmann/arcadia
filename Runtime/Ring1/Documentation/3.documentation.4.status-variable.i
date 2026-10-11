  <h3 id="status-variable">Status Variable</h3>
  <p><em>Arcadia.Ring1</em> provides by-thread status variable.</p>

  <p><code>
  Arcadia_Status
  Arcadia_Thread_getStatus
    (
      Arcadia_Thread* thread
    );
  </code></p>
  <p>gets the value of the status variable. The initial value of the status variable is <code>Arcadia_Status_Success</code>.</p>

  <p><code>
  void
  Arcadia_Thread_setStatus
    (
      Arcadia_Thread* thread,
      Arcadia_Status status
    );
  </code></p>
  <p>sets the value of the status variable to the value <code>status</code>.</p>
