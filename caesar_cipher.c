#include <stdio.h>
#include <ctype.h>
#include <string.h>

void encipher(const char *p, char *c, unsigned int offset) {
    int i = 0;
    while (p[i] != '\0') {
        char ch = p[i];
        if (isalpha((unsigned char)ch)) {
            char base = isupper((unsigned char)ch) ? 'A' : 'a';
            c[i] = (char)(((ch - base + offset) % 26) + base);
        } else {
            c[i] = ch;
        }
        i++;
    }
    c[i] = '\0';
}

void decipher(const char *c, char *p, unsigned int offset) {
    int i = 0;
    while (c[i] != '\0') {
        char ch = c[i];
        if (isalpha((unsigned char)ch)) {
            char base = isupper((unsigned char)ch) ? 'A' : 'a';
            p[i] = (char)(((ch - base - offset + 26) % 26) + base);
        } else {
            p[i] = ch;
        }
        i++;
    }
    p[i] = '\0';
}

unsigned int autodetect_offset(const char *cipher, char *decoded) {
    char test[200];
    double best_score = -1.0;
    unsigned int best_offset = 0;

    const double english_freq[26] = {
        8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.966,0.153,
        0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,
        2.758,0.978,2.360,0.150,1.974,0.074
    };

    for (unsigned int shift = 0; shift < 26; ++shift) {
        decipher(cipher, test, shift);
        int hist[26] = {0};
        for (int i = 0; test[i] != '\0'; ++i) {
            unsigned char ch = (unsigned char)test[i];
            if (isalpha(ch))
                hist[toupper(ch) - 'A']++;
        }

        double score = 0.0;
        for (int i = 0; i < 26; ++i)
            score += hist[i] * english_freq[i];

        if (score > best_score) {
            best_score = score;
            best_offset = shift;
            strcpy(decoded, test);
        }
    }
    return best_offset;
}

int main(void) {
    const char plaintext[] = "This is lowkey fun";
    char ciphertext[200];
    char decoded[200];
    unsigned int offset = 5;

    encipher(plaintext, ciphertext, offset);
    unsigned int guessed_offset = autodetect_offset(ciphertext, decoded);

    printf("Plain text:  %s\n", plaintext);
    printf("Cipher text: %s\n", ciphertext);
    printf("Guessed offset: %u\n", guessed_offset);
    printf("Deciphered:  %s\n", decoded);

    return 0;
}
