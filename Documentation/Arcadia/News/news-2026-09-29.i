<div class="news-item">

  <div class="news-item-header">

    <h2>2026-09-29</h2>

  </div>

  <div class="news-item-body">
  <ul style="list-style-position: inside">
    <li>
    The former <em>Machine Interface Language</em> is now the <em>Arcadia Program Definition Language</em>, or
    <em>Arcadia PDL</em> for short. Its <a href="@{siteAddress}/specifications/program-definition-language/">specification</a>
    documents the grammar accepted by the current parser, which covers modules, procedures, classes with fields,
    constructors and methods, and enumerations. It replaces the earlier MIL specification.
    </li>
    <li>
    The separate MIL front end, compiler and MIL back end have been merged into a single compiler module, and the
    MIL compiler has been renamed PDLC. The <em>Arcadia PDLC</em> library and the <code>Arcadia.PDLC.CIL</code>
    command-line tool are now top-level modules, <code>PDLC</code> and <code>PDLC.CIL</code>, rather than living
    under <code>repository/</code>. The tool reads its own DDL configuration file and validates that file against a
    DDLS schema before it compiles.
    </li>
    <li>
    <em>Arcadia ADL</em> is now documented. The <a href="@{siteAddress}/Arcadia/ADL/Introduction/">Introduction</a>,
    <a href="@{siteAddress}/Arcadia/ADL/Concepts-and-Terms/">Concepts and Terms</a>,
    <a href="@{siteAddress}/Arcadia/ADL/Specification/">Specification</a>, and
    <a href="@{siteAddress}/Arcadia/ADL/Module-Interface/">Module Interface</a> documents describe the language,
    its concepts, its grammar, and the definitions the module provides.
    </li>
    <li>
    ADL assets gained declarative descriptions of materials and textures. A material declares its material type, its
    ambient color source, and its source and destination blend functions; a texture references a pixel buffer and
    declares its magnification and minification filters as well as its U and V address modes. The supporting
    enumerations are blend functions, texture filters, texture address modes, and ambient color sources.
    </li>
    <li>
    The new <em>Arcadia.Engine.Text</em> library renders styled text runs from a small markup language. It provides a
    markup scanner and a markup parser, markup text runs that carry text, glyph source, pixel size, color name,
    alignment and placement, and a text alignment enumeration. A run's glyph source may be a dynamic token, which the
    application resolves against live state.
    </li>
    <li>
    <em>Arcadia Engine</em> provides a font cache and an FPS counter, and its window and window backend abstractions
    gained operations for icons, title, position, size, fullscreen mode, vertical synchronization, and pixel capture.
    Two new examples demonstrate the result: <code>TextRendering</code> draws centered, styled text with a live frame
    rate counter, and <code>FontTexture</code> draws a model whose ambient color texture is a font atlas.
    </li>
    <li>
    Vertical synchronization is now configurable through the <code>visuals.verticalSynchronization</code>
    configuration key and is applied through <code>Arcadia_Engine_Visuals_Window_setVerticalSynchronization</code>
    in every window mode. The OpenGL 4 WGL and GLX backends apply the requested swap interval lazily during rendering,
    because the interval can only be adjusted while the GL context is current. A backend that cannot honor the
    request ignores it and leaves the driver default in effect.
    </li>
    <li>
    Pixels can now be captured from the back buffer. <code>Arcadia_Engine_Visuals_Window_capturePixels</code> and
    <code>Arcadia_Engine_Visuals_WindowBackend_capturePixels</code> return a new, canvas-sized pixel buffer, and
    <code>Arcadia_Engine_Visuals_FrameBufferResource_capturePixels</code> does the same for a frame buffer resource.
    A capture must happen between a successful <code>Window_beginRender</code> and its matching
    <code>Window_endRender</code>; because it reads the back buffer before presentation, it is unaffected by
    visibility, occlusion, and minimization.
    </li>
    <li>
    The new <code>Arcadia.DDLS.Validator</code> command-line tool validates a DDL file against a set of DDLS
    schemas. It accepts <code>--ddls=</code> arguments naming the schema files, a <code>--schema=</code> argument
    naming the schema to validate against, and a <code>--ddl=</code> argument naming the data definition file. It
    reports diagnostics to standard error and exits with a nonzero status on failure.
    </li>
    <li>
    The <em>Arcadia Ring 1</em> type system supports interface types. Interfaces are registered with
    <code>Arcadia_registerInterfaceType</code>, declared with <code>Arcadia_declareInterfaceType</code> and
    <code>Arcadia_defineInterfaceType</code>, and operations are invoked with
    <code>Arcadia_InterfaceCall</code> and <code>Arcadia_InterfaceCallWithReturn</code>. An object type implements
    an interface by registering a separate dispatch for every ancestor interface with
    <code>Arcadia_registerObjectTypeWithInterfaces</code>; registration is rejected if the implementation is not
    closed under the ancestors of the implemented interfaces. A new test module,
    <code>Arcadia.Ring1.Tests.InterfaceTypeTests</code>, covers the type relations, operation dispatch, dispatch
    inheritance under override, and the closure requirement.
    </li>
    <li>
    The documentation of <code>Arcadia_Object</code>, <code>Arcadia_ByteArray</code>,
    <code>Arcadia_ByteArrayBuilder</code>, <code>Arcadia_String</code>, and <code>Arcadia_StringBuilder</code> has
    moved from <em>Arcadia Ring 2</em> to <em>Arcadia Ring 1</em>, where these types are defined.
    </li>
  </ul>
  </div>

</div>
