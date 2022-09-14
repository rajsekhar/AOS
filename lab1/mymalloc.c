/*
    mymalloc.c
*/

#include "mymalloc.h"

node_t *g_head = NULL;

void mergeFreeList ();

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

    // not required ading extra check to avoid seg fault
    if (ptr == NULL) {
        fprintf(stderr, "got the null pointer" );
        return;
    }

    header_t *hprt = (header_t*)ptr - 1;
    assert(hprt->magic == 1234567);
    // we got the free block
    node_t *ptrFreeBlk = (void*)hprt;

    node_t *ptrHead = g_head,
            *ptrPrv = NULL;

    // insert free block in respective location
    while (ptrHead != NULL) {

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
    // check and megre the free list, if adjacent block is free
    mergeFreeList ();
    // set pointer to null
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