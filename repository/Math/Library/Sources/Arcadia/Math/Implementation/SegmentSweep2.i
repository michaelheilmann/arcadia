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

// Included twice from SegmentSweep2.c. No dynamic comparator or managed
// allocations are required: each segment owns an indexed AVL node, and a
// separate binary min-heap expires that node at its rightmost X coordinate.
#define SW_CAT1(a,b) a##b
#define SW_CAT(a,b) SW_CAT1(a,b)
#define SW_NAME(a) SW_CAT(a,SW_SUFFIX)
#define SW_NONE SIZE_MAX

typedef struct SW_NAME(SweepNode) {
  SW_REAL lowY;
  SW_REAL highY;
  SW_REAL maxX;
  SW_REAL subtreeHighY;
  size_t left;
  size_t right;
  int height;
} SW_NAME(SweepNode);

typedef struct SW_NAME(SweepStart) {
  SW_REAL x;
  SW_REAL y;
  size_t index;
} SW_NAME(SweepStart);

typedef struct SW_NAME(SweepState) {
  const SW_SEG* segments;
  SW_NAME(SweepNode)* nodes;
  size_t root;
  size_t* expiryHeap;
  size_t heapLength;
  SW_EVENT* events;
  size_t eventCount;
  size_t eventCapacity;
  Arcadia_Math_SegmentSweep2Status status;
} SW_NAME(SweepState);

static int
SW_NAME(compareStart)
  (
    const void* left,
    const void* right
  )
{
  const SW_NAME(SweepStart)* a = left;
  const SW_NAME(SweepStart)* b = right;
  if (a->x < b->x) return -1;
  if (a->x > b->x) return 1;
  if (a->y < b->y) return -1;
  if (a->y > b->y) return 1;
  return a->index < b->index ? -1 : a->index > b->index ? 1 : 0;
}

static int
SW_NAME(compareEvent)
  (
    const void* left,
    const void* right
  )
{
  const SW_EVENT* a = left;
  const SW_EVENT* b = right;
  if (a->intersection.a[0] < b->intersection.a[0]) return -1;
  if (a->intersection.a[0] > b->intersection.a[0]) return 1;
  if (a->intersection.a[1] < b->intersection.a[1]) return -1;
  if (a->intersection.a[1] > b->intersection.a[1]) return 1;
  if (a->firstIndex < b->firstIndex) return -1;
  if (a->firstIndex > b->firstIndex) return 1;
  return a->secondIndex < b->secondIndex ? -1 : a->secondIndex > b->secondIndex ? 1 : 0;
}

static int
SW_NAME(nodeHeight)
  (
    const SW_NAME(SweepState)* state,
    size_t index
  )
{
  return index == SW_NONE ? 0 : state->nodes[index].height;
}

static void
SW_NAME(updateNode)
  (
    SW_NAME(SweepState)* state,
    size_t index
  )
{
  SW_NAME(SweepNode)* node = state->nodes + index;
  const int lh = SW_NAME(nodeHeight)(state, node->left);
  const int rh = SW_NAME(nodeHeight)(state, node->right);
  node->height = (lh > rh ? lh : rh) + 1;
  node->subtreeHighY = node->highY;
  if (node->left != SW_NONE &&
      state->nodes[node->left].subtreeHighY > node->subtreeHighY)
    node->subtreeHighY = state->nodes[node->left].subtreeHighY;
  if (node->right != SW_NONE &&
      state->nodes[node->right].subtreeHighY > node->subtreeHighY)
    node->subtreeHighY = state->nodes[node->right].subtreeHighY;
}

static size_t
SW_NAME(rotateLeft)
  (
    SW_NAME(SweepState)* state,
    size_t index
  )
{
  const size_t top = state->nodes[index].right;
  state->nodes[index].right = state->nodes[top].left;
  state->nodes[top].left = index;
  SW_NAME(updateNode)(state, index);
  SW_NAME(updateNode)(state, top);
  return top;
}

