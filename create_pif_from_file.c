// create_pif_from_file.c
// Utility to create PIF from a source file using generate_pif_from_string

#include "pif_generator.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_pif_file>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];

    FILE *f = fopen(input_file, "rb");
    if (!f) {
        fprintf(stderr, "Error: cannot open input file %s\n", input_file);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) { fclose(f); fprintf(stderr, "Out of memory\n"); return 1; }

    if (fread(buffer, 1, size, f) != (size_t)size) {
        fprintf(stderr, "Error: failed to read file %s\n", input_file);
        free(buffer);
        fclose(f);
        return 1;
    }
    buffer[size] = '\0';
    fclose(f);

    PIFEntry *pif_entries = NULL;
    int pif_count = 0;

    int res = generate_pif_from_string(buffer, &pif_entries, &pif_count, NULL);
    free(buffer);

    if (res < 0 || pif_count == 0) {
        fprintf(stderr, "Error: failed to generate PIF from %s\n", input_file);
        return 1;
    }

    if (write_pif_to_file(output_file, pif_entries, pif_count) != 0) {
        fprintf(stderr, "Error: failed to write PIF to %s\n", output_file);
        free_pif_entries(pif_entries, pif_count);
        return 1;
    }

    printf("Wrote %d entries to %s\n", pif_count, output_file);

    free_pif_entries(pif_entries, pif_count);
    return 0;
}
