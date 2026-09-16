// Arcadia
// Copyright (C) 2024-2026 Michael Heilmann
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
// details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#if !defined(ARCADIA_MEDIA_DSP_GRAPH_H_INCLUDED)
#define ARCADIA_MEDIA_DSP_GRAPH_H_INCLUDED

#if !defined(ARCADIA_MEDIA_PRIVATE) || 1 != ARCADIA_MEDIA_PRIVATE
  #error("do not include directly, include `Arcadia/Media/Include.h` instead")
#endif
#include "Arcadia/Collections/Include.h"
#include "Arcadia/Media/DSP.h"

/// @brief A DSP graph owner with an explicit render output.
///
/// This first-stage graph owns a list of DSP nodes for GC/rooting purposes and
/// renders by delegating to its explicit output node. It is itself a DSP, so it
/// can be rendered directly or passed to Arcadia_Media_SampleBuffer_fill.
/// Connection-based DAG validation and traversal can be added on top of this
/// owner model without changing callers that render the graph.
Arcadia_declareObjectType(u8"Arcadia.Media.DSP.Graph", Arcadia_Media_DSP_Graph,
                          u8"Arcadia.Media.DSP");

struct Arcadia_Media_DSP_GraphDispatch {
  Arcadia_Media_DSPDispatch parent;
};

struct Arcadia_Media_DSP_Graph {
  Arcadia_Media_DSP parent;
  Arcadia_List* nodes;
  Arcadia_Media_DSP* output;
};

/// @brief Create an empty DSP graph.
/// @param thread A pointer to this thread.
/// @return The created graph.
Arcadia_Media_DSP_Graph*
Arcadia_Media_DSP_Graph_create
  (
    Arcadia_Thread* thread
  );

/// @brief Add a node to a DSP graph's owned node list.
/// @param thread A pointer to this thread.
/// @param self A pointer to this graph.
/// @param node The node to keep reachable through the graph.
void
Arcadia_Media_DSP_Graph_addNode
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Media_DSP* node
  );

/// @brief Set the node rendered as this graph's output.
/// @param thread A pointer to this thread.
/// @param self A pointer to this graph.
/// @param output The output DSP node. It is kept reachable through the graph.
void
Arcadia_Media_DSP_Graph_setOutput
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Media_DSP* output
  );

/// @brief Reset all nodes owned by this graph exactly once.
///
/// Reset restores render state while preserving graph topology and node
/// configuration. It does not recurse through node input pointers; instead, the
/// graph resets its owned node list in insertion order.
void
Arcadia_Media_DSP_Graph_reset
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self
  );

/// @brief Reseed seedable nodes and reset all nodes owned by this graph.
///
/// Each owned node receives a deterministic seed derived from @p seed and its
/// insertion index, so multiple random nodes in one graph do not share identical
/// streams. Non-seedable nodes ignore reseeding but are still reset.
void
Arcadia_Media_DSP_Graph_reseed
  (
    Arcadia_Thread* thread,
    Arcadia_Media_DSP_Graph* self,
    Arcadia_Natural32Value seed
  );

#endif // ARCADIA_MEDIA_DSP_GRAPH_H_INCLUDED
