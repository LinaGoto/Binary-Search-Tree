#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

typedef struct tree_s {
  int value;
  tree_s *left, *right;
} tree_t;

class binarytree {
private:
  tree_t *root;

  /* supplimental code */
  int getdepth(tree_t *tree, int depth);
  int leftright(int x, int y, int depth);

  tree_t *locatetree(int x, int y);
  tree_t *findtree(int value);
  tree_t *findpre(tree_t *target);
  tree_t *findempty(int value);
  tree_t *findmin(tree_t *tree);
  tree_t *findmax(tree_t *tree);

public:
  binarytree(void);

  /* operation */
  void add (int value);
  void del (int value);
  void search (int value);
  
  /* show tree */
  void show(void);
};

binarytree::binarytree(void) {
  root = NULL;
}

/* find the tree that points the target */
tree_t *binarytree::findpre(tree_t *target) {
  tree_t *cur = root;

  while ((cur != NULL) && (cur != target) && (cur -> left != target) && (cur -> right != target)) {
    if (target -> value < cur -> value) cur = cur -> left; else cur = cur -> right;
  }
  return cur;
}

/* find the tree that has the value */
tree_t *binarytree::findtree(int value) {
  tree_t *cur = root;
  
  /* finding value */
  while ((cur != NULL) && (cur -> value != value)) {
    if (value < cur -> value) cur = cur -> left; else cur = cur -> right;
  }
  return cur;
}

/* find the tree to add */
tree_t *binarytree::findempty(int value) {
  tree_t *cur = root;
  
  /* finding the value */
  while (cur != NULL) {
    /* value is already defined */
    if (value == cur -> value) break;
    /* if left is terminated */
    if ((value < cur -> value) && (cur -> left  == NULL)) break;
    /* if right is terminated */
    if ((value > cur -> value) && (cur -> right == NULL)) break;
    /* update pointer */
    if (value < cur -> value) cur = cur -> left; else cur = cur -> right;
  }

  return cur;
}

/* find the mininum value; very right tree will be the maximum */
tree_t *binarytree::findmin(tree_t *target) {
  while ((target != NULL) && (target -> left != NULL)) target = target -> left;
  return target;
}

/* find the maximum value; very left tree will be the minimum */
tree_t *binarytree::findmax(tree_t *target) {
  while ((target != NULL) && (target -> right != NULL)) target = target -> right;
  return target;
}

void binarytree::add(int value) {
  tree_t *cur, *data;

  /* set up new data */
  data = new tree_t;
  data -> value = value;
  data -> left  = NULL;
  data -> right = NULL;

  cur = findempty(value);

  if (cur == NULL) {
    root = data;
  } else if (cur -> value != value) {
    if (value < cur -> value) cur -> left = data; else cur -> right = data;
  }
}

void binarytree::del(int value) {
  tree_t *cur = root;
  tree_t *pre, *maxtree, *pmaxtree;
  
  /* find the poiter */
  cur = findtree(value);

  /* value was not found */
  if (cur == NULL) return;

  if (cur == root) {
    /* if it's root */
    if ((cur -> left == NULL) && (cur -> right == NULL)) {
      /* no leaves, simply delete it */
      root = NULL;
    } else if (cur -> right == NULL) {
      /* left only */
      root  = cur -> left;
    } else if (cur -> left == NULL) {
      /* right only */
      root  = cur -> right;
    } else {
      /* two leaves */
      /* find the maximum tree */
      maxtree = findmax(cur -> left);

      /* max tree is next pointer */
      if (cur -> left == maxtree) {
	maxtree -> right = cur -> right;
      } else {
	pmaxtree = findpre(maxtree);
	pmaxtree -> right = maxtree -> left;
	maxtree -> left  = cur -> left;
	maxtree -> right = cur -> right;
      }
      root = maxtree;
    }
  } else {
    /* if not root, find pre-pointer */
    pre = findpre(cur);

    if ((cur -> left == NULL) && (cur -> right == NULL)) {
      /* no leaves, simply delete it */
      if (value < pre -> value) pre -> left = NULL; else pre -> right = NULL;
    } else if (cur -> right == NULL) {
      /* left only */
      if (value < pre -> value) pre -> left = cur -> left; else pre -> right = cur -> left;
    } else if (cur -> left == NULL) {
      /* right only */
      if (value < pre -> value) pre -> left = cur -> right; else pre -> right = cur -> right;
    } else {
      /* two leaves */
      /* find the maximum tree */
      maxtree = findmax(cur -> left);

      /* max tree is next pointer */
      if (cur -> left == maxtree) {
	maxtree -> right = cur -> right;
      } else {
	pmaxtree = findpre(maxtree);
	pmaxtree -> right = maxtree -> left;
	maxtree -> left  = cur -> left;
	maxtree -> right = cur -> right;
      }
      
      if (value < pre -> value) pre -> left = maxtree; else pre -> right = maxtree;
    }
  }
  
  delete cur;
}

