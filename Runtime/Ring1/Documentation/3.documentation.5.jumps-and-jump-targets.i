  <h3>Jumps and Jump Targets</h3>
  <p><em>Arcadia.Ring1</em> provides non-local jumps by allowing to save and restore the C program state.</p>

  <p><code>
  typedef <my-mv>implementation detail</my-mv> Arcadia_JumpTarget;
  </code></p>
  <p>is a type of which its values are stack allocated.</p>

  <p><code>
  void Arcadia_Thread_pushJumpTarget(Arcadia_Thread* thread, Arcadia_JumpTarget* jumpTarget);
  </code></p>
  <p>
  Push a jump target on top of the jump target stack.
  Initially, the jump target stack is empty.
  </p>

  <p><code>
  void Arcadia_Thread_popJumpTarget(Arcadia_Thread* thread);
  </code></p>
  <p>
  Pop the jump target on top of the jump target stack.
  The jump target stack must not be empty.
  </p>

  <p><code>
  void Arcadia_jump(Arcadia_Thread* thread);
  </code></p>
  <p>
  Jump to the jump target on top of the jump target stack.
  The jump target stack must not be empty.
  </p>

  <p><code>
  #define Arcadia_JumpTarget_save(jumpTarget) <my-mv>implementation detail</my-mv>
  </code></p>
  <p>Macro to save the current C program state into a jump target.</p>

  <p>The canonical way of using <code>Arcadia_Thread_pushJumpTarget</code>, <code>Arcadia_Thread_popJumpTarget</code>, and
  <code>Arcadia_Thread_jump</code> is the following.</p>
  <p><code>
  Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);<br>
  if (Arcadia_JumpTarget_save(thread, &jumpTarget)) {<br>
    &nbsp;<my-mv>(1)</my-mv><br>
    &nbsp;Arcadia_Thread_popJumpTarget(thread);<my-mv>(2)</my-mv><br>
  } else {<br>
    &nbsp;Arcadia_Thread_popJumpTarget(thread);<my-mv>(3)</my-mv><br>
    &nbsp;<my-mv>(4)</my-mv><br>
  }<br>
  <my-mv>(5)</my-mv>
  </code></p>
  <p>
  The    if-branch is always entered at first as the <code>Arcadia_JumpTarget_save()</code>       at
  its first invocation returns logically true. If the program at <my-mv>(1)</my-mv> does   not
  jump to the jump target (using <code>Arcadia_Thread_jump()</code>),  then <code>Arcadia_Thread_popJumpTarget();<my-mv>
  (2)</my-mv></code> and <my-mv>(5)</my-mv> are executed. If the program at <my-mv>(1)</my-mv>
  jumps to the jump target, then <code>Arcadia_Thread_JumpTarget_save()</code> is re-evaluated and  returns
  logically false and <code>Arcadia_Thread_popJumpTarget();<my-mv>(2)</my-mv></code> and <my-mv>(4)</my-mv>
  is executed.
  </p>

  <p>One usage example is the implementation of the <code>Arcadia_safeExecute</code> function.</p>
  <p><code>
  void Arcadia_safeExecute(Arcadia_Thread* thread, void (*f)(Arcadia_Thread*)) {<br>
  &nbsp;Arcadia_JumpTarget jumpTarget;<br>
  &nbsp;Arcadia_Thread_pushJumpTarget(thread, &jumpTarget);<br>
  &nbsp;if (Arcadia_Thread_JumpTarget_save(thread, &jumpTarget)) {<br>
  &nbsp;&nbsp;f();<br>
  &nbsp;&nbsp;Arcadia_Thread_popJumpTarget(thread);<br>
  &nbsp;}<br>
  }
  </code></p>
