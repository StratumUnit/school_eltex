Структура `TreeNode` представляет собой узел бинарного дерева поиска, содержащий указатель на данные контакта `contact` и ссылки на левое `left` и правое `right` поддеревья.

```c
struct TreeNode
{
  struct Contact *contact;
  struct TreeNode *left;
  struct TreeNode *right;
};

struct Contact
{
  int id;
  char firstName[NAME_LEN];
  char lastName[NAME_LEN];
  char job[JOB_LEN];
  char phoneNumber[PHONE_LEN];
  struct Address address;
};


```


## Реализация

В основном все основные функции модификации дерева работают через двойные указатели `struct TreeNode **` и возвращают код результата:
- 0 — успешное выполнение операции
- -1 — ошибка, узел не найден, дерево пусто или неверные аргументы

### Алгоритм Вставки

Вставка элементов в дерево происходит рекурсивным сравнением фамилий `lastName` по алфавиту с помощью функции `strcmp`. Элементы, которые меньше текущего по алфавиту, уходят в левое поддерево, остальные — в правое.

```c

int insert(struct TreeNode **node, struct TreeNode *newNode)
{

  if (*node == NULL)

  {
    *node = newNode;
    return 0;
  }

  if (strcmp(newNode->contact->lastName, (*node)->contact->lastName) < 0)
  {
    return insert(&(*node)->left, newNode);
  }
  else
  {
    return insert(&(*node)->right, newNode);
  }
}
```

  

### Поиск

Поиск элемента в бинарном дереве выполняется за логарифмическое время O(log n). Найденный узел передается в вызывающий код через указатель `struct TreeNode **node`.

  

```c

int findConctact(struct TreeNode **root, const char *lastname, struct TreeNode **node)
{
  struct TreeNode **ptr = node;
  if (root == NULL || *root == NULL)
  {

    *node = NULL;
    return -1;
  }

  int cmpRes = strcmp((*root)->contact->lastName, lastname);
  if (cmpRes == 0)
  {
    *node = *root;
    return 0;
  }
  else if (cmpRes < 0)
  {
    return findConctact(&((*root)->right), lastname, ptr);
  }
  else
  {
    return findConctact(&((*root)->left), lastname, ptr);
  }
  return -1;
}

```

  

### Удаление элементов

Функция производит рекурсивный поиск узла по фамилии. При обнаружении элемента рассматриваются три классических случая структуры BST:


#### 1. Удаление узла с одним потомком

Если у узла нет детей, связь у родителя зануляется. Если есть один ребенок (левый или правый), указатель родителя `*root` перенаправляется напрямую на него, минуя удаляемый узел.

  

```c
if ((*root)->left == NULL && (*root)->right == NULL) {
    free(forDel->contact); free(forDel); *root =  NULL;
}  else  if ((*root)->left == NULL) {
    *root = (*root)->right; free(forDel->contact); free(forDel);
}  else  if ((*root)->right == NULL) {
    *root = (*root)->left; free(forDel->contact); free(forDel);
}
```


#### 2. Удаление узла с двумя потомками

В правом поддереве находится самый левый элемент (преемник по алфавиту). Данные контакта преемника копируются в текущий узел, а сам преемник аккуратно извлекается со своего прежнего места в памяти с помощью двойного указателя `successorRef`.

  

```c

struct TreeNode **successorRef = &((*root)->right);
while ((*successorRef)->left != NULL)
{
    successorRef = &((*successorRef)->left);
}
struct TreeNode *successor = *successorRef;
free(forDel->contact);
forDel->contact = successor->contact;
*successorRef = successor->right;
free(successor);


```

  

### Редактирование контакта

Функция разбирает вариативный формат аргументов. Если редактируется фамилия (ключ поиска `'l'`), то во избежание нарушения порядка элементов, старый контакт полностью удаляется из дерева, а затем заново создается и вставляется с новой фамилией через `addContact`.

