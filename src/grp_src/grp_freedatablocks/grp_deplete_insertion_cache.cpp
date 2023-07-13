/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freedatablocks.h"

#include "core.h"
#include "devtools.h"
#include "daal.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;
#include <cmath>

namespace sofs20
{
    /* only fill the current block to its end */
    void grpDepleteInsertionCache(void)
    {
        soProbe(444, "%s()\n", __FUNCTION__);

        /* replace the following line with your code */
        //binDepleteInsertionCache();

        SOSuperblock* p_sb = soGetSuperblockPointer();

        if(p_sb->insertion_cache.idx < REF_CACHE_SIZE){  
            return;  // Nothing is done if cache is not full
        }

        uint32_t buff[RPB]; 
        uint32_t tailBlk = BlockNullReference;  // Last block with references in reference table
        uint32_t tailIdx = BlockNullReference;  // Index of the first cell without references in tailBlk

        uint32_t aux = (p_sb->reftable.blk_idx + (p_sb->reftable.count - ((RPB - p_sb->reftable.ref_idx)/RPB))); 

        if(p_sb->reftable.ref_idx + p_sb->reftable.count < RPB){ 
            tailBlk = p_sb->reftable.blk_idx;
            tailIdx = p_sb->reftable.ref_idx + p_sb->reftable.count;
        }

        else if ((p_sb->reftable.ref_idx + p_sb->reftable.count) == RPB){

            if((p_sb->reftable.blk_idx == (p_sb->rt_size - 1))){ 
                tailBlk = 0;
                tailIdx = 0;
            }
            else{
                tailBlk = p_sb->reftable.blk_idx++;
                tailIdx = 0;
            }
        }
        else{

            if(aux < (p_sb->rt_size - 1)){
                tailBlk = aux;
                tailIdx = (p_sb->reftable.count - (RPB - p_sb->reftable.ref_idx))%RPB;
            }

            else if(aux == (p_sb->rt_size - 1)){ 
                tailBlk = 0;
                tailIdx = (p_sb->reftable.count - (RPB - p_sb->reftable.ref_idx))%RPB;
            }

            else{
                tailBlk = p_sb->rt_size - aux;
                tailIdx = (p_sb->reftable.count - (RPB - p_sb->reftable.ref_idx))%RPB;
            }      
        }

        soReadDataBlock(tailBlk, &buff); 
        uint32_t i = tailIdx; // Index on Reference Data Block
        uint32_t j = 0;       // Index on Cache
        uint32_t k = 0;

        while(i < RPB && j < REF_CACHE_SIZE){

            buff[i] = p_sb->insertion_cache.ref[j];
            p_sb->insertion_cache.ref[j] = BlockNullReference;
            i++;
            j = i - tailIdx;
        }

        for (unsigned int nul = i; nul < RPB; nul++){
            buff[nul] = BlockNullReference;
        }

        while (j < REF_CACHE_SIZE){
            p_sb->insertion_cache.ref[k] = p_sb->insertion_cache.ref[j];
            p_sb->insertion_cache.ref[j] = BlockNullReference;
            j++;
            k++;
        }

        p_sb->insertion_cache.idx = k;
        p_sb->reftable.count = i;
        soWriteDataBlock(tailBlk, &buff);

        soSaveSuperblock();
    }
};