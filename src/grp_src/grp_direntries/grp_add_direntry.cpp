#include "direntries.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs20
{
    void grpAddDirentry(int pih, const char *name, uint16_t cin)
    {
        soProbe(202, "%s(%d, %s, %u)\n", __FUNCTION__, pih, name, cin);

        /* replace the following line with your code */
        //binAddDirentry(pih, name, cin);

        SOInode* parent_inode = soGetInodePointer(pih);
        SODirentry dirs[DPB];
        uint32_t blk = 0;
        uint32_t dir = 0;
        bool hasEmpDir = false;

        if(parent_inode->size == 0){
            blk = soAllocFileBlock(pih,0);
            for(uint32_t i=0;i<DPB; i++){
                dirs[i].in = 0x0;  
                strcpy(dirs[i].name,"");           
            }
            dir = 0;
            hasEmpDir = true;
            soWriteDataBlock(blk,dirs);
        }
 
        for(uint32_t i = 0; i < parent_inode->blkcnt ; i++){
            soReadFileBlock(pih,i,dirs);
            for(uint32_t j = 0; j < DPB; j++){
                if(strcmp(name,dirs[j].name) == 0){
                    throw SOException(EEXIST, __FUNCTION__);              
                }
                if(strcmp("",dirs[j].name) == 0 && hasEmpDir == false){
                    blk = i;
                    dir = j;
                    hasEmpDir = true;
                }
            }   
        }

        if(hasEmpDir == false){
            blk = soAllocFileBlock(pih,parent_inode->blkcnt);
            dirs[0].in = cin;
            strcpy(dirs[0].name,name);
            parent_inode->size += 64;
            for(uint32_t i=1;i<DPB; i++){
                dirs[i].in = 0x0;  
                strcpy(dirs[i].name,"");           
            }
            soWriteDataBlock(blk,dirs);
        }
        else{
            soReadDataBlock(blk,dirs);
            dirs[dir].in = cin;
            strcpy(dirs[dir].name,name);
            soWriteDataBlock(blk,dirs);
            parent_inode->size += 64;        
        }       
       
    }
};

