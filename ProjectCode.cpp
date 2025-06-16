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

//chatgpt // help to maximize the data, make sure not out of box
string truncateString(const string& str, int width) {
    if (str.length() <= width) {
        return str;
    } else {
        return str.substr(0, width - 3) + "...";
    }
}

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

struct Admin {
    int id;
    char username[50];
    char password[50];
};

struct Order {
    int orderId;
    int customerId;
    int productId;
    int quantity;
    double totalPrice;
    int status; // 0=pending, 1=completed
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
            pagetype = 2;
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
        
        void clearCategory() {
		    Category* current = categoryList;
		    while (current) {
		        Category* toDelete = current;
		        current = current->mainNext;
		        delete toDelete;
		    }
		    categoryList = nullptr;
		
		    for (int i = 0; i < HASH_SIZE; ++i) {
		        ChashTable[i] = nullptr;
		    }
		}
        
        void loadCategory() {
        	clearCategory();
        	
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
        
        //display the category for customers select whitch categort that want to display
        int displayCategoryAndChoose() {
		    system("cls");
		    Category* Ctemp = categoryList;
		
		    cout << "=== Category List ===\n";
		    cout << "+----+-----------------------------------------+\n";
		    cout << "| ID | Category Name                           |\n";
		    cout << "+----+-----------------------------------------+\n";
		    cout << "|  0 |                             Display All |\n";
		
		    while (Ctemp) {
		        cout << "| " << setw(2) << Ctemp->id << " | " << setw(39) << Ctemp->name << " |\n";
		        Ctemp = Ctemp->mainNext;
		    }
		    cout << "+----+-----------------------------------------+\n";
		
		    int choose;
		    cout << "Enter the category ID that you want to find: ";
		    cin >> choose;
		    
		    return choose;
		}
};

class Product {
    private:
        static const int HASH_SIZE = 100;
        Product* productList = nullptr;
        Product* hashTable[HASH_SIZE] = { nullptr };
//        ADTstackA<Product*> productStack;
        
    public:
    	ADTstackA<Product*> productStack;
    	
        int id;
        string name;
        int categoryId;
        double price;
        int quantity;
        string description;
        Product* mainNext;
        Product* hashNext;
        
        // save shopping cart product
        int product_choice;
		int product_quantity;
		double product_totalprice;
		int loginuserid = 0;

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
        	clearProducts();
        	
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
        
        // turn the product name to lower
        string toLower(const string& s) {
	        string result = s;
	        for (size_t i = 0; i < result.length(); i++) {
	            result[i] = tolower(result[i]);
	        }
	        return result;
	    }
        
        //merge sort // can write having the helps form gpt
        void mergeSortByMode(Product** arr, int left, int right, int mode) {
		    if (left >= right) return;
		    int mid = (left + right) / 2;
		    mergeSortByMode(arr, left, mid, mode);
		    mergeSortByMode(arr, mid + 1, right, mode);
		    mergeByMode(arr, left, mid, right, mode);
		}
		
		void mergeByMode(Product** arr, int left, int mid, int right, int mode) {
		    int n1 = mid - left + 1;
		    int n2 = right - mid;
		    Product** L = new Product*[n1];
		    Product** R = new Product*[n2];
		    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
		    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];
		
		    int i = 0, j = 0, k = left;
		    while (i < n1 && j < n2) {
		        bool condition = false;
		        if (mode == 1) condition = toLower(L[i]->name) <= toLower(R[j]->name);
				else if (mode == 2) condition = toLower(L[i]->name) >= toLower(R[j]->name);
		        else if (mode == 3) condition = L[i]->price <= R[j]->price;
		        else if (mode == 4) condition = L[i]->price >= R[j]->price;
		
		        if (condition)
		            arr[k++] = L[i++];
		        else
		            arr[k++] = R[j++];
		    }
		    while (i < n1) arr[k++] = L[i++];
		    while (j < n2) arr[k++] = R[j++];
		
		    delete[] L;
		    delete[] R;
		}
        
