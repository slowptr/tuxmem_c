#include "../include/tm_mem.h"
#include "../include/tm_utils.h"

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

  tm_mem_region_t region;
  status = tm_mem_get_region (&mem, "/usr/lib/libatomic.so.1.2.0", &region);
  if (status != TM_MEM_OK)
    {
      printf ("Error: %s\n", tm_utils_status_to_str (status));
      return status;
    }

  printf ("Region: %s | %d | %d\n", region.name, region.start, region.size);

  tm_mem_close (&mem);
  return 0;
}
