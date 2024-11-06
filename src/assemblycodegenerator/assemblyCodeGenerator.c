#include "../../include/assemblyCodeGenerator.h"

/**
 *
 */
void generateAssemblyCode(QuadrupleLinkedList *quadrupleLinkedList)
{
    char *fileName = "output.s";
    FILE *file = fopen(fileName, "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(file, ".file \"%s\"\n", fileName);
    fprintf(file, ".text\n");

    QuadrupleLinkedList *quadList = quadrupleLinkedList;

    while (quadList != NULL)
    {
        Quadruple *current = quadList->quadruple;
        switch (current->op)
        {
        case ASSIGN:

            break;
        case GASSIGN:
            fprintf(file, "%s:\n", current->result->name);
            fprintf(file, "    .long %d\n", current->arg2->value);
            break;

        case INITMETHOD:
            if (strcmp(current->arg1->name, "main") == 0)
            {
                fprintf(file, ".globl main\n");
            }
            fprintf(file, "%s:\n", current->arg1->name);
            break;

        case RETURN:

            break;
        default:
            break;
        }
        quadList = quadList->next;
    }

    fprintf(file, ".ident   \"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0\" \n");
}