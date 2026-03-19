#include <bits/stdc++.h>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <functional>
#include <optional>
using namespace std;

// ----------------------------- Utility helpers -----------------------------
namespace util
{
    string trim(const string &s)
    {
        auto a = s.find_first_not_of(" \t\n\r");
        if (a == string::npos)
            return "";
        auto b = s.find_last_not_of(" \t\n\r");
        return s.substr(a, b - a + 1);
    }

    string toLower(const string &s)
    {
        string out = s;
        transform(out.begin(), out.end(), out.begin(), [](unsigned char c)
                  { return tolower(c); });
        return out;
    }

    string simpleHash(const string &input)
    {
        return to_string(hash<string>{}(input));
    }

    bool isValidStationName(const string &s)
    {
        string trimmed = trim(s);
        return !trimmed.empty() && trimmed.length() <= 50;
    }
}

// ----------------------------- Graph class ---------------------------------
class Graph
{
public:
    unordered_map<string, vector<string>> adj;

    void addEdge(const string &u, const string &v, bool bidir = true)
    {
        string a = util::trim(u);
        string b = util::trim(v);

        // Validate station names
        if (!util::isValidStationName(a) || !util::isValidStationName(b) || a == b)
        {
            return;
        }

        // avoid duplicate entries
        auto &vecA = adj[a];
        if (find(vecA.begin(), vecA.end(), b) == vecA.end())
            vecA.push_back(b);

        if (bidir)
        {
            auto &vecB = adj[b];
            if (find(vecB.begin(), vecB.end(), a) == vecB.end())
                vecB.push_back(a);
        }
    }

    vector<string> getAllStations() const
    {
        vector<string> out;
        out.reserve(adj.size());
        for (auto &p : adj)
            out.push_back(p.first);
        sort(out.begin(), out.end());
        return out;
    }

    void printAdj() const
    {
        cout << "\nAdjacency list:\n";
        for (auto &p : adj)
        {
            cout << p.first << " -> ";
            for (size_t i = 0; i < p.second.size(); ++i)
            {
                cout << p.second[i];
                if (i + 1 < p.second.size())
                    cout << " , ";
            }
            cout << "\n";
        }
    }

