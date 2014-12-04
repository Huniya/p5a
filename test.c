#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mfs.h"
#include "udp.h"


int main() {
   int fd = MFS_Init("localhost", 4000);
   if (fd > -1) {
      printf("fd %d\n",fd);
      MFS_Lookup(0, "testing");
   };
   exit(0);
}
