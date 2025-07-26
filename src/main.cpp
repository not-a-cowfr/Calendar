#include <iostream>
#include <iomanip>
#include <ctime>
#include "./calendar.cpp"

using namespace std;

int main()
{
    Calendar::Mode greg = Calendar::Mode::Auto;

    vector<Event> events;
    Events::loadFromFile(events, "events.txt");

    time_t now = time(NULL);
    struct tm timestamp = *localtime(&now);
    
    int year = timestamp.tm_year + 1900, month = timestamp.tm_mon + 1, day = 1;

    string color, name, description, temp;
    int temp1, temp2;
    int choice;

    Calendar::displayMonth(year, month, events, greg);
    // TODO Make good ui
    while (choice != 9)
    {
        cout << "\nTemporary UI:\n"
             << " 1) Next\n"
             << " 2) Previous\n"
             << " 3) Go To\n"
             << " 4) Add event\n"
             << " 5) Change Calendar Mode\n"
             << " 6) Test\n"
             << " 9) Exit\n\n"
             << "Enter your choice: " << flush;

        cin >> choice;

        switch (choice)
        {
        case 1:
            month++;
            if (month > 12)
            {
                month = 1;
                year++;
            }
            break;
        case 2:
            month--;
            if (month < 1)
            {
                month = 12;
                year--;
            }
            break;
        case 3:
            cout << "\nEnter y m: ";
            cin >> year >> month;
            break;
        case 4:
            cout << "What day? ";
            cin >> day;
            cout << "Enter color (ansi): ";
            cin >> color;
            cout << "Enter name: ";
            cin >> name;
            description = "";
            Events::createEvent(events, month, day, 0, color, name, description);
            break;
        case 5:
            cout << "Gregorian? (g/j): ";
            cin >> temp;
            if (temp[0] == 'g')
            {
                greg = Calendar::Mode::Gregorian;
            }
            else
                greg = Calendar::Mode::Julian;
            break;
        case 6:
            cout << "Enter day num and len: ";
            cin >> temp1 >> temp2;
            Calendar::dayName(temp1, temp2);
            break;
        case 7:
            cout << "Events:\n";
            for (auto event : events)
            {
                cout << event.name << "\n";
            }
            break;

        default:
            continue;
            break;
        }

        Calendar::displayMonth(year, month, events, greg);
    }

    return 0;
}
