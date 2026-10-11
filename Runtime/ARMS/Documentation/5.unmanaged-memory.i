<h2>Unmanaged Memory</h2>
<p>
<em>Arcadia ARMS</em> allows for allocation, reallocation, and deallocation of <em>unmanaged</em> memory via <em>memory managers</em>.
</p>

<p>
There are two different memory managers available for different use cases.
Currently, the <em>default memory manager</em> and the <em>slab memory manager</em> are available.
The former is described in section <a href="#default-memory-manager">"Default Memory Manager"</a>,
the latter is described in section <a href="#slab-memory-manager">"Slab Memory Manager"</a>.
All memory manager share a commmon interface which is described in section <a href="#memory-manager-interface">"Memory Manager Interface"</a>.

<h3 id="memory-manager-interface">Memory Manager Interface</h3>
<p>This section describes the interface of the memory managers.</p>

<h4>Arcadia_ARMS_MemoryManager_allocate</h4>
<p><code>
Arcadia_ARMS_MemoryManager_Status Arcadia_ARMS_MemoryManager_allocate(Arcadia_ARMS_MemoryManager* memoryManager, void** p, Arcadia_ARMS_Size n);
</code></p>
<p>
Allocates an unmanaged memory block of size <code>n</code> using the specified memory manager.
</p>

<p>
<em>If this function succeeds</em>:
A pointer to the beginning of the memory block is stored in <code>*p</code>.
<code>Arcadia_ARMS_MemoryManager_Status_Success</code> is returned.
</p>

<p>
<em>If this function fails</em>:
The environment is not observably changed.
The function returns one of the status code in the table below.
</p>

<table>
  <tr><td>Status Code</td><td>Description</td></tr>
  <tr><td>Arcadia_ARMS_MemoryManager_Status_AllocationFailed</td><td>The allocation failed.</td></tr>
  <tr><td>Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid</td><td><code>p</code> is a null pointer.</td></tr>
</table>


<p>Note that <code>n</code> can be 0 as 0 is a valid size for a memory block.</p>


<h4>Arcadia_ARMS_MemoryManager_reallocate</h4>
<p><code>
Arcadia_ARMS_MemoryManager_Status Arcadia_ARMS_MemoryManager_reallocate(Arcadia_ARMS_MemoryManager* memoryManager, void** p, Arcadia_ARMS_Size n);
</code></p>

<p>
<em>If this function succeeds</em>:
A pointer to the beginning of the new memory block is stored in <code>*p</code>.
The first <my-mv>k</my-mv> values of the new memory memory block and the old memory block are equal
where <code><my-mv>k</my-mv></code> is <code>min(n, <my-mv>m</my-mv>)</code> where <code>n</code> is the size of the
new memory block and <my-mv>m</my-mv> is the size of the new memory block.
<code>Arcadia_ARMS_MemoryManager_Status_Success</code> is returned.
</p>

<p>
<em>If this function fails</em>:
The environment is not observably changed.
The function returns one of the status code in the table below.
</p>

<table>
  <tr><td>Status Code</td><td>Description</td></tr>
  <tr><td>Arcadia_ARMS_MemoryManager_Status_AllocationFailed</td><td>The allocation failed.</td></tr>
  <tr><td>Arcadia_ARMs_MemoryManager_Status_ArgumentValueInvalid</td><td><code>p</code> is a null pointer.</td></tr>
</table>

<p>Note that <code>n</code> can be 0 as 0 is a valid size for a memory block.</p>

<h4>Arcadia_ARMS_MemoryManager_deallocate</h4>
<p><code>
Arcadia_ARMS_MemoryManager_Status Arcadia_ARMs_MemoryManager_deallocate(Arcadia_ARMS_MemoryManager* memoryManager, void* p);
</code></p>

<p>Deallocates an unmanaged memory block.</p>
<p>
<em>If this function succeeds</em>:
The memory block pointed to by <code>p</code> was deallocated.
</p>

<p>
<em>If this function fails</em>:
The environment is not observably changed.
This function fails only if <code>p</code> is a null pointer. In that case, it returns <code>Arcadia_ARMS_MemoryManager_Status_ArgumentValueInvalid</code>.
</p>

<h3>Default Memory Manager</h3>
<p>
Obtain a pointer to the <em>default memory manager</em> by a call to to the function
</p>
<p><code>
Arcadia_ARMS_MemoryManager* Arcadia_ARMS_getDefaultMemoryManager()
</code></p>
<p>
This function always succeeds if Arcadia ARMS is initialized. Otherwise its behaviour is undefined.
The memory manager returned is valid as long as Arcadia ARMS is initialized.
</p>

<h3>Slab Memory Manager</h3>
<p>
Obtain a pointer to the <em>slab memory manager</em> by a call to to the function
</p>
<p><code>
Arcadia_ARMS_MemoryManager* Arcadia_ARMS_getSlabMemoryManager()
</code></p>
<p>
This function always succeeds if Arcadia ARMS is initialized. Otherwise its behaviour is undefined.
The memory manager returned is valid as long as Arcadia ARMS is initialized.
</p>

<p>Warning: The Slab Memory Manager is deprecated.</p>
