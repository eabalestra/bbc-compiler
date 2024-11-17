#include "../../include/assemblyCodeGenerator.h"
#include "../../include/tree.h"

char *getRegisterNameByOffset(int offset);
char *getValueToString(Node *node);

void generateAddition(FILE *file, char *result, char *arg1, char *arg2);
void generateMultiplication(FILE *file, char *result, char *arg1, char *arg2);
void generateSubtraction(FILE *file, char *result, char *arg1, char *arg2);
void generateNegation(FILE *file, char *result, char *arg1);
void generateDivision(FILE *file, char *result, char *arg1, char *arg2);

void generateAnd(FILE *file, char *result, char *arg1, char *arg2);
void generateOr(FILE *file, char *result, char *arg1, char *arg2);
void generateNot(FILE *file, char *result, char *arg1);

void generateEquals(FILE *file, char *result, char *arg1, char *arg2);
void generateLessThan(FILE *file, char *result, char *arg1, char *arg2);

void generateGreaterThan(FILE *pFile, char *result, char *arg1, char *arg2);

void generateModule(FILE *pFile, char *result, char *arg1, char *arg2);

void generateMethodCall(FILE *pFile, char *arg1, char *arg2);

void generateLoadParameter(FILE *pFile, char *arg1, char *arg2);

void generateJumpByFalse(FILE *pFile, char *arg1, char *result);