        static void displaySingleProduct(Product* p) {
        	string desc = truncateString(p->description, 26);
        	
		    if (p) {
		        cout << "| " << setw(2) << p->id << " | " << left << setw(39) << p->name << " | " 
				     << right << setw(6) << fixed << setprecision(2) << p->price << " | " 
				     << setw(8) << p->categoryId << " | " << setw(8) << p->quantity << " | " 
				     << setw(20) << desc << " |" << endl;
		    }
		}
		
		//let user select the sort mode
		void displaySortMenu(int& mode) {
		    cout << "============================================================" << endl;
		    cout << "||                     Sort Product                       ||" << endl;
		    cout << "============================================================\n" << endl;
		    cout << "Sort products by:" << endl;
		    cout << "1. Name (A to Z)" << endl;
		    cout << "2. Name (Z to A)" << endl;
		    cout << "3. Price (Low to High)" << endl;
		    cout << "4. Price (High to Low)" << endl;
		    cout << "Enter your choice (1-4): ";
		    cin >> mode;
		
		    while (mode < 1 || mode > 4) {
		        cout << "Invalid choice. Please enter 1-4: ";
		        cin >> mode;
		    }
		}
		
		//display the all product
		void displayProductsFromStack() {
		    system("cls");
		
		    const int MAX = 1000;
		    Product* arr[MAX];
		    int count = 0;
		
		    productStack.Functraverse([&](Product* p) {
		        arr[count++] = p;
		    });
		
		    if (count == 0) {
		        cout << "No products found under this category." << endl;
		        cout << "Press Enter to continue...";
		        cin.ignore();
		        cin.get();
		        return;
		    }
		
		    int mode;
		    displaySortMenu(mode);
		    mergeSortByMode(arr, 0, count - 1, mode);
		
		    cout << "=== Product List ===\n";
		    cout << "\n+----+-----------------------------------------+--------+----------+----------+----------------------------+\n";
		    cout << "| ID | Product Name                            | Price  | Category | Quantity | Description                |\n";
		    cout << "+----+-----------------------------------------+--------+----------+----------+----------------------------+\n";
		
		    for (int i = 0; i < count; ++i) {
		        displaySingleProduct(arr[i]);
		    }
		
		    cout << "+----+-----------------------------------------+--------+----------+----------+----------------------------+\n";
		    addToCart();
		}
        
        //display the category that user selected
        void displayProductsByCategory(int categoryId) {
		    const int MAX = 1000;
		    Product* arr[MAX];
		    int count = 0;
		
		    productStack.Functraverse([&](Product* p) {
		        if (p->categoryId == categoryId) {
		            arr[count++] = p;
		        }
		    });
		
		    if (count == 0) {
		        cout << "No products found under this category." << endl;
		        cout << "Press Enter to continue...";
		        cin.ignore();
		        cin.get();
		        return;
		    }
		
		    int mode;
		    displaySortMenu(mode);
		
		    mergeSortByMode(arr, 0, count - 1, mode);
		
		    system("cls");
		    cout << "=== Sorted Products in Category ID: " << categoryId << " ===\n";
		    cout << "+----+-----------------------------------------+--------+----------+----------+----------------------------+\n";
		    cout << "| ID | Product Name                            | Price  | Category | Quantity | Description                |\n";
		    cout << "+----+-----------------------------------------+--------+----------+----------+----------------------------+\n";
		    for (int i = 0; i < count; ++i) {
		        displaySingleProduct(arr[i]);
		    }
		    cout << "+----+-----------------------------------------+--------+----------+----------+----------------------------+\n";
		    addToCart();
		}
        
