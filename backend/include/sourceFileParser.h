#ifndef SOURCE_FILE_PARSER_H
#define SOURCE_FILE_PARSER_H

struct line {
    char* ptrToString;
    size_t lengthOfString;
};

struct sourceFile_t {
    char* text;
    struct line* arrOfStringStructs;
    size_t sizeOfText;
    size_t numberOfStrings;

    const char* fileName;
};

const int NUM_OF_CLOSEST_LINES = 2;

char* copyFileContent (sourceFile_t* srcFile, const char* fileName);

void getStructSourceFile (sourceFile_t* srcFile, const char* fileName);

void getArrOfStringStructs (sourceFile_t* srcFile);

void getLengthOfStrings (sourceFile_t* srcFile);

void freeStructSourceFile (sourceFile_t* srcFile);

size_t getNumberOfSymbols (char* text, char searchedSymbol);

void replaceSymbols (char* text, char targetSymbol, char replacementSymbol);

void fprintfCommentsToAsm (node_t* node, sourceFile_t* srcFile, FILE* asmFile);

#endif
