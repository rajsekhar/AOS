/*
    mymalloc.h
*/

#ifndef _MYMALLOC_H
#define _MYMALLOC_H     1

#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdbool.h>

#pragma pack(1)

typedef struct __node_t {
    int size;
    struct __node_t *next;
} node_t;

typedef struct {
    int size;
    int magic;
} header_t;

void *mymalloc (size_t size);

void myfree (void *ptr);

node_t *initMemory ();

#endif