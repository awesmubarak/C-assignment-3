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
    int code_or_run = 0, count = 0, number, i;
    char *token;

    struct Int_Sequence int_sequence = {-1, -1, -1, {0}};

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

struct Int_Sequence convert_to_gradient(struct Int_Sequence int_seq) {
    struct Int_Sequence grad_seq;
    int i, p1, p2, p3, p4, gh, gv, gp, gn, max_g_l, max_g_d, max_g, h_l, v_l;

    grad_seq.width = int_seq.width;
    grad_seq.height = int_seq.height;
    grad_seq.total_g_codes = int_seq.total_g_codes;

    for (i = 1; i <= (int_seq.width * int_seq.height); i++) {
        v_l = !(i % int_seq.width);
        h_l = i > (int_seq.width * (int_seq.height - 1));
        if (!v_l && !h_l) {
            /* Get the value of each pixel of the image */
            p1 = int_seq.sequence[i];
            p2 = int_seq.sequence[i + 1];
            p3 = int_seq.sequence[i + int_seq.width];
            p4 = int_seq.sequence[i + int_seq.width + 1];

            /* Calculate each directional gradient */
            gh = abs((p1 - p2 + p3 - p4) / 2);
            gv = abs((p1 - p3 + p2 - p4) / 2);
            gp = abs(p1 - p4);
            gn = abs(p2 - p3);

            /* Figure out which is the largest gradient */
            max_g_l = gh > gv ? gh : gv;
            max_g_d = gp > gn ? gp : gn;
            max_g = max_g_l > max_g_d ? max_g_l : max_g_d;

            grad_seq.sequence[i] = max_g;
        }
    }

    return grad_seq;
}

int main() {
    /*
     * This function acts like a gateway, connecting all the other required
     * functions togehter. First it will get the input as a string, next it will
     * pass this input to be converted into an int array, then it will convert
     * the int_seq into a gradient sequence, and finally print it.
    */
    char *input_run;
    struct Int_Sequence int_sequence;
    struct Int_Sequence grad_seq;

    input_run = get_input();
    int_sequence = convert_to_sequence(input_run);
    grad_seq = convert_to_gradient(int_sequence);
    print_image(grad_seq);

    return 0;
}
