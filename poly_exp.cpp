///////////////////////////////////////////////////////////////////////////////
// poly_exp.cpp
//
// Definitions for two algorithms:
//
// soccer_exhaustive
// soccer_dyn_prog
//
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "poly_exp.hpp"


void check_vector(const std::vector<std::string> field, bool exh) {
    if (field.size() == 0) {                       // 1
        throw std::invalid_argument("Not enough columns");                  // 1
    }
    else if (field[0].length() == 0) {                                  // 1
        throw std::invalid_argument("Not enough rows");                     // 1
    }
    if (exh){
      if (field.size() + field[0].length() - 2 >= 32) {
          throw std::invalid_argument("Too Long Input");
      }
    }
    int exp_length = field[0].length();                                 // 1
    for (int i = 0; i < field.size(); i++) {                            // 1  [n]
        if (field[i].length() != exp_length) {                              // 1
            throw std::invalid_argument("Rows not same size");                  // 1
        }
    }
    for (int i = 0; i < field.size(); i++) {                            // 1 [n]
       std::string curr_row = field[i];                                     // 1
       for (auto ch : curr_row) {                                               // 1 [m]
           if ((ch == '.') || (ch == 'X')) {                                        // 1
               continue;                                                                // 1
           }
           else {                                                                   // 1
               throw std::invalid_argument("Invalid Character");                        // 1
           }
       }
    }
}

bool check_cand(const std::vector<char> candidate, const std::vector<std::string> field) {
    int r_count = 0;
    int d_count = 0;
    if (candidate.size() == 0 && field[d_count][r_count] == 'X') {
         return false;
    }
    for (auto ch : candidate) {
        if (ch == 'r') {
            r_count++;
        }
        else if (ch == 'd') {
            d_count++;
        }
        if (d_count >= field.size()) {
            return false;
        }
        if (r_count >= field[0].length()) {
            return false;
        }
        if (field[d_count][r_count] == 'X') {
            return false;
        }

    }
    if (d_count == field.size() - 1 && r_count == field[0].length() - 1) {
        return true;
    }
    return false;
}

int algorithms::soccer_exhaustive(const std::vector<std::string> field) {
    check_vector(field,true);

    int len = field.size() + field[0].length() - 2;
    int counter = 0;
    for (int b = 0; b <= pow(2, len)-1; b++) {
        std::vector<char> candidate;
        for (int k = 0; k <= len - 1; k++) {
            auto bit = (b >> k) & 1;
            if (bit == 1) {
                candidate.push_back('r');
            }
            else {
                candidate.push_back('d');
            }
        }
        if (check_cand(candidate, field)) counter = counter + 1;

    }
    return counter;
}

int algorithms::soccer_dyn_prog(const std::vector<std::string> field) {
check_vector(field,false);
 // Corner case: initial cell is impassable
    if (field[0][0] == 'X') {
        return 0;
    }
    int row = field.size();
    int col = field[0].length();
    int** A = new int* [row];
    for (int i = 0; i < row; i++) {
        A[i] = new int[col];
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            A[i][j] = { 0 };
        }
    }
    A[0][0] = 1;
    for (int i = 0; i <= row - 1; i++) {
        for (int j = 0; j <= col - 1; j++) {
            if (field[i][j] == 'X') {
                A[i][j] = 0;
                continue;
            }
            int above = 0;
            int left = 0;
            if (i > 0 && field[i - 1][j] == '.') {
                above = A[i - 1][j];
            }
            if (j > 0 && field[i][j - 1] == '.') {
                left = A[i][j - 1];
            }
            A[i][j] += above + left;
        }
    }
    return A[row-1][col-1];
}
