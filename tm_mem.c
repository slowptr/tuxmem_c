#include "tm_mem.h"

tm_mem_errors_t
_tm_mem_get_exe_link (uint32_t pid, const char *out)
{
  // open proc/<pid>/exe file
  // read link
  // write link to out
  // if error, return error
  return TM_MEM_OK;
}

tm_mem_errors_t
tm_mem_open__name (const char *name, tm_mem_t *mem)
{
  // open proc folder
  // iterate over all process folders
  // if yes, open exe file
  //  and read link
  //  compare link to name
  //  profit
  // if no, continue
  // if no process found, return error
  return TM_MEM_OK;
}
tm_mem_errors_t
tm_mem_open__pid (uint32_t pid, tm_mem_t *mem)
{
  // get name through exe link
  // set pid
  // open mem file
  return TM_MEM_OK;
}
void
tm_mem_close (tm_mem_t *mem)
{
  // close mem file
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

