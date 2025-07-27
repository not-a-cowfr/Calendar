#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

struct Event
{
    int year; // Unimplemented
    int month;
    int day;
    int time;
    int repeat; // Unimplemented
    std::string color;
    std::string name;
    std::string description;

    Event(
        int year,
        int month,
        int day,
        int time,
        int repeat,
        std::string color,
        std::string name,
        std::string description) : year(year), month(month), day(day), time(time), repeat(repeat),
                                   color(color), name(name), description(description) {}
};

namespace Events
{
    /// @brief Creates an event and adds it to a vector
    /// @param eventsVector The vector to add events to
    /// @param month
    /// @param day
    /// @param time The time as an integer
    /// @param color The color as the number part of an ansi code
    /// @param name
    /// @param description
    void createEvent(std::vector<Event> &eventsVector, int month, int day, int time,
                     std::string color, std::string name, std::string description)
    {
        eventsVector.emplace_back(0, month, day, time, 1, color, name, description);
        std::ofstream outputFile("events.txt", std::ios::app);
        outputFile << 0 << "\t" << month << "\t" << day << "\t" << time << "\t" << 1 << "\t" << color << "\t" << name << "\n";
    }

    /// @brief Gets events in month
    /// @param month The month to search for events
    /// @param eventsVector The vector to get from
    /// @return A vector containing the events in the specified month
    std::vector<Event> getInMonth(int month, const std::vector<Event> &eventsVector)
    {
        std::vector<Event> results;
        for (auto event : eventsVector)
        {
            if (event.month == month)
                results.push_back(event);
        }

        return results;
    }

    /// @brief Loads events from file into a vector
    /// @param eventsVector The vector to load events into
    /// @param fileName The file to load events from
    void loadFromFile(std::vector<Event> &eventsVector, std::string fileName)
    {
        std::ifstream inputFile(fileName);
        if (!inputFile.is_open())
            return;

        std::string line;
        while (std::getline(inputFile, line))
        {
            std::istringstream lineStream(line);
            std::string temp;

            int year, month, day, time, repeat;
            std::string color, name, description;
            try
            {
                std::getline(lineStream, temp, '\t');
                year = std::stoi(temp);
                std::getline(lineStream, temp, '\t');
                month = std::stoi(temp);
                std::getline(lineStream, temp, '\t');
                day = std::stoi(temp);
                std::getline(lineStream, temp, '\t');
                time = std::stoi(temp);
                std::getline(lineStream, temp, '\t');
                repeat = std::stoi(temp);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error reading event: " << line << '\n';
            }

            std::getline(lineStream, color, '\t');
            std::getline(lineStream, name, '\t');
            std::getline(lineStream, description, '\t');

            eventsVector.emplace_back(year, month, day, time, repeat, color, name, description);
        }
    }
}
