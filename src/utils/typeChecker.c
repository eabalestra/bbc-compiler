#include "../../include/typeChecker.h"

void checkTypes(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Tree *leftChild = tree->left;
    Tree *rightChild = tree->right;

    if (rightChild != NULL && (tree->root->flag == VARDECL || tree->root->flag == ASSIGN || tree->root->flag == EQUALS 
        || tree->root->flag == LESSTHAN || tree->root->flag == GRATERTHAN || tree->root->flag == AND || tree->root->flag == OR))
        {

        
        Type rightChildType;
        Node *rightChildNode = rightChild->root;
        //new
        Node *leftChildNode = leftChild->root;
        
        //TODO: HACER UNA FUNCIÓN VOID PARA LO SIGUIENTE.
        
        if(tree->root->flag == LESSTHAN || tree->root->flag == GRATERTHAN){
            

            if(leftChild->root->type == BOOLEAN || rightChild->root->type == BOOLEAN)
            {
                printf("Type Error [Line %d]: Arithmetic operator with boolean values.\n", leftChildNode->line_number);
                exit(1);
            }
         
        }
        if((tree->root->flag == AND || tree->root->flag == OR)){

            if(leftChild->root->type == INTEGER || rightChild->root->type == INTEGER)
            {    
                printf("Type Error [Line %d]: Boolean operator with integer values.\n", leftChildNode->line_number);
                exit(1);
            }
        }
        //

        if (rightChildNode->flag == ID || rightChildNode->flag == NUMBER || rightChildNode->flag == BOOL)
        {
            rightChildType = rightChild->root->type;
        }
        else
        {
            rightChildType = checkExpressionTypes(rightChild);
        }

        if (leftChild->root->type != rightChildType)
        {            
            printf("Type Error [Line %d]: Variable '%s' is declared as type '%s', "
                   "but is assigned a value of incompatible type '%s'.\n",
                   leftChildNode->line_number, leftChildNode->name,
                   nodeTypeToString(leftChildNode->type), nodeTypeToString(rightChildType));
            exit(1);
        }
    }

    // TODO
    // caso return
    // caso parametros
    // caso condiciones creo que funciona?

    checkTypes(tree->left);
    checkTypes(tree->right);
}

Type checkExpressionTypes(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Node *node = tree->root;
    Tag nodeFlag = node->flag;

    if (nodeFlag == METHODCALL)
    {
        return checkExpressionTypes(tree->left); // chequea el tipo del metodo
        
    }
    if (nodeFlag == ID || nodeFlag == NUMBER || nodeFlag == BOOL)
    {
        return node->type;
    }

    Type leftType = checkExpressionTypes(tree->left);
    Type rightType = checkExpressionTypes(tree->right);

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


