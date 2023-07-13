/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2009-2020
 */

#include "freedatablocks.h"

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
    uint32_t grpAllocDataBlock()
    {
        soProbe(441, "%s()\n", __FUNCTION__);

        SOSuperblock* so= soGetSuperblockPointer();

        if(so -> retrieval_cache.idx==0 && so -> dbfree==0 ){
    	    throw SOException(ENOSPC, __FUNCTION__);    
        }
        if (so -> retrieval_cache.idx == REF_CACHE_SIZE) {
            soReplenishRetrievalCache();            
        }
        so -> dbfree--; 
        uint32_t res =so -> retrieval_cache.ref[so->retrieval_cache.idx];
        so -> retrieval_cache.ref[so->retrieval_cache.idx]=BlockNullReference;
        so -> retrieval_cache.idx +=1 ;
        soSaveSuperblock();

        /* replace the following line with your code 
        binAllocDataBlock();*/
        return res;
    }
};

