#include "main.h"

int main(void) {
  menu();
  return 0;
}

void menu(void) {
  load();
  biography* index[MAX];
  biography* index_sort[MAX];
  for (int i = 0; i < MAX; i++) {
    index[i] = &biography_list[i];
    index_sort[i] = &biography_list[i];
  }
  HeapSort(index_sort, MAX);
  int flag = 1;
  char choice;
  while (flag == 1) {
    std::cout << "Menu:\n1) Output database\n2) Output sort database\n";
    std::cout
        << "3) Search queue\n4) Search AVL-tree\n5) Encoding Haffman\n6) End the programm"
        << std::endl;
    std::cin >> choice;
    clean();
    switch (choice) {
      case '1': {
        output_database(index);
        break;
      }
      case '2': {
        output_database(index_sort);
        break;
      }
      case '3': {
        search(index_sort, 0);
        clean();
        break;
      }
      case '4': {
        search(index_sort, 1);
        clean();
        break;
      }
      case '5': {
        encoding();
        clean();
        break;
      }
      case '6': {
        flag = 0;
        break;
      }
      default: { break; }
    }
    clean();
  }
}

void init_list(void) {
  for (int i = 0; i < MAX; i++) {
    biography_list[i].author[0] = '\0';
    biography_list[i].title[0] = '\0';
    biography_list[i].publishing[0] = '\0';
  }
}

void load(void) {
  FILE* fp;
  if ((fp = fopen("testBase1.dat", "rb")) != NULL) {
    init_list();
    for (int i = 0; i < MAX; i++) {
      if (fread(&biography_list[i], sizeof(struct biography), 1, fp) != 1) {
        if (feof(fp)) break;
        std::cout << "Ошибка при чтении файла.\n";
      }
    }
    fclose(fp);
  } else {
    std::cout << "Нет такого файла\n";
  }
}

void HeapSort(biography* index[], int R) {
  for (int i = R / 2 - 1; i >= 0; i--) {
    HeapSort_year(index, R, i);
  }
  for (int i = R - 1; i >= 0; i--) {
    std::swap(index[0], index[i]);
    HeapSort_year(index, i, 0);
  }
  for (int j = 1, L = 0; j < R; j++) {
    if ((index[j]->year != index[j - 1]->year) || (j == R - 1)) {
      if (j == R - 1) {
        j++;
      }
      j--;
      for (int i = (L + j) / 2; i >= L; i--) {
        HeapSort_author(index, i, j, L);
      }
      for (int i = j; i > L; i--) {
        std::swap(index[L], index[i]);
        i--;
        HeapSort_author(index, L, i, L);
        i++;
      }
      j++;
      L = j;
    }
  }
}

void HeapSort_year(biography* index[], int L, int R) {
  int largest = R;
  int l = 2 * R + 1;
  int r = 2 * R + 2;
  if ((l < L) && (index[l]->year > index[largest]->year)) largest = l;
  if ((r < L) && (index[r]->year > index[largest]->year)) largest = r;
  if (largest != R) {
    std::swap(index[R], index[largest]);
    HeapSort_year(index, L, largest);
  }
}

void HeapSort_author(biography* index[], int L, int R, int start) {
  int i, j, p = 0;
  biography* x = index[L];
  i = L;
  while (p < 1) {
    j = 2 * (i - start);
    if (j > R - start) break;
    if ((j < R - start) &&
        (strcmp(index[j + 1 + start]->author, index[j + start]->author) >= 0))
      j++;
    if (strcmp(x->author, index[j + start]->author) >= 0) break;
    std::swap(index[i], index[j + start]);
    i = j + start;
  }
  index[i] = x;
}

void search(biography* index[], int tree) {
  tLE* p;
  char ch[4];
  clean();
  std::cout << "Enter key search (Year) ";
  std::cin >> ch;
  clean();
  int flag = 1;
  for (int i = 0; (i < 4) && (flag == 1); i++) {
    if ((ch[i] < 48) || (ch[i] > 57)) flag = 0;
  }
  if (flag == 1) {
    int K = atoi(ch);
    Q.head = NULL;
    Q.tail = Q.head;
    for (int i = 0; i < MAX; i++) {
      if (index[i]->year == K) {
        p = new struct tLE;
        p->data = index[i];
        push(p);
      }
    }
    if (Q.head == NULL) {
      std::cout << "There is no such year in the database\n" << std::endl;
    } else {
      if (tree == 1) {
        AVL_search();
      } else {
        output_queue();
      }
      while (p != NULL) {
        p = Q.head;
        pop();
      }
    }
  } else {
    std::cout << "ERROR !!\n" << std::endl;
  }
  std::cout << "Press the keyboard to exit ";
  std::cin >> ch;
}

