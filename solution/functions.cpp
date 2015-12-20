/**
 * @file functions.cpp
 * Zdrojovy kod vsech funkci
 *
 * @author Michal Kalita
 */

#include "functions.h"

bool clear_stream(istream &in) {
    int c;

    while (1) {
        c = in.peek();
        if (c == '\n') {
            in.get();
            return false;
        } else if (c == EOF) {
            return false;
        }
        if (!isspace(c))
            return true;
        in.get();
    }
}

double **load_matrix_file(char *file_ch, unsigned int &rows, unsigned int &cols) {
    ifstream file;
    file.open(file_ch);

    if (!file.is_open()) {
        cout << "Chyba, soubor nelze otevrit" << endl;
        return nullptr;
    }

    unsigned int array_rows = 3, array_cols = 4;
    double **array = NULL;

    array = new double *[array_rows]; // pole ukazatelu, bude obsahovat adresy radku
    array[0] = new double[array_cols]; // prvni radek matice

    cols = rows = 0;

    // nacitani prvniho radku matice
    while (clear_stream(file)) {
        if (cols >= array_cols) { // neni misto v poli, je treba zvetsit pole
            make_bigger(array[0], array_cols);
        }
        file >> array[0][cols];
        ++cols;
    }

    if (cols == 0) {
        cout << "konec, zadna data" << endl;
        return nullptr;
    }

    rows++;

    // pokud pole je vetsi nez pocet hodnot, optimalizuje pole
    if (cols != array_cols) {
        optimize_array_size(array[0], cols);
        array_cols = cols;
    }

    bool skip_create = false, ok;
    // nacitani dalsich radku
    for (; !file.eof(); ++rows) { // projde cely soubor a pricita pocet radku
        if (rows >= array_rows) { // overuje, zda jsou volne pozice
            make_bigger(array, array_rows);
        }

        if (skip_create) // vytvori novy radek pokud je potreba
            skip_create = false;
        else
            array[rows] = new double[cols];

        ok = false;
        for (int col = 0; col < cols; ++col) { // prochazeni sloupcu
            file >> array[rows][col]; // nacteni hodnoty
            if (array[rows][col] != 0) {
                ok = true; // stav radku
            }
        }

        if (!ok) { // pokud v radku jsou vsechny hodnoty 0, opakuje nacitani do stejne adresy
            rows--;
            skip_create = true;
        }
    }

    if (rows != array_rows) {
        optimize_array_size(array, rows);
        array_rows = rows;
    }

    return array;
}

double **load_matrix_file(unsigned int &rows, unsigned int &cols) {
    char file[256];
    clear_stream(cin);
    while (1) {
        cout << "Nazev souboru: ";
        cin.getline(file, 256, '\n');
        if (file_exist(file))
            break;
        cout << "Chyba, soubor neexistuje" << endl;
    }
    return load_matrix_file(file, rows, cols);
}

double **load_matrix_user(unsigned int &rows, unsigned int &cols) {
    unsigned int array_rows = 3, array_cols = 4;
    double **array;

    cout << "Zadejte matici:" << endl;

    array = new double *[array_rows]; // pole ukazatelu, bude obsahovat adresy radku
    array[0] = new double[array_cols]; // prvni radek matice

    cin.ignore(INT_MAX, '\n'); // maze vsechna data na vstupu
    // nacinatni prvniho radku matice
    while (clear_stream(cin)) {
        if (cols >= array_cols) { // neni misto v poli, je treba zvetsit pole
            make_bigger(array[0], array_cols);
        }
        array[0][cols] = get_number(); // TODO: udelat uzivatelsky privetivejsi
        ++cols;
    }

    if (cols == 0) {
        cout << "konec, zadna data" << endl;
        return nullptr;
    }

    rows++;

    // pokud pole je vetsi nez pocet hodnot, optimalizuje pole
    if (cols != array_cols) {
        optimize_array_size(array[0], cols);
        array_cols = cols;
    }

    clear_stream(cin);

    // nacitani dalsich radku
    for (; clear_stream(cin); ++rows) {
        if (rows >= array_rows) {
            make_bigger(array, array_rows);
        }
        array[rows] = new double[cols];

        for (int col = 0; col < cols; ++col) {
            array[rows][col] = get_number();
        }

        clear_stream(cin);
    }

    if (rows != array_rows) {
        optimize_array_size(array, rows);
        array_rows = rows;
    }

    cout << "konec nacitani, zadana matice " << cols << "x" << rows << endl;

    return array;
}

