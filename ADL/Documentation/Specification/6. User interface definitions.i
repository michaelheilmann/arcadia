      <h2 id="user-interface">6 User interface definitions</h2>
      <p>
      The user-interface definitions describe widgets and the panels that contain
      them. The widget definitions share a rectangle, described by the entries
      <code>left</code> and <code>bottom</code>, the coordinates of the lower-left
      corner in canvas coordinates, and by the entries <code>width</code> and
      <code>height</code>. Each registered user-interface type is validated
      against its own schema before construction.
      </p>
      <p>
      The alignment entries <code>horizontalAlignment</code>,
      <code>verticalAlignment</code>, <code>mainAlignment</code>, and
      <code>crossAlignment</code> are ordinary strings. The current implementation
      stores the value as written for the legacy definitions. The new
      <code>UI.Panel</code> definition validates its layout alignment values.
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
"backgroundColor"          : &lt;string or map&gt; // required, named reference or inline Color
"hoveredBackgroundColor"   : &lt;string or map&gt; // optional, named reference or inline Color
"pressedBackgroundColor"   : &lt;string or map&gt; // optional, named reference or inline Color
"disabledBackgroundColor"  : &lt;string or map&gt; // optional, named reference or inline Color
"focusedBackgroundColor"   : &lt;string or map&gt; // optional, named reference or inline Color
"pixelSize"                : &lt;number&gt;   // optional, Natural32, defaults to 24
"borderWidth"              : &lt;number&gt;   // optional, Natural32, defaults to 0
"borderRed"                : &lt;number&gt;   // optional, Natural8, defaults to 87
"borderGreen"              : &lt;number&gt;   // optional, Natural8, defaults to 104
"borderBlue"               : &lt;number&gt;   // optional, Natural8, defaults to 118
      </pre>
      <p>
      The <code>text</code> entry is the label associated with the button. An omitted
      alignment entry becomes <code>"Center"</code>. The <code>backgroundColor</code>
      entry is required. Omitting it is a semantical error during reading; no
      default Color is created. Each of the five color entries accepts either
      a String reference to a named <code>"Color"</code> or an inline
      <code>"Color"</code> map without <code>name</code>, as specified in
      section 3. String references resolve and type-check during linking.
      </p>
      <p>
      The four state color entries are optional. An omitted
      <code>hoveredBackgroundColor</code>, <code>pressedBackgroundColor</code>,
      <code>disabledBackgroundColor</code>, or <code>focusedBackgroundColor</code>
      uses the same logical ADL Color definition as <code>backgroundColor</code>,
      whether named or inline; no additional definition occurrence is created.
      Consumers may still copy or convert its component values. Separately written
      inline Colors, even with identical values, denote distinct occurrences.
      Inline Colors contain no references of their own and therefore need no
      further reference resolution.
      </p>
      <p>Valid example (one complete ADL asset):</p>
      <pre>
{
  type: "UI.Button", name: "Buttons.Start",
  left: 0, bottom: 0, width: 100, height: 32, text: "Start",
  backgroundColor: { type: "Color", red: 255, green: 0, blue: 0 },
  hoveredBackgroundColor: { type: "Color", red: 200, green: 0, blue: 0 }
}
      </pre>
      <p>
      Each inline Color in this example is a distinct occurrence. The String
      reference form, such as <code>backgroundColor: "Colors.Red"</code>, also
      remains valid when <code>Colors.Red</code> is a separately loaded named
      Color. A map with a missing <code>type</code>, a nested <code>name</code>,
      missing RGB components, or a non-Color type is invalid.
      </p>
      <p>
      The former <code>backgroundRed</code>, <code>backgroundGreen</code>, and
      <code>backgroundBlue</code> entries are no longer permitted in a
      <code>UI.Button</code> definition. An occurrence of any of these entries
      is a semantical error during reading, even if <code>backgroundColor</code>
      is also present. This is a deliberate backward-incompatible change;
      existing assets using these entries must be migrated.
      </p>
      <p>
      <code>pixelSize</code> is the text size in pixels,
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

      <h3>UI.Panel</h3>
      <p>
      Type name: <code>"UI.Panel"</code>. A container that selects its layout
      algorithm independently of the child list. The current implementation
      supports a nested <code>UI.StackLayout</code> map; other layout types are
      rejected. The layout is initially measured and arranged after the children
      have been instantiated. Subsequent invalidation and input routing will be
      added as part of the UI layout migration.
      </p>
      <pre>
