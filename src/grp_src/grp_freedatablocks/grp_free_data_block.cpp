/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
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
    void grpFreeDataBlock(uint32_t bn)
    {
        soProbe(442, "%s(%u)\n", __FUNCTION__, bn);

        SOSuperblock* so= soGetSuperblockPointer(); //pointer para o superblock

        if(bn>so -> dbtotal){
    	    throw SOException(EINVAL, __FUNCTION__);    //verifica se o numero de refencia do dtablock é valido
        }

        if (so -> insertion_cache.idx == REF_CACHE_SIZE) {
            soDepleteInsertionCache();                  //chama a função deplete se o cache estiver cheio
        }
        so -> dbfree++;                             //aumenta o numero de datablocks livres no superblock
        so -> insertion_cache.ref[so->insertion_cache.idx] = bn;        //guarda a referencia do datablock livre
        so -> insertion_cache.idx += 1;
        soSaveSuperblock();           
 
        /* replace the following line with your code 
        binFreeDataBlock();*/
    }
};

