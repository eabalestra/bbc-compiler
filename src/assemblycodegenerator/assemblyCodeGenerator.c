#include "../../include/assemblyCodeGenerator.h"
#include "../../include/tree.h"

char *getRegisterNameByOffset(int offset);
void generateAddition(FILE *file, char *result, char *arg1, char *arg2);
void generateMultiplication(FILE *file, char *result, char *arg1, char *arg2);
char *getValueToString(Node *node);

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
            generateAddition(file, result, arg1, arg2);
            fprintf(file, "\n");
            break;

        case MULTIPLY:
            generateMultiplication(file, result, arg1, arg2);
            fprintf(file, "\n");
            break;

        case ASSIGN:
            fprintf(file, "    movl   %s, %%r10\n", arg2);
            fprintf(file, "    movl   %%r10, %s\n", result);
            fprintf(file, "\n");
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
                char *registerName = getRegisterNameByOffset(parameterOffset);
                fprintf(file, "    movl   %s, -%d(%%rbp)\n", registerName, parameterOffset * 8);
                methodParameters = methodParameters->left;
            }

            fprintf(file, "\n");
            break;

        case RETURN:
            if (current->arg2 == NULL)
            {
                fprintf(file, "    mov   $0, %%rax \n");
            }
            else
            {
                fprintf(file, "    mov   %s, %%rax \n", arg2);
            }
            fprintf(file, "    leave \n");
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

void generateMultiplication(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%r10\n", arg1);
    fprintf(file, "    imull   %s, %%r10\n", arg2);
    fprintf(file, "    movl   %%r10, %s\n", result);
}

char *getRegisterNameByOffset(int offset)
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
    if (node == NULL)
    {
        return "";
    }

    char *result = malloc(100);

    Tag tag = node->flag;

    switch (tag)
    {
    case NUMBER:
    case BOOL:
        sprintf(result, "$%d", node->value);
        break;

    case TEMP:
    case PARAM:
    case ID:
        sprintf(result, "-%d(%%rbp)", node->offset * 8);
        break;
    }

    return result;
}