void AVL_search() {
  struct tree* Root = NULL;
  tLE* p = Q.head;
  while (p != NULL) {
    AddAVL_Tree(Root, p);
    p = p->next;
  }
  page = 0;
  AVL_output(Root);
  std::cout << "Enter key search (Author)";
  char ch[12];
  std::cin >> ch;
  clean();
  page = 0;
  AVL_output_key(Root, ch);
  DestroyTree(Root);
}

void encoding() {
  int n = 0;
  float total = 0.0;
  FILE* f;
  f = fopen("testBase1.dat", "rb");
  while (!feof(f)) {
    char ch;
    int flag = 1;
    ch = fgetc(f);
    if (!feof(f)) {
      total++;
      for (int i = 0; i < n; i++) {
        if (ptr[i].symbol == ch) {
          ptr[i].probability += 1;
          flag = 0;
          break;
        }
      }
      if (flag == 1) {
        ptr[n].symbol = ch;
        ptr[n].probability = 1;
        n++;
      }
    }
  }
  fclose(f);
  for (int i = 0; i < n; i++) {
    ptr[i].probability = ptr[i].probability / total;
  }
  sortByProbability(n, ptr);
  long double* chance_l = new long double[n];
  for (int i = 0; i < n; ++i) {
    ptr[i].length = 0;
    chance_l[i] = ptr[i].probability;
  }
  Haffman(n, chance_l, ptr);
  output_encoding(n, ptr);
  delete (chance_l);
}

void sortByProbability(int n, coding ptr[]) {
  coding temp;
  for (int j = 1; j <= n - 1; j++) {
    for (int i = 0; i < n - 1; i++) {
      if ((ptr[i].probability) < (ptr[i + 1].probability)) {
        temp.probability = ptr[i].probability;
        temp.symbol = ptr[i].symbol;

        ptr[i].probability = ptr[i + 1].probability;
        ptr[i].symbol = ptr[i + 1].symbol;

        ptr[i + 1].probability = temp.probability;
        ptr[i + 1].symbol = temp.symbol;
      }
    }
  }
}

void Haffman(int h, long double* array, coding ptr[]) {
  if (h == 2) {
    ptr[0].binary[0] = 0;
    ptr[0].length = 1;
    ptr[1].binary[0] = 1;
    ptr[1].length = 1;
  } else {
    double q = array[h - 2] + array[h - 1];
    int j = up(h, q, array);
    Haffman(h - 1, array, ptr);
    down(h, j, ptr);
  }
}

void down(int n, int j, coding ptr[]) { 
  int pref[20];
  for (int i = 0; i < 20; i++) pref[i] = ptr[j].binary[i];
  int l = ptr[j].length;
  for (int i = j; i < n - 2; i++) {
    for (int k = 0; k < 20; k++) ptr[i].binary[k] = ptr[i + 1].binary[k];
    ptr[i].length = ptr[i + 1].length;
  }
  for (int i = 0; i < 20; i++) {
    ptr[n - 2].binary[i] = pref[i];
    ptr[n - 1].binary[i] = pref[i];
  }
  ptr[n - 1].binary[l] = 1;
  ptr[n - 2].binary[l] = 0;
  ptr[n - 1].length = l + 1;
  ptr[n - 2].length = l + 1;
}

int up(int n, double q, long double* array) {
  int i = 0, j = 0;
  for (i = n - 2; i >= 1; i--) {
    if (array[i - 1] < q)
      array[i] = array[i - 1];
    else {
      j = i;
      break;
    }
    if ((i - 1) == 0 && ptr[i - 1].probability < q) {
      j = 0;
      break;
    }
  }
  array[j] = q;
  return j;
}

void push(tLE* p) {
  p->next = NULL;
  if (Q.head != NULL) {
    Q.tail->next = p;
  } else {
    Q.head = p;
  }
  Q.tail = p;
}

void pop() {
  tLE* p = Q.head;
  if (p != NULL) {
    Q.head = p->next;
    delete (p);
  }
}

void LL_turn(tree*& p) {
  tree* q;
  q = p->Left;
  p->Bal = 0;
  q->Bal = 0;
  p->Left = q->Right;
  q->Right = p;
  p = q;
}

