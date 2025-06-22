#include <stdio.h>
#include <stdlib.h>

#define M_WAY 3

typedef struct BTNode {
    int n;
    int isLeaf;           //leaf node인 경우 1
    int isRoot;           //root node인 경우 1
    int keys[M_WAY];      //3-Way B-Tree이기 때문에 최대 2개의 키값을 갖는다
                          //split 등을 위하여 하기 위해 1개의 여유 키값을 갖도록 선언
    struct BTNode* childs[M_WAY+1]; //child node pointer의 개수도 갖는 이유로 +1
}BTNode;

BTNode* initBTNode();
BTNode* BTInsert(BTNode* root, int key);
BTNode* splitChild(BTNode* root);
void inorderTraversal(BTNode * root);

BTNode* initBTNode()
{
    int i;
    BTNode* newBTNode;
    newBTNode = (BTNode*)malloc(sizeof(BTNode));
    newBTNode->n = 0;
    newBTNode->isLeaf = 1;        //항상 leaf node에서 생기므로
    newBTNode->isRoot = 0;
    
    for (i = 0; i < M_WAY+1; i++)
        newBTNode->childs[i] = NULL;
    
    // keys 배열도 초기화
    for (i = 0; i < M_WAY; i++)
        newBTNode->keys[i] = 0;
    
    return newBTNode;
}

BTNode* BTInsert(BTNode* root, int key)
{
    int i, pos, mid;
    BTNode* split;
    
    //해당 노드가 leaf 노드일 때
    if (root->isLeaf)
    {
        //노드의 키 값들 중 key가 들어갈 위치 탐색
        for (pos = 0; pos < root->n; pos++)
            if (root->keys[pos] > key)
                break;
        
        //정렬을 통해 위치를 맞춘 후 pos 위치에 key를 삽입
        for (i = root->n; i > pos; i--)
            root->keys[i] = root->keys[i-1];
        root->keys[pos] = key;
        root->n++;
        
        //insertion 수행 후 노드의 키 값이 다 찼을 때
        //해당 노드가 root 노드인 경우에만 split 수행
        if (root->n == M_WAY && root->isRoot)
            return splitChild(root);
        //그냥 leaf 노드인 경우 상위 함수에서 split 수행
        return root;
    }
    //해당 노드가 leaf 노드가 아닐 때
    else
    {
        //노드의 키 값들 중 key가 들어갈 위치 탐색
        for (pos = 0; pos < root->n; pos++)
            if (root->keys[pos] > key)
                break;
        
        //해당 위치에 존재하는 child 노드로 들어가 leaf 노드까지 탐색
        root->childs[pos] = BTInsert(root->childs[pos], key);
        
        //insertion 수행 후 child 노드의 키의 개수가 다 찼을 때
        if (root->childs[pos]->n == M_WAY)
        {
            //child 노드에 대해 split 수행
            split = splitChild(root->childs[pos]);
            //split 수행 후 적절하게 parent 노드에 추가
            for (i = root->n; i > pos; i--)
            {
                root->keys[i] = root->keys[i-1];
                root->childs[i+1] = root->childs[i];
            }
            root->keys[pos] = split->keys[0];  // 수정: 0번 인덱스 사용
            root->childs[pos] = split->childs[0];
            root->childs[pos+1] = split->childs[1];
            root->n++;
        }
        //최종적으로 root 노드가 실제 b-tree의 root 노드인 경우 중에
        //키의 개수가 다 찼을 때 split 수행
        if (root->n == M_WAY && root->isRoot)
            return splitChild(root);
        
        return root;
    }
}

BTNode* splitChild(BTNode* root)
{
    int i, mid;
    BTNode* newParent;
    BTNode* newSibling;
    
    newParent = initBTNode();
    newParent->isLeaf = 0;
    //root 노드가 실제 root 노드였다면
    //newParent 노드가 실제 root 노드가 된다
    if (root->isRoot)
        newParent->isRoot = 1;
    root->isRoot = 0;
    
    //newSibling 노드는 root와 같은 level 상에 존재한다
    newSibling = initBTNode();
    newSibling->isLeaf = root->isLeaf;
    
    //root 노드의 중간 지점을 정하고 (M_WAY=3이므로 mid=1)
    mid = (M_WAY - 1) / 2;  // mid = 1
    
    // newSibling에 오른쪽 키와 자식들을 복사
    newSibling->keys[0] = root->keys[2];
    newSibling->n = 1;
    
    // 자식 노드들도 복사 (leaf가 아닌 경우에만)
    if (!root->isLeaf) {
        newSibling->childs[0] = root->childs[2];
        newSibling->childs[1] = root->childs[3];
    }
    
    // newParent에 중간 키 저장
    newParent->keys[0] = root->keys[1];
    newParent->n = 1;
    
    // root의 키 개수 조정
    root->n = 1;
    
    // 사용하지 않는 부분 초기화
    root->keys[1] = root->keys[2] = 0;
    if (!root->isLeaf) {
        root->childs[2] = root->childs[3] = NULL;
    }
    
    // newParent의 자식 노드들 설정
    newParent->childs[0] = root;
    newParent->childs[1] = newSibling;
    
    return newParent;
}

void inorderTraversal(BTNode * root)
{
    int i;
    
    if (root == NULL) return;
    
    printf("\n");
    for (i = 0; i < root->n; i++)
    {
        //leaf 노드가 아니라면 밑으로 탐색
        if (!(root->isLeaf))
        {
            inorderTraversal(root->childs[i]);
            printf(" ");
        }
        // 데이터를 출력
        printf("%d", root->keys[i]);
    }
    //key 값보다 child 노드가 한 개 더 많으므로
    //마지막 child 노드에 대해 밑으로 탐색
    if (!(root->isLeaf))
    {
        inorderTraversal(root->childs[i]);
    }
    printf("\n");
}

int main()
{
    BTNode* root;
    int i, n, t;
    
    root = initBTNode();
    root->isRoot = 1;
    
    printf("넣을 데이터의 개수: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        printf("데이터를 입력하세요: ");
        scanf("%d", &t);
        root = BTInsert(root, t);
    }
    printf("트리 출력: ");
    inorderTraversal(root);
    
    return 0;
}