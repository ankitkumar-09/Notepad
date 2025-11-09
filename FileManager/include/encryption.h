#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#ifdef __cplusplus
extern "C" {
#endif

void derive_key_from_password(const char *password, unsigned char *key);
int aes_encrypt_file(const char *infile, const char *outfile, const char *password);
int aes_decrypt_file(const char *infile, const char *outfile, const char *password);

#ifdef __cplusplus
}
#endif

#endif
