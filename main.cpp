#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

const int N = 4000;

struct Record {
    char name[30];
    short int department;
    char job_title[22];
    char year[10];
};

struct tree {
    Record* data;
    int weight;
    bool use;
    tree* left;
    tree* right;
};

string prompt(const string& str)
{
    cout << str;
    cout << "\n> ";
    string ans;
    cin >> ans;
    return ans;
}

int strcomp(const string& str1, const string& str2, int len = -1)
{
    if (len == -1) {
        len = (int)str1.length();
    }
    for (int i = 0; i < len; ++i) {
        if (str1[i] == '\0' and str2[i] == '\0') {
            return 0;
        } else if (str1[i] == ' ' and str2[i] != ' ') {
            return -1;
        } else if (str1[i] != ' ' and str2[i] == ' ') {
            return 1;
        } else if (str1[i] < str2[i]) {
            return -1;
        } else if (str1[i] > str2[i]) {
            return 1;
        }
    }
    return 0;
}

bool compareByDay(const Record* a, const Record* b)
{
    char *A, *B;
    A = (char*)malloc(2 * sizeof(char));
    B = (char*)malloc(2 * sizeof(char));
    A[0] = a->year[0];
    A[1] = a->year[1];
    B[0] = b->year[0];
    B[1] = b->year[1];
    int day_a = atoi(A);
    int day_b = atoi(B);
    delete A;
    delete B;
    return ((day_a > day_b)
            || ((day_a == day_b) && (strcmp(a->name, b->name) > 0)));
}

// Функция для построения пирамиды
void heapify(
        Record* arr[],
        int n,
        int i,
        bool (*compare)(const Record*, const Record*))
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && compare(arr[left], arr[largest]))
        largest = left;

    if (right < n && compare(arr[right], arr[largest]))
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, compare);
    }
}

// Функция сортировки пирамидой
void heapSort(
        Record* arr[], int n, bool (*compare)(const Record*, const Record*))
{
    // Построение пирамиды (первый проход)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, compare);

    // Извлечение элементов из пирамиды
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, compare);
    }
}

void mainSort(Record** masPointers, int numEmployees)
{
    heapSort(masPointers, numEmployees, compareByDay);
}

void print_head()
{
    cout << "Record           Name             D. nom     Job title            "
            "Date\n";
}

void print_record(Record* record, int i)
{
    cout << setw(3) << i << " " << record->name << "  " << setw(4)
         << record->department << "  " << record->job_title << "  "
         << record->year << "\n";
}

void show_list(Record* records[], int n = N)
{
    int ind = 0;
    while (true) {
        //system("cls");
        print_head();
        for (int i = 0; i < 20; i++) {
            Record* record = records[ind + i];
            print_record(record, ind + i + 1);
        }
        cout << ind / 20 + 1 << endl;
        string chose = prompt(
                "n: Next page\t"
                "p: Previous page\t"
                "d: Skip 10 next pages\n"
                "a: Skip 10 prev pages\n"
                "g: Select page\n"
                "Any key: Exit");
        switch (chose[0]) {
        case 'n':
            ind += 20;
            break;
        case 'p':
            ind -= 20;
            break;
        case 'd':
            ind += 200;
            break;
        case 'a':
            ind -= 200;
            break;
        case 'g':
            cout << endl << "enter page nom.: ";
            int h;
            cin >> h;
            h--;
            ind = h * 20;
            break;
        default:
            return;
        }
        if (ind < 0) {
            ind = 0;
        } else if (ind > n - 20) {
            ind = n - 20;
        }
    }
}

