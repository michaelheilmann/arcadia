/**
 * @file maid-global-mutex.h
 * @brief Header file of the global mutex functionality.
 * @author Michael Heilmann (contact@michaelheilmann.com)
 */
#if !defined(MAID_GLOBAL_MUTEX_H_INCLUDED)
#define MAID_GLOBAL_MUTEX_H_INCLUDED

/**
 * @brief Wait indefinitly until a lock of the global recursive mutex is acquired.
 * @return The zero value on success. A non-zero value on failure.
 */
int maid_global_mutex_lock();

/**
 * @brief Relinquish a lock of the global recursive mutex.
 * @return The zero value on success. A non-zero value on failure.
 * In particualar, this function fails if the current thread has not acquired a lock of the global recursive mutex.
 */
int maid_global_mutex_unlock();

#endif // MAID_GLOBAL_MUTEX_H_INCLUDED
