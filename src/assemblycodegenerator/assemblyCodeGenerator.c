#include "../../include/assemblyCodeGenerator.h"
#include "../../include/tree.h"

char *getRegisterNameByOffset(int offset);
char *getValueToString(Node *node);

void generateAddition(FILE *file, char *result, char *arg1, char *arg2);
void generateMultiplication(FILE *file, char *result, char *arg1, char *arg2);
void generateSubstraction(file, result, arg1, arg2);
void generateNegation(FILE *file, char *result, char *arg1);
void generateDivision(FILE *file, char *result, char *arg1, char *arg2);

void generateAnd(FILE *file, char *result, char *arg1, char *arg2);
void generateOr(FILE *file, char *result, char *arg1, char *arg2);

void generateEquals(FILE *file, char *result, char *arg1, char *arg2);
void generateLessThan(FILE *file, char *result, char *arg1, char *arg2);

/**
 * Generates code from a quadruple linked list.
 * Iterates over the quadruples and generates assembly instructions.
 *
 * @param quadrupleLinkedList Pointer to the linked list of quadruples.
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
        case MOD:

            break;
        case PLUS:
            generateAddition(file, result, arg1, arg2);
            break;
        case MINUS:
            generateNegation(file, result, arg2);
            break;
        case SUBTRACTION:
            generateSubstraction(file, result, arg1, arg2);
            break;
        case MULTIPLY:
            generateMultiplication(file, result, arg1, arg2);
            break;
        case DIVISION:
            generateDivision(file, result, arg1, arg2);
            break;
        case AND:
            generateAnd(file, result, arg1, arg2);
            break;
        case OR:
            generateOr(file, result, arg1, arg2);
            break;
        case EQUALS:
            generateEquals(file, result, arg1, arg2);
            break;
        case LESSTHAN:
            generateLessThan(file, result, arg1, arg2);
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

/**
 * Generates assembly code for addition.
 *
 * @param file File pointer to the assembly output file.
 * @param result The result variable.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateAddition(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%r10\n", arg1);
    fprintf(file, "    addl   %s, %%r10\n", arg2);
    fprintf(file, "    movl   %%r10, %s\n", result);
    fprintf(file, "\n");
}

/**
 * Generates assembly code for multiplication.
 *
 * @param file File pointer to the assembly output file.
 * @param result The result variable.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateMultiplication(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%r10\n", arg1);
    fprintf(file, "    imull   %s, %%r10\n", arg2);
    fprintf(file, "    movl   %%r10, %s\n", result);
    fprintf(file, "\n");
}

/**
 * Generates assembly code for negating a value.
 *
 * @param file File pointer to the assembly output file.
 * @param result The result variable.
 * @param arg1  The first operand.
 */
void generateNegation(FILE *file, char *result, char *arg1)
{
    fprintf(file, "    movl   %s, %%r10\n", arg1); // Load operand into r10.
    fprintf(file, "    negl   %%r10\n");           // Negate the value in r10.
    fprintf(file, "    movl   %%r10, %s\n", arg1); // Store the result.
    fprintf(file, "\n");
}

/**
 * Generates assembly code for subtraction: result = arg1 - arg2.
 * @param file The output file to write the assembly code.
 * @param result The variable to store the result of the subtraction.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateSubstraction(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%r10\n", arg1);
    fprintf(file, "    subl   %s, %%r10\n", arg2);
    fprintf(file, "    movl   %%r10, %s\n", result);
    fprintf(file, "\n");
}

/**
 * Generates assembly code for AND operation.
 * @param file The output file to write the assembly code.
 * @param result The variable to store the result of the AND operation.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateAnd(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%eax\n", arg1);
    fprintf(file, "    movl   %s, %%ebx\n", arg1);
    fprintf(file, "    andl   %%ebx, %%eax\n");
    fprintf(file, "    movl   %%eax, %s\n", result);
    fprintf(file, "\n");
}

/**
 * Generates assembly code for OR operation.
 * @param file The output file to write the assembly code.
 * @param result The variable to store the result of the OR operation.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateOr(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%eax\n", arg1);
    fprintf(file, "    movl   %s, %%ebx\n", arg1);
    fprintf(file, "    orl   %%ebx, %%eax\n");
    fprintf(file, "    movl   %%eax, %s\n", result);
    fprintf(file, "\n");
}

/**
 * Generates assembly code for division.
 * @param file The output file to write the assembly code.
 * @param result The variable to store the quotient of the division.
 * @param arg1 The dividend.
 * @param arg2 The divisor.
 */
void generateDivision(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%eax\n", arg1);   // Load dividend (arg1) into %eax.
    fprintf(file, "    cltd\n");                     // Sign-extend %eax into %edx for division.
    fprintf(file, "    movl   %s, %%r10\n", arg2);   // Load divisor (arg2) into %r10.
    fprintf(file, "    idivl  %%r10\n");             // Perform signed division, result in %eax.
    fprintf(file, "    movl   %%eax, %s\n", result); // Store quotient from %eax into result.
    fprintf(file, "\n");
}

/**
 * Generates assembly code for equality comparison.
 * @param file The output file to write the assembly code.
 * @param result The variable to store the result.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateEquals(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%r10\n", arg1);
    fprintf(file, "    cmpl   %s, %%r10\n", arg2);
    fprintf(file, "    sete   %%al\n");
    fprintf(file, "    movzbl %%al, %%eax\n");
    fprintf(file, "    movl   %%eax, %s\n", result);
    fprintf(file, "\n");
}

/**
 * Generates assembly code for less-than comparison.
 * @param file The output file to write the assembly code.
 * @param result The variable to store the result.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateLessThan(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movl   %s, %%eax\n", arg1);   // Load the first operand (arg1) into %eax.
    fprintf(file, "    cmpl   %s, %%eax\n", arg2);   // Compare %eax (arg1) with arg2.
    fprintf(file, "    setl   %%al\n");              // Set %al (lower byte of %eax) to 1 if %eax < arg2, else 0.
    fprintf(file, "    movzbl %%al, %%eax\n");       // Zero-extend %al into %eax (ensures the result is a full 32-bit value).
    fprintf(file, "    movl   %%eax, %s\n", result); // Store the result into the destination (result).
    fprintf(file, "\n");
}

/**
 * Retrieves the register name corresponding to the given offset.
 * Used to map function parameters to registers during assembly generation.
 *
 * @param offset The parameter offset (1-based index).
 * @return A string representing the corresponding register name.
 */
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

/**
 * Converts a node value into a string suitable for assembly code.
 * Handles numbers, booleans, temporary variables, parameters, and identifiers.
 *
 * @param node Pointer to the Node containing the value to convert.
 * @return A string representing the node's value in assembly syntax.
 */
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