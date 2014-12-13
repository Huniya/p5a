#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mfs.h"
#include "udp.h"


int main() {
   int fd = MFS_Init("localhost", 4000);
   if (fd > -1) {
      // printf("lookup should be error %d\n", MFS_Lookup(0, "testing"));
      // printf("lookup shouldnt be error %d\n", MFS_Lookup(0, "."));
      printf("creat shouldnt be error %d\n", MFS_Creat(0, MFS_REGULAR_FILE, "lucas"));
      printf("lookup shouldnt be error %d\n", MFS_Lookup(0, "lucas"));

      char data[4096];
      int i = 0;
      for (i = 0; i < 4096; i++) {
         data[i] = 'c';
      }
      MFS_Write(1, data, 0);
      MFS_Write(1, data, 1);
      MFS_Write(1, data, 2);

      printf("shouldnt be error %d\n", MFS_Write(1, data, 0));
      printf("shouldnt be error %d\n", MFS_Write(1, data, 0));
      printf("should be error %d\n", MFS_Write(1, data, 14));


      char buffer[4096];
      MFS_Read(1, buffer, 0);
      MFS_Creat(0, MFS_DIRECTORY, "testing");
      MFS_Lookup(0, "testing");
      MFS_Lookup(0, "lucas");
      MFS_Stat_t stat;
      MFS_Stat(0, &stat);
      printf("stat stuff %d %d\n", stat.size, stat.type);
      MFS_Stat(1, &stat);
      printf("stat stuff %d %d\n", stat.size, stat.type);
      MFS_Stat(2, &stat);
      printf("stat stuff %d %d\n", stat.size, stat.type);



       printf("unlink shouldnt be error %d\n", MFS_Unlink(0, "lucas"));
      printf("lookup should be error %d\n", MFS_Lookup(0, "lucas"));
   };
   exit(0);
}
