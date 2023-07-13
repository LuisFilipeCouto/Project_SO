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
    uint16_t grpDeleteDirentry(int pih, const char *name)
    {
        soProbe(203, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        /* replace the following line with your code */
        /*return binDeleteDirentry(pih, name);*/

        SODirentry dEntry;
        SOInode* inode = soGetInodePointer(pih);
        uint16_t in = -1;
        SODirentry dirEntries[DPB];
        uint32_t blk_dlt;
        uint32_t dir_dlt;
        for (uint32_t i = 0; i < inode->blkcnt; i++)              //iterate through blocks
        {
            soReadFileBlock(pih,i,dirEntries);
            for (uint32_t j = 0; j < DPB; j++)
            {
                dEntry = dirEntries[j];
                if (strcmp(name, dEntry.name) == 0)         //name and dEntry.name should be equal
                {   
                    in = dEntry.in;
                    strcpy(dEntry.name, "\0");              //delete name
                    dEntry.in = 0;         
                    dirEntries[j] = dEntry;
                    inode->size -= 64;
                    soWriteFileBlock(pih,i,dirEntries);
                    if((j == DPB - 1 && i == inode->blkcnt - 1) || (i < DPB - 1 && strcmp("",dirEntries[j+1].name) == 0)){
                        break;
                    } 
                    else{
                        blk_dlt = i;
                        dir_dlt = j;
                    }                                    
                }
                else if(strcmp("",dirEntries[j].name) == 0){
                    SODirentry moved;
                    strcpy(moved.name,dirEntries[j-1].name);
                    moved.in = dirEntries[j-1].in;
                    strcpy(dirEntries[j-1].name,"\0");
                    dirEntries[j-1].in = 0;
                    soWriteFileBlock(pih,i,dirEntries);
                    if(j-1 == 0){
                        soFreeFileBlocks(pih,inode->blkcnt-1);
                    }
                    soReadFileBlock(pih,blk_dlt,dirEntries);
                    dirEntries[dir_dlt] = moved;
                    soWriteFileBlock(pih,blk_dlt,dirEntries);
                    break;
                }
                else if(j == DPB-1 && i == inode->blkcnt - 1){
                    SODirentry moved;
                    strcpy(moved.name,dirEntries[j].name);
                    moved.in = dirEntries[j].in;
                    strcpy(dirEntries[j].name,"\0");
                    dirEntries[j].in = 0;
                    soWriteFileBlock(pih,i,dirEntries);
                    
                    soReadFileBlock(pih,blk_dlt,dirEntries);
                    dirEntries[dir_dlt] = moved;
                    soWriteFileBlock(pih,blk_dlt,dirEntries);
                    break;
                }
            }
        }
        if(in != -1){
            return in; //return the inode number of the deleted entry
        }
        else{
            throw SOException(ENOENT, __FUNCTION__);            //throws exception if dEntry is not found
        }
        
        
    }
};