int quick_search(Record* arr[], const std::string& key)
{
    int l = 0;
    int r = N - 1;
    while (l < r) {
        int m = (l + r) / 2;
        if (strcomp(&arr[m]->year[0], key, 2) < 0) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    if (strcomp(&arr[r]->year[0], key, 2) == 0) {
        return r;
    }
    return -1;
}

void search(Record* arr[], int& ind, int& n)
{
    string key;
    do {
        key = prompt("Input search key (day)");
    } while (key.length() != 2);
    ind = quick_search(arr, key);
    if (ind == -1) {
        cout << "Not found\n";
    } else {
        int i;
        int j = 0;
        for (i = ind + 1; strcomp(&arr[i]->year[0], key, 2) == 0; i++) {
        }
        n = i - ind;
        show_list(&arr[ind], n);
    }
}

void Print_tree(tree* p, int& i)
{
    if (p) {
        Print_tree(p->left, i);
        print_record(p->data, i++);
        Print_tree(p->right, i);
    }
}

void SDP(tree*& root, tree D)
{
    if (root == nullptr) {
        root = new tree;
        root->data = D.data;
        root->left = nullptr;
        root->right = nullptr;
        root->weight = D.weight;
    } else if (D.data->department < root->data->department) {
        SDP(root->left, D);
    } else if (D.data->department > root->data->department) { //=
        SDP(root->right, D);
    }
}

void createA1(tree*& root, tree* V, int len)
{
    for (int i = 0; i < len; i++) {
        V[i].use = false;
    }

    for (int i = 0; i < len; i++) {
        int max = 0, index = 0;
        for (int j = 0; j < len; j++) {
            if (V[j].weight > max && V[j].use == false) {
                max = V[j].weight;
                index = j;
            }
        }
        V[index].use = true;
        SDP(root, V[index]);
    }
}

void search_in_tree(tree* root, int key, int& i)
{
    if (root) {
        if (key < root->data->department) {
            search_in_tree(root->left, key, i);
        } else if (key > root->data->department) {
            search_in_tree(root->right, key, i);
        } else if (key == root->data->department) {
            search_in_tree(root->left, key, i);
            print_record(root->data, i++);
            search_in_tree(root->right, key, i);
        }
    }
}

void rmtree(tree* root)
{
    if (root) {
        rmtree(root->right);
        rmtree(root->left);
        delete root;
    }
}

void Tree(Record* arr[], int n)
{
    tree* root = NULL;
    tree* tempArr = new tree[n];
    for (int i = 0; i < n; ++i) {
        tempArr[i].data = arr[i];
        tempArr[i].weight = rand() % 1000;
        tempArr[i].use = false;
        tempArr[i].left = NULL;
        tempArr[i].right = NULL;
    }
    createA1(root, tempArr, n);
    print_head();
    int i = 1;
    Print_tree(root, i);
    int key;
    do {
        key = stoi(prompt("Input search key (home), 1000 - exit"));
        print_head();
        i = 1;
        search_in_tree(root, key, i);
    } while (key != 1000);
    rmtree(root);
}

int up(int n, double q, double* array, double chance[])
{ // находит в array место, куда вставить число q и вставляет его
    int i = 0, j = 0; // сдвигая вниз остальные элементы
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

void down(int n, int j, int Length[], char c[][20])
{ // формирует кодовое слово
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

void haffman(int n, double* array, int Length[], char c[][20], double chance[])
{
    if (n == 2) {
        c[0][0] = '0';
        Length[0] = 1;
        c[1][0] = '1';
        Length[1] = 1;
    } else {
        double q = array[n - 2] + array[n - 1];
        int j = up(n, q, array, chance); // поиск и вставка суммы
        haffman(n - 1, array, Length, c, chance);
        down(n, j, Length, c); // достраиваем код
    }
}

unordered_map<char, int>
get_char_counts_from_file(const string& file_name, int& file_size)
{
    ifstream file(file_name);
    if (!file.is_open()) {
        throw runtime_error("Could not open file");
    }
    unordered_map<char, int> counter_map;
    file_size = 0;
    for (char c = (char)file.get(); c != EOF; c = (char)file.get()) {
        if (c != '\n' && c != '\r') {
            counter_map[c]++;
            file_size++;
        }
    }
    file.close();
    return counter_map;
}

vector<pair<double, char>>
calc_probabilities(const unordered_map<char, int>& counter_map, int count)
{
    vector<pair<double, char>> probabilities;
    probabilities.reserve(counter_map.size());
    for (auto i : counter_map) {
        probabilities.emplace_back(
                make_pair((double)i.second / count, i.first));
    }
    return probabilities;
}

int haffman()
{
    int file_size;
    unordered_map<char, int> counter_map;
    try {
        counter_map = get_char_counts_from_file("testBase2.dat", file_size);
    } catch (runtime_error& exc) {
        cout << exc.what();
        return 1;
    }
    auto probabilities = calc_probabilities(counter_map, file_size);
    counter_map.clear();

    sort(probabilities.begin(),
         probabilities.end(),
         greater<pair<double, char>>());

    double ent = 0;

    for (auto i : probabilities) {
        ent += i.first * log2(i.first);
    }

    ent *= -1;

    const int n = (int)probabilities.size();

    auto c = new char[n][20];
    auto Length = new int[n];
    auto p = new double[n];
    for (int i = 0; i < n; ++i) {
        p[i] = probabilities[i].first;
    }

    double shen = 0;

    double temp = 0;

    double chance_l[n];
    for (int i = 0; i < n; ++i) {
        Length[i] = 0;
        chance_l[i] = p[i];
    }

    cout << "\nHaffman Code:\n";
    haffman(n, chance_l, Length, c, p);
    cout << "\n  Length Code\n";
    for (int i = 0; i < n; i++) {
        cout << i << " ";
        printf("%c | %4.6lf %d ", probabilities[i].second, p[i], Length[i]);
        for (int j = 0; j < Length[i]; ++j) {
            printf("%c", c[i][j]);
        }
        cout << '\n';
        temp += p[i] * Length[i];
    }

    double hafAver = temp;
    cout << "Entropy = " << ent << endl;
    cout << "Average word length = " << hafAver << endl;
    cout << "Redundancy = " << hafAver - ent << endl;
    delete[] p;
}

void mainloop(Record* unsorted_ind_array[], Record* sorted_ind_array[])
{
    int search_ind, search_n = -1;
    //system("cls");
    while (true) {
        std::string chose = prompt(
                "1: Show unsorted list\n"
                "2: Show sorted list\n"
                "3: Search\n"
                "4: Tree \n"
                "5: Coding\n"
                "Any key: Exit");
        switch (chose[0]) {
        case '1':
            show_list(unsorted_ind_array);
            break;
        case '2':
            show_list(sorted_ind_array);
            break;
        case '3':
            search(sorted_ind_array, search_ind, search_n);
            break;
        case '4':
            if (search_n == -1) {
                cout << "Please search first\n";
            } else {
                Tree(&sorted_ind_array[search_ind], search_n);
            }
            break;
        case '5':
            haffman();
            break;
        default:
            return;
        }
    }
}

int main()
{
    FILE* fp;
    fp = fopen("testBase2.dat", "rb");
    Record mas[4000] = {0};
    int i = fread((Record*)mas, sizeof(Record), 4000, fp);

    Record* unsortmasPointers[N];
    for (int i = 0; i < N; i++) {
        unsortmasPointers[i] = &mas[i];
    }

    Record* sortmasPointers[N];
    for (int i = 0; i < N; i++) {
        sortmasPointers[i] = &mas[i];
    }

    mainSort(sortmasPointers, N);

    mainloop(unsortmasPointers, sortmasPointers);
}