void generateGoTo(FILE *pFile, char *arg1);

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

        printf("CURRENT OP: %s\n", nodeFlagToString(current->op));
        switch (current->op)
        {
        case PARAM:
            int orderOfParameter = current->arg2->value;
            char *registerParameterName = getRegisterNameByOffset(orderOfParameter);
            generateLoadParameter(file, arg1, registerParameterName);
            break;
        case MOD:
            generateModule(file, result, arg1, arg2);
            break;
        case PLUS:
            generateAddition(file, result, arg1, arg2);
            break;
        case MINUS:
            generateNegation(file, result, arg2);
            break;
        case SUBTRACTION:
            generateSubtraction(file, result, arg1, arg2);
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
        case NOT:
            generateNot(file, result, arg2);
            break;
        case LESSTHAN:
            generateLessThan(file, result, arg1, arg2);
            break;
        case GRATERTHAN:
            generateGreaterThan(file, result, arg1, arg2);
            break;
        case CALL:
            generateMethodCall(file, current->arg1->name, result);
            break;
        case JMPF:
            generateJumpByFalse(file, arg2, current->result->name);
            break;
        case GOTO:
            generateGoTo(file, current->arg1->name);
            break;
        case LABEL:
            fprintf(file, "%s:\n", current->arg1->name);
            break;
        case ASSIGN:
            fprintf(file, "    movq   %s, %%r10\n", arg2);
            fprintf(file, "    movq   %%r10, %s\n", result);
            fprintf(file, "\n");
            break;
        case GASSIGN:
            fprintf(file, "%s:\n", current->result->name);
            fprintf(file, "    .long %d\n", current->arg2->value);
            break;
        case EXTERNAL_CALL:
            fprintf(file, "    call   %s@PLT\n", current->arg1->name);
            fprintf(file, "    movl   %%eax, %s\n", result);
            break;
        case INITMETHOD:
            if (strcmp(current->arg1->name, "main") == 0)
            {
                fprintf(file, "\n    .globl main");
            }

            fprintf(file, "\n%s:\n", current->arg1->name);
            int offset = current->arg1->offset;
            fprintf(file, "    enter   $(8 * %d), $0 \n", offset);

            Tree *methodParameters = current->arg1->parameters;

            while (methodParameters != NULL)
            {
                int parameterOffset = methodParameters->root->offset;
                char *registerName = getRegisterNameByOffset(parameterOffset);
                fprintf(file, "    movq   %s, -%d(%%rbp)\n", registerName, parameterOffset * 8);
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
    fprintf(file, ".section .note.GNU-stack,\"\",@progbits\n");
}

/**
 * Generates assembly code for addition.
 *
 * @param file File pointer to the assembly output file.
 * @param result The result variable.
 * @param arg1 The first operand.
 */
void generateNot(FILE *file, char *result, char *arg1)
{
    fprintf(file, "    movl   %s, %%eax\n", arg1);   // Load the value of arg1 into %eax.
    fprintf(file, "    cmpl   $0, %%eax\n");         // Compare %eax with 0.
    fprintf(file, "    sete   %%al\n");              // If %eax is zero, set %eax to 1 (logical NOT).
    fprintf(file, "    movzbl %%al, %%eax\n");       // Zero-extend %al to %eax.
    fprintf(file, "    movl   %%eax, %s\n", result); // Store the result in the result variable.
    fprintf(file, "\n");
}

/**
 * Generates assembly code for addition.
 *
 * @param pfile File pointer to the assembly output file.
 * @param arg1 the label to jump to.
 */
void generateGoTo(FILE *pFile, char *arg1)
{
    fprintf(pFile, "    jmp    %s\n", arg1);
    fprintf(pFile, "\n");
}

/**
 * Generates assembly code for a conditional jump when the value is false (zero).
 *
 * @param pFile File pointer to the assembly output file.
 * @param arg1 The variable to evaluate (check if it's zero).
 * @param result The label to jump to if `arg1` is zero.
 */
void generateJumpByFalse(FILE *pFile, char *arg1, char *result)
{
    fprintf(pFile, "    cmpl   $0, %s\n", arg1);
    fprintf(pFile, "    je     %s\n", result);
    fprintf(pFile, "\n");
}

/**
 * Generates assembly code to load a parameter.
 *
 * @param pFile File pointer to the assembly output file.
 * @param arg1 The source variable to load.
 * @param arg2 The destination register or memory location where the parameter will be stored.
 */
void generateLoadParameter(FILE *pFile, char *arg1, char *arg2)
{
    fprintf(pFile, "    movq    %s, %s\n", arg1, arg2);
}

/**
 * Generates assembly code for a method call.
 *
 * @param pFile File pointer to the assembly output file.
 * @param arg1 The method to call.
 * @param arg2 The variable to store the method's return value (stored in `%eax`).
 */
void generateMethodCall(FILE *pFile, char *arg1, char *arg2)
{
    fprintf(pFile, "    call   %s\n", arg1);
    fprintf(pFile, "    movl   %%eax, %s\n", arg2);
    fprintf(pFile, "\n");
}

/**
 * Generates assembly code for the modulo operation.
 *
 * @param pFile File pointer to the assembly output file.
 * @param result The destination variable to store the modulo result.
 * @param arg1 The dividend variable.
 * @param arg2 The divisor variable.
 */
void generateModule(FILE *pFile, char *result, char *arg1, char *arg2)
{
    fprintf(pFile, "    movl   %s, %%eax\n", arg1);   // Load dividend (arg1) into %eax.
    fprintf(pFile, "    cltd\n");                     // Sign-extend %eax into %edx for division.
    fprintf(pFile, "    movq   %s, %%r10\n", arg2);   // Load divisor (arg2) into %r10.
    fprintf(pFile, "    idivq  %%r10\n");             // Perform signed division, result in %eax.
    fprintf(pFile, "    movl   %%edx, %s\n", result); // Store quotient from %eax into result.
    fprintf(pFile, "\n");
}

/**
 * Generates assembly code for less-than comparison.
 * @param pfile File pointer to the assembly output file.
 * @param result The variable to store the result.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateGreaterThan(FILE *pFile, char *result, char *arg1, char *arg2)
{
    fprintf(pFile, "    movl   %s, %%eax\n", arg1);   // Load the first operand (arg1) into %eax.
    fprintf(pFile, "    cmpl   %s, %%eax\n", arg2);   // Compare %eax (arg1) with arg2.
    fprintf(pFile, "    setg   %%al\n");              // Set %al (lower byte of %eax) to 1 if %eax > arg2, else 0.
    fprintf(pFile, "    movzbl %%al, %%eax\n");       // Zero-extend %al into %eax (ensures the result is a full 32-bit value).
    fprintf(pFile, "    movl   %%eax, %s\n", result); // Store the result into the destination (result).
    fprintf(pFile, "\n");
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
    fprintf(file, "    movq   %s, %%r10\n", arg1);
    fprintf(file, "    addq   %s, %%r10\n", arg2);
    fprintf(file, "    movq   %%r10, %s\n", result);
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
    fprintf(file, "    movq   %s, %%r10\n", arg1);
    fprintf(file, "    imulq   %s, %%r10\n", arg2);
    fprintf(file, "    movq   %%r10, %s\n", result);
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
    fprintf(file, "    movl   %s, %%eax\n", arg1); // Load operand into r10.
    fprintf(file, "    negl   %%eax\n");           // Negate the value in r10.
    fprintf(file, "    movl   %%eax, %s\n", result); // Store the result.
    fprintf(file, "\n");
}

/**
 * Generates assembly code for subtraction: result = arg1 - arg2.
 * @param file The output file to write the assembly code.
 * @param result The variable to store the result of the subtraction.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 */
void generateSubtraction(FILE *file, char *result, char *arg1, char *arg2)
{
    fprintf(file, "    movq   %s, %%r10\n", arg1);
    fprintf(file, "    subq   %s, %%r10\n", arg2);
    fprintf(file, "    movq   %%r10, %s\n", result);
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
    fprintf(file, "    movq   %s, %%eax\n", arg1);   // Load dividend (arg1) into %eax.
    fprintf(file, "    cltd\n");                     // Sign-extend %eax into %edx for division.
    fprintf(file, "    movq   %s, %%r10\n", arg2);   // Load divisor (arg2) into %r10.
    fprintf(file, "    idivq  %%r10\n");             // Perform signed division, result in %eax.
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
    fprintf(file, "    movl   %s, %%eax\n", arg1);
    fprintf(file, "    cmpl   %s, %%eax\n", arg2);
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
    if (result == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

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
    default:
        break;
    }

    return result;
}