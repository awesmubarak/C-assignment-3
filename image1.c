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
    int width = -1, height = -1, g_levels = -1, code_or_run = 0, count = 0, number, i;
    char current_run_char;
    char *token;
    char run[] = "78 26 3 1 364 0 4 1 7 0 3 1 43 2 7 1 12 0 7 1 4 0 6 1 33 2 28 0 2 2 2 0 2 1 5 0 1 2 2 0 1 1 31 2 27 0 6 2 14 1 28 2 28 0 7 2 5 0 4 2 4 0 2 2 1 1 26 2 29 0 7 2 4 0 4 2 6 0 2 2 1 1 27 2 26 0 9 2 15 1 29 2 25 0 9 2 9 0 4 2 1 1 29 2 27 0 9 2 12 1 28 0 5 2 24 0 11 2 4 0 3 1 30 0 11 2 19 0 12 2 2 0 8 1 25 0 17 2 15 0 10 2 1 0 17 1 17 0 21 1 13 0 9 1 3 0 14 1 17 0 11 1 2 0 8 1 15 0 9 1 10 0 4 1 19 0 9 1 4 0 7 1 18 0 9 1 31 0 8 1 5 0 6 1 21 0 9 1 28 0 8 1 6 0 5 1 25 0 7 1 28 0 7 1 5 0 8 1 24 0 3 1 333";
    char final_string[100000]; //arbitary

    int newlines = 0;

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
                    final_string[count] = current_run_char;
                    count += 1;
                    if (!((count - newlines) % width)) {
                        final_string[count + 1] = '\n';
                    }
                }
                code_or_run = 0;
            } else {
                code_or_run = 1;
                current_run_char = greycode_char(g_levels, number);
            }
        }
        token = strtok(NULL, " ");
    }

    for (i = 0; i < (width * height); i++) {
        printf("%c", final_string[i]);
        if (!(i % width)) {
            printf("\n");
        }
    }

    return 0;
}
