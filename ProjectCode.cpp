#include<iostream>
#include<cmath>
#include<windows.h>
#include<iomanip>
#include<string>
#include<ctime>
#include<sstream>
#include<fstream>
#include<thread>
#include<chrono>
#include<algorithm>
#include<map>
#include<conio.h>
#include<cctype>
#include<cstdlib>
#include<cstring> // Added for strcmp

using namespace std;

const int maxTokens = 100;
string tokens[maxTokens]; 

int split(const string& line, const string& delimiter, string tokens[], int maxTokens) {
    int count = 0;
    size_t start = 0;
    size_t end = line.find(delimiter);

    while (end != string::npos && count < maxTokens - 1) {
        tokens[count++] = line.substr(start, end - start);
        start = end + delimiter.length();
        end = line.find(delimiter, start);
    }

    if (count < maxTokens) {
        tokens[count++] = line.substr(start);
    }

    return count;
}

struct product{
    int productID;
    string productName;
    double productPrice;
    string productDescription;
    int productCategoryID;
    int productStock;
};

struct category{
    int categoryID;
    string categoryName;
};

struct User {
    int id;
    string username;
    string password;
    string address;
    string email;
    string phone;
};

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');

    if (first == string::npos || last == string::npos) {
        return "";
    }

    return str.substr(first, last - first + 1);
}

class loading{
    public:
        void displayLoadingAnimation() {
        const char* frames[] = { "|", "/", "-", "\\" }; 
        const int frameCount = 4;                    
        const int animationDuration = 10;             
    
        for (int i = 0; i < animationDuration * 2; ++i) {
            cout << "\r" << frames[i % frameCount] << " " << flush;
    
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        system("cls");
    }
};

class UserLogin; // Forward declaration

class UserRegister {
    private:
        string name, homeAddress, emailAddress;
        char* password;
        char* confirmPassword;
        char pw;
        int phoneNumber;
        int i = 0;
        int attemptCount = 0;
    
    public:
        void userRegister() {
            password = new char[21];
            confirmPassword = new char[21];
            int lastid;
            string line, lastline;
            ifstream takeuserid("register.txt");
            int takelastid;

            if (!takeuserid) {
                cout << "Error: Unable to open the file. Please try again." << endl;
                exit(1);
            }
            
            while(getline(takeuserid, line)) {
                lastline = line;
            }
            takeuserid.close();
            
            if (!lastline.empty()) {
                split(lastline, string("|"), tokens, maxTokens);
                try {
                    lastid = stoi(tokens[0]);
                } catch (exception& e) {
                    lastid = 0;
                }
            }
            lastid++;
            
            cout << "=================================================" << endl;
            cout << "||                 Register                    ||" << endl;
            cout << "=================================================" << endl;

            cout << "\nNAME              : ";
            getline(cin, name);

            cout << "PASSWORD          : ";
            i = 0;
            while (true) {
                pw = getch();
                if (pw == '\r') { 
                    password[i] = '\0';
                    break;
                } else if (pw == '\b') {
                    if (i > 0) {
                        --i;
                        cout << "\b \b";
                    }
                } else if (i < 20) {
                    password[i++] = pw;
                    cout << '*';
                }
            }

            do {
                if (attemptCount == 3) { 
                    cout << "\nToo many failed attempts. Returning to main menu...\n";
                    this_thread::sleep_for(chrono::seconds(3));
                    system("cls");
                    return;
                }
                cout << "\nCONFIRM PASSWORD  : ";
                i = 0;
                while (true) {
                    pw = getch();
                    if (pw == '\r') {
                        confirmPassword[i] = '\0';
                        break;
                    } else if (pw == '\b') {
                        if (i > 0) {
                            --i;
                            cout << "\b \b";
                        }
                    } else if (i < 20) {
                        confirmPassword[i++] = pw;
                        cout << '*';
                    }
                }

                if (strcmp(password, confirmPassword) != 0) {
                    cout << endl;
                    for (int i = 0; i < 5; ++i) {
                        cout << "x\t";
                        this_thread::sleep_for(chrono::milliseconds(500));
                    }
                    attemptCount++;
                    cout << "\nPasswords do not match. Please try again." << endl;
                }
            } while (strcmp(password, confirmPassword) != 0);
            
            cout << endl;
            for (int i = 0; i < 5; ++i) {
                SetConsoleOutputCP(CP_UTF8);
                cout << "v\t";
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            cout << "\nPassword confirmed successfully!" << endl;

            cout << "HOME ADDRESS      : ";
            getline(cin, homeAddress);
            while (true) { 
                cout << "EMAIL ADDRESS     : ";
                cin >> emailAddress;
            
                if (emailAddress.find('@') != string::npos) {
                    break; 
                } else {
                    cout << "\nInvalid email address. Please include '@' and try again!!!!!" << endl << "(Example:wonderfulpet@gmail.com)" << endl;
                }
            }
            cout << "PHONE NUMBER      : +60";
            cin >> phoneNumber;
            
            ofstream registerFile("register.txt", ios::app);

            if (!registerFile) {
                cout << "Error: Unable to open the file. Please try again." << endl;
                exit(1);
            }

            registerFile << left << setw(5) << lastid << "|" << left << setw(15) << name << "|" << setw(15) << confirmPassword << "|" << setw(60) << homeAddress << "|" << setw(30) << emailAddress << "|" << "+60" << phoneNumber << endl;
            registerFile.close();

            cout << "\nRegistration successful!" << endl;
            cin.ignore();
            
            this_thread::sleep_for(chrono::seconds(1));
            system("cls");
            
            delete[] password;
            delete[] confirmPassword;
        }
};

class UserLogin {
    protected:
        int loginuserid;
        int pagetype;
    
