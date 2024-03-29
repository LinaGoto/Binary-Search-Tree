#include <iostream>
#include <cstring>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

/* Lina Goto

   binary search tree using linked list */

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

/* adding value */
void binarytree::add(int value) {
  tree_t *cur, *data;

  /* if data is duplicated, do nothing */
  if (findtree(value) != NULL) return;

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

/* deleting value */
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

/* finding depth */
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

/* finding the rightmost */
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

/* fiding node */
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

/* search; node existing */
void binarytree::search(int value){
  tree_t *cur = findtree(value);
  if (cur == NULL) {
    cout << "not found." << endl;
  } else {
    cout << "exists." << endl;
  }
}

/* printing tree */
void binarytree::show(void) {
  int d, w, i, z;
  tree_t *tree;
  
  for (d = 0; d < getdepth(root, 0) + 1; d ++) {
    int gap = (getwidth(getdepth(root, 0) - d) - 1) * 4;
    for (z = 0; z < gap / 2; z ++) printf(" ");
    for (w = 0; w < getwidth(d); w ++) {
      tree = locatetree(d, w);
      if (tree == NULL) {
	printf("    ");
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

  /* input or file */
  char option[10];
  cout << "first input: input by console (i) or file (f)?" << endl;
  cin.get (option, 10);
  cin.get();

  /* number entered by input */
  if(option[0] == 'i' || option[0] == 'I'){
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

  /* file */
  if(option[0] == 'f' || option[0] == 'F'){
    string myText;
    
    /* Read from the text file */
    ifstream MyReadFile("random_number.txt");
    
    /* read from file and send to add */
    while(getline(MyReadFile, myText, ' ')) {
      binarytree.add(atoi(myText.c_str()));
    }
    
    /*  Close the file */
    MyReadFile.close();
  }
  
   while (1){
    /* input delete or search */
    char input[10];
    cout << "input / delete / search / print?" << endl;
    cin.get (input,10);
    cin.get();
    
    /* number entered by input */
    if(input[0] == 'i' || input[0] == 'I'){
      input[0] = 'a';
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
    
    /* deleting */
    if(input[0] == 'd' || input[0] == 'D'){
      input[0] = 'a';
      int del;
      cout << "enter a number to delete" << endl;
      cin >> del;
      cin.get();
      binarytree.del(del);
      del = 0;
    }
    
    /* searching */
    if(input[0] == 's' || input[0] == 'S'){
      input[0] = 'a';
      int srch;
      cout << "enter a number to search" << endl;
      cin >> srch;
      cin.get();
      binarytree.search(srch);
      srch = 0;
    }
    
    if(input[0] == 'p' || input[0] == 'P'){
      input[0] = 'a';
       binarytree.show();
    }
  }
  return 0;
  
  
}
