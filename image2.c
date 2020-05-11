#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char greycode_char(int g_codes, int current_code) {
    char current_run_char;

    switch (g_codes) {
    case 2:
        switch (current_code) {
        case 0:
            current_run_char = '#';
            break;
        case 1:
            current_run_char = ' ';
            break;
        }
        return current_run_char;

        break;
    case 3:
        switch (current_code) {
        case 0:
            current_run_char = '#';
            break;
        case 1:
            current_run_char = '.';
            break;
        case 2:
            current_run_char = ' ';
            break;
        }
        return current_run_char;

        break;
    case 4:
        switch (current_code) {
        case 0:
            current_run_char = '#';
            break;
        case 1:
            current_run_char = ':';
            break;
        case 2:
            current_run_char = '.';
            break;
        case 3:
            current_run_char = ' ';
            break;
        }

        break;
    }
    return current_run_char;
}

const char *get_input(int max_len) {
    static char *buffer;
    buffer = (char *)malloc(max_len + 1);

    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    fflush(stdin);

    return buffer;
}

int main(int argc, char const *argv[]) {
    int width = -1, height = -1, g_levels = -1, code_or_run = 0, count = 0, number, i, j;
    char current_run_char;
    char *token;
    char run[] = "69 31 4 1 227 3 2 0 1 3 2 0 3 3 3 0 3 3 2 1 50 3 1 0 3 3 3 0 4 3 4 0 5 3 2 1 44 3 2 0 3 3 4 0 5 3 5 0 5 3 3 1 41 3 2 0 3 3 4 0 6 3 6 0 5 3 3 1 39 3 2 0 4 3 4 0 6 3 6 0 6 3 3 1 37 3 2 0 4 3 5 0 6 3 7 0 6 3 3 1 7 2 7 1 22 3 2 0 4 3 5 0 6 3 7 0 6 3 3 1 5 2 17 1 14 3 2 0 4 3 5 0 6 3 7 0 6 3 3 1 5 2 17 1 15 3 2 0 3 3 6 0 5 3 7 0 5 3 3 1 4 2 20 1 14 3 2 0 3 3 6 0 5 3 6 0 6 3 3 1 5 2 19 1 15 3 2 0 3 3 5 0 5 3 6 0 5 3 3 1 9 2 14 1 18 3 2 0 4 3 4 0 4 3 5 0 5 3 3 1 43 3 2 0 4 3 3 0 3 3 5 0 5 3 3 1 46 3 2 0 3 3 2 0 3 3 4 0 4 3 2 1 51 3 1 0 3 3 2 0 2 3 3 0 3 3 2 1 55 3 1 0 2 3 1 0 2 3 2 0 3 3 1 1 45 2 14 3 1 0 1 3 1 0 1 3 1 0 2 3 1 1 45 2 16 0 1 2 2 0 1 1 3 0 1 1 45 2 16 0 1 2 2 0 1 1 3 0 1 1 47 2 13 0 10 1 44 2 16 0 8 1 48 2 13 0 8 1 51 2 13 1 107 2 5 1 58 2 11 1 57 2 12 1 55 2 14 1 53 2 16";
    int final_sequence[100000]; //arbitary

    int current_number;

    token = strtok(run, " ");
    while (token != NULL) {
        sscanf(token, "%d", &number);
        if (width == -1) {
            width = number;
        } else if (height == -1) {
            height = number;
        } else if (g_levels == -1) {
            g_levels = number;
        } else {
            if (code_or_run) {
                for (i = 0; i < number; i++) {
                    final_sequence[count] = current_number;
                    count += 1;
                }
                code_or_run = 0;
            } else {
                code_or_run = 1;
                current_run_char = greycode_char(g_levels, number);
                current_number = number;
            }
        }
        token = strtok(NULL, " ");
    }


    // for (i = 0; i < (width * height); i++) {
    //     printf("%i\n", i);
    //     printf("%i\n", i + 1);
    //     printf("%i\n", i + width);
    //     printf("%i\n", i + width + 1);
    // }



    for (i = 0; i < (width * height); i++) {
        printf("%i", greycode_char(g_levels, final_sequence[i]));
        if (!(i % width)) {
            printf("\n");
        }
    }

    return 0;
}
