
#include <stddef.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdbool.h>
#pragma pack(1)

#include "mymalloc.h"

typedef struct __node_t {
    int size;
    struct __node_t *next;
} node_t;


typedef struct {
    int size;
    int magic;
} header_t;


node_t *g_head; // free list

void InitMemory ();
void mergeFreeList ();
bool merge (node_t *ptrCrnt, node_t *ptrHead, node_t *ptrPrv);

void *mymalloc (size_t size) {

    if (g_head == NULL) {
        InitMemory ();
    }

    header_t *ptrNew = NULL;

    if (g_head != NULL) {

        node_t *ptrCrnt = g_head,
                *ptrPrev = NULL,
                *ptrNew1 = NULL;

        while (ptrCrnt != NULL) {
            size_t iReqSize = size + sizeof(header_t) + sizeof(node_t);
            if (iReqSize < ptrCrnt->size) {

                size_t sz = ptrCrnt->size;

                ptrNew1 = ptrCrnt;
                ptrNew1->size = sizeof(header_t) + size;
                ptrCrnt = (void*)ptrCrnt + size + sizeof(header_t);
                ptrCrnt->size = sz - (size + sizeof(header_t));
                ptrCrnt->next = ptrNew1->next;

                if (ptrPrev == NULL)
                    { g_head = ptrCrnt; }
                else
                    { g_head->next = ptrCrnt; }

                ptrNew = (void*)ptrNew1;
                ptrNew->magic = 1234567;
                break;
            } else {
                ptrPrev = ptrCrnt;
                ptrCrnt = ptrCrnt->next;
            }
        }
    }
    return ptrNew != NULL?(void*)ptrNew+sizeof(header_t):NULL;
}

void myfree (void *ptr) {

    header_t *hprt = (header_t*)ptr - 1;
    assert(hprt->magic == 1234567);
    node_t *ptrCrnt = (void*)hprt;

    node_t *ptrHead = g_head,
            *ptrPrv = NULL;

    while (ptrHead != NULL) {
        // check next availabe
        // if (merge(ptrCrnt, ptrHead, ptrPrv)) {
        //     break;
        // } else 
        if (ptrCrnt > ptrHead) {
            ptrPrv = ptrHead;
            ptrHead = ptrHead->next;
        } else {
            if (ptrPrv != NULL) {
                ptrPrv->next = ptrCrnt;
                ptrCrnt->next = ptrHead;
            } else {
                ptrCrnt->next = ptrHead;
                g_head = ptrCrnt;
            }
            break;
        }
    }
    mergeFreeList ();
    ptr = NULL;
}

void mergeFreeList () {

    node_t *ptrCrnt = g_head;

    while (ptrCrnt != NULL) {
        
        node_t *ptrNext = ptrCrnt->next;
        if (((void*)ptrCrnt+ptrCrnt->size) == ptrNext) {
            ptrCrnt->size = ptrCrnt->size + ptrNext->size;
            ptrCrnt->next = ptrNext->next;
        }
        if (ptrCrnt->next == NULL)
            break;
        ptrCrnt = ptrCrnt->next;
    }

    // final check
    if (((void*)g_head+g_head->size) == ptrCrnt) {
            g_head->size = g_head->size + ptrCrnt->size;
            g_head->next = ptrCrnt->next;
    }
}

void InitMemory () {
    // mmap() returns a pointer to a chunk of free space
    if (g_head == NULL) {
        g_head = mmap(NULL, 16384, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        g_head->size = 16384 - sizeof(node_t);
        g_head->next = NULL;
    }
}

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

