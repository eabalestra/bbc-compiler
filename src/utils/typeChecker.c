#include "../../include/typeChecker.h"

Node *currentMethod; // variable global para la funcion actual en la que estoy

Type currentMethodType;
int flagReturn = 0;
Type checkTypes(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return NONTYPE;
    }

    Node *root = tree->root;
    Type hi;
    Type hd;

    switch (root->flag)
    {
        case NUMBER:
            return INTEGER;
            break;
        case BOOL:
            return BOOLEAN;
            break;
        case PARAM:
        case ID:
            return root->type;
            break;
        // Expr cases
        case MINUS:
            hi = checkTypes(tree->left);
            
            if(hi != INTEGER)
            {
                printf("ERROR. MINUS CON UN NONTYPE\n");
                exit(1);
            }

            return INTEGER;
            break;

        case MOD:
        case PLUS:
        case MULTIPLY:
        case DIVISION:
            hi = checkTypes(tree->left);
            hd = checkTypes(tree->right);


            if(hi != INTEGER || hd != INTEGER)
            {
                printf("OPERADOR INTEGER CON VALORES NO INTEGER\n");
                exit(1);
            }
            return INTEGER;
            break;
            
        case AND:
        case OR:
            hi = checkTypes(tree->left);
            hd = checkTypes(tree->right);
            if(hi != BOOLEAN || hd != BOOLEAN)
            {
                printf("OPERADOR BOOLEANO CON VALORES NO BOOLEANOS\n");
                exit(1);
            }
            return BOOLEAN;
            break;
        case NOT:
            hi = checkTypes(tree->left);

            if(hi != BOOLEAN){
                printf("ERROR.3\n");
                exit(1);
            }
            return BOOLEAN;
            break;

        case GRATERTHAN:
        case LESSTHAN:
            hi = checkTypes(tree->left);
            hd = checkTypes(tree->right);

            if(hi != INTEGER || hd != INTEGER)
            {
                printf("OPERADOR INTEGER CON VALORES NO INTEGER\n");
                exit(1);
            }

            return BOOLEAN;
            break;

        case ASSIGN:
            hi = checkTypes(tree->left);
            hd = checkTypes(tree->right);
            if(hi != hd)
            {
                printf("ERROR. ASSIGN\n");
                exit(1);
            }
            break;
        case EQUALS:
            hi = checkTypes(tree->left);
            hd = checkTypes(tree->right);
            if(hi != hd)
            {
                printf("ERROR. EQUALS\n");
                exit(1);
            }
            return BOOLEAN;
            break;

        case VARDECL:
            hi = checkTypes(tree->left);
            hd = checkTypes(tree->right);
            if(hd == NONTYPE)
            {
                return;
            }
            if(hi != hd)
            {
                printf("ERROR. VARDECL\n");
                exit(1);
            }
            break;
        case RETURN:
            hi = checkTypes(tree->left);
            if (hi != currentMethodType)
            {
                printf("ERROR TYPE RETURN\n");
                exit(1);
            }
            flagReturn = 1;
            break;
        case EXTERN:
            flagReturn = 1;
            break;
        case METHODCALL:
            Tree *formalParameters = tree->left->root->parameters;
            Tree *actualParameters = tree->right;
            checkParameters(tree->left, formalParameters, actualParameters);
            return checkTypes(tree->left);
            break;
        case METHODDECL:
            currentMethodType = checkTypes(tree->left);
            checkTypes(tree->right);
            if (currentMethodType != VOID && flagReturn == 0)
            {
                printf("ERROR. NO RETURN\n");
                exit(1);
            }
            flagReturn = 0;
            break;
        default:
            break;
    }
    checkTypes(tree->left);
    checkTypes(tree->right);
}

/**
 *
 */
Type getExprType(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Node *node = tree->root;
    Tag nodeFlag = node->flag;

    if (nodeFlag == METHODCALL)
    {
        return getExprType(tree->left); // chequea el tipo del metodo
    }
    if (nodeFlag == ID || nodeFlag == NUMBER || nodeFlag == BOOL || nodeFlag == PARAM)
    {
        return node->type;
    }

    Type leftType = getExprType(tree->left);
    Type rightType = getExprType(tree->right);

    if (nodeFlag == PLUS || nodeFlag == MINUS || nodeFlag == MULTIPLY || nodeFlag == MOD || nodeFlag == DIVISION)
    {
        if (leftType == INTEGER && rightType == INTEGER)
        {
            return INTEGER;
        }
    }
    if (nodeFlag == AND || nodeFlag == OR || nodeFlag == NOT)
    {
        if (leftType == BOOLEAN && rightType == BOOLEAN)
        {
            return BOOLEAN;
        }
    }
    if (nodeFlag == NOT)
    {
        if (leftType == BOOLEAN)
        {
            return BOOLEAN;
        }
    }
    if (nodeFlag == EQUALS)
    {
        if (leftType == BOOLEAN && rightType == BOOLEAN || leftType == INTEGER && rightType == INTEGER)
        {
            return BOOLEAN;
        }
    }
    if (nodeFlag == GRATERTHAN || nodeFlag == LESSTHAN)
    {
        if (leftType == INTEGER && rightType == INTEGER)
        {
            return BOOLEAN;
        }
    }
    else
    {
        printf("Type Error [Line %d]: Operator '%s' is not defined for types '%s' and '%s'.\n",
               node->line_number, nodeFlagToString(nodeFlag), nodeTypeToString(leftType), nodeTypeToString(rightType));
        exit(1);
    }
}

void checkParameters(Tree *method, Tree *formalParameters, Tree *actualParameters)
{
    while (formalParameters != NULL && actualParameters != NULL && actualParameters->root->flag != EMPTY)
    {
        Tree *expression = actualParameters->left;

        Node *formalParameter = formalParameters->root;
        Node *actualParameterNode = expression->root;
        Type actualParameterType = getExprType(expression);

        if (formalParameter->type != actualParameterType)
        {
            printf("Type Error [Line %d]: Parameter '%s' is declared as type '%s', "
                   "but is assigned a value of incompatible type '%s'.\n",
                   actualParameterNode->line_number, formalParameter->name,
                   nodeTypeToString(formalParameter->type), nodeTypeToString(actualParameterType));
            exit(1);
        }

        formalParameters = formalParameters->left;
        actualParameters = actualParameters->right;
    }
    if (formalParameters != NULL)
    {
        printf("Type Error [Line %d]: Too few parameters in method call.\n", method->root->line_number);
        exit(1);
    }
    if (actualParameters != NULL && actualParameters->root->flag != EMPTY)
    {
        printf("Type Error [Line %d]: Too many parameters in method call.\n", method->root->line_number);
        exit(1);
    }
}