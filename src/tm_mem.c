#define _GNU_SOURCE // needed for process_vm functions

#include "../include/tm_mem.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
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

      cur_pid = atoi (entry->d_name);
      if (cur_pid == 0)
        continue;

      const char *exe = "";
      tm_mem_errors_t status;
      status = _tm_mem_get_exe_link (cur_pid, &exe);
      if (status != TM_MEM_OK)
        continue;

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
  // open /proc/<pid>/maps
  char path[64];
  snprintf (path, sizeof (path), "/proc/%d/maps", mem->pid);

  FILE *maps = fopen (path, "r");
  if (maps == NULL)
    {
      perror ("fopen");
      return TM_MEM_ERROR_UNDEFINED;
    }

  // iterate through lines
  char line[256];
  while (fgets (line, sizeof (line), maps))
    {
      // parse line
      char *start = strtok (line, "-");
      char *end = strtok (NULL, " ");
      char *perms = strtok (NULL, " ");
      strtok (NULL, " "); // offset
      strtok (NULL, " "); // dev
      strtok (NULL, " "); // inode
      char *pathname = strtok (NULL, " ");

      // strip away newline
      pathname[strlen (pathname) - 1] = '\0';

      // check if name is empty
      if (strlen (pathname) == 0)
        continue;

      // skip if not readable
      if (perms[0] != 'r')
        continue;

      // convert to numbers
      uint64_t start_address = strtoull (start, NULL, 16);
      uint64_t end_address = strtoull (end, NULL, 16);

      // add to regions
      *regions
          = realloc (*regions, sizeof (tm_mem_region_t) * (*num_regions + 1));
      (*regions)[*num_regions].name = strdup (pathname);
      (*regions)[*num_regions].start = start_address;
      (*regions)[*num_regions].size = end_address - start_address;
      (*num_regions)++;
    }

  fclose (maps);
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_get_region (tm_mem_t *mem, const char *name, tm_mem_region_t *region)
{
  // overhead through the extra iteration over region list

  uint32_t num_regions;
  tm_mem_region_t *regions;
  tm_mem_errors_t status
      = tm_mem_get_region_list (mem, &regions, &num_regions);
  if (status != TM_MEM_OK)
    return status;

  for (uint32_t i = 0; i < num_regions; i++)
    {
      if (strcmp (regions[i].name, name) == 0)
        {
          *region = regions[i];
          free (regions);
          return TM_MEM_OK;
        }
    }
  free (regions);
  return TM_MEM_ERROR_NOT_FOUND;
}

tm_mem_errors_t
tm_mem_read (tm_mem_t *mem, uintptr_t addr, void *buf, uint32_t len)
{
  struct iovec local_iov = { buf, len };
  struct iovec remote_iov = { (void *)addr, len };

  ssize_t bytes_read
      = process_vm_readv (mem->pid, &local_iov, 1, &remote_iov, 1, 0);

  return (bytes_read == len) ? TM_MEM_OK : TM_MEM_ERROR_UNDEFINED;
}
tm_mem_errors_t
tm_mem_write (tm_mem_t *mem, uintptr_t addr, void *buf, uint32_t len)
{
  struct iovec local_iov = { buf, len };
  struct iovec remote_iov = { (void *)addr, len };

  ssize_t bytes_written
      = process_vm_writev (mem->pid, &local_iov, 1, &remote_iov, 1, 0);

  return (bytes_written == len) ? TM_MEM_OK : TM_MEM_ERROR_UNDEFINED;
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
tm_mem_alloc (tm_mem_t *mem, void *addr, uint32_t size, void **mapped_mem)
{
  *mapped_mem = mmap (addr, size, PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, fileno (mem->mem_file), 0);

  return (*mapped_mem == MAP_FAILED) ? TM_MEM_ERROR_MAP_FAILED : TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_free (void *addr, uint32_t size)
{
  if (munmap (addr, size) == -1)
    return TM_MEM_ERROR_UNDEFINED;

  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_change_prot (void *addr, uint32_t size, uint32_t prot)
{
  if (mprotect (addr, size, prot) == -1)
    return TM_MEM_ERROR_UNDEFINED;

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

