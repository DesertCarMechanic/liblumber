
#include <stddef.h>
#include <sane.h>

void l_mem_copy_blind(void *to, void *from, size_t nr_bytes)
{
    byte8 *bto = (byte8 *)to;
    byte8 *bfrom = (byte8 *)from;

    for (u64 i = 0; i<nr_bytes; i++) {
        *bto = *bfrom;
        bto++;
        bfrom++;
    }
}

