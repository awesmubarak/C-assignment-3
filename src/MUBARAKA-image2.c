#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Int_Sequence {
    /*
     * This structure holds the integer sequence representing the output.
     *
     * width: the width of the output image
     * height: the height of the output image
     * total_g_codes: the numbeer of greycodes represented by the image
     * sequence: the sequence itself
    */
    int width;
    int height;
    int total_g_codes;
    int sequence[10000]; //arbitary
};

char *get_input() {
    /*
     * This function will accept the user input.
     *
     * [[NOTE, usually this would be in a header file]]
     *
     * returns: string containg the sanitised input
     */

    /* Setup a dynamically allocated string for the input. This allows us to
     * return a pointer to the memory location, and avoids having to copy the
     * string around.
     */
    int max_len = 1001; // arbitary
    static char *buffer;
    buffer = (char *)malloc(max_len);

    fprintf(stderr, "RLE: "); /* Prompt for user, printed to stderr */

    /* Truncate input by only accepting specified number of chars, and throwing
     * away the rest of stdin
     */
    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = 0; /* Truncates buffer at newlines */
    fflush(stdin); /* If stdin > max_len, discard the rest of stdin */

    return buffer;
}


char greycode_char(int total_g_codes, int current_code) {
    /*
     * This function will return the correct character for a given greycode.
     *
     * total_g_codes: total number of greycodes as specified by user
     * current_code: the code for which the char should be returned
     *
     * returns: char, equivilant to the given greycode
     */
    char current_run_char;

    switch (total_g_codes) {
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

void print_image(struct Int_Sequence int_sequence) {
    /*
     * Will print the image, given the sequence of characters. The newlines are
     * taken care of here too.
     *
     * int_sequence: a int_sequence struct containg the code's information
     * output_sequence: a sequence of integers corresponding to greycodes
          representing the output
    */
    int i, p1, p2, p3, p4, gh, gv, gp, gn, max_g_l, max_g_d, max_g;

    // TODO: convert this into a seperate function

    for (i = 1; i <= (int_sequence.width * int_sequence.height); i++) {
        if (!(i % int_sequence.width) || (i > (int_sequence.width * (int_sequence.height - 1)))) {
            if (!(i % int_sequence.width)) {
                printf("\n");
            }
        } else {
            /* Get the value of each pixel of the image */
            p1 = int_sequence.sequence[i];
            p2 = int_sequence.sequence[i + 1];
            p3 = int_sequence.sequence[i + int_sequence.width];
            p4 = int_sequence.sequence[i + int_sequence.width + 1];

            /* Calculate each directional gradient */
            gh = abs((p1 - p2 + p3 - p4) / 2);
            gv = abs((p1 - p3 + p2 - p4) / 2);
            gp = abs(p1 - p4);
            gn = abs(p2 - p3);

            /* Figure out which is the largest gradient */
            max_g_l = gh > gv ? gh : gv;
            max_g_d = gp > gn ? gp : gn;
            max_g = max_g_l > max_g_d ? max_g_l : max_g_d;

            printf("%c", greycode_char(int_sequence.total_g_codes, max_g));
        }
    }
}

struct Int_Sequence convert_to_sequence(char *input_run) {
    /*
     * This function will convert the input string into an integer sequence,
     * and also store all other required parameters.
     *
     * input_run: the input string entered by the user
     *
     * returns: struct Int_Sequence, reepresents the input from the user
    */
    int code_or_run = 0, count = 0, number, i;
    char *token;

    struct Int_Sequence int_sequence = {-1, -1, -1};

    int current_number;

    token = strtok(input_run, " ");
    while (token != NULL) {
        sscanf(token, "%d", &number);
        if (int_sequence.width == -1) {
            int_sequence.width = number;
        } else if (int_sequence.height == -1) {
            int_sequence.height = number;
        } else if (int_sequence.total_g_codes == -1) {
            int_sequence.total_g_codes = number;
        } else {
            if (code_or_run) {
                for (i = 0; i < number; i++) {
                    int_sequence.sequence[count] = current_number;
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
    return int_sequence;
}

int main() {
    /*
     * This function acts like a gateway, connecting all the other required
     * functions togehter. First it will get the input as a string, next it will
     * pass this input to be converted into an int array, and finally it will
    */
    char *input_run;
    struct Int_Sequence int_sequence;

    input_run = get_input();
    int_sequence = convert_to_sequence(input_run);
    print_image(int_sequence);

    return 0;
}
