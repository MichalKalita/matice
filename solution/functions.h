/**
 * @file
 * Hlavickovy soubor
 */

#ifndef MATICE_HEADER_H
#define MATICE_HEADER_H

#include <iostream>
#include <fstream>
#include <regex>
#include <cctype>
#include <cmath>
#include <algorithm>

using namespace std;

void make_bigger(double *&array, unsigned int &size);

void make_bigger(double **&array, unsigned int &size);

void optimize_array_size(double *&array, unsigned int size);

void optimize_array_size(double **&array, unsigned int size);

void print(double **array, unsigned int cols, unsigned int rows);

double **gauss_elimination(const double **array_source, unsigned int cols, unsigned int rows);

void saveToFile(double **array, unsigned int cols, unsigned int rows);

bool inline file_exist(const string &name);

double get_number();

double **load_matrix_user(unsigned int &rows, unsigned int &cols);

double **load_matrix_file(unsigned int &rows, unsigned int &cols);

double **load_matrix_file(char *file_ch, unsigned int &rows, unsigned int &cols);

bool clear_stream(istream &in);

bool solve_zero_problem(double **&array, unsigned int cols, unsigned int rows);

double determinant(double **array, unsigned int cols, unsigned int rows);

#endif //MATICE_HEADER_H
