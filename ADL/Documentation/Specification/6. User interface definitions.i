      <h2 id="user-interface">6 User interface definitions</h2>
      <p>
      The user-interface definitions describe widgets and the panels that contain
      them. The widget definitions share a rectangle, described by the entries
      <code>left</code> and <code>bottom</code>, the coordinates of the lower-left
      corner in canvas coordinates, and by the entries <code>width</code> and
      <code>height</code>. All five widget types are read by a single reader, and each
      of them validates its own schema.
      </p>
      <p>
      The alignment entries <code>horizontalAlignment</code>,
      <code>verticalAlignment</code>, <code>mainAlignment</code>, and
      <code>crossAlignment</code> are ordinary strings. The current implementation
      stores the value as written and does not constrain its literal set, so the
      accepted values are an engine concern rather than a language constraint.
      </p>

      <h3>UI.Button</h3>
      <p>Type name: <code>"UI.Button"</code>. A rectangular user-interface button.</p>
      <pre>
"type"                     : "UI.Button"
"name"                     : &lt;string&gt;
"left"                     : &lt;number&gt;   // Integer32
"bottom"                   : &lt;number&gt;   // Integer32
"width"                    : &lt;number&gt;   // Natural32
"height"                   : &lt;number&gt;   // Natural32
"text"                     : &lt;string&gt;   // the button label
"horizontalAlignment"      : &lt;string&gt;   // optional, defaults to "Center"
"verticalAlignment"        : &lt;string&gt;   // optional, defaults to "Center"
"backgroundColor"          : &lt;string&gt;   // optional, reference to a Color definition
"hoveredBackgroundColor"   : &lt;string&gt;   // optional, reference to a Color definition
"pressedBackgroundColor"   : &lt;string&gt;   // optional, reference to a Color definition
"disabledBackgroundColor"  : &lt;string&gt;   // optional, reference to a Color definition
"focusedBackgroundColor"   : &lt;string&gt;   // optional, reference to a Color definition
"backgroundRed"            : &lt;number&gt;   // optional, Natural8, defaults to 56
"backgroundGreen"          : &lt;number&gt;   // optional, Natural8, defaults to 120
"backgroundBlue"           : &lt;number&gt;   // optional, Natural8, defaults to 240
"pixelSize"                : &lt;number&gt;   // optional, Natural32, defaults to 24
"borderWidth"              : &lt;number&gt;   // optional, Natural32, defaults to 0
"borderRed"                : &lt;number&gt;   // optional, Natural8, defaults to 87
"borderGreen"              : &lt;number&gt;   // optional, Natural8, defaults to 104
"borderBlue"               : &lt;number&gt;   // optional, Natural8, defaults to 118
      </pre>
      <p>
      The <code>text</code> entry is the label associated with the button. An omitted
      alignment entry becomes <code>"Center"</code>. Each of the five
      <code>*BackgroundColor</code> entries names a <code>"Color"</code> definition;
      an omitted <code>hoveredBackgroundColor</code>, <code>pressedBackgroundColor</code>,
      <code>disabledBackgroundColor</code>, or <code>focusedBackgroundColor</code>
      falls back to <code>backgroundColor</code>, which may itself be omitted. While
      linking, every one of the five color references that is present is resolved and
      must be a <code>"Color"</code> definition.
      </p>
      <p>
      The <code>backgroundRed</code>, <code>backgroundGreen</code>, and
      <code>backgroundBlue</code> entries are the background color as 8-bit RGB
      components. <code>pixelSize</code> is the text size in pixels,
      <code>borderWidth</code> the border thickness in pixels, and
      <code>borderRed</code>, <code>borderGreen</code>, <code>borderBlue</code> the
      border color as 8-bit RGB components.
      </p>

      <h3>UI.Label</h3>
      <p>Type name: <code>"UI.Label"</code>. A user-interface text label.</p>
      <pre>
"type"                : "UI.Label"
"name"                : &lt;string&gt;
"left"                : &lt;number&gt;   // Integer32
"bottom"              : &lt;number&gt;   // Integer32
"width"               : &lt;number&gt;   // Natural32
"height"              : &lt;number&gt;   // Natural32
"text"                : &lt;string&gt;   // the label text
"horizontalAlignment" : &lt;string&gt;
"verticalAlignment"   : &lt;string&gt;
"red"                 : &lt;number&gt;   // Natural8, the red component
"green"               : &lt;number&gt;   // Natural8, the green component
"blue"                : &lt;number&gt;   // Natural8, the blue component
"alpha"               : &lt;number&gt;   // Natural8, the alpha component
"pixelSize"           : &lt;number&gt;   // Natural32
      </pre>
      <p>
      The <code>red</code>, <code>green</code>, <code>blue</code>, and
      <code>alpha</code> entries describe the text color as 8-bit components.
      Unlike <code>"UI.Button"</code>, no entry of this definition is optional.
      </p>

      <h3>UI.StackPanel</h3>
      <p>Type name: <code>"UI.StackPanel"</code>. A panel that stacks its children along one axis.</p>
      <pre>
