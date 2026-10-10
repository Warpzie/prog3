#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;
//niektore classy nie su implementovane ale su tu kvoli tomu ze boli v triednom diagrame, niektore su nove kvoli funkcianalitam v hlavnom scenari
//admin@gmail.com, admin
//test, test
class Date { //uhh
private:
    std::chrono::system_clock::time_point tp;
public:
    Date() : tp(std::chrono::system_clock::now()) {}
    explicit Date(const std::chrono::system_clock::time_point& t) : tp(t) {}
    static Date today() { return Date(std::chrono::system_clock::now()); }
    std::string toString() const {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::localtime(&t);
        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d");
        return ss.str();
    }
    bool operator<(const Date& other) const { return tp < other.tp; }
};

inline std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.toString();
    return os;
}

class User
{
protected:
    string name;
    string password;
    string role; // "customer" or "admin"
public:
    int id;
    string email;

    //construct User
    User(int idin, string n, string em, string pass, string r = "customer") {
        this->id = idin;
        this->name = n;
        this->email = em;
        this->password = pass;
        this->role = r;
    }

    string getName() const {
        return name;
    }
    string getEmail() const {
        return email;
    }
    string getPassword() const {
        return password;
    }
    string getRole() const {
        return role;
    }
    bool Login(string em, string pass);


};


class Customer : public User {
public:
    Customer(int idin, string n, string em, string pass) : User(idin, n, em, pass, "customer") {}
    string getName() {
        return name;
    }
    string getPass() {
        return password;
    }
    // Customer permissions
    bool canCreateOrder() { return true; }
    bool canViewOrder(int orderCreatorId) { return orderCreatorId == this->id; } // len svoje orders
    bool canUploadFiles() { return true; }
    bool canReview() { return true; }
};

class Admin : public User {
public:
    Admin(int idin, string n, string em, string pass) : User(idin, n, em, pass, "admin") {}
    string getName() {
        return name; //??
    }
    string getPass() {
        return password;
    }
    // Admin permissions
    bool canViewAllOrders() { return true; }
    bool canConfirmOrder() { return true; }
    bool canRejectOrder() { return true; }
    bool canManageCustomers() { return true; }
    bool canEditOrder(int orderId) { return true; } // can edit any order
    bool canUploadProgress() { return true; }
};

class Objednavka {
private:
    int id;
    string name;
    string state;
    Date dateOfCreation;
    string description;
    string zadavatel; //added pre priradenie objednavky k uzivatelovi
public:

    Date getDate() const { //pre zoradenie podla datumu
        return dateOfCreation;
    }
    string getDesc() const {
        return description;
    }
    string getState() const {
        return state;
    }
    int getId() const {
        return id;
    }
    void setState(string st) {
        state = st;
    }
    Objednavka(int idin, string n, string st, Date dateOf, string desc) {
        this->id = idin;
        this->name = n;
        this->state = st;
        this->dateOfCreation = dateOf;
        this->description = desc;
    }
};



class File : public Objednavka {
private:
    int id;
    string filename;
    string type;
    Date dateOfCreation;
    string route;
public:
    //upload, download
    string getRoute() { // pre stahovanie
        return route;
    }
    string getType() { //pre spravny sposob zobrazenie
        return type;
    }
    File(int idin, string fn, string ty, Date dateOf, string rt) : Objednavka(idin, fn, "", dateOf, "") {
        this->id = idin;
        this->filename = fn;
        this->type = ty;
        this->dateOfCreation = dateOf;
        this->route = rt;
    }
};

class Artwork {
private:
    int id;
    string state;
    string category;
public:
    //create, edit, delete
    string getCategory() { // pre filtrovanie podla kategorie
        return category;
    }
    string getState() {
        return state;
    }
    void setState(string st) {
        state = st;
    }
    Artwork(int idin, string st, string cat) {
        this->id = idin;
        this->state = st;
        this->category = cat;
    }

};

// ==================== OrderManager ====================
class OrderManager {
private:
    vector<Objednavka> orders;
    string ordersFile = "orders.txt";

    // helper na rozdelenie stringu
    vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    OrderManager() {
        loadOrdersFromFile();
    }

    void loadOrdersFromFile() {
        ifstream file(ordersFile);
        if (!file.is_open()) {
            cout << "No existing orders file. Starting fresh.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> parts = split(line, '|');
            if (parts.size() >= 5) {
                int id = stoi(parts[0]);
                string name = parts[1];
                string state = parts[2];
                Date dateOf; // simplified, can be enhanced
                string desc = parts[4];
                orders.push_back(Objednavka(id, name, state, dateOf, desc));
            }
        }
        file.close();
        cout << "Loaded " << orders.size() << " orders from file.\n";
    }

