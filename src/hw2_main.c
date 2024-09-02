#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

void CreatePPMFiles(const char *inputFilename, const char *outputFilename);
void CreateSBUFiles(const char *inputFilename, const char *outputFilename);

int main(int argc, char **argv)
{
    int opt;
    bool iChecked = false, oChecked = false, cChecked = false, pChecked = false, rChecked = false;
    char *inFile = NULL, *outFile = NULL; 
    char *cArg = NULL;                    
    char *pArg = NULL;                  
    char *font = NULL;                   
    bool DEBUG = false;
    int argumentIndex = 1;
    bool validArgumentFound = true;
    const char *optionCharacters = "i:o:c:p:r:";
    int getoptResult;

    for (; argumentIndex < argc && validArgumentFound; argumentIndex++)
    {
        getoptResult = getopt(argc, argv, optionCharacters);
        if (getoptResult == -1)
            break;

        opt = getoptResult;
        if (opt == -1)
        {
            validArgumentFound = false;
            continue;
        }

        if (opt == 'i')
        {
            if (iChecked)
            {
                if (DEBUG)
                    printf("Duplicate -i argument.\n");
                return DUPLICATE_ARGUMENT;
            }
            iChecked = true;
            inFile = optarg;                
            if (inFile == NULL || inFile[0] == '-') 
            {
                if (DEBUG)
                    printf("Missing parameter for -i argument.\n");
                return MISSING_ARGUMENT;
            }
        }
        else if (opt == 'o')
        {
            if (oChecked)
            {
                if (DEBUG)
                    printf("Duplicate -o argument.\n");
                return DUPLICATE_ARGUMENT;
            }
            oChecked = true;
            outFile = optarg;                       
            if (outFile == NULL || outFile[0] == '-') 
            {
                if (DEBUG)
                    printf("Missing parameter for -o argument.\n");
                return MISSING_ARGUMENT;
            }
        }
        else if (opt == 'c')
        {
            if (cChecked)
            {
                if (DEBUG)
                    printf("Duplicate -c argument.\n");
                return DUPLICATE_ARGUMENT;
            }
            cChecked = true;
            cArg = optarg;                   
            if (cArg == NULL || cArg[0] == '-') 
            {
                if (DEBUG)
                    printf("Missing parameter for -c argument.\n");
                return MISSING_ARGUMENT;
            }
        }
        else if (opt == 'p')
        {
            if (pChecked)
            {
                if (DEBUG)
                    printf("Duplicate -p argument.\n");
                return DUPLICATE_ARGUMENT;
            }
            pChecked = true;
            pArg = optarg;                      
            if (pArg == NULL || pArg[0] == '-') 
            {
                if (DEBUG)
                    printf("Missing parameter for -p argument.\n");
                return MISSING_ARGUMENT;
            }
        }
        else if (opt == 'r')
        {
            if (rChecked)
            {
                if (DEBUG)
                    printf("Duplicate -r argument.\n");
                return DUPLICATE_ARGUMENT;
            }
            rChecked = true;
            font = optarg;                     
            if (font == NULL || font[0] == '-') 
            {
                if (DEBUG)
                    printf("Missing parameter for -r argument.\n");
                return MISSING_ARGUMENT;
            }
        }
        else if (opt == '?')
        {
            if (optopt == 'i' || optopt == 'o' || optopt == 'c' || optopt == 'p' || optopt == 'r')
            {
                if (DEBUG)
                    printf("Option -%c requires an argument.\n", optopt);
                return MISSING_ARGUMENT;
            }
            else
            {
                if (DEBUG)
                    printf("Unrecognized option '-%c'.\n", optopt);
                return UNRECOGNIZED_ARGUMENT;
            }
        }
        else
        {
            if (DEBUG)
                printf("Unexpected error occurred.\n");
            return UNRECOGNIZED_ARGUMENT;
        }
    }

    if (!iChecked || !oChecked)
    {
        if (DEBUG)
            printf("Input and/or output file not specified.\n");
        return MISSING_ARGUMENT;
    }

    if (rChecked)
    {
        FILE *fontFile = fopen(font, "r");
        if (!fontFile)
        {
            if (DEBUG)
                printf("The font file %s cannot be opened.\n", font);
            return R_ARGUMENT_INVALID;
        }
        fclose(fontFile);
    }

    FILE *file = fopen(inFile, "r");
    if (!file)
    {
        printf("The input file does not exist or cannot be opened.\n");
        return INPUT_FILE_MISSING;
    }
    fclose(file);

    if (pChecked)
    {
        int count = 0;
        char *token;
        const char delimiter[2] = ",";
        bool parsingComplete = false;
        bool expectedParameterMismatch = false;
        int expectedCount = 2;

        token = strtok(pArg, delimiter);
        while (!parsingComplete)
        {
            if (token != NULL)
            {
                count++;
                token = strtok(NULL, delimiter);
            }
            else
            {
                parsingComplete = true;
            }
        }

        expectedParameterMismatch = (count != expectedCount);

        if (expectedParameterMismatch)
        {
            if (DEBUG)
                printf("Invalid number of parameters provided to -p argument: expected %d, got %d.\n", expectedCount, count);
            return P_ARGUMENT_INVALID;
        }
    }

    if (cChecked && cArg)
    {
        int count = 0;
        char *token;
        const char delimiter[2] = ",";
        bool parsingComplete = false;
        token = strtok(cArg, delimiter);
        while (!parsingComplete)
        {
            if (token != NULL)
            {
                count++;
                token = strtok(NULL, delimiter);
            }
            else
            {
                parsingComplete = true;
            }
        }
        const int expectedCount = 4;
        if (count != expectedCount)
        {
            if (DEBUG)
                printf("Invalid number of parameters provided to -c argument: expected %d, got %d.\n", expectedCount, count);
            return C_ARGUMENT_INVALID;
        }
    }
    if (pChecked && !cChecked)
    {
        if (DEBUG)
            printf("The -p argument was provided but the -c argument was not.\n");
        return C_ARGUMENT_MISSING;
    }

    const char *inputExt = strrchr(inFile, '.');
    const char *outputExt = strrchr(outFile, '.');

    if (!inputExt || !outputExt)
    {
        return MISSING_ARGUMENT;
    }

    FILE *outputFileTest = fopen(outFile, "w");
    if (!outputFileTest)
    {
        return OUTPUT_FILE_UNWRITABLE;
    }
    else
    {
        fclose(outputFileTest);
        if (strcmp(inputExt, ".ppm") == 0)
        {
            if (strcmp(outputExt, ".ppm") == 0)
            {
                CreatePPMFiles(inFile, outFile);
            }
            else if (strcmp(outputExt, ".sbu") == 0)
            {
                CreateSBUFiles(inFile, outFile);
            }
            else
            {
                return UNRECOGNIZED_ARGUMENT;
            }
        }
        else if (strcmp(inputExt, ".sbu") == 0)
        {
            if (strcmp(outputExt, ".ppm") == 0)
            {
                CreatePPMFiles(inFile, outFile);
            }
            else if (strcmp(outputExt, ".sbu") == 0)
            {
                CreateSBUFiles(inFile, outFile);
            }
            else
            {
                return UNRECOGNIZED_ARGUMENT;
            }
        }
        else
        {
            return UNRECOGNIZED_ARGUMENT;
        }
    }

    return 0;
}

