
#include "avl.h"
#include "definiciones.h"
#include <stdio.h>
#include <string.h>

static void _print_element(element data) {
    printf("%d ", data.lexical_token);
}

int main(void){

    AVLTree* tree1 = avl_create();
    AVLTree* tree2 = avl_create();
    AVLTree* tree3 = avl_create();

    if (!tree1 || !tree2 || !tree3) {
        printf("Error al crear el árbol AVL\n");
        return -1;
    }
 
    element data1  = {1,  "01"};
    element data2  = {2,  "02"};
    element data3  = {3,  "03"};
    element data4  = {4,  "04"};
    element data5  = {5,  "05"};
    element data6  = {6,  "06"};
    element data7  = {7,  "07"};
    element data8  = {8,  "08"};
    element data9  = {9,  "09"};
    element data10 = {10, "10"};
    element data11 = {11, "11"};
    element data12 = {12, "12"};
    element data13 = {13, "13"};
    element data14 = {14, "14"};
    element data15 = {15, "15"};
    element data16 = {16, "16"};
    element data17 = {17, "17"};
    element data18 = {18, "18"};
    element data19 = {19, "19"};
    element data20 = {20, "20"};
    element data21 = {21, "21"};
    element data22 = {22, "22"};
    element data23 = {23, "23"};
    element data24 = {24, "24"};
    element data25 = {25, "25"};
    element data26 = {26, "26"};
    element data27 = {27, "27"};
    element data28 = {28, "28"};
    element data29 = {29, "29"};
    element data30 = {30, "30"};

    printf("%d\n", strcmp("15", "20"));


    avl_insert(tree1, data20);
    avl_insert(tree1, data4);

    avl_insert(tree2, data20);
    avl_insert(tree2, data4);
    avl_insert(tree2, data26);
    avl_insert(tree2, data3);
    avl_insert(tree2, data9);

    avl_insert(tree3, data20);
    avl_insert(tree3, data4);
    avl_insert(tree3, data26);
    avl_insert(tree3, data3);
    avl_insert(tree3, data9);
    avl_insert(tree3, data21);
    avl_insert(tree3, data30);
    avl_insert(tree3, data2);
    avl_insert(tree3, data7);
    avl_insert(tree3, data11);
    
    
    printf("Arbol AVL 1:\n");

    printf("Inorden: ");
    avl_traverse(tree1, INORDER, _print_element);

    printf("\nPreorden: ");
    avl_traverse(tree1, PREORDER, _print_element);

    avl_insert(tree1, data15);

    printf("\nInsertamos 15\nInorden: ");
    avl_traverse(tree1, INORDER, _print_element);

    printf("\nPreorden: ");
    avl_traverse(tree1, PREORDER, _print_element);

    /////
    printf("\n\nArbol AVL 2:\n");

    printf("Inorden: ");
    avl_traverse(tree2, INORDER, _print_element);

    printf("\nPreorden: ");
    avl_traverse(tree2, PREORDER, _print_element);

    avl_insert(tree2, data15);

    printf("\nInsertamos 15\nInorden: ");
    avl_traverse(tree2, INORDER, _print_element);

    printf("\nPreorden: ");
    avl_traverse(tree2, PREORDER, _print_element);

    /////
    printf("\n\nArbol AVL 3:\n");

    printf("Inorden: ");
    avl_traverse(tree3, INORDER, _print_element);

    printf("\nPreorden: ");
    avl_traverse(tree3, PREORDER, _print_element);

    avl_insert(tree3, data15);

    printf("\nInsertamos 15\nInorden: ");
    avl_traverse(tree3, INORDER, _print_element);

    printf("\nPreorden: ");
    avl_traverse(tree3, PREORDER, _print_element);


    avl_destroy(tree1);
    avl_destroy(tree2);
    avl_destroy(tree3);

    return 0;

    
    
}

