#include "../include/tm_mem.h"

int
main ()
{
  tm_mem_errors_t status;
  tm_mem_t mem;
  status = tm_mem_open__name ("", &mem);
  if (status != TM_MEM_OK)
    return status;

  printf ("%s | %d\n", mem.name, mem.pid);

  tm_mem_close (&mem);
  return 0;
}
