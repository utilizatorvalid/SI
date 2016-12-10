//
// Created by l on 11/29/16.
//
#include <openssl/evp.h>
#include <openssl/conf.h>

#ifndef EX1_OVER_AES_SERVICE_H
#define EX1_OVER_AES_SERVICE_H

#endif //EX1_OVER_AES_SERVICE_H

unsigned char initial_vector[] = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

unsigned char *encrypt_aes_cbc(unsigned char *plaintext, int *len, unsigned char *key) {
    /*
        This function gets:
        plaintext:  unsigned char pointer to the plaintext
        len:        int pointer to the lehth of the plaintext
        key:        unsigned char pointer to the key
        return:     return unsigned char* to the chipher text
    */

    //init encryptor for cbc mode with initial vector
    EVP_CIPHER_CTX *encriptor = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(encriptor, EVP_aes_128_cbc(), NULL, key, initial_vector);
    int c_len = *len + 128;
    int f_len = 0;
    //pointer to the ciphertext
    unsigned char *ciphertext = (unsigned char *) malloc(c_len);
    
    //update and final plaintext and getting ciphertext from EVP encryption function    
    EVP_EncryptUpdate(encriptor, ciphertext, &c_len, plaintext, *len);
    EVP_EncryptFinal_ex(encriptor, ciphertext + c_len, &f_len);


    *len = c_len + f_len;
    return ciphertext;
}

unsigned char *encrypt_aes_ecb(unsigned char *plaintext, int *len, unsigned char *key) {
     /*
        This function gets:
        plaintext:  unsigned char pointer to the plaintext
        len:        int pointer to the lehth of the plaintext
        key:        unsigned char pointer to the key
        return:     return unsigned char* to the chipher text
    */

    //init encryptor for ecv mode 
    EVP_CIPHER_CTX *encriptor = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(encriptor, EVP_aes_128_ecb(), NULL, key, NULL);
    int c_len = *len + 128;
    int f_len = 0;

    //pointer to the ciphertext
    unsigned char *ciphertext = (unsigned char *) malloc(c_len);

    //update and final plaintext and getting ciphertext from EVP encryption function  
    EVP_EncryptUpdate(encriptor, ciphertext, &c_len, plaintext, *len);
    EVP_EncryptFinal_ex(encriptor, ciphertext + c_len, &f_len);

    *len = c_len + f_len;
    return ciphertext;
}

unsigned char *decrypt_aes_cbc(unsigned char *ciphertext, int *len, unsigned char *key) {
     /*
        This function gets:
        plaintext:  unsigned char pointer to the ciphertext
        len:        int pointer to the lehth of the ciphertext
        key:        unsigned char pointer to the key
        return:     return unsigned char* to the plaintext
    */

    //init decryptor for cbc mode with initial vector
    EVP_CIPHER_CTX *decryptor = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(decryptor, EVP_aes_128_cbc(), NULL, key, initial_vector);

    int p_len = *len;
    int f_len = 0;

    //pointer to the plaintext to decrypt in
    unsigned char *plaintext = (unsigned char *) malloc(p_len);
    EVP_DecryptUpdate(decryptor, plaintext, &p_len, ciphertext, *len);
    EVP_DecryptFinal_ex(decryptor, plaintext + p_len, &f_len);
    *len = p_len + f_len;
    return plaintext;
}

unsigned char *decrypt_aes_ecb(unsigned char *ciphertext, int *len, unsigned char *key) {
     /*
        This function gets:
        plaintext:  unsigned char pointer to the ciphertext
        len:        int pointer to the lehth of the ciphertext
        key:        unsigned char pointer to the key
        return:     return unsigned char* to the plaintext
    */

    //init decryptor for ecb mode
    EVP_CIPHER_CTX *decryptor = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(decryptor, EVP_aes_128_ecb(), NULL, key, NULL);

    int p_len = *len;
    int f_len = 0;

    // pointer to the plaintext to decrypt in
    unsigned char *plaintext = (unsigned char *) malloc(p_len);
    EVP_DecryptUpdate(decryptor, plaintext, &p_len, ciphertext, *len);
    EVP_DecryptFinal_ex(decryptor, plaintext + p_len, &f_len);
    *len = p_len + f_len;
    return plaintext;
}