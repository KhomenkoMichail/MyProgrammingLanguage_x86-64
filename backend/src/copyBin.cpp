#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
    if (argc != 3) return printf("Usage: %s input.bin output.h\n", argv[0]), 1;


    const char* inputFileName = argv[1];
    const char* outputFileName = argv[2];

    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        fprintf(stderr, "Error of opening file \"%s\"", inputFileName);
        perror("");
        return 1;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        fprintf(stderr, "Error of opening file \"%s\"", inputFileName);
        perror("");
        fclose(inputFile);
        return 1;
    }

    fprintf(outputFile, "#ifndef STDLIB_HEX_H\n");
    fprintf(outputFile, "#define STDLIB_HEX_H\n\n");
    fprintf(outputFile, "#include <stdint.h>\n\n");
//    fprintf(outputFile, "#include <stddef.h>\n\n");

    fprintf(outputFile, "const uint8_t STDLIB_CODE[] = \"");

    int byte;
    size_t stdLibSize = 0;
    while ((byte = fgetc(inputFile)) != EOF) {
        fprintf(outputFile, "\\x%02X", byte);
        stdLibSize++;
    }

    fprintf(outputFile, "\";\n\n");
    fprintf(outputFile, "const size_t STDLIB_SIZE = %zu;\n\n", stdLibSize);

    fprintf(outputFile, "#endif\n");


    if (fclose(inputFile)) {
        fprintf(stderr, "Error of closing file \"%s\"", inputFileName);
        perror("");
        fclose(outputFile);
        return 1;
    }

    if (fclose(outputFile)) {
        fprintf(stderr, "Error of closing file \"%s\"", outputFileName);
        perror("");
        return 1;
    }

    return 0;
}
