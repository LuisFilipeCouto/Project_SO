/*
 *  \author António Rui Borges - 2012-2015
 *  \authur Artur Pereira - 2016-2020
 */

#include "freedatablocks.h"

#include <string.h>
#include <errno.h>
#include <iostream>

#include "core.h"
#include "devtools.h"
#include "daal.h"

namespace sofs20
{
    void grpReplenishRetrievalCache(void)
    {
        soProbe(443, "%s()\n", __FUNCTION__);

        /* replace the following line with your code */
       // binReplenishRetrievalCache();

        SOSuperblock* spb = soGetSuperblockPointer(); 
        uint32_t refs[REF_CACHE_SIZE]; 
        uint32_t assigned = 0;

        if(spb->retrieval_cache.idx == REF_CACHE_SIZE){ //quando a retrieval cache ficar sem refs para DB livres

            if(spb->reftable.count == 0 && spb->insertion_cache.idx == 0){
                throw SOException(ENOSPC,__FUNCTION__);
            }

            else if(spb->reftable.count > 0){  //reftable tem referencias para DB livres 

                uint32_t* refpointer = soGetReferenceBlockPointer(spb->reftable.blk_idx);                
                uint32_t k = spb->reftable.ref_idx;

                while(k < RPB && assigned < REF_CACHE_SIZE){
                    if(refpointer[k] != BlockNullReference){
                        refs[assigned] = refpointer[k];
                        refpointer[k] = BlockNullReference;
                        spb->reftable.ref_idx++;                        
                        spb->reftable.count--;
                        assigned++;
                    }
                    k++;
                }
                if(k == RPB && (spb->reftable.blk_idx == (spb->rt_size - 1))){
                    spb->reftable.blk_idx = 0;
                    spb->reftable.ref_idx = 0;
                } 
                else if(k == RPB){
                    spb->reftable.blk_idx++;
                    spb->reftable.ref_idx = 0;
                }      

                soSaveReferenceBlock();
            }

            else{ //reftable ou nao existe ou nao tem refs livres
                for(uint32_t i = 0; i < spb->insertion_cache.idx; i++){//percorre os DBS da insertion cache
                    refs[i] = spb->insertion_cache.ref[i];
                    spb->insertion_cache.ref[i] = BlockNullReference;
                    assigned++;
                }              

            }
            uint32_t c;
            uint32_t p = 0;
            spb->retrieval_cache.idx = spb->retrieval_cache.idx - assigned;
            for(c = (REF_CACHE_SIZE - assigned); c < REF_CACHE_SIZE; c++){
                spb->retrieval_cache.ref[c] = refs[p];
                p++;
            }

        }
        soSaveSuperblock();
    }
};

