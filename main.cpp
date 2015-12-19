/** @file */
#include "header.h"

int main(int argc, char **args) {
    double **array = NULL;
    unsigned int rows = 0, cols = 0;
    int iterator;
    char character;

    if (argc == 1) { // zadna data v parametrech
        cout << "Nacist matici ze souboru? [a/n]: ";
        cin >> character;

        if (character == 'a' || character == 'A') {
            array = load_matrix_file(rows, cols);
        } else {
            cout << "load_matrix_user" << endl;
            array = load_matrix_user(rows, cols);
        }
    }

    if (argc == 2) {
        cout << "load_matrix_file" << endl;
        array = load_matrix_file(args[1], rows, cols);
    }
    else if (argc > 2) {
        cout << "Nerozpoznane parametry programu" << endl;
        cout << "program [input_file]" << endl;
        cout << "program - nazev programu" << endl;
        cout << "input_file - soubor s matici\n"
                "\ttextovy soubor, na kazdem radku je stejny pocet cisel,"
                "cisla jsou oddelena bilym znakem" << endl;
        return 1;
    }

    print(array, cols, rows);

    double **triangle = gauss_elimination((const double **) array, cols, rows);

    print(array, cols, rows);

    if (triangle != 0) {
        cout << "Vypocteno" << endl;
        print(triangle, cols, rows);

        for (iterator = 0; iterator < rows; ++iterator) {
            cout << "x" << iterator + 1 << " = " << triangle[iterator][cols - 1] << endl;
        }

        cout << "Chcete ulozit do souboru? [a/n]: ";
//        cin.ignore(INT_MAX, '\n'); // maze vsechna data na vstupu
        cin >> character;
        if (character == 'a' || character == 'A' || character == '\n') {
            saveToFile(triangle, cols, rows);
        }
    } else {
        cout << "Nelze vypocitat pomoci gaussovy eliminacni metody" << endl;
    }

    if (triangle != 0) {
        delete[] triangle[0];
        for (iterator = 1; iterator < rows; ++iterator) {
            delete[] triangle[iterator];
        }
        delete[] triangle;
    }

    delete[] array[0];
    for (iterator = 1; iterator < rows; ++iterator) {
        delete[] array[iterator];
    }
    delete[] array;

    return 0;
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

    // nacintani prvniho radku matice
    while (file.peek() != '\n') {
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
    while (1) {
        cin.ignore(INT_MAX, '\n'); // maze vsechna data na vstupu
        cout << "Nazev souboru: ";
        cin.getline(file, 256);
        if (file_exist(file))
            break;
        cout << "Chyba, soubor neexistuje" << endl;
    }
    return load_matrix_file(file, rows, cols);
}

double **load_matrix_user(unsigned int &rows, unsigned int &cols) {
    unsigned int array_rows = 3, array_cols = 4;
    double **array;

    cout << "Nazdar, zadej matici:" << endl;

    array = new double *[array_rows]; // pole ukazatelu, bude obsahovat adresy radku
    array[0] = new double[array_cols]; // prvni radek matice

    // nacinatni prvniho radku matice
    while (cin.peek() != '\n') {
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

    cin.get();

    // nacitani dalsich radku
    for (; cin.peek() != '\n'; ++rows) {
        if (rows >= array_rows) {
            make_bigger(array, array_rows);
        }
        array[rows] = new double[cols];

        for (int col = 0; col < cols; ++col) {
            array[rows][col] = get_number();
        }

        cin.get();
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

// prekopiruje velke pole do mensiho,
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

    int i, j, k;
    double c, sum, *values, **array;
    values = new double[rows];
    array = new double *[rows]; // vytvori pole ukazatelu

    for (i = 0; i < rows; ++i) { // kopirovani celeho pole
        array[i] = new double[cols];
        copy(array_source[i], array_source[i] + cols, array[i]); // kopirovani dat
    }

    // upravi pole ze vlevo dole jsou nuly
    for (j = 0; j < rows; j++) // prochazi cele pole po radcich
    {
        for (i = j + 1; i < rows; i++) { // prochazi vsechny radky pod radkem j
            c = array[i][j] /
                array[j][j]; // konstanta, kterou se musi vynasobit hodnoty radku, slo vyrobit nulu
            for (k = 0; k <= rows; k++) {
                array[i][k] = array[i][k] - c * array[j][k]; // nasobeni vsech hodnot v radku
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

void saveToFile(double **array, unsigned int cols, unsigned int rows) {
    string output_file;
    char character;

    cin.ignore(INT_MAX, '\n'); // maze vsechna data na vstupu
    while (1) {
        cout << "Zadejte nazev souboru: ";
        getline(cin, output_file);

        if (!regex_match(output_file, regex("[a-zA-Z0-9]+.html"))) {
            cout << "Nazev souboru se musi skladat z pismen nebo cislic a mit priponu .html" << endl;
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