void CreatePPMFiles(const char *inputFilename, const char *outputFilename)
{
    FILE *inputFile = fopen(inputFilename, "r");
    if (!inputFile)
    {
        return;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile)
    {
        fclose(inputFile);
        return;
    }

    char line[1000];
    if (fgets(line, sizeof(line), inputFile) == NULL)
    {
        // printf("Failed to read the file header.\n");
        fclose(inputFile);
        return;
    }
    if (strncmp(line, "P3", 2) == 0)
    {
        unsigned width, height, r, g, b;
        if (fscanf(inputFile, "%u %u", &width, &height) != 2)
        {
            // printf("Failed to read image dimensions.\n");
            fclose(inputFile);
            return;
        }

        unsigned int maxColor;
        if (fscanf(inputFile, "%u", &maxColor) != 1)
        {
            // printf("Failed to read max color value.\n");
            fclose(inputFile);
            return;
        }
        fprintf(outputFile, "P3\n%u %u\n%u\n", width, height, maxColor);
        // printf("P3\n%u %u\n%u\n", width, height, maxColor);

        for (unsigned int i = 0; i < width * height; i++)
        {
            if (fscanf(inputFile, "%u %u %u", &r, &g, &b) != 3)
            {
                fclose(inputFile);
                return;
            }
            fprintf(outputFile, "%u %u %u ", r, g, b);
            // printf("%u %u %u ", r, g, b);
        }
        fprintf(outputFile, "\n");
        // printf("\n");
        fclose(outputFile);
    }
    else if (strncmp(line, "SBU", 3) == 0)
    {
        unsigned int width, height;
        fscanf(inputFile, "%u %u\n", &width, &height);
        unsigned int colorTableSize;
        fscanf(inputFile, "%u\n", &colorTableSize);
        unsigned int *colorTableR = (unsigned int *)malloc(colorTableSize * sizeof(unsigned int));
        unsigned int *colorTableG = (unsigned int *)malloc(colorTableSize * sizeof(unsigned int));
        unsigned int *colorTableB = (unsigned int *)malloc(colorTableSize * sizeof(unsigned int));
        if (!colorTableR || !colorTableG || !colorTableB)
        {
            free(colorTableR);
            free(colorTableG);
            free(colorTableB);
            fclose(inputFile);
            fclose(outputFile);
            return;
        }
        for (unsigned int i = 0; i < colorTableSize; i++)
        {
            fscanf(inputFile, "%u %u %u", &colorTableR[i], &colorTableG[i], &colorTableB[i]);
        }
        fprintf(outputFile, "P3\n%u %u\n255\n", width, height);
        // printf("P3\n%u %u\n255\n", width, height);
        char token[10];
        while (fscanf(inputFile, "%s", token) == 1)
        {
            if (token[0] == '*')
            {
                unsigned int count = atoi(token + 1);
                unsigned int index;
                fscanf(inputFile, "%u", &index);
                for (unsigned int i = 0; i < count; i++)
                {
                    fprintf(outputFile, "%u %u %u ", colorTableR[index], colorTableG[index], colorTableB[index]);
                    // printf("%u %u %u ", colorTableR[index], colorTableG[index], colorTableB[index]);
                }
            }
            else
            {
                unsigned int index = atoi(token);
                fprintf(outputFile, "%u %u %u ", colorTableR[index], colorTableG[index], colorTableB[index]);
                // printf("%u %u %u ", colorTableR[index], colorTableG[index], colorTableB[index]);
            }
        }

        fprintf(outputFile, "\n");
        // printf("\n");

        free(colorTableR);
        free(colorTableG);
        free(colorTableB);
        fclose(outputFile);
    }

    fclose(inputFile);
}

