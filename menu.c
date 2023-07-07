#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum kategoria { pierwsze, drugie, desert, napoj };
enum dieta { wszystkozerne, wegetarianski, weganskie };

struct Menu {
  char imie[25];
  double cena;
  int kategoria;
  int dieta;
  double waga;
  int kalorie;
};

struct Node {
  int key;
  struct Node *left;
  struct Node *right;
  int height;
  struct Menu menu;
};

int height(struct Node *node);
int max(int a, int b);
struct Node *rightRotate(struct Node *y);
struct Node *leftRotate(struct Node *x);
int getBalance(struct Node *node);
struct Node *newNode(int key, struct Menu menu);
struct Node *insert(struct Node *root, int key, struct Menu menu);
struct Node *minValueNode(struct Node *node);
struct Node *deleteNode(struct Node *root, int key);
struct Node *search(struct Node *root, int key);
void printMenuItem(struct Menu menu);
void inorderTraversal(struct Node *root);
void showFullMenu(struct Node *root);
struct Menu createMenuItem();
struct Menu randomMenu();
void editMenuItem(struct Menu *menu);
void manageScene(struct Node *root);

int main() {
  srand(time(NULL));
  struct Node *root = NULL;

  manageScene(root);

  return 0;
}

int height(struct Node *node) {
  if (node == NULL)
    return 0;
  return node->height;
}

int max(int a, int b) { return (a > b) ? a : b; }

struct Node *rightRotate(struct Node *y) {
  struct Node *x = y->left;
  struct Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

struct Node *leftRotate(struct Node *x) {
  struct Node *y = x->right;
  struct Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}

int getBalance(struct Node *node) {
  if (node == NULL)
    return 0;
  return height(node->left) - height(node->right);
}

struct Node *newNode(int key, struct Menu menu) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  node->menu = menu;
  return node;
}

struct Node *insert(struct Node *root, int key, struct Menu menu) {
  if (root == NULL)
    return newNode(key, menu);

  if (key < root->key)
    root->left = insert(root->left, key, menu);
  else if (key > root->key)
    root->right = insert(root->right, key, menu);
  else
    return root;

  root->height = 1 + max(height(root->left), height(root->right));

  int balance = getBalance(root);

  if (balance > 1 && key < root->left->key)
    return rightRotate(root);

  if (balance < -1 && key > root->right->key)
    return leftRotate(root);

  if (balance > 1 && key > root->left->key) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && key < root->right->key) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

struct Node *minValueNode(struct Node *node) {
  struct Node *current = node;
  while (current->left != NULL)
    current = current->left;
  return current;
}

struct Node *deleteNode(struct Node *root, int key) {
  if (root == NULL)
    return root;

  if (key < root->key)
    root->left = deleteNode(root->left, key);
  else if (key > root->key)
    root->right = deleteNode(root->right, key);
  else {
    if ((root->left == NULL) || (root->right == NULL)) {
      struct Node *temp = root->left ? root->left : root->right;
      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else
        *root = *temp;
      free(temp);
    } else {
      struct Node *temp = minValueNode(root->right);
      root->key = temp->key;
      root->menu = temp->menu;
      root->right = deleteNode(root->right, temp->key);
    }
  }

  if (root == NULL)
    return root;

  root->height = 1 + max(height(root->left), height(root->right));

  int balance = getBalance(root);

  if (balance > 1 && getBalance(root->left) >= 0)
    return rightRotate(root);

  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && getBalance(root->right) <= 0)
    return leftRotate(root);

  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

struct Node *search(struct Node *root, int key) {
  if (root == NULL || root->key == key)
    return root;

  if (key < root->key)
    return search(root->left, key);

  return search(root->right, key);
}

void printMenuItem(struct Menu menu) {
  printf("Nazwa: %s\n", menu.imie);
  printf("Cena: %.2f Eur\n", menu.cena);
  printf("Kategoria: ");
  switch (menu.kategoria) {
  case pierwsze:
    printf("pierwsze\n");
    break;
  case drugie:
    printf("drugie\n");
    break;
  case desert:
    printf("desert\n");
    break;
  case napoj:
    printf("napoj\n");
    break;
  default:
    printf("niewiadomo\n");
    break;
  }
  printf("Te danie jest: ");
  switch (menu.dieta) {
  case wszystkozerne:
    printf("wszystkozerne\n");
    break;
  case wegetarianski:
    printf("wegetarianskie\n");
    break;
  case weganskie:
    printf("weganskie\n");
    break;
  default:
    printf("niewiadomo\n");
    break;
  }
  printf("Weight: %.2f g\n", menu.waga);
  printf("Calories: %d kcal\n\n", menu.kalorie);
}

void inorderTraversal(struct Node *root) {
  if (root != NULL) {
    inorderTraversal(root->left);
    printf("%d  ", root->key);
    inorderTraversal(root->right);
  }
}

void showFullMenu(struct Node *root) {
  if (root != NULL) {
    showFullMenu(root->left);
    printf("ID dania: %d\n", root->key);
    printMenuItem(root->menu);
    showFullMenu(root->right);
  }
}

struct Menu createMenuItem() {
  struct Menu menu;

  printf("Podaj nazwa produktu: ");
  scanf(" %24[^\n]", menu.imie);

  printf("Podaj cena produktu: ");
  scanf("%lf", &menu.cena);

