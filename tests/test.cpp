#include <iostream>
#include "../src/calendar.cpp"

using namespace std;

// Day of the week test
int test(int startY, int endY, Calendar::Mode gregorian)
{
    int total = 0;
    // int today = dayOfTheWeek(startY-1, 12, 31, gregorian), yesterday;
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

                /*
                    cout << setfill(' ') << setw(5) << i
                    << " | " << setw(4) << yY << "/" << setfill('0') << setw(2) << yM << "/" << setw(2) << yD << setfill(' ')
                    << " | " << setw(4) << y << "/" << setfill('0') << setw(2) << m << "/" << setw(2) << d << ": "
                    << "\n";
                */

                // int oldDotw = dayOfTheWeekOld(y, m, d, gregorian);
                // int newDotw = dayOfTheWeek(y, m, d, gregorian);
                // int otherDotw = dayofweek(m, d, y, gregorian);
                // if (oldDotw != otherDotw)
                // {
                //     cout << setfill(' ') << setw(5) << i
                //          << " | " << setw(4) << yY << "/" << setfill('0') << setw(2) << yM << "/" << setw(2) << yD << setfill(' ')
                //          << " | " << setw(4) << y << "/" << setfill('0') << setw(2) << m << "/" << setw(2) << d << ": "
                //          << oldDotw << " " << newDotw << "\n";
                //     total++;
                // }

                int yesterday = Calendar::dayOfTheWeek(yY, yM, yD, gregorian);
                int today = Calendar::dayOfTheWeek(y, m, d, gregorian);

                // if (true)
                if (yesterday != ((today + 6) % 7)) // || (i > 56 && i < 65))
                // // if ((m == 2 && d >= 28) || (m==3 && d == 1))
                {
                    cout << setfill(' ') << setw(5) << i
                         << " | " << setw(4) << yY << "/" << setfill('0') << setw(2) << yM << "/" << setw(2) << yD << setfill(' ')
                         << " | " << setw(4) << y << "/" << setfill('0') << setw(2) << m << "/" << setw(2) << d
                         << " " << yesterday << " " << today << "\n";
                    // cout << y << "-" << m << "-" << d << ": " << yesterday << " " << today << "\n";
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