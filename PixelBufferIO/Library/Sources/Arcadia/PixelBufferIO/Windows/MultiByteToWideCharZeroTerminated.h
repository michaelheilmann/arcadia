#if !defined(ARCADIA_IMAGING_WNDOWS_MULTIBYTETOWIDECHARZEROTERMINATED_H_INCLUDED)
#define ARCADIA_IMAGING_WNDOWS_MULTIBYTETOWIDECHARZEROTERMINATED_H_INCLUDED

#define WIN32_LEAND_AND_MEAN
#include <windows.h>

/// Convert a zero-terminated multi-byte string into a zero-terminated wide character string.
/// @param source The zero-terminated multi-byte string.
/// @return The zero-terminated wide character string.
static inline wchar_t*
Arcadia_Windows_multiByteToWideCharZeroTerminated
  (
    const char* source
  )
{
  int result;
  result = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
  if (!result) {
    return NULL;
  }
  wchar_t* target = malloc(sizeof(wchar_t) * result);
  if (!target) {
    return NULL;
  }
  result = MultiByteToWideChar(CP_ACP, 0, source, -1, target, result);
  if (!result) {
    free(target);
    target = NULL;
    return NULL;
  }
  return target;
}

#endif // ARCADIA_IMAGING_WNDOWS_MULTIBYTETOWIDECHARZEROTERMINATED_H_INCLUDED