  printf("Podaj jakiej kategori jest danie (0 - pierwsze, 1 - drugie, 2 - "
         "desert, 3 - napoj): ");
  scanf("%d", &menu.kategoria);

  printf("Podaj dla jakiej diety jest danie (0 - nie, 1 - wegetarianski, 2 - "
         "weganskie): ");
  scanf("%d", &menu.dieta);

  printf("Podaj waga dania w gramach: ");
  scanf("%lf", &menu.waga);

  printf("Podaj ilosc kcal w daniu: ");
  scanf("%d", &menu.kalorie);

  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }

  return menu;
}

struct Menu randomMenu() {
  struct Menu menu;

  char names[][25] = {"Mieso", "Zupa", "Kotlet", "Sok", "Herbata"};
  int numNames = sizeof(names) / sizeof(names[0]);
  int randomIndex = rand() % numNames;
  strcpy(menu.imie, names[randomIndex]);

  menu.cena = (double)(rand() % 1000) / 10.0;
  menu.kategoria = rand() % 4;
  menu.dieta = rand() % 3;
  menu.waga = (double)(rand() % 1000);
  menu.kalorie = rand() % 1000;

  return menu;
}

void editMenuItem(struct Menu *menu) {
  int choice;
  printf("Jakie pole chcesz edytowac?\n");
  printf("1. Nazwa\n");
  printf("2. Cena\n");
  printf("3. Kategoria\n");
  printf("4. Dieta\n");
  printf("5. Waga\n");
  printf("6. Kalorii\n");
  printf("Podaj wybor: ");
  scanf("%d", &choice);

  switch (choice) {
  case 1:
    printf("Podaj nowa nazwa dania: ");
    scanf(" %24[^\n]", menu->imie);
    break;
  case 2:
    printf("Podaj nowa cena dania: ");
    scanf("%lf", &(menu->cena));
    break;
  case 3:
    printf("Podaj nowa kategoria dania (0 - pierwsze, 1 - drugie, "
           "2 - desert, 3 - napoj): ");
    scanf("%d", (int *)&(menu->kategoria));
    break;
  case 4:
    printf("Podaj nowa dieta dania (0 - nie, 1 - "
           "wegetarianski, 2 - weganskie): ");
    scanf("%d", (int *)&(menu->dieta));
    break;
  case 5:
    printf("Podaj nowa waga dania: ");
    scanf("%lf", &(menu->waga));
    break;
  case 6:
    printf("Podaj nowa ilosc kalorii w daniu: ");
    scanf("%d", &(menu->kalorie));
    break;
  default:
    printf("Nieprawidlowy wybor. Zero modyfikacji.\n");
    return;
  }

  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

void manageScene(struct Node *root) {
  int choice;
  while (1) {
    printf("\n\nMenu glowne:\n");
    printf("1. Dodaj nowe dania\n");
    printf("2. Wyszukaj danie\n");
    printf("3. Wygeneruj nowe dania\n");
    printf("4. Wydrukuj wszystkie ID\n");
    printf("5. Wydrukuj cale menu\n");
    printf("6. Usun danie z menu\n");
    printf("7. Edytuj danie\n");
    printf("8. Wyjdz\n");
    printf("Podaj wybor: ");
    scanf("%d", &choice);
    printf("\n");

    switch (choice) {
    case 1: {
      struct Menu menu = createMenuItem();
      int key;
      printf("Podaj ID dla nowego dania: ");
      scanf("%d", &key);
      while (search(root, key) != NULL) {
        printf("Podaj ID ktore sie nie powtarza: ");
        scanf("%d", &key);
      }
      root = insert(root, key, menu);
      printf("Nowe danie zostalo dodane.\n");
      break;
    }
    case 2: {
      int key;
      printf("Podaj ID dania do wyszukania: ");
      scanf("%d", &key);
      struct Node *result = search(root, key);
      if (result != NULL) {
        printf("Znaleziono danie:\n");
        printMenuItem(result->menu);
      } else {
        printf("Nieznaleziono dania.\n");
      }
      break;
    }
    case 3: {
      int quantity;
      printf("Napisz ilosc dan do wygenerowania: ");
      scanf("%d", &quantity);
      int ID = 1, i = 0;
      while (i < quantity) {
        if (search(root, ID) == NULL) {
          struct Menu randMenu = randomMenu();
          root = insert(root, ID, randMenu);
          i++;
        }
        ID++;
      }
      break;
    }
    case 4:
      printf("Wszystkie ID w menu:\n");
      inorderTraversal(root);
      break;
    case 5:
      printf("Wszystkie dania w menu:\n");
      showFullMenu(root);
      break;
    case 6: {
      int key;
      printf("Podaj ID dania do usuniecia: ");
      scanf("%d", &key);
      root = deleteNode(root, key);
      printf("Usunieto pomyslnie.\n");
      break;
    }
    case 7: {
      int key;
      printf("Wprowadz ID dania do edytowania: ");
      scanf("%d", &key);
      struct Node *result = search(root, key);
      editMenuItem(&result->menu);
      break;
    }
    case 8:
      printf("Wychodzimy z programu...\n");
      return;
    default:
      printf("Nieprawodlowy wybor. Sprobuj ponownie.\n");
      break;
    }

    printf("\n");
  }
}
