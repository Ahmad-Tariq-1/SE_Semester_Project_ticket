#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <conio.h>
#include <iomanip>

using namespace std;

class ErrorHandling
{
public:
    bool emailValidation(const string &email)
    {
        const string gmailSuffix = "@gmail.com";

        if (email.empty())
        {
            return false;
        }

        for (char ch : email)
        {
            if (isspace(ch))
            {
                return false;
            }
        }

        if (email.size() <= gmailSuffix.size() + 4)
        {
            return false;
        }

        string prefix = email.substr(0, email.size() - gmailSuffix.size());
        string suffix = email.substr(email.size() - gmailSuffix.size());

        if (suffix != gmailSuffix)
        {
            return false;
        }

        for (char ch : prefix)
        {
            if (!islower(ch) && !isdigit(ch) && ch != '.' && ch != '_' && ch != '-')
            {
                return false;
            }
        }
        return true;
    }

    bool nameValidation(string str)
    {
        if (str.empty())
        {
            return false;
        }
        return true;
    }

    bool menuChoice(string choice)
    {
        for (int i = 0; i < choice.size(); ++i)
        {
            if (!isdigit(choice[i]))
            {
                return false;
            }
        }
        if (choice.empty())
        {
            return false;
        }
        for (int i = 0; i < choice.size(); ++i)
        {
            if (choice[i] == ' ')
            {
                return false;
            }
        }
        return true;
    }

    void passLogic(string &password, string promptText)
    {
        char pass[32] = {0};
        char ch;
        bool enter = false;
        int i = 0;
        bool show = false;

        cout << promptText;

        while (!enter)
        {
            ch = _getch();

            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
            {
                pass[i] = ch;
                if (show)
                {
                    cout << ch;
                }
                else
                {
                    cout << "*";
                }
                i++;
            }

            if (ch == '\b' && i >= 1)
            {
                cout << "\b \b";
                i--;
            }

            if (ch == '\r')
            {
                enter = true;
            }

            if (ch == '\t')
            {
                show = !show;
                cout << "\r" << promptText;
                for (int j = 0; j < i; j++)
                {
                    cout << (show ? pass[j] : '*');
                }
            }
        }

        pass[i] = '\0';
        password = pass;
    }

    bool passwordValidation(const string &str)
    {
        bool isLengthValid = str.length() >= 6;

        if (!isLengthValid)
        {
            cout << "\n\n\tPassword must be at least 6 characters long.\n\n";
        }

        return isLengthValid;
    }

    bool idValidation(string idString)
    {
        for (int i = 0; i < idString.size(); ++i)
        {
            if (!isdigit(idString[i]))
            {
                return false;
            }
        }
        if (idString.empty())
        {
            return false;
        }
        for (int i = 0; i < idString.size(); ++i)
        {
            if (idString[i] == ' ')
            {
                return false;
            }
        }
        return true;
    }
};

ErrorHandling errorHandler;
struct User
{
    string username;
    string password;
    string email;
    unordered_map<int, int> tickets;              
    unordered_map<int, int> canceledVipTickets;     
    unordered_map<int, int> canceledRegularTickets;
};

struct Event
{
    int eventId;
    string eventName;
    int totalSeats;
    int vipSeats;
    int regularSeats;
    priority_queue<int> vipSeatsQueue;                               
    priority_queue<int, vector<int>, greater<int>> regularSeatsQueue; 
    Event() : eventId(0), eventName(""), totalSeats(0), vipSeats(0), regularSeats(0){}

    Event(int id, const string &name, int total, int vip, int regular)
        : eventId(id), eventName(name), totalSeats(total), vipSeats(vip), regularSeats(regular) {}
};

unordered_map<string, User> users;
unordered_map<int, Event> events;
unordered_map<string, bool> admins;
void setupDefaultAdmin();
void updateUserProfile(const string &username);
void signup();
void login();
bool adminLogin(string &loggedInAdmin);
void searchEvents();
bool adminLogin();
void buyTickets(const string &username);
void viewTickets(const string &username);
void cancelTicket(const string &username);
void addEvent();
void updateEvent();
void manageTickets();
void viewAllUsers();
void displayUserMenu(const string &username);
void addNewAdmin();
void removeAdmin();
void updateadminPanel(const string &adminUsername);
void updateAdminPassword(const string &adminUsername);
void displayAdminMenu();
void displayAllEvents();
void displayAllUserData();
string toLowerCase(const string &str)
{
    string lowerStr = str; // Create a copy to modify
    for (int i = 0; i < lowerStr.length(); i++)
    {
        lowerStr[i] = tolower(lowerStr[i]);
    }
    return lowerStr;
}

