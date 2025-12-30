#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// parsing purely so I can pull the input once and
// read through it twice, while still using strtok
typedef struct Node {
    int data;
    struct Node *next;
} ll_Input;

ll_Input *parse_input(char *raw_input) {
    ll_Input *head = NULL;
    ll_Input *tail = NULL;
    ll_Input *current = NULL;
    char *line;

    // line by line
    for (line = strtok(raw_input, "\n"); line != NULL;
         line = strtok(NULL, "\n")) {
        current = (ll_Input *)malloc(sizeof(ll_Input));

        // parse to signed int
        current->data = atoi(line + 1);
        if (line[0] == 'L') {
            current->data *= -1;
        }
        // append
        if (!tail) {
            head = current;
            tail = current;
        } else {
            tail->next = current;
            tail = current;
        }
        current->next = NULL;
    }
    return head;
}

void free_ll(ll_Input *input) {
    ll_Input *current;

    while (input) {
        current = input;
        input = current->next;
        free(current);
    }
}

int solve_part_1(ll_Input *parsed_input) {
    int current = 50;
    int times_zero = 0;

    while (parsed_input) {
        // apply to current number, handle conditions around zero
        current += parsed_input->data;
        while (current < 0) {
            current += 100;
        }
        while (current > 99) {
            current -= 100;
        }

        // note if zero
        if (current == 0) {
            times_zero++;
        }

        parsed_input = parsed_input->next;
    }

    return times_zero;
}

int solve_part_2(ll_Input *parsed_input) {
    int current = 50;
    int previous = 50;
    int times_crossing_zero = 0;

    // line by line
    while (parsed_input) {

        // apply to current number, handle conditions around zero
        previous = current;
        current += parsed_input->data;

        while (current < 0) {
            current += 100;
            if (previous != 0) { // handle if we clicked left FROM zero
                times_crossing_zero++;
            } else {
                previous = current;
            }
        }
        while (current > 99) {
            current -= 100;
            if (current != 0) {        // skip if we clicked TO zero
                times_crossing_zero++; // since it's already handled
            }
        }

        // note if zero
        if (current == 0) {
            times_crossing_zero++;
        }
        parsed_input = parsed_input->next;
    }

    return times_crossing_zero;
}

int main() {
    char *input = pull_input("https://adventofcode.com/2025/day/1/input");
    ll_Input *parsed = parse_input(input);

    printf("(part 1) times zero: %d\n", solve_part_1(parsed));
    printf("(part 2) times crossing zero: %d\n", solve_part_2(parsed));

    free_ll(parsed);
    free(input);
    return 0;
}
