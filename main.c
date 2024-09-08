#include "SymbolTable.h"

int main() {
    SymbolTable *table = createTable();

    Node *node1 = createNode(ID, NONTYPE, "x", NULL);
    Node *node2 = createNode(ID, NONTYPE, "y", NULL);
    insert(table, node1);
    insert(table, node2);

    printSymbolTable(table);

    Node *node3 = createNode(ID, NONTYPE, "x", NULL);
    insert(table, node3);

    printSymbolTable(table);


    Node *wanted_node = search(table, "x");
    printf("ENCONTRE: %s\n", (char *) wanted_node->value);

    return 0;
}