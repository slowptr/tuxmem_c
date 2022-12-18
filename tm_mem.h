#ifndef TM_MEM_H
#define TM_MEM_H

#include <stdint.h>

typedef enum tm_mem_errors_e
{
  TM_MEM_OK = 0,
  TM_MEM_ERROR_UNDEFINED,
  TM_MEM_ERROR_NO_ACCESS,
} tm_mem_errors_t;

typedef struct tm_mem_s
{
  const char *name;
  uint32_t pid;
  int mem_fd;
} tm_mem_t;

typedef struct tm_mem_region_s
{
  const char *name;
  uint32_t start;
  uint32_t size;
} tm_mem_region_t;

tm_mem_errors_t tm_mem_open__name (const char *name, tm_mem_t *mem);
tm_mem_errors_t tm_mem_open__pid (uint32_t pid, tm_mem_t *mem);
void tm_mem_close (tm_mem_t *mem);

tm_mem_errors_t tm_mem_get_region_list (tm_mem_t *mem,
                                        tm_mem_region_t **regions,
                                        uint32_t *num_regions);
tm_mem_errors_t tm_mem_get_region (tm_mem_t *mem, const char *name,
                                   tm_mem_region_t *region);

tm_mem_errors_t tm_mem_read (tm_mem_t *mem, uintptr_t addr, void *buf,
                             uint32_t len);
tm_mem_errors_t tm_mem_write (tm_mem_t *mem, uintptr_t addr, void *buf,
                              uint32_t len);
tm_mem_errors_t tm_mem_find_pattern (tm_mem_t *mem, tm_mem_region_t *region,
                                     void *pattern, uint32_t pattern_len,
                                     uintptr_t *addr);
tm_mem_errors_t tm_mem_alloc (tm_mem_t *mem, uintptr_t *addr, uint32_t size);
tm_mem_errors_t tm_mem_free (tm_mem_t *mem, uintptr_t addr);
tm_mem_errors_t tm_mem_change_prot (tm_mem_t *mem, uintptr_t addr,
                                    uint32_t size, uint32_t prot);

tm_mem_errors_t tm_mem_hook_create (tm_mem_t *mem, void *addr, void *hook,
                                    void **orig);
tm_mem_errors_t tm_mem_hook_restore (tm_mem_t *mem, void *addr, void *orig);
tm_mem_errors_t tm_mem_inject_library (tm_mem_t *mem, const char *lib_path);

/*
tm_mem_errors_t tm_mem_get_region_data (tm_mem_t *mem, tm_mem_region_t *region,
                                        void **data, uint32_t *len);

tm_mem_errors_t tm_mem_get_region_data__name (tm_mem_t *mem, const char *name,
                                              void **data, uint32_t *len);
*/
#endif /* TM_MEM_H */
