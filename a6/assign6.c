#include <stdio.h>
#include <openssl/bn.h>
#include <string.h>

void printBN(char *msg, BIGNUM * a) {
  /* Use BN_bn2hex(a) for hex string
   * Use BN_bn2dec(a) for decimal string */
  char * number_str = BN_bn2hex(a);
  printf("%s %s\n", msg, number_str);
  OPENSSL_free(number_str);
}

int main() {

	// Set up n.
	BIGNUM* n = BN_new();
	BN_hex2bn(&n, "D018CF45D48BCDD39CE440EF7EB4DD69211BC9CF3C8E4C75B90F3119843D9E3C29EF500D10936F0580809F2AA0BD124B02E13D9F581624FE309F0B747755931D4BF74DE1928210F651AC0CC3B222940F346B981049E70B9D8339DD20C61C2DEFD1186165E7238320A82312FFD2247FD42FE7446A5B4DD75066B0AF9E426305FBE01CC46361AF9F6A33FF6297BD48D9D37C1467DC75DC2E69E8F86D7869D0B71005B8F131C23B24FD1A3374F823E0EC6B198A16C6E3CDA4CD0BDBB3A4596038883BAD1DB9C68CA7531BFCBCD9A4ABBCDD3C61D7931598EE81BD8FE264472040064ED7AC97E8B9C05912A1492523E4ED70342CA5B4637CF9A33D83D1CD6D24AC07");
	//printBN("n = ", n);

	// Set up e.
	BIGNUM* e = BN_new();
	BN_hex2bn(&e, "10001");
	//printBN("e = ", e);

	// Set up signature.
	BIGNUM* signature = BN_new();
	BN_hex2bn(&signature, "47742b042136cb85fe15de61e01ccc597d4f2606a64c63549a259ac90f645b5b1000b0a9c8629a2947630af408ecf42b8ce04606d5e2a3f9c775ee97d228f8555d79f250b21142f536f36caca4a90403737f2beba33b4d92bb96b849d2aad753d82526eed2c8c607eb1c5b9bc7db3502c2f96e7693f6a877dfdcc3d5cec002099a4db3a1a7d5092a01af9cf33a6ecd9adb1c61997e53bb6b7f5c1e5c3f7bb7ba0c2f69f933fdd0c58c715f6447fb31166b15c667b69882e49d96478776b8f619bc3abd0b379efdef18c7500adeb9ae4b4348aa3634bc437ab931080d2dcceec6f54de80a45f2ee86bb16656715c9473574b29bca8ff95e0be92152c81454ba73");
	//printBN("signature = ", signature);
	//printf("\n");

	// Set up computation to retrieve original signature (decryption).
	BN_CTX *ctx = BN_CTX_new();
	BIGNUM* original = BN_new();
	BN_mod_exp(original, signature, e, n, ctx);
	BN_CTX_free(ctx);
	printBN("Decrypted hash:\n", original);
	printf("\n");

	// Remove padding on original hash (padding explained here: https://tools.ietf.org/html/rfc2437#page-18).
	char* original_hash = BN_bn2hex(original);
	original_hash += 446;
	char* computed_hash = "3E43944E8C49CB31351E205E03361C03B9E9C16E9C8BF9860E8F6AAE54DEE138";

	// Determines if the signature hashes match.
	if (strcmp(computed_hash, original_hash) == 0) {
		printf("Signature verified. Valid certificate.\n");
	} else {
		printf("Signatures do not match. Invalid certificate.\n");
	}

	BN_hex2bn(&original, original_hash);

	printBN("Decrypted hash cleaned up:\n", original);
	printf("OpenSSL computed hash:\n");
	printf(" %s\n", computed_hash);
}