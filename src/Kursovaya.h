#pragma once
#include <string.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cmath>

#define MAX 4000

struct biography {
  char author[12];
  char title[32];
  char publishing[16];
  unsigned short int year;
  unsigned short int pages;
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

struct coding {
  char symbol;
  float probability;
  int binary[500];
  int length;
} ptr[500];

typedef struct coding coding;

void menu(void);
void init_list(void);
void load(void);
void HeapSort(biography* index[], int);
void HeapSort_year(biography* index[], int, int);
void HeapSort_author(biography* index[], int, int, int);
void search(biography* index[], int);
void AVL_search();
void encoding();
void sortByProbability(int, coding ptr[]);
void Haffman(int, double *array, coding ptr[]);
void down(int, int, coding ptr[]);
int  up(int, double, double *array);
void push(tLE* p);
void pop();
void LL_turn(tree*& p);
void LR_turn(tree*& p);
void RR_turn(tree*& p);
void RL_turn(tree*& p);
void DestroyTree(tree* p);
void AddAVL_Tree(tree*& p, tLE* D);
void output_queue();
void AVL_output(tree* p);
void output_encoding(int, coding ptr[]);
void input_database(biography* index[]);
void clean(void);
