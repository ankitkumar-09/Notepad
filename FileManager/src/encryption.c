#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

#define AES_KEYLEN 256
#define BLOCK_SIZE AES_BLOCK_SIZE

// Derive a 256-bit AES key from a password using SHA-256
void derive_key_from_password(const char *password, unsigned char *key) {
    SHA256((const unsigned char *)password, strlen(password), key);
}

// Encrypt file with AES (CBC mode + random IV)
int aes_encrypt_file(const char *infile, const char *outfile, const char *password) {
    FILE *in = fopen(infile, "rb");
    FILE *out = fopen(outfile, "wb");
    if (!in || !out) {
        printf("Error: cannot open file.\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }

    unsigned char key[32];
    unsigned char iv[BLOCK_SIZE];
    derive_key_from_password(password, key);
    RAND_bytes(iv, BLOCK_SIZE);

    // write IV at start of file
    fwrite(iv, 1, BLOCK_SIZE, out);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char inbuf[1024], outbuf[1040];
    int inlen, outlen;

    while ((inlen = fread(inbuf, 1, sizeof(inbuf), in)) > 0) {
        EVP_EncryptUpdate(ctx, outbuf, &outlen, inbuf, inlen);
        fwrite(outbuf, 1, outlen, out);
    }

    EVP_EncryptFinal_ex(ctx, outbuf, &outlen);
    fwrite(outbuf, 1, outlen, out);

    EVP_CIPHER_CTX_free(ctx);
    fclose(in);
    fclose(out);
    printf("File '%s' encrypted -> '%s'\n", infile, outfile);
    return 1;
}

// Decrypt file with AES (CBC mode)
int aes_decrypt_file(const char *infile, const char *outfile, const char *password) {
    FILE *in = fopen(infile, "rb");
    FILE *out = fopen(outfile, "wb");
    if (!in || !out) {
        printf("Error: cannot open file.\n");
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }

    unsigned char key[32];
    unsigned char iv[BLOCK_SIZE];
    derive_key_from_password(password, key);

    // read IV from start of file
    fread(iv, 1, BLOCK_SIZE, in);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char inbuf[1024], outbuf[1040];
    int inlen, outlen;

    while ((inlen = fread(inbuf, 1, sizeof(inbuf), in)) > 0) {
        if (!EVP_DecryptUpdate(ctx, outbuf, &outlen, inbuf, inlen)) {
            printf("Error during decryption (wrong password?)\n");
            EVP_CIPHER_CTX_free(ctx);
            fclose(in);
            fclose(out);
            return 0;
        }
        fwrite(outbuf, 1, outlen, out);
    }

    if (!EVP_DecryptFinal_ex(ctx, outbuf, &outlen)) {
        printf("Decryption failed! Possibly wrong password.\n");
        EVP_CIPHER_CTX_free(ctx);
        fclose(in);
        fclose(out);
        return 0;
    }
    fwrite(outbuf, 1, outlen, out);

    EVP_CIPHER_CTX_free(ctx);
    fclose(in);
    fclose(out);
    printf("File '%s' decrypted -> '%s'\n", infile, outfile);
    return 1;
}
