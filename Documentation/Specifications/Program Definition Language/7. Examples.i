    <h2>Examples</h2>
    <pre><code>module Arcadia.Engine {
}

abstract class Arcadia.Engine.Event extends Arcadia.Object {
  variable timestamp : Arcadia.Natural64

  constructor(variable timestamp : Arcadia.Natural64) {
    storeField self timestamp timestamp
  }
}

class Arcadia.Engine.ConcreteEvent extends Arcadia.Engine.Event {
  constructor(variable timestamp : Arcadia.Natural64) {
    construct $0 Arcadia.Engine.Event self timestamp
  }
}

procedure makeEvent(variable timestamp : Arcadia.Natural64) : Arcadia.Engine.Event {
  construct $0 Arcadia.Engine.ConcreteEvent void timestamp
  return $0
}

enumeration Arcadia.Engine.Visuals.WindingMode {
  Clockwise = 0
  CounterClockwise = 1
}

procedure sum(variable x : Arcadia.Integer32, variable y : Arcadia.Integer32) : Arcadia.Integer32 {
  loadArgument $1 x
  loadArgument $2 y
  add $1 $1 $2
  return $1
 }</code></pre>
    <p><code>Event</code> is abstract even though it has no abstract methods. <code>ConcreteEvent</code> forwards initialization to the abstract parent on the same receiver, and <code>makeEvent</code> allocates the concrete class. The following separate procedure is invalid because it attempts to instantiate <code>Event</code> directly:</p>
    <pre><code>procedure invalidEvent(variable timestamp : Arcadia.Natural64) : Arcadia.Engine.Event {
  construct $0 Arcadia.Engine.Event void timestamp
  return $0
}</code></pre>
    <h3>Executable Procedures and Methods</h3>
    <pre><code>module Demo

class Demo.Counter {
  variable value : Arcadia.Natural64
  constructor(variable initial : Arcadia.Natural64) {
    storeField self value initial
  }
  method read : Arcadia.Natural64 {
    loadField $0 self value
    return $0
  }
}

procedure initial : Arcadia.Natural64 {
  return 42
}

procedure run : Arcadia.Natural64 {
  call $0 initial
  construct $1 Demo.Counter void $0
  invoke $2 read $1
  return $2
}</code></pre>
    <p><code>run</code> calls a zero-argument procedure, allocates and initializes a counter, invokes its method, and returns 42. Instruction operands are adjacent; only declaration parameter lists use commas and parentheses.</p>
