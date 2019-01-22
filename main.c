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
                score += strlen(common_letters) - i;
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
            ciphertext[i] = tolower(input[i]) + key;
            if (ciphertext[i] > 'z') {
                ciphertext[i] -= 26;
            }
        } else {
            ciphertext[i] = input[i];
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

    letter_frequency(data, data_len);

    char* command = calloc(1ul << 20, 1);

    strcat(command, "python3 graph.py ");

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
            'a', freqs[0], 'b', freqs[1], 'c', freqs[2], 'd', freqs[3], 'e', freqs[4], 'f',
            freqs[5], 'g', freqs[6], 'h', freqs[7], '0', freqs[8], 'j', freqs[9], 'k', freqs[10],
            'l', freqs[11], 'm', freqs[12], 'n', freqs[13], 'o', freqs[14], 'p', freqs[15], 'q',
            freqs[16], 'r', freqs[17], 's', freqs[18], 't', freqs[19], 'u', freqs[20], 'v',
            freqs[21], 'w', freqs[22], 'x', freqs[23], 'y', freqs[24], 'z', freqs[25]);

    system(command);

    unsigned char* ciphertext = caesar_cipher(data, data_len, 7);

    letter_frequency(ciphertext, data_len);

    sum = 0;
    for (int i = 0; i < 26; ++i) {
        sum += letter_frequency_arr[i];
    }

    for (int i = 0; i < 26; ++i) {
        freqs[i] = (double) letter_frequency_arr[i] / (double) sum;
    }

    memset(command, 0, 1ul << 20);
    strcat(command, "python3 graph.py ");

    sprintf(command,
            "python3 graph.py %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f "
            "%c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f %c%f "
            "%c%f",
            'a', freqs[0], 'b', freqs[1], 'c', freqs[2], 'd', freqs[3], 'e', freqs[4], 'f',
            freqs[5], 'g', freqs[6], 'h', freqs[7], '0', freqs[8], 'j', freqs[9], 'k', freqs[10],
            'l', freqs[11], 'm', freqs[12], 'n', freqs[13], 'o', freqs[14], 'p', freqs[15], 'q',
            freqs[16], 'r', freqs[17], 's', freqs[18], 't', freqs[19], 'u', freqs[20], 'v',
            freqs[21], 'w', freqs[22], 'x', freqs[23], 'y', freqs[24], 'z', freqs[25]);

    system(command);

    printf("Top 6 probabilities:\ne: %f\nt: %f\na: %f\no: %f\ni: %f\nn: %f\n",
            0.04 * freqs['e' - 'a'],
            0.04 * freqs['t' - 'a'],
            0.04 * freqs['a' - 'a'],
            0.04 * freqs['o' - 'a'],
            0.04 * freqs['i' - 'a'],
            0.04 * freqs['n' - 'a']
            );

    unsigned long max_score = 0;
    int best_key = 0;
    for (int i = 1; i < 26; ++i) {
        unsigned char* tmp_ciphertext = caesar_cipher(ciphertext, data_len, i);
        const unsigned long s = plaintext_frequency(tmp_ciphertext, data_len);
        if (s > max_score) {
            max_score = s;
            best_key = i;
        }
        free(tmp_ciphertext);
    }

    printf("Best scoring key tested: %d\n", 26 - best_key);

    printf("Top 6 letter results:\ne: %c\nt: %c\na: %c\no: %c\ni: %c\nn: %c\n",
            'e' + 26 - best_key,
            't' + 26 - best_key,
            'a' + 26 - best_key,
            'o' + 26 - best_key,
            'i' + 26 - best_key,
            'n' + 26 - best_key
            );

    fclose(fp);

    free(data);
    free(ciphertext);
    free(command);

    return 0;
}
