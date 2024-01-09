#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <string.h>
#include <locale.h>

using namespace std;

const int N = 4000;

struct Record
{
    char fio[32];
    char street[18];
    short int apartment_number;
    short int house_number;
    char settlement_date[10];
};

struct Node
{
    Record record;
    Node* next;
};

string prompt(const string& str)
{
    cout << str;
    cout << "\n> ";
    string ans;
    cin >> ans;
    return ans;
}

Node* load_to_memory()
{
    ifstream file("testBase4.dat", ios::binary);
    if (!file.is_open())
    {
        return NULL;
    }

    Node* root = NULL;

    for (int i = 0; i < N; ++i)
    {
        Record record;
        file.read((char*)&record, sizeof(Record));
        root = new Node{ record, root };
    }

    file.close();
    return root;
}

void make_index_array(Record* arr[], Node* root, int n = N)
{
    Node* p = root;
    for (int i = 0; i < n; i++)
    {
        arr[i] = &(p->record);
        p = p->next;
    }
}

void digitalSort(Node*& head, const int KDI[])
{
    int L = KDI[0] + KDI[1];
    Node* p;
    struct Queue
    {
        Node* tail;
        Node* head;
    } q[256];

    for (int j = 0; j < L; j++)
    {
        for (int i = 0; i < 256; i++)
        {
            q[i].tail = NULL;
            q[i].head = NULL;
        }

        p = head;
        while (p)
        {
            int d{};
            if (j < KDI[1])
            {
                d = 255 - abs(p->record.street[L - j - 1 - KDI[0]]);
            }
            else if (j < KDI[1] + KDI[0])
            {
                d = (unsigned char)p->record.settlement_date[L - j - 1];
            }

            Node* nextNode = p->next;
            p->next = NULL;

            if (q[d].head == NULL)
            {
                q[d].head = p;
                q[d].tail = p;
            }
            else
            {
                q[d].tail->next = p;
                q[d].tail = p;
            }

            p = nextNode;
        }

        int i = 0;
        while (q[i].head == NULL)
        {
            i++;
        }

        head = q[i].head;
        Node* tail = q[i].tail;

        for (int k = i + 1; k < 256; k++)
        {
            if (q[k].head != NULL)
            {
                tail->next = q[k].head;
                tail = q[k].tail;
            }
        }
    }
}

void print_head()
{
    cout << "Record  FIO                                      Street       apartment_number     house_number      Settlement date\n";
}

void print_record(Record* record, int i)
{
    cout << "[" << setw(4) << i << "]"
        << "  " << record->fio << setw(10)
        << "  " << record->street << setw(10)
        << "  " << setw(5) << record->apartment_number << setw(10)
        << "  " << setw(5) << record->house_number << setw(10)
        << "  " << setw(8) << record->settlement_date << "\n";
}

void show_list(Record* ind_arr[], int n = N)
{
    int ind = 0;
    while (true)
    {
        system("CLS");
        print_head();
        for (int i = 0; i < 20; i++)
        {
            Record* record = ind_arr[ind + i];
            print_record(record, ind + i + 1);
        }
        string chose = prompt("w: Next page\t"
            "q: Last page\t"
            "e: Skip 10 next pages\n"
            "s: Prev page\t"
            "a: First page\t"
            "d: Skip 10 prev pages\n"
            "Any key: Exit");

        switch (chose[0])
        {
        case 'w':
            ind += 20;
            break;
        case 's':
            ind -= 20;
            break;
        case 'a':
            ind = 0;
            break;
        case 'q':
            ind = n - 20;
            break;
        case 'd':
            ind -= 200;
            break;
        case 'e':
            ind += 200;
            break;
        default:
            return;
        }

        if (ind < 0)
        {
            ind = 0;
        }
        else if (ind > n - 20)
        {
            ind = n - 20;
        }
    }
}

int quick_search(Record* arr[], char* key)
{
    int l = 0;
    int r = N - 1;
    while (l < r)
    {
        int m = (l + r) / 2;
        if (strncmp(arr[m]->settlement_date, key, 2) < 0)
        {
            l = m + 1;
        }
        else
        {
            r = m;
        }
    }
    if (strncmp(arr[r]->settlement_date, key, 2) == 0)
    {
        return r;
    }
}

void search(Record* arr[], int& ind, int& n)
{
    Node* head = nullptr, * tail = nullptr;
    char key[3];
    cout << "Input search key (first two characters of birth date): ";
    cin >> key;
    ind = quick_search(arr, key);
    if (ind == -1)
    {
        cout << "Not found" << endl;
    }
    else
    {
        head = new Node{ *arr[ind], nullptr };
        tail = head;
        int i;
        for (i = ind + 1; i < 4000 && strncmp(arr[i]->settlement_date, key, 2) == 0; ++i)
        {
            tail->next = new Node{ *arr[i], nullptr };
            tail = tail->next;
        }
        n = i - ind;
        auto find_arr = new Record * [n];
        make_index_array(find_arr, head, n);
        show_list(find_arr, n);
        delete[] find_arr;
    }
}

