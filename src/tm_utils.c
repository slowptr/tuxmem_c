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
    case TM_MEM_ERROR_MAP_FAILED:
      return "ERR: Map failed";
    case TM_MEM_ERROR_UNMAP_FAILED:
      return "ERR: Unmap failed";
    case TM_MEM_ERROR_PROT_FAILED:
      return "ERR: Protection failed";
    case TM_MEM_ERROR_RW_FAILED:
      return "ERR: Read/write failed";
    default:
      return "OK";
    }
}