    void saveOrdersToFile() {
        ofstream file(ordersFile);
        if (!file.is_open()) {
            cerr << "Error: Could not open orders file for writing.\n";
            return;
        }

        for (const auto& order : orders) {
            file << order.getDesc() << "|" << order.getState() << "\n";
        }
        file.close();
    }

    // vytvorenie orderu - mozu len customeri a admin
    bool createOrder(User* user, string orderName, string description) {
        if (user->getRole() != "customer" && user->getRole() != "admin") {
            cout << "Error: Only customers and admins can create orders.\n";
            return false;
        }

        int newId = orders.size() + 1;
        Objednavka newOrder(newId, orderName, "pending", Date::today(), description);
        orders.push_back(newOrder);
        saveOrdersToFile();
        cout << "Order created successfully (ID: " << newId << "). Awaiting admin confirmation.\n";
        return true;
    }

    // potvrdit order - len admin
    bool confirmOrder(User* user, int orderId) {
        if (user->getRole() != "admin") {
            cout << "len admin moze confirmovat orders\n";
            return false;
        }

        for (auto& order : orders) {
            if (order.getId() == orderId) {
                order.setState("confirmed");
                saveOrdersToFile();
                cout << "order #" << orderId << " confirmed\n";
                return true;
            }
        }
        cout << "Error: Order not found.\n";
        return false;
    }

    // zobrazenie orderov - customer vid len svoje, admin vidi vsetky
    void viewOrders(User* user) {
        if (orders.empty()) {
            cout << "No orders found.\n";
            return;
        }

        cout << "\n=== orders ===\n";
        for (const auto& order : orders) {
            cout << "  - " << order.getDesc() << " [" << order.getState() << "]\n";
        }
    }

    // zmena stavu orderu - len admin
    bool updateOrderState(User* user, int orderId, string newState) {
        if (user->getRole() != "admin") {
            cout << "Error: Only admins can update order state.\n";
            return false;
        }

        for (auto& order : orders) {
            if (order.getId() == orderId) {
                order.setState(newState);
                saveOrdersToFile();
            cout << "Order state updated to: " << newState << "\n";
                return true;
            }
        }
        cout << "Error: Order not found.\n";
        return false;
    }

    int getTotalOrders() const {
        return orders.size();
    }
};


class AuthManager {
private:
    vector<User> users;
    string usersFile = "users.txt";

    // Helper: split string by delimiter
    vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    AuthManager() {
        loadUsersFromFile();
    }

    // Load users from file on startup
    void loadUsersFromFile() {
        ifstream file(usersFile);
        if (!file.is_open()) {
            cout << "no users file, starting new one\n";
            return;
        }

        string line;
        int lineNum = 0;
        while (getline(file, line)) {
            lineNum++;
            if (line.empty()) continue;

            vector<string> parts = split(line, '|');
            if (parts.size() >= 5) {
                int id = stoi(parts[0]);
                string name = parts[1];
                string email = parts[2];
                string password = parts[3];
                string role = parts[4];
                users.push_back(User(id, name, email, password, role));
            }
        }
        file.close();
        cout << "Loaded " << users.size() << " users from file.\n";
    }

    // ulozenie userov do suboru
    void saveUsersToFile() {
        ofstream file(usersFile);
        if (!file.is_open()) {
            cerr << "Error: Could not open users file for writing.\n";
            return;
        }

        for (const auto& user : users) {
            file << user.id << "|" << user.getName() << "|" << user.getEmail() << "|" << user.getPassword() << "|" << user.getRole() << "\n";
        }
        file.close();
    }

    // registracia noveho usera s rolou
    bool registerUser(int id, string name, string email, string password, string role = "customer") {
        // check role
        if (role != "customer" && role != "admin") {
            cout << "Error: Invalid role. Use 'customer' or 'admin'.\n";
            return false;
        }

        // check ci uz existuje admin
        if (role == "admin") {
            for (const auto& user : users) {
                if (user.getRole() == "admin") {
                    cout << "Error: An admin already exists. Only one admin allowed.\n";
                    return false;
                }
            }
        }

        // check ci email uz existuje
        for (const auto& user : users) {
            if (user.getEmail() == email) {
                cout << "Error: Email already registered.\n";
                return false;
            }
        }

        // Validate input
        if (name.empty() || email.empty() || password.empty()) {
            cout << "Error: Name, email, and password cannot be empty.\n";
            return false;
        }

        // pridat noveho usera
        users.push_back(User(id, name, email, password, role));
        saveUsersToFile();
        cout << "User registered successfully as " << role << "!\n";
        return true;
    }

