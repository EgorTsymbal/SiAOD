#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

// Var - 48
// Base - 4
// C - 3: по дате поселения и названию улицы, К = год поселения.
// S - DigitalSort
// D - 1
// E - 4


using namespace std;

struct Village
{
    char villager[32];
    char street[18];
    short int apartment_number;
    short int house_number;
    char Settlement_date[10];
};





int ExtractYear(const char* date)
{
    int year = 0;
    if (strlen(date) >= 8)
    {
        sscanf(date + 6, "%d", &year);
        if (year >= 0 && year <= 99)
        {
            if (year >= 0 && year <= 21)
            {
                year += 2000;
            }
            else
            {
                year += 1900;
            }
        }
    }
    return year;
}

bool CompareVillage(const Village& a, const Village& b)
{
    int year_a = ExtractYear(a.Settlement_date);
    int year_b = ExtractYear(b.Settlement_date);

    if (year_a != year_b)
    {
        return year_a < year_b;
    }

    return strcmp(a.street, b.street) < 0;
}

vector<Village> FilterByYear(const vector<Village>& villages, int year)
{
    vector<Village> result;

    for (int i = 0; i < villages.size(); i++)
    {
        const Village& village = villages[i];
        if (ExtractYear(village.Settlement_date) == year)
        {
            result.push_back(village);
        }
    }
    return result;
}



int main()
{
    ifstream inputFile("testBase4.dat", ios::binary);
    if (!inputFile)
    {
        cerr << "Ошибка открытия файла" << endl;
        return 1;
    }

    vector<Village> villages;
    Village village;

    while (inputFile.read((char*)&village, sizeof(Village)))
    {
        villages.push_back(village);
    }

    inputFile.close();

    

    int page = 1;
    char ch;

    while (true)
    {
        cout << "Page: " << page << endl;
        int count = (page - 1) * 20 + 1;
        for (int i = (page - 1) * 20; i < 20 * page && i < villages.size(); i++)
        {
            cout << count << ". " << "\t" << villages[i].villager << "      " << villages[i].street << "  " << villages[i].apartment_number << "  " << villages[i].house_number << "\t" << villages[i].Settlement_date << endl;
            count++;
        }
        cout << "\n\r" << "= Home " << "\t" << "- Back up" << "  " << "+ Forward" << "  " << "o Another page" << "  " << "/ Exit" << "  " << "k Search by year" << endl;
        cin >> ch;

        switch (ch)
        {
        case '=':
            page = 1;
            break;

        case '-':
            if (page > 1) page--;
            break;

        case '+':
            if (page < villages.size() / 20) page++;
            break;

        case 'o':
            cout << "Enter the page number: ";
            cin >> page;
            if (page < 1) page = 1;
            if (page > villages.size() / 20) page = villages.size() / 20;
            break;

        case '/':
            cout << "Exit the program? (Enter \"/\" to exit): ";
            cin >> ch;
            if (ch == '/') return 0;
            break;

        case 'k':
        {
            int searchYear;
            cout << "Enter the year to search for (for example: 1995): ";
            cin >> searchYear;

            vector<Village> filteredVillages = FilterByYear(villages, searchYear);

            if (filteredVillages.empty())
            {
                cout << "No settlements found in the year " << searchYear << endl;
            }
            else
            {
                cout << "Settlements carried out per year " << searchYear << ":" << endl;
                for (int i = 0; i < filteredVillages.size(); i++)
                {
                    const Village& village = filteredVillages[i];
                    cout << "\t" << village.villager << "      " << village.street << "  " << village.apartment_number << "  " << village.house_number << "\t" << village.Settlement_date << endl;
                }
            }
            cout << "Press Enter to return to the home screen...";
            cin.ignore();
            cin.get();
            break;
        }
        }

        system("cls");
    }

    return 0;
}

