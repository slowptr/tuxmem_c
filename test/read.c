#include "../include/tm_mem.h"
#include "../include/tm_utils.h"
#include <unistd.h>

int
main ()
{
  if (!tm_utils_is_root ())
    {
      printf ("You must be root to run this program.\n");
      return 1;
    }

  tm_mem_errors_t status;
  tm_mem_t mem;
  status = tm_mem_open__pid (getpid (), &mem);
  if (status != TM_MEM_OK)
    {
      printf ("Error: %s\n", tm_utils_status_to_str (status));
      return status;
    }

  int test = 1234;
  uintptr_t test_addr = (uintptr_t)&test;
  int read_test = 0;

  status = tm_mem_read (&mem, test_addr, &read_test, sizeof (read_test));
  if (status != TM_MEM_OK)
    {
      printf ("Error: %s\n", tm_utils_status_to_str (status));
      return status;
    }

  if (read_test != test)
    {
      printf ("Error: read_test != test\n");
      return TM_MEM_ERROR_UNDEFINED;
    }

  tm_mem_close (&mem);
  return 0;
}
