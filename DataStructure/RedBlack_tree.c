#include <stdio.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

typedef struct RBNode {
    int key;
    int color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

RBNode *root = NULL;

RBNode* createNode(int key) {
    RBNode *newNode = (RBNode*)malloc(sizeof(RBNode));
    newNode->key = key;
    newNode->color = RED;  // 새 노드는 빨간색으로 시작
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

// 좌회전
void rotateLeft(RBNode **root, RBNode *x) {
    RBNode *y = x->right;
    
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

// 우회전
void rotateRight(RBNode **root, RBNode *y) {
    RBNode *x = y->left;
    
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    
    x->parent = y->parent;
    
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    
    x->right = y;
    y->parent = x;
}

// 삽입 후 균형 조정
void fixInsert(RBNode **root, RBNode *z) {
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;  // 삼촌 노드
            
            if (y != NULL && y->color == RED) {
                // Case 1: 삼촌이 빨간색
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Case 2: z가 오른쪽 자식
                    z = z->parent;
                    rotateLeft(root, z);
                }
                // Case 3: z가 왼쪽 자식
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(root, z->parent->parent);
            }
        } else {
            // 대칭적인 경우
            RBNode *y = z->parent->parent->left;  // 삼촌 노드
            
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;  // 루트는 항상 검은색
}

// 노드 삽입
void insert(RBNode **root, int key) {
    RBNode *z = createNode(key);
    RBNode *y = NULL;
    RBNode *x = *root;
    
    // BST 삽입 과정
    while (x != NULL) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    
    z->parent = y;
    if (y == NULL)
        *root = z;  // 첫 번째 노드
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    
    // Red-Black 트리 속성 복구
    fixInsert(root, z);
}

// 중위 순회
void inorderTraversal(RBNode *node) {
    if (node != NULL) {
        inorderTraversal(node->left);
        printf("%d(%s) ", node->key, (node->color == RED) ? "R" : "B");
        inorderTraversal(node->right);
    }
}

// 트리 출력 (레벨별)
void printLevel(RBNode *node, int level, int targetLevel) {
    if (node == NULL) {
        if (level == targetLevel) {
            printf("    ");
        }
        return;
    }
    
    if (level == targetLevel) {
        printf("%d(%s) ", node->key, (node->color == RED) ? "R" : "B");
        return;
    }
    
    printLevel(node->left, level + 1, targetLevel);
    printLevel(node->right, level + 1, targetLevel);
}

int getHeight(RBNode *node) {
    if (node == NULL) return 0;
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

void printTree(RBNode *root) {
    int height = getHeight(root);
    printf("\n=== Red-Black Tree Structure ===\n");
    for (int i = 0; i < height; i++) {
        printf("Level %d: ", i);
        printLevel(root, 0, i);
        printf("\n");
    }
    printf("=================================\n");
}

// Red-Black 트리 속성 검증
int verifyBlackHeight(RBNode *node) {
    if (node == NULL) return 1;
    
    int leftHeight = verifyBlackHeight(node->left);
    int rightHeight = verifyBlackHeight(node->right);
    
    if (leftHeight == 0 || rightHeight == 0 || leftHeight != rightHeight)
        return 0;
    
    return leftHeight + (node->color == BLACK ? 1 : 0);
}

int verifyProperties(RBNode *node) {
    if (node == NULL) return 1;
    
    // 빨간 노드의 자식은 검은색이어야 함
    if (node->color == RED) {
        if ((node->left && node->left->color == RED) ||
            (node->right && node->right->color == RED)) {
            printf("Red property violation at node %d\n", node->key);
            return 0;
        }
    }
    
    return verifyProperties(node->left) && verifyProperties(node->right);
}

int main() {
    int choice, key;
    
    printf("=== Red-Black Tree Implementation ===\n");
    
    while (1) {
        printf("\n연산 선택:\n");
        printf("1. 노드 삽입\n");
        printf("2. 중위 순회 출력\n");
        printf("3. 트리 구조 출력\n");
        printf("4. 속성 검증\n");
        printf("5. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("삽입할 키 값: ");
                scanf("%d", &key);
                insert(&root, key);
                printf("키 %d가 삽입되었습니다.\n", key);
                break;
                
            case 2:
                printf("중위 순회: ");
                inorderTraversal(root);
                printf("\n");
                break;
                
            case 3:
                printTree(root);
                break;
                
            case 4:
                if (root && root->color != BLACK) {
                    printf("Root color violation!\n");
                } else if (verifyBlackHeight(root) == 0) {
                    printf("Black height violation!\n");
                } else if (!verifyProperties(root)) {
                    printf("Red property violation!\n");
                } else {
                    printf("모든 Red-Black 트리 속성을 만족합니다.\n");
                }
                break;
                
            case 5:
                printf("프로그램을 종료합니다.\n");
                return 0;
                
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    
    return 0;
}