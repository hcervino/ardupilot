#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#define PRU0_SHARED_BASE     0x4a311000
struct shared{
uint32_t pinToRead;
uint16_t head; //head offset from PRU0_SHARED_BASE
uint16_t tail; //tail offset from PRU0_SHARED_BASE
char buffer[4032];
};

int main (){
void* value;
int fd = open("/dev/mem", O_RDWR|O_SYNC);
struct shared lrb;
lrb.pinToRead=1; // Dummy value
lrb.head= 9; // pinToRead(4bytes)+head(2bytes)+tail(2bytes)
lrb.tail= 13;// pinToRead(4bytes)+head(2bytes)+tail(2bytes)+head(4bytes)

close(fd);
return 0;
}
