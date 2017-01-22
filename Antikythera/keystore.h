/*
 * 
 *    keystore.h
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

#ifndef KEYSTORE_H
#define KEYSTORE_H

typedef struct keystore_keyring {
    struct KeyStore_KeyPair**   keychain;
    short                       maxkeylength;
    long                        keys;
} KeyStore_KeyRing;

KeyStore_KeyRing* KeyStore_Create(short maxkeylength);
short KeyStore_Destroy(KeyStore_KeyRing *kr);

typedef short(*KeyStore_Enumerator)(char*, unsigned long);

short KeyStore_SetKey(KeyStore_KeyRing *kr, char *key, unsigned long value);
short KeyStore_GetKey(KeyStore_KeyRing *kr, char *key, unsigned long *value);
void KeyStore_EnumerateKeys(KeyStore_KeyRing *kr, KeyStore_Enumerator callback);
void KeyStore_Dump(KeyStore_KeyRing *kr);

#endif /* KEYSTORE_H */