    vector<string> shortestPath(const string &src, const string &dest) const
    {
        string s = util::trim(src);
        string t = util::trim(dest);

        if (s.empty() || t.empty())
            return {};
        if (adj.find(s) == adj.end() || adj.find(t) == adj.end())
            return {};

        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;
        queue<string> q;

        q.push(s);
        visited[s] = true;
        parent[s] = "";
        bool found = false;

        while (!q.empty())
        {
            auto cur = q.front();
            q.pop();

            if (cur == t)
            {
                found = true;
                break;
            }

            auto it = adj.find(cur);
            if (it == adj.end())
                continue;

            for (auto &nei : it->second)
            {
                if (!visited[nei])
                {
                    visited[nei] = true;
                    parent[nei] = cur;
                    q.push(nei);
                }
            }
        }

        if (!found)
            return {};

        vector<string> path;
        string cur = t;
        while (cur != "")
        {
            path.push_back(cur);
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

// ----------------------------- User classes --------------------------------
struct Trip
{
    vector<string> path;
    bool peak;
    int fare;
    time_t timestamp;
};

class User
{
public:
    string username;
    string passwordHash;
    vector<Trip> history;

    User() = default;
    User(const string &u, const string &ph) : username(u), passwordHash(ph) {}

    
    User(const User &other) = default;
    User &operator=(const User &other) = default;
    User(User &&other) = default;
    User &operator=(User &&other) = default;
    ~User() = default;

    void addTrip(const Trip &t) { history.push_back(t); }
};

// Simple file-based user manager
class UserManager
{
    vector<User> users;
    string storageFile = "users.db";

public:
    UserManager() { load(); }
    ~UserManager() { save(); }

    bool signup(const string &username, const string &password)
    {
        string u = util::trim(username);
        if (u.size() < 3 || password.size() < 3)
        {
            cout << "Username and password must be at least 3 characters.\n";
            return false;
        }

        if (findByName(u) != nullptr)
        {
            cout << "Username already exists.\n";
            return false;
        }

        users.emplace_back(u, util::simpleHash(password));
        cout << "User '" << u << "' registered successfully!\n";
        return true;
    }

    User *login(const string &username, const string &password)
    {
        User *u = findByName(util::trim(username));
        if (!u)
        {
            cout << "User not found.\n";
            return nullptr;
        }

        if (u->passwordHash == util::simpleHash(password))
            return u;

        cout << "Invalid password.\n";
        return nullptr;
    }

    void save()
    {
        ofstream ofs(storageFile, ios::trunc);
        if (!ofs)
        {
            cout << "Warning: Could not save user data.\n";
            return;
        }

        for (auto &u : users)
        {
            ofs << u.username << '\n'
                << u.passwordHash << '\n';
            ofs << u.history.size() << "\n\n";

            for (auto &t : u.history)
            {
                ofs << t.timestamp << '\n'
                    << (t.peak ? 1 : 0) << '\n'
                    << t.fare << '\n'
                    << t.path.size() << '\n';

                for (auto &s : t.path)
                    ofs << s << '\n';
            }
        }

        cout << "User data saved successfully!\n";
    }

    void load()
    {
        ifstream ifs(storageFile);
        if (!ifs)
        {
            cout << "No existing user data found. Starting fresh.\n";
            return;
        }

        users.clear();
        string uname, ph;

        while (getline(ifs, uname) && getline(ifs, ph))
        {
            if (uname.empty() || ph.empty())
                continue;

            User u(uname, ph);
            size_t histCount;

            if (!(ifs >> histCount))
                break;
            ifs.ignore(1000, '\n');

            for (size_t i = 0; i < histCount; i++)
            {
                Trip t;
                size_t pathsz;
                int peakInt;

                if (!(ifs >> t.timestamp >> peakInt >> t.fare >> pathsz))
                    break;
                ifs.ignore(1000, '\n');

                t.peak = (peakInt != 0);
                t.path.clear();

                for (size_t j = 0; j < pathsz; j++)
                {
                    string station;
                    if (!getline(ifs, station) || station.empty())
                        break;
                    t.path.push_back(station);
                }

                if (t.path.size() == pathsz)
                {
                    u.history.push_back(t);
                }
            }
            users.push_back(move(u));
        }

        cout << "Loaded " << users.size() << " users from storage.\n";
    }

    User *findByName(const string &username)
    {
        for (auto &u : users)
            if (u.username == username)
                return &u;
        return nullptr;
    }

    vector<string> listUsernames() const
    {
        vector<string> out;
        for (auto &u : users)
            out.push_back(u.username);
        return out;
    }
};

// ----------------------------- Metro System -------------------------------
class MetroSystem
{
    Graph g;
    UserManager &userMgr;
    User *currentUser = nullptr;
    double peakMultiplier = 1.5;
    int baseCostPerStation = 10;

public:
    MetroSystem(UserManager &um) : userMgr(um) {}
    ~MetroSystem() { logout(); }

    void loadGraphFromInput()
    {
        cout << "Enter number of connections (edges): ";
        int n;
        if (!(cin >> n) || n <= 0)
        {
            cout << "Invalid number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }

        cin.ignore(1000, '\n');

        cout << "Enter pairs (stationA stationB) for each connection (one pair per line):\n";
        string a, b;

        for (int i = 0; i < n; i++)
        {
            cout << "Connection " << (i + 1) << ": ";
            if (!(cin >> a >> b))
            {
                cout << "Invalid connection input.\n";
                cin.clear();
                cin.ignore(1000, '\n');
                break;
            }
            g.addEdge(a, b, true);
        }

        cout << "Graph loaded with " << g.getAllStations().size() << " stations.\n";
    }

    void showStations()
    {
        auto s = g.getAllStations();
        cout << "\nStations (" << s.size() << "):\n";
        for (auto &st : s)
            cout << " - " << st << "\n";
    }

    void showAdj() { g.printAdj(); }

    void detectPeakAndShowTime()
    {
        using namespace chrono;
        auto now = system_clock::now();
        time_t t = system_clock::to_time_t(now);
        tm local = *localtime(&t);

        cout << "Current time: " << setw(2) << setfill('0') << local.tm_hour << ":"
             << setw(2) << local.tm_min << ":" << setw(2) << local.tm_sec << "\n";
        cout << "Peak hours: 07:00-10:00 and 17:00-20:00\n";

        bool peak = isPeakNow();
        cout << (peak ? "Peak hours active - fares are 1.5x\n" : "Off-peak hours - regular fares\n");
    }

    void signup()
    {
        cout << "Username: ";
        string u;
        getline(cin, u);
        u = util::trim(u);

        cout << "Password: ";
        string p;
        getline(cin, p);

        userMgr.signup(u, p);
    }

    void login()
    {
        if (currentUser)
        {
            cout << "Already logged in as " << currentUser->username << "\n";
            return;
        }

        cout << "Username: ";
        string u;
        getline(cin, u);

        cout << "Password: ";
        string p;
        getline(cin, p);

        User *res = userMgr.login(u, p);
        if (res)
        {
            currentUser = res;
            cout << "Welcome, " << currentUser->username << "!\n";
        }
    }

    void logout()
    {
        if (currentUser)
        {
            cout << "Goodbye, " << currentUser->username << "!\n";
            currentUser = nullptr;
        }
        else
        {
            cout << "No user is currently logged in.\n";
        }
    }

    void bookTicket()
    {
        if (!currentUser)
        {
            cout << "Please login first (option 3).\n";
            return;
        }

        showStations();
        cout << "Enter boarding station: ";
        string src;
        getline(cin, src);

        cout << "Enter destination station: ";
        string dest;
        getline(cin, dest);

        auto path = g.shortestPath(src, dest);
        if (path.empty())
        {
            cout << "No path found between '" << src << "' and '" << dest << "'.\n";
            suggestStations(src);
            suggestStations(dest);
            return;
        }

        cout << "Optimal route: ";
        for (size_t i = 0; i < path.size(); ++i)
            cout << path[i] << (i + 1 == path.size() ? "" : " -> ");
        cout << "\n";

        bool peak = isPeakNow();
        int stationsCount = max(0, (int)path.size() - 1);
        int fare = calculateFare(stationsCount, peak);

        cout << "Journey details:\n";
        cout << " - Stations traveled: " << stationsCount << "\n";
        cout << " - Time: " << (peak ? "Peak hours" : "Off-peak hours") << "\n";
        cout << " - Total fare: " << fare << " rupees\n";

        cout << "Confirm booking? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore(1000, '\n');

        if (confirm == 'y' || confirm == 'Y')
        {
            Trip t;
            t.path = path;
            t.peak = peak;
            t.fare = fare;
            t.timestamp = time(nullptr);
            currentUser->addTrip(t);
            cout << "Ticket booked successfully! Saved to your history.\n";
        }
        else
        {
            cout << "Booking cancelled.\n";
        }
    }

    void showHistory()
    {
        if (!currentUser)
        {
            cout << "Please login to see your trip history.\n";
            return;
        }

        if (currentUser->history.empty())
        {
            cout << "No trips recorded yet.\n";
            return;
        }

        cout << "\nTrip history for " << currentUser->username << ":\n";
        cout << "--------------------------------------------------\n";

        for (size_t idx = 0; idx < currentUser->history.size(); idx++)
        {
            auto &t = currentUser->history[idx];
            auto tm = localtime(&t.timestamp);

            cout << "Trip #" << (idx + 1) << " - ";
            cout << "[" << setw(2) << setfill('0') << tm->tm_mday << "-"
                 << setw(2) << (tm->tm_mon + 1) << "-" << (tm->tm_year + 1900)
                 << " " << setw(2) << tm->tm_hour << ":" << setw(2) << tm->tm_min << "] ";
            cout << (t.peak ? "(PEAK) " : "(OFF-PEAK) ");
            cout << "Fare: " << t.fare << " rupees\n";
            cout << "Route: ";
            for (size_t i = 0; i < t.path.size(); ++i)
                cout << t.path[i] << (i + 1 == t.path.size() ? "" : " -> ");
            cout << "\n--------------------------------------------------\n";
        }
    }

    void checkRouteCostOnly()
    {
        showStations();
        cout << "Enter boarding station: ";
        string s;
        getline(cin, s);

        cout << "Enter destination station: ";
        string d;
        getline(cin, d);

        auto path = g.shortestPath(s, d);
        if (path.empty())
        {
            cout << "No route found between '" << s << "' and '" << d << "'.\n";
            suggestStations(s);
            suggestStations(d);
            return;
        }

        bool peak = isPeakNow();
        int stationsCount = max(0, (int)path.size() - 1);
        int fare = calculateFare(stationsCount, peak);

        cout << "\nRoute found:\n";
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); ++i)
            cout << path[i] << (i + 1 == path.size() ? "" : " -> ");
        cout << "\n";
        cout << "Stations traveled: " << stationsCount << "\n";
        cout << "Current time: " << (peak ? "Peak hours" : "Off-peak hours") << "\n";
        cout << "Estimated fare: " << fare << " rupees\n";
    }

    bool isPeakNow()
    {
        using namespace chrono;
        auto now = system_clock::now();
        time_t t = system_clock::to_time_t(now);
        tm local = *localtime(&t);
        return ((local.tm_hour >= 7 && local.tm_hour < 10) ||
                (local.tm_hour >= 17 && local.tm_hour < 20));
    }

    int calculateFare(int stationsCount, bool peak)
    {
        if (stationsCount <= 0)
            return 0;
        if (stationsCount > 100)
        {
            cout << "Warning: Unusually long route detected.\n";
        }

        double base = stationsCount * baseCostPerStation;
        if (peak)
            base *= peakMultiplier;

        // Prevent integer overflow
        if (base > INT_MAX - 1000)
        {
            return INT_MAX;
        }

        return (int)round(base);
    }

    void suggestStations(const string &s)
    {
        string q = util::trim(s);
        if (q.empty())
            return;

        auto all = g.getAllStations();
        string ql = util::toLower(q);
        vector<string> suggestions;

        // Exact prefix matches
        for (auto &st : all)
        {
            string sl = util::toLower(st);
            if (sl.rfind(ql, 0) == 0 && sl != ql)
            {
                suggestions.push_back(st);
            }
        }

        // Substring matches if no prefix matches found
        if (suggestions.empty())
        {
            for (auto &st : all)
            {
                string sl = util::toLower(st);
                if (sl.find(ql) != string::npos && sl != ql)
                {
                    suggestions.push_back(st);
                }
            }
        }

        // Display suggestions
        if (!suggestions.empty())
        {
            cout << "Did you mean one of these instead of '" << q << "'?\n";
            for (auto &sug : suggestions)
            {
                cout << " - " << sug << "\n";
            }
        }
    }

    void playSudoku();
};

// ----------------------------- Sudoku Module -------------------------------
namespace Sudoku
{
    const int N = 9;

    bool usedInRow(int grid[N][N], int row, int num)
    {
        for (int col = 0; col < N; col++)
            if (grid[row][col] == num)
                return true;
        return false;
    }

    bool usedInCol(int grid[N][N], int col, int num)
    {
        for (int row = 0; row < N; row++)
            if (grid[row][col] == num)
                return true;
        return false;
    }

    bool usedInBox(int grid[N][N], int boxStartRow, int boxStartCol, int num)
    {
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (grid[r + boxStartRow][c + boxStartCol] == num)
                    return true;
        return false;
    }

    bool isSafe(int grid[N][N], int row, int col, int num)
    {
        return !usedInRow(grid, row, num) &&
               !usedInCol(grid, col, num) &&
               !usedInBox(grid, row - row % 3, col - col % 3, num) &&
               grid[row][col] == 0;
    }

    bool findUnassignedLocation(int grid[N][N], int &row, int &col)
    {
        for (row = 0; row < N; row++)
            for (col = 0; col < N; col++)
                if (grid[row][col] == 0)
                    return true;
        return false;
    }

    bool solveSudoku(int grid[N][N])
    {
        int row, col;
        if (!findUnassignedLocation(grid, row, col))
            return true;

        for (int num = 1; num <= 9; num++)
        {
            if (isSafe(grid, row, col, num))
            {
                grid[row][col] = num;
                if (solveSudoku(grid))
                    return true;
                grid[row][col] = 0;
            }
        }
        return false;
    }

    void printGrid(int grid[N][N])
    {
        cout << "\nSudoku Solution:\n";
        cout << "-------------------\n";
        for (int r = 0; r < N; r++)
        {
            if (r % 3 == 0 && r != 0)
                cout << "------+-------+------\n";

            for (int c = 0; c < N; c++)
            {
                if (c % 3 == 0 && c != 0)
                    cout << "| ";
                cout << grid[r][c] << " ";
            }
            cout << "\n";
        }
        cout << "-------------------\n";
    }
}

void MetroSystem::playSudoku()
{
    cout << "\n=== Sudoku Game ===\n";
    int puzzle[Sudoku::N][Sudoku::N] = {
        {1, 0, 6, 0, 0, 2, 3, 0, 0},
        {0, 5, 0, 0, 0, 6, 0, 9, 1},
        {0, 0, 9, 5, 0, 1, 4, 6, 2},
        {0, 3, 7, 9, 0, 5, 0, 0, 0},
        {5, 8, 1, 0, 2, 7, 9, 0, 0},
        {0, 0, 0, 4, 0, 8, 1, 5, 7},
        {0, 0, 0, 2, 6, 0, 5, 4, 0},
        {0, 0, 4, 1, 5, 0, 6, 0, 9},
        {9, 0, 0, 8, 7, 4, 2, 1, 0}};

    cout << "Current puzzle (0 represents empty cells):\n";
    Sudoku::printGrid(puzzle);

    cout << "Press Enter to solve this Sudoku puzzle...";
    string tmp;
    getline(cin, tmp);

    // Create a copy for solving
    int solvingPuzzle[Sudoku::N][Sudoku::N];
    memcpy(solvingPuzzle, puzzle, sizeof(puzzle));

    if (Sudoku::solveSudoku(solvingPuzzle))
    {
        cout << "Solved successfully!\n";
        Sudoku::printGrid(solvingPuzzle);
    }
    else
    {
        cout << "No solution exists for this puzzle.\n";
    }
}

// ----------------------------- Main menu ----------------------------------
void clearScreen()
{
    cout << "\n-----------------------------------------\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    UserManager userMgr;
    MetroSystem metro(userMgr);

    cout << " -------------------------------------------METRO PROJECT-------------------------------------------\n\n";
    cout << "************************************* Setting Up Our MyMetro ****************************************\n\n";
    cout << "Load graph now. You will be asked for number of connections and pairs.\n";

    metro.loadGraphFromInput();
    metro.showAdj();
    metro.detectPeakAndShowTime();

    bool running = true;
    while (running)
    {
        clearScreen();
        cout << "\n---- MAIN MENU ----\n";
        cout << "1) Play/solve Sudoku\n";
        cout << "2) Signup\n";
        cout << "3) Login\n";
        cout << "4) Logout\n";
        cout << "5) Book ticket (requires login)\n";
        cout << "6) Check shortest route & fare\n";
        cout << "7) Show stations\n";
        cout << "8) Show user history (requires login)\n";
        cout << "9) Exit\n";
        cout << "Enter choice: ";

        int ch;
        if (!(cin >> ch))
        {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        cin.ignore(1000, '\n');

        switch (ch)
        {
        case 1:
            metro.playSudoku();
            break;
        case 2:
            metro.signup();
            break;
        case 3:
            metro.login();
            break;
        case 4:
            metro.logout();
            break;
        case 5:
            metro.bookTicket();
            break;
        case 6:
            metro.checkRouteCostOnly();
            break;
        case 7:
            metro.showStations();
            break;
        case 8:
            metro.showHistory();
            break;
        case 9:
            running = false;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }

        cout << "\nPress Enter to continue...";
        string dummy;
        getline(cin, dummy);
    }

    cout << "\nThanks for using MyMetro! Have a safe journey!\n";
    return 0;
}

// DEFAULT EXAMPLE---->
/*
6
noidacity golfcourse
noidacity mayurvihar
mayurvihar golfcourse
mayurvihar dwarka
dwarka rajivchok
sector62 noidacity */
