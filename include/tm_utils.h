#ifndef TM_UTILS_H
#define TM_UTILS_H

#include "../include/tm_mem.h"
#include <stdbool.h>

bool tm_utils_is_root ();
const char *tm_utils_status_to_str (tm_mem_errors_t status);

#endif /* TM_UTILS_H */
