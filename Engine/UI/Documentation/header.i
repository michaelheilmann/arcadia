@{include("./../../../Documentation/Commons/header-common.i")}
<link rel='stylesheet' href='@{siteAddress}/assets/cxx-doc.css?v=1'>
<link rel='canonical' href='@{siteAddress}/Arcadia/Visuals/'>
<title>Michael Heilmann's Arcadia UI</title>
<style>
</style>
</head>
<body class="my-content-page-1">
  <header>
    <div>
    <a href="@{siteAddress}/">Home</a>
    </div>
  </header>
<main>
  <div class="left-column"></div>
  <div class="center-column">
  <h1>Michael Heilmann's Arcadia Visuals</h1>
  <p>
  This is the documentation for Michael  Heilmann's Arcadia UI.
  Arcadia UI provides a user interface (windows, buttons, text fields, etc.) for <em>Michael Heilmann's Arcadia Engine</em>.
  </p>

  <p>
  UI Basics:
  A user interface consists of a UI canvas node in which UI widgets are placed.
  The UI canvas node has a width and height, which may change, depending on the context in which the UI canvas is used.
  For example, a UI canvas might resize according to the application's window inner size.
  A UI canvas / its contained UI controls should react reasonably to these size changees.
  </p>
