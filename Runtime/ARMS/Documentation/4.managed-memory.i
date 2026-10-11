<h2>Managed Memory</h2>

<h3>Creating types</h3>
<p>The user adds a type to the Arcadia ARMS by invoking <code>Arcadia_ARMS_Status Arcadia_ARMS_addType(const char* name, size_t nameLength,
void* context, Arcadia_ARMS_TypeRemovedCallbackFunction* typeRemoved, Arcadia_ARMS_VisitCallbackFunction *visit, Arcadia_ARMS_FinalizeCallbackFunction* finalize)</code>. The first argument <code>name</code> is
a pointer to an array of <code>nameLength</code> Bytes denoting the type name. No two types of the same name can be
registered and this function fails with <code>Arcadia_ARMS_Status_TypeExists</code> if an attempt is made to do so. <code>visit</code>
must point to a <code>Arcadia_ARMS_VisitCallbackFunction</code> or must be a null pointer. <code>finalize</code> must point to a
<code>Arcadia_ARMS_FinalizeCallbackFunction</code> or must be a null pointer. <code>typeRemoved</code> must point to a <code>Arcadia_ARMS_TypeRemovedCallbackFunction</code>
or must be a null pointer. <code>context</code> is an opaque pointer which is passed to the type removed callback function,
the visit callback function, and the finalize callback function. If <code>Arcadia_ARMS_addType</code> fails it returns a value different from
<code>Arcadia_ARMS_Status_Success</code>. The following table lists the possible values returned in case of failure
</p>
<table>
  <tr><td>Value</td><td>Description</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>pObject</code> is a null pointer</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>name</code> is a null pointer</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>nameLength</code> exceeds limits</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>size</code> exceeds limits</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_AllocationFailed</code></td><td>an allocation failed</td></tr>
</table>

@{include("managed-memory/type-removed-callback-function.i")}
@{include("managed-memory/visit-callback-function.i")}
@{include("managed-memory/finalize-callback-function.i")}

<h3>Creating objects</h3>
<p>
To create an object, the user of Arcadia ARMS creates an object by invoking <code>Arcadia_ARMS_Status Arcadia_ARMS_allocate(void** pObject,
char const* name, size_t nameLength, size_t size)</code>. <code>name</code> is a pointer to an array of <code>nameLength</code> Bytes
denoting the type name of the type to be assigned to object. <code>size</code> denotes the size, in Bytes, of the object to allocated
(0 is a valid size). If this function is invoked successfully, the <code>*pObject</code> is assigned a pointer to an object of the
specified size. The contents of the Bytes are unspecified. The object is assigned the type of the specified name.
If this function fails it returns a value different from  <code>Arcadia_ARMS_Status_Success</code>.
The following table lists the possible values returned in case of failure
</p>
<table>
  <tr><td>Value</td><td>Description</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>pObject</code> is a null pointer</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>name</code> is a null pointer</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>nameLength</code> exceeds limits</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>size</code> exceeds limits</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_TypeNotExists</code></td><td>the type does not exist</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_AllocationFailed</code></td><td>an allocation failed</td></tr>
</table>

<h3>4.3 Freeing resources</h3>
<p>To relinqish resources, the user invokes <code>Arcadia_ARMS_Status Arcadia_ARMS_run()</code> which destroys dead objects \(D\) such that the new universe
only contains live objects, in other terms \(U@@new = U@@old - D\) or equivalently \(U@@new = L\) holds.</p>

<p><code>Arcadia_ARMS_run</code> always succeeds.</p>

<h3>Locks</h3>
<p>
A locked object \(x\) is element of the root set \(R\).
Hence this object and all objects reachable from this object are live.
</p>

<p>
To add a lock to an object, the user invokes <code>Arcadia_ARMS_Status Arcadia_ARMS_lock(void* object)</code>.
If this function is invoked successfully, the lock count of the object pointed to by <code>object</code>
was incremented by one. The initial lock count of an object is 0. A lock count of 0 means an object is not locked.
If this function fails it returns a value different from  <code>Arcadia_ARMS_Status_Success</code>.
The following table lists the possible values returned in case of failure
<table>
  <tr><td>Value</td><td>Description</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>object</code> is a null pointer</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_OperationInvalid</code></td><td>the lock would overflow</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_AllocationFailed</code></td><td>an allocation failed</td></tr>
</table>

<p>
To remove a lock from an object, the user invokes <code>Arcadia_ARMS_Status Arcadia_ARMS_unlock(void* object)</code>.
If this function is invoked successfully, the lock count of the object pointed to by <code>object</code>
was decremented by one. If this function fails it returns a value different from  <code>Arcadia_ARMS_Status_Success</code>.
The following table lists the possible values returned in case of failure
<table>
  <tr><td>Value</td><td>Description</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_ArgumentInvalid</code></td><td><code>object</code> is a null pointer</td></tr>
  <tr><td><code>Arcadia_ARMS_Status_OperationInvalid</code></td><td>the lock count would underflow</td></tr>
</table>
