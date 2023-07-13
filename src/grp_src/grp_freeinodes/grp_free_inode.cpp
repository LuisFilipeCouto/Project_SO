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

#include "core.h"
#include "devtools.h"
#include "daal.h"

namespace sofs20
{
    void grpFreeInode(uint16_t in)
    {
        soProbe(402, "%s(%u)\n", __FUNCTION__, in);

        SOSuperblock* so = soGetSuperblockPointer(); //pointer para o superblock

        if(!(so -> itotal>in && in>=0)){
    	    throw SOException(EINVAL, __FUNCTION__);    //verifica se o numero de refencia do inode é valido
        }

        int handler = soOpenInode(in);
        SOInode* inp = soGetInodePointer(handler);

        so -> ifree++;

        so->ibitmap[in/32] &= ~(1UL << in);

        inp -> mode=0;
        inp -> lnkcnt=0;
        inp -> owner=0;
        inp -> group=0;
        inp -> size=0;
        inp -> blkcnt=0;
        inp -> atime=0;
        inp -> mtime=0;
        inp -> ctime=0;
        for(unsigned int i=0; i<N_DIRECT ; i++){
            inp -> d[i]=BlockNullReference;
        }
        for(unsigned int i=0; i<N_INDIRECT ; i++){
            inp -> i1[i]=BlockNullReference;
        }
        for(unsigned int i=0; i<N_DOUBLE_INDIRECT ; i++){
            inp -> i2[i]=BlockNullReference;
        }
        
        soSaveInode(handler);
        soCloseInode(handler);
        soSaveSuperblock(); 
        /* replace the following line with your code 
        binFreeInode(in);*/
    }
};

