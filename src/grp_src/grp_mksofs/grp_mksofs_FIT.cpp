#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "devtools.h"
#include "bin_mksofs.h"

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>

namespace sofs20
{
    void grpFillInodeTable(uint32_t itotal, bool date)
    {
        soProbe(604, "%s(%u)\n", __FUNCTION__, itotal);
        /* replace the following line with your code */
        //binFillInodeTable(itotal, date);

        SOInode table [IPB];
        
        table[0].mode =  S_IFDIR | 0755; //permissions should be 0755      
        table[0].lnkcnt = 2; //2 directory entries
        table[0].owner = getuid(); //owner and group are given by syscalls getuid and getgid
        table[0].group = getgid();
        table[0].size = 128;    
        table[0].blkcnt = 1;
        
       if(date){
            time_t start_t;
            time(&start_t);
            int tmp = difftime(start_t, 0);
            table[0].atime = tmp;
            table[0].mtime = tmp;
            table[0].ctime = tmp;
        }else{
            table[0].atime = 0;
            table[0].mtime = 0;
            table[0].ctime = 0;
        }


        table[0].d[0] = 0; 
        for (uint32_t j = 1; j < N_DIRECT; j++) {
            table[0].d[j] = BlockNullReference;

        }

        for (uint32_t j = 0; j < N_INDIRECT; j++) {
            table[0].i1[j] = BlockNullReference;

        }

        for (uint32_t j = 0; j < N_DOUBLE_INDIRECT; j++) {
            table[0].i2[j] = BlockNullReference;

        }

        uint32_t y = 0;
        uint32_t x = 1;


        for (uint32_t i = 1; i < itotal; i++)
        {
            //creating inodes
            table[x].mode = 0;
            table[x].lnkcnt = 0;
            table[x].owner = 0;
            table[x].group = 0;
            table[x].size = 0;
            table[x].blkcnt = 0;
            table[x].atime = 0;
            table[x].mtime = 0;
            table[x].ctime = 0;
            

            for (uint32_t j = 0; j < N_DIRECT; j++) {
                table[x].d[j] = BlockNullReference;
            }

            for (uint32_t j = 0; j < N_INDIRECT; j++) {
                table[x].i1[j] = BlockNullReference;
            }

            for (uint32_t j = 0; j < N_DOUBLE_INDIRECT; j++) {
                table[x].i2[j] = BlockNullReference;
            }

            x++;
            if(x == IPB){
                //putting the inodes created into the block
                soWriteRawBlock(y+1,table);
                y++;
                x=0;
            }
        }
    }
};