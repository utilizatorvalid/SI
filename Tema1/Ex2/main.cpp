#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <openssl/sha.h>
#include <openssl/md5.h>

using namespace std;

void write_file(char *content, char *file_name) {
    cout << "writing into file" << file_name;
    ofstream outf;
    outf.open(file_name);
    if (outf.is_open()) {
        outf << content;
    }
    outf.close();
}

char *read_file(char *file_name) {

    string content;
    string line;
    ifstream myfile(file_name);
    if (myfile.is_open()) {

        while (getline(myfile, line))
            content += line;

//        content[content.length()-1]="\0";
        myfile.close();
    }
    return (char *) content.c_str();
}

void getSHA256(char *content, char *file_name) {


    int i = 0;
    SHA256_CTX ctx;
    char formated_result[64];
    unsigned char result[SHA256_DIGEST_LENGTH];
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, content, strlen(content));
    SHA256_Final(result, &ctx);

    printf("SHA256(\"%s\")=\n", content);
    cout << SHA256_DIGEST_LENGTH;

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(formated_result + i * 2, "%02x", result[i]);
    }
    cout << "result:" << formated_result << "\n";
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

    printf("MD5(\"%s\")=\n", content);
    cout << MD5_DIGEST_LENGTH;

    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(formated_result + i * 2, "%02x", result[i]);
    }
    cout << "result:" << formated_result << "\n";
    write_file(formated_result, file_name);
}

int compare_file(char *file_path1, char *file_path2) {
    int count = 0;
    string f1(read_file(file_path1));
    string f2(read_file(file_path2));
    int n = f1.length();
    cout << "\ncompare:\n" << file_path1 << " vs " << file_path2 << '\n';
    cout << "f1=" << f1 << '\n';
    cout << "f2=" << f2 << '\n';
//    cout << n << '\n';
    for (int i = 0; i < n; i++) {
        if (f1[i] == f2[i])
            count++;
    }
    printf("number of equal bytes are:%i\n", count);
    return count;
}

void hash_and_compare(char *file1, char *file2) {
    char *file1_content;
    char *file2_content;
    file1_content = read_file((char *) file1);
    file2_content = read_file((char *) file2);

    getSHA256(file1_content, (char *) "h1_sha256.out");
    getSHA256(file2_content, (char *) "h2_sha256.out");

    getMD5(file1_content, (char *) "h1_md5.out");
    getMD5(file2_content, (char *) "h2_md5.out");
    compare_file((char *) "h1_sha256.out", (char *) "h2_sha256.out");
    compare_file((char *) "h1_md5.out", (char *) "h2_md5.out");
}

int main() {
    hash_and_compare((char *) "file1.in", (char *) "file2.in");
    return 0;
}