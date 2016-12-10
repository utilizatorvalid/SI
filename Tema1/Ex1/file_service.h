//
// Created by l on 11/29/16.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include <vector>
#include <string>
#ifndef EX1_FILE_SERVICE_H
#define EX1_FILE_SERVICE_H

#endif //EX1_FILE_SERVICE_H
using namespace std;
void trim (char *word) {
    /*
    eliminate last newLine from string
    */
    if (word[strlen(word) - 1] == '\n') {
        word[strlen(word) - 1] = '\0';
    }
}

bool file_exists(char* fname){
    /*
    this function checking if file exitst or not
    True if there is such a file
    False otherwise
    */
    if (FILE *file = fopen(fname, "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}
char* read_file(char *file_name) {
    /*
    return char*  to the file_content
    */
    FILE *f = fopen(file_name, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *file_content = (char *) malloc(fsize + 1);
    fread(file_content, fsize, 1, f);
    fclose(f);
    return file_content;

}
void write_to_file(char *file_name, char* data){
    /*
    this function withe to the file data 
    */
    FILE *file = fopen(file_name,"w");
    fprintf(file, "%s", (const char *)data);
    fclose(file);
}

vector <string> read_dictionary(char* file_name){
    /*
    this function read file and return vector with words 
    */
    vector<string> word_dictionary;
    FILE *dictionary = fopen(file_name, "r");
    if (dictionary == NULL) {
        perror("Failed to open the dictionary");
        exit(EXIT_FAILURE);
    }
    char * word=(char*)malloc(30);
    size_t word_len=0;
    size_t read;
    while((read = getline(&word, &word_len,dictionary))!=-1){
        trim(word);
        word_dictionary.push_back(string(word));

    }
    fclose(dictionary);
    return word_dictionary;


}
char *complete_to_16(char buffer[]) {
    /*
        this function complete to16 bytes word containd in buffet with \x20
    */
    int len = strlen(buffer);
    char *key = (char *) malloc(30);
    strcpy(key, buffer);
    for (; len < 16; len++) {
        strcat(key, "\x20");
    }
    key[len] = '\0';
    return key;
}

