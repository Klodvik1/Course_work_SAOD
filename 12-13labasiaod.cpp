#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>

using namespace std;

void shannon(const int n, double p[], int Length[], char c[][20]) {
  double *q = new double[n];
  q[0] = 0;
  Length[0] = -floor(log2(p[0]));
  for (int i = 1; i < n; ++i) {
    q[i] = q[i - 1] + p[i - 1];
    Length[i] = -floor(log2(p[i]));
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 0; j < Length[i - 1]; ++j) {
      q[i - 1] *= 2;
      c[i - 1][j] = floor(q[i - 1]) + '0';
      if (q[i - 1] >= 1) {
        q[i - 1] -= 1;
      }
    }
  }
}

void Purrrr(const int n, double p[], int Length[], char c[][20]) {
  double *q = new double[n];
  Length[0] = -floor(log2(p[0])) + 1;
  q[0] = p[0] / 2;
  for (int i = 1; i < n; ++i) {
    double tmp = 0;
    for (int k = 0; k < i; k++)
      tmp += p[k];

    q[i] = tmp + p[i] / 2;
    Length[i] = -floor(log2(p[i])) + 1;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < Length[i]; ++j) {
      q[i] *= 2;
      c[i][j] = floor(q[i]) + '0';
      if (q[i] >= 1) {
        q[i] -= 1;
      }
    }
  }
}

int Med(int L, int R, double *p) {
  double SL = 0;
  for (int i = L; i <= R; i++) {
    SL = SL + p[i];
  }
  double SR = p[R];
  int m = R;
  while (SL >= SR) {
    m--;
    SL = SL - p[m];
    SR = SR + p[m];
  }
  return m;
}

void Fano(int L, int R, int k, double *p, int Length[], char c[][20]) {
  int m;
  if (L < R) {
    k++;
    m = Med(L, R, p);
    for (int i = L; i <= R; i++) {
      if (i <= m) {
        c[i][k] = '0';
        Length[i] = Length[i] + 1;
      } else {
        c[i][k] = '1';
        Length[i] = Length[i] + 1;
      }
    }
    Fano(L, m, k, p, Length, c);
    Fano(m + 1, R, k, p, Length, c);
  }
}

int up(int n, double q, double *array, double chance[]) {
  int i = 0, j = 0;
  for (i = n - 2; i >= 1; i--) {
    if (array[i - 1] < q)
      array[i] = array[i - 1];
    else {
      j = i;
      break;
    }
    if ((i - 1) == 0 && chance[i - 1] < q) {
      j = 0;
      break;
    }
  }
  array[j] = q;
  return j;
}

void down(int n, int j, int Length[], char c[][20]) {
  char pref[20];
  for (int i = 0; i < 20; i++)
    pref[i] = c[j][i];
  int l = Length[j];
  for (int i = j; i < n - 2; i++) {
    for (int k = 0; k < 20; k++)
      c[i][k] = c[i + 1][k];
    Length[i] = Length[i + 1];
  }
  for (int i = 0; i < 20; i++) {
    c[n - 2][i] = pref[i];
    c[n - 1][i] = pref[i];
  }
  c[n - 1][l] = '1';
  c[n - 2][l] = '0';
  Length[n - 1] = l + 1;
  Length[n - 2] = l + 1;
}

void huffman(int n, double *array, int Length[], char c[][20],
             double chance[]) {
  if (n == 2) {
    c[0][0] = '0';
    Length[0] = 1;
    c[1][0] = '1';
    Length[1] = 1;
  } else {
    double q = array[n - 2] + array[n - 1];
    int j = up(n, q, array, chance);
    huffman(n - 1, array, Length, c, chance);
    down(n, j, Length, c);
  }
}

unordered_map<char, int> get_char_counts_from_file(const string &file_name,
                                                   int &file_size) {
  ifstream file(file_name);
  if (!file.is_open()) {
    throw runtime_error("Could not open file");
  }
  unordered_map<char, int> counter_map;
  file_size = 0;
  ifstream fin(file_name, ios::binary);
  while (!fin.eof()) {
    char chr;
    fin.read(reinterpret_cast<char *>(&chr), sizeof(chr));
    file_size++;
    counter_map[chr]++;
  }
  file.close();
  return counter_map;
}

