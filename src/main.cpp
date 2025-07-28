#include <algorithm>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "./calendar.cpp"

using namespace std;

bool validateIntInput(int &input, int min, int max, bool acceptEnter);
bool isValidColor(string color);
void showColors();
bool displayEvents(std::vector<Event> &eventsVector, Calendar::Mode mode, int month);
void editEvent(Event &event, Calendar::Mode mode);
void helpMenu();
void handleCommands(string command);

int main()
{
    Calendar::Mode calendarMode = Calendar::Mode::Gregorian;

    vector<Event> events, monthEvents;
    Events::loadFromFile(events, "events.txt");
    sort(events.begin(), events.end(), Events::compareEvents);

    const time_t now = time(NULL);
    const struct tm timestamp = *localtime(&now);

    int curYear = timestamp.tm_year + 1900, curMonth = timestamp.tm_mon + 1,
        year = curYear, month = curMonth,
        day;

    string color, name, description, tempStr;

    char key, keyCode = -1, queue = -1;
    bool exit = false, keyWasQueued = false;

    // Display the month without clearing in case of errors or similar
    Calendar::displayMonth(year, month, events, calendarMode);
    cout << "Press ? for help.\n";
    queue = _getch();

    while (!exit)
    {
        cout << "\e[H\e[0J\n"; // ANSI codes to go top left and erase display
        Calendar::displayMonth(year, month, events, calendarMode);
        cout << flush;

        if (queue == -1) // Queue is for some actions that print "status messages" (such as the help menu)
        {
            key = _getch();
            keyWasQueued = false;
        }
        else
        {
            key = queue;
            queue = -1;
            keyWasQueued = true;
        }

        if (key == 0 || key == 224 || key == -32) //  for special keys like arrow keys
            keyCode = _getch();
        else
            keyCode = -1;

        if (key == 'q')
        {
            cout << ":q";
            key = _getch();
            exit = (key == 13 || key == 'q'); // 13 is Enter
        }
        else if (key == 3) // ^C
        {
            exit = true;
            break;
        }
        else if (key == 'j' || keyCode == 80) // Down arrow
        {
            month++;
            if (month > 12)
            {
                month = 1;
                year++;
            }
        }
        else if (key == 'k' || keyCode == 72) // Up arrow
        {
            month--;
            if (month < 1)
            {
                month = 12;
                year--;
            }
        }
        else if (key == 'h' || keyCode == 75) // Left arrow
        {
            year--;
        }
        else if (key == 'l' || keyCode == 77) // Right arrow
        {
            year++;
        }
        else if (key == 'g')
        {
            cout << "g: ";
            getline(cin, tempStr);

            try
            {
                year = stoi(tempStr);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Invalid year" << '\n';
            }
        }
        else if (key == 'G')
        {
            cout << "G: ";
            getline(cin, tempStr);

            try
            {
                month = (stoi(tempStr) + 11) % 12 + 1; // Keep month from 1-12
            }
            catch (const std::exception &e)
            {
                std::cerr << "Invalid month" << '\n';
            }
        }
        else if (key == 'n')
        {
            cout << "Enter the day (leave blank to cancel): ";

            if (!validateIntInput(day, 0, Calendar::daysInMonth(year, month, calendarMode), true))
                continue;

            cout << "Enter the name: ";
            getline(cin, name);

            cout << "\n(Optional) ";
            showColors();
            while (getline(cin, color))
            {
                if (isValidColor(color))
                    break;

                cout << "Invalid: ";
            }

            cout << "(Optional) Enter description: ";
            getline(cin, description);

            Events::createEvent(events, 0, month, day, 0, 1, color, name, description);
        }
        else if (key == 'e')
        {
            if (displayEvents(events, calendarMode, month))
                queue = _getch();
        }
        else if (key == 'E')
        {
            if (displayEvents(events, calendarMode, -1))
                queue = _getch();
        }
        else if (key == 'm')
        {
            if (calendarMode == Calendar::Mode::Gregorian)
                calendarMode = Calendar::Mode::Julian;
            else
                calendarMode = Calendar::Mode::Gregorian;
        }
        else if (key == ':')
        {
            cout << ":";
            getline(cin, tempStr);
            if (tempStr == "q")
                exit = true;
            else
            {
                handleCommands(tempStr);
                cout << "\n";
                queue = _getch();
            }
        }
        else if (key == '?' || key == '/')
        {
            helpMenu();
            cout << "\n";
            queue = _getch();
        }
        else if (!keyWasQueued || !(key == 13 || key == 27 || key == ' ')) // 13 is Enter. 27 is Esc
        {
            cout << "Press ? for help.\n";
            queue = _getch();
        }
    }

    return 0;
}

