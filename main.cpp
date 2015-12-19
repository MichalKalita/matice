/** @file */
#include <iostream>
#include <fstream>
#include <regex>


using namespace std;

void make_bigger(double *&array, unsigned int &size);

void make_bigger(double **&array, unsigned int &size);

void optimize_array_size(double *&array, unsigned int size);

void optimize_array_size(double **&array, unsigned int size);

void print(double **array, unsigned int cols, unsigned int rows);

double **gauss_elimination(double **array, unsigned int cols, unsigned int rows);

void saveToFile(double **array, unsigned int cols, unsigned int rows);

bool inline file_exist(const string &name);

int main() {
    double **array = NULL;
    unsigned int rows = 0, cols = 0, array_rows = 1, array_cols = 10;
    int iterator;
    char character;

    cout << "Nazdar, zadej matici:" << endl;

    array = new double *[array_rows]; // pole ukazatelu, bude obsahovat adresy radku
    array[0] = new double[array_cols]; // prvni radek matice

    // nacinatni prvniho radku matice
    while (cin.peek() != '\n') {
        if (cols >= array_cols) { // neni misto v poli, je treba zvetsit pole
            make_bigger(array[0], array_cols);
        }
        cin >> array[0][cols];
        ++cols;
    }

    if (cols == 0) {
        cout << "konec, zadna data" << endl;
        return 1;
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
            cin >> array[rows][col];
        }

        cin.get();
    }

    if (rows != array_rows) {
        optimize_array_size(array, rows);
        array_rows = rows;
    }

    cout << "konec nacitani, zadana matice " << cols << "x" << rows << endl;

    print(array, cols, rows);

    double **triangle = gauss_elimination(array, cols, rows);
    if (triangle != 0) {
        cout << "Vypocteno" << endl;
        print(triangle, cols, rows);

        for (iterator = 0; iterator < rows; ++iterator) {
            cout << "x" << iterator + 1 << " = " << triangle[iterator][cols - 1] << endl;
        }

        cout << "Chcete ulozit do souboru? [A/n]: ";
        cin.ignore(INT_MAX, '\n'); // maze vsechna data na vstupu
        cin.get(character);
        if (character == 'a' || character == 'A' || character == '\n') {
            saveToFile(triangle, cols, rows);
        }
    } else {
        cout << "Nelze vypocitat gaussovu eliminacni metodou." << endl;
    }

    if (triangle != 0) {
        delete[] triangle[0];
        for (iterator = 1; iterator < array_rows; ++iterator) {
            delete[] triangle[iterator];
        }
        delete[] triangle;
    }

    delete[] array[0];
    for (iterator = 1; iterator < array_rows; ++iterator) {
        delete[] array[iterator];
    }
    delete[] array;

    return 0;
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


double **gauss_elimination(double **array, unsigned int cols, unsigned int rows) {
    if (rows != cols - 1) {
        cout << "Nelze vypocitat pomoci gaussovy eliminacni metody" << endl;
        return 0;
    }

    int i, j, k;
    double c, sum, *values, **result;
    values = new double[rows];
    result = new double *[rows]; // vytvori pole ukazatelu

    // upravi pole ze vlevo dole jsou nuly
    for (j = 0; j < rows; j++) // prochazi cele pole po radcich
    {
        for (i = j + 1; i < rows; i++) { // prochazi vsechny radky pod radkem j
            c = array[i][j] / array[j][j]; // konstanta, kterou se musi vynasobit hodnoty radku, slo vyrobit nulu
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
        result[i] = new double[cols]; // vytvoru radek pole
        for (j = 0; j < cols - 1; ++j) {
            if (i == j) {
                result[i][j] = 1;
            } else {
                result[i][j] = 0;
            }
        }
        result[i][cols - 1] = values[i];
    }

    delete[] values;

    return result;
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
            "<h2>Zadani</h2>\n"
            "<table>\n";

    for(int row = 0; row < rows; row++) {
        file << "<tr>\n";
        for (int col = 0; col < cols; ++col) {
            file << "<td>" << array[row][col] << "</td>\n";
        }
        file << "</tr>\n";
    }

    file << "</table>\n"
            "<h2>Vysledky</h2>\n";

    for (int i = 0; i < rows; ++i) {
        file << "<h3>x<sub>" << i+1 << "</sub> = " << array[i][cols-1] << "</h3>\n";
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