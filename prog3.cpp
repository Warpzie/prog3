#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

class Date {
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
private:
    int id;
    string email;

public:
    //construct User
    User(int idin, string n, string em, string pass) {
        this->id = idin;
        this->name = n;
        this->email = em;
        this->password = pass;
    }

    string getName() {
        return name;
    }
    string getEmail() {
        return email;
    }
    string getPassword() {
        return password;
    }
    bool Login(string em, string pass);


};


class Customer : public User {
public:
    Customer(int idin, string n, string em, string pass) : User(idin, n, em, pass) {}
    string getName() {
        return name;
    }
    string getPass() {
        return password;
    }
};
class Admin : public User {
public:
    Admin(int idin, string n, string em, string pass) : User(idin, n, em, pass) {}
    string getName() {
        return name; //??
    }
    string getPass() {
        return password;
    }
};

class Objednavka {
private:
    int id;
    string name;
    string state;
    Date dateOfCreation;
    string description;
public:
    //create, confirm, changeState
    Date getDate() { //pre zoradenie podla datumu
        return dateOfCreation;
    }
    string getDesc() {
        return description;
    }
    string getState() {
        return state;
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

int main() {

    return 0;
}
