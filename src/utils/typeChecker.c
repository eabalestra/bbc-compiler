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
    Type hiType;
    Type hdType;

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
            hiType = checkTypes(tree->left);
            
            if (tree->right == NULL) { // unary minus case
                if(hiType != INTEGER)
                {
                    printf("Type Mismatch Error [Line %d]: Unary '-' operator requires INTEGER type, but got '%s'.\n",
                    root->line_number, nodeTypeToString(hiType));
                    exit(1);
                }
            } else {
                hdType = checkTypes(tree->right);
                if (hiType != INTEGER || hdType != INTEGER) {
                    printf("Type Mismatch Error [Line %d]: Binary '-' operator requires INTEGER types, but got '%s' and '%s'.\n",
                    root->line_number, nodeTypeToString(hiType), nodeTypeToString(hdType));
                    exit(1);
                }
            }
            return INTEGER;
            break;

        case MOD:
        case PLUS:
        case MULTIPLY:
        case DIVISION:
            hiType = checkTypes(tree->left);
            hdType = checkTypes(tree->right);


            if(hiType != INTEGER || hdType != INTEGER)
            {
                printf("Type Mismatch Error [Line %d]: Binary '%s' operator requires INTEGER types, but got '%s' and '%s'.\n",
                root->line_number, nodeFlagToString(root->flag), nodeTypeToString(hiType), nodeTypeToString(hdType));
                exit(1);
            }
            return INTEGER;
            break;
            
        case AND:
        case OR:
            hiType = checkTypes(tree->left);
            hdType = checkTypes(tree->right);
            if(hiType != BOOLEAN || hdType != BOOLEAN)
            {
                printf("Type Mismatch Error [Line %d]: Binary '%s' operator requires BOOLEAN types, but got '%s' and '%s'.\n",
                root->line_number, nodeFlagToString(root->flag), nodeTypeToString(hiType), nodeTypeToString(hdType));
                exit(1);
            }
            return BOOLEAN;
            break;
        case NOT:
            hiType = checkTypes(tree->left);

            if(hiType != BOOLEAN){
                printf("Type Mismatch Error [Line %d]: Unary '%s' operator requires BOOLEAN types, but got '%s'.\n",
                root->line_number, nodeFlagToString(root->flag), nodeTypeToString(hiType));
                exit(1);
            }
            return BOOLEAN;
            break;

        case GRATERTHAN:
        case LESSTHAN:
            hiType = checkTypes(tree->left);
            hdType = checkTypes(tree->right);

            if(hiType != INTEGER || hdType != INTEGER)
            {
                printf("Type Mismatch Error [Line %d]: Binary '%s' operator requires INTEGER types, but got '%s' and '%s'.\n",
                root->line_number, nodeFlagToString(root->flag), nodeTypeToString(hiType), nodeTypeToString(hdType));
                exit(1);
            }

            return BOOLEAN;
            break;

        case ASSIGN:
            hiType = checkTypes(tree->left);
            hdType = checkTypes(tree->right);
            if(hiType != hdType)
            {
                printf("Type Mismatch Error [Line %d]: Assignment type mismatch. Cannot assign '%s' to '%s'.\n", 
                root->line_number, nodeTypeToString(hdType), nodeTypeToString(hiType));
                exit(1);
            }
            break;
        case EQUALS:
            hiType = checkTypes(tree->left);
            hdType = checkTypes(tree->right);
            if(hiType != hdType)
            {
                printf("Type Mismatch Error [Line %d]: Binary '%s' operator requires matching types, but got '%s' and '%s'.\n",
                root->line_number, nodeFlagToString(root->flag), nodeTypeToString(hiType), nodeTypeToString(hdType));
                exit(1);
            }
            return BOOLEAN;
            break;

        case VARDECL:
            hiType = checkTypes(tree->left);
            hdType = checkTypes(tree->right);
            if(hdType == NONTYPE)
            {
                return;
            }
            if(hiType != hdType)
            {
                printf("Type Mismatch Error [Line %d]: Declaration type mismatch. Cannot assign '%s' to '%s'.\n", 
                root->line_number, nodeTypeToString(hdType), nodeTypeToString(hiType));
                exit(1);
            }
            break;
        case RETURN:
            if (currentMethodType == VOID) {
                if (tree->left != NULL) {
                    printf("Type Mismatch Error [Line %d]: Void function should not return a value.\n", 
                    root->line_number);
                    exit(1);
                }
            } else {
                if (tree->left == NULL) {
                    printf("Type Mismatch Error [Line %d]: Return type mismatch, expected '%s' but got nothing.\n",
                    root->line_number, nodeTypeToString(currentMethodType));
                    exit(1);
                }
                hiType = checkTypes(tree->left);
                printf("TYPE: %s\n",  nodeTypeToString(hiType));
                if (hiType != currentMethodType)
                {
                    printf("Type Mismatch Error [Line %d]: Return type mismatch, expected '%s' but got '%s'.\n",
                    root->line_number, nodeTypeToString(currentMethodType), nodeTypeToString(hiType));
                    exit(1);
                }
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
                printf("Type Mismatch Error [Line %d]: Method '%s' expected a return statement of type '%s' and got none.\n",
                root->line_number, tree->left->root->name, nodeTypeToString(currentMethodType));
                exit(1);
            }
            flagReturn = 0;
            break;
        case IF:
        case WHILE:
            hiType = checkTypes(tree->left);
            if (hiType != BOOLEAN) {
                printf("Type Mismatch Error [Line %d]: Conditional mismatch. Condition type should be 'bool' but found '%s'.\n",
                root->line_number, nodeTypeToString(hiType));
                exit(1);
            }
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
        printf("Type Mismatch Error [Line %d]: Operator '%s' is not defined for types '%s' and '%s'.\n",
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
        printf("Type Error [Line %d]: Too few parameters in method call for method '%s'.\n", method->root->line_number, method->root->name);
        exit(1);
    }
    if (actualParameters != NULL && actualParameters->root->flag != EMPTY)
    {
        printf("Type Error [Line %d]: Too many parameters in method call for method '%s'.\n", method->root->line_number, method->root->name);
        exit(1);
    }
}