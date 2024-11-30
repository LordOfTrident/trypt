#include <stdio.h>  /* printf, fprintf, stderr, FILE, fopen, fclose, fseek */
#include <string.h> /* strerror */
#include <stdlib.h> /* size_t, EXIT_FAILURE */
#include <errno.h>  /* errno */
#include <string.h> /* uint8_t */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "cipher.h"

#define VERSION "1.0.0"

static char *argv0;

static void Usage(void) {
	printf("  _                 _   \n"
	       " | |_ _ _ _  _ _ __| |_ \n"
	       " |  _| '_| || | '_ \\  _|\n"
	       "  \\__|_|  \\_, | .__/\\__|\n"
	       "          |__/|_| \n"
	       "Linux file encrypting/decrypting tool\n"
	       "(Using AES 256 CFB)\n"
	       "\n"
	       "Source code: https://github.com/LordOfTrident/trypt\n"
	       "Usage:\n"
	       "  %s help | -h              Show this message\n"
	       "  %s version | -v           Show the version\n"
	       "  %s en <IN> <OUT> <KEY>    Encrypt file IN with KEY into file OUT\n"
	       "  %s de <IN> <OUT> <KEY>    Decrypt file IN with KEY into file OUT\n"
	       "\n"
	       "Keys must be either 16, 24 or 32 bytes long\n",
	       argv0, argv0, argv0, argv0);
}

static void Version(void) {
	printf("trypt version %s compiled on %s\n", VERSION, __DATE__);
}

static char *Shift(int *argc, char ***argv) {
	if (*argc == 0)
		return NULL;

	-- *argc;
	return *(*argv) ++;
}

static uint8_t *GenerateIv(void) {
	FILE *file = fopen("/dev/urandom", "rb");
	if (file == NULL)
		Die("Failed to open file: %s", strerror(errno));

	uint8_t *iv = malloc(IV_LEN);
	CheckAlloc(iv);

	if (fread(iv, 1, IV_LEN, file) < IV_LEN)
		Die("Failed to read file: %s", strerror(errno));

	fclose(file);
	return iv;
}

static bool IsRegularFile(const char *path) {
    struct stat fileStat;
    stat(path, &fileStat);
    return S_ISREG(fileStat.st_mode);
}

static uint8_t *ReadFile(const char *path, size_t *size) {
	size_t size_;
	if (size == NULL)
		size = &size_;

	if (!IsRegularFile(path))
		Die("Path \"%s\" is not a regular file", path);

	FILE *file = fopen(path, "rb");
	if (file == NULL)
		Die("Failed to open file \"%s\" for reading: %s", path, strerror(errno));

	fseek(file, 0, SEEK_END);
	*size = ftell(file);
	rewind(file);

	uint8_t *bytes = malloc(*size);
	CheckAlloc(bytes);

	if (fread(bytes, 1, *size, file) < *size)
		Die("Failed to read file: %s", strerror(errno));

	fclose(file);
	return bytes;
}

static void WriteFileFromChunks(const char *path, uint8_t **chunks, size_t *sizes, int count) {
	FILE *file = fopen(path, "wb");
	if (file == NULL)
		Die("Failed to open file \"%s\" for writing: %s", path, strerror(errno));

	for (int i = 0; i < count; ++ i) {
		for (size_t j = 0; j < sizes[i]; ++ j)
			fputc(chunks[i][j], file);
	}

	fclose(file);
}

static int CheckKeyLength(const char *key) {
	int len = strlen(key);
	if (len != 16 && len != 24 && len != 32)
		Die("Invalid key length %i: Expected 16, 32 or 64", len);

	return len;
}

static void EncryptFile(const char *inPath, const char *outPath, char *key) {
	int keyLen = CheckKeyLength(key);

	size_t   size;
	uint8_t *bytes     = ReadFile(inPath, &size),
	        *iv        = GenerateIv(),
	        *encrypted = Encrypt(bytes, size, key, keyLen, iv);
	free(bytes);

	WriteFileFromChunks(outPath, (uint8_t*[]){iv, encrypted}, (size_t[]){IV_LEN, size}, 2);
	free(encrypted);
	free(iv);
}

static void DecryptFile(const char *inPath, const char *outPath, char *key) {
	int keyLen = CheckKeyLength(key);

	size_t   size;
	uint8_t *bytes     = ReadFile(inPath, &size),
	        *decrypted = Decrypt(bytes + IV_LEN, size - IV_LEN, key, keyLen, bytes);
	free(bytes);

	WriteFileFromChunks(outPath, (uint8_t*[]){decrypted}, (size_t[]){size - IV_LEN}, 1);
	free(decrypted);
}

int main(int argc, char **argv) {
	argv0 = Shift(&argc, &argv);
	if (argc == 0) {
		Usage();
		return EXIT_FAILURE;
	}

	char *subcmd = Shift(&argc, &argv);
	if (strcmp(subcmd, "help") == 0 || strcmp(subcmd, "-h") == 0)
		Usage();
	else if (strcmp(subcmd, "version") == 0 || strcmp(subcmd, "-v") == 0)
		Version();
	else if (strcmp(subcmd, "en") == 0) {
		char *inPath  = Shift(&argc, &argv),
		     *outPath = Shift(&argc, &argv),
		     *key     = Shift(&argc, &argv);

		if (inPath == NULL)
			Die("Expected a path to a file to encrypt");
		if (outPath == NULL)
			Die("Expected a path to write the encrypted file into");
		if (key == NULL)
			Die("Expected a key for encryption");

		EncryptFile(inPath, outPath, key);
	} else if (strcmp(subcmd, "de") == 0) {
		char *inPath  = Shift(&argc, &argv),
		     *outPath = Shift(&argc, &argv),
		     *key     = Shift(&argc, &argv);

		if (inPath == NULL)
			Die("Expected a path to a file to decrypt");
		if (outPath == NULL)
			Die("Expected a path to write the decrypted file into");
		if (key == NULL)
			Die("Expected a key for decryption");

		DecryptFile(inPath, outPath, key);
	} else
		Die("Unknown subcommand \"%s\"", subcmd);

	return 0;
}