    private:
        bool checkCredentials(const string& fileName, const string& username, const string& password) {
            ifstream file(fileName);
            if (!file) {
                cout << "\nError: Unable to open " << fileName << "!" << endl;
                return false;
            }

            string line;
            while (getline(file, line)) {
                string fields[10];
                int numFields = split(line, "|", fields, 10);
                if (numFields >= 3) {
                    string storedUserID = fields[0];
                    string storedUsername = fields[1];
                    string storedPassword = fields[2];

                    storedUserID = trim(storedUserID);
                    storedUsername = trim(storedUsername);
                    storedPassword = trim(storedPassword);

                    if (storedUsername == username && storedPassword == password) {
                        loginuserid = stoi(storedUserID);
                        return true;
                    }
                }
            }
            return false;
        }

        void handleFailedUserLogin(const string& username) {
            ifstream file("register.txt");
            if (!file) {
                cout << "\nError: Unable to open register.txt!" << endl;
                exit(1);
            }

            string line, storedPassword, storedPhone, storedUsernames;
            while (getline(file, line)) {
                string fields[10];
                int numFields = split(line, "|", fields, 10);
                if (numFields >= 6) {
                    storedUsernames = fields[1];
                    storedUsernames = trim(storedUsernames);
                    if (storedUsernames == username) {
                        storedPassword = fields[2];
                        storedPhone = fields[5];
                        break;
                    }
                }
            }

            string inputPhone;
            cout << "\n\nToo many failed attempts. Please verify your phone number." << endl;
            cout << "Enter your registered phone number :+60";
            cin >> inputPhone;

            if ("+60" + inputPhone == storedPhone) {
                cout << "\nVerification successful! Your password is: " << storedPassword << endl;
                customerPage(username);
            } else {
                cout << "\nVerification failed. Please try again or contact our Service Center (+6012345678)." << endl;
                exit(0);
            }
        }

        void customerPage(const string& username) {
            cout << "\nWelcome, " << username << "! This is the customer page." << endl;
            pagetype = 1;
            this_thread::sleep_for(chrono::seconds(2));
            // um.user_main_page();
        }