void LR_turn(tree*& p) {
  tree* q;
  tree* r;
  q = p->Left;
  r = q->Right;
  if (r->Bal < 0) {
    p->Bal = 1;
  } else {
    p->Bal = 0;
  }
  if (r->Bal > 0) {
    q->Bal = -1;
  } else {
    q->Bal = 0;
  }
  r->Bal = 0;
  q->Right = r->Left;
  p->Left = r->Right;
  r->Left = q;
  r->Right = p;
  p = r;
}

void RR_turn(tree*& p) {
  tree* q;
  q = p->Right;
  p->Bal = 0;
  q->Bal = 0;
  p->Right = q->Left;
  q->Left = p;
  p = q;
}

void RL_turn(tree*& p) {
  tree* q;
  tree* r;
  q = p->Right;
  r = q->Left;
  if (r->Bal > 0) {
    p->Bal = -1;
  } else {
    p->Bal = 0;
  }
  if (r->Bal < 0) {
    q->Bal = 1;
  } else {
    q->Bal = 0;
  }
  r->Bal = 0;
  q->Left = r->Right;
  p->Right = r->Left;
  r->Left = p;
  r->Right = q;
  p = r;
}

void AddAVL_Tree(tree*& p, tLE* D) {
  if (p == NULL) {
    p = new struct tree;
    p->Data = D->data;
    p->Left = NULL;
    p->Right = NULL;
    p->Middle = NULL;
    p->Bal = 0;
  } else {
    if (strcmp(p->Data->author, D->data->author) > 0) {
      AddAVL_Tree(p->Left, D);
      if (Rost == 1) {
        if (p->Bal > 0) {
          p->Bal = 0;
          Rost = 0;
        } else {
          if (p->Bal == 0) {
            p->Bal = -1;
            Rost = 1;
          } else {
            if (p->Left->Bal < 0) {
              LL_turn(p);
              Rost = 0;
            } else {
              LR_turn(p);
              Rost = 0;
            }
          }
        }
      }
    } else {
      if (strcmp(p->Data->author, D->data->author) < 0) {
        AddAVL_Tree(p->Right, D);
        if (Rost == 1) {
          if (p->Bal < 0) {
            p->Bal = 0;
            Rost = 0;
          } else {
            if (p->Bal == 0) {
              p->Bal = 1;
              Rost = 1;
            } else {
              if (p->Right->Bal > 0) {
                RR_turn(p);
                Rost = 0;
              } else {
                RL_turn(p);
                Rost = 0;
              }
            }
          }
        }
      } else if (strcmp(p->Data->author, D->data->author) == 0) {
        AddAVL_Tree(p->Middle, D);
      }
    }
  }
}

void DestroyTree(tree* p) {
  if (p != NULL) {
    DestroyTree(p->Left);
    DestroyTree(p->Right);
    DestroyTree(p->Middle);
    delete (p);
  }
}

void output_queue() {
  tLE* p = Q.head;
  int i = 0;
  while (p != NULL) {
    std::cout << std::setw(2) << i + 1 << ' ' << std::setw(12)
              << p->data->author << std::setw(35) << p->data->title
              << std::setw(20) << p->data->publishing;
    std::cout << std::setw(10) << p->data->year << std::setw(10)
              << p->data->pages << std::endl;
    p = p->next;
    i++;
  }
}

void AVL_output(tree* p) {
  if (p != NULL) {
    AVL_output(p->Left);
    std::cout << std::setw(2);
    std::cout << page + 1;
    std::cout << ' ' << std::setw(12);
    std::cout << p->Data->author << std::setw(35) << p->Data->title;
    std::cout << std::setw(20) << p->Data->publishing;
    std::cout << std::setw(10) << p->Data->year << std::setw(10);
    std::cout << p->Data->pages << std::endl;
    page++;
    AVL_output(p->Middle);
    AVL_output(p->Right);
  }
}

void AVL_output_key(tree* p, char* key) {
  if (p != NULL) {
    AVL_output_key(p->Left, key);
    if (strncmp(key, p->Data->author, strlen(key)) == 0) {
      std::cout << std::setw(2);
      std::cout << page + 1;
      std::cout << ' ' << std::setw(12);
      std::cout << p->Data->author << std::setw(35) << p->Data->title;
      std::cout << std::setw(20) << p->Data->publishing;
      std::cout << std::setw(10) << p->Data->year << std::setw(10);
      std::cout << p->Data->pages << std::endl;
      page++;
    }
    AVL_output_key(p->Middle, key);
    AVL_output_key(p->Right, key);
  }
}

