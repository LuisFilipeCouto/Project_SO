#include "fileblocks.h"

#include "freedatablocks.h"
#include "daal.h"
#include "core.h"
#include "devtools.h"

#include <errno.h>

#include <iostream>

namespace sofs20
{

#if true
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn);
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn);
#endif

    /* ********************************************************* */

    uint32_t grpAllocFileBlock(int ih, uint32_t fbn)
    {
        soProbe(302, "%s(%d, %u)\n", __FUNCTION__, ih, fbn);

        /* replace the following two lines with your code */
        //return binAllocFileBlock(ih, fbn);

        
        SOInode* inode_pointer = soGetInodePointer(ih);  

        uint32_t adbn;
        uint32_t dbleindirect_start = N_DIRECT + N_INDIRECT * RPB;
        uint32_t dbleindirect_finish = dbleindirect_start + N_DOUBLE_INDIRECT * RPB * RPB; // n_direct + n_indrect*rpb + n_double_indirect * rpb * rpb

        if(fbn < 0 || fbn > dbleindirect_finish){
            throw SOException(EINVAL,__FUNCTION__);
        }
        
        if(fbn < N_DIRECT){
            adbn = soAllocDataBlock();
            inode_pointer->blkcnt++;
            inode_pointer->d[fbn] = adbn;
        } 
        else if(fbn >= N_DIRECT && fbn < dbleindirect_start){
            adbn = grpAllocIndirectFileBlock(inode_pointer, fbn - N_DIRECT);
        }
        else{
            adbn = grpAllocDoubleIndirectFileBlock(inode_pointer, fbn - dbleindirect_start);
        }

        soSaveInode(ih);

        return adbn;
    }

    /* ********************************************************* */

#if true
    /*
    */
    static uint32_t grpAllocIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* replace the following two lines with your code */
       /* cIndirectFileBlock’ was not dethrow SOException(ENOSYS, __FUNCTION__); 
        return 0;*/

        uint32_t indirect_idx = (afbn) / RPB; 
        uint32_t indirect_pos = (afbn) % RPB;
        uint32_t refs[RPB];
        uint32_t adbn;

        if(ip->i1[indirect_idx] == BlockNullReference){
            uint32_t refblk = soAllocDataBlock();    //inicializa um bloco para guardar refencias para outros blocos

            for(uint32_t i = 0; i < RPB; i++){
                refs[i] = BlockNullReference;        //preenche com nullref um array com o nº de referencias que cabem num bloco
            }

            soWriteDataBlock(refblk,refs);           //escreve no bloco o conteudo do array
            ip->i1[indirect_idx] = refblk;           //guarda o bloco q é referencia para outros blocos

            adbn = soAllocDataBlock();
            refs[indirect_pos] = adbn;               //escreve na posição o bloco q foi allocado
            soWriteDataBlock(refblk,refs);           //escreve no bloco das referencias  
            ip->blkcnt = ip->blkcnt + 2;             //adiciona 2 blocos, um pras refs e outro o allocado 
        }
        else{
            soReadDataBlock(ip->i1[indirect_idx],refs);
            adbn = soAllocDataBlock();
            refs[indirect_pos] = adbn;
            soWriteDataBlock(ip->i1[indirect_idx],refs);
            ip->blkcnt++;
        }

        return adbn;
    }
#endif

    /* ********************************************************* */

#if true
    /*
    */
    static uint32_t grpAllocDoubleIndirectFileBlock(SOInode * ip, uint32_t afbn)
    {
        soProbe(302, "%s(%d, ...)\n", __FUNCTION__, afbn);

        /* replace the following two lines with your code */
        /*throw SOException(ENOSYS, __FUNCTION__); 
        return 0;*/
     
        uint32_t dblindirect_idx = (afbn) / (RPB*RPB); 
        uint32_t indirect_idx = (afbn - dblindirect_idx * RPB * RPB) / RPB;
        uint32_t indirect_pos = (afbn - dblindirect_idx * RPB * RPB) % RPB;
        uint32_t i2refs[RPB];
        uint32_t i1refs[RPB];

        uint32_t blk;        

        if(ip->i2[dblindirect_idx] == BlockNullReference){
            blk = soAllocDataBlock();

            for(uint32_t i = 0; i < RPB; i++){
                i2refs[i] = BlockNullReference;
            }
            soWriteDataBlock(blk,i2refs);
            ip->i2[dblindirect_idx] = blk;
            ip->blkcnt++;
        }
        else{
            soReadDataBlock(ip->i2[dblindirect_idx],i2refs); //lê o bloco onde i2 está armazenado e escreve num array que contem rpb refs
        }

        if(i2refs[indirect_idx] == BlockNullReference){
            blk = soAllocDataBlock();

            for(uint32_t i = 0; i < RPB; i++){
                i1refs[i] = BlockNullReference;
            }
            soWriteDataBlock(blk,i1refs);
            i2refs[indirect_idx] = blk;
            ip->blkcnt++;
        }
        else{
            soReadDataBlock(i2refs[indirect_idx],i1refs);  //lê o bloco i1 e guarda num array 
        }

        blk = soAllocDataBlock();
        i1refs[indirect_pos] = blk;
        ip->blkcnt++;
        soWriteDataBlock(i2refs[indirect_idx],i1refs);
        soWriteDataBlock(ip->i2[dblindirect_idx],i2refs);
            
        return blk;
    }
#endif
};

