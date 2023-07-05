#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum stanZam { czeka, dostarczone, anulowane };
enum platnosc { oczekujace, oplacone, zwrocone };
enum typOplaty { karta, gotowka, online };

struct MenuNode {
  void *value;
  void *quantity;
  struct MenuNode *next;
};

struct MenuQueue {
  int size;
  int max_size;
  struct MenuNode *head;
  struct MenuNode *tail;
};

struct Order {
  int stanZamow;
  int idtable;
  struct MenuQueue menu;
};

struct Payment {
  double suma;
  int plat;
  int typOpl;
};

struct Node {
  int key;
  struct Node *left;
  struct Node *right;
  int height;
  struct Order order;
  struct Payment pay;
};

// zapisywanie jaki jest nastepny key w drzewie
int nextKey = 1;

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

struct Node *insertNode(struct Node *node, struct Order *order,
                        struct Payment *payment) {
  if (node == NULL) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->key = nextKey++;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    newNode->order = *order;
    newNode->pay = *payment;
    return newNode;
  }

  if (nextKey < node->key)
    node->left = insertNode(node->left, order, payment);
  else if (nextKey > node->key)
    node->right = insertNode(node->right, order, payment);
  else
    return node;

  node->height = 1 + max(height(node->left), height(node->right));

  int balance = getBalance(node);

  if (balance > 1 && nextKey < node->left->key)
    return rightRotate(node);

  if (balance < -1 && nextKey > node->right->key)
    return leftRotate(node);

  if (balance > 1 && nextKey > node->left->key) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && nextKey < node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
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

struct Node *searchNode(struct Node *root, int key) {
  if (root == NULL || root->key == key)
    return root;

  if (key < root->key)
    return searchNode(root->left, key);

  return searchNode(root->right, key);
}

void inorderTraversal(struct Node *root) {
  if (root != NULL) {
    inorderTraversal(root->left);
    printf("%d ", root->key);
    inorderTraversal(root->right);
  }
}

void viewOrder(struct Node *root, int key) {
  struct Node *node = searchNode(root, key);
  if (node == NULL) {
    printf("Nieznaleziono zamowienia z ID: %d\n", key);
    return;
  }

  printf("Zamowienia szczegoly:\n");
  printf("  Stan zamowienia: ");

  switch (node->order.stanZamow) {
  case czeka:
    printf("Czeka\n");
    break;
  case dostarczone:
    printf("Dostarczone\n");
    break;
  case anulowane:
    printf("Anulowane\n");
    break;
  default:
    printf("ERROR\n");
  }

  printf("  ID Stolu: %d\n", node->order.idtable);

  printf("  Menu (ID,ilosc): ");
  struct MenuNode *menuNode = node->order.menu.head;
  while (menuNode != NULL) {
    printf("%d,", (int)(intptr_t)menuNode->value);
    printf("%d\t\t", (int)(intptr_t)menuNode->quantity);
    menuNode = menuNode->next;
  }
  printf("\n");
}

void viewPayment(struct Node *root, int key) {
  struct Node *node = searchNode(root, key);
  if (node == NULL) {
    printf("Oplata nie znaleziono z kluczem: %d\n", key);
    return;
  }

  printf("Szczegoly oplaty:\n");
  printf("  Suma: %.2lf\n", node->pay.suma);
  printf("  Platnosc: ");

  switch (node->pay.plat) {
  case oczekujace:
    printf("Oczekujace\n");
    break;
  case oplacone:
    printf("Oplacone\n");
    break;
  case zwrocone:
    printf("Zwrocone\n");
    break;
  default:
    printf("ERROR\n");
  }

  printf("  Typ Oplaty: ");

  switch (node->pay.typOpl) {
  case karta:
    printf("Karta\n");
    break;
  case gotowka:
    printf("Gotowka\n");
    break;
  case online:
    printf("Online\n");
    break;
  default:
    printf("ERROR\n");
  }
}

