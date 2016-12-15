#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

using namespace std;

void write_file(char *content, char *file_name) {
    printf("writing into file %s",file_name);
    ofstream outf;
    outf.open(file_name);
    if (outf.is_open()) {
        outf << content;
    }
    outf.close();
}

char *read_file(char *file_name) {
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

void getSHA256(char *content, char *file_name) {


    int i = 0;
    SHA256_CTX ctx;
    char formated_result[64];
    unsigned char result[SHA256_DIGEST_LENGTH];
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, content, strlen(content));
    SHA256_Final(result, &ctx);

    printf("***************************************\n'%s'\n***************************************",content);
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(formated_result + i * 2, "%02x", result[i]);
    }
    printf("SHA256(\"%s\")=%s\n", file_name,formated_result);
    write_file(formated_result, file_name);
}

void getMD5(char *content, char *file_name) {
    int i = 0;
    MD5_CTX ctx;
    char formated_result[64];
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Init(&ctx);
    MD5_Update(&ctx, content, strlen(content));
    MD5_Final(result, &ctx);



    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(formated_result + i * 2, "%02x", result[i]);
    }
    
    printf("MD5(\"%s\")=%s\n", file_name, formated_result);
    write_file(formated_result, file_name);
}

int compare_file(char *file_path1, char *file_path2) {
    int count = 0;
    char* f1 = read_file(file_path1);
    char* f2 = read_file(file_path2);
    int n = strlen(f1);
    printf("\ncompare:\n %s vs %s \n",file_path1, file_path2);
    printf("f1=%s\n",f1);
    printf("f2=%s\n",f2);
    for (int i = 0; i < n; i+=2) {
        if (f1[i] == f2[i] && f1[i+1] == f2[i+1])
            count++;
    }
    printf("number of equal bytes are:%i\n", count);
    return count;
}

int *  hash_and_compare(char *file1, char *file2) {
    int *result = new int[2];
    char *file1_content;
    char *file2_content;
    file1_content = read_file((char *) file1);
    file2_content = read_file((char *) file2);
    printf("\nfile1: %s\n",file1);
    getSHA256(file1_content, (char *) "h1_sha256.out");
    getMD5(file1_content, (char *) "h1_md5.out");


    printf("\nfile2: %s\n",file2);
    getSHA256(file2_content, (char *) "h2_sha256.out");
    getMD5(file2_content, (char *) "h2_md5.out");

    result[0] = compare_file((char *) "h1_sha256.out", (char *) "h2_sha256.out");
    result[1] = compare_file((char *) "h1_md5.out", (char *) "h2_md5.out");
    return result;
}

int main() {
    int *result = new int[2];
    ofstream outf;
    outf.open("test_result.txt");
    for(int i = 0; i<=9; i++){
        char* file1 = (char*) malloc(50);
        char* file2 = (char*) malloc(50);

        
        sprintf(file1, "tests/%d/file1.in", i);
        sprintf(file2, "tests/%d/file2.in", i);
        // printf("%s vs %s\n", file1, file2);
        result = hash_and_compare(file1, file2);
        if(outf.is_open()){
            outf<<"test"<<i<<"\n";
            outf<<"\t"<<file1<<" vs "<<file2<<'\n';
            outf<<"\t"<<"SHA256 equal bytes : "<<result[0]<<'\n';
            outf<<"\t"<<"MD5    equal bytes : "<<result[1]<<'\n';
            outf<<"\n************************************\n";   
        }
        printf("%i,%i", result[0], result[1]);        
    }
    outf.close();
    return 0;
}