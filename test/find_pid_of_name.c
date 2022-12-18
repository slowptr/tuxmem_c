#include "../include/tm_mem.h"

#define PROC_NAME "/usr/lib/chromium/chromium"

int
main ()
{
  tm_mem_errors_t status;
  tm_mem_t mem;
  status = tm_mem_open__name (PROC_NAME, &mem);
  if (status != TM_MEM_OK)
    return status;

  printf ("%s | %d\n", mem.name, mem.pid);

  tm_mem_close (&mem);
  return 0;
}
