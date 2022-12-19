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

  pid_t pid = fork ();
  if (pid == 0)
    {
      status = tm_mem_open__pid (getppid (), &mem); // open parent's memory
      if (status != TM_MEM_OK)
        {
          printf ("Error: %s\n", tm_utils_status_to_str (status));
          return status;
        }

      uint32_t size = 1024;
      uint32_t *mapped;
      // NULL means that we don't care about the address
      status = tm_mem_alloc (&mem, NULL, size, (void **)&mapped);
      if (status != TM_MEM_OK)
        {
          printf ("Error: %s\n", tm_utils_status_to_str (status));
          return status;
        }

      mapped[0] = 0xdeadbeef;
      if (mapped[0] != 0xdeadbeef)
        {
          printf ("Error: mapped[0] != 0xdeadbeef\n");
          return TM_MEM_ERROR_UNDEFINED;
        }

      status = tm_mem_free (mapped, size);
      if (status != TM_MEM_OK)
        {
          printf ("Error: %s\n", tm_utils_status_to_str (status));
          return status;
        }
    }

  tm_mem_close (&mem);
  return 0;
}
