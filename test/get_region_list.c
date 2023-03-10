#include "../include/tm_mem.h"
#include "../include/tm_utils.h"

#include <stdlib.h>

#define PROC_NAME "/usr/lib/chromium/chromium"

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
  status = tm_mem_open__name (PROC_NAME, &mem);
  if (status != TM_MEM_OK)
    {
      printf ("Error: %s\n", tm_utils_status_to_str (status));
      return status;
    }

  uint32_t num_regions;
  tm_mem_region_t *regions;
  status = tm_mem_get_region_list (&mem, &regions, &num_regions);
  if (status != TM_MEM_OK)
    {
      printf ("Error: %s\n", tm_utils_status_to_str (status));
      return status;
    }

  if (num_regions < 10)
    {
      printf ("Error: Not enough regions (for test case) found: %d.\n",
              num_regions);
      return TM_MEM_ERROR_UNDEFINED;
    }

  free (regions);

  tm_mem_close (&mem);
  return 0;
}
