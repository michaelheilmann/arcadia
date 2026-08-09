/*
 * Author: Michael Heilmann (contact@michaelheilmann.com)
 *
 * This is the underscore library.
 *
 * The underscore library is an isolation operations which must be successful and of which the outcome cannot be determined with reasonable effort.
 * All things start or end here.
 *
 * The underscore library provides a global recursive intra-process mutex.
 */
#if !defined(__H_INCLUDED)
#define __H_INCLUDED

void
_lockGlobalMutex
  (
  );

void
_unlockGlobalMutex
  (
  );

#endif // __H_INCLUDED
