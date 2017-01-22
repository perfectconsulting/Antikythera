/*
 *
 *    keystore.c
 *    Version 1.00 (C99)  
 * 
 *    Copyright 2017 Steven Janes (www.perfectconsulting.co.uk)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include "constants.h"

//#include "leak_detector_c.h"

typedef struct keystore_keyring {
    struct KeyStore_KeyPair**   keychain;
    short                       maxkeylength;
    long                        keys;
} KeyStore_KeyRing;

typedef struct keystore_keypair {
    char*                       key;
    unsigned long               value;
    void*                       next;
} KeyStore_KeyPair;

typedef short(*KeyStore_Enumerator)(char*, unsigned long);

static short FastCompare(char *s1, char *s2)
{
    short n = *s1 - *s2;
    if(n != 0)
        return(n);
   
    return(strcmp(s1++,s2++));
};

static KeyStore_KeyPair* CreateKeyPair(char *key, unsigned long value) {
    if (key == NULL)
        return NULL;

    KeyStore_KeyPair* kp = malloc(sizeof (KeyStore_KeyPair));

    short l = strlen(key);
    char *pad = malloc(l+1);
    kp->key = strncpy(pad, key, l);
    kp->value = value;
    kp->next = NULL;

    return (kp);
}

static long DestroyKeyPair(KeyStore_KeyPair *kp) {
    if (kp == NULL)
        return (0);

    long n = 0;

    KeyStore_KeyPair *p;

    while (kp != NULL) {
        p = kp;
        kp = kp->next;

        free(p->key);
        p->key = NULL;
        free(p);
        n++;
    }

    return (n);
}

KeyStore_KeyRing* KeyStore_Create(short maxkeylength) {
    KeyStore_KeyRing *kr;
    kr = malloc(sizeof (KeyStore_KeyRing));

    if (kr == NULL)
        return NULL;

    KeyStore_KeyPair** kp = malloc(sizeof (KeyStore_KeyPair*) *maxkeylength);
    
    for (int i = 0; i < maxkeylength; i++) {
        kp[i] = (KeyStore_KeyPair*) NULL;
    }

    kr->keychain = (void*)kp;
    kr->maxkeylength = maxkeylength;

    return (kr);
}

short KeyStore_Destroy(KeyStore_KeyRing *kr) {
    if (kr == NULL)
        return (FALSE);

    long n = kr->keys;

    for (short i = 0; i < kr->maxkeylength; i++) {
        n -= DestroyKeyPair((KeyStore_KeyPair*)kr->keychain[i]);
        kr->keychain[i] = NULL;
    }

    free(kr->keychain);
    free(kr);
    
    return (n == 0);
}

short KeyStore_SetKey(KeyStore_KeyRing *kr, char *key, unsigned long value) {
    if (kr == NULL || key == NULL)
        return (FALSE);

    short l = strlen(key);
    if (l == 0)
        return (FALSE);

    if (l > kr->maxkeylength)
        return (FALSE);

    KeyStore_KeyPair *kp, *last;
    kp = (KeyStore_KeyPair*)kr->keychain[l - 1];
    last = NULL;

    short r = 1;

    while (kp != NULL) {
        r = FastCompare(kp->key, key);
        if (r >= 0)
            break;
        last = kp;
        kp = kp->next;
    }

    if (r == 0) {
        kp->value = value;
        return (TRUE);
    }

    KeyStore_KeyPair *new = CreateKeyPair(key, value);
    if (new == NULL)
        return (FALSE);

    kr->keys++;

    if (r > 0) {
        if (last == NULL)
            kr->keychain[l - 1] = (void*)new;
        else
            last->next = new;

        new->next = kp;
        return (TRUE);
    }

    last->next = new;
    new->next = kp;

    return (TRUE);
}

short KeyStore_GetKey(KeyStore_KeyRing *kr, char *key, unsigned long *value) {
    if (kr == NULL || key == NULL)
        return (FALSE);

    short l = strlen(key);

    if (l == 0)
        return (FALSE);

    KeyStore_KeyPair *kp;
    kp = (KeyStore_KeyPair*) kr->keychain[l - 1];

    short r = 1;

    while (kp != NULL) {
        r = FastCompare(kp->key, key);
        if (r >= 0)
            break;

        kp = kp->next;
    }

    if (r != 0)
        return (FALSE);

    *value = kp->value;
    return (TRUE);
}

void KeyStore_EnumerateKeys(KeyStore_KeyRing *kr, KeyStore_Enumerator callback) {
    for (short i = 0; i < kr->maxkeylength; i++) {
        KeyStore_KeyPair *kp = (KeyStore_KeyPair*) kr->keychain[i];

        while (kp != NULL) {
            if (!(*callback)(kp->key, kp->value))
                return;

            kp = kp->next;
        }
    }
}

void KeyStore_Dump(KeyStore_KeyRing *kr) {
    if (kr == NULL)
        return;

    KeyStore_KeyPair* kp;

    printf("-----------------------------------------------------------------------------------\n");
    printf("(@%p) keys=%ld maxkeylength=%d\n", kr, kr->keys, (int) kr->maxkeylength);
    for (short i = 0; i < kr->maxkeylength; i++) {
        kp = (KeyStore_KeyPair*)kr->keychain[i];

        while (kp != NULL) {
            printf("(@%p) [%s] %lu (0x%08lx)\n", kp, kp->key, kp->value, kp->value);
            kp = kp->next;
        }
    }
    printf("-----------------------------------------------------------------------------------\n");
}