vector<pair<double, char>>
calc_probabilities(const unordered_map<char, int> &counter_map, int count) {
  vector<pair<double, char>> probabilities;
  probabilities.reserve(counter_map.size());
  for (auto i : counter_map) {
    probabilities.emplace_back(make_pair((double)i.second / count, i.first));
  }
  return probabilities;
}

int main() {
  double entropy;
  double avgShannon, avgFano, avgGilbert, avgHaffman;
  int file_size;
  unordered_map<char, int> counter_map;
  try {
    counter_map = get_char_counts_from_file("file.txt", file_size);
  } catch (runtime_error &exc) {
    cout << exc.what();
    return 1;
  }
  auto probabilities = calc_probabilities(counter_map, file_size);
  counter_map.clear();

  sort(probabilities.begin(), probabilities.end(),
       greater<pair<double, char>>());

  const int n = (int)probabilities.size();

  auto c = new char[n][20];
  auto Length = new int[n];
  auto p = new double[n];
  for (int i = 0; i < n; ++i) {
    p[i] = probabilities[i].first;
    entropy += p[i] * log2(p[i]);
  }

  shannon(n, p, Length, c);
  cout << "\nShannon Code:\n";
  cout << "\nai         Pi:          Li:          Code:\n";
  for (int i = 0; i < n; i++) {
    printf(" %c |     %4.6lf    |   %d   |", probabilities[i].second, p[i],
           Length[i]);
    avgShannon += p[i] * Length[i];
    for (int j = 0; j < Length[i]; ++j) {
      printf("  %c", c[i][j]);
    }
    cout << '\n';
  }

  Purrrr(n, p, Length, c);
  cout << "\nGilbert-Mur Code:\n";
  cout << "\nai         Pi:          Li:        Code:\n";
  for (int i = 0; i < n; i++) {
    printf(" %c |     %4.6lf    |   %d   |", probabilities[i].second, p[i],
           Length[i]);
    avgGilbert += p[i] * Length[i];
    for (int j = 0; j < Length[i]; ++j) {
      printf("%c", c[i][j]);
    }
    cout << '\n';
  }

  cout << "\nFano Code:\n";
  for (int i = 0; i < n; ++i) {
    Length[i] = 0;
  }
  Fano(0, n - 1, -1, p, Length, c);

  cout << "\nai         Pi:          Li:        Code:\n";
  for (int i = 0; i < n; i++) {
    printf(" %c |     %4.6lf    |   %d   |", probabilities[i].second, p[i],
           Length[i]);
    avgFano += p[i] * Length[i];
    for (int j = 0; j < Length[i]; ++j) {
      printf("  %c", c[i][j]);
    }
    cout << '\n';
  }

  double chance_l[n];
  for (int i = 0; i < n; ++i) {
    Length[i] = 0;
    chance_l[i] = p[i];
  }
  cout << "\nHaffman Code:\n";
  huffman(n, chance_l, Length, c, p);
  cout << "\nai         Pi:          Li:      Code:\n";
  for (int i = 0; i < n; i++) {
    printf(" %c |     %4.6lf    |   %d   |", probabilities[i].second, p[i],
           Length[i]);
    avgHaffman += p[i] * Length[i];
    for (int j = 0; j < Length[i]; ++j) {
      printf("%c", c[i][j]);
    }
    cout << '\n';
  }
  delete[] p;
  printf("\n Entropy|      Average Lenght Code Word         |  \n");
  printf("________|_______________________________________|");
  printf("\n        | Haffman | Shennon |   Fano  | Gilbert | ");
  std::cout << "\n"
            << -entropy << " | " << avgHaffman << " | " << avgShannon << " | "
            << avgFano << " | " << avgGilbert << " | " << std::endl;
  return 0;
}
