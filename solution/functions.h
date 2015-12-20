/**
 * @file functions.h
 * Hlavickovy soubor
 *
 * @author Michal Kalita
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

/**
 * Zvetsi pole ukazatelu na dvou nasobek puvodni velikosti
 * @param array pole
 * @param size puvodni velikost
 */
void make_bigger(double *&array, unsigned int &size);

/**
 * Zvetsi pole ukazatelu na ukazatele na dvou nasobek puvodni velikosti
 * @param array pole
 * @param size puvodni velikost
 * @sa make_bigger()
 */
void make_bigger(double **&array, unsigned int &size);

/**
 * Optimalizuje velikost pole ukazatelu
 * vytvori nove pole s optimalni velikosti a zkopiruje data, pote stare pole uvolni
 * @param array pole
 * @param size optimalni velikost
 * @sa optimize_array_size
 */
void optimize_array_size(double *&array, unsigned int size);

/**
 * Optimalizuje velikost pole ukazatelu ukazatelu
 * vytvori nove pole s optimalni velikosti a zkopiruje data, pote stare pole uvolni
 * @param array pole
 * @param size optimalni velikost
 * @sa optimize_array_size
 */
void optimize_array_size(double **&array, unsigned int size);

/**
 * Vypise obsah matice na standartni vystup cout
 * @param array pole
 * @param cols pocet sloupcu
 * @param rows pocet radku
 */
void print(double **array, unsigned int cols, unsigned int rows);

/**
 * Vypocita hodnoty matice pomoci gaussovy eliminacni metody
 * @param array_source matice
 * @param cols pocet sloupcu
 * @param rows pocet radku
 * @return vysledne pole nebo nullptr pokud nelze vypocitat
 */
double **gauss_elimination(const double **array_source, unsigned int cols, unsigned int rows);

/**
 * Ulozi matici do souboru
 * @param array matice
 * @param cols pocet sloupcu
 * @param rows pocet radku
 */
void saveToFile(double **array, unsigned int cols, unsigned int rows);

/**
 * Overuje existenci souboru
 * @param name nazev souboru
 * @return pravda, existuje-li soubor
 */
bool inline file_exist(const string &name);

/**
 * Ziska od uzivatele cislo pomoci standartniho vstupu cin
 * @return ziskana hodnota
 */
double get_number();

/**
 * Nacte matici od uzivatele
 * @param rows vraci pocet nactenych radku
 * @param cols vraci pocet nactenych sloupcu
 * @return nactena matice
 * @sa load_matrix_file, get_number
 */
double **load_matrix_user(unsigned int &rows, unsigned int &cols);

/**
 * Nacte matici ze souboru, na nazev souboru se pta uzivatele
 * @param rows vraci pocet nactenych radku
 * @param cols vraci pocet nactenych sloupcu
 * @return nactena matice
 * @sa load_matrix_file, load_matrix_user
 */
double **load_matrix_file(unsigned int &rows, unsigned int &cols);

/**
 * Nacte matici ze souboru
 * @param file_ch nazev souboru
 * @param rows vraci pocet nactenych radku
 * @param cols vraci pocet nactenych sloupcu
 * @return nactena matice
 * @sa load_matrix_user, load_matrix_file
 */
double **load_matrix_file(char *file_ch, unsigned int &rows, unsigned int &cols);

/**
 * Maze vsechny bile znaky ze vstupniho streamu
 * @param in vstupni stream
 * @return pravda, pokud serie bilych znaku konci novym radkem nebo koncem souboru
 */
bool clear_stream(istream &in);

/**
 * Na hlavni diagonale nesmi byt nuly, ani o jednu diagonalu pod hlavni, nesmi byt nuly
 * @param array vstupne vystupni dvojrozmerne pole
 * @param cols pocet sloupcu
 * @param rows pocet radku
 * @return zdarilo se odstranit nuly z hlavni diagonaly
 */
bool solve_zero_problem(double **&array, unsigned int cols, unsigned int rows);

/**
 * Vypocita determinant matice
 * @param array matice
 * @param cols pocet sloupcu
 * @param rows pocet radku
 * @return determinant nebo nekonecno, kdyz nelze vypocist
 */
double determinant(double **array, unsigned int cols, unsigned int rows);

#endif //MATICE_HEADER_H