/// @brief Validates an integer input
/// @param input The integer variable for the input
/// @param min The minimum value of the integer
/// @param max The maximum value of the integer
/// @param acceptEnter Whether or not to accept enter (will return false if enter is accepted)
/// @return true if an integer is accepted. false will only be returned with `acceptEnter = true`
bool validateIntInput(int &input, int min, int max, bool acceptEnter)
{
    string inputStr;
    while (true)
    {
        getline(cin, inputStr);
        try
        {
            input = stoi(inputStr);
            if (input >= min && input <= max)
                return true;

            cout << "Must be between " << min << " and " << max << ": ";
        }
        catch (const exception &e)
        {
            if (inputStr == "" && acceptEnter) // Getline doesn't include the \n, so Enter is just ""
                return false;

            cout << "You must enter an integer: ";
            cin.clear();
        }
    }
}

/// @brief Determines if a string is a valid ANSI color code attribute
/// @param color ANSI color code attribute to test (just the `n` in `\\e]nm`)
/// @return return Whether the color is valid (true/false)
bool isValidColor(string color)
{
    for (auto ch : color)
    {
        if ((ch < '0' || ch > '9') && ch != ';')
            return false;
    }

    return true;
}

/// @brief Shows the foreground and background ANSI code colors
void showColors()
{
    cout << "Enter a foreground and/or background color seperated by ';'\n";

    for (int i = 30; i <= 37; i++)
        cout << "\e[" << i << "m" << i << "\e[0m ";
    cout << " ";
    for (int i = 90; i <= 97; i++)
        cout << "\e[" << i << "m" << setw(3) << i << "\e[0m ";
    cout << "\n";

    for (int i = 40; i <= 47; i++)
        cout << "\e[" << i << "m" << i << "\e[0m ";
    cout << " ";
    for (int i = 100; i <= 107; i++)
        cout << "\e[" << i << "m" << i << "\e[0m ";
    cout << "\n";
}

/// @brief Displays a list of events and allows editing/deleting them
/// @param eventsVector The vector of events to filter from
/// @param mode Whether to use the Gregorian or Julian calendar
/// @param month The month to filter by. -1 to show all
/// @return Whether a key should be queued (true/false)
bool displayEvents(std::vector<Event> &eventsVector, Calendar::Mode mode, int month)
{
    vector<int> filteredEvents;
    int i = 0, filteredI = 0, idInput, id, intInput;
    string stringInput;
    char key;

    for (auto event : eventsVector)
    {
        if (event.month == month || month == -1)
        {
            cout << setw(2) << filteredI << ": "
                 << setw(2) << Calendar::monthName(event.month, 3) << " "
                 << "\e[" << event.color << "m"
                 << setw(2) << event.day << "\e[0m " << event.name << "\n";
            filteredI++;
            filteredEvents.push_back(i); // Store the actual id of the filtered event
        }
        i++;
    }

    if (filteredI == 0)
    {
        cout << "No events";
        if (month != -1)
            cout << " this month";
        cout << ".\n";
        return true;
    }

    cout << "\nEnter the id of the event to view/edit (blank to go back): ";
    if (!validateIntInput(idInput, 0, filteredI - 1, true))
        return false;

    id = filteredEvents[idInput];

    cout << "\e[" << eventsVector[id].color << "m" << idInput << "\e[0m: "
         << Calendar::monthName(eventsVector[id].month) << " " << eventsVector[id].day;
    if (eventsVector[id].name != "")
        cout << "\nName: " << eventsVector[id].name;
    if (eventsVector[id].description != "")
        cout << "\nDescription: " << eventsVector[id].description;

    cout << "\n\n 1) \e[4mE\e[0mdit\n 2) \e[4mD\e[0melete\n 3) \e[4mB\e[0mack\n";
    key = _getch();

    if (tolower(key) != 'e' && tolower(key) != 'd' &&
        key != '1' && key != '2')
    {
        return false;
    }

    bool edit = tolower(key) == 'e' || key == '1';
    if (edit)
        editEvent(eventsVector[id], mode);
    else
        eventsVector.erase(eventsVector.begin() + id);

    Events::saveToFile(eventsVector, "events.txt");

    return false;
}