void displayAllUserData()
{
    if (users.empty())
    {
        cout << "No users available.\n";
        return;
    }

    cout << "User Data:\n";
    for (const auto &userPair : users)
    {
        const string &username = userPair.first;
        const User &user = userPair.second;

        if (admins.find(username) != admins.end() && admins.at(username))
        {
            continue;
        }

        cout << "Username: " << user.username << "\nEmail: " << user.email << "\nTickets:\n";

        if (user.tickets.empty())
        {
            cout << "  No tickets bought.\n";
        }
        else
        {
            for (const auto &ticketPair : user.tickets)
            {
                int eventID = ticketPair.first;
                int totalTickets = ticketPair.second;

                int vipTickets = 0;
                int regularTickets = 0;
                const Event &event = events.at(eventID);

                if (user.canceledVipTickets.find(eventID) != user.canceledVipTickets.end())
                {
                    vipTickets = user.canceledVipTickets.at(eventID);
                }

                if (user.canceledRegularTickets.find(eventID) != user.canceledRegularTickets.end())
                {
                    regularTickets = user.canceledRegularTickets.at(eventID);
                }

                cout << "\nEvent ID: " << eventID
                     << ",\tEvent Name: " << event.eventName
                     << ",\tVIP Tickets: " << vipTickets
                     << ",\tRegular Tickets: " << regularTickets << '\n';
            }
        }

        cout << "-------------------------------\n";
    }
}

void updateUserProfile(const string &username)
{
    if (users.find(username) == users.end())
    {
        cout << "\n\tUser not found.";
        return;
    }

    string choice;
    string newValue;
    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Update Username" << setw(20) << "|\n";
        cout << "\t\t| [2]   |     Update Password" << setw(20) << "|\n";
        cout << "\t\t| [3]   |     Update Email" << setw(23) << "|\n";
        cout << "\t\t| [4]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "1")
        {
            while (true)
            {
                cout << "Enter new username: ";
                getline(cin, newValue);
                if (errorHandler.nameValidation(newValue))
                {
                    break;
                }
                cout << "\n\tInvalid username\n\n";
            }
            users[username].username = newValue;
            cout << "\n\tUsername updated successfully!\n";
        }

        else if (choice == "2")
        {
            while (true)
            {
                errorHandler.passLogic(newValue, "Enter new Password: ");
                if (errorHandler.passwordValidation(newValue))            
                {
                    break; 
                }
            }
            users[username].password = newValue;
            cout << "\n\tPassword updated successfully!\n";
        }
        else if (choice == "3")
        {
            while (true)
            {
                cout << "Enter new email: ";
                getline(cin, newValue);
                if (errorHandler.emailValidation(newValue))
                {
                    break;
                }
                else
                {
                    cout << "\n\tInvalid Email. Please try again.\n\n";
                }
            }
            users[username].email = newValue;
            cout << "\n\tEmail updated successfully!\n";
        }
        else if (choice == "4")
        {
            cout << "\nExiting User Panel...\n";
            break;
        }
        else
        {
            cout << "\n\tInvalid choice. Please try again.\n";
        }
    }
}
void setupDefaultAdmin()
{
    string defaultAdminUsername = "ahmed";
    string defaultAdminPassword = "123456";
    string defaultAdminEmail = "ahmed2708@gmail.com";
    

    if (users.find(defaultAdminUsername) == users.end())
    {
        User adminUser = {defaultAdminUsername, defaultAdminPassword, defaultAdminEmail, {}};
        users[defaultAdminUsername] = adminUser;
        admins[defaultAdminUsername] = true;
        cout << "Default admin account created successfully!\n";
    }
    else
    {
        cout << "Default admin already exists.\n";
    }
}