int strcomp(const string& str1, const string& str2, int len = -1)
{
    if (len == -1)
    {
        len = (int)str1.length();
    }
    for (int i = 0; i < len; ++i)
    {
        if (str1[i] == '\0' && str2[i] == '\0')
        {
            return 0;
        }
        else if (str1[i] == ' ' && str2[i] != ' ')
        {
            return -1;
        }
        else if (str1[i] != ' ' && str2[i] == ' ')
        {
            return 1;
        }
        else if (str1[i] < str2[i])
        {
            return -1;
        }
        else if (str1[i] > str2[i])
        {
            return 1;
        }
    }
    return 0;
}

struct Vertex
{
    Record* data;
    Vertex* left;
    Vertex* right;
    int balance;

    Vertex(Record* data, Vertex* left = nullptr, Vertex* right = nullptr, int balance = 0) : data(data), left(left), right(right), balance(balance) {}
};

bool grow;

void ll(Vertex*& p)
{
    Vertex* q = p->left;
    p->balance = q->balance = 0;
    p->left = q->right;
    q->right = p;
    p = q;
}

void rr(Vertex*& p)
{
    Vertex* q = p->right;
    p->balance = q->balance = 0;
    p->right = q->left;
    q->left = p;
    p = q;
}

void lr(Vertex*& p)
{
    Vertex* q = p->left;
    Vertex* r = q->right;
    if (r->balance < 0)
    {
        p->balance = 1;
    }
    else
    {
        p->balance = 0;
    }
    if (r->balance > 0)
    {
        q->balance = -1;
    }
    else
    {
        q->balance = 0;
    }
    r->balance = 0;
    q->right = r->left;
    p->left = r->right;
    r->left = q;
    r->right = p;
    p = r;
}

void rl(Vertex*& p)
{
    Vertex* q = p->right;
    Vertex* r = q->left;
    if (r->balance > 0)
    {
        p->balance = -1;
    }
    else
    {
        p->balance = 0;
    }
    if (r->balance < 0)
    {
        q->balance = 1;
    }
    else
    {
        q->balance = 0;
    }
    r->balance = 0;
    q->left = r->right;
    p->right = r->left;
    r->right = q;
    r->left = p;
    p = r;
}

void add_to_avl(Vertex*& p, Record* data)
{
    if (!p)
    {
        p = new Vertex(data);
        grow = true;
    }
    else if (strcomp(p->data->settlement_date, data->settlement_date) == 1)
    {
        add_to_avl(p->left, data);
        if (grow)
        {
            if (p->balance > 0)
            {
                p->balance = 0;
                grow = false;
            }
            else if (p->balance == 0)
            {
                p->balance = -1;
                grow = true;
            }
            else
            {
                if (p->left->balance < 0)
                {
                    ll(p);
                    grow = false;
                }
                else
                {
                    lr(p);
                    grow = false;
                }
            }
        }
    }
    else if (strcomp(p->data->settlement_date, data->settlement_date) <= 0)
    {
        add_to_avl(p->right, data);
        if (grow)
        {
            if (p->balance < 0)
            {
                p->balance = 0;
                grow = false;
            }
            else if (p->balance == 0)
            {
                p->balance = 1;
                grow = true;
            }
            else
            {
                if (p->right->balance > 0)
                {
                    rr(p);
                    grow = false;
                }
                else
                {
                    rl(p);
                    grow = false;
                }
            }
        }
    }
    else
    {
        std::cout << "Data already exists";
    }
}

void Print_tree(Vertex* p, int& i)
{
    if (p)
    {
        Print_tree(p->left, i);
        print_record(p->data, i++);
        Print_tree(p->right, i);
    }
}

void search_in_tree(Vertex* root, const string& key, int& i)
{
    if (root)
    {
        if (strcomp(key, root->data->settlement_date) < 0)
        {
            search_in_tree(root->left, key, i);
        }
        else if (strcomp(key, root->data->settlement_date) > 0)
        {
            search_in_tree(root->right, key, i);
        }
        else if (strcomp(key, root->data->settlement_date) == 0)
        {
            search_in_tree(root->left, key, i);
            print_record(root->data, i++);
            search_in_tree(root->right, key, i);
        }
    }
}

void rmtree(Vertex* root)
{
    if (root)
    {
        rmtree(root->right);
        rmtree(root->left);
        delete root;
    }
}

