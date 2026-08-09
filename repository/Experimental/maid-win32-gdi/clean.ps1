Remove-Item -LiteralPath "./Debug" -Force -Recurse
Remove-Item -LiteralPath "./MinSizeRel" -Force -Recurse
Remove-Item -LiteralPath "./Release" -Force -Recurse
Remove-Item -LiteralPath "./RelWithDebInfo" -Force -Recurse
Remove-Item -LiteralPath "./x64" -Force -Recurse
Remove-Item -LiteralPath "./Win32" -Force -Recurse
Remove-Item -LiteralPath "./.vs" -Force -Recurse
Remove-Item -LiteralPath "./MinSizeRel" -Force -Recurse
Remove-Item -LiteralPath "./maid-win32-gdi.dir" -Force -Recurse
Remove-Item -LiteralPath "./CMakeFiles" -Force -Recurse

Remove-Item -LiteralPath "./maid.sln" -Force -Recurse

Remove-Item -LiteralPath "./ZERO_CHECK.vcxproj" -Force -Recurse
Remove-Item -LiteralPath "./ZERO_CHECK.vcxproj.filters" -Force -Recurse


Remove-Item -LiteralPath "./ALL_BUILD.vcxproj" -Force -Recurse
Remove-Item -LiteralPath "./ALL_BUILD.vcxproj.filters" -Force -Recurse
Remove-Item -LiteralPath "./ALL_BUILD.vcxproj.user" -Force -Recurse

Remove-Item -LiteralPath "./maid-win32-gdi.vcxproj" -Force -Recurse
Remove-Item -LiteralPath "./maid-win32-gdi.vcxproj.filters" -Force -Recurse
Remove-Item -LiteralPath "./maid-win32-gdi.vcxproj.user" -Force -Recurse

Remove-Item -LiteralPath "./CMakeCache.txt" -Force -Recurse
Remove-Item -LiteralPath "./cmake_install.cmake" -Force -Recurse