/// @brief Prompts the user to edit an event. Does not save to file
/// @param event The event to edit
/// @param mode Whether to use the Gregorian or Julian calendar
void editEvent(Event &event, Calendar::Mode mode)
{
    int intInput, daysInMonth;
    string stringInput;

    cout << "\nLeave blank to keep the current value\n\n";

    cout << "Enter the new month: ";
    if (validateIntInput(intInput, 0, 12, true))
    {
        event.month = intInput;
    }

    cout << "Enter the new day: ";
    daysInMonth = Calendar::daysInMonth(event.year, event.month, mode);
    if (validateIntInput(intInput, 1, daysInMonth, true))
    {
        event.day = intInput;
    }

    cout << "Enter the new name: ";
    getline(cin, stringInput);
    if (stringInput != "")
        event.name = stringInput;

    showColors();

    while (getline(cin, stringInput))
    {
        if (isValidColor(stringInput))
            break;

        cout << "Invalid: ";
    }
    if (stringInput != "")
        event.color = stringInput;

    cout << "Enter description: ";
    getline(cin, stringInput);
    if (stringInput != "")
        event.description = stringInput;
}

/// @brief Shows a help menu for the actions
void helpMenu() // TODO Define actions in an array of structs (key and aliases, desc, function?)
{
    cout << "q -> Quit with confirmation\n"
         //  << "^C -> Immediately quit\n"
         << "j/k or down/up -> Next/previous month\n"
         << "h/l or left/right -> Next/previous year\n"
         << "g -> Go to year\t\t\t|\t" << "G -> Go to month\n"
         << "n -> Bew event\n"
         << "e -> Show events in month\t|\t" << "E -> Show all events\n"
         << "m -> Switch between Gregorian (normal) and Julian\n"
         << ": -> Command prompt\n"
         << "? -> Show this menu" << flush;
}

/// @brief Handles `:` commands (except for :q)
/// @param command The command name
void handleCommands(string command) // TODO Define commands in an array of structs (command and aliases, desc, function?)
{
    string tempStr;

    if (command == "detect")
    {
        const char key = _getch();
        cout << "Detected key: " << (int)key << " ";
        if (key == 0 || key == 224 || key == -32)
        {
            cout << "key code: " << _getch();
        }
    }
    else if (command == "esc")
    {
        cout << "Enter ANSI escape code (everything after \\e[): ";
        getline(cin, tempStr);
        cout << "\e[" << tempStr << "test" << "\e[0m ";
    }
    else if (command == "julian" || command == "gregorian")
    {
        cout << "\nThe Gregorian calendar is the calendar we use today.\n"
             << "It has leap years every 4 years, "
                "except if the year is divisible by 100, it also must be divisible by 400.\n\n"
             << "The Julian calendar only has leap years every 4 years (causing a year length to be 365.25 days).\n"
             << "This led to the dates coming out sync with the seasons (Easter) by around 10 days due to Earth's actual year length being ~365.24.\n"
             << "In the 16th century, Pope Gregory's advisors devised the Gregorian calendar to fix this.\n"
             << "however some countries took until the earliest 20th century to switch from the Julian to Gregorian calendar.\n\n"
             << "For a more detailed explanation: \n"
             << " - https://en.wikipedia.org/wiki/Gregorian_calendar\n"
             << " - https://en.wikipedia.org/wiki/Julian_calendar\n"
             << " - https://en.wikipedia.org/wiki/Adoption_of_the_Gregorian_calendar";
    }
    else if (command == "help" || command == "h" || command == "?")
    {
        cout << " detect -> Detect the next keypress\n"
             << " esc -> Test an ANSI escape code\n"
             << " q -> Quit\n"
             << " julian or :gregorian -> Prints info about the Julian and Gregorian calendars\n"
             << " help -> Show this menu";
    }
    else
    {
        cout << "Type :help for help";
    }
}