#include "../include/tm_utils.h"
#include <unistd.h>

bool
tm_utils_is_root ()
{
  return getuid () == 0;
}

const char *
tm_utils_status_to_str (tm_mem_errors_t status)
{
  switch (status)
    {
    case TM_MEM_ERROR_UNDEFINED:
      return "ERR: Undefined";
    case TM_MEM_ERROR_NO_ACCESS:
      return "ERR: No access";
    case TM_MEM_ERROR_NOT_FOUND:
      return "ERR: Not found";
    default:
      return "OK";
    }
}
