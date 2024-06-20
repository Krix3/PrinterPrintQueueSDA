#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <windows.h>

using namespace std;

// ��������� ��� ������������� ������� ������
struct PrintJob
{
    string client;
    int priority;
    time_t requestTime;

    PrintJob(string c, int p) : client(c), priority(p), requestTime(time(nullptr)) {}
};

// ��������� ��� ������������� ���������� ������
struct PrintStatistics
{
    string client;
    time_t printTime;

    PrintStatistics(string c) : client(c), printTime(time(nullptr)) {}
};

// ����� ��� ��������� ����������� �������
struct ComparePriority
{
    bool operator()(const PrintJob& a, const PrintJob& b)
    {
        return a.priority < b.priority; // ����� ������� ��������� ����� ������� �������� ���������
    }
};

// ����� ��� ���������� �������� ������
class PrinterQueue
{
private:
    priority_queue<PrintJob, vector<PrintJob>, ComparePriority> printQueue;

public:
    void addJob(const PrintJob& job)
    {
        printQueue.push(job);
    }

    bool processJob(PrintStatistics& stat)
    {
        if (!printQueue.empty())
        {
            PrintJob job = printQueue.top();
            printQueue.pop();
            stat = PrintStatistics(job.client);
            cout << "Printing job for client: " << job.client
                << " at " << put_time(localtime(&stat.printTime), "%Y-%m-%d %H:%M:%S") << endl;
            return true;
        }
        return false;
    }
};

// ����� ��� �������� � ������ ���������� ������
class StatisticsQueue {
private:
    queue<PrintStatistics> statsQueue;

public:
    void addStat(const PrintStatistics& stat)
    {
        statsQueue.push(stat);
    }

    void printStats() const
    {
        queue<PrintStatistics> tempQueue = statsQueue;
        cout << "Print statistics:" << endl;
        while (!tempQueue.empty())
        {
            PrintStatistics stat = tempQueue.front();
            tempQueue.pop();
            cout << "Client: " << stat.client
                << ", Print Time: " << put_time(std::localtime(&stat.printTime), "%Y-%m-%d %H:%M:%S") << endl;
        }
    }
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    PrinterQueue printerQueue;
    StatisticsQueue statisticsQueue;

    char choice;
    do
    {
        cout << "Enter 'a' to add a print job, 'p' to process a job, 's' to show stats, 'q' to quit: ";
        cin >> choice;
        cin.ignore();

        if (choice == 'a')
        {
            string client;
            int priority;
            cout << "Enter client name: ";
            getline(cin, client);
            cout << "Enter priority (higher number means higher priority): ";
            cin >> priority;
            cin.ignore();

            printerQueue.addJob(PrintJob(client, priority));
        }
        else if (choice == 'p')
        {
            PrintStatistics stat("");
            if (printerQueue.processJob(stat))
            {
                statisticsQueue.addStat(stat);
            }
            else
            {
                cout << "No jobs in the queue." << endl;
            }
        }
        else if (choice == 's')
        {
            statisticsQueue.printStats();
        }
    } while (choice != 'q');

    return 0;
}