#include "../include/tm_mem.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

tm_mem_errors_t
_tm_mem_get_exe_link (uint32_t pid, const char **out)
{
  char path[64];
  snprintf (path, sizeof (path), "/proc/%d/exe", pid);

  char exe[64];
  ssize_t len = readlink (path, exe, sizeof (exe));
  if (len == -1)
    return TM_MEM_ERROR_UNDEFINED;

  exe[len] = '\0';

  *out = strdup (exe);

  return TM_MEM_OK;
}
tm_mem_errors_t
_tm_mem_open_mem_fd (uint32_t pid, FILE **out)
{
  char path[64];
  snprintf (path, sizeof (path), "/proc/%d/mem", pid);

  FILE *mem = fopen (path, "r");
  if (mem == NULL)
    {
      perror ("fopen");
      return TM_MEM_ERROR_UNDEFINED;
    }

  *out = mem;
  return TM_MEM_OK;
}
tm_mem_errors_t
_tm_mem_get_pid_of_name (const char *name, uint32_t *out)
{
  uint32_t cur_pid = 0;
  DIR *proc_folder = NULL;
  DIR *pid_folder = NULL;

  proc_folder = opendir ("/proc");
  if (proc_folder == NULL)
    {
      perror ("opendir");
      return TM_MEM_ERROR_NO_ACCESS;
    }

  struct dirent *entry;
  while ((entry = readdir (proc_folder)))
    {
      if (entry->d_type != DT_DIR)
        continue;

      uint32_t cur_pid = atoi (entry->d_name);
      if (cur_pid == 0)
        continue;

      const char *exe = "";
      tm_mem_errors_t status;
      status = _tm_mem_get_exe_link (cur_pid, &exe);
      if (status != TM_MEM_OK)
        continue;

      printf ("%s\n", exe);

      if (strcmp (exe, name) == 0)
        {
          *out = cur_pid;
          break;
        }
    }

  closedir (proc_folder);
  closedir (pid_folder);
  return (cur_pid == *out) ? TM_MEM_OK : TM_MEM_ERROR_NOT_FOUND;
}

tm_mem_errors_t
tm_mem_open__name (const char *name, tm_mem_t *mem)
{
  mem->name = name;

  tm_mem_errors_t status;
  status = _tm_mem_get_pid_of_name (name, &mem->pid);
  if (status != TM_MEM_OK)
    return status;

  status = _tm_mem_open_mem_fd (mem->pid, &mem->mem_file);
  if (status != TM_MEM_OK)
    return status;

  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_open__pid (uint32_t pid, tm_mem_t *mem)
{
  mem->pid = pid;

  tm_mem_errors_t status;
  status = _tm_mem_get_exe_link (pid, &mem->name);
  if (status != TM_MEM_OK)
    return status;

  status = _tm_mem_open_mem_fd (mem->pid, &mem->mem_file);
  if (status != TM_MEM_OK)
    return status;

  return TM_MEM_OK;
}
void
tm_mem_close (tm_mem_t *mem)
{
  fclose (mem->mem_file);
}

tm_mem_errors_t
tm_mem_get_region_list (tm_mem_t *mem, tm_mem_region_t **regions,
                        uint32_t *num_regions)
{
  // iterate through /proc/<pid>/maps
  // and add each region to the list
  //  if the region is not already in the list
  //  and the region permissions are correct
  // if no regions found, return error
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_get_region (tm_mem_t *mem, const char *name, tm_mem_region_t *region)
{
  // iterate through /proc/<pid>/maps
  // go through each line
  // parse line
  // check permissions
  // if name matches, return region
  // if no region found, return error
  return TM_MEM_OK;
}

tm_mem_errors_t
tm_mem_read (tm_mem_t *mem, uintptr_t addr, void *buf, uint32_t len)
{
  // use process_vm_readv
  // if error, return error (bytes_read == len)
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_write (tm_mem_t *mem, uintptr_t addr, void *buf, uint32_t len)
{
  // use process_vm_writev
  // if error, return error (bytes_written == len)
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_find_pattern (tm_mem_t *mem, tm_mem_region_t *region, void *pattern,
                     uint32_t pattern_len, uintptr_t *addr)
{
  // iterate through region
  // read 1 bytes
  // if byte matches pattern[0]
  // read pattern_len bytes
  // if bytes match pattern
  // get addr, return
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_alloc (tm_mem_t *mem, uintptr_t *addr, uint32_t size)
{
  // use mmap
  // if error, return error
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_free (tm_mem_t *mem, uintptr_t addr)
{
  // use munmap
  // if error, return error
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_change_prot (tm_mem_t *mem, uintptr_t addr, uint32_t size,
                    uint32_t prot)
{
  // use mprotect
  // if error, return error
  return TM_MEM_OK;
}

tm_mem_errors_t
tm_mem_hook_create (tm_mem_t *mem, void *addr, void *hook, void **orig)
{
  // read mem
  // mmap mem using alloc
  // read and save the original bytes
  // write jump instruction to target
  // munmap mem using free
  // if error, return error
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_hook_restore (tm_mem_t *mem, void *addr, void *orig)
{
  // read mem
  // write original bytes to target
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_inject_library (tm_mem_t *mem, const char *lib_path)
{
  // get lib path length
  // read mem
  // mmap mem using alloc
  // write lib path to mem
  // write dlopen call to mem
  // munmap mem using free
  // if error, return error
  return TM_MEM_OK;
}