        void managerPage(const string& username) {
            cout << "\nWelcome, " << username << "! This is the manager page." << endl;
            pagetype = 2;
            this_thread::sleep_for(chrono::seconds(2));
        }

        void staffPage(const string& username) {
            cout << "\nWelcome, " << username << "! This is the staff page." << endl;
            pagetype = 3;
            this_thread::sleep_for(chrono::seconds(2));
        }

    public:
        void login() {
            string username, inputPassword;
            int attempts = 3;

            cout << endl << "============================================================" << endl;
            cout         << "||                     USER LOGIN                         ||" << endl;
            cout         << "============================================================" << endl;

            while (attempts > 0) {
                cout << "\nUSERNAME : ";
                getline(cin, username);

                cout << "PASSWORD : ";
                char ch;
                char password[30];
                int i = 0;

                while (true) {
                    ch = getch();
                    if (ch == '\r') {
                        password[i] = '\0';
                        break;
                    } else if (ch == '\b') {
                        if (i > 0) {
                            --i;
                            cout << "\b \b";
                        }
                    } else if (i < 29) {
                        password[i++] = ch;
                        cout << '*';
                    }
                }
                inputPassword = password;

                if (authenticateUser(username, inputPassword, *this)) {
                    return;
                }

                --attempts;
                if (attempts > 0) {
                    cout << endl << endl;
                    for (int i = 0; i < 5; ++i) {
                        cout << "x\t";
                        this_thread::sleep_for(chrono::milliseconds(500));
                    }
                    cout << "\n\nInvalid username or password. You have " << attempts << " attempt(s) remaining.\n";
                } else if (isUserAccount(username, *this)) {
                    handleFailedUserLogin(username);
                    return;
                } else {
                    cout << "\n\nPlease register an account or contact our Service Center (+6012345678)" << endl;
                    exit(0);
                }
            }
        }
        
        int getpagetype(){
        	return pagetype;
		}

        friend bool authenticateUser(const string& username, const string& password, UserLogin& ul);
        friend bool isUserAccount(const string& username, UserLogin& ul);
};

bool isUserAccount(const string& username, UserLogin& ul) {
    ifstream file("register.txt");
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        string fields[10];
        int numFields = split(line, "|", fields, 10);
        if (numFields > 1 && !fields[1].empty()) {
            string storedUsername = trim(fields[1]);
            if (storedUsername == username) return true;
        }
    }
    return false;
}

bool authenticateUser(const string& username, const string& password, UserLogin& ul) {
    if (ul.checkCredentials("register.txt", username, password)) {
        ul.customerPage(username);
        return true;
    } else if (ul.checkCredentials("manager.txt", username, password)) {
        ul.managerPage(username);
        return true;
    } else if (ul.checkCredentials("staff.txt", username, password)) {
        ul.staffPage(username);
        return true;
    }
    return false;
}

template<typename T>
class ADTstackA {
    private:
        struct node {
            T data;
            node* next;
        };
        node* top;

    public:
        ADTstackA() : top(nullptr) {}

        bool empty() {
            return top == nullptr;
        }

        void push(T item) {
            node* temp = new node;
            temp->data = item;
            temp->next = top;
            top = temp;
        }

        T pop() {
            if (empty()) {
                cout << "Stack is Empty" << endl;
                return T();
            }
            node* temp = top;
            T item = top->data;
            top = top->next;
            delete temp;
            return item;
        }

        T peek() {
            if (empty()) return T();
            return top->data;
        }

        void reverse(ADTstackA<T>& reversedStack) {
            ADTstackA<T> temp;
            while (!empty()) {
                temp.push(pop());
            }
            while (!temp.empty()) {
                T item = temp.pop();
                reversedStack.push(item);
                push(item);
            }
        }

        void clear() {
            while (!empty()) {
                pop();
            }
        }

        void traverse(void (*func)(T)) {
            ADTstackA<T> temp;
            while (!empty()) {
                T item = pop();
                func(item);
                temp.push(item);
            }
            while (!temp.empty()) {
                push(temp.pop());
            }
        }

