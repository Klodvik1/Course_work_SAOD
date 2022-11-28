#pragma once
#include <string.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#define MAX 4000

struct biography {
  char author[12];  // Автор 12 символов Фамилия И О
  char title[32];  // Заглавие 32 символа Имя Отчество Фамилия
  char publishing[16];       // Издательство 16 символов
  unsigned short int year;   // Год заглавления
  unsigned short int pages;  // Кол-во страниц
} biography_list[MAX];

struct tLE {
  tLE* next;
  biography* data;
};

struct Queue {
  tLE* head;
  tLE* tail;
} Q;

int Rost = 0, page;

struct tree {
  biography* Data;
  tree* Left;
  tree* Middle;
  tree* Right;
  int Bal;
};

void menu(void);
void init_list(void);
void load(void);
void HeapSort(biography* index[], int);
void HeapSort_year(biography* index[], int, int);
void HeapSort_author(biography* index[], int, int, int);
void search(biography* index[], int);
void AVL_search();
void push(tLE* p);
void pop();
void LL_turn(tree*& p);
void LR_turn(tree*& p);
void RR_turn(tree*& p);
void RL_turn(tree*& p);
void AddAVL_Tree(tree*& p, tLE* D, int);
void AVL_key(tree* p, int);
void DestroyTree(tree* p);
void output_AVL(tree* p);
void output_queue();
void input_database(biography* index[]);
void clean(void);
