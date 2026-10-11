<h4>Type removed callback function</h4>
<p>
A type removed function is a function of the signature <code>void Arcadia_ARMS_TypeRemovedCallbackFunction(void* context, Arcadia_ARMS_Natural8 const* name, Arcadia_ARMS_Size nameLength)</code>.
The type removed callback function is supplied to a type when that type is created.
The function is invoked when the type is removed (for example, when <code>Arcadia_ARMS_shutdown</code> is invoked).
The first argument is the context as supplied to the <code>Arcadia_ARMS_addType</code>.
The second argument is an array of Bytes (the name of the type) and the third argument the length of that array.
</p>

<p>
<code>
struct File {<br>
&nbsp;...;<br>
};<br>
...<br>
static bool g_registered = false;
...<br>
void File_typeRemoved(void* context, Arcadia_ARMS_Natural8 const* name, Arcadia_ARMS nameLength) {<br>
&nbsp;g_registered = false;<br>
}<br>
...<br>
int main(int argc, char**argv) {<br>
&nbsp;Arcadia_ARMS_startup();<br>
&nbsp;if (!g_registered) {<br>
&nbsp;&nbsp;Arcadia_ARMS_addType("File", strlen("File"), NULL, &typeRemovedCallback, NULL, NULL);<br>
&nbsp;&nbsp;g_registered = true;<br>
&nbsp;}</br>
&nbsp;Arcadia_ARMS_shutdown();<br>
&nbsp;return EXIT_SUCCESS;<br>
}
</code>
</p>
<p>Note that types can be created with a null pointer for the finalize function.</p>
