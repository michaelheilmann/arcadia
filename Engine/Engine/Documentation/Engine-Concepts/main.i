@{include("./disclaimer.i")}

<h2>I Introduction</h2>

  <p>
  This is the documentation for Michael  Heilmann's Arcadia Engine.
  Arcadia Engine is the module upon which a game engine is based on.
  </p>

  <h2>Capabilities and Concepts</h2>
  <p>This section is an introduction to capabilities and concepts of the engine.</p>

  <h3>Nodes and node traversals</h3>
  The engine is built around a unified node-based architecture. Every element of a game - materials, meshes, audio sources, animations, physics components, and even gameplay logic such as triggers or state machines - is represented as a node.
  The engine operates by visiting these nodes. Dedicated visitors handle specific concerns: one visitor renders visual content, another one emits audio, yet another one updates physics, and others evaluate gameplay logic or AI.
  Each visitor interprets and transforms nodes according to its domain. A game, therefore, is a structured collection of interconnected nodes loaded from a declarative description. During runtime, the engine continuously traverses
  this graph, with its various visitors updating, rendering, and evolving the state of the game world frame by frame.</p>

  <h3>Retained State</h3>
  <p>When evaluated, nodes update a retained state representation of their audio-visual
  aspects in the currently selected video and audio backends. Examples for a backend
  is for example the OpenGL backend and the OpenAL backend. That is, the backends
  contain a fully representable presentation of the audio-visual rendering of the
  names at a particular point of a particular iteration. This allows to decouple
  the frequency of node iterations from the frequency of video and audio presentations
  of the retained state to the user. For example, nodes might update their graphical
  representations only 4 times per second whilst the retainted state is rendered
  at 120 frames per second.</p>

  <h3>Backend Context and Retained State Re-Creation</h3>
  <p>Notes are visited and update the retained state. The retained state is a
  backend-specific state. That is, the retained state is tied to a particular
  video APIs (OpenGL, Vulkan, Direct3D) and to particular audio APIs (OpenAL,
  Bass). When the engine - for example due to a user changing the settings -
  makes a substantial change to a current backend (e.g., switching from
  windowed to fullscreen mode) or switching the current backend altogether
  (e.g., from OpenGL to Vulkan), then the nodes ensure that their retained
  state is re-created.</p>


  <h3>Examples of Nodes</h3>
  <p>This section provides a growing collection of currently known nodes to illustrate the node concept in action.</p>


  <p><b>Model Nodes</b>:
  A model consists of a mesh node and a material node.
  Furthermore, it consists of a local space to world space transformation.
  Model nodes are static, that is, they can not move.
  In general, scenes are made up of model nodes.</p>

  <p><b>Mesh Nodes</b>:
  A mesh node consists of a list of vertices.
  A vertex is made up of a position in local space, a per-vertex ambient color and a per-vertex ambient color texture coordinate.
  Furthermore, a mesh provides a per-mesh ambient color.
  </p>

  <p><b>Material Node</b>:
  A material node consists of an ambient color texture.
  Furthermore, it has a flags which determines the source for the per-fragment ambient color. This flag can assume three values:
  </p>
  <ul>
    <li>the per-mesh ambient color is used</li>
    <li>the per-vertex ambient color is used</li>
    <li>the ambient color texture is used
        In that case, the per-vertex ambient color texture coordintes are used for texture mapping.</li>
  </ul>
  <p>In usual engine scenarios, only the last flag, using the ambient texture color, is relevant.</p>

  <p><b>Texture Node</b>:
  A texture node consists of a pixel buffer node.</p>

  <p><b>Pixel Buffer Node</b>:
  <i>To be done</i></p>

  <p><b>Viewport Node</b>:
  <i>To be done</i></p>
