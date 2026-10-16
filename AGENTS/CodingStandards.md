# Coding Standards

## C Library Functions And String Handling

- Do not use C standard library functions such as `printf`, `fprintf`, `sprintf`, `snprintf`, `vprintf`, `vsnprintf`, `puts`, `fputs`, or other printf-style C library functions.
- Use Arcadia functionality instead. In particular, use `Arcadia_String` and `Arcadia_StringBuilder` for strings and for constructing messages and diagnostics.
- Formatting and concatenation must be expressed with the Arcadia string APIs, for example `Arcadia_String_createFromCxxString`, `Arcadia_StringBuilder_insertBackCxxString`, `Arcadia_StringBuilder_insertBackString`, and `Arcadia_String_create`, rather than with C formatting functions.
- Diagnostics and error messages must be passed around as `Arcadia_String` objects, not as native C strings.