void buyTickets(const string &username)
{
    if (events.empty())
    {
        cout << "No events available.\n";
        return;
    }

    string eventIDStr;
    int eventID;

    cout << "Enter event ID: ";
    getline(cin, eventIDStr);

    if (!errorHandler.idValidation(eventIDStr) || events.find(stoi(eventIDStr)) == events.end())
    {
        cout << "Invalid event ID.\n";
        return;
    }

    eventID = stoi(eventIDStr);

    Event &event = events[eventID];
    int vipTicketsBooked = 0;
    int regularTicketsBooked = 0;

    while (true)
    {
        cout << "\n\t\t ______________________________________________\n";
        cout << "\t\t|       |" << setw(40) << "|\n";
        cout << "\t\t| [1]   |     Buy VIP tickets" << setw(21) << "|\n";
        cout << "\t\t| [2]   |     Buy Regular tickets" << setw(16) << "|\n";
        cout << "\t\t| [0]   |     Exit" << setw(31) << "|\n";
        cout << "\t\t|_______|______________________________________|\n";

        string choice;
        while (true)
        {
            cout << "Enter your choice: ";
            getline(cin, choice);
            if (errorHandler.menuChoice(choice))
            {
                break;
            }
            cout << "\n\tInvalid option! Please try again.\n\n";
        }

        if (choice == "1" || choice == "2")
        {
            string numberOfTicketsStr;
            int numberOfTickets;

            cout << "Enter number of tickets: ";
            getline(cin, numberOfTicketsStr);

            if (!errorHandler.idValidation(numberOfTicketsStr))
            {
                cout << "Number of tickets must be a positive integer.\n";
                continue;
            }

            numberOfTickets = stoi(numberOfTicketsStr);

            if (choice == "1")
            {
                if (event.vipSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough VIP seats available. Total VIP seats left: " << event.vipSeatsQueue.size() << ".\n";
                    return;
                }

                while (vipTicketsBooked < numberOfTickets && !event.vipSeatsQueue.empty())
                {
                    event.vipSeatsQueue.pop();
                    vipTicketsBooked++;
                }

                users[username].canceledVipTickets[eventID] += vipTicketsBooked;
            }
            else if (choice == "2")
            {
                if (event.regularSeatsQueue.size() < numberOfTickets)
                {
                    cout << "Not enough Regular seats available. Total Regular seats left: " << event.regularSeatsQueue.size() << ".\n";
                    return;
                }

                while (regularTicketsBooked < numberOfTickets && !event.regularSeatsQueue.empty())
                {
                    event.regularSeatsQueue.pop();
                    regularTicketsBooked++;
                }

                users[username].canceledRegularTickets[eventID] += regularTicketsBooked;
            }

            if ((choice == "1" && vipTicketsBooked == numberOfTickets) || (choice == "2" && regularTicketsBooked == numberOfTickets))
            {
                users[username].tickets[eventID] += numberOfTickets;
                cout << "Tickets booked successfully! ";

                if (choice == "1")
                    cout << "VIP: " << vipTicketsBooked << "\n";
                else if (choice == "2")
                    cout << "Regular: " << regularTicketsBooked << "\n";

                return;
            }
        }
        else if (choice == "0")
        {
            cout << "\nExiting...\n";
            return;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
void signup()
{
    string username, password, email;

    while (true)
    {

        while (true)
        {
            cout << "\nEnter Username: ";
            getline(cin, username);
            if (errorHandler.nameValidation(username))
            {
                break;
            }
            else
            {
                cout << "\n\n\tInvalid Username. Please try again.\n";
            }
        }

        string lowerUsername = toLowerCase(username);

        bool usernameExists = false;
        for (const auto &user : users)
        {
            if (toLowerCase(user.first) == lowerUsername)
            {
                usernameExists = true;
                break;
            }
        }

        if (usernameExists)
        {
            cout << "\n\tUsername already exists. Please try again.\n";
            continue;
        }

        if (errorHandler.nameValidation(username))
        {
            break; 
        }
        else
        {
            cout << "\n\tInvalid Username. Please try again.\n";
        }
    }

    while (true)
    {
        errorHandler.passLogic(password, "Enter Password: ");
        if (errorHandler.passwordValidation(password))      
        {
            break;
        }
    }

    while (true)
    {
        cout << "\nEnter Valid Email: ";
        getline(cin, email);

        string lowerEmail = toLowerCase(email);

        bool emailExists = false;
        for (const auto &user : users)
        {
            if (toLowerCase(user.second.email) == lowerEmail)
            {
                emailExists = true;
                break;
            }
        }

        if (emailExists)
        {
            cout << "\n\tEmail already exists. Please try again.\n";
            continue;
        }

        if (errorHandler.emailValidation(email))
        {
            break;
        }
        else
        {
            cout << "\n\tInvalid Email. Please try again.\n";
        }
    }

    User newUser = {username, password, email, {}};
    users[username] = newUser;

    cout << "\n\tSignup Successfully!";
}
