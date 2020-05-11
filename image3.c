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
            current_run_char = ' ';
            break;
        case 2:
            current_run_char = '.';
            break;
        }
        return current_run_char;

        break;
    case 4:
        switch (current_code) {
        case 0:
            current_run_char = ' ';
            break;
        case 1:
            current_run_char = '.';
            break;
        case 2:
            current_run_char = ':';
            break;
        case 3:
            current_run_char = '#';
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
    int width = -1, height = -1, g_levels = -1, code_or_run = 0, count = 0, number, i;
    char *token;
    char run[] = "39 26 2 1 89 0 14 1 22 0 21 1 16 0 26 1 13 0 6 1 3 0 7 1 3 0 7 1 13 0 6 1 3 0 7 1 3 0 7 1 13 0 12 1 1 0 12 1 16 0 9 1 3 0 9 1 23 0 12 1 23 0 3 1 14 0 3 1 20 0 18 1 26 0 6 1 21 0 4 1 25 0 4 1 5 0 6 1 22 0 7 1 8 0 5 1 16 0 5 1 16 0 5 1 10 0 5 1 22 0 5 1 4 0 5 1 28 0 8 1 31 0 8 1 28 0 5 1 4 0 5 1 21 0 6 1 10 0 6 1 11 0 9 1 16 0 8 1 8 0 5 1 20 0 5 1 83";
    int final_sequence[10000], current_number;
    int larger_sequence[19999];

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
                current_number = number;
                code_or_run = 1;
            }
        }
        token = strtok(NULL, " ");
    }

    int v_addition = 0, h_addition = 0;
    int original_count = 0;

    for (i = 0; i < ((((2 * width) - 1) * ((2 * height) - 1))); i++) {
        if (!h_addition && !v_addition) {
            larger_sequence[i] = final_sequence[original_count];

            printf("%c", greycode_char(g_levels, final_sequence[original_count]));
            printf("%c", final_sequence[original_count]);
            original_count += 1;
            if (!(original_count % 39)) {
                printf("\n");
            }
            // } else if (h_addition && !v_addition) {
            /* code */
        // } else if (!h_addition && v_addition) {
        //     larger_sequence[i] = (final_sequence[original_count-1]+final_sequence[original_count+1])/2;
        } else {
            larger_sequence[i] = 1;
        }

        // flip the horries
        if (!(i % 78)) {
            if (h_addition == 0) {
                h_addition = 1;
            } else {
                h_addition = 0;
            }
        }

        // flip the verties
        if (v_addition == 0) {
            v_addition = 1;
        } else {
            v_addition = 0;
        }
    }


    for (i = 0; i < (width * height); i++) {
        printf("%c", greycode_char(g_levels, final_sequence[i]));
        if (!(i % width)) {
            printf("\n");
        }
    }


    printf("\n");

    for (i = 0; i < (((2 * width) - 1) * ((2 * height) - 1)); i++) {
        printf("%c", greycode_char(2, larger_sequence[i]));
        if (!(i % 78)) {
            printf("\n");
        }
    }
    return 0;
}