static size_t
SW_NAME(rotateRight)
  (
    SW_NAME(SweepState)* state,
    size_t index
  )
{
  const size_t top = state->nodes[index].left;
  state->nodes[index].left = state->nodes[top].right;
  state->nodes[top].right = index;
  SW_NAME(updateNode)(state, index);
  SW_NAME(updateNode)(state, top);
  return top;
}

static size_t
SW_NAME(rebalance)
  (
    SW_NAME(SweepState)* state,
    size_t index
  )
{
  SW_NAME(updateNode)(state, index);
  const int balance = SW_NAME(nodeHeight)(state, state->nodes[index].left) -
                      SW_NAME(nodeHeight)(state, state->nodes[index].right);
  if (balance > 1) {
    const size_t child = state->nodes[index].left;
    if (SW_NAME(nodeHeight)(state, state->nodes[child].left) <
        SW_NAME(nodeHeight)(state, state->nodes[child].right))
      state->nodes[index].left = SW_NAME(rotateLeft)(state, child);
    return SW_NAME(rotateRight)(state, index);
  }
  if (balance < -1) {
    const size_t child = state->nodes[index].right;
    if (SW_NAME(nodeHeight)(state, state->nodes[child].right) <
        SW_NAME(nodeHeight)(state, state->nodes[child].left))
      state->nodes[index].right = SW_NAME(rotateRight)(state, child);
    return SW_NAME(rotateLeft)(state, index);
  }
  return index;
}

static int
SW_NAME(compareNode)
  (
    const SW_NAME(SweepState)* state,
    size_t a,
    size_t b
  )
{
  if (state->nodes[a].lowY < state->nodes[b].lowY) return -1;
  if (state->nodes[a].lowY > state->nodes[b].lowY) return 1;
  return a < b ? -1 : a > b ? 1 : 0;
}

static size_t
SW_NAME(insertNode)
  (
    SW_NAME(SweepState)* state,
    size_t root,
    size_t index
  )
{
  if (root == SW_NONE) return index;
  if (SW_NAME(compareNode)(state, index, root) < 0)
    state->nodes[root].left = SW_NAME(insertNode)(state, state->nodes[root].left, index);
  else
    state->nodes[root].right = SW_NAME(insertNode)(state, state->nodes[root].right, index);
  return SW_NAME(rebalance)(state, root);
}

// Remove the minimum node from root without altering its node identity.
static size_t
SW_NAME(detachMinimum)
  (
    SW_NAME(SweepState)* state,
    size_t root,
    size_t* minimum
  )
{
  if (state->nodes[root].left == SW_NONE) {
    *minimum = root;
    return state->nodes[root].right;
  }
  state->nodes[root].left = SW_NAME(detachMinimum)(state, state->nodes[root].left, minimum);
  return SW_NAME(rebalance)(state, root);
}

static size_t
SW_NAME(removeNode)
  (
    SW_NAME(SweepState)* state,
    size_t root,
    size_t index
  )
{
  if (root == SW_NONE) return root;
  const int comparison = SW_NAME(compareNode)(state, index, root);
  if (comparison < 0) {
    state->nodes[root].left = SW_NAME(removeNode)(state, state->nodes[root].left, index);
  } else if (comparison > 0) {
    state->nodes[root].right = SW_NAME(removeNode)(state, state->nodes[root].right, index);
  } else {
    const size_t left = state->nodes[root].left;
    const size_t right = state->nodes[root].right;
    if (left == SW_NONE) return right;
    if (right == SW_NONE) return left;
    size_t successor = SW_NONE;
    const size_t remainder = SW_NAME(detachMinimum)(state, right, &successor);
    state->nodes[successor].left = left;
    state->nodes[successor].right = remainder;
    root = successor;
  }
  return SW_NAME(rebalance)(state, root);
}

static int
SW_NAME(compareExpiry)
  (
    const SW_NAME(SweepState)* state,
    size_t a,
    size_t b
  )
{
  if (state->nodes[a].maxX < state->nodes[b].maxX) return -1;
  if (state->nodes[a].maxX > state->nodes[b].maxX) return 1;
  return a < b ? -1 : a > b ? 1 : 0;
}

