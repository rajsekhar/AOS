/*
    mymalloc.c
*/

#include "mymalloc.h"

node_t *g_head = NULL;

void *mymalloc (size_t size) {

    if (g_head == NULL) {
        g_head = initMemory ();
    }

    header_t *ptrNewBuff = NULL;

    if (g_head != NULL) {

        node_t *ptrCrnt = g_head,
                *ptrPrev = NULL,
                *ptrNewMem = NULL;

        while (ptrCrnt != NULL) {
            size_t iReqSize = size + sizeof(header_t) + sizeof(node_t);
            if (iReqSize < ptrCrnt->size) {

                size_t sz = ptrCrnt->size;

                ptrNewMem = ptrCrnt;
                ptrNewMem->size = sizeof(header_t) + size;
                ptrCrnt = (void*)ptrCrnt + size + sizeof(header_t);
                ptrCrnt->size = sz - (size + sizeof(header_t));
                ptrCrnt->next = ptrNewMem->next;

                if (ptrPrev == NULL)
                    { g_head = ptrCrnt; }
                else
                    { g_head->next = ptrCrnt; }

                ptrNewBuff = (void*)ptrNewMem;
                ptrNewBuff->magic = 1234567;
                break;
            } else {
                ptrPrev = ptrCrnt;
                ptrCrnt = ptrCrnt->next;
            }
        }
    }
    return ptrNewBuff != NULL?(void*)ptrNewBuff+sizeof(header_t):NULL;
}

void myfree (void *ptr) {

    header_t *hprt = (header_t*)ptr - sizeof(header_t);
    assert(hprt->magic == 1234567);
    node_t *ptrFreeBlk = (void*)hprt;

    node_t *ptrHead = g_head,
            *ptrPrv = NULL;

    while (ptrHead != NULL) {
        // check next availabe
        // if (merge(ptrCrnt, ptrHead, ptrPrv)) {
        //     break;
        // } else 
        if (ptrFreeBlk > ptrHead) {
            ptrPrv = ptrHead;
            ptrHead = ptrHead->next;
        } else {
            if (ptrPrv != NULL) {
                ptrPrv->next = ptrFreeBlk;
                ptrFreeBlk->next = ptrHead;
            } else {
                ptrFreeBlk->next = ptrHead;
                g_head = ptrFreeBlk;
            }
            break;
        }
    }
    mergeFreeList ();
    ptr = NULL;
}

void mergeFreeList () {

    node_t *ptrCrnt = g_head;

    while (ptrCrnt != NULL && ptrCrnt->next != NULL) {
        
        node_t *ptrNext = ptrCrnt->next;
        if (((void*)ptrCrnt+ptrCrnt->size) == ptrNext) {
            ptrCrnt->size = ptrCrnt->size + ptrNext->size;
            ptrCrnt->next = ptrNext->next;
        }
        ptrCrnt = ptrCrnt->next;
    }

    // final check
    if (((void*)g_head+g_head->size) == ptrCrnt) {
            g_head->size = g_head->size + ptrCrnt->size;
            g_head->next = ptrCrnt->next;
    }
}

node_t *initMemory () {
    // mmap() returns a pointer to a chunk of free space
    node_t *head = NULL;
    head = mmap(NULL, 16384, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->size = 16384 - sizeof(node_t);
    head->next = NULL;
    return head;
}

/*
// not using
bool merge (node_t *ptrCrnt, node_t *ptrHead, node_t *ptrPrv) {
            
    bool merge = false;
    // check next availabe
    if (((void*)ptrCrnt+ptrCrnt->size) == ptrHead) {
                
        ptrCrnt->size = ptrCrnt->size + ptrHead->size;
        if (ptrPrv != NULL) {
            ptrPrv->next = ptrCrnt;
                    ptrCrnt->next = ptrHead->next;
        } else {
            ptrCrnt->next = ptrHead->next;
            g_head = ptrCrnt;
        }
        merge = true;
    }
    return merge;
}

*/