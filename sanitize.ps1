# The author of this software is Michael Heilmann (contact@michaelheilmann.com).
#
# Copyright(c) 2024-2025 Michael Heilmann (contact@michaelheilmann.com).
#
# Permission to use, copy, modify, and distribute this software for any
# purpose without fee is hereby granted, provided that this entire notice
# is included in all copies of any software which is or includes a copy
# or modification of this software and in all copies of the supporting
# documentation for such software.
#
# THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY.IN PARTICULAR, THE AUTHOR NOR LICENT MAKES ANY REPRESENTATION OR
# FITNESS FOR ANY PARTICULAR PURPOSE.

# This script
# - removes trailing spaces and tabs from the following file types
#   - *.txt (this includes every CMakeLists.txt)
#   - *.c
#   - *.h
#   - *.i
#   - *.tl
#   - *.pdl
# - and normalizes the line endings of those files to the Linux line ending
#   convention, that is '\n' (LF), never '\r\n' (CRLF).
#
# Files that already conform are not written, hence their modification time is
# left alone. The byte order mark, if any, and all bytes that are not line
# endings or trailing spaces and tabs are left alone as well.
function removeWhitespaceAtEndOfLinesAndNormalizeLineEndings {
  $forbiddenFolders=@(".git",".vs");
  $allowedExtensions=@(".txt", ".c", ".h", ".i",  ".tl", ".te", ".pdl", ".pdl");

  # ISO-8859-1 maps every byte value 0-255 onto the code point with the same
  # value and back. Round-tripping file content through it is therefore
  # byte-lossless for every input byte, which keeps the encoding of the
  # processed files untouched, independent of what that encoding is.
  $bytePreservingEncoding = [System.Text.Encoding]::GetEncoding(28591)

  $processedFileCount=0;
  $changedFileCount=0;

  $xl = new-object system.collections.stack;
  $x = Get-Item -Path '.';
  $xl.Push($x);

  while ($xl.Count -ne 0) {
    $x = $xl.Pop();
    if ($x.PSIsContainer) {
      # if the item is '.git' or '.vs', skip.
      if ($x.Name -In $forbiddenFolders) {
        continue;
      }
      Get-ChildItem -Path $x.FullName | Foreach-Object {
        $xl.Push($_);
      }
    } else {
      if ($x.Extension -NotIn $allowedExtensions) {
        # if the item is not a file this script handles, skip.
        continue;
      }
      # if the item is a file, process it.
      $processedFileCount += 1;

      $originalContent = $bytePreservingEncoding.GetString([System.IO.File]::ReadAllBytes($x.FullName));
      # '\r\n' first, so that every remaining '\r' is a lone carriage return.
      $content = $originalContent.Replace("`r`n", "`n").Replace("`r", "`n");
      # trailing spaces and tabs, either in front of a line ending or at the end of the file.
      $content = [System.Text.RegularExpressions.Regex]::Replace($content, "[ \t]+(?=\n|\z)", "");
      if ($content -ne $originalContent) {
        Write-Host "changed " $x.FullName;
        [System.IO.File]::WriteAllBytes($x.FullName, $bytePreservingEncoding.GetBytes($content));
        $changedFileCount += 1;
      }
    }
  }

  Write-Host ("processed " + $processedFileCount + " files, changed " + $changedFileCount + " files");
}

removeWhitespaceAtEndOfLinesAndNormalizeLineEndings