        //add the item to the shopping cart
        void addToCart() {
		    while (true) {
		        int pid, qty;
		
		        cout << "\nEnter the Product ID to add to cart: ";
		        cin >> pid;
		        cout << "Enter quantity: ";
		        cin >> qty;
		
				// check is the shopping cart having the record, if have add the quantity
		        Product* current = productList;
		        Product* selected = nullptr;
		        while (current != nullptr) {
		            if (current->id == pid) {
		                selected = current;
		                break;
		            }
		            current = current->mainNext;
		        }
		
				//if product id not found
		        if (selected == nullptr) {
		            cout << "❌ Product ID not found.\n";
		        } 
				else if (qty <= 0 || qty > selected->quantity) {
		            cout << "❌ Invalid quantity. Available: " << selected->quantity << "\n";
		        } 
				else {
					//read the line and add product into shopping cart
		            ifstream fin("shoppingcart.txt");
					string lines[1000];
					int lineCount = 0;
					
					bool fileEmpty = true;
					string line;
					while (getline(fin, line)) {
					    if (line.length() > 1) {
					        fileEmpty = false;
					        lines[lineCount++] = line;
					    }
					}
					fin.close();
					
					//Find out if the user has already added the same product
					bool found = false;
					for (int i = 1; i < lineCount; ++i) { // Skip header
					    stringstream ss(lines[i]);
					    string token;
					    int cartID, cusID, productID, quantity, status;
					    float total;
					
					    getline(ss, token, '|'); cartID = stoi(token);
					    getline(ss, token, '|'); cusID = stoi(token);
					    getline(ss, token, '|'); productID = stoi(token);
					    getline(ss, token, '|'); quantity = stoi(token);
					    getline(ss, token, '|'); total = stof(token);
					    getline(ss, token);      status = stoi(token);
					
					    if (cusID == loginuserid && productID == selected->id) {
					        quantity += qty;
					        total = selected->price * quantity;
					
					        // Rebuild line
					        ostringstream updated;
					        updated << cartID << "|" << cusID << "|" << productID << "|"
					                << quantity << "|" << fixed << setprecision(2) << total << "|" << status;
					        lines[i] = updated.str();
					        found = true;
					        break;
					    }
					}
					
					//if not found add a new record
					if (!found) {
					    int nextCartID = 1;
					    for (int i = 1; i < lineCount; ++i) {
					        stringstream ss(lines[i]);
					        string token;
					        getline(ss, token, '|');
					        int tempID = stoi(token);
					        if (tempID >= nextCartID) nextCartID = tempID + 1;
					    }
					
					    ostringstream newline;
					    newline << nextCartID << "|" << loginuserid << "|" << selected->id << "|"
					            << qty << "|" << fixed << setprecision(2) << selected->price * qty << "|0";
					    lines[lineCount++] = newline.str();
					}
					
					ofstream fout("shoppingcart.txt");
					fout << "Cart ID|Cus ID|Product ID|QTY|Total Price|Status\n";
					for (int i = 1; i < lineCount; ++i) {
					    fout << lines[i] << "\n";
					}
					fout.close();
					
					cout << "Item added to cart successfully.\n";
		        }
		
				//if yes user can enter again
		        int choice;
		        cout << "\nDo you want to add another item?" << endl;
		        cout << "1. Yes" << endl;
		        cout << "0. No (Return)" << endl;
		        cout << "Choice: ";
		        cin >> choice;
		
		        if (choice != 1) {
		            cout << "Returning..." << endl;
		            break;
		        }
		    }
		}


        /*void searchProduct() {
            int categoryId;
            string field;
            char dir;

            system("cls");
            Category c;
            int selectedCategory = c.displayCategoryAndChoose();
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
        }*/
    
    	void displayShoppingCart() {
		    system("cls");
		    FILE* file = fopen("shoppingcart.txt", "r");
		
		    if (file == NULL) {
		        cout << "Unable to open shoppingcart.txt" << endl;
		        return;
		    }
		
		    char line[256];
		    bool hasHeader = false;
		    bool hasItem = false;
		
		    cout << "======================= Shopping Cart =======================\n";
		    cout << "+--------+------------+--------+--------------+--------+\n";
		    cout << "|Cart ID |Product ID  |Qty     |Total Price   |Status  |\n";
		    cout << "+--------+------------+--------+--------------+--------+\n";
		
			// get the user data according the user id
		    while (fgets(line, sizeof(line), file)) {
		        if (!hasHeader) {
		            hasHeader = true;
		            continue;
		        }
		
		        int cartID, userID, productID, qty, status;
		        float total;
		        if (sscanf(line, "%d|%d|%d|%d|%f|%d", &cartID, &userID, &productID, &qty, &total, &status) == 6) {// this still need to change becasue idk what status are
		            if (userID == loginuserid) {
		                printf("|%-8d|%-12d|%-8d|%-14.2f|%-8s|\n",
    					cartID, productID, qty, total,(status == 0 ? "Pending" : (status == 1 ? "Done" : "Unknown")));
		                hasItem = true;
		            }
		        }
		    }
			
		    fclose(file);
		
		    cout << "+--------+------------+--------+--------------+--------+\n";
		    //calling the function check out, edit, and delete
		    cout << "Purchase your product in cart (Not function yet, want to quit please press enter)"<<endl;
		
		    if (!hasItem) {
		        cout << "No items in your cart.\n";
		    }
		
		    cin.ignore();
		    cin.get();
		}
		
