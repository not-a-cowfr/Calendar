#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
#include "./events.cpp"

/// @brief Namespace with calendar related functions
namespace Calendar
{
    /// @brief Enum for whether to use the Gregorian or Julian calendar
    enum Mode
    {
        Julian = 0,
        Gregorian = 1,
        // TODO: Implement Auto
    };

    void displayMonth(int year, int month, std::vector<Event> events, Mode mode);
    void displayDays(int startDay, int endDay, int &column, std::pair<int, int> yearMonth,
                     std::vector<Event> events, std::string formatCode = "");
    int dayOfTheWeek(int year, int month, int day, Mode mode);
    bool isLeapYear(int year, Mode mode);
    std::string monthName(int month, int length = 0);
    std::string dayName(int day, int length = 0);
    std::pair<int, int> lastMonth(int year, int month);
    std::pair<int, int> nextMonth(int year, int month);


    /// @brief Generates a calendar for the given month and year
    /// @param year The year
    /// @param month The month (1-12)
    /// @param mode Whether to use the Gregorian or Julian calendar
    void displayMonth(int year, int month, std::vector<Event> events, Mode mode)
    {
        int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        days[1] += isLeapYear(year, mode);

        int dotw = dayOfTheWeek(year, month, 1, mode);
        int i = 0;
        const int prevIndex = (month + 10) % 12;

        std::cout << monthName(month) << " " << year;
        if (mode == Mode::Julian)
            std::cout << " (Julian)";

        std::cout << "\n";

        for (int d = 0; d < 7; d++)
        {
            std::cout << " " << dayName(d, 2) << " ";
        }
        std::cout << "\n";

        displayDays(days[prevIndex] - dotw + 1, days[prevIndex], i, lastMonth(year, month),
                    Events::getInMonth(prevIndex + 1, events), "\e[90m");

        displayDays(1, days[month - 1], i, {year, month}, Events::getInMonth(month, events));

        // Fill the remaining 6 weeks
        displayDays(1, 6 * 7 - i, i, nextMonth(year, month),
                    Events::getInMonth((month % 12) + 1, events), "\e[90m");
    }

    /// @brief Prints a series of days with line breaks after Saturdays
    /// @param startDay The first day to print
    /// @param endDay The last day to print
    /// @param column The column/day of the week (the mod 7 value is used for linebreaks)
    /// @param yearMonth The year and month as a pair of ints
    /// @param formatCode The ansi formatting code
    /// @param events The vector of the events to consider (usually the month's events)
    void displayDays(int startDay, int endDay, int &column, std::pair<int, int> yearMonth,
                     std::vector<Event> events, std::string formatCode)
    {
        time_t timestamp = time(0);
        struct tm datetime = *localtime(&timestamp);
        int curYear = datetime.tm_year + 1900,
            curMonth = datetime.tm_mon + 1,
            today = datetime.tm_mday;

        auto [year, month] = yearMonth;

        std::cout << formatCode;
        for (int day = startDay; day <= endDay; day++)
        {
            char left = ' ', right = ' ';
            std::string leftFormat = "", rightFormat = "";

            for (auto event : events)
            {
                if (event.day == day)
                {
                    leftFormat = "\e[" + event.color + "m";
                    rightFormat = "\e[0m" + formatCode;
                    left = '[';
                    right = ']';
                    break;
                }
            }

            if (day == today && year == curYear && month == curMonth)
            {
                left = '<';
                right = '>';
            }

            std::cout << leftFormat << left << std::setw(2) << day << right << rightFormat;

            if (column++ % 7 == 6)
                std::cout << "\n";
        }
        std::cout << "\e[0m";
    }

    /// @brief Calculates the day of the week of given date.
    /// Based on a modification of Zeller's Congruence by Claus Tøndering where Sunday is 0.
    ///     See https://www.tondering.dk/claus/cal/chrweek.php#calcdow
    ///     see also https://en.wikipedia.org/wiki/Zeller%27s_congruence
    /// @param year The year (use negative numbers for BC)
    /// @param month The month (0-12)
    /// @param day The day (0-6)
    /// @param mode Whether to use the Gregorian or Julian calendar
    /// @return An integer 0-6 (starting at Sunday)
    int dayOfTheWeek(int year, int month, int day, Mode mode)
    {
        int y = year, m = month - 2;
        if (month <= 2)
        {
            m += 12;
            y--;
        }

        y = y % 2800 + 2800; // Fix negative years by using a year with the same calendar
                             // Julian repeats every 28 years, Gregorian repeats every 400 years

        const int calendar = mode ? (-y / 100 + y / 400) : 5; // Gregorian or Julian formula versions of formula

        return (day + (31 * m) / 12 + y + y / 4 + calendar) % 7;
    }

    /// @brief Calculates if a year is a leap year in the specified calendar
    /// @param year The year in question
    /// @param mode Whether to use the Gregorian or Julian calendar
    /// @return true if the year is a leap year, or false
    bool isLeapYear(int year, Mode mode)
    {
        return mode ? year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)
                    : year % 4 == 0;
    }

    /// @brief Determines whether to use the Gregorian or Julian calendar
    /// @param year The year
    /// @param month The month (1-12)
    /// @return Gregorian or Julian as a Calendar::Mode
    Mode determineCalendar(int year, int month)
    {
        return (year > 1752 || month >= 9) ? Gregorian : Julian;
    }

    /// @brief Returns the month name from the month number, up to a specified length
    /// @param month The month as an integer (1-12)
    /// @param length The amount of characters to return. 0 or less for the full name
    /// @return The month name as a string, up to the specified length
    std::string monthName(int month, int length)
    {
        std::string monthNames[12] =
            {"January", "February", "March", "April", "May", "June",
             "July", "August", "September", "October", "November", "December"};

        if (length < 1)
            return monthNames[month - 1];

        std::string result = "";
        for (int i = 0; i < length && monthNames[month - 1][i] != '\0'; i++)
            result += monthNames[month - 1][i];

        return result;
    }

    /// @brief Returns the day name from the day number, up to a specified length
    /// @param day The day as an integer (0-6), starting at Sunday
    /// @param length The amount of characters to return. 0 or less for the full name
    /// @return The day name as a string, up to the specified length
    std::string dayName(int day, int length)
    {
        std::string dayNames[7] =
            {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

        if (length < 1)
            return dayNames[day];

        std::string result = "";
        for (int i = 0; i < length && dayNames[day][i] != '\0'; i++)
            result += dayNames[day][i];

        return result;
    }

    /// @brief Calculates the previous month (and year)
    /// @param year The year
    /// @param month The month (1-12)
    /// @return A pair of ints representing the year and month
    std::pair<int, int> lastMonth(int year, int month)
    {
        if (month == 1)
            return {year - 1, 12};
        return {year, month - 1};
    }

    /// @brief Calculates the next month (and year)
    /// @param year The year
    /// @param month The month (1-12)
    /// @return A pair of ints representing the year and month
    std::pair<int, int> nextMonth(int year, int month)
    {
        if (month == 12)
            return {year + 1, 1};
        return {year, month + 1};
    }

    /// @brief Calculates the amount of days in a specific month
    /// @param year The year
    /// @param month The month (1-12)
    /// @param mode Whether to use the Gregorian or Julian calendar
    /// @return An integer representing the amount of days in the month
    int daysInMonth(int year, int month, Mode mode)
    {
        int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        days[1] += isLeapYear(year, mode);

        return days[month - 1];
    }
}
