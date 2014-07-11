// port.c - port handler

#include <bbb.h>

uint32_t *port_mmap(int n){
  static const off_t adrs[] = {
    0x44e07000,
    0x4804c000,
    0x481ac000,
    0x481ae000
  };

  int fd;
  void *map;

  fd = open("/dev/mem", O_RDWR | O_SYNC);
  if(fd < 0)
    return MAP_FAILED;

  map = mmap(NULL,
    getpagesize(),
    PROT_READ | PROT_WRITE,
    MAP_SHARED,
    fd,
    adrs[GPIO_PORT(n)]);

  close(fd);
  return map;
}

void port_outp(uint32_t *regs, uint32_t value){
  regs[0x13c / sizeof(uint32_t)] = value;
}

uint32_t port_inp(uint32_t *regs){
  return regs[0x138 / sizeof(uint32_t)];
}

void port_set(uint32_t *regs, uint32_t mask){
  regs[0x194 / sizeof(uint32_t)] = mask;
}

void port_clear(uint32_t *regs, uint32_t mask){
  regs[0x190 / sizeof(uint32_t)] = mask;
}