		//new function check out
		void checkout(){
			int productID;
			
			cout << "Enter the product that you want to purchase : " << endl;
			cin >> productID;
			
		}
		
};

class UserModule {
    private:
        Product p;
        Category c;
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
                    case 1: {
                    	//load catagory
					    c.clearCategory();
					    c.loadCategory();
					
					    int selectedCategory = c.displayCategoryAndChoose();
						
						//load product
					    p.clearProducts();
					    p.loadProducts();
					
					    if (selectedCategory == 0) {
					        p.displayProductsFromStack();
					    } else {
					        p.displayProductsByCategory(selectedCategory);
					    }
					    break;
					}
                        
                    case 2:
                    	p.displayShoppingCart();
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

//class AdminModule(){
//	private:
//        Product p;
//        Category c;
//        
//    public:
//        void admin_menu() {
//            cout << string(28, '-') << endl;
//            cout << setw(20) << "User Dashboard" << endl;
//            cout << string(28, '-') << endl;
//            cout << "1. Manage Product" << endl;
//            cout << "2. Manage Order" << endl;
//            cout << "3. Report" << endl;
//            cout << "4. Profile" << endl;
//            cout << "0. Logout" << endl;
//        }
//        
//        void user_main_page() {
//            int choice;
//            do {
//                system("cls");
//                user_menu();
//                cout << "Please Select the option: ";
//                cin >> choice;
//                switch (choice) {
//                    case 1: {
//                    	//load catagory
//					    c.clearCategory();
//					    c.loadCategory();
//					
//					    int selectedCategory = c.displayCategoryAndChoose();
//						
//						//load product
//					    p.clearProducts();
//					    p.loadProducts();
//					
//					    if (selectedCategory == 0) {
//					        p.displayProductsFromStack();
//					    } else {
//					        p.displayProductsByCategory(selectedCategory);
//					    }
//					    break;
//					}
//                        
//                    case 2:
//                    	p.displayShoppingCart();
//                        break;
//                    case 3: 
//                        cout << "Order History feature not implemented yet." << endl;
//                        this_thread::sleep_for(chrono::seconds(1));
//                        break;
//                    case 4: 
//                        cout << "Profile feature not implemented yet." << endl;
//                        this_thread::sleep_for(chrono::seconds(1));
//                        break;
//                    case 0: 
//                        cout << "Logging out..." << endl;
//                        this_thread::sleep_for(chrono::seconds(1));
//                        break;
//                    default: 
//                        cout << "Invalid option!" << endl;
//                        this_thread::sleep_for(chrono::seconds(1));
//                }
//            } while (choice != 0);
//        }
//}

// Base class for admin operations

// Derived class 1: Product Management
class AdminOperation {
public:
    virtual void execute() = 0;
    virtual ~AdminOperation() {}
};

// ProductManagement 类
class ProductManagement : public AdminOperation{
private:
    Product* product;
    Category* category;
    
public:
    ProductManagement(Product* p, Category* c) : product(p), category(c) {}
    