void generateRandomData(struct Order *order, struct Payment *payment) {
  order->stanZamow = rand() % 3;
  order->idtable = rand() % 10 + 1;

  // Losujemy ilosc dan w zamowieniu
  int menuSize = rand() % 10 + 1;
  order->menu.size = menuSize;
  order->menu.max_size = 25;

  struct MenuNode *currentNode = NULL;
  struct MenuNode *prevNode = NULL;

  for (int i = 0; i < menuSize; i++) {
    struct MenuNode *newNode =
        (struct MenuNode *)malloc(sizeof(struct MenuNode));

    newNode->value = (void *)(intptr_t)(rand() % 100 + 1);
    newNode->quantity = (void *)(intptr_t)(rand() % 5 + 1);

    newNode->next = NULL;

    if (prevNode == NULL) {
      order->menu.head = newNode;
      prevNode = newNode;
    } else {
      prevNode->next = newNode;
      prevNode = newNode;
    }

    if (i == menuSize - 1) {
      order->menu.tail = newNode;
    }
  }

  // losowa suma do liczby wprowadzonej
  payment->suma = ((double)rand() / RAND_MAX) * 100.0;
  payment->plat = rand() % 3;
  payment->typOpl = rand() % 3;
}

void insertRandomData(struct Node **root, int count) {
  srand(time(NULL));

  for (int i = 0; i < count; i++) {
    struct Order order;
    struct Payment payment;

    generateRandomData(&order, &payment);

    *root = insertNode(*root, &order, &payment);
  }
}

struct MenuNode *createMenuNodeWithUser() {
  struct MenuNode *newNode = (struct MenuNode *)malloc(sizeof(struct MenuNode));

  printf("Podaj ID produktu w menu: ");
  scanf("%d", (int *)&newNode->value);

  printf("Podaj ilosc produktu: ");
  scanf("%d", (int *)&newNode->quantity);

  newNode->next = NULL;

  return newNode;
}

struct Node *createNodeWithUser(struct Node *root) {
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

  newNode->key = nextKey++;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->height = 1;

  printf("Podaj stan zamowienia (0-Czeka, 1-Dostarczone, 2-Anulowane): ");
  scanf("%d", &newNode->order.stanZamow);

  printf("Podaj ID stolu: ");
  scanf("%d", &newNode->order.idtable);

  printf("Podaj suma: ");
  scanf("%lf", &newNode->pay.suma);

  printf("Podaj czy jest oplacone (0-Oczekujace, 1-Oplacone, 2-Zwrocone): ");
  scanf("%d", &newNode->pay.plat);

  printf("Podaj typ oplaty (0-Karta, 1-Gotowka, 2-Online): ");
  scanf("%d", &newNode->pay.typOpl);

  int menuSize;
  printf("Podaj ile roznych produktow jest w zamowieniu: ");
  scanf("%d", &menuSize);

  newNode->order.menu.size = menuSize;
  newNode->order.menu.max_size = 25;

  struct MenuNode *currentNode = NULL;
  struct MenuNode *prevNode = NULL;

  for (int i = 0; i < menuSize; i++) {
    struct MenuNode *menuNode = createMenuNodeWithUser();

    if (prevNode == NULL) {
      newNode->order.menu.head = menuNode;
      prevNode = menuNode;
    } else {
      prevNode->next = menuNode;
      prevNode = menuNode;
    }

    if (i == menuSize - 1) {
      newNode->order.menu.tail = menuNode;
    }
  }

  return newNode;
}

