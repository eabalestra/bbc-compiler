#include "../../include/symbolTable.h"

int main() 
{
    SymbolTable *table = createSymbolTable();
    /* Node *result = findSymbolNode("x", table, 0);
    printf("\ndeberia ser null: %d\n", result == NULL); */

    /* pushLevelToSymbolTable(table);
    printf("niveles que tiene, deberia ser 0: %d --\n", table->levels);
    
    popLevelFromSymbolTable(table);
    printf("pop del nivel, deberia ser -1: %d --\n", table->levels);

    pushLevelToSymbolTable(table);
    printf("cuantos niveles tiene?: %d --\n", table->levels); */

    Node *node = createNode(PROG, NONTYPE, NULL, "x", -1);
    pushLevelToSymbolTable(table);
    insertSymbolInSymbolTable(node, table, 0);
    printf("nodos en el level 0: %d \n", table->levelData->size);

    Node *result = findSymbolNode("x", table, 0);
    printf("nodo encontrado: %s \n", result->name);
    
    Node *node2 = createNode(PROG, NONTYPE, NULL, "y", -1);
    pushLevelToSymbolTable(table);
    insertSymbolInSymbolTable(node2, table, 1);
    result = findSymbolNode("y", table, 0);


    printf("nodos en el level 0: %d \n", table->levelData->size);
    printf("%d\n", result == NULL);
    //printf("nodo encontrado: %s \n", result->name);

    printf("niveles que tiene: %d --\n", table->levels);
    printSymbolTable(table);
    return 0;
}