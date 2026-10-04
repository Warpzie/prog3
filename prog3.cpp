// Online C++ compiler (editor)
// Write and run C++ online using this editor.

// Online C++ compiler (editor)
// Write and run C++ online using this editor.

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

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

    ~User();

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
private:
    string objednavky[];//added pre priradenie obejdnavky ku zakaznikovi
public:
    Customer(int idin, string n, string em, string pass) : User(idin, n, em, pass) {}
    string getName() {
        return name;
    }
    string getPass() {
        return password;
    }
    string getObjednavky(string name) { //pick z objednavok ??
        return objednavky(name);
    }
    void setObjednavky(Objednavka.name) {
        objednavky.append(Objednavka.name);
    }
}
class Admin : public User {
public:
    Admin(int idin, string n, string em, string pass) : User(idin, n, em, pass) {}
    string getName() {
        return name; //??
    }
    string getPass() {
        return password;
    }
}

class Objednavka {
private:
    int id;
    string name;
    string state;
    date dateOfCreation;
    string description;
public:
    //create, confirm, changeState
    string getDate() { //pre zoradenie podla datumu
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
    Objednavka(int idin, string n, string st, date dateOf, string desc) {
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
    date dateOfCreation;
    string route;
public:
    //upload, download
    string getRoute() { // pre stahovanie
        return route;
    }
    string getType() { //pre spravny sposob zobrazenie
        return type;
    }
    File(int idin, string fn, string ty, date dateOf, string rt) {
        this->id = idin;
        this->filename = fn;
        this->type = ty;
        this->dateOfCreation = dateOf;
        this->route = rt;
    }
}

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

}

int main() {

    return 0;
}
