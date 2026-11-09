# Arcadia
# Copyright (C) 2024-2026 Michael Heilmann
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU Affero General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

if (NOT DEFINED MyDependenciesDirectory OR "${MyDependenciesDirectory}" STREQUAL "")
  message(FATAL_ERROR "MyDependenciesDirectory is not defined")
endif()

if (NOT DEFINED MyDependenciesGitHubReleasePrefix OR "${MyDependenciesGitHubReleasePrefix}" STREQUAL "")
  message(FATAL_ERROR "MyDependenciesGitHubReleasePrefix is not defined")
endif()

# When no architecture is specified, preserve the original behavior.
if(NOT DEFINED MyDependenciesArchitecture OR "${MyDependenciesArchitecture}" STREQUAL "")
  set(architectures x64 x86)
elseif(MyDependenciesArchitecture STREQUAL "x64" OR MyDependenciesArchitecture STREQUAL "x86")
  set(architectures "${MyDependenciesArchitecture}")
else()
  message(FATAL_ERROR "Unsupported MyDependenciesArchitecture: '${MyDependenciesArchitecture}' (expected x64 or x86)")
endif()

file(MAKE_DIRECTORY "${MyDependenciesDirectory}")

foreach(architecture IN LISTS architectures)
  set(archive "${MyDependenciesDirectory}/dependencies-${architecture}.zip")
  set(url "${MyDependenciesGitHubReleasePrefix}/dependencies-${architecture}.zip")
  set(destination "${MyDependenciesDirectory}/${architecture}")

  # Retry only transient HTTP responses and interrupted transfers. Missing
  # release assets (404), certificate errors, and other permanent failures
  # must still fail immediately.
  set(maxAttempts 3)
  foreach(attempt RANGE 1 ${maxAttempts})
    message(STATUS "Downloading ${url} to ${archive} (attempt ${attempt}/${maxAttempts})")
    file(DOWNLOAD
         "${url}"
         "${archive}"
         SHOW_PROGRESS
         TIMEOUT 120
         INACTIVITY_TIMEOUT 30
         STATUS status
         LOG downloadLog)

    list(GET status 0 transferCode)
    list(GET status 1 transferMessage)
    if (transferCode EQUAL 0)
      break()
    endif()

    # Keep the final response after proxy negotiation and redirects.
    string(REGEX MATCHALL "HTTP/[0-9.]+[ \t]+[0-9][0-9][0-9]" httpResponses "${downloadLog}")
    set(httpDetails "")
    set(httpStatus "")
    if (httpResponses)
      list(GET httpResponses -1 httpResponse)
      set(httpDetails "; ${httpResponse}")
      string(REGEX MATCH "[0-9][0-9][0-9]$" httpStatus "${httpResponse}")
    endif()

    file(REMOVE "${archive}")
    set(errorDetails "libcurl ${transferCode}: ${transferMessage}${httpDetails}")
    set(retryable FALSE)
    if (httpStatus MATCHES "^(429|500|502|503|504)$")
      set(retryable TRUE)
    elseif (httpStatus STREQUAL "" AND transferCode MATCHES "^(7|28|52|56)$")
      # No definitive HTTP rejection; connection interrupted or timed out.
      set(retryable TRUE)
    endif()

    # Some CI gateways consistently return HTTP/2 504. On Windows, try a
    # different HTTP stack/protocol before giving up, without suppressing
    # certificate verification or treating HTTP failures as successful.
    if (WIN32 AND attempt EQUAL maxAttempts AND httpStatus STREQUAL "504")
      find_program(arcadiaCurlExecutable NAMES curl.exe)
      if (arcadiaCurlExecutable)
        message(STATUS "HTTP/2 504 after ${maxAttempts} attempts; trying curl.exe with HTTP/1.1")
        execute_process(
          COMMAND "${arcadiaCurlExecutable}"
                  --http1.1 --fail --location --silent --show-error
                  --connect-timeout 30 --max-time 120
                  --output "${archive}"
                  --write-out "HTTP %{http_code}"
                  "${url}"
          RESULT_VARIABLE curlExitCode
          OUTPUT_VARIABLE curlHttpStatus
          ERROR_VARIABLE curlError)
        string(STRIP "${curlHttpStatus}" curlHttpStatus)
        string(STRIP "${curlError}" curlError)
        if (curlExitCode EQUAL 0)
          message(STATUS "Dependency download succeeded using curl.exe HTTP/1.1 (${curlHttpStatus})")
          break()
        endif()
        file(REMOVE "${archive}")
        set(errorDetails "${errorDetails}; curl.exe HTTP/1.1 failed (exit ${curlExitCode}; ${curlHttpStatus}; ${curlError})")
      else()
        message(WARNING "curl.exe not found; HTTP/1.1 fallback unavailable")
      endif()
    endif()

    if (NOT retryable OR attempt EQUAL maxAttempts)
      message(FATAL_ERROR
        "Downloading ${url} failed after ${attempt} attempt(s) (${errorDetails})")
    endif()

    # Short bounded backoff: two seconds, then four seconds.
    math(EXPR delay "${attempt} * 2")
    message(WARNING "Download failed (${errorDetails}); retrying in ${delay}s")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E sleep "${delay}")
  endforeach()

  file(REMOVE_RECURSE "${destination}")
  file(MAKE_DIRECTORY "${destination}")

  message(STATUS "Extracting ${archive} to ${destination}")
  file(ARCHIVE_EXTRACT
       INPUT "${archive}"
       DESTINATION "${destination}")
endforeach()
