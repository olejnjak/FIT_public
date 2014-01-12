/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

TNODE *makeNode (TNODE *parent) { /*funkce vytvori novy uzel a vrati na nej ukazatel*/
  TNODE *node = (TNODE*)malloc(sizeof(*node));
  node->m_Parent = parent;
  node->m_Decoration = DECORATION_NONE;
  node->m_Branches[0] = node->m_Branches[1] = node->m_Branches[2] = NULL;
  return node;
}

int checkPath ( char * path ) { /*funkce zkontroluje zda zadana cesta je platna*/
  while (*path >= '0' && *path <='2') path++; /*projde retezec dokud nenarazi na znak ruzny od '0' '1' '2'*/
  if (*path == 0) return 1; /*pokud cyklus prosel cely retezec a narazil na ukoncovaci 0 je cesta platna*/
  return 0; /*jinak cesta platna neni*/
}

int setDecoration ( TNODE ** root, char * path, int decor ) {
  TNODE *now;
  if (!checkPath (path)) return 0; /*zkontroluji zda cesta plati*/

  if (*root == NULL && *path == 0) {
    *root = makeNode (NULL);
    (*root)->m_Decoration = decor;
  }
  else if (*root == NULL && *path != 0) *root = makeNode(NULL);

  now = *root;
  while (*path != 0) {
    int branch = *path-'0';

    if (now->m_Branches[branch] == NULL) now->m_Branches[branch] = makeNode (NULL);
    now = now->m_Branches[branch];
    path++;
  }
  now->m_Decoration = decor;
  return 1;
}

void destroyTree ( TNODE * root ) {
  if (root == NULL) return;
  destroyTree (root->m_Branches[0]);
  destroyTree (root->m_Branches[1]);
  destroyTree (root->m_Branches[2]);
  free (root);
}

int cutBranch ( TNODE ** root, char * path ) {
  TNODE *now = *root;
  int delka;

  if (*root == NULL) return 0;
  if (!checkPath(path)) return 0;

  delka = strlen(path);
  if (delka == 0) {
    destroyTree (*root);
    *root = NULL;
    return 1;
  }
  while (delka>1) {
    int branch = *path - '0';
    if (now->m_Branches[branch] == NULL) return 0;
    now = now->m_Branches[branch];
    delka--;
    path++;
  }
  if (now->m_Branches[*path - '0'] == NULL) return 0;
  destroyTree(now->m_Branches[*path - '0']);
  now->m_Branches[*path - '0'] = NULL;
  return 1;
}

int easyToCatchFire ( TNODE * root ) {
  int vrat = 0;
  if (root == NULL) return 0;

  if (root->m_Branches[0] != NULL) {
    if (root->m_Decoration + root->m_Branches[0]->m_Decoration == 3) return 1;
    vrat = easyToCatchFire (root->m_Branches[0]);
    if (vrat == 1) return 1;
  }
  if (root->m_Branches[1] != NULL) {
    if (root->m_Decoration + root->m_Branches[1]->m_Decoration == 3) return 1;
    vrat = easyToCatchFire (root->m_Branches[1]);
    if (vrat == 1) return 1;
  }
  if (root->m_Branches[2] != NULL) {
    if (root->m_Decoration + root->m_Branches[2]->m_Decoration == 3) return 1;
    vrat = easyToCatchFire (root->m_Branches[2]);
    if (vrat == 1) return 1;
  }
  if (root->m_Branches[2] != NULL && root->m_Branches[1] != NULL) {
    if (root->m_Branches[2]->m_Decoration + root->m_Branches[1]->m_Decoration == 3) return 1;
  }
  if (root->m_Branches[0] != NULL && root->m_Branches[1] != NULL) {
    if (root->m_Branches[0]->m_Decoration + root->m_Branches[1]->m_Decoration == 3) return 1;
  }
  return 0;
}
