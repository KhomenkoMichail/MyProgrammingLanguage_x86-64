unsigned long long labelHash = getStringHash(labelName);
            struct stringLabel* searchedLabel = (struct stringLabel*)bsearch(&labelHash, Asm->stringLabels, Asm->stringLabelCounter, sizeof(struct stringLabel), bsearchLabelComparator);

            if ((searchedLabel != NULL))
                if(strcmp(labelName, searchedLabel->labelName) == 0)
                    labelAddress = searchedLabel->labelAddress;

            (Asm->commandBuffer)[Asm->commandCounter++] = labelAddress;

            fprintf(Asm->listingFile, "%7d  | ", labelAddress);
            return 0;


        if(sscanf(commandString, ":%s", labelName) == 1) {

            if(Asm->stringLabelCounter >= NUM_OF_STRING_LABELS) {
                printf("ERROR! Too many string labels. ONLY %d string labels are supported!", NUM_OF_STRING_LABELS);
                return 0;
            }

            unsigned long long labelHash = getStringHash(labelName);
            struct stringLabel* searchedLabel = (struct stringLabel*)bsearch(&labelHash, Asm->stringLabels, Asm->stringLabelCounter, sizeof(struct stringLabel), bsearchLabelComparator);

            if((searchedLabel != NULL) && (strcmp(searchedLabel->labelName, labelName) == 0))
                return 1;

            strcpy(((Asm->stringLabels)[Asm->stringLabelCounter]).labelName, labelName);
            ((Asm->stringLabels)[Asm->stringLabelCounter]).labelAddress = Asm->commandCounter - signSize;
            ((Asm->stringLabels)[Asm->stringLabelCounter]).stringLabelHash = labelHash;

            Asm->stringLabelCounter++;

            qsort(Asm->stringLabels, Asm->stringLabelCounter, sizeof(struct stringLabel), structLabelComparator);



int structLabelComparator(const void* firstStruct, const void* secondStruct) {
    assert(firstStruct);
    assert(secondStruct);

    const struct stringLabel* firstLabel = (const struct stringLabel*)firstStruct;
    const struct stringLabel* secondLabel = (const struct stringLabel*)secondStruct;

    if (firstLabel->stringLabelHash < secondLabel->stringLabelHash)
        return -1;
    else if (firstLabel->stringLabelHash > secondLabel->stringLabelHash)
        return 1;
    else
        return firstLabel->stringLabelHash - ;
}

int bsearchLabelComparator(const void* key, const void* element) {
    unsigned long long keyHash = *(unsigned long long*)key;
    const struct stringLabel* label = (struct stringLabel*)element;

    return keyHash - label->stringLabelHash;
}