double get_number() {
    double number;
    cin.clear();
    cin >> number;
    while (cin.fail()) {
        cout << endl;
        cout << "Spatny vstup, zadejte cislo: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> number;
        cout << endl;
    }
    return number;
}

void print(double **array, unsigned int cols, unsigned int rows) {
    cout << "Vypis matice:" << endl;
    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < cols; ++col) {
            cout << array[row][col];
            if (col != cols) {
                cout << "\t";
            }
        }
        cout << endl;
    }
}

void optimize_array_size(double *&array, unsigned int size) {
    double *temporary = new double[size]; // novy adresni prostor
    copy(array, array + size, temporary); // kopirovani dat do noveho mensiho prostoru
    delete array; // uvolneni pameti stareho pole
    array = temporary; // do pole je zapsana adresa dat
}

void optimize_array_size(double **&array, unsigned int size) {
    double **temporary = new double *[size]; // novy adresni prostor
    copy(array, array + size, temporary); // kopirovani dat do noveho mensiho prostoru
    delete array; // uvolneni pameti stareho pole
    array = temporary; // do pole je zapsana adresa dat
}

void make_bigger(double *&array, unsigned int &size) {
    unsigned int new_size = size * 2; // nova velikost pole je dvojnasobna
    double *temporary = new double[new_size]; // novy adresni prostor
    copy(array, array + size, temporary); // kopirovani dat do noveho vetsiho prostoru
    delete array; // uvolneni pameti stareho pole
    array = temporary; // do pole je zapsana adresa dat
    size = new_size; // vraceni velikosti pole pres parametr funkce
}

void make_bigger(double **&array, unsigned int &size) {
    unsigned int new_size = size * 2; // nova velikost pole je dvojnasobna
    double **temporary = new double *[new_size]; // novy adresni prostor
    copy(array, array + size, temporary); // kopirovani dat do noveho vetsiho prostoru
    delete array; // uvolneni pameti stareho pole
    array = temporary; // do pole je zapsana adresa dat
    size = new_size; // vraceni velikosti pole pres parametr funkce
}

double **gauss_elimination(const double **array_source, unsigned int cols, unsigned int rows) {
    if (rows != cols - 1) {
        return 0;
    }

    int i, j, k, problem_row = -1;
    double c, sum, *values, **array;
    values = new double[rows];
    array = new double *[rows]; // vytvori pole ukazatelu

    for (i = 0; i < rows; ++i) { // kopirovani celeho pole
        array[i] = new double[cols];
        copy(array_source[i], array_source[i] + cols, array[i]); // kopirovani dat
    }

    solve_zero_problem(array, cols, rows);

    // upravi pole ze vlevo dole jsou nuly
    for (j = 0; j < rows; j++) // prochazi cele pole po radcich
    {
        for (i = j + 1; i < rows; i++) { // prochazi vsechny radky pod radkem j
            if (array[j][j] == 0) {
                solve_zero_problem(array, cols, rows);

                if (problem_row == j) {
                    return nullptr;
                } else {
                    problem_row = j;
                }
            }
            c = array[i][j] /
                array[j][j]; // konstanta, kterou se musi vynasobit hodnoty radku, slo vyrobit nulu
            for (k = 0; k <= rows; k++) {
                array[i][k] = array[i][k] - c * array[j][k]; // nasobeni vsech hodnot v radku
                if(abs(array[i][k]) < 1e-9) { // pokud je cislo hodne blizko nule, bere jej jako nulu
                    array[i][k] = 0;
                }
            }
        }
    }

    // vypocita hodnoty dilcich promennych
    for (i = rows - 1; i >= 0; i--) { // prochazi vsechny radky odspodu
        sum = 0;
        for (j = i + 1; j < rows; j++) { // prochazi vsechny hodnoty v radku
            sum += array[i][j] * values[j];
        }
        values[i] = (array[i][cols - 1] - sum) / array[i][i];
    }

    // vygeneruje vystupni pole, ktere ma hodnoty v hezkem tvaru
    for (i = 0; i < rows; ++i) {
        array[i][i] = 1;
        for (j = i + 1; j < cols - 1; ++j) {
            array[i][j] = 0;
        }
        array[i][cols - 1] = values[i];
    }

    delete[] values;

    return array;
}

