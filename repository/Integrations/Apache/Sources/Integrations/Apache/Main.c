#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  const char* header =
    u8"Content-Type: text/html\n\n"
    ;
  fwrite(header, 1, strlen(header), stdout);
  const char *content =
    u8"<!DOCTYPE html>\n"
    u8"<html>\n"
    u8"<head>\n"
    u8"  <meta charset=\"UTF-8\">\n"
    u8"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    u8"</head>\n"
    u8"<body>\n"
    u8"Welcome to Arcadia.\n"
    u8"</body>\n"
    u8"</html>\n"
    ;
  fwrite(content, 1, strlen(content), stdout);
  return EXIT_SUCCESS;
}
