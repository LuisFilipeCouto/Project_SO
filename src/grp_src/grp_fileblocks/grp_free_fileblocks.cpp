#include "fileblocks.h"

#include "freedatablocks.h"
#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <inttypes.h>
#include <errno.h>
#include <assert.h>

namespace sofs20
{
//#if false
    /* free all blocks between positions ffbn and RPB - 1
     * existing in the block of references given by i1.
     * Return true if, after the operation, all references become BlockNullReference.
     * It assumes i1 is valid.
     */
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn);

    /* free all blocks between positions ffbn and RPB**2 - 1
     * existing in the block of indirect references given by i2.
     * Return true if, after the operation, all references become BlockNullReference.
     * It assumes i2 is valid.
     */
    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn);
//#endif

    /* ********************************************************* */

    void grpFreeFileBlocks(int ih, uint32_t ffbn)
    {
        soProbe(303, "%s(%d, %u)\n", __FUNCTION__, ih, ffbn);

        /* replace the following line with your code */
        //binFreeFileBlocks(ih, ffbn);
    
        if(ffbn < 0 || ffbn >= (N_DIRECT + N_INDIRECT*RPB + N_DOUBLE_INDIRECT*RPB*RPB)){ // ih must be a valid handler of an inode in-use
            throw SOException(EINVAL,__FUNCTION__);
        }

        SOInode* inode = soGetInodePointer(ih);
        uint32_t indexI1 = (ffbn - N_DIRECT)/RPB;
        uint32_t indexI2 = (ffbn - N_DIRECT - N_INDIRECT*RPB)/(RPB*RPB);
        uint32_t ffbnI1 = (ffbn - N_DIRECT)%RPB;
        uint32_t ffbnI2 = (ffbn - N_DIRECT - N_INDIRECT*RPB)%(RPB*RPB);
        bool delI1 = false;  
        bool delI2 = false;  
        uint32_t i = 0;
        uint32_t j = 0;
        uint32_t k = 0;
        
        while(i < N_DIRECT){ // Iterate through elements stored directly -- > d[]
        
            if(i >= ffbn && inode->d[i] != BlockNullReference){ 
                soFreeDataBlock(inode->d[i]);
                inode->d[i] = BlockNullReference;
                inode->blkcnt--;
            }
            i++;
        }

        while (j < N_INDIRECT){ // Iterate through elements stored indirectly --> i1[]
               
            if(inode->i1[j] != BlockNullReference){    

                if(j == indexI1){
                    delI1 = grpFreeIndirectFileBlocks(inode, inode->i1[j], ffbnI1);
                }

                if (j > indexI1){
                    delI1 = grpFreeIndirectFileBlocks(inode, inode->i1[j], 0);
                }

                if(delI1){
                    soFreeDataBlock(inode->i1[j]);
                    inode->i1[j] = BlockNullReference;
                    inode->blkcnt--;
                }
            }
            j++;
        }   

        while (k < N_DOUBLE_INDIRECT){ // Iterate through elements stored double indirectly --> i2[]

            if(inode->i2[k] != BlockNullReference){

                if(k == indexI2){
                    delI2 = grpFreeDoubleIndirectFileBlocks(inode, inode->i1[k], ffbnI2);
                }

                if (k > indexI2){
                    delI2 = grpFreeDoubleIndirectFileBlocks(inode, inode->i1[k], 0);
                }

                if(delI2){
                    soFreeDataBlock(inode->i2[k]);
                    inode->i2[k] = BlockNullReference;
                    inode->blkcnt--;
                }
            }
            k++;
        }
        soSaveInode(ih);
    }

    /* ********************************************************* */

//#if false
    static bool grpFreeIndirectFileBlocks(SOInode * ip, uint32_t i1, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i1, ffbn);

        /* replace the following line with your code */
        //throw SOException(ENOSYS, __FUNCTION__);

        uint32_t buff[RPB];
        soReadDataBlock(i1, buff);
        bool allFree = true;

        for (uint32_t i = 0; i < RPB; i++){
            
            if (i >= ffbn && buff[i] != BlockNullReference){
                soFreeDataBlock(buff[i]);
                buff[i] = BlockNullReference;
                ip->blkcnt--;
            }
            
            if (buff[i] != BlockNullReference){
                allFree = false;
            }
        }
        soWriteDataBlock(i1, buff);
        return allFree;
    }

    /* ********************************************************* */

    static bool grpFreeDoubleIndirectFileBlocks(SOInode * ip, uint32_t i2, uint32_t ffbn)
    {
        soProbe(303, "%s(..., %u, %u)\n", __FUNCTION__, i2, ffbn);

        /* replace the following line with your code */
        //throw SOException(ENOSYS, __FUNCTION__);

        uint32_t buff[RPB];
        soReadDataBlock(i2, buff);
        bool allFree = true;
        bool del = false;

        for (uint32_t i = 0; i < RPB; i++){
            
            if (buff[i] != BlockNullReference){
                
                if (i == ffbn/RPB){
                    del = grpFreeIndirectFileBlocks(ip, buff[i], ffbn%RPB);
                }
                
                else if (i > ffbn/RPB){
                    del = grpFreeIndirectFileBlocks(ip, buff[i], 0);
                }

                if (del){
                    soFreeDataBlock(buff[i]);   
                    buff[i] = BlockNullReference;
                    ip->blkcnt--;
                }

                if (buff[i] != BlockNullReference){
                    allFree = false;
                }
            }
        }
        soWriteDataBlock(i2, buff);
        return allFree;
    }
//#endif
    /* ********************************************************* */
};