bool solve_zero_problem(double **&array, unsigned int cols, unsigned int rows) {
    int a, b, c, optimal, max_count_zero, count_zero;

    for (a = 0; a < rows; ++a) {
        // hledame radek, ktery by sel vymenit
        // nejoptimalnejsi je ten, ktery ma nejvic nul, ale nema je na pozici diagonaly
        optimal = -1;
        max_count_zero = 0;
        for (c = 0; c < cols - 1; ++c) {
            if (array[a][c] == 0)
                max_count_zero++;
        }
        for (b = a + 1; b < rows; ++b) {
            if (array[b][a] == 0 || array[a][b] == 0) // nelze vymenit, na hlavni diagonale je nula
                continue;

            count_zero = 0;
            for (c = 0; c < cols - 1; ++c) {
                if (array[b][c] == 0)
                    count_zero++;
            }

            // pokud je array[a][a] nula, pouzije nahradu i za cenu ze neni optimalni
            // pokud ma vice nul jak predesly vysledek
            if (count_zero > max_count_zero || (array[a][a] == 0 && optimal == -1)) {
                optimal = b; // optimalni radek pro vymenu je tenhle radek
                max_count_zero = count_zero;
            }
        }
        if (optimal == -1) { // nenalezena lepsi moznost
            if (array[a][a] == 0) { // nebylo nalezeno reseni, na diagonale je nule, nelze vypocitat matici
                return false;
            }
        } else {
            swap(array[a], array[optimal]);
        }
    }
    return true;
}

void saveToFile(double **array, unsigned int cols, unsigned int rows) {
    string output_file;
    char character;

    cin.ignore(INT_MAX, '\n'); // maze vsechna data na vstupu
    while (1) {
        cout << "Zadejte nazev souboru: ";
        getline(cin, output_file);

        if (!regex_match(output_file, regex("[a-zA-Z0-9/]*[a-zA-Z0-9]{1}.html"))) {
            cout << "Nazev souboru se musi skladat z pismen nebo cislic a musi mit priponu .html" << endl;
            continue;
        }

        if (file_exist(output_file)) {
            cout << "Tento soubor jiz existuje, chcete jej prepsat? [a/N]: ";
            cin.get(character);

            if (character != 'a' && character != 'A') { // odpoved neni ano, odpoved je ne
                continue;
            }
        }

        break;
    }

    ofstream file;
    file.open(output_file, ios_base::trunc);
    file.precision(3);
    file << "<!DOCTYPE HTML>\n"
            "<html>\n"
            "<head>\n"
            "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
            "    <title>Matice " << rows << "x" << cols << "</title>\n"
            "</head>\n"
            "<body>\n"
            "\n"
            "<h1>Matice " << rows << "x" << cols << "</h1>\n"
            "<h2>Vypoctena matice</h2>\n"
            "<table>\n";

    for (int row = 0; row < rows; row++) {
        file << "<tr>\n";
        for (int col = 0; col < cols; ++col) {
            file << "<td>" << array[row][col] << "</td>\n";
        }
        file << "</tr>\n";
    }

    file << "</table>\n"
            "<h2>Vysledky</h2>\n";

    for (int i = 0; i < rows; ++i) {
        file << "<h3>x<sub>" << i + 1 << "</sub> = " << array[i][cols - 1] << "</h3>\n";
    }

    file << "</body>\n"
            "</html>\n";

    cout << "Data byla ulozena do souboru " << output_file << endl;
}

bool inline file_exist(const string &name) {
    ifstream file;
    file.open(name.c_str());
    {
        file.close();
        return file.good();
    }
}

double determinant(double **array, unsigned int cols, unsigned int rows) {
    if (cols != rows || cols < 2) {
        return INFINITY;
    }

    if (cols == 2)
        return (array[0][0] * array[1][1]) - (array[0][1] * array[1][0]);

    double sum = 0, possitive, negative;

    for (int i = 0; i < rows; ++i) {
        possitive = negative = 1;
        for (int j = 0; j < cols; ++j) {
            if (i + j < rows) {
                possitive *= array[i + j][j];
                negative *= array[i + j][cols - j - 1];
            } else {
                possitive *= array[i + j - rows][j];
                negative *= array[i + j - rows][cols - j - 1];
            }
        }
        sum += possitive;
        sum -= negative;
    }

    return sum;
}