"type"                   : "UI.StackPanel"
"name"                   : &lt;string&gt;
"left"                   : &lt;number&gt;   // Integer32
"bottom"                 : &lt;number&gt;   // Integer32
"width"                  : &lt;number&gt;   // Natural32
"height"                 : &lt;number&gt;   // Natural32
"vertical"               : &lt;string&gt;   // Boolean
"gap"                    : &lt;number&gt;   // Integer32
"paddingLeft"            : &lt;number&gt;   // Integer32
"paddingBottom"          : &lt;number&gt;   // Integer32
"paddingRight"           : &lt;number&gt;   // Integer32
"paddingTop"             : &lt;number&gt;   // Integer32
"mainAlignment"          : &lt;string&gt;
"crossAlignment"         : &lt;string&gt;
"children"               : &lt;list of strings&gt;   // references to user-interface definitions
"backgroundModel"        : &lt;string&gt;   // optional, reference to a Model definition
"backgroundBorderWidth"  : &lt;number&gt;   // optional, Natural32, defaults to 0
      </pre>
      <p>
      The <code>vertical</code> entry selects the stack axis. Each element of the
      <code>children</code> list names a child definition; the children are added in
      list order. While linking, the <code>backgroundModel</code> reference, when
      present, is resolved.
      </p>

      <h3>UI.SegmentedChoice</h3>
      <p>Type name: <code>"UI.SegmentedChoice"</code>. A user-interface control that offers a choice among labeled segments.</p>
      <pre>
"type"           : "UI.SegmentedChoice"
"name"           : &lt;string&gt;
"left"           : &lt;number&gt;   // Integer32
"bottom"         : &lt;number&gt;   // Integer32
"width"          : &lt;number&gt;   // Natural32
"height"         : &lt;number&gt;   // Natural32
"selectedIndex"  : &lt;number&gt;   // Natural32
"labels"         : &lt;list of strings&gt;   // the segment labels
"pixelSize"      : &lt;number&gt;   // optional, Natural32, defaults to 24
"borderWidth"    : &lt;number&gt;   // optional, Natural32, defaults to 0
"borderRed"      : &lt;number&gt;   // optional, Natural8, defaults to 87
"borderGreen"    : &lt;number&gt;   // optional, Natural8, defaults to 104
"borderBlue"     : &lt;number&gt;   // optional, Natural8, defaults to 118
      </pre>
      <p>
      The <code>labels</code> list holds the text of each segment in order; the
      <code>selectedIndex</code> entry selects one of them. The <code>labels</code>
      strings are segment text, not references.
      </p>

      <h3>UI.Slider</h3>
      <p>Type name: <code>"UI.Slider"</code>. A user-interface control that selects a value within a range.</p>
      <pre>
"type"    : "UI.Slider"
"name"    : &lt;string&gt;
"left"    : &lt;number&gt;   // Integer32
"bottom"  : &lt;number&gt;   // Integer32
"width"   : &lt;number&gt;   // Natural32
"height"  : &lt;number&gt;   // Natural32
"minimum" : &lt;number&gt;   // Real64
"maximum" : &lt;number&gt;   // Real64
"value"   : &lt;number&gt;   // Real64
"step"    : &lt;number&gt;   // Real64
      </pre>
      <p>
      The four range entries describe the selection interval and the increment the
      control snaps to. No entry of this definition is optional.
      </p>

      <h3>UI.Toggle</h3>
      <p>Type name: <code>"UI.Toggle"</code>. A user-interface control that is either on or off.</p>
      <pre>
"type"    : "UI.Toggle"
"name"    : &lt;string&gt;
"left"    : &lt;number&gt;   // Integer32
"bottom"  : &lt;number&gt;   // Integer32
"width"   : &lt;number&gt;   // Natural32
"height"  : &lt;number&gt;   // Natural32
"checked" : &lt;string&gt;   // Boolean
      </pre>

      <h3>Definitions without a reader</h3>
      <p>
      The <code>Arcadia.ADL</code> module defines three further user-interface
      definition types, <code>"Canvas"</code>, <code>"Container"</code>, and
      <code>"ScrollPanel"</code>, that share the container interface
      <code>"UI.StackPanel"</code> implements. No reader is registered for them, so an
      asset cannot carry them as its <code>type</code> entry; they occur only in the C
      interface of the module. This document does not specify their entries.
      </p>
