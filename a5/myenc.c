#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

// [1] https://rosettacode.org/wiki/Repeat_a_string#C
char * string_repeat( int n, const char * s ) {
  size_t slen = strlen(s);
  char * dest = malloc(n*slen+1);
 
  int i; char * p;
  for ( i=0, p = dest; i < n; ++i, p += slen ) {
    memcpy(p, s, slen);
  }
  *p = '\0';
  return dest;
}

int main() {

    //unsigned char *iv = (unsigned char*)"\xaa\xbb\xcc\xdd\xee\xff\x00\x99\x88\x77\x66\x55\x44\x33\x22\x11";
    //unsigned char *ciphertext = (unsigned char*)"\x76\x4a\xa2\x6b\x55\xa4\xda\x65\x4d\xf6\xb1\x9e\x4b\xce\x00\xf4\xed\x05\xe0\x93\x46\xfb\x0e\x76\x25\x83\xcb\x7d\xa2\xac\x93\xa2";
    unsigned char iv[16] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
    unsigned char ciphertext[32] = {0x76, 0x4a, 0xa2, 0x6b, 0x55, 0xa4, 0xda, 0x65, 0x4d, 0xf6, 0xb1, 0x9e, 0x4b, 0xce, 0x00, 0xf4, 0xed, 0x05, 0xe0, 0x93, 0x46, 0xfb, 0x0e, 0x76, 0x25, 0x83, 0xcb, 0x7d, 0xa2, 0xac, 0x93, 0xa2};
    unsigned char key[16];
    char plaintext[32] = "This is a top secret.";
    char line[16];
    unsigned char outbuf[1024];
    int outlen, len = 0, i = 0, j = 0, count = 0;

    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);

    FILE *input;
    char* filename = "words.txt";
    input = fopen(filename, "r");

    if (input == NULL) {
        fprintf(stderr, "Error: failed to open specified file.\n");
        exit(EXIT_FAILURE);
    }

    printf("Opened file.\n");

    while (fgets(line, sizeof(line), input)) {

        //printf("line %d: %s", i, line);

        line[strcspn(line, "\n")] = 0;
        len = strlen(line);

        strcpy(key, line);

        if (len < 16) {
            int num_pad = 16 - len;
            char* padding = string_repeat(num_pad, "#");
            strcat(key, padding);
            //printf("current key: %s\n", key);
        }

        EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, 1);
        OPENSSL_assert(EVP_CIPHER_CTX_key_length(&ctx) == 16);
        OPENSSL_assert(EVP_CIPHER_CTX_iv_length(&ctx) == 16);
        EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, 1); 

        if(!EVP_CipherUpdate(&ctx, outbuf, &outlen, plaintext, strlen(plaintext))) {
            EVP_CIPHER_CTX_cleanup(&ctx);
            return 0;
        }

        if(!EVP_CipherFinal_ex(&ctx, outbuf+16, &outlen)) {
            EVP_CIPHER_CTX_cleanup(&ctx);
            return 0;
        }

        for (j = 0; j < 32; j++) {
            if (ciphertext[j] == outbuf[j]) {
                count++;
            }
        }

        if (count == 32) {
            key[strcspn(key, "#")] = 0;
            
            printf("Expected ciphertext: \n");
            for (i = 0; i < 32; i++) {
                printf("%0X", ciphertext[i]);
            }
            printf("\n");
            printf("Actual ciphertext: \n");
            for (i = 0; i < 32; i++) {
                printf("%0X", outbuf[i]);
            }
            printf("\n\n");
                
            printf("The key is: %s\n", key);
            break;

        }
        i++;
        j = 0;
        count = 0;
    }    

    EVP_CIPHER_CTX_cleanup(&ctx);
    fclose(input);
    return 0;
}