#include "serial.h"
#include "raspi.h"
int _write(int file, char *ptr, int len)
{
    unsigned int ra;
    for(ra=0;ra<150;ra++)
        dummy(ra);
    for(ra=0;ra<len;ra++)
        print_ch(*ptr++);
    return len;
}
void _sbrk() {}
void _close() {}
void _fstat() {}
void _isatty() {}
void _lseek() {}
int _read(int file, char *ptr, int len)
{
    *ptr = get_char();
    return(1);
}