        template<typename Func>
        void Functraverse(Func func) {
            ADTstackA<T> temp;
            while (!empty()) {
                T item = pop();
                func(item);
                temp.push(item);
            }
            while (!temp.empty()) {
                push(temp.pop());
            }
        }

        void writeToFile(ofstream& file) {
            ADTstackA<T> temp;
            file << left << setw(3) << "id" << "|" << left << setw(31) << "name" << "|" << left << setw(5) << "price" << "|" << left << setw(5) << "categoryId" << "|" << left << setw(5) << "quantity" << "|" << left << setw(20) << "description" << "\n";
            while (!empty()) {
                T item = pop();
                file << left << setw(3) << item->id << "|" << left << setw(31) << item->name << "|" << left << setw(5) << item->price << "|" << left << setw(5) << item->categoryId << "|" << left << setw(5) << item->quantity << "|" << left << setw(20) << item->description << "\n";
                temp.push(item);
            }
            while (!temp.empty()) {
                push(temp.pop());
            }
        }
};

class Category {
    private:
        static const int HASH_SIZE = 100;
        Category* categoryList = nullptr;
        Category* ChashTable[HASH_SIZE] = { nullptr };

    public:
        int id;
        string name;
        Category* mainNext;
        Category* hashNext;

        Category() : id(0), name(""), mainNext(nullptr), hashNext(nullptr) {}
        
        void loadCategory() {
            ifstream file("category.txt");
            string line;
            getline(file, line); // Skip header
            while (getline(file, line)) {
                stringstream ss(line);
                string token;
                Category* newCategory = new Category();

                getline(ss, token, '|');
                newCategory->id = stoi(token);
                getline(ss, newCategory->name, '|');

                newCategory->mainNext = categoryList;
                categoryList = newCategory;

                int index = newCategory->id % HASH_SIZE;
                newCategory->hashNext = ChashTable[index];
                ChashTable[index] = newCategory;
            }
            file.close();
        }
        
        void displayCategoryFromTXT() {
            Category* Ctemp = categoryList;

            cout << "+----+-----------------------------------------+" << endl;
            cout << "+ ID + Category Name                           +" << endl;
            cout << "+----+-----------------------------------------+" << endl;

            while (Ctemp) {
                cout << "| " << setw(2) << Ctemp->id << " | " << setw(39) << Ctemp->name << "|" << endl;
                Ctemp = Ctemp->mainNext;
            }
            cout << "+----+-----------------------------------------+" << endl;
        }
};

class Product {
    private:
        static const int HASH_SIZE = 100;
        Product* productList = nullptr;
        Product* hashTable[HASH_SIZE] = { nullptr };
        ADTstackA<Product*> productStack;
        
    public:
        int id;
        string name;
        int categoryId;
        double price;
        int quantity;
        string description;
        Product* mainNext;
        Product* hashNext;

        Product() : id(0), name(""), categoryId(0), price(0.0), quantity(0), description(""), mainNext(nullptr), hashNext(nullptr) {}
        
        void clearProducts() {
            productStack.clear();
            Product* current = productList;
            while (current) {
                Product* toDelete = current;
                current = current->mainNext;
                delete toDelete;
            }
            productList = nullptr;
            for (int i = 0; i < HASH_SIZE; ++i) hashTable[i] = nullptr;
        }
        
        void loadProducts() {
            ifstream file("product.txt");
            string line;
            getline(file, line); // Skip header
            while (getline(file, line)) {
                stringstream ss(line);
                string token;
                Product* newProduct = new Product();

                getline(ss, token, '|');
                newProduct->id = stoi(token);
                getline(ss, newProduct->name, '|');
                getline(ss, token, '|');
                newProduct->price = stod(token);
                getline(ss, token, '|');
                newProduct->categoryId = stoi(token);
                getline(ss, token, '|');
                newProduct->quantity = stoi(token);
                getline(ss, newProduct->description);

                newProduct->mainNext = productList;
                productList = newProduct;
                productStack.push(newProduct);

                int index = newProduct->id % HASH_SIZE;
                newProduct->hashNext = hashTable[index];
                hashTable[index] = newProduct;
            }
            file.close();
        }
        