static void
SW_NAME(pushExpiry)
  (
    SW_NAME(SweepState)* state,
    size_t index
  )
{
  size_t position = state->heapLength++;
  while (position) {
    const size_t parent = (position - 1) / 2;
    if (SW_NAME(compareExpiry)(state, state->expiryHeap[parent], index) <= 0) break;
    state->expiryHeap[position] = state->expiryHeap[parent];
    position = parent;
  }
  state->expiryHeap[position] = index;
}

static size_t
SW_NAME(popExpiry)
  (
    SW_NAME(SweepState)* state
  )
{
  const size_t first = state->expiryHeap[0];
  const size_t last = state->expiryHeap[--state->heapLength];
  size_t position = 0;
  while (2 * position + 1 < state->heapLength) {
    size_t child = 2 * position + 1;
    if (child + 1 < state->heapLength &&
        SW_NAME(compareExpiry)(state, state->expiryHeap[child + 1], state->expiryHeap[child]) < 0)
      ++child;
    if (SW_NAME(compareExpiry)(state, last, state->expiryHeap[child]) <= 0) break;
    state->expiryHeap[position] = state->expiryHeap[child];
    position = child;
  }
  if (state->heapLength) state->expiryHeap[position] = last;
  return first;
}

static void
SW_NAME(reportPair)
  (
    SW_NAME(SweepState)* state,
    size_t previous,
    size_t current
  )
{
  // Canonical operand order is essential: floating intersection coordinates
  // can differ in the last bit when the operands are swapped.
  const size_t first = previous < current ? previous : current;
  const size_t second = previous < current ? current : previous;
  SW_HIT hit;
  const Arcadia_Math_SegmentIntersection2Kind kind =
    SW_INTERSECT(state->segments + first, state->segments + second, &hit);
  if (kind == Arcadia_Math_SegmentIntersection2Kind_Invalid) {
    state->status = Arcadia_Math_SegmentSweep2Status_Invalid;
    return;
  }
  if (kind == Arcadia_Math_SegmentIntersection2Kind_None) return;
  if (state->eventCount == state->eventCapacity) {
    const size_t limit = SIZE_MAX / sizeof(*state->events);
    if (state->eventCapacity == limit) {
      state->status = Arcadia_Math_SegmentSweep2Status_NoMemory;
      return;
    }
    size_t next = state->eventCapacity ? state->eventCapacity + state->eventCapacity / 2 + 1 : 16;
    if (next <= state->eventCapacity || next > limit) next = limit;
    void* resized = realloc(state->events, next * sizeof(*state->events));
    if (!resized) {
      state->status = Arcadia_Math_SegmentSweep2Status_NoMemory;
      return;
    }
    state->events = resized;
    state->eventCapacity = next;
  }
  SW_EVENT* event = state->events + state->eventCount++;
  event->firstIndex = first;
  event->secondIndex = second;
  event->intersection = hit;
}

// Query all active Y ranges overlapping [low, high], without assuming that
// line segments sharing a Y bounding box actually intersect.
static void
SW_NAME(queryNode)
  (
    SW_NAME(SweepState)* state,
    size_t root,
    size_t current,
    SW_REAL low,
    SW_REAL high
  )
{
  if (root == SW_NONE || state->status != Arcadia_Math_SegmentSweep2Status_Completed)
    return;
  const SW_NAME(SweepNode)* node = state->nodes + root;
  if (node->subtreeHighY < low) return;
  if (node->left != SW_NONE)
    SW_NAME(queryNode)(state, node->left, current, low, high);
  if (state->status != Arcadia_Math_SegmentSweep2Status_Completed) return;
  if (node->lowY <= high && node->highY >= low)
    SW_NAME(reportPair)(state, root, current);
  if (node->lowY <= high && node->right != SW_NONE)
    SW_NAME(queryNode)(state, node->right, current, low, high);
}

