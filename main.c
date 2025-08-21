#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct word{char w[40]; int num;} WORD;
typedef struct node{WORD info;struct node *next} NODE; // node of a singly linked list
typedef struct node_t{char word[40]; struct node_t *left,*right; int height,word_length;}NODE_T; // node of an AVL tree


// Function that adds element to the end of a singly linked list
void add_end(NODE **phead, WORD *wo) {
  NODE *p, *new_n = (NODE *)malloc(sizeof(NODE));
  new_n->info.num=wo->num;
  strcpy(new_n->info.w,wo->w);
  new_n->next = 0;
  if (*phead == 0) *phead = new_n;
  else
  {
    for (p = *phead; p->next; p = p->next);
    p->next = new_n;
  }
}

// Function that deletes a singly linked list
void delete_list(NODE **phead) {
    while (*phead) {
        NODE *p = (*phead)->next;
        free(*phead);
        *phead = p;
    }
}

// Function that returns the larger of two given numbers
int max(int a, int b) {
    return (a > b) ? a : b;
}
// Function that returns the height of a tree node
int height(NODE_T *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function that calculates the balance of a tree node
int balance(NODE_T *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Function that creates a new tree node
NODE_T* newTreeNode(const char *word) {
    NODE_T *node=(struct Node *)malloc(sizeof(NODE_T));
    strcpy(node->word, word);
    node->left=NULL;
    node->right=NULL;
    node->height=1;
    node->word_length=strlen(word);
    return node;
}

// Function that performs a right rotation around node x
NODE_T* rightRotation(NODE_T *x) {
    NODE_T *y=x->left;
    NODE_T *temp=y->right;
    y->right=x;
    x->left=temp;
    x->height=max(height(x->left),height(x->right)) + 1;
    y->height=max(height(y->left),height(y->right)) + 1;
    return y;
}

// Function that performs a left rotation around node x
NODE_T* leftRotation(NODE_T *x) {
    NODE_T *y=x->right;
    NODE_T *temp=y->left;
    y->left=x;
    x->right=temp;
    x->height=max(height(x->left),height(x->right))+1;
    y->height=max(height(y->left),height(y->right))+1;
    return y;
}

// Function that inserts a node into an AVL tree
NODE_T* avl_tree(NODE_T *root,char *word) {
    // First part of the function – insertion into the tree
    if (root == NULL)
        return newTreeNode(word);
    if (strcmp(word,root->word)<0)
        root->left=avl_tree(root->left,word);
    else if (strcmp(word,root->word)>0)
        root->right=avl_tree(root->right,word);
    else
        return root; // Inserting duplicates is not allowed; however, there won't be any because they are previously eliminated in the main function
    // Second part of the function – height balancing
    root->height=1+max(height(root->left),height(root->right));
    int b=balance(root);
    if(b>1){
        if(balance(root->left)>=0)
            return rightRotation(root);
        else{
            root->left=leftRotation(root->left);
            return rightRotation(root);
        }
    }
    if(b<-1){
        if(balance(root->right)<=0)
            return leftRotation(root);
        else{
            root->right=rightRotation(root->right);
            return leftRotation(root);
        }
    }
    return root;
}

// Function that deletes an AVL tree
void deleteTree(NODE_T* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}

// Recursive function for inorder traversal of the tree
int inorder(NODE_T* root,int *array,int index) {
    if (root != NULL) {
        index = inorder(root->left,array,index);
        printf("%s ", root->word);
        array[index]=root->word_length;
        index++;
        index = inorder(root->right,array,index);
    }
    return index;
}

// Function for sorting in ascending order using bubble sort
void bubbleSort (int array[], int n)
{
  int pos=n-1;
  int i, j;
  do{
  int bound=pos;
  pos=0;
  for (i=0; i<bound; i++)
  {
    for (j=0; j<bound-i; j++)
      if (array[j]>array[j+1])
      {
         int temp=array[j];
         array[j]=array[j+1];
         array[j+1]=temp;
      }
  }
  }while(pos!=0);
}

// Function for sorting in descending order using insertion sort
void insertionSort(int array[],int n)
{
   int i,j;
   for (i=1;i<n;i++)
   {
      int x=array[i];
      for (j=i;j>0 && x>array[j-1];j--)
         array[j]=array[j-1];
      array[j]=x;
   }
}

int main()
{
    FILE *fp,*file_d,*file_r;
    char character,str[8];
    NODE_T *root=NULL;
    NODE *head=NULL;
    int n=0,index=0, array[400];
    int c=100,size_w,count=0,num=1;
    char **arr;
    char words[500], *token, filename[40],*file_duplicates,duplicates[100];
    WORD *word,*unique;
    do{
        printf("==================================================================================================\n");
        printf("Select an option by entering the corresponding character: Sort words from a txt file [S], Exit [E]\n");
        printf("==================================================================================================\n");
        scanf(" %c",&character);
        if(character=='S'){
            root = NULL;
            head = NULL;
            printf("Enter the name of the file containing the text you want to sort\n");
            scanf("\n%s",filename);
            if (fp=fopen(filename,"r")) {
            arr= (char**) malloc (c*sizeof(char*));
            while (fscanf(fp,"%s",words)!=EOF){
                token = strtok(words, " ");
                while (token != NULL) {
                    if (n == c) arr = (char**)realloc(arr, (c *= 2) * sizeof(char*));
                    arr[n++]=strdup(token);
                    token = strtok(NULL, " ");
                }
            }
            fclose(fp);
            printf("\nContent of the entered file before removing duplicate words and sorting:\n");
            for(int k=0;k<n;k++){
                printf("%s ",arr[k]);
            }
            // Reads elements from an array and transfers them to a list so that each word in the array corresponds to a node in the list,
            // and in that node, alongside the word, its number of occurrences in the array is stored
            for (int k = 0; k < n; k++) {
                NODE* current = head;
                int found=0;
                while (current != NULL) {
                    if (strcmp(current->info.w,arr[k]) == 0) {
                        current->info.num+=1;
                        found=1;
                    }
                    current = current->next;
                }
                if (!found) {
                    WORD new_word;
                    strcpy(new_word.w, arr[k]);
                    new_word.num = 1;
                    add_end(&head, &new_word);
                }
            }

            file_duplicates = (char *)malloc(20 * sizeof(char));
            sprintf(file_duplicates,"duplicates_%d.txt",num);

            // Forming an AVL tree, counting duplicates, and writing duplicates to the file "duplicates"
            NODE* current2 = head;
            while (current2 != NULL) {
                root=avl_tree(root,current2->info.w);
                count=count+(current2->info.num - 1);
                if(current2->info.num>1){
                if(file_d=fopen(file_duplicates,"a+")){
                for(int i=0;i<current2->info.num -1;i++){
                fprintf(file_d,"%s  ",current2->info.w);}
                fprintf(file_d,"\n");
                fclose(file_d);
                }}
                current2 = current2->next;
            }

            printf("\n\nAfter removing duplicate words and sorting, the text read from the entered file looks like this:\n");
            index=inorder(root,array,index);
            printf("\n\nArray of word lengths: ");
            for(int i=0;i<index;i++){
                printf("%d  ",array[i]);
            }
            printf("\nArray of word lengths sorted in ascending order: ");
            bubbleSort(array,index);
            for(int i=0;i<index;i++){
                printf("%d  ",array[i]);
            }
            printf("\nArray of word lengths sorted in descending order: ");
            insertionSort(array,index);
            for(int i=0;i<index;i++){
                printf("%d  ",array[i]);
            }


            printf("\n\nNumber of duplicates:%d\n",count);
            if(count!=0){
            printf("Do you want to see the contents of the duplicates file? [YES]\n");
            scanf(" %s",str);
            if((strcmp(str,"YES"))==0){
                printf("Contents of the file %s:\n",file_duplicates);
                file_r=fopen(file_duplicates,"r");
                if(file_r!=NULL){
                    while(fgets(duplicates,500,file_r)!=NULL)
                        printf("%s",duplicates);
                }
                fclose(file_r);
            } }
            num=num+1;
            // Freeing memory and resetting the index
            deleteTree(root);
            n=0;
            index=0;
            count=0;
            for (int i = 0; i < n; i++) {
                free(arr[i]);
            }
            free(arr);
            delete_list(&head);
        }else printf("An error occurred while opening the entered file\n");}
        if(character!='E' && character!='S') printf("The option you entered does not exist\n");
    } while(character!='E');
 return 0;
}