        void sortStackByField(const string& field, bool ascending) {
            ADTstackA<Product*> sortedStack;

            while (!productStack.empty()) {
                Product* temp = productStack.pop();

                ADTstackA<Product*> tempStack;
                while (!sortedStack.empty()) {
                    Product* top = sortedStack.peek();
                    bool condition = false;

                    if (field == "id")
                        condition = ascending ? (top->id > temp->id) : (top->id < temp->id);
                    else if (field == "price")
                        condition = ascending ? (top->price > temp->price) : (top->price < temp->price);
                    else if (field == "quantity")
                        condition = ascending ? (top->quantity > temp->quantity) : (top->quantity < temp->quantity);

                    if (!condition) break;

                    tempStack.push(sortedStack.pop());
                }

                sortedStack.push(temp);

                while (!tempStack.empty()) {
                    sortedStack.push(tempStack.pop());
                }
            }

            while (!sortedStack.empty()) {
                productStack.push(sortedStack.pop());
            }
        }
        
        static void displaySingleProduct(Product* p) {
		    if (p) {
		        cout << "| " << setw(2) << p->id << " | " << setw(39) << p->name << " | " 
		             << setw(6) << fixed << setprecision(2) << p->price << " | " 
		             << setw(8) << p->categoryId << " | " << setw(8) << p->quantity << " | " 
		             << setw(20) << p->description << " |" << endl;
		    }
		}
		
		void displayProductsFromStack() {
	        cout << "\n=== Product List ===\n";
	        cout << "\n+----+-----------------------------------------+--------+----------+----------+----------------------+\n";
	        cout << "| ID | Product Name                            | Price  | Category | Quantity | Description          |\n";
	        cout << "+----+-----------------------------------------+--------+----------+----------+----------------------+\n";
	
	        productStack.traverse(displaySingleProduct);  // ?????????
	        cout << "+----+-----------------------------------------+--------+----------+----------+----------------------+\n";
	        cout << "Press Enter to Continue...";
	        cin.ignore();
	        cin.get();
	    }
        
        void addProduct() {
            Product* newProduct = new Product();
            cout << "Enter Product ID: ";
            cin >> newProduct->id;

            if (productIdExists(newProduct->id)) {
                cout << "Error: Product ID already exists in stack.\n";
                delete newProduct;
                cout << "Press Enter to Continue...";
                cin.ignore();
                cin.get();
                return;
            }

            cout << "Enter Product Name: ";
            cin.ignore();
            getline(cin, newProduct->name);
            
            Category c;
            c.displayCategoryFromTXT();
            
            cout << "Enter Category ID: ";
            cin >> newProduct->categoryId;
            cout << "Enter Price: $";
            cin >> newProduct->price;
            cout << "Enter Stock: ";
            cin >> newProduct->quantity;
            cout << "Enter Product Description: ";
            cin.ignore();
            getline(cin, newProduct->description);

            productStack.push(newProduct);

            ofstream file("product.txt", ios::app);
            if (file.is_open()) {
                file << left << setw(3) << newProduct->id << "|" << left << setw(31) << newProduct->name << "|" 
                     << left << setw(5) << fixed << setprecision(2) << newProduct->price << "|" 
                     << left << setw(5) << newProduct->categoryId << "|" 
                     << left << setw(5) << newProduct->quantity << "|" 
                     << left << setw(20) << newProduct->description << endl;
                file.close();
            } else {
                cout << "Error opening product.txt for writing.\n";
            }
            
            cout << "Press Enter to Continue...";
            cin.get();
        }

        bool productIdExists(int idToCheck) {
            bool exists = false;
            productStack.Functraverse([&](Product* p) {
                if (p->id == idToCheck) {
                    exists = true;
                }
            });
            return exists;
        }

