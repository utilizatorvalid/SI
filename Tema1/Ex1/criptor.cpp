
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "file_service.h"
#include "over_AES_service.h"
#include <time.h>
#include <string>

using namespace std;

bool validate_input(char *file_in, char *file_out, char *mode) {
    if (!file_exists(file_in)) {
        perror(file_in);
        return false;
    }
    if (strcmp(mode, "ECB") != 0 && strcmp(mode, "CBC") != 0) {
        perror("Your encryption mode is not suported yet");
        return false;
    }
    return true;
}

int main() {
    vector <string> dictionary = read_dictionary((char *) "word_dict.txt");
    char *response = (char *) malloc(10),
            *file_in = (char *) malloc(50),
            *file_out = (char *) malloc(50),
            *mode = (char *) malloc(10), *word;// = (char *) dictionary[random_index].c_str();

    while (true) {
        srand(time(NULL));
        int random_index = rand() % dictionary.size();
        // printf("Index of key is:%d", random_index);
        word = (char *) dictionary[random_index].c_str();
        printf("So you want to encrypt something huh?[Y/N]>>");
        scanf("%s", response);
        bool want_to_ecrypt = true;
        if (strcmp(response, (char *) "Y") == 0 || strcmp(response, (char *) "y") == 0)
            want_to_ecrypt = true;
        if (strcmp(response, (char *) "N") == 0 || strcmp(response, (char *) "n") == 0)
            want_to_ecrypt = false;
        if (!want_to_ecrypt)
            break;
        printf("\n Then give me some inputs:\n\t1.InputFile\n\t2.OutputFile\n\t3.Encryption Mode (ECB/CBC)\n");

        printf("1>>");
        scanf("%s", file_in);
        printf("2>>");
        scanf("%s", file_out);
        printf("3>>");
        scanf("%s", mode);

        trim(file_in);
        trim(file_out);
        trim(mode);
        bool valid_input = validate_input(file_in, file_out, mode);
        if (not valid_input) {
            printf("Try again and give some valid inputs\n");
            continue;
        }

        char *plaintext = read_file(file_in);
        int text_length = strlen(plaintext);


        unsigned char *key_data;
        key_data = (unsigned char *) complete_to_16(word);



        printf("key:'%s'\n", key_data);
        unsigned char *ciphertext;
        if (strcmp(mode, "ECB") == 0)
            ciphertext = encrypt_aes_ecb((unsigned char *) plaintext, &text_length, key_data);
        if (strcmp(mode, "CBC") == 0)
            ciphertext = encrypt_aes_cbc((unsigned char *) plaintext, &text_length, key_data);

        FILE *file = fopen(file_out, "w");
        fprintf(file, "%s", (const char *) ciphertext);
        fclose(file);
        printf("####file:%s==>%s was encrypted successfully with key:%s\n", file_in, file_out, key_data);
    }

    printf("See you again\n");

    return 0;
}