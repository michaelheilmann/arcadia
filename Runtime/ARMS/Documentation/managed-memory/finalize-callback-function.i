<h4>Finalize callback function</h4>
<p>
A finalize function is a function of the signature <code>void Arcadia_ARMS_FinalizeCallbackFunction(void* context, void *object)</code>.
The finalize callback function is supplied to a type when that type is created via <code>Arcadia_ARMS_addType</code>.
The first argument is the context as supplied to the <code>Arcadia_ARMS_addType</code>.
The second argument is a pointer to the object.
The finalize function shall perform cleanup of unmanaged resources like unmanaged memory, file handles, etc.
In the following example, <code>File_finalize</code> is implemented to invoke <code>fclose</code> on the field <code>fd</code> of struct <code>File</code> if it was not null.
</p>
<p>
<code>
struct File {<br>
&nbsp;FILE* fd;<br>
};<br>
...<br>
void File_finalize(void* context, File* file) {<br>
&nbsp;if (file->fd) {<br>
&nbsp;&nbsp;fclose(file->fd);<br>
&nbsp;&nbsp;file->fd = NULL;<br>
&nbsp;}<br>
}<br>
...<br>
int main(int argc, char**argv) {<br>
&nbsp;Arcadia_ARMS_startup();<br>
&nbsp;Arcadia_ARMS_addType("File", strlen("File"), NULL, NULL, NULL, &finalizeFile);<br>
&nbsp;struct File* file;<br>
&nbsp;Arcadia_ARMS_allocate(&file, "File", strlen("File"), sizeof(struct File));<br>
&nbsp;file->fd = fopen(...);<br>
&nbsp;Arcadia_ARMS_RunStatistics statistics = { .destroyed = 0 };<br>
&nbsp;Arcadia_ARMS_run(&statistics);<br>
&nbsp;Arcadia_ARMS_shutdown();<br>
&nbsp;return EXIT_SUCCESS;<br>
}
</code>
</p>
<p>Note that types can be created with a null pointer for the finalize function.</p>
