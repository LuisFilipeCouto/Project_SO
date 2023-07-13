/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freeinodes.h"

#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <iostream>

#include "core.h"
#include "devtools.h"
#include "daal.h"

namespace sofs20
{
    uint16_t grpAllocInode(uint32_t mode)
    {
        soProbe(401, "%s(0x%x)\n", __FUNCTION__, mode);

        /* replace the following line with your code */
        
        //return binAllocInode(mode);
        int isValid = 0;
        if(mode && S_IFREG ){
            isValid = 1;
            if(mode - S_IFREG > 0777 || mode - S_IFREG < 0000){
                SOException(EINVAL, __FUNCTION__);

            }
        }

        if(isValid == 0 && (mode && S_IFDIR)){
            isValid = 1;
            if(mode - S_IFDIR > 0777 || mode - S_IFDIR < 0000){
                SOException(EINVAL, __FUNCTION__);

            }

        }
        if(isValid == 0 && (mode && S_IFLNK)){
            isValid = 1;
            if(mode - S_IFLNK > 0777 || mode - S_IFLNK < 0000)
            {
                SOException(EINVAL, __FUNCTION__);

            }
        }

        if(! isValid){
            SOException(EINVAL, __FUNCTION__);
        }

        SOSuperblock *block = soGetSuperblockPointer();
        if(block->ifree == 0){
            SOException(ENOSPC, __FUNCTION__);

        }
        
        uint32_t idx = block->iidx;
        while(1){
            if(block->ibitmap[idx/32] != BlockNullReference){
                block->ifree --;
                block->ibitmap[idx/32] = (block->ibitmap[idx/32] << 1) | 0x1;
                int inodehandler = soOpenInode(idx+1);
                SOInode* temp = soGetInodePointer(inodehandler);
                temp->mode = mode;
                temp->owner = getuid();
                temp->group = getgid();
                time_t start_t;
                time(&start_t);
                int tmp = difftime(start_t, 0);
                temp->atime = tmp;
                temp->mtime = tmp;
                temp->ctime = tmp;
                soSaveInode(inodehandler);
                soCloseInode(inodehandler);
                block->iidx++;
                soSaveSuperblock();
                return block->iidx;
            }
            idx++;
            if(idx == MAX_INODES)
                idx = 0;
        }
        
    }
};