void tree(Record* arr[], int n)
{
    Vertex* root = nullptr;

    for (int i = 0; i < n; ++i)
    {
        add_to_avl(root, arr[i]);
    }

    print_head();

    int i = 1;
    Print_tree(root, i);

    string key;
    do
    {
        getline(cin, key);
        if (!key.empty() && key != "q")
        {
            print_head();
            i = 1;
            search_in_tree(root, key, i);
        }
        cout << "Input search key (full birth date), q - exit\n> ";
    } while (key[0] != 'q');

    rmtree(root);
}


void Gilbert_Moore(const int n, double p[], int Length[], char c[][20]) {
    double* q = new double[n];
    Length[0] = ceil(log2(1 / p[0]));
    q[0] = p[0] / 2;

    for (int i = 1; i < n; ++i) {
        double tmp = 0;
        for (int k = 0; k < i; k++)
            tmp += p[k];

        q[i] = tmp + p[i] / 2;
        Length[i] = ceil(log2(1 / p[i]));  // ? ??????? ?—???‚? ?°? ?†? ?»? ?µ? ?…? ?•
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

unordered_map<char, int> get_char_counts_from_file(const string& file_name, int& file_size, int n = N)
{
    ifstream file(file_name, ios::binary);
    if (!file.is_open())
    {
        throw runtime_error("Could not open file");
    }
    Record record_arr[N];
    file.read((char*)record_arr, sizeof(Record) * n);
    file.close();

    unordered_map<char, int> counter_map;
    file_size = 0;

    for (int i = 0; i < n; ++i)
    {

        for (int j = 0; j < strlen(record_arr[i].fio); ++j)
        {
            counter_map[record_arr[i].fio[j]]++;
            file_size++;
        }
    }

    return counter_map;
}


vector<pair<double, char>> calc_probabilities(const unordered_map<char, int>& counter_map, int count)
{
    vector<pair<double, char>> probabilities;
    probabilities.reserve(counter_map.size());
    for (auto i : counter_map)
    {
        probabilities.emplace_back(make_pair((double)i.second / count, i.first));
    }
    return probabilities;
}

void coding()
{
    int file_size;
    unordered_map<char, int> counter_map;
    counter_map = get_char_counts_from_file("testBase4.dat", file_size);

    auto probabilities = calc_probabilities(counter_map, file_size);
    counter_map.clear();

    sort(probabilities.begin(), probabilities.end(), greater<>());
    cout << "Probabil.  char\n";
    for (auto i : probabilities)
    {
        cout << fixed << i.first << " | " << i.second << '\n';
    }

    const int n = (int)probabilities.size();

    auto c = new char[n][20];
    auto Length = new int[n];
    auto p = new double[n];
    for (int i = 0; i < n; ++i)
    {
        p[i] = probabilities[i].first;
    }

    double shen = 0;
    Gilbert_Moore(n, p, Length, c);
    cout << "\nMur Code:\n";
    cout << "\nCh  Prob      Code\n";
    double avg_len = 0;
    double entropy = 0;
    for (int i = 0; i < n; i++)
    {
        avg_len += Length[i] * p[i];
        entropy -= p[i] * log2(p[i]);
        printf("%c | %.5lf | ", probabilities[i].second, p[i]);
        for (int j = 0; j < Length[i]; ++j)
        {
            printf("%c", c[i][j]);
        }
        cout << '\n';
        shen += p[i];
    }
    cout << "Average length = " << avg_len << '\n'
        << "Entropy = " << entropy << '\n'
        << "Average length < entropy + 2\n"
        << "N = " << n << endl;
}

void mainloop(Record* unsorted_ind_array[], Record* sorted_ind_array[])
{
    int search_ind, search_n = -1;
    while (true)
    {
        string chose = prompt("1: Show unsorted list\n"
            "2: Show sorted list\n"
            "3: Search\n"
            "4: Tree\n"
            "5: Coding\n"
            "Any key: Exit");
        switch (chose[0])
        {
        case '1':
            system("CLS");
            show_list(unsorted_ind_array);
            break;
        case '2':
            system("CLS");
            show_list(sorted_ind_array);
            break;
        case '3':
            search(sorted_ind_array, search_ind, search_n);
            break;
        case '4':
            if (search_n == -1)
            {
                cout << "Please search anything before making tree\n";
            }
            else
            {
                tree(&sorted_ind_array[search_ind], search_n);
            }
            break;
        case '5':
            system("CLS");
            coding();
            break;
        default:
            return;
        }
    }
}

int main()
{
    Node* head = load_to_memory();
    int KDI[] = { 2, 30 };
    if (!head)
    {
        cout << "File not found" << endl;
        return 1;
    }
    auto unsorted_ind_arr = new Record * [N];
    auto sorted_ind_arr = new Record * [N];
    make_index_array(unsorted_ind_arr, head);
    digitalSort(head, KDI);
    make_index_array(sorted_ind_arr, head);
    mainloop(unsorted_ind_arr, sorted_ind_arr);
}
