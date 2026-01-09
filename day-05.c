#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////// going wild with data structures //////////////////////////////////

typedef struct {
    long long *array;
    size_t size;
    size_t capacity;
} long_array;

typedef struct {
    long long start;
    long long end;
} range;

typedef struct {
    range *array;
    size_t size;
    size_t capacity;
} range_array;

typedef struct {
    long_array *ingredient_ids;
    range_array *fresh_ingredient_ranges;
} input;

long_array *init_long_array() {
    long_array *a = (long_array *)malloc(sizeof(long_array));
    a->size = 0;
    a->capacity = 32;
    a->array = malloc(a->capacity * sizeof(long long));

    return a;
}

void append_long_array(long_array *a, long long l) {
    if (a->size == a->capacity) {
        a->capacity *= 2;
        a->array = realloc(a->array, a->capacity * sizeof(long long));
    }

    a->array[a->size++] = l;
}

void free_long_array(long_array *a) {
    free(a->array);
    free(a);
}

range_array *init_range_array() {
    range_array *a = (range_array *)malloc(sizeof(range_array));
    a->size = 0;
    a->capacity = 32;
    a->array = malloc(a->capacity * sizeof(range));

    return a;
}

void append_range_array(range_array *a, long long start, long long end) {
    if (a->size == a->capacity) {
        a->capacity *= 2;
        a->array = realloc(a->array, a->capacity * sizeof(range));
    }

    a->array[a->size++] = (range){start, end};
}

void free_range_array(range_array *a) {
    free(a->array);
    free(a);
}

void free_input(input *i) {
    free_long_array(i->ingredient_ids);
    free_range_array(i->fresh_ingredient_ranges);
    free(i);
}

// Compare function for qsort
int compare_range(const void *a, const void *b) {
    range *rangeA = (range *)a;
    range *rangeB = (range *)b;

    if (rangeA->start != rangeB->start) {
        if (rangeA->start > rangeB->start) {
            return 1; // must be int, not ll
        } else {
            return -1;
        }
    }

    if (rangeA->end > rangeB->end) {
        return 1;
    } else {
        return -1;
    }
}

// Function to merge ranges, ie, remove overlaps
range_array *merge_ranges(range_array *ranges) {

    // Sort the ranges
    qsort(ranges->array, ranges->size, sizeof(range), compare_range);

    // build a separate list of 'merged'/simplified ranges
    range_array *result = init_range_array();

    // Start the first range
    append_range_array(result, ranges->array[0].start, ranges->array[0].end);

    for (int i = 1; i < ranges->size; i++) {

        // If the current range overlaps with the last merged range
        if (ranges->array[i].start <= result->array[result->size - 1].end + 1) {

            // Merge the ranges by updating the end of the last range
            if (ranges->array[i].end > result->array[result->size - 1].end) {
                result->array[result->size - 1].end = ranges->array[i].end;
            }
        } else {
            // No overlap, add the current range to the result->array list
            append_range_array(result, ranges->array[i].start,
                               ranges->array[i].end);
        }
    }

    return result;
}

//////////////// end of data struct business //////////////////////////////////

input *parse_input(char *raw_input) {
    input *parsed = (input *)malloc(sizeof(input));
    parsed->ingredient_ids = init_long_array();
    parsed->fresh_ingredient_ranges = init_range_array();

    char *line;

    for (line = strtok(raw_input, "\n"); line != NULL;
         line = strtok(NULL, "\n")) {
        if (strchr(line, '-')) {
            long long start, end;
            sscanf(line, "%lld-%lld", &start, &end);
            append_range_array(parsed->fresh_ingredient_ranges, start, end);
        } else if (strlen(line) > 0) {
            long long i;
            sscanf(line, "%lld", &i);
            append_long_array(parsed->ingredient_ids, i);
        }
    }

    // remove range overlaps
    range_array *merged = merge_ranges(parsed->fresh_ingredient_ranges);
    free_range_array(parsed->fresh_ingredient_ranges);
    parsed->fresh_ingredient_ranges = merged;

    return parsed;
}

int solve_part_1(input *in) {
    int fresh_ingredients = 0;

    for (int i = 0; i < in->ingredient_ids->size; i++) {
        long long ingredient = in->ingredient_ids->array[i];

        for (int j = 0; j < in->fresh_ingredient_ranges->size; j++) {
            if (ingredient >= in->fresh_ingredient_ranges->array[j].start &&
                ingredient <= in->fresh_ingredient_ranges->array[j].end) {
                fresh_ingredients++;
                break;
            }
        }
    }

    return fresh_ingredients;
}

long long solve_part_2(input *in) {
    long long possible_fresh_ingredients = 0;

    for (int i = 0; i < in->fresh_ingredient_ranges->size; i++) {
        long long start = in->fresh_ingredient_ranges->array[i].start;
        long long end = in->fresh_ingredient_ranges->array[i].end;

        possible_fresh_ingredients += end - start + 1;
    }

    return possible_fresh_ingredients;
}

int main() {
    char *raw_input = pull_input("https://adventofcode.com/2025/day/5/input");

    input *parsed = parse_input(raw_input);

    int part_1 = solve_part_1(parsed);
    long long part_2 = solve_part_2(parsed);

    printf("(part 1) fresh ingredients : %d\n", part_1);
    printf("(part 2) possible ingredients : %lld\n", part_2);

    free_input(parsed);
    free(raw_input);
    return 0;
}
