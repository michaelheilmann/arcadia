<section class="cxx entity object">
<h1 id="Arcadia_FileSystem">Arcadia_FileSystem</h1>
<p><code>Arcadia_FileSystem</code> extends <code>Arcadia_Object</code>.</p>
<p><code>Arcadia_FileSystem</code> provides access to the file system.</p>
<p>
For safe directory traversal, use <a href="#Arcadia_FileSystem_isSymbolicLink"><code>Arcadia_FileSystem_isSymbolicLink</code></a>
to inspect an entry before opening it or descending into it. This tests the final path component
without following it, but does not eliminate races between inspection and subsequent access.
On Windows, this operation identifies any reparse point, including junctions, rather than only symbolic links.
</p>
<p>
For safe directory traversal, use <a href="#Arcadia_FileSystem_isSymbolicLink"><code>Arcadia_FileSystem_isSymbolicLink</code></a>
to inspect an entry before opening it or descending into it. This tests the final path component
without following it, but does not eliminate races between inspection and subsequent access.
On Windows, this operation identifies any reparse point, including junctions, rather than only symbolic links.
</p>
</section>

@{include("FileSystem/FileSystem/createDirectoryIterator.i")}
@{include("FileSystem/FileSystem/createFileHandle.i")}

@{include("FileSystem/FileSystem/deleteDirectoryFile.i")}
@{include("FileSystem/FileSystem/deleteFile.i")}
@{include("FileSystem/FileSystem/deleteRegularFile.i")}
@{include("FileSystem/FileSystem/directoryFileExists.i")}

@{include("FileSystem/FileSystem/getConfigurationDirectory.i")}
@{include("FileSystem/FileSystem/getFileContents.i")}
@{include("FileSystem/FileSystem/getFileType.i")}
@{include("FileSystem/FileSystem/getFileType.i")}
@{include("FileSystem/FileSystem/getOrCreate.i")}
@{include("FileSystem/FileSystem/getSaveDirectory.i")}
@{include("FileSystem/FileSystem/getWorkingDirectory.i")}

@{include("FileSystem/FileSystem/isSymbolicLink.i")}

@{include("FileSystem/FileSystem/isSymbolicLink.i")}

@{include("FileSystem/FileSystem/regularFileExists.i")}

@{include("FileSystem/FileSystem/setFileContents.i")}


