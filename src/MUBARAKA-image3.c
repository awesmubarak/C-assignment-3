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

void print_image(struct Int_Sequence int_seq) {
    /*
     * Will print the image, given the sequence of characters. The newlines are
     * taken care of here too.
     *
     * int_seq: a int_seq struct containg the code's information
     * output_sequence: a sequence of integers corresponding to greycodes
          representing the output
    */
    int i;
    for (i = 1; i <= (int_seq.width * int_seq.height); i++) {
        printf("%c", greycode_char(int_seq.total_g_codes, int_seq.sequence[i - 1]));
        if (!(i % (int_seq.width))) {
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

    struct Int_Sequence int_seq = {-1, -1, -1};

    int current_number;

    token = strtok(input_run, " ");
    while (token != NULL) {
        sscanf(token, "%d", &number);
        if (int_seq.width == -1) { // stop -1 from being a magic number
            int_seq.width = number;
        } else if (int_seq.height == -1) {
            int_seq.height = number;
        } else if (int_seq.total_g_codes == -1) {
            int_seq.total_g_codes = number;
        } else {
            if (code_or_run) {
                for (i = 0; i < number; i++) {
                    int_seq.sequence[count] = current_number;
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
    return int_seq;
}

struct Int_Sequence convert_total_g_code_1_to_4(struct Int_Sequence int_seq) {
    /*
     * Will convert a sequence coded for an output of greycode level 1 to level
     * 4.
     *
     * int_seq: an int sequence
     *
     * returns: converted int sequence
    */
    int i;
    int_seq.total_g_codes = 4;
    for (i = 0; i < (int_seq.width * int_seq.height); i++) {
        if (int_seq.sequence[i] == 1) {
            int_seq.sequence[i] = 3;
        }
    }
    return int_seq;
}

struct Int_Sequence expand_seq(struct Int_Sequence int_seq) {
    /*
     * Will expand a sequence.
    */
    int i, a, b, c, d;
    int v_addition = 0, h_addition = 0, original_count = 0, horizontal_count = 0;

    struct Int_Sequence expanded_seq;
    expanded_seq.total_g_codes = int_seq.total_g_codes;

    expanded_seq.width = (2 * int_seq.width) - 1;
    expanded_seq.height = (2 * int_seq.height) - 1;

    for (i = 1; i <= (expanded_seq.width * expanded_seq.height); i++) {
        if (!h_addition && !v_addition) {
            expanded_seq.sequence[i - 1] = int_seq.sequence[original_count];

            original_count += 1;
        } else if (!h_addition && v_addition) {
            a = int_seq.sequence[original_count];
            b = int_seq.sequence[original_count - 1];
            expanded_seq.sequence[i - 1] = (a + b) / 2;
        } else if (h_addition && !v_addition) {
            a = int_seq.sequence[horizontal_count];
            b = int_seq.sequence[horizontal_count + int_seq.width];
            expanded_seq.sequence[i - 1] = (a + b) / 2;
            horizontal_count++;
        } else {
            a = int_seq.sequence[horizontal_count];
            b = int_seq.sequence[horizontal_count - 1];
            c = int_seq.sequence[horizontal_count + int_seq.width];
            d = int_seq.sequence[horizontal_count + int_seq.width - 1];

            expanded_seq.sequence[i - 1] = (a + b + c + d) / 4;
        }

        // flip the horizontals
        if (!(i % (expanded_seq.width))) {
            h_addition = h_addition == 1 ? 0 : 1;
            v_addition = 1;
        }

        // flip the verticals
        v_addition = v_addition == 1 ? 0 : 1;
    }
    return expanded_seq;
}

struct Int_Sequence smooth_image(struct Int_Sequence rough_seq) {
    int i, l_top, l_left, l_right, l_bottom, limit, total_value;
    struct Int_Sequence smooth_seq;

    smooth_seq.height = rough_seq.height;
    smooth_seq.width = rough_seq.width;
    smooth_seq.total_g_codes = rough_seq.total_g_codes;

    for (i = 0; i < (rough_seq.width * rough_seq.height); i++) {
        /* Check if the cell is at the edges of the image */
        l_top = i < rough_seq.width;
        l_left = !(i % rough_seq.width);
        l_right = !((i + 1) % rough_seq.width);
        l_bottom = i > rough_seq.width * (rough_seq.height - 1);
        limit = l_top || l_left || l_right || l_bottom;

        if (!limit) {
            total_value = rough_seq.sequence[i - rough_seq.width - 1];
            total_value += 2 * rough_seq.sequence[i - rough_seq.width];
            total_value += rough_seq.sequence[i - rough_seq.width + 1];
            total_value += 2 * rough_seq.sequence[i - 1];
            total_value += 4 * rough_seq.sequence[i];
            total_value += 2 * rough_seq.sequence[i+ 1];
            total_value += rough_seq.sequence[i + rough_seq.width - 1];
            total_value += 2 * rough_seq.sequence[i + rough_seq.width];
            total_value += rough_seq.sequence[i + rough_seq.width + 1];
            total_value += 7;

            smooth_seq.sequence[i] = total_value / 16;
        } else {
            smooth_seq.sequence[i] = rough_seq.sequence[i];
        }
        total_value = 0;
    }

    return smooth_seq;
}

struct Int_Sequence smooth_image_caller(struct Int_Sequence rough_seq, int n_its) {
    /*
     * This function will call the smooth_image() function n times, as specified
     * in the specification. The function is recursive, and can be called with
     * any n number of iterations.
     *
     * rough_seq: the sequence that is to be smoothed
     * n_its: number of iterations to carry out
     *
     * returns: the smooth sequence
     */
    struct Int_Sequence smooth_seq;

    if (n_its != 1){
        rough_seq = smooth_image_caller(rough_seq, n_its - 1);
    }
    smooth_seq = smooth_image(rough_seq);

    return smooth_seq;
}

int main() {
    /*
     * This function acts like a gateway, connecting all the other required
     * functions together. First it will get the input as a string, next it will
     * pass this input to be converted into an int array, and finally it will
    */
    char *input_run;
    struct Int_Sequence int_seq;
    struct Int_Sequence expanded_seq;
    struct Int_Sequence smooth_seq;

    input_run = get_input();
    int_seq = convert_to_sequence(input_run);

    int_seq = convert_total_g_code_1_to_4(int_seq);
    expanded_seq = expand_seq(int_seq);
    smooth_seq = smooth_image_caller(expanded_seq, 3);

    print_image(int_seq);
    print_image(expanded_seq);
    print_image(smooth_seq);

    return 0;
}