static int parent    (int i) { return (i - 1) / 2; }
static int getwidth  (int i) { return (1 << i); }

int binarytree::getdepth(tree_t *tree, int depth) {
  static int maxdepth;

  if (depth == 0) maxdepth = 0;

  if ((tree -> left == NULL) && (tree -> right == NULL)) {
    if (maxdepth < depth) maxdepth = depth;
  } else {
    if (tree -> left  != NULL) getdepth(tree -> left,  depth + 1);
    if (tree -> right != NULL) getdepth(tree -> right, depth + 1);
  }
  return maxdepth;
}

int binarytree::leftright(int x, int y, int relativedepth) {
  int target = getwidth(x) + y - 1;
  int amIleft = 0;

  while (relativedepth > 0) {
    amIleft = ((target & 1) == 0);
    target = parent(target);
    relativedepth --;
  }
  return amIleft;
}

tree_t *binarytree::locatetree(int x, int y) {
  tree_t *tree = root;
  int depth = x;

  while ((tree != NULL) && (depth > 0)) {
    if (leftright(x, y, depth) == 0) {
      tree = tree -> left;
    } else {
      tree = tree -> right;
    }
    depth --;
  }
  
  return tree;
}

void binarytree::search(int value){
  tree_t *cur = findtree(value);
  if (cur == NULL) {
    cout << "not found." << endl;
  } else {
    cout << "exists." << endl;
  }
}

void binarytree::show(void) {
  int d, w, i, z;
  tree_t *tree;
  
  for (d = 0; d < getdepth(root, 0) + 1; d ++) {
    int gap = (getwidth(getdepth(root, 0) - d) - 1) * 4;
    for (z = 0; z < gap / 2; z ++) printf(" ");
    for (w = 0; w < getwidth(d); w ++) {
      tree = locatetree(d, w);
      if (tree == NULL) {
	printf("  x ");
      } else {
	printf("%4d", tree -> value);
      }
      for (z = 0; z < gap; z ++) printf(" ");
    }
    printf("\n");
  }
  printf("\n");
}

int main(void) {
  /* encoding */
  binarytree binarytree;
  
#if 0
  int i;
  int array[] = {783, 386, 277, 415, 293, 835, 886, 992, 149, 921, 862, 527, 190, 559, 263, 426,  40, 926, 3, 55, 845, -1};
  
  i = 0;
  while (array[i] >= 0) binarytree.add(array[i++]);

  /* print encoded data */
  binarytree.show();
  
  binarytree.del(277);

  binarytree.show();
#endif

  /* input or by file */
  char input[10];
  cout << "input or file?" << endl;
  cin.get (input,10);
  cin.get();
  
  /* number entered by input */
  if(input[0] == 'i' || input[0] == 'I'){
    cout << "enter a series of numbers separated by a single space" << endl;
    char series[1024];
    cin.getline(series, 1024);

    /* read the input until space and send it to binarytree*/

    int value = 0;
    
    for (int i = 0; series[i] != '\0'; i++){
      if (series[i] != ' '){
	value = value*10 + (series[i] - '0');
	if (series[i+1] == ' ' || series[i+1] == '\0'){
	  binarytree.add(value);
	  value = 0;
	}
      }
    }
  }

#if 0  
  if(input[0] == 'f' || input[0] == 'F'){
    char myText[1024];
    
    /* Read from the text file */
    ifstream MyReadFile("random_number.txt");
    
    /* read the file and send it to viod file */
    getline (MyReadFile, myText);

     /* read the input until space and send it to binarytree*/

    int value = 0;

    for (int i = 0; series[i] != '\0'; i++){
      if (series[i] != ' '){
        value = value*10 + (series[i] - '0');
        if (series[i+1] == ' ' || series[i+1] == '\0'){
          binarytree.add(value);
          value = 0;
        }
      }
    }
    
    /*  Close the file */
    MyReadFile.close();
  }
#endif
  
  binarytree.show();


  /* deleting or search */
  char move[10];
  cout << "delete or search?" << endl;
  cin.get (move,10);
  cin.get();

  /* deleting */
  if(move[0] == 'd' || move[0] == 'D'){
    int del;
    cout << "enter a number to delete" << endl;
    cin >> del;
    binarytree.del(del);
  }

  /* searching */
  if(move[0] == 's' || move[0] == 'S'){
    int srch;
    cout << "enter a number to search" << endl;
    cin >> srch;
    binarytree.search(srch);
  }
    
  binarytree.show();
  
  return 0;
  
  
}