    // prihlasenie - return user ak sa podarilo
    User* loginUser(string email, string password) {
        for (auto& user : users) {
            if (user.getEmail() == email && user.getPassword() == password) {
                cout << "Login successful! Welcome, " << user.getName() << ".\n";
                return &user;
            }
        }
        cout << "Error: Invalid email or password.\n";
        return nullptr;
    }

    // vyhladanie usera podla emailu
    User* getUserByEmail(string email) {
        for (auto& user : users) {
            if (user.getEmail() == email) {
                return &user;
            }
        }
        return nullptr;
    }

    // vypis vsetkych userov
    void listAllUsers() {
        if (users.empty()) {
            cout << "No users registered.\n";
            return;
        }
        cout << "Registered users:\n";
        for (const auto& user : users) {
            cout << "  - " << user.getName() << " (" << user.getEmail() << ")\n";
        }
    }
};

int main() {
    AuthManager auth;
    OrderManager orders;

    int choice = 0;
    do {
        cout << "\n=== Login System ===" << endl;
        cout << "1. Register\n2. Login\n3. List Users\n4. Exit\n";
        cout << "Choose: ";
        cin >> choice;
        cin.ignore(); // vycisti buffer

        if (choice == 1) {
            // registracia
            string name, email, password, roleInput;
            cout << "Enter name: ";
            getline(cin, name);
            cout << "Enter email: ";
            getline(cin, email);
            cout << "Enter password: ";
            getline(cin, password);
            cout << "Register as (customer/admin): ";
            getline(cin, roleInput);

            int newId = 1;
            auth.registerUser(newId, name, email, password, roleInput);
        }
        else if (choice == 2) {
            // Login
            string email, password;
            cout << "email: ";
            getline(cin, email);
            cout << "password: ";
            getline(cin, password);

            User* loggedInUser = auth.loginUser(email, password);
            if (loggedInUser) {
                cout << "Logged in as: " << loggedInUser->getName() << " (" << loggedInUser->getRole() << ")\n";

                // menu podla role
                int userChoice = 0;
                do {
                    cout << "\n=== " << (loggedInUser->getRole() == "customer" ? "customer menu" : "admin menu") << " ===" << endl;

                    if (loggedInUser->getRole() == "customer") {
                        cout << "1. Create Order\n2. View My Orders\n3. Logout\n";
                        cout << "Choose: ";
                        cin >> userChoice;
                        cin.ignore();

                        if (userChoice == 1) {
                            string orderName, description;
                            cout << "Enter order name: ";
                            getline(cin, orderName);
                            cout << "Enter description: ";
                            getline(cin, description);
                            orders.createOrder(loggedInUser, orderName, description);
                        }
                        else if (userChoice == 2) {
                            orders.viewOrders(loggedInUser);
                        }
                        else if (userChoice == 3) {
                            cout << "bye!\n";
                        }
                        else {
                            cout << "Invalid choice.\n";
                        }
                    }
                    else if (loggedInUser->getRole() == "admin") {
                        cout << "1. create order\n2.view all orders\n3. confirm order\n4. change order state\n5. manage customers\n6. logout\n";
                        cout << "choose: ";
                        cin >> userChoice;
                        cin.ignore();

                        if (userChoice == 1) {
                            string orderName, description;
                            cout << "Enter order name: ";
                            getline(cin, orderName);
                            cout << "Enter description: ";
                            getline(cin, description);
                            orders.createOrder(loggedInUser, orderName, description);
                        }
                        else if (userChoice == 2) {
                            orders.viewOrders(loggedInUser);
                        }
                        else if (userChoice == 3) {
                            int orderId;
                            cout << "Enter order ID to confirm: ";
                            cin >> orderId;
                            cin.ignore();
                            orders.confirmOrder(loggedInUser, orderId);
                        }
                        else if (userChoice == 4) {
                            int orderId;
                            string newState;
                            cout << "Enter order ID: ";
                            cin >> orderId;
                            cin.ignore();
                            cout << "Enter new state (in_progress/completed): ";
                            getline(cin, newState);
                            orders.updateOrderState(loggedInUser, orderId, newState);
                        }
                        else if (userChoice == 5) {
                            auth.listAllUsers();
                        }
                        else if (userChoice == 6) {
                            cout << "bye!\n";
                        }
                        else {
                            cout << "Invalid choice.\n";
                        }
                    }
                } while (userChoice != (loggedInUser->getRole() == "customer" ? 3 : 6));
            }
        }
        else if (choice == 3) {
            // zobraz userov
            auth.listAllUsers();
        }
        else if (choice == 4) {
            cout << "bye!\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 4);

    return 0;
}
