#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////// Just data structure stuff ////////////////////////////////////
// might consider a data_struct.c for reuse

// more appropriate exercise for a
// dynamic array
typedef struct {
    char **array;
    int rows;
    int cols;
} Dynamic2DArray;

// just init as empty and grow from there...
Dynamic2DArray *init_Dynamic2DArray() {
    Dynamic2DArray *dArray = malloc(sizeof(Dynamic2DArray));
    dArray->rows = 0;
    dArray->cols = 0;
    dArray->array = NULL;

    return dArray;
}

void grow_Dynamic2DArray(Dynamic2DArray *dArray, int newRows, int newCols) {

    // resize existing rows if cols increased, init new cols with '\0'
    if (newCols > dArray->cols) {
        int oldCols = dArray->cols;
        for (int i = 0; i < dArray->rows; i++) {
            dArray->array[i] =
                realloc(dArray->array[i], newCols * sizeof(char));
            memset(dArray->array[i] + oldCols, '\0',
                   (newCols - oldCols) * sizeof(char));
        }
        dArray->cols = newCols;
    }

    // add rows if rows increased
    if (newRows > dArray->rows) {
        dArray->array = realloc(dArray->array, newRows * sizeof(char *));
        for (int i = dArray->rows; i < newRows; i++) {
            dArray->array[i] = malloc(newCols * sizeof(char));
            memset(dArray->array[i], '\0', newCols * sizeof(char));
        }
        dArray->rows = newRows;
    }
}

Dynamic2DArray *dup_Dynamic2DArray(Dynamic2DArray *src) {
    Dynamic2DArray *dup = init_Dynamic2DArray();
    grow_Dynamic2DArray(dup, src->rows, src->cols);

    for (int i = 0; i < src->rows; i++) {
        memcpy(dup->array[i], src->array[i], src->cols * sizeof(char));
    }

    return dup;
}

void append_Dynamic2DArray(Dynamic2DArray *dArray, char *line) {
    int len = strlen(line); // dropping '/0'
    int cols = (len > dArray->cols) ? len : dArray->cols;

    grow_Dynamic2DArray(dArray, dArray->rows + 1, cols);
    memcpy(dArray->array[dArray->rows - 1], line, len);
}

void free_Dynamic2DArray(Dynamic2DArray *dArray) {
    for (int i = 0; i < dArray->rows; i++) {
        free(dArray->array[i]);
    }
    if (dArray->rows > 0) {
        free(dArray->array);
    }
    free(dArray);
}

////////////// end data structure, begin puzzle logic /////////////////////////

bool is_a_roll(Dynamic2DArray *input, int row, int col) {
    return (row >= 0 && row < input->rows && col >= 0 && col < input->cols &&
            (input->array[row][col] == '@' || input->array[row][col] == 'x'));
}

void mark_rolls_for_deletion(Dynamic2DArray *input) {
    for (int row = 0; row < input->rows; row++) {
        for (int col = 0; col < input->cols; col++) {

            // ensure roll to begin with
            if (is_a_roll(input, row, col)) {

                int adjacent_rolls = 0;

                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (!(i == 0 && j == 0) // not the roll in question
                            && is_a_roll(input, row + i, col + j)) {
                            adjacent_rolls++;
                        }
                    }
                }

                if (adjacent_rolls < 4) { // mark accessible
                    input->array[row][col] = 'x';
                }
            }
        }
    }
}

int count_marked_rolls(Dynamic2DArray *input) {
    int accessible_rolls = 0;

    for (int row = 0; row < input->rows; row++) {
        for (int col = 0; col < input->cols; col++) {
            //            printf("%c ", input->array[row][col]);
            if (input->array[row][col] == 'x') {
                accessible_rolls++;
            }
        }
        //        printf("\n");
    }

    return accessible_rolls;
}

void delete_marked_rolls(Dynamic2DArray *input) {
    for (int row = 0; row < input->rows; row++) {
        for (int col = 0; col < input->cols; col++) {
            if (input->array[row][col] == 'x') {
                input->array[row][col] = '.';
            }
        }
    }
}

int solve_part_1(Dynamic2DArray *input) {
    mark_rolls_for_deletion(input);
    return count_marked_rolls(input);
}

int solve_part_2(Dynamic2DArray *input) {
    int deleted_rolls = 0;
    mark_rolls_for_deletion(input);

    int marked_rows = count_marked_rolls(input);

    while (marked_rows > 0) {
        deleted_rolls += marked_rows;   // tally
        delete_marked_rolls(input);     // remove
        mark_rolls_for_deletion(input); // iterate
        marked_rows = count_marked_rolls(input);
    }

    return deleted_rolls;
}

int main() {
    char *line;
    char *input = pull_input("https://adventofcode.com/2025/day/4/input");

    // parse input (duplicating for each part since I monkey with state)
    Dynamic2DArray *parsed_input_p1 = init_Dynamic2DArray();
    for (line = strtok(input, "\n"); line != NULL; line = strtok(NULL, "\n")) {
        append_Dynamic2DArray(parsed_input_p1, line);
    }
    Dynamic2DArray *parsed_input_p2 = dup_Dynamic2DArray(parsed_input_p1);

    // solve
    int part_1 = solve_part_1(parsed_input_p1);
    int part_2 = solve_part_2(parsed_input_p2);

    printf("(part 1) accessible rolls: %d\n", part_1);
    printf("(part 2) removed rolls: %d\n", part_2);

    // quit
    free_Dynamic2DArray(parsed_input_p1);
    free_Dynamic2DArray(parsed_input_p2);
    free(input);
    return 0;
}