void editNode(struct Node *node) {
  int choice;

  printf("Edycja dania z ID: %d\n", node->key);
  printf("1. Zamien stan zamowienia\n");
  printf("2. Zamien ID stolu\n");
  printf("3. Zamien suma\n");
  printf("4. Zamien czy oplacone\n");
  printf("5. Zamien typ oplaty\n");
  printf("6. Zamien dania w zamowieniu\n");
  printf("Podaj swoj wybor (1-6): ");
  scanf("%d", &choice);
  printf("\n");

  switch (choice) {
  case 1:
    printf("Wprowadz nowy stan zamowienia (0-Czeka, 1-Dostarczone, "
           "2-Anulowane): ");
    scanf("%d", &node->order.stanZamow);
    printf("Stan zamowienia zostal zmieniony!\n");
    break;
  case 2:
    printf("Enter the new idtable: ");
    scanf("%d", &node->order.idtable);
    printf("ID stolu zostalo zmienione!\n");
    break;
  case 3:
    printf("Wprowadz nowa suma: ");
    scanf("%lf", &node->pay.suma);
    printf("Suma zostala zmieniona!\n");
    break;
  case 4:
    printf("Wprowadz zmiany czy oplacono (0-Oczekujace, 1-Oplacone, "
           "2-Zwrocone): ");
    scanf("%d", &node->pay.plat);
    printf("Czy oplacono zostalo zmienione!\n");
    break;
  case 5:
    printf("Wprowadz w jaki sposob oplacono (0-Karta, 1-Gotowka, 2-Online): ");
    scanf("%d", &node->pay.typOpl);
    printf("Sposob oplaty zostal zmieniony!\n");
    break;
  case 6: {
    int menuSize;
    printf("Wprowadz ile produktow w zamowieniu: ");
    scanf("%d", &menuSize);

    struct MenuNode *currentNode = node->order.menu.head;
    while (currentNode != NULL) {
      struct MenuNode *temp = currentNode;
      currentNode = currentNode->next;
      free(temp);
    }

    node->order.menu.size = menuSize;
    node->order.menu.max_size = 25;
    node->order.menu.head = NULL;
    node->order.menu.tail = NULL;

    struct MenuNode *prevNode = NULL;

    for (int i = 0; i < menuSize; i++) {
      struct MenuNode *menuNode = createMenuNodeWithUser();

      if (prevNode == NULL) {
        node->order.menu.head = menuNode;
        prevNode = menuNode;
      } else {
        prevNode->next = menuNode;
        prevNode = menuNode;
      }

      if (i == menuSize - 1) {
        node->order.menu.tail = menuNode;
      }
    }

    printf("Produkty w zamowieniu zostaly zmienione!\n");
    break;
  }
  default:
    printf("Niepoprawny wpis! Zero zmian.\n");
  }
}

void sceneManagement(struct Node **root) {
  int choice;

  while (1) {
    printf("\n\nMenu Glowne\n");
    printf("1. Dodaj zamowienie\n");
    printf("2. Wygeneruj losowe zamowienie\n");
    printf("3. Edytuj zamowienie\n");
    printf("4. Usun zamowienie\n");
    printf("5. Wszystkie zamowienia\n");
    printf("6. Pokaz szczegolnie zamowienie\n");
    printf("7. Wyjdz\n");
    printf("Podaj co wybierasz (1-7): ");
    scanf("%d", &choice);
    printf("\n");

    switch (choice) {
    case 1: {
      struct Node *newNode = createNodeWithUser(*root);
      *root = insertNode(*root, &newNode->order, &newNode->pay);
      printf("Dodano zamowienie!\n");
      break;
    }
    case 2: {
      int count;
      printf("Podaj ile zamowien wygenerowac: ");
      scanf("%d", &count);
      insertRandomData(root, count);
      printf("Wygenerowano i dodano %d zamowien!\n", count);
      break;
    }
    case 3: {
      int key;
      printf("Podaj ID zamowienia do edycji: ");
      scanf("%d", &key);
      struct Node *node = searchNode(*root, key);
      if (node != NULL) {
        printf("Edytowanie zamowienia z ID %d\n", key);
        editNode(node);
        printf("Zamowienie edytowane pomyslnie!\n");
      } else {
        printf("Nie znaleziono zamowienia z ID: %d!\n", key);
      }
      break;
    }
    case 4: {
      int key;
      printf("Podaj ID zamowienia do usuniecia: ");
      scanf("%d", &key);
      *root = deleteNode(*root, key);
      printf("Zamowienie z ID %d zostalo usuniete!\n", key);
      break;
    }
    case 5: {
      printf("Przechodzenie w porzadku:\n");
      inorderTraversal(*root);
      printf("\n");
      break;
    }
    case 6: {
      int key;
      printf("Podaj ID zamowienia do szczegolnego wyswietlania: ");
      scanf("%d", &key);
      struct Node *node = searchNode(*root, key);
      if (node != NULL) {
        printf("Wyswietlany jest zamowienie z ID %d:\n\n", key);
        viewOrder(*root, key);
        viewPayment(*root, key);
      } else {
        printf("Zamowienia z ID %d nie znaleziono!\n", key);
      }
      break;
    }
    case 7:
      printf("Wychodzimy z programu...\n");
      return;
    default:
      printf("Nieprawidlowy wybor! Prosze sprobuj ponownie.\n");
    }
  }
}

int main() {
  struct Node *root = NULL;

  sceneManagement(&root);

  return 0;
}
