#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/// @brief Struct containing data on an event
struct Event
{
    int year; // TODO
    int month;
    int day;
    int time;   // TODO. The time of day in minutes (0-1439)
    int repeat; // TODO
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
    void createEvent(std::vector<Event> &eventsVector, int year, int month, int day, int time,
                     int repeat, std::string color, std::string name, std::string description);
    std::vector<Event> getInMonth(int month, const std::vector<Event> &eventsVector);
    bool compareEvents(const Event &event1, const Event &event2);
    void saveToFile(std::vector<Event> &eventsVector, std::string fileName);
    void loadFromFile(std::vector<Event> &eventsVector, std::string fileName);

    /// @brief Creates an event and adds it to a vector
    /// @param eventsVector The vector to add events to
    /// @param year The year the event starts
    /// @param month The month the event starts (1-12)
    /// @param day The day of the month the event starts (1-31)
    /// @param time The time as an integer
    /// @param color The color as the number part of an ansi code
    /// @param repeat
    /// @param name The name of the event
    /// @param description Extra info/description of the event
    void createEvent(std::vector<Event> &eventsVector, int year, int month, int day, int time,
                     int repeat, std::string color, std::string name, std::string description)
    {
        eventsVector.emplace_back(year, month, day, time, repeat, color, name, description);
        std::sort(eventsVector.begin(), eventsVector.end(), compareEvents);
        std::ofstream outputFile("events.txt", std::ios::app);
        outputFile << year << "\t" << month << "\t" << day << "\t" << time << "\t" << month << "\t"
                   << color << "\t" << name << "\t" << description << "\n";
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

    /// @brief A compare function for sorting events
    /// @param event1
    /// @param event2
    /// @return Whether event1 comes before event2 (true/false)
    bool compareEvents(const Event &event1, const Event &event2)
    {
        if (event1.year < event2.year)
            return true;
        else if (event1.month < event2.month)
            return true;
        else
            return event1.day < event2.day;
    }

    /// @brief Sorts and saves events from a vector into a file
    /// @param eventsVector The vector of events
    /// @param fileName The file name to save as
    void saveToFile(std::vector<Event> &eventsVector, std::string fileName)
    {
        std::sort(eventsVector.begin(), eventsVector.end(), compareEvents);
        std::ofstream outputFile(fileName);
        if (!outputFile.is_open())
        {
            std::cerr << "Unable to open file \"" << fileName << "\"\n";
            return;
        }
        for (auto event : eventsVector)
        {
            outputFile << event.year << "\t" << event.month << "\t" << event.day << "\t"
                       << event.time << "\t" << event.repeat << "\t"
                       << event.color << "\t" << event.name << "\t" << event.description << "\n";
        }
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
