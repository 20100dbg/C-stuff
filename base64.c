#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 1024
char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char* input;

char* encode(char* buf, int len);
char* decode(char* buf, int len);
int get_index(char c);
void print_help();


int main(int argc, char **argv)
{
    if (argc == 1) {
        print_help();
        return 0;
    }

    int c;
    char* input;
    char* output;
    int opt_newline = 1;
    int opt_encode = 0;
    int opt_decode = 0;


    while ((c = getopt (argc, argv, ":d:e:nh")) != -1) {

        switch (c)
        {

          case 'e': 
            opt_encode = 1;
            input = optarg;
            break;

          case 'd':
            opt_decode = 1;
            input = optarg;
            break;
          
          case ':':
            input = malloc(MAX_LEN);
            fgets(input, MAX_LEN, stdin);

            switch (optopt) {
                case 'e': opt_encode = 1; break;
                case 'd': opt_decode = 1; break;
            }
            break;

          case 'n': 
            opt_newline = 0;
            break;

          case 'h':
            print_help();
            break;

          default:
            abort();
        }
    }

    if (opt_encode) {
        output = encode(input, strlen(input));
    }
    else if (opt_decode) {
        output = decode(input, strlen(input));   
    }

    if (opt_encode || opt_decode) {
        printf("%s", output);
        if (opt_newline) printf("\n");
        free(output);
    }

    return 0;
}

void print_help() {
    printf("Base64 encoder/decoder\n\n");
    printf("-e plaintext\tEncode plaintext to base64\n");
    printf("-d encoded\tDecode base64 to plaintext\n");
    printf("-n\t\tdo not output ending newline\n");
    printf("-h\t\tShow this help\n");
}


char* encode(char* input, int len)
{
    char* output = malloc(MAX_LEN);
    unsigned char tmp[4];
    int idx = 0;
    int remainder = (len % 3) > 0 ? 3 - (len % 3) : 0;

    //pad string with \0 if not enough chars
    while (len % 3 != 0) {
        input[len++] = '\0';
    }
    
    //encode
    for (int i = 0; i < len; i+=3)
    {
        output[idx++] = b64_table[(0b11111100 & input[i]) >> 2];
        output[idx++] = b64_table[(0b00000011 & input[i]) << 4 | (0b11110000 & input[i+1]) >> 4];            
        output[idx++] = b64_table[(0b00001111 & input[i+1]) << 2 | (0b11000000 & input[i+2]) >> 6];
        output[idx++] = b64_table[0b00111111 & input[i+2]];
    }

    //pad with = sign
    for (int i = idx - remainder; i < idx; i++) {
        output[i] = '=';
    }

    output[idx] = '\0';
    return output;
}


//return char position in base64 alphabet, 0 if not found
int get_index(char c)
{
    for (int i = 0; i < 64; i++) {
        if (c == b64_table[i]) return i;
    }
    return 0;
}

char* decode(char* input, int len)
{
    char *output = malloc(MAX_LEN);
    int idx = 0;

    //clean/prepare end of the string
    for (int i = 0; i < len; i++)
    {
        if (input[i] == '=') {
            input[i] = '\0';
            len--;
        }
    }

    //decode
    for (int i = 0; i < len; i += 4)
    {
        output[idx++] = get_index(input[i]) << 2 | (0b00110000 & get_index(input[i+1])) >> 4;
        output[idx++] = (0b00001111 & get_index(input[i+1])) << 4 | (0b00111100 & get_index(input[i+2])) >> 2;
        output[idx++] = (0b00000011 & get_index(input[i+2])) << 6 | get_index(input[i+3]);
    }

    output[idx] = '\0';
    return output;

}