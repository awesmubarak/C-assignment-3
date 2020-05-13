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

struct Int_Sequence convert_total_g_code_1_to_4(struct Int_Sequence int_sequence) {
    /*
     * Will convert a sequence coded for an output of greeycode level 1 to level
     * 4.
     *
     * int_sequence: an int sequence
     *
     * returns: converted int sequence
    */
    int i;
    for (i = 0; i < (int_sequence.width * int_sequence.height); i++) {
        if (int_sequence.sequence[i] == 1) {
            int_sequence.sequence[i] = 3;
        }
    }
    return int_sequence;
}

struct Int_Sequence expand_seq(struct Int_Sequence int_sequence) {
    int i, a, b, c, d;
    int v_addition = 0, h_addition = 0;
    int original_count = 0;
    double result;
    int horizontal_count = 0;

    struct Int_Sequence expanded_seq;

    expanded_seq.width = (2 * int_sequence.width) - 1;
    expanded_seq.height = (2 * int_sequence.height) - 1;

    for (i = 0; i < (expanded_seq.width * expanded_seq.height); i++) {
        if (!h_addition && !v_addition) {
            expanded_seq.sequence[i] = int_sequence.sequence[original_count];

            original_count += 1;
        } else if (!h_addition && v_addition) {
            expanded_seq.sequence[i] = (int_sequence.sequence[original_count] + int_sequence.sequence[original_count - 1]) / 2;
        } else if (h_addition && !v_addition) {
            horizontal_count++;
            expanded_seq.sequence[i] = (int_sequence.sequence[horizontal_count] + int_sequence.sequence[horizontal_count - int_sequence.width]) / 2;
        } else {
            b = int_sequence.sequence[horizontal_count];
            a = int_sequence.sequence[horizontal_count - int_sequence.width];
            c = int_sequence.sequence[horizontal_count + 1 - int_sequence.width];
            d = int_sequence.sequence[horizontal_count + 1];


            result = (double)(a + b + c + d) / 4;
            printf("%f\n", result);

            // expanded_seq[i] = ceil(result);
            expanded_seq.sequence[i] = result;
            printf("%i\n", expanded_seq.sequence[i]);


            // expanded_seq[i] = (final_sequence[horizontal_count] + final_sequence[horizontal_count - width]) / 2;

            // expanded_seq[i] = 5;
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
    return expanded_seq;
}

int main() {
    /*
     * This function acts like a gateway, connecting all the other required
     * functions togehter. First it will get the input as a string, next it will
     * pass this input to be converted into an int array, and finally it will
    */
    char *input_run;
    struct Int_Sequence int_sequence;
    struct Int_Sequence expanded_seq;

    input_run = get_input();
    int_sequence = convert_to_sequence(input_run);

    int_sequence = convert_total_g_code_1_to_4(int_sequence);
    expanded_seq = expand_seq(int_sequence);

    print_image(int_sequence);
    print_image(expanded_seq);

    return 0;
}