void output_encoding(int n, coding ptr[]) {
  std::cout << " Nom | Symbol |\tProbability\t|\t\tCode\t\t|";
  long double avgHaffman = 0, entropy = 0;
  int arg[14] = {0, 7, -79, 127, -80, 9, 15, 12, 11, 10, 8, 27, 13, 14};
  const char* charg[14] = {"NOP1", "NOP2",  "NOP3",  "NOP4", "NOP5",
                           "/t",   "NOP6",  "/n1",   "/n2",  "NOP6",
                           "/n3",  "DEL->", "DEL<-", "NOP7"};
  for (int i = 0; i < n; i++) {
    std::cout << "\n_____|________|_________________|__________________________"
                 "_____|\n";
    avgHaffman += ptr[i].probability * ptr[i].length;
    entropy += ptr[i].probability * log2(ptr[i].probability);
    std::cout << std::setw(3) << i + 1 << std::setw(3) << "|" << std::setw(4);
    int flag = 0;
    for (int j = 0; j < 14; j++) {
      if (arg[j] == ptr[i].symbol) {
        flag = 1;
        printf("  %s", charg[j]);
        std::cout << std::setw(3);
        break;
      }
    }
    if (flag == 0) {
      std::cout << ptr[i].symbol << std::setw(5);
    }
    std::cout << "|" << std::setw(15) << ptr[i].probability << std::setw(3)
              << "|" << std::setw(12);
    for (int j = 0; j <= ptr[i].length; j++) std::cout << ptr[i].binary[j];
    std::cout << std::setw(20 - ptr[i].length) << "|";
  }
  std::cout << "\n_____|________|_________________|____________________________"
               "___|\n";
  std::cout << "\n\nAverage Lenght Code Word: " << avgHaffman << std::endl;
  printf("Entropy: %.10Lf \n", -entropy);
  printf ("%.10Lf < %.10Lf", avgHaffman, -entropy +1);
  std::cout << "\n\nPress the keyboard to exit ";
  char ch;
  std::cin >> ch;
}

void output_database(biography* index[]) {
  int n = 1;
  char choice;
  int countMax = MAX / 20, flag = 1, i = 0;
  while (flag == 1) {
    int t = i;
    std::cout << std::setw(12) << "Author" << std::setw(15) << "Title";
    std::cout << std::setw(40) << "Publishing" << std::setw(13) << "Year";
    std::cout << std::setw(12) << "Pages" << std::endl;
    for (i = t; i < t + 20; i++) {
      std::cout << std::setw(2) << i + 1 << ' ' << std::setw(12);
      std::cout << index[i]->author << std::setw(35) << index[i]->title;
      std::cout << std::setw(20) << index[i]->publishing << std::setw(10);
      std::cout << index[i]->year << std::setw(10) << index[i]->pages;
      std::cout << std::endl;
    }
    std::cout << "\nPage " << n << ' ' << "of " << countMax << std::endl;
    std::cout
        << "Menu:\n1) Press 'n' or 'b' to scroll forward or backward to the "
        << std::endl;
    std::cout
        << "page.\n2) Press 'p' or 'm' to scroll 10 pages forward or backward."
        << std::endl;
    std::cout << "3) Press 's' or 'k' to jump to the beginning or end."
              << std::endl;
    std::cout << "4) Press 'q' to end the program." << std::endl;
    std::cin >> choice;
    clean();
    switch (choice) {
      case 'n': {
        if (i <= MAX - 20)
          n++;
        else
          i = t;
        break;
      }
      case 'p': {
        if (i <= MAX - 20) {
          n += 10;
          i += 180;
        } else
          i = t;
        break;
      }
      case 'b': {
        if (i >= 40) {
          n--;
          i -= 40;
        } else
          i = t;
        break;
      }
      case 'm': {
        if (i >= 200) {
          n -= 10;
          i -= 220;
        } else
          i = t;
        break;
      }
      case 's': {
        n = 1;
        i = 0;
        break;
      }
      case 'k': {
        n = 200;
        i = MAX - 20;
        break;
      }
      case 'q': {
        flag = 0;
        break;
      }
      default: {
        i = t;
        break;
      }
    }
  }
  clean();
}

void clean(void) { system("cls"); }
