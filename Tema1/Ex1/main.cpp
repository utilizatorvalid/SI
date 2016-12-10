
#include "file_service.h"
#include "over_AES_service.h"
#include <vector>
#include <string>
using namespace std;

bool validate_input(char* file_in, char *file_out, char *mode){
    if (!file_exists(file_in)){
        perror(file_in);
        return false;
    }
    if (!file_exists(file_in)){
        perror(file_in);
        return false;
    }
    if (strcmp(mode, "ECB") != 0 && strcmp(mode, "CBC") != 0) {
        perror("Chosen encryption mode is not supported yet");
        return false;
    }
    return true;
}

int main() {
        char    *file_in   = (char *) malloc(50),
                *file_out  = (char *) malloc(50),
                *mode      = (char *) malloc(10);
        printf("Welcome to key founder runnable");
        printf("\n Give me some inputs:\n\t1.PlainFile\n\t2.CipherFile\n\t3.Encryption Mode (ECB/CBC)\n");
            printf("1>>");
            scanf("%s",file_in);
            printf("2>>");
            scanf("%s",file_out);
            printf("3>>");
            scanf("%s",mode);
        trim(file_in);
        trim(file_out);
        trim(mode);
        bool valid_input = validate_input(file_in, file_out, mode);
        if(not valid_input){
            printf("Try again and give some valid inputs\n");
            exit(0);
        }

        char* plaintext_f = read_file((char *) file_in);
        char* ciphertext_f = read_file((char *) file_out);

        printf("plaintext is:`%s`\n",plaintext_f);
        printf("ciphertext is:`%s`\n",ciphertext_f);

        vector<string> dictionary = read_dictionary((char*)"word_dict.txt");
        int number_of_iteration=0;
        for(int index=0; index<dictionary.size();index++){
            char* key = (char*)malloc(30);
            key = (char*)dictionary[index].c_str();
            key = complete_to_16(key);
//        printf("%s|%d\n",key,strlen(key));
            unsigned char * ciphertext;
            int plain_text_length = strlen(plaintext_f);
            if(strcmp(mode,"ECB")==0)
                ciphertext = encrypt_aes_ecb((unsigned char*)plaintext_f,
                                             &plain_text_length,
                                             (unsigned char*)key);
            if(strcmp(mode,"CBC")==0)
                ciphertext = encrypt_aes_cbc((unsigned char*)plaintext_f,
                                             &plain_text_length,
                                             (unsigned char*)key);
//            printf("%s|`%s`|`%s`\n",key,plaintext_f,ciphertext);
            number_of_iteration++;
            if(strcmp((const char*)ciphertext, ciphertext_f)==0){
                printf("After  %d tested keys, The correct one was found:%s\n",
                       number_of_iteration,
                       key);

                exit(0);
            }
        }
        printf("After  %d tested keys, The correct one was not found Sorry :( :(\n"
                ,number_of_iteration);
        return 0;
    }
