#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define  MAX_RLE_LEN 10000

struct Int_Sequence {
    /*
     * This structure holds the integer sequence representing the output.
     *
     * [[NOTE, usually this would be in a header file]]
     *
     * width: the width of the output image
     * height: the height of the output image
     * total_g_codes: the numbeer of greycodes represented by the image
     * sequence: the sequence itself
    */
    int width;
    int height;
    int total_g_codes;
    int sequence[MAX_RLE_LEN];
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
    static char *buffer;
    buffer = (char *)malloc(MAX_RLE_LEN);

    fprintf(stderr, "RLE: "); /* Prompt for user, printed to stderr */

    /* Truncate input by only accepting specified number of chars, and throwing
     * away the rest of stdin
     */
    fgets(buffer, MAX_RLE_LEN, stdin);
    buffer[strcspn(buffer, "\n")] = 0; /* Truncates buffer at newlines */
    fflush(stdin); /* If stdin > MAX_RLE_LEN, discard the rest of stdin */

    return buffer;
}

char greycode_char(int total_g_codes, int current_code) {
    /*
     * This function will return the correct character for a given greycode.
     *
     * [[NOTE, usually this would be in a header file]]
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

void print_image(struct Int_Sequence int_sequence) {
    /*
     * Will print the image, given the sequence of characters. The newlines are
     * taken care of here too.
     *
     * [[NOTE, usually this would be in a header file]]
     *
     * int_sequence: a int_sequence struct containg the code's information
     * output_sequence: a sequence of integers corresponding to greycodes
          representing the output
    */
    int i;
    for (i = 1; i <= (int_sequence.width * int_sequence.height); i++) {
        printf("%c", greycode_char(int_sequence.total_g_codes, int_sequence.sequence[i - 1]));
        if (!(i % (int_sequence.width))) {
            printf("\n");
        }
    }
}

struct Int_Sequence convert_to_sequence(char *input_run) {
    /*
     * This function will convert the input string into an integer sequence,
     * and also store all other required parameters.
     *
     * [[NOTE, usually this would be in a header file]]
     *
     * input_run: the input string entered by the user
     *
     * returns: struct Int_Sequence, reepresents the input from the user
    */
    const int non_init = -1;
    int code_or_run = 0, count = 0, number, i;
    char *token;

    struct Int_Sequence int_sequence = {non_init, non_init, non_init, {0}};

    int current_number;

    token = strtok(input_run, " ");
    while (token != NULL) {
        sscanf(token, "%d", &number);
        if (int_sequence.width == non_init) {
            int_sequence.width = number;
        } else if (int_sequence.height == non_init) {
            int_sequence.height = number;
        } else if (int_sequence.total_g_codes == non_init) {
            int_sequence.total_g_codes = number;
        } else {
            /* All base values have been initialised */
            if (code_or_run) {
                /* Greycode */
                for (i = 0; i < number; i++) {
                    int_sequence.sequence[count] = current_number;
                    count += 1;
                }
                code_or_run = 0;
            } else {
                /* Number of repetitions for the code */
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
     * print the int_seq.
    */
    char *input_run;
    struct Int_Sequence int_sequence;

    input_run = get_input();
    int_sequence = convert_to_sequence(input_run);
    print_image(int_sequence);

    return 0;
}
