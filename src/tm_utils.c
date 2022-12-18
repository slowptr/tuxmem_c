#include "../include/tm_utils.h"
#include <unistd.h>

bool
tm_utils_is_root ()
{
  return getuid () == 0;
}