    void execute() override {
        product->loadProducts();
        if(category) category->loadCategory();
        
        int choice;
        do {
            system("cls");
            displayProductMenu();
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();  // 清除输入缓冲区
            
            try {
                switch(choice) {
                    case 1: addProduct(); break;
                    case 2: editProduct(); break;
                    case 3: deleteProduct(); break;
//                    case 4: searchProduct(); break;
                    case 0: cout << "Returning to main menu...\n"; break;
                    default: throw invalid_argument("Invalid choice!");
                }
            } catch(const exception& e) {
                cerr << "Error: " << e.what() << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        } while(choice != 0);
    }

private:
    void displayProductMenu() {
        cout << "====================================" << endl;
        cout << "||      PRODUCT MANAGEMENT        ||" << endl;
        cout << "====================================" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Edit Product" << endl;
        cout << "3. Delete Product" << endl;
        cout << "4. Search Product" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "====================================" << endl;
    }
    
    void addProduct() {
        Product newProduct;
        
        cout << "\n--- Add New Product ---" << endl;
        
        cout << "Product Name: ";
        string name;
        getline(cin, name);
        if(name.empty()) throw invalid_argument("Product name cannot be empty!");
        newProduct.name = name;
        
        cout << "Price: RM";
        if(!(cin >> newProduct.price) || newProduct.price <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Price must be a positive number!");
        }
        
        cout << "Category ID: ";
        if(!(cin >> newProduct.categoryId)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Invalid category ID!");
        }
        
        cout << "Quantity: ";
        if(!(cin >> newProduct.quantity) || newProduct.quantity < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Quantity must be a non-negative number!");
        }
        
        cin.ignore();  // 清除换行符
        cout << "Description: ";
        getline(cin, newProduct.description);
        
        // 生成ID
        newProduct.id = generateProductId();
        
        // 添加到文件
        ofstream file("product.txt", ios::app);
        if(!file) throw runtime_error("Cannot open product file!");
        
        file << newProduct.id << "|" << newProduct.name << "|" << newProduct.price << "|"
             << newProduct.categoryId << "|" << newProduct.quantity << "|" 
             << newProduct.description << endl;
        
        file.close();
        
        cout << "\nProduct added successfully! ID: " << newProduct.id << endl;
        cout << "Press Enter to continue...";
        cin.get();
    }
    
    void editProduct() {
        cout << "\n--- Edit Product ---" << endl;
        cout << "Enter Product ID to edit: ";
        int id;
        if(!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Invalid product ID!");
        }
        cin.ignore();
        
        // 查找产品
        Product* existing = nullptr;
        product->loadProducts();
        product->productStack.Functraverse([&](Product* p) {
            if(p->id == id) existing = p;
        });
        
        if(!existing) throw runtime_error("Product not found!");
        
        // 显示当前信息
        cout << "\nCurrent Product Details:" << endl;
        Product::displaySingleProduct(existing);
        
        // 获取新信息
        Product updated = *existing;
        
        cout << "\nLeave blank to keep current value" << endl;
        
        cout << "New Name [" << existing->name << "]: ";
        string newName;
        getline(cin, newName);
        if(!newName.empty()) updated.name = newName;
        
        cout << "New Price [RM" << existing->price << "]: ";
        string priceStr;
        getline(cin, priceStr);
        if(!priceStr.empty()) {
            try {
                updated.price = stod(priceStr);
                if(updated.price <= 0) throw invalid_argument("");
            } catch(...) {
                throw invalid_argument("Invalid price value!");
            }
        }
        
        cout << "New Category ID [" << existing->categoryId << "]: ";
        string catStr;
        getline(cin, catStr);
        if(!catStr.empty()) {
            try {
                updated.categoryId = stoi(catStr);
            } catch(...) {
                throw invalid_argument("Invalid category ID!");
            }
        }
        
        cout << "New Quantity [" << existing->quantity << "]: ";
        string qtyStr;
        getline(cin, qtyStr);
        if(!qtyStr.empty()) {
            try {
                updated.quantity = stoi(qtyStr);
                if(updated.quantity < 0) throw invalid_argument("");
            } catch(...) {
                throw invalid_argument("Invalid quantity value!");
            }
        }
        
        cout << "New Description [" << existing->description << "]: ";
        string newDesc;
        getline(cin, newDesc);
        if(!newDesc.empty()) updated.description = newDesc;
        
        // 确认更新
        cout << "\nConfirm update? (Y/N): ";
        char confirm;
        cin >> confirm;
        cin.ignore();
        
        if(toupper(confirm) == 'Y') {
            // 更新文件
            ifstream inFile("product.txt");
            ofstream outFile("temp_product.txt");
            string line;
            
            // 复制标题行
            getline(inFile, line);
            outFile << line << endl;
            
            // 处理每一行
            while(getline(inFile, line)) {
                stringstream ss(line);
                string token;
                getline(ss, token, '|');
                int currentId = stoi(token);
                
                if(currentId == id) {
                    outFile << updated.id << "|" << updated.name << "|" << updated.price << "|"
                            << updated.categoryId << "|" << updated.quantity << "|" 
                            << updated.description << endl;
                } else {
                    outFile << line << endl;
                }
            }
            
            inFile.close();
            outFile.close();
            
            // 替换文件
            remove("product.txt");
            rename("temp_product.txt", "product.txt");
            
            cout << "Product updated successfully!" << endl;
        } else {
            cout << "Update cancelled." << endl;
        }
        
        cout << "Press Enter to continue...";
        cin.get();
    }
    
    void deleteProduct() {
        cout << "\n--- Delete Product ---" << endl;
        cout << "Enter Product ID to delete: ";
        int id;
        if(!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Invalid product ID!");
        }
        cin.ignore();
        
        // 查找产品
        Product* toDelete = nullptr;
        product->loadProducts();
        product->productStack.Functraverse([&](Product* p) {
            if(p->id == id) toDelete = p;
        });
        
        if(!toDelete) throw runtime_error("Product not found!");
        
        // 显示产品信息
        cout << "\nProduct to Delete:" << endl;
        Product::displaySingleProduct(toDelete);
        
        // 确认删除
        cout << "\nARE YOU SURE YOU WANT TO DELETE THIS PRODUCT? (Y/N): ";
        char confirm;
        cin >> confirm;
        cin.ignore();
        
        if(toupper(confirm) == 'Y') {
            // 从文件中删除
            ifstream inFile("product.txt");
            ofstream outFile("temp_product.txt");
            string line;
            
            // 复制标题行
            getline(inFile, line);
            outFile << line << endl;
            
            // 处理每一行
            while(getline(inFile, line)) {
                stringstream ss(line);
                string token;
                getline(ss, token, '|');
                int currentId = stoi(token);
                
                if(currentId != id) {
                    outFile << line << endl;
                }
            }
            
            inFile.close();
            outFile.close();
            
            // 替换文件
            remove("product.txt");
            rename("temp_product.txt", "product.txt");
            
            cout << "Product deleted successfully!" << endl;
        } else {
            cout << "Deletion cancelled." << endl;
        }
        
        cout << "Press Enter to continue...";
        cin.get();
    }
    
    void searchProduct() {
	    cout << "\n--- Search Product ---" << endl;
	    cout << "1. Search by ID" << endl;
	    cout << "2. Search by Name" << endl;
	    cout << "3. Search by Category" << endl;
	    cout << "Choice: ";
	
	    int choice;
	    if (!(cin >> choice)) {
	        cin.clear();
	        cin.ignore(numeric_limits<streamsize>::max(), '\n');
	        throw invalid_argument("Invalid choice!");
	    }
	    cin.ignore();
	
	    const int MAX_RESULTS = 100;
	    Product* results[MAX_RESULTS];
	    int resultCount = 0;
	
	    product->loadProducts();
	
	    switch (choice) {
	        case 1: {
	            cout << "Enter Product ID: ";
	            int id;
	            if (!(cin >> id)) {
	                cin.clear();
	                cin.ignore(numeric_limits<streamsize>::max(), '\n');
	                throw invalid_argument("Invalid product ID!");
	            }
	            cin.ignore();
	
	            product->productStack.Functraverse([&](Product* p) {
	                if (p->id == id && resultCount < MAX_RESULTS) {
	                    results[resultCount++] = p;
	                }
	            });
	            break;
	        }
	        case 2: {
	            cout << "Enter Product Name: ";
	            string name;
	            getline(cin, name);
	            string lowerName = product->toLower(name);
	
	            product->productStack.Functraverse([&](Product* p) {
	                string productName = product->toLower(p->name);
	                if (productName.find(lowerName) != string::npos && resultCount < MAX_RESULTS) {
	                    results[resultCount++] = p;
	                }
	            });
	            break;
	        }
	        case 3: {
	            int catId = category->displayCategoryAndChoose();
	
	            product->productStack.Functraverse([&](Product* p) {
	                if (p->categoryId == catId && resultCount < MAX_RESULTS) {
	                    results[resultCount++] = p;
	                }
	            });
	            break;
	        }
	        default:
	            throw invalid_argument("Invalid choice!");
	    }
	
	    // 显示结果
	    if (resultCount == 0) {
	        cout << "\nNo products found." << endl;
	    } else {
	        cout << "\nSearch Results:" << endl;
	        cout << "+----+-----------------------------------------+--------+----------+" << endl;
	        cout << "| ID | Name                                    | Price  | Category |" << endl;
	        cout << "+----+-----------------------------------------+--------+----------+" << endl;
	
	        for (int i = 0; i < resultCount; ++i) {
	            Product* p = results[i];
	            cout << "| " << setw(2) << p->id << " | " << left << setw(39) << truncateString(p->name, 39)
	                 << " | " << right << setw(6) << fixed << setprecision(2) << p->price
	                 << " | " << setw(8) << p->categoryId << " |" << endl;
	        }
	        cout << "+----+-----------------------------------------+--------+----------+" << endl;
	    }
	
	    cout << "Press Enter to continue...";
	    cin.get();
	}
    
    int generateProductId() {
        int lastId = 0;
        product->loadProducts();
        
        product->productStack.Functraverse([&](Product* p) {
            if(p->id > lastId) lastId = p->id;
        });
        
        return lastId + 1;
    }
    
    static string truncateString(const string& str, size_t width) {
        if(str.length() <= width) return str;
        return str.substr(0, width - 3) + "...";
    }
};

// OrderManagement 类
class OrderManagement : public AdminOperation {
	public:
	    void execute() override {
	        cout << "=== ORDER MANAGEMENT ===" << endl;
	        cout << "This feature is under development." << endl;
	        cout << "Press Enter to continue...";
	        cin.ignore();
	        cin.get();
	    }
};

// ReportGeneration 类
class ReportGeneration : public AdminOperation {
	public:
	    void execute() override {
	        cout << "=== REPORT GENERATION ===" << endl;
	        cout << "This feature is under development." << endl;
	        cout << "Press Enter to continue...";
	        cin.ignore();
	        cin.get();
	    }
};

class AdminModule {
	private:
	    Product p;
	    Category c;
	    