void CreateSBUFiles(const char *inputFilename, const char *outputFilename)
{
    char header[10000];
    unsigned int width, height;

    FILE *inputFile = fopen(inputFilename, "r");
    if (!inputFile)
    {
        return;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile)
    {
        fclose(inputFile);
        return;
    }

    if (fgets(header, sizeof(header), inputFile) == NULL)
    {
        fclose(inputFile);
        fclose(outputFile);
        return;
    }
    if (strncmp(header, "P3", 2) == 0)
    {
        unsigned int maxColor;
        fscanf(inputFile, "%u %u %u", &width, &height, &maxColor);

        unsigned int *colorTableR = malloc(256 * 256 * 256 * sizeof(unsigned int));
        unsigned int *colorTableG = malloc(256 * 256 * 256 * sizeof(unsigned int));
        unsigned int *colorTableB = malloc(256 * 256 * 256 * sizeof(unsigned int));
        if (!colorTableR || !colorTableG || !colorTableB)
        {
            free(colorTableR);
            free(colorTableG);
            free(colorTableB);
            fclose(inputFile);
            fclose(outputFile);
            return;
        }
        unsigned int colorTableSize = 0;

        unsigned int *imageData = malloc(width * height * sizeof(unsigned int));
        if (!imageData)
        {
            free(colorTableR);
            free(colorTableG);
            free(colorTableB);
            fclose(inputFile);
            fclose(outputFile);
            return;
        }
        unsigned int r, g, b;
        for (unsigned int i = 0; i < width * height; i++)
        {
            fscanf(inputFile, "%u %u %u", &r, &g, &b);

            int colorIndex = -1;
            for (unsigned int j = 0; j < colorTableSize; j++)
            {
                if (colorTableR[j] == r && colorTableG[j] == g && colorTableB[j] == b)
                {
                    colorIndex = j;
                    break;
                }
            }

            if (colorIndex == -1)
            {
                colorIndex = colorTableSize;
                colorTableR[colorTableSize] = r;
                colorTableG[colorTableSize] = g;
                colorTableB[colorTableSize] = b;
                colorTableSize++;
            }

            imageData[i] = colorIndex;
        }
        fprintf(outputFile, "SBU\n%u %u\n%u\n", width, height, colorTableSize);

        for (unsigned int i = 0; i < colorTableSize; i++)
        {
            fprintf(outputFile, "%u %u %u ", colorTableR[i], colorTableG[i], colorTableB[i]);
        }
        fprintf(outputFile, "\n");

        for (unsigned int i = 0; i < width * height; i++)
        {
            fprintf(outputFile, "%u ", imageData[i]);
        }

        free(colorTableR);
        colorTableR = NULL;
        free(colorTableG);
        colorTableG = NULL;
        free(colorTableB);
        colorTableB = NULL;
        free(imageData);
        imageData = NULL;
        fclose(outputFile);
    }
    else if (strncmp(header, "SBU", 3) == 0)
    {
        fprintf(outputFile, "SBU\n");
        if (fscanf(inputFile, "%u %u\n", &width, &height) != 2)
        {
            fclose(inputFile);
            fclose(outputFile);
            return;
        }
        fprintf(outputFile, "%u %u\n", width, height);
        // printf("%u %u\n", width, height);

        unsigned int colorTableSize;
        fscanf(inputFile, "%u", &colorTableSize);
        fprintf(outputFile, "%u\n", colorTableSize);
        // printf("%u\n", colorTableSize);

        for (unsigned int i = 0; i < colorTableSize; i++)
        {
            unsigned int r, g, b;

            if (fscanf(inputFile, "%u %u %u", &r, &g, &b) != 3)
            {
                fclose(inputFile);
                fclose(outputFile);
                return;
            }

            fprintf(outputFile, "%u %u %u ", r, g, b);
            // printf("%u %u %u ", r, g, b);
        }

        fprintf(outputFile, "\n");
        // printf("\n");

        char token[10];
        while (fscanf(inputFile, "%s", token) == 1)
        {
            if (token[0] == '*')
            {
                unsigned int count = atoi(token + 1);

                unsigned int index;
                if (fscanf(inputFile, "%u", &index) != 1)
                {
                    fclose(inputFile);
                    fclose(outputFile);
                    return;
                }

                fprintf(outputFile, "*%u %u ", count, index);
                // printf("*%u %u ", count, index);
            }
            else
            {
                unsigned int index = atoi(token);
                fprintf(outputFile, "%u ", index);
                // printf("%u ", index);
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}