        void searchProduct() {
            int categoryId;
            string field;
            char dir;

            system("cls");
            Category c;
            c.displayCategoryFromTXT();
            cout << "Enter Category ID to filter: ";
            cin >> categoryId;

            ADTstackA<Product*> filteredStack;
            
            productStack.Functraverse([&](Product* p) {
                if (p->categoryId == categoryId) {
                    filteredStack.push(p);
                }
            });

            if (filteredStack.empty()) {
                cout << "No products found under Category ID " << categoryId << ".\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }

            while (true) {
                cout << "Sort by (id/price/quantity): ";
                cin >> field;
                if (field == "id" || field == "price" || field == "quantity") {
                    while (true) {
                        cout << "Ascending (a) or Descending (d)? ";
                        cin >> dir;
                        if (dir == 'a' || dir == 'd') {
                            sortStackByField(field, dir == 'a');

                            cout << "\n=== Filtered & Sorted Product List ===\n";
                            cout << "\n+----+-----------------------------------------+--------+----------+----------+----------------------+\n";
                            cout << "| ID | Product Name                            | Price  | Category | Quantity | Description          |\n";
                            cout << "+----+-----------------------------------------+--------+----------+----------+----------------------+\n";
                            filteredStack.traverse(displaySingleProduct);
                            cout << "+----+-----------------------------------------+--------+----------+----------+----------------------+\n";
                            cout << "Press Enter to continue...";
                            cin.ignore();
                            cin.get();
                            return;
                        } else {
                            cout << "Invalid input. Please enter 'a' or 'd'.\n";
                            cin.ignore();
                            cin.get();
                        }
                    }
                } else {
                    cout << "Invalid field. Please enter 'id', 'price' or 'quantity'.\n";
                    cin.ignore();
                    cin.get();
                }
            }
        }
    
};

class UserModule {
    private:
        Product p;
    public:
        void user_menu() {
            cout << string(28, '-') << endl;
            cout << setw(20) << "User Dashboard" << endl;
            cout << string(28, '-') << endl;
            cout << "1. Display Product" << endl;
            cout << "2. Shopping Cart" << endl;
            cout << "3. Order History" << endl;
            cout << "4. Profile" << endl;
            cout << "0. Logout" << endl;
        }
        
        void user_main_page() {
            int choice;
            do {
                system("cls");
                user_menu();
                cout << "Please Select the option: ";
                cin >> choice;
                switch (choice) {
                    case 1: 
                        p.loadProducts();
                        p.displayProductsFromStack(); 
                        break;
                    case 2: 
                        cout << "Shopping Cart feature not implemented yet." << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        break;
                    case 3: 
                        cout << "Order History feature not implemented yet." << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        break;
                    case 4: 
                        cout << "Profile feature not implemented yet." << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        break;
                    case 0: 
                        cout << "Logging out..." << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        break;
                    default: 
                        cout << "Invalid option!" << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                }
            } while (choice != 0);
        }
};

int main() {
    UserRegister r;
    UserLogin l;
    int choice_menu;
    
    do {
        system("cls");
        cout << string(28, '-') << endl;
        cout << setw(20) << "Menu" << endl;
        cout << string(28, '-') << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "0. Exit Program" << endl;
        cout << string(28, '-') << endl;
        cout << "Please select the module : ";
        cin >> choice_menu;
        cin.ignore();
        
        if (choice_menu == 1) {
            system("cls");
            l.login();
            
            // After successful login
            if (l.getpagetype() == 1) { // Customer
                UserModule um;
                um.user_main_page();
            }
            // Add other user types here if needed
        }
        else if (choice_menu == 2) {
            system("cls");
            r.userRegister();
        }
        else if (choice_menu == 0) {
            cout << "Thank you! See you next time." << endl;
            break;
        }
        else {
            system("cls");
            cout << "Invalid input. Please select again." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while (true);
    
    return 0;
}