Arcadia_Math_SegmentSweep2Status
SW_ENUM
  (
    const SW_SEG* segments,
    size_t numberOfSegments,
    SW_CALLBACK callback,
    void* context,
    size_t* numberOfIntersections
  )
{
  if (numberOfSegments && !segments) return Arcadia_Math_SegmentSweep2Status_Invalid;
  if (numberOfSegments > SIZE_MAX / sizeof(SW_NAME(SweepStart)) ||
      numberOfSegments > SIZE_MAX / sizeof(SW_NAME(SweepNode)) ||
      numberOfSegments > SIZE_MAX / sizeof(size_t))
    return Arcadia_Math_SegmentSweep2Status_Invalid;
  for (size_t i = 0; i < numberOfSegments; ++i) {
    const SW_SEG* segment = segments + i;
    if (!isfinite(segment->a[0]) || !isfinite(segment->a[1]) ||
        !isfinite(segment->b[0]) || !isfinite(segment->b[1]))
      return Arcadia_Math_SegmentSweep2Status_Invalid;
  }
  if (numberOfSegments < 2) {
    if (numberOfIntersections) *numberOfIntersections = 0;
    return Arcadia_Math_SegmentSweep2Status_Completed;
  }
  SW_NAME(SweepStart)* starts = malloc(numberOfSegments * sizeof(*starts));
  SW_NAME(SweepNode)* nodes = malloc(numberOfSegments * sizeof(*nodes));
  size_t* heap = malloc(numberOfSegments * sizeof(*heap));
  SW_NAME(SweepState) state = {0};
  state.segments = segments;
  state.nodes = nodes;
  state.root = SW_NONE;
  state.expiryHeap = heap;
  state.status = Arcadia_Math_SegmentSweep2Status_Completed;
  if (!starts || !nodes || !heap) {
    state.status = Arcadia_Math_SegmentSweep2Status_NoMemory;
    goto cleanup;
  }
  for (size_t i = 0; i < numberOfSegments; ++i) {
    const SW_SEG* segment = segments + i;
    SW_NAME(SweepNode)* node = nodes + i;
    node->lowY = segment->a[1] < segment->b[1] ? segment->a[1] : segment->b[1];
    node->highY = segment->a[1] > segment->b[1] ? segment->a[1] : segment->b[1];
    node->maxX = segment->a[0] > segment->b[0] ? segment->a[0] : segment->b[0];
    node->subtreeHighY = node->highY;
    node->left = SW_NONE;
    node->right = SW_NONE;
    node->height = 1;
    starts[i].index = i;
    if (segment->a[0] < segment->b[0] ||
        (segment->a[0] == segment->b[0] && segment->a[1] <= segment->b[1])) {
      starts[i].x = segment->a[0];
      starts[i].y = segment->a[1];
    } else {
      starts[i].x = segment->b[0];
      starts[i].y = segment->b[1];
    }
  }
  qsort(starts, numberOfSegments, sizeof(*starts), SW_NAME(compareStart));
  for (size_t i = 0; i < numberOfSegments; ++i) {
    const SW_NAME(SweepStart)* start = starts + i;
    while (state.heapLength && nodes[heap[0]].maxX < start->x) {
      const size_t expired = SW_NAME(popExpiry)(&state);
      state.root = SW_NAME(removeNode)(&state, state.root, expired);
    }
    const size_t index = start->index;
    SW_NAME(queryNode)(&state, state.root, index, nodes[index].lowY, nodes[index].highY);
    if (state.status != Arcadia_Math_SegmentSweep2Status_Completed) goto cleanup;
    state.root = SW_NAME(insertNode)(&state, state.root, index);
    SW_NAME(pushExpiry)(&state, index);
  }
  if (state.eventCount > 1)
    qsort(state.events, state.eventCount, sizeof(*state.events), SW_NAME(compareEvent));
  {
    size_t delivered = 0;
    for (; delivered < state.eventCount; ++delivered) {
      if (callback && !callback(context, state.events + delivered)) {
        ++delivered;
        state.status = Arcadia_Math_SegmentSweep2Status_Stopped;
        break;
      }
    }
    if (numberOfIntersections) *numberOfIntersections = delivered;
  }
cleanup:
  free(state.events);
  free(heap);
  free(nodes);
  free(starts);
  return state.status;
}

#undef SW_NONE
#undef SW_NAME
#undef SW_CAT
#undef SW_CAT1
