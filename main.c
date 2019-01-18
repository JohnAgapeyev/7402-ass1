#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* common_letters = "etaoinshrdlucmfwyp";

static uint64_t letter_frequency_arr[26];

unsigned long plaintext_frequency(const unsigned char* input, const size_t len) {
    unsigned long score = 0;
    for (size_t i = 0; i < strlen(common_letters); ++i) {
        for (size_t j = 0; j < len; ++j) {
            if (tolower(input[j]) == common_letters[i]) {
                ++score;
            }
        }
    }
    return score;
}

void letter_frequency(const unsigned char* input, const size_t len) {
    memset(letter_frequency_arr, 0, sizeof(uint64_t) * 26);
    for (size_t i = 0; i < len; ++i) {
        if (isalpha(input[i])) {
            letter_frequency_arr[tolower(input[i]) - 'a']++;
        }
    }
}

unsigned char* caesar_cipher(
        const unsigned char* input, const size_t len, const unsigned char key) {
    unsigned char* ciphertext = malloc(len);
    for (size_t i = 0; i < len; ++i) {
        if (isalpha(input[i])) {
            ciphertext[i] = (tolower(input[i]) + key) % 26;
        }
    }
    return ciphertext;
}

size_t file_len(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    size_t sz = ftell(fp);
    rewind(fp);
    return sz;
}

int cmp(const void* lhs, const void* rhs) {
    uint64_t x, y;
    memcpy(&x, lhs, sizeof(uint64_t));
    memcpy(&y, rhs, sizeof(uint64_t));
    return x < y;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Needs a filename\n");
        return 1;
    }
    FILE* fp;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        perror("InFile");
        return 1;
    }

    size_t data_len = file_len(fp);

    unsigned char* data = malloc(data_len);

    fread(data, 1, data_len, fp);

    printf("Frequency score: %zu\n", plaintext_frequency(data, data_len));

    uint64_t tmp[26];

    letter_frequency(data, data_len);

    memcpy(tmp, letter_frequency_arr, sizeof(uint64_t) * 26);

    qsort(tmp, 26, sizeof(uint64_t), cmp);

    char* command = calloc(1ul << 20, 1);

    strcat(command, "python3 graph.py ");

    for (int i = 0; i < 26; ++i) {
        printf("%c: %lu\n", 'a' + i, letter_frequency_arr[i]);
    }

    double freqs[26];

    uint64_t sum = 0;
    for (int i = 0; i < 26; ++i) {
        sum += letter_frequency_arr[i];
    }

    for (int i = 0; i < 26; ++i) {
        freqs[i] = (double) letter_frequency_arr[i] / (double) sum;
    }

    sprintf(command,
            "python3 graph.py %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f "
            "%c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f "
            "%c%f",
            'a', freqs[0], 'b', freqs[1], 'c',
            freqs[2], 'd', freqs[3], 'e', freqs[4],
            'f', freqs[5], 'g', freqs[6], 'h',
            freqs[7], '0', freqs[8], 'j', freqs[9],
            'k', freqs[10], 'l', freqs[11], 'm',
            freqs[12], 'n', freqs[13], 'o', freqs[14],
            'p', freqs[15], 'q', freqs[16], 'r',
            freqs[17], 's', freqs[18], 't', freqs[19],
            'u', freqs[20], 'v', freqs[21], 'w',
            freqs[22], 'x', freqs[23], 'y', freqs[24],
            'z', freqs[25]);

    system(command);

    fclose(fp);

    free(data);

    return 0;
}
