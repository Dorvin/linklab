#include <stdlib.h>
#include <libunwind.h>

int get_callinfo(char *fname, size_t fnlen, unsigned long long *ofs)
{
  unw_cursor_t cursor;
  unw_context_t context;
  unw_word_t offset;
  unw_word_t pc;
  if (unw_getcontext(&context))
    return -1;
  if (unw_init_local(&cursor, &context))
    return -1;
  while (unw_step(&cursor) > 0) {
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    if (pc == 0) {
      break;
    }
    //printf("0x%lx:", pc);
    if (unw_get_proc_name(&cursor, fname, fnlen, &offset) == 0) {
      if(fname[0] == 'm' && fname[1] == 'a' && fname[2] == 'i' && fname[3] == 'n'){
        *ofs = (unsigned long long)offset;
        return (int)pc;
      }
      //printf(" (%s+0x%lx)\n", sym, offset);
    } else {
      //printf(" -- error: unable to obtain symbol name for this frame\n");
    }
  }
  return -1;
}
