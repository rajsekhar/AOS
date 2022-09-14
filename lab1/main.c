
/*
    main.c
*/

#include "mymalloc.h"

int main (int argc, char **argv) {

    char *szBuff1 = NULL, *szBuff2 = NULL, 
         *szBuff3 = NULL, *szBuff4 = NULL, *szBuff5 = NULL;

    szBuff1 = (char *)mymalloc (100);

    szBuff2 = (char *)mymalloc (150);

    szBuff3 = (char *)mymalloc (200);

    szBuff4 = (char *)mymalloc (500);

    if (szBuff3 != NULL) {
        myfree (szBuff3);
        szBuff3 = NULL;
    }

    if (szBuff4 != NULL) {
        myfree (szBuff4);
        szBuff4 = NULL;
    }

    szBuff5 = (char *)mymalloc (50);

    if (szBuff2 != NULL) {
        myfree (szBuff2);
        szBuff2 = NULL;
    }

    if (szBuff1 != NULL) {
        myfree (szBuff1);
        szBuff1 = NULL;
    }

    if (szBuff5 != NULL) {
        myfree (szBuff5);
        szBuff5 = NULL;
    }
    return 0;
}