```c
   if (lastNameChanged)
  {
    struct Contact oldContact = *(tmp->contact);
    snprintf(oldContact.lastName, NAME_LEN, "%s", newLastName);
    delNode(root, tmp->contact->lastName);
    addContact(root, oldContact.id, oldContact.firstName, oldContact.lastName,
               oldContact.job, oldContact.phoneNumber, oldContact.address.country,
               oldContact.address.city, oldContact.address.street);
  }
```

### Периодическая балансировка

Для предотвращения вырождения бинарного дерева поиска в связный список (при вводе данных по алфавиту) реализована автоматическая балансировка. Она вызывается каждые `N` успешных модификаций.


#### 1. Сбор данных в массив

Дерево рекурсивно обходится симметричным методом, указатели на узлы копируются в динамический массив.

#### 2. Восстановление идеально сбалансированного дерева

Массив делится пополам. Элемент из середины (`mid`) становится локальным корнем поддерева, обеспечивая минимально возможную высоту дерева. Левая и правая части массива рекурсивно формируют его левых и правых потомков.
  

```c
int mid = (end - beg) / 2 + beg;
struct TreeNode *root = nodes[mid];
root->left = buildTreeFromArray(nodes, beg, mid - 1);
root->right = buildTreeFromArray(nodes, mid + 1, end);
return root;
```

## Тестирование
Для проверки корректности работы телефонной книги разработан набор автоматических тестов с использованием фреймворка `Unity`. Тесты полностью изолированы друг от друга — состояние корня дерева и счетчик модификаций сбрасываются перед каждым запуском в функции `setUp`.

#### 1. Удаление узла с одним потомком (`test_DelNode_NodeWithOneChild_ShouldBypassNode`)
Проверяет корректность перестройки связей при удалении узла, у которого есть только один ребенок (левый или правый). Тест контролирует, что родитель удаляемого узла перенаправляет свою ссылку напрямую на «внука», минуя удаляемый элемент, и структура дерева не нарушается.

#### 2. Проверка структуры BST (`test_AddContact_ShouldInsertInCorrectBstOrder`)
Контролирует правильность работы рекурсивной вставки. Проверяет, что первый добавленный контакт становится главным корнем дерева, элементы меньше корня по алфавиту уходят строго в левое поддерево, а элементы больше — строго в правое.

#### 3. Поиск элементов (`test_FindContact_ShouldFindExistingAndReturnMinusOneForMissing`)
Проверяет два сценария поиска по фамилии:
- Успешный поиск существующего контакта и возврат корректного указателя на узел.
- Поиск отсутствующего контакта (функция должна вернуть `-1`, а возвращаемый указатель занулить).

#### 4. Удаление листа (`test_DelNode_LeafNode_ShouldRemoveSuccessfully`)
Тестирует базовый случай удаления элемента, у которого нет потомков. Проверяет, что память из-под контакта и узла успешно освобождается, а соответствующий указатель (`left` или `right`) у родительского узла становится равен `NULL`.

#### 5. Редактирование полей без смены фамилии (`test_EditContact_FieldChangeWithoutLastName_ShouldKeepStructure`)
Проверяет изменение обычных данных контакта (например, телефона или должности), которые не влияют на ключ сортировки. Тест контролирует, что данные обновляются успешно, а сам узел остается на своем прежнем месте в дереве.

#### 6. Редактирование со сменой ключа (`test_EditContact_LastNameChange_ShouldMoveNodeToCorrectPosition`)
Проверяет самый сложный сценарий редактирования. Тест подтверждает, что при изменении фамилии (ключа поиска) контакт корректно извлекается из старого места в дереве и вставляется в новую позицию в соответствии с алфавитным порядком, сохраняя целостность BST.

#### 7. Автоматическая балансировка (`test_PeriodicBalance_ShouldRebuildTreeToMinimalHeight`)
Имитирует худший случай для BST — добавление 5 элементов в алфавитном порядке. Тест проверяет, что на 5-ой операции автоматически срабатывает балансировка, высота дерева становится минимальной, а средний элемент массива становится новым корнем.