"type"                   : "UI.Panel"
"name"                   : &lt;string&gt;
"left"                   : &lt;number&gt; // Integer32
"bottom"                 : &lt;number&gt; // Integer32
"width"                  : &lt;number&gt; // Natural32
"height"                 : &lt;number&gt; // Natural32
"sizePolicy"             : "Content" | "Fixed" // optional, default Content
"children"               : &lt;list of strings&gt; // ordered definition references
"backgroundModel"        : &lt;string&gt; // optional ADL Model definition name
"backgroundBorderWidth"  : &lt;number&gt; // optional Natural32, default 0
"layout" : {
  "type"                 : "UI.StackLayout"
  "orientation"          : "Vertical" | "Horizontal"
  "gap"                  : &lt;number&gt; // optional Integer32, default 0, nonnegative
  "paddingLeft"          : &lt;number&gt; // optional Integer32, default 0, nonnegative
  "paddingBottom"        : &lt;number&gt; // optional Integer32, default 0, nonnegative
  "paddingRight"         : &lt;number&gt; // optional Integer32, default 0, nonnegative
  "paddingTop"           : &lt;number&gt; // optional Integer32, default 0, nonnegative
  "mainAlignment"        : &lt;string&gt; // optional, default "Center"
  "crossAlignment"       : &lt;string&gt; // optional, default "Center"
}
      </pre>
      <p>
      <code>sizePolicy: "Content"</code> measures the panel from its children
      and layout padding (the existing default). <code>sizePolicy: "Fixed"</code>
      instead uses the panel's explicit <code>width</code> and
      <code>height</code> as its desired size, while still measuring and arranging
      its children. This is a persistent sizing behavior, not a migration flag.
      It is useful for tabbed content areas whose dimensions must not change
      when children are added or removed.
      </p>
      <p>
      For <code>UI.Panel</code>, alignment must be one of
      <code>Start</code>, <code>Center</code>, <code>End</code>, or
      <code>Stretch</code>. A stack layout does not own its children.
      The existing <code>UI.StackPanel</code> definition remains supported.
      </p>
      <p>
      <code>backgroundModel</code> optionally refers to a model definition used
      to render the panel background before its children. The optional
      <code>backgroundBorderWidth</code> controls the model-backed frame's border
      (as for <code>UI.StackPanel</code>); it defaults to zero. A missing or
      non-model background reference is a semantic error during node creation.
      </p>
      <p>
      An optional <code>childLayouts</code> list overrides margins and cross-axis
      alignment for selected child names. Names must occur exactly once in
      <code>children</code> and may not be repeated in <code>childLayouts</code>.
      Unspecified margins default to zero; an unspecified cross-axis alignment
      inherits the layout's <code>crossAlignment</code>. Negative margins,
      unknown names, and unknown properties are invalid.
      </p>
      <pre>
"childLayouts" : [ // optional
  { "name" : "Options.Toggle.VSync", "marginLeft" : 384 },
  { "name" : "Options.Button.Apply", "marginRight" : 120, "crossAlignment" : "End" }
]
      </pre>
      <p>
      Per-child fields are <code>marginLeft</code>, <code>marginBottom</code>,
      <code>marginRight</code>, <code>marginTop</code> (optional nonnegative
      Integer32 values), and <code>crossAlignment</code> (an optional alignment
      string). Runtime changes to child collections use the same container slot
      mechanism, while these ADL overrides apply at initial construction.
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
