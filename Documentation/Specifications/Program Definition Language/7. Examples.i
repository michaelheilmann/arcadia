    <h2>Examples</h2>
    <pre><code>module Arcadia.Engine {
}

class Arcadia.Engine.Event extends Arcadia.Object {
  variable timestamp : Arcadia.Natural64

  constructor(variable timestamp : Arcadia.Natural64)
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