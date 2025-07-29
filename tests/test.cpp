#include <iostream>
#include "../src/calendar.cpp"

using namespace std;

/// @brief A simple test that loops through every day in the years specified,
///        and checks if the day of the week lines up with the previous day's dotw
/// @param startY The starting year
/// @param endY The ending year
/// @param gregorian Whether to use Gregorian or Julian calendar
/// @return The number of incorrect dates found
int test(int startY, int endY, Calendar::Mode gregorian)
{
    int total = 0;
    int i = 0;
    int yY = startY - 1, yM = 1, yD = 1;
    for (int y = startY; y <= endY; y++)
    {
        int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        days[1] += isLeapYear(y, gregorian);

        for (int m = 1; m <= 12; m++)
        {
            for (int d = 1; d <= days[m - 1]; d++)
            {
                int yesterday = Calendar::dayOfTheWeek(yY, yM, yD, gregorian);
                int today = Calendar::dayOfTheWeek(y, m, d, gregorian);

                if (yesterday != ((today + 6) % 7))
                {
                    cout << setfill(' ') << setw(5) << i
                         << " | " << setw(4) << yY << "/" << setfill('0') << setw(2) << yM << "/" << setw(2) << yD << setfill(' ')
                         << " | " << setw(4) << y << "/" << setfill('0') << setw(2) << m << "/" << setw(2) << d
                         << " " << yesterday << " " << today << "\n";
                   total++;
                }
                yD = d;
                yM = m;
                yY = y;

                i++;
            }
        }
    }

    return total;
}

int main()
{
    Calendar::Mode greg = Calendar::Mode::Gregorian;
    int testResult = test(-10000, 10000, greg);
    cout << "Total incorrect dates: " << testResult;

    return testResult;
}
