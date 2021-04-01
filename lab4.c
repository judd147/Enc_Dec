/**
 * This program reads a text file and a key for encryption or decryption. 
 * The file name and key are provided using command line arguments. If the arguments are not 
 * sufficient or the file is not readable, the program will exit and provide an error message.
 * The encrypted or decrypted messages will output to another file.
 * 
 * @author Liyao Zhang {@literal <zhanl217@wfu.edu>}
 * @date Oct. 11, 2020
 * @assignment Lab 4
 * @course CSC 250
 **/

#include "get_args.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* function declarations */
unsigned int encode(char* message, char key);
char* decode(unsigned int message, char key);
void encFile(char* file_name, int mykey);
void decFile(char* file_name, int mykey);

int main(int argc, char* argv[])
{
    int enc = 0;
    int mykey = 0;
    int* enc_ptr = &enc;
    int* key_ptr = &mykey;
    char file_name[20];
    process_args(argc, argv, enc_ptr, key_ptr, file_name);
    if(enc == 1){
        encFile(file_name, mykey);
    }
    else if(enc == 0){
        decFile(file_name, mykey);
    }
    return 0;
}

unsigned int encode(char* message, char key)
{
    int i;
    unsigned int sum = 0;
    /* bitwise opertations */
    for(i = 0; i < strlen(message); i++){
        unsigned int temp;
        int k = 24-i*8;
        message[i] = message[i] ^ key;
        temp = message[i]<<k;
        sum += temp;
    }
    return sum;
}

char* decode(unsigned int message, char key)
{
    int i;
    char* hex = malloc(4*sizeof(char));
    unsigned int temp1 = message>>24; /*39*/
    unsigned int temp2 = message>>16; /*393a*/
    unsigned int temp11 = temp1<<8; /*3900*/
    unsigned int ans2 = temp2 - temp11; /*3a*/
    unsigned int temp3 = message>>8; /*393a3b*/
    unsigned int temp22 = temp2<<8; /*393a00*/
    unsigned int ans3 = temp3 - temp22; /*3b*/
    unsigned int temp33 = temp3<<8;/*393a3b00*/
    unsigned int ans4 = message - temp33; /*3c*/
    hex[0] = temp1;
    hex[1] = ans2;
    hex[2] = ans3;
    hex[3] = ans4;
    
    for(i = 0; i < 4; i++){
        hex[i] = hex[i] ^ key;
    }
    return hex;
}

void encFile(char* file_name, int mykey)
{
    FILE *file_ptr;
    FILE *out_ptr;
    char* suffix = ".enc"; 
    char output[24];
    char message[4];
    char ch;
    unsigned int sum;
    int count = 0;
    int index = 0;
    
    strcpy(output, file_name);
    strcat(output, suffix);
    
    file_ptr = fopen(file_name, "r");
    out_ptr = fopen(output, "w");
    if(!file_ptr){
        printf("Error -- Couldn't open file\n");
        exit(-1);
    }
    while((ch = fgetc(file_ptr)) != EOF){
        message[index] = ch;
        count++;
        index++;
        if(index == 4){
            index = 0;
            sum = encode(message, mykey);
            fprintf(out_ptr, "%d\n", sum);
        }
    }
    fclose(file_ptr);
    fclose(out_ptr);
    printf("encryped %d characters in %s\n", count, file_name);
    printf("encryped file is %s\n", output);
}

void decFile(char* file_name, int mykey)
{
    FILE *file_ptr;
    FILE *out_ptr;
    char* suffix = ".dec"; 
    char output[24];
    char* temp;
    int ch;
    int count = 0;
    
    strcpy(output, file_name);
    strcat(output, suffix);
    
    file_ptr = fopen(file_name, "r");
    out_ptr = fopen(output, "w");
    if(!file_ptr){
        printf("Error -- Couldn't open file\n");
        exit(-1);
    }
    /* read an integer at a time */
    while(fscanf(file_ptr, "%d", &ch) != EOF){
        temp = decode(ch, mykey);
        count++;
        fprintf(out_ptr, "%s\n", temp);
        free(temp);
    }
    fclose(file_ptr);
    fclose(out_ptr);
    printf("decrypted %d characters in %s\n", 4*count, file_name);
    printf("decrypted file is %s\n", output);
}
