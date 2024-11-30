#include "cipher.h"

static void Prepare(gcry_cipher_hd_t *hd, char *key, int keyLen, uint8_t *iv) {
	gcry_error_t err;

	err = gcry_cipher_open(hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CFB, 0);
	if (err)
		Fatal("gcry_cipher_open(): %s", gcry_strerror(err));

	err = gcry_cipher_setkey(*hd, key, keyLen);
	if (err)
		Fatal("gcry_cipher_setkey(): %s", gcry_strerror(err));

	err = gcry_cipher_setiv(*hd, iv, IV_LEN);
	if (err)
		Fatal("gcry_cipher_setiv(): %s", gcry_strerror(err));
}

uint8_t *Encrypt(uint8_t *msg, size_t size, char *key, int keyLen, uint8_t *iv) {
	gcry_error_t     err;
	gcry_cipher_hd_t hd;
	Prepare(&hd, key, keyLen, iv);

	uint8_t *cipher = malloc(size);
	CheckAlloc(cipher);

	err = gcry_cipher_encrypt(hd, cipher, size, msg, size);
	if (err)
		Fatal("gcry_cipher_encrypt(): %s", gcry_strerror(err));

	gcry_cipher_close(hd);
	return cipher;
}

uint8_t *Decrypt(uint8_t *cipher, size_t size, char *key, int keyLen, uint8_t *iv) {
	gcry_error_t     err;
	gcry_cipher_hd_t hd;
	Prepare(&hd, key, keyLen, iv);

	uint8_t *msg = malloc(size);
	CheckAlloc(msg);

	err = gcry_cipher_decrypt(hd, msg, size, cipher, size);
	if (err)
		Fatal("gcry_cipher_decrypt(): %s", gcry_strerror(err));

	gcry_cipher_close(hd);
	return msg;
}