	public:
	    void admin_menu() {
	        cout << string(28, '-') << endl;
	        cout << setw(20) << "Admin Dashboard" << endl;
	        cout << string(28, '-') << endl;
	        cout << "1. Manage Product" << endl;
	        cout << "2. Manage Order" << endl;
	        cout << "3. Report" << endl;
	        cout << "4. Profile" << endl;
	        cout << "0. Logout" << endl;
	    }
	
	    void admin_main_page() {
	        int choice;
	        do {
	            system("cls");
	            admin_menu();
	            cout << "Please Select the option: ";
	            cin >> choice;
	            
	            try {
	                AdminOperation* operation = nullptr;
	                
	                switch(choice) {
	                    case 1: 
	                        operation = new ProductManagement(&p, &c);
	                        operation->execute();
	                        delete operation;
	                        break;
	                    case 2: 
	                        operation = new OrderManagement();
	                        operation->execute();
	                        delete operation;
	                        break;
	                    case 3: 
	                        operation = new ReportGeneration();
	                        operation->execute();
	                        delete operation;
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
	                        throw invalid_argument("Invalid option!");
	                }
	            } catch(const exception& e) {
	                cerr << "Error: " << e.what() << endl;
	                cin.clear();
	                cin.ignore(numeric_limits<streamsize>::max(), '\n');
	                this_thread::sleep_for(chrono::seconds(1));
	            }
	        } while(choice != 0);
	    }
};

// Friend function implementations
bool validateProduct(const Product& p) {
    return !p.name.empty() && p.price > 0 && p.quantity >= 0;
}

bool authenticateAdmin(const string& username, const string& password) {
    ifstream file("admin.txt");
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUser, storedPass;
        getline(ss, storedUser, '|');
        getline(ss, storedPass, '|');
        
        if (trim(storedUser) == username && trim(storedPass) == password) {
            return true;
        }
    }
    return false;
}

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
            }else if(l.getpagetype() == 2){
            	AdminModule am;
                am.admin_main_page();
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