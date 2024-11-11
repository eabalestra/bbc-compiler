#include "../../include/assemblyCodeGenerator.h"
#include "../../include/tree.h"

char *getPlantillaRegister(int offset);
void generateAddPlantilla(FILE *file, Quadruple *current);

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

        char *result = getValueToString(current->result);
        char *arg1 = getValueToString(current->arg1);
        char *arg2 = getValueToString(current->arg2);

        switch (current->op)
        {
        case PLUS:

            break;

        case ASSIGN:
            break;
        case GASSIGN:
            fprintf(file, "%s:\n", current->result->name);
            fprintf(file, "    .long %d\n", current->arg2->value);
            break;

        case INITMETHOD:
            if (strcmp(current->arg1->name, "main") == 0)
            {
                fprintf(file, "    .globl main\n");
            }

            fprintf(file, "\n%s:\n", current->arg1->name);
            int offset = current->arg1->offset;
            fprintf(file, "    enter   $(8 * %d), $0 \n", offset);

            Tree *methodParameters = current->arg1->parameters;

            while (methodParameters != NULL)
            {
                int parameterOffset = methodParameters->root->offset;
                char *registerName = getPlantillaRegister(parameterOffset);
                fprintf(file, "    movl   %s, -%d(%%rbp)\n", registerName, parameterOffset * 8);
                methodParameters = methodParameters->left;
            }

            fprintf(file, "\n");
            break;

        case RETURN:
            fprintf(file, "    ret");
            fprintf(file, "\n");
            break;

        default:
            break;
        }
        quadList = quadList->next;
    }

    fprintf(file, ".ident   \"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0\" \n");
}

void generateAddition(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%r10\n", arg1);
    fprintf(file, "    addl   %s, %%r10\n", arg2);
    fprintf(file, "    movl   %%r10, %s\n", result);
}

void generateAddPlantilla(FILE *file, Quadruple *current)
{
    // Suma con literales.
    //  -%d(%%rbp) -> parameterOffset * 8
    fprintf(file, "    movl   -%d(%%rbp), %%r10\n", current->arg1->offset * 8);
    fprintf(file, "    addl   %d, %%r10\n", current->arg2->value); //%s -> literal
    fprintf(file, "    movl   %%r10, -%d(%%rbp)\n", current->arg1->offset * 8);
}

void generateAddition2(FILE *file, Quadruple *current)
{
    if (current->arg1->type == INTEGER || current->arg1->type == BOOLEAN)
    {
        fprintf(file, "    movl   -%d(%%rbp), %%r10\n", current->arg1->offset * 8);
    } else {
        fprintf(file, "    movl   $%s, %%r10\n", current->arg1->value);
    }

    fprintf(file, "    addl   %d, %%r10\n", current->arg2->value); 
    
    if (current->arg1->type == INTEGER || current->arg1->type == BOOLEAN)
    {
        fprintf(file, "    movl   %%r10, -%d(%%rbp)\n", current->arg1->offset * 8);    
    } else {
        fprintf(file, "    movl   $%s, %%r10\n", current->arg1->value);
    }
}

char *getPlantillaRegister(int offset)
{
    switch (offset)
    {
    case 1:
        return "%rdi";
    case 2:
        return "%rsi";
    case 3:
        return "%rdx";
    case 4:
        return "%rcx";
    case 5:
        return "%r8";
    case 6:
        return "%r9";
    default:
        return "error";
    }
}

char *getValueToString(Node *node)
{
    Tag tag = node->flag;
    char *result = NULL;
    switch (tag)
    {
    case BOOL:
    case INTEGER:
        return snprintf(result, 12, "%d", node->value);
    default:
        return;
    }
}
