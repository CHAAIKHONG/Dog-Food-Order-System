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
#include<conio.h>
#include<cctype>
#include<cstdlib>
#include<cstring> // Added for strcmp
using namespace std;
const int maxTokens = 100;
static const int MAX_RESULTS = 100;
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
        int i;
        int attemptCount;
    public:
		UserRegister(){
			attemptCount = 0;
			i=0;
		}
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
                i = 0;
				cout << "\nCONFIRM PASSWORD  : ";
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
            registerFile << left << setw(5) << lastid << "|" << left << setw(15) << name << "|" << setw(15) << confirmPassword << "|" << setw(60) << homeAddress << "|" << setw(30) 
			             << emailAddress << "|" << "+60" << phoneNumber << endl;
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
        void staffPage(const string& username) {
            cout << "\nWelcome, " << username << "! This is the staff page." << endl;
            pagetype = 3;
            this_thread::sleep_for(chrono::seconds(2));
        }
		void managerPage(const string& username)
		{
	        cout << "\nWelcome, " << username << "! This is the manager page." << endl;
	        pagetype = 2;
	        this_thread::sleep_for(chrono::seconds(2));
	    }
    public:
    	int getLoginUserID() {
	        return loginuserid;
	    }
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
		void viewProfile(int userId, int type) {
			string filename;
			if(type == 3){
	    		filename = "staff.txt";
			}else if(type == 2){
				filename = "manager.txt";
			}
			else{
				filename = "register.txt";
			}
			ifstream file(filename);
			if (!file) {
	            cout << "Error: Unable to open register.txt!" << endl;
	            return;
	        }
	        string line;
	        bool found = false;
	        // Skip header if exists
	        getline(file, line);
	        while (getline(file, line)) {
	            string fields[6];
	            int numFields = split(line, "|", fields, 6);
	            if (numFields >= 6) {
	                string storedId = trim(fields[0]);
	                if (stoi(storedId) == userId) {
	                    found = true;
	                    displayProfileInfo(fields, type);
	                    break;
	                }
	            }
	        }
	        file.close();
	        if (!found) {
	            cout << "Profile not found!" << endl;
	        }
	    }
	    void editProfile(int userId, int type) {
	    	string filename;
			if(type == 3){
	    		filename = "staff.txt";
			}else if(type == 2){
				filename = "manager.txt";
			}
			else{
				filename = "register.txt";
			}
    		string tempFilename = "temp_" + filename;
	        ifstream inFile(filename);
	        ofstream outFile(tempFilename);
	        string line;
	        bool found = false;
	        User updatedUser;
	        getline(inFile, line);
	        outFile << line << endl;
	        while (getline(inFile, line)) {
	            string fields[6];
	            int numFields = split(line, "|", fields, 6);
	            if (numFields >= 6) {
	                string storedId = trim(fields[0]);
	                if (stoi(storedId) == userId) {
	                    found = true;
	                    updatedUser = getUserFromFields(fields);
	                    displayEditMenu(updatedUser, type);
	                    outFile << updatedUser.id << "|" << updatedUser.username << "|" // Write updated user to temp file
	                           << updatedUser.password << "|" << updatedUser.address << "|"
	                           << updatedUser.email << "|" << updatedUser.phone << endl;
	                } else {
	                    outFile << line << endl;
	                }
	            }
	        }
	        inFile.close();
	        outFile.close();
	        if (found) {
	            remove(filename.c_str());// Replace original file with updated file
				rename(tempFilename.c_str(), filename.c_str());
	            cout << "Profile updated successfully! Please enter to continue." << endl;
	            cin.get();
	        } else {
	        	remove(tempFilename.c_str());
	            cout << "Profile not found! Please enter to continue." << endl;
	            cin.get();
	        }
	    }
	    void displayProfileInfo(const string fields[], int type) {
	        cout << "\n=== PROFILE INFORMATION ===" << endl;
	        cout << "Username: " << trim(fields[1]) << endl;
			if(type == 1){
	        	cout << "Address: " << trim(fields[3]) << endl;
			}
	        cout << "Email: " << trim(fields[4]) << endl;
	        cout << "Phone: " << trim(fields[5]) << endl;
	        cout << "===========================" << endl;
	    }
	    void displayEditMenu(User& user, int type) {
	        int choice;
	        do {
	            system("cls");
	            cout << "\n=== EDIT PROFILE ===" << endl;
	            cout << "1. Change Password" << endl;
	            cout << "2. Update Email" << endl;
	            cout << "3. Update Phone" << endl;
				if(type == 1){
	            	cout << "4. Update Address" << endl;
				}
	            cout << "0. Back to Profile" << endl;
	            cout << "Choice: ";
	            cin >> choice;
	            cin.ignore();
	            
	            switch(choice) {
	                case 1: changePassword(user); break;
	                case 2: updateEmail(user.email); break;
	                case 3: updatePhone(user.phone); break;
	                case 4: updateField(user.address, "Enter new address: "); break;
	                case 0: break;
	                default: cout << "Invalid choice!" << endl;
	            }
	        } while(choice != 0);
	    }
	    void changePassword(User& user) {
	        char currentPassword[50];
	        char newPassword[50];
	        char confirmPassword[50];
	        cout << "Enter current password: ";
	        getPasswordInput(currentPassword, 50);
	        if (strcmp(currentPassword, user.password.c_str()) != 0) {
	            cout << "Incorrect current password! Press Enter to continue." << endl;
				cin.get();
	            return;
	        }
	        cout << "Enter new password: ";
	        getPasswordInput(newPassword, 50);
	        cout << "Confirm new password: ";
	        getPasswordInput(confirmPassword, 50);
	        if (strcmp(newPassword, confirmPassword) != 0) {
	            cout << "Passwords don't match! Press Enter to continue." << endl;
				cin.get();
	            return;
	        }
	        user.password = newPassword;
	        cout << "Password changed successfully!" << endl;
			cout << "Press Enter to continue." << endl;
			cin.get();
	    }
	    void updateEmail(string& email) {
	        string newEmail;
	        cout << "Current email: " << email << endl;
	        cout << "Enter new email: ";
	        getline(cin, newEmail);
	        bool valid = false;// Simple email validation
	        for (size_t i = 0; i < newEmail.length(); i++) {
	            if (newEmail[i] == '@') {
	                valid = true;
	                break;
	            }
	        }
	        if (valid) {
	            email = newEmail;
	            cout << "Email updated successfully! Press Enter to continue." << endl;
				cin.get();
	        } else {
	            cout << "Invalid email format! Must contain '@'! Press Enter to continue." << endl;
				cin.get();
	        }
	    }
	    void updatePhone(string& phone) {
	        string newPhone;
	        cout << "Current phone: " << phone << endl;
	        cout << "Enter new phone (e.g., +60123456789): +60";
	        getline(cin, newPhone);
	        bool valid = true;// Simple phone validation - check if all characters are digits
	        for (size_t i = 0; i < newPhone.length(); i++) {
	            if (!isdigit(newPhone[i])) {
	                valid = false;
	                break;
	            }
	        }
	        if (valid && newPhone.length() >= 9 && newPhone.length() <= 11) {
	            phone = "+60" + newPhone;
	            cout << "Phone updated successfully! Press Enter to continue." << endl;
				cin.get();
	        } else {
	            cout << "Invalid phone number! Must be 9-11 digits after +60. Press Enter to continue." << endl;
				cin.get();
	        }
	    }
	    void updateField(string& field, const string& prompt) {
	        string newValue;
	        cout << "Current: " << field << endl;
	        cout << prompt;
	        getline(cin, newValue);
	        if (!newValue.empty()) {
	            field = newValue;
	            cout << "Updated successfully! Press Enter to continue." << endl;
				cin.get();
	        }
	    }
	    void getPasswordInput(char* buffer, int maxLength) {
	        int i = 0;
	        char ch;
	        while ((ch = getch()) != '\r' && i < maxLength - 1) {
	            if (ch == '\b') {
	                if (i > 0) {
	                    i--;
	                    cout << "\b \b";
	                }
	            } else {
	                buffer[i++] = ch;
	                cout << '*';
	            }
	        }
	        buffer[i] = '\0';
	        cout << endl;
	    }
	    User getUserFromFields(const string fields[]) {
	        User user;
	        user.id = stoi(trim(fields[0]));
	        user.username = trim(fields[1]);
	        user.password = trim(fields[2]);
	        user.address = trim(fields[3]);
	        user.email = trim(fields[4]);
	        user.phone = trim(fields[5]);
	        return user;
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
    }else if (ul.checkCredentials("manager.txt", username, password)) {
        ul.managerPage(username);
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
        Category* ChashTable[HASH_SIZE] = { nullptr };
    public:
        Category* categoryList = nullptr;
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
        int displayCategoryAndChoose(string type) { //display the category for customers select whitch categort that want to display
		    Category* Ctemp = categoryList;
		    cout << "=== Category List ===\n";
		    cout << "+----+-----------------------------------------+\n";
		    cout << "| ID | Category Name                           |\n";
		    cout << "+----+-----------------------------------------+\n";
		    if(type == "user"){
		    	cout << "|  99|                          Search Product |\n";
		    	cout << "|  0 |                             Display All |\n";
			}
		    while (Ctemp) {
		        cout << "| " << setw(2) << Ctemp->id << " | " << setw(39) << Ctemp->name << " |\n";
		        Ctemp = Ctemp->mainNext;
		    }
		    cout << "+----+-----------------------------------------+\n";
		    int choose;
			if(type == "user"){
		    	cout << "Enter the category ID that you want to find: ";
			}else{
				cout << "Select the category for product: ";
			}
		    cin >> choose;
		    return choose;
		}
		string getCategoryNameById(int categoryId) {
			int index = categoryId % HASH_SIZE;
			Category* current = ChashTable[index];
			while (current) {
				if (current->id == categoryId)
					return current->name;
				current = current->hashNext;
			}
			return "Unknown";
		}
};
class Product {
    private:
        static const int HASH_SIZE = 100;
        Product* productList = nullptr;
        Product* hashTable[HASH_SIZE] = { nullptr };
        Product* nameHashTable[HASH_SIZE] = { nullptr };
        Product* searchResults[MAX_RESULTS];
        int searchResultCount;
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
        Product* nameHashNext;
        int product_choice;
		int product_quantity;
		double product_totalprice;
		int loginuserid = 0;
        Product() : id(0), name(""), categoryId(0), price(0.0), quantity(0), description(""), mainNext(nullptr), hashNext(nullptr) {}
		int hashFunction(const string& name) {
			int hash = 0;
			for(char c : name) {
				hash = (hash * 31 + tolower(c)) % HASH_SIZE;
			}
			return hash;
		}
		void buildNameHashTable() {
			for (int i = 0; i < HASH_SIZE; i++) {
				nameHashTable[i] = nullptr;
			}
			Product* current = productList;
			while (current != nullptr) {
				string cleanName = toLower(trim(current->name));
				int index = hashFunction(cleanName);
				current->nameHashNext = nameHashTable[index];
				nameHashTable[index] = current;
				current = current->mainNext;
			}
		}
		void searchByName(const string& name) {
			searchResultCount = 0;
			string lowerName = toLower(trim(name));
			cout << "Searching for: " << lowerName << endl;
			int index = hashFunction(lowerName);
			Product* current = nameHashTable[index];
			while (current != nullptr) {
				if (toLower(trim(current->name)) == lowerName) {
					searchResults[searchResultCount++] = current;
					break; 
				}
				current = current->nameHashNext;
			}
			cout << "Total matches found: " << searchResultCount << endl;
		}
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
		void loadProducts() {//load product from stack
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
			buildNameHashTable();
		}
		string getProductNameFromStackById(int productId) {
			string result = "Unknown";
			loadProducts();
			productStack.Functraverse([&](Product* p) {
				if (p->id == productId) {
					result = p->name;
				}
			});
			return result;
		}
		string toLower(const string& s) {// turn the product name to lower
			string result = s;
			for (size_t i = 0; i < result.length(); i++) {
				result[i] = tolower(result[i]);
			}
			return result;
		}
		void mergeSortByMode(Product** arr, int left, int right, int mode) {//merge sort // can write having the helps form gpt
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
			//get mode
			int i = 0, j = 0, k = left;
			while (i < n1 && j < n2) {
				bool condition = false;
				if (mode == 1) condition = toLower(L[i]->name) <= toLower(R[j]->name);
				else if (mode == 2) condition = toLower(L[i]->name) >= toLower(R[j]->name);
				else if (mode == 3) condition = L[i]->price <= R[j]->price;
				else if (mode == 4) condition = L[i]->price >= R[j]->price;
				else if (mode == 5) condition = L[i]->id <= R[j]->id;
				else if (mode == 6) condition = L[i]->id >= R[j]->id;
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
		static void displaySingleProduct(Product* p, Category& categoryManager) {
			string name = truncateString(p->name, 39);
			string desc = truncateString(p->description, 26);
			string categoryName = truncateString(categoryManager.getCategoryNameById(p->categoryId), 16);
			if (p) {
				cout << "| " << setw(2) << p->id << " | " << left << setw(39) << name << " | " 
					<< right << setw(6) << fixed << setprecision(2) << p->price << " | " 
					<< setw(16) << categoryName << " | " << setw(8) << p->quantity << " | " 
					<< setw(21) << desc << " |" << endl;
			}
		}
		void displaySortMenu(int& mode) {//let user select the sort mode
			cout << "============================================================" << endl;
			cout << "||                     Sort Product                       ||" << endl;
			cout << "============================================================\n" << endl;
			cout << "Sort products by:" << endl;
			cout << "1. Name (A to Z)" << endl;
			cout << "2. Name (Z to A)" << endl;
			cout << "3. Price (Low to High)" << endl;
			cout << "4. Price (High to Low)" << endl;
			cout << "5. ID (Adcending)" << endl;
			cout << "6. ID (Decending)" << endl;
			cout << "Enter your choice (1-6): ";
			cin >> mode;
			while (mode < 1 || mode > 6) {
				cout << "Invalid choice. Please enter 1-6: ";
				cin >> mode;
			}
		}
		void displayProductsFromStack(Category& c, int type) {//display the all product
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
			cout << "\n+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
			cout << "| ID | Product Name                            | Price  | Category         | Quantity | Description                |\n";
			cout << "+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
			for (int i = 0; i < count; ++i) {
				displaySingleProduct(arr[i], c);
			}
			cout << "+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
			viewProductDetailsById(type);
		}
		//display the category that user selected
		void displayProductsByCategory(int categoryId, Category& c, int type) {
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
			cout << "+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
			cout << "| ID | Product Name                            | Price  | Category         | Quantity | Description                |\n";
			cout << "+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
			for (int i = 0; i < count; ++i) {
				displaySingleProduct(arr[i], c);
			}
			cout << "+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
			viewProductDetailsById(type);
		}
		void viewProductDetailsById(int type) {//view product details from sorted data
			int pid;
			cout << "\nEnter a Product ID to view details (0 to go back): ";
			cin >> pid;
			if (pid == 0){
				return;
			}
			Product* current = productList;
			Product* selected = nullptr;
			while (current != nullptr) {
				if (current->id == pid) {
					selected = current;
					break;
				}
				current = current->mainNext;
			}
			while (true) {
				system("cls");
				cout << "=================== PRODUCT DETAILS ===================\n";
				cout << "ID:          " << selected->id << endl;
				cout << "Name:        " << selected->name << endl;
				cout << "Price:       RM " << fixed << setprecision(2) << selected->price << endl;
				cout << "Quantity:    " << selected->quantity << endl;
				cout << "Description: " << selected->description << endl;
				cout << "=======================================================\n";
				if(type != 1){
					cin.ignore();
					cout << "Press Enter to continure." << endl;
					cin.get();
					return;
				}
				cout << "1. Add to cart\n";
				cout << "0. Back\n";
				cout << "Choice: ";
				int choice;
				cin >> choice;
				if (choice == 0) break;
				else if (choice == 1) {
					int qty;
					cout << "Enter quantity: ";
					cin >> qty;
					if (qty <= 0) {
						cout << "Invalid quantity, please enter the quantity more that 0." << endl;
						this_thread::sleep_for(chrono::seconds(2));
					}
					else if (qty > selected->quantity) {
						cout << "The quantity is out of stock. Available stock: " << selected->quantity << endl;
						this_thread::sleep_for(chrono::seconds(2));
					}
					else {
						addToCartDirectly(selected->id, qty, selected->price);
						cout << "Item added to cart.\n";
						this_thread::sleep_for(chrono::seconds(1));
						break;
					}
				} else {
					cout << "Invalid choice.\n";
				}
			}
		}
		void searchProduct() {//search (hashing)
			system("cls");
			Category c;
			c.loadCategory();
			buildNameHashTable();
			while(true) {
				cout << "============================================================" << endl;
				cout << "||                     SEARCH PRODUCT                     ||" << endl;
				cout << "============================================================" << endl;
				cout << "1. Search by Product Name" << endl;
				cout << "0. Back" << endl;
				cout << "Choice: ";
				int choice;
				cin >> choice;
				cin.ignore();
				if(choice == 0) {
					return;
				}
				else if(choice == 1) {
					system("cls");
					cout << "Enter product name to search : ";
					string searchName;
					getline(cin, searchName);
					searchByName(searchName);
					if(searchResultCount == 0) {
						cout << "No products found matching \"" << searchName << "\"" << endl;
						cout << "Press Enter to continue...";
						cin.ignore();
						cin.get();
						system("cls");
						continue;
					}
					// Display search results
					cout << "\n+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
					cout << "| ID | Product Name                            | Price  | Category         | Quantity | Description                |\n";
					cout << "+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
					for(int i = 0; i < searchResultCount; i++) {
						displaySingleProduct(searchResults[i], c);
					}
					cout << "+----+-----------------------------------------+--------+------------------+----------+----------------------------+\n";
					cout << "\nEnter product ID to view details (0 to go back): ";
					int pid;
					cin >> pid;
					if(pid == 0) {
						system("cls");
						continue;
					}
					// Find the selected product
					Product* selected = nullptr;
					for(int i = 0; i < searchResultCount; i++) {
						if(searchResults[i]->id == pid) {
							selected = searchResults[i];
							break;
						}
					}
					if(!selected) {
						cout << "Invalid product ID.\n";
						cout << "Press Enter to continue...";
						cin.ignore();
						cin.get();
						system("cls");
						continue;
					}
					while(true) {
						system("cls");
						cout << "============================================================" << endl;
						cout << "||                    PRODUCT DETAILS                      ||" << endl;
						cout << "============================================================" << endl;
						cout << "ID:          " << selected->id << endl;
						cout << "Name:        " << selected->name << endl;
						cout << "Price:       " << selected->price << endl;
						cout << "Quantity:    " << selected->quantity << endl;
						cout << "Description: " << selected->description << endl;
						cout << "============================================================" << endl;
						cout << "1. Add to cart" << endl;
						cout << "0. Go back" << endl;
						cout << "Choice: ";
						int detailChoice;
						cin >> detailChoice;
						if(detailChoice == 0) {
							break;
						}
						else if(detailChoice == 1) {
							cout << "Enter quantity: ";
							int qty;
							cin >> qty;
							if(qty <= 0 || qty > selected->quantity) {
								cout << "Invalid quantity. Available: " << selected->quantity << "\n";
								cout << "Press Enter to continue...";
								cin.ignore();
								cin.get();
							}
							else {
								addToCartDirectly(selected->id, qty, selected->price);
								cout << "Item added to cart successfully.\n";
								cout << "Press Enter to continue...";
								cin.ignore();
								cin.get();
								break;
							}
						}
						else {
							cout << "Invalid choice. Please try again.\n";
							cout << "Press Enter to continue...";
							cin.ignore();
							cin.get();
						}
					}
					system("cls");
				}
			}
		}
		void addToCartDirectly(int pid, int qty, double price) {//add to cart, searcing
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
				if (cusID == loginuserid && productID == pid) {
					quantity += qty;
					total = price * quantity;
					ostringstream updated;
					updated << cartID << "|" << cusID << "|" << productID << "|"
							<< quantity << "|" << fixed << setprecision(2) << total;
					lines[i] = updated.str();
					found = true;
					break;
				}
			}
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
				newline << nextCartID << "|" << loginuserid << "|" << pid << "|"
						<< qty << "|" << fixed << setprecision(2) << (price * qty);
				lines[lineCount++] = newline.str();
			}
			ofstream fout("shoppingcart.txt");
			fout << "Cart ID|Cus ID|Product ID|QTY|Total Price\n";
			for (int i = 1; i < lineCount; ++i) {
				fout << lines[i] << "\n";
			}
			fout.close();
		}
		void displayShoppingCart() {
			while(true) { // Add loop to keep showing cart until user chooses to go back
				system("cls");
				ifstream file("shoppingcart.txt");
				if (!file) {
					cout << "Unable to open shoppingcart.txt" << endl;
					return;
				}
				string line;
				bool hasHeader = false;
				bool hasItem = false;
				float cartTotal = 0.0f;
				cout << "======================= Shopping Cart =======================\n";
				cout << "+--------+--------------------------------+--------+--------------+\n";
				cout << "|Cart ID |Product Name                    |Qty     |Total Price   |\n";
				cout << "+--------+--------------------------------+--------+--------------+\n";
				while (getline(file, line)) {
					if (!hasHeader) {
						hasHeader = true; // skip header
						continue;
					}
					stringstream ss(line);
					string token;
					int cartID, userID, productID, qty;
					float total;
					getline(ss, token, '|'); cartID = stoi(token);
					getline(ss, token, '|'); userID = stoi(token);
					getline(ss, token, '|'); productID = stoi(token);
					getline(ss, token, '|'); qty = stoi(token);
					getline(ss, token, '|'); total = stof(token);
					string productName = truncateString(getProductNameFromStackById(productID), 30);
					if (userID == loginuserid) {
						cout << "| " << setw(7) << cartID
							<< "| " << setw(31) << productName
							<< "| " << setw(7) << qty
							<< "| RM " << setw(10) << fixed << setprecision(2) << total << "|\n";
						hasItem = true;
						cartTotal += total;
					}
				}
				file.close();
				cout << "+--------+--------------------------------+--------+--------------+\n";
				if (hasItem) {
					cout << "| Total:                        RM " << setw(30) << fixed << setprecision(2) << cartTotal << " |\n";
					cout << "+-----------------------------------------------------------------+\n";
				}
				if (!hasItem) {
					cout << "No items in your cart.\n";
					cout << "\nPress Enter to continue...";
					cin.ignore();
					cin.get();
					return;
				} else {
					cout << "\nOptions:\n";
					cout << "1. Edit Item Quantity\n";
					cout << "2. Remove Item\n";
					cout << "3. Checkout\n";
					cout << "0. Back to Main Menu\n";
					cout << "Choice: ";
					int choice;
					cin >> choice;
					switch (choice) {
						case 1:
							editCartItem();
							break;
						case 2:
							deleteCartItem();
							break;
						case 3:
							checkoutCart();
							break;
						case 0:
							return; // Exit the loop and return to main menu
						default:
							cout << "Invalid choice!" << endl;
							this_thread::sleep_for(chrono::seconds(1));
					}
				}
			}
		}
		void editCartItem() {
		// Show current cart
			int cartID, newQty;
			cout << "\nEnter Cart ID to edit: ";
			cin >> cartID;
			cout << "Enter new quantity: ";
			cin >> newQty;
			ifstream fin("shoppingcart.txt");
			ofstream fout("temp.txt");
			string line;
			bool found = false;
			bool headerWritten = false;
			while (getline(fin, line)) {
				if (line.find("Cart ID") != string::npos) {
					if (!headerWritten) {
						fout << line << "\n";
						headerWritten = true;
					}
					continue;
				}
				stringstream ss(line);
				string token;
				int currentCartID, currentUserID, productID, quantity;
				float total;
				getline(ss, token, '|'); currentCartID = stoi(token);
				getline(ss, token, '|'); currentUserID = stoi(token);
				getline(ss, token, '|'); productID = stoi(token);
				getline(ss, token, '|'); quantity = stoi(token);
				getline(ss, token, '|'); total = stof(token);
				if (currentCartID == cartID && currentUserID == loginuserid) {
					// Find the product to check stock
					Product* product = nullptr;
					Product* current = productList;
					while (current != nullptr) {
						if (current->id == productID) {
							product = current;
							break;
						}
						current = current->mainNext;
					}
					if (product == nullptr) {
						cout << "Product not found!" << endl;
						fout << line << "\n";
						continue;
					}
					if (newQty <= 0 || newQty > product->quantity) {
						cout << "Invalid quantity! Available stock: " << product->quantity << endl;
						fout << line << "\n";
					} else {
						// Update the quantity and total
						float newTotal = product->price * newQty;
						fout << currentCartID << "|" << currentUserID << "|" << productID << "|" 
							<< newQty << "|" << fixed << setprecision(2) << newTotal << "\n";
						cout << "Quantity updated successfully!" << endl;
						found = true;
					}
				} else {
					fout << line << "\n";
				}
			}
			fin.close();
			fout.close();
			if (!found) {
				cout << "Cart item not found or doesn't belong to you!" << endl;
				remove("temp.txt");
			} else {
				remove("shoppingcart.txt");
				rename("temp.txt", "shoppingcart.txt");
			}
			cout << "Press Enter to continue...";
			cin.ignore();
			cin.get();
		}
		void deleteCartItem() {// Delete a shopping cart item
			int cartID;
			cout << "\nEnter Cart ID to delete: ";
			cin >> cartID;
			ifstream fin("shoppingcart.txt");
			ofstream fout("temp.txt");
			string line;
			bool found = false;
			bool headerWritten = false;
			while (getline(fin, line)) {
				if (line.find("Cart ID") != string::npos) {
					if (!headerWritten) {
						fout << line << "\n";
						headerWritten = true;
					}
					continue;
				}
				stringstream ss(line);
				string token;
				int currentCartID, currentUserID;
				getline(ss, token, '|'); currentCartID = stoi(token);
				getline(ss, token, '|'); currentUserID = stoi(token);
				if (currentCartID == cartID && currentUserID == loginuserid) {
					found = true;
					cout << "Item removed from cart!" << endl;
				} else {
					fout << line << "\n";
				}
			}
			fin.close();
			fout.close();
			if (!found) {
				cout << "Cart item not found or doesn't belong to you!" << endl;
				remove("temp.txt");
			} else {
				remove("shoppingcart.txt");
				rename("temp.txt", "shoppingcart.txt");
			}
			cout << "Press Enter to continue...";
			cin.ignore();
			cin.get();
		}
		void checkoutCart() {// Checkout entire cart and record to order history
			char confirm;
			cout << "\nProceed to checkout? (Y/N): ";
			cin >> confirm;
			if (toupper(confirm) != 'Y') {
				return;
			}
			time_t now = time(0);// Get current date and time
			tm* ltm = localtime(&now);
			stringstream dateSS;
			dateSS << 1900 + ltm->tm_year << "-" 
				<< setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
				<< setw(2) << setfill('0') << ltm->tm_mday;
			string orderDate = dateSS.str();
			int orderID = 1;// Generate order ID
			ifstream orderFile("orderhistory.txt");
			string lastLine;
			while (getline(orderFile, lastLine)) {
				if (!lastLine.empty() && lastLine.find("Order ID") == string::npos) {
					stringstream ss(lastLine);
					string token;
					getline(ss, token, '|');
					try {
						orderID = stoi(token) + 1;
					} catch (...) {
						orderID = 1;
					}
				}
			}
			orderFile.close();
			// Process shopping cart
			ifstream cartFile("shoppingcart.txt");
			ofstream orderOut("orderhistory.txt", ios::app);
			ofstream tempCart("temp.txt");
			string line;
			float totalAmount = 0.0f;
			bool hasItems = false;
			bool headerWritten = false;
			// Write header if file is empty
			if (orderID == 1) {
				orderOut << "Order ID|User ID|Product ID|Quantity|Total Price|Order Date|Status\n";
			}
			while (getline(cartFile, line)) {
				if (line.find("Cart ID") != string::npos) {
					if (!headerWritten) {
						tempCart << line << "\n";
						headerWritten = true;
					}
					continue;
				}
				stringstream ss(line);
				string token;
				int cartID, userID, productID, quantity;
				float total;
				getline(ss, token, '|'); cartID = stoi(token);
				getline(ss, token, '|'); userID = stoi(token);
				getline(ss, token, '|'); productID = stoi(token);
				getline(ss, token, '|'); quantity = stoi(token);
				getline(ss, token, '|'); total = stof(token);
				if (userID == loginuserid) {
					// Add to order history
					orderOut << orderID << "|" << userID << "|" << productID << "|" 
							<< quantity << "|" << fixed << setprecision(2) << total << "|"
							<< orderDate << "|Preparing\n";
					// Update product stock
					Product* product = productList;
					while (product != nullptr) {
						if (product->id == productID) {
							product->quantity -= quantity;
							break;
						}
						product = product->mainNext;
					}
					totalAmount += total;
					hasItems = true;
				} else {
					tempCart << line << "\n";
				}
			}
			cartFile.close();
			orderOut.close();
			tempCart.close();
			if (hasItems) {// Update product file
				ofstream productOut("product.txt");
				productOut << "id|name|price|categoryId|quantity|description\n";
				Product* current = productList;
				while (current != nullptr) {
					productOut << current->id << "|" << current->name << "|" << fixed << setprecision(2) 
							<< current->price << "|" << current->categoryId << "|" 
							<< current->quantity << "|" << current->description << "\n";
					current = current->mainNext;
				}
				productOut.close();
				remove("shoppingcart.txt");// Replace shopping cart file
				rename("temp.txt", "shoppingcart.txt");
				cout << "\nCheckout successful! " << endl;
				cout << "Total Amount: RM " << fixed << setprecision(2) << totalAmount << endl;
			} else {
				remove("temp.txt");
				cout << "No items in your cart to checkout!" << endl;
			}
			cout << "Press Enter to continue...";
			cin.ignore();
			cin.get();
		}
		void displayOrderHistory() {
			system("cls");
			ifstream file("orderhistory.txt");
			if (!file) {
				cout << "No order history found." << endl;
				this_thread::sleep_for(chrono::seconds(1));
				return;
			}
			string line;
			bool hasOrders = false;
			float totalSpent = 0.0f;
			int currentOrderID = -1;
			bool firstOrder = true;
			getline(file, line);// Skip header
			cout << "================================ ORDER HISTORY ================================" << endl;
			while (getline(file, line)) {
				stringstream ss(line);
				string token;
				int orderID, userID, productID, quantity;
				float total;
				string orderDate, status;
				getline(ss, token, '|'); orderID = stoi(token);
				getline(ss, token, '|'); userID = stoi(token);
				getline(ss, token, '|'); productID = stoi(token);
				getline(ss, token, '|'); quantity = stoi(token);
				getline(ss, token, '|'); total = stof(token);
				getline(ss, orderDate, '|');
				getline(ss, status);
				if (userID != loginuserid) continue;
				hasOrders = true;
				if (orderID != currentOrderID) {
					if (!firstOrder) {
						cout << "+------------------------------------------------------------------+" << endl;
						cout << "| Order Total:                          RM " << setw(23) << fixed << setprecision(2) << totalSpent << " |" << endl;
						cout << "====================================================================" << endl << endl;
						totalSpent = 0.0f;
					}
					firstOrder = false;
					currentOrderID = orderID;
					cout << "+------------------------------------------------------------------+" << endl;
					cout << "| Order ID: " << setw(54) << left << orderID << " |" << endl;
					cout << "| Date: " << setw(58) << left << orderDate << " |" << endl;
					cout << "| Status: " << setw(54) << left << status << " |" << endl;
					cout << "+------------------------------------------------------------------+" << endl;
					cout << "| Product Name                      | Qty | Price    | Subtotal    |" << endl;
					cout << "+------------------------------------------------------------------+" << endl;
				}
				loadProducts();// Find product name by manually searching through product list
				string productName = "Unknown Product";
				Product* currentProduct = productList;
				while (currentProduct != nullptr) {
					if (currentProduct->id == productID) {
						productName = currentProduct->name;
						break;
					}
					currentProduct = currentProduct->mainNext;
				}
				// Display with product name instead of ID
				cout << "| " << setw(33) << left << truncateString(productName, 33) << " | " 
					<< setw(3) << right << quantity << " | " 
					<< setw(8) << right << fixed << setprecision(2) << (total/quantity) << " | "
					<< setw(11) << right << fixed << setprecision(2) << total << " |" << endl;
		
				totalSpent += total;
			}
			if (hasOrders) {
				cout << "+------------------------------------------------------------------+" << endl;
				cout << "| Order Total:                          RM " << setw(23) << fixed << setprecision(2) << totalSpent << " |" << endl;
				cout << "====================================================================" << endl;
			} else {
				cout << "No orders found for your account." << endl;
			}
			file.close();
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
		}
};
class UserModule {
    private:
        Product p;
        Category c;
        UserLogin ul;
        int userId;
    public:
    	void setLoginUserID(int uid) {
    		userId = uid;
	        p.loginuserid = uid;
	    }
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
        void user_main_page(int type) {
            int choice;
            do {
                system("cls");
                user_menu();
                cout << "Please Select the option: ";
                cin >> choice;
                switch (choice) {
                    case 1: {
                        c.clearCategory();
                        c.loadCategory();//load catagory
                    	system("cls");
                        int selectedCategory = c.displayCategoryAndChoose("user");
                        p.clearProducts();
                        p.loadProducts();//load product
                        if (selectedCategory == 0) {
                            p.displayProductsFromStack(c, type);
                        } 
                        else if (selectedCategory == 99) {
                            p.searchProduct();
                        } 
                        else {
                            p.displayProductsByCategory(selectedCategory, c, type);
                        }
                        break;
                    }
                    case 2:
                        p.displayShoppingCart();
                        break;
                    case 3: 
                        p.displayOrderHistory();
                        break;
                    case 4: 
                        profileMenu(type);
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
        void profileMenu(int type) {
	        int choice;
	        do {
	            system("cls");
	            cout << "=== PROFILE MENU ===" << endl;
	            cout << "1. View Profile" << endl;
	            cout << "2. Edit Profile" << endl;
	            cout << "0. Back" << endl;
	            cout << "Choice: ";
	            cin >> choice;
	            cin.ignore();
	            switch(choice) {
	                case 1:
	                    ul.viewProfile(userId, type);
	                    cout << "\nPress Enter to continue...";
	                    cin.get();
	                    break;
	                case 2:
	                    ul.editProfile(userId, type);
	                    break;
	                case 0:
	                    break;
	                default:
	                    cout << "Invalid choice!" << endl;
	                    this_thread::sleep_for(chrono::seconds(1));
	            }
	        } while(choice != 0);
	    }
};
class AdminOperation {// Base class for admin operations
public:
    virtual void execute() = 0;
    virtual ~AdminOperation() {}
};
class ProductManagement : public AdminOperation{// ProductManagement
	private:
		Product* product;
		Category* category;
		int type;
	public:
		ProductManagement(Product* p, Category* c, int t) : product(p), category(c), type(t) {}
		void execute() override {
			product->loadProducts();
			if(category) category->loadCategory();
			int choice;
			do {
				system("cls");
				displayProductMenu();
				cout << "Enter your choice: ";
				cin >> choice;
				cin.ignore();
				try {
					switch(choice) {
						case 1: addProduct(); break;
						case 2: editProduct(); break;
						case 3: deleteProduct(); break;
						case 4: searchProduct(); break;
						case 5: product->displayProductsFromStack(*category, type);
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
		void displayProductMenu() {
			cout << "====================================" << endl;
			cout << "||      PRODUCT MANAGEMENT        ||" << endl;
			cout << "====================================" << endl;
			cout << "1. Add Product" << endl;
			cout << "2. Edit Product" << endl;
			cout << "3. Delete Product" << endl;
			cout << "4. Search Product" << endl;
			cout << "5. Display All Product" << endl;
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
			newProduct.categoryId = category->displayCategoryAndChoose("admin");
			cout << "Quantity: ";
			if(!(cin >> newProduct.quantity) || newProduct.quantity < 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				throw invalid_argument("Quantity must be a non-negative number!");
			}
			cin.ignore();
			cout << "Description: ";
			getline(cin, newProduct.description);
			newProduct.id = generateProductId();
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
			cout << "\n--- Current Products ---" << endl;
			cout << "| " << setw(2) << "ID" << " | " << left << setw(39) << "Name" << " | " 
					<< setw(6) << "Price" << " | " 
					<< setw(16) << "Category Name" << " | " << setw(8) << "Quantity" << " | " 
					<< setw(26) << "Description" << " |" << endl;
			cout << string(114, '-') << endl;
			product->loadProducts();
			category->loadCategory();
			product->productStack.Functraverse([&](Product* p) {
				Product::displaySingleProduct(p, *category);
			});
			cout << "\n--- Edit Product ---" << endl;
			cout << "Enter Product ID to edit: ";
			int id;
			if(!(cin >> id)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				throw invalid_argument("Invalid product ID!");
			}
			cin.ignore();
			Product* existing = nullptr;
			product->loadProducts();
			product->productStack.Functraverse([&](Product* p) {
				if(p->id == id) existing = p;
			});
			if(!existing) throw runtime_error("Product not found!");
			cout << "\nCurrent Product Details:" << endl;
			Product::displaySingleProduct(existing, *category);
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
			cout << "\nConfirm update? (Y/N): ";
			char confirm;
			cin >> confirm;
			cin.ignore();
			if(toupper(confirm) == 'Y') {
				ifstream inFile("product.txt");
				ofstream outFile("temp_product.txt");
				string line;
				getline(inFile, line);
				outFile << line << endl;
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
			cout << "\n--- Current Products ---" << endl;
			cout << "| " << setw(2) << "ID" << " | " << left << setw(39) << "Name" << " | " 
					<< setw(6) << "Price" << " | " 
					<< setw(16) << "Category Name" << " | " << setw(8) << "Quantity" << " | " 
					<< setw(26) << "Description" << " |" << endl;
			cout << string(114, '-') << endl;
			product->loadProducts();
			category->loadCategory();
			product->productStack.Functraverse([&](Product* p) {
				Product::displaySingleProduct(p, *category);
			});
			cout << "\n--- Delete Product ---" << endl;
			cout << "Enter Product ID to delete: ";
			int id;
			if(!(cin >> id)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				throw invalid_argument("Invalid product ID!");
			}
			cin.ignore();
			Product* toDelete = nullptr;
			product->loadProducts();
			product->productStack.Functraverse([&](Product* p) {
				if(p->id == id) toDelete = p;
			});
			if(!toDelete) throw runtime_error("Product not found!");
			cout << "\nProduct to Delete:" << endl;
			Product::displaySingleProduct(toDelete, *category);
			cout << "\nARE YOU SURE YOU WANT TO DELETE THIS PRODUCT? (Y/N): ";
			char confirm;
			cin >> confirm;
			cin.ignore();
			if(toupper(confirm) == 'Y') {
				ifstream inFile("product.txt");
				ofstream outFile("temp_product.txt");
				string line;
				getline(inFile, line);
				outFile << line << endl;
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
			cout << "1. Search by Name" << endl;
			cout << "2. Search by Category" << endl;
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
				case 2: {
					system("cls");
					int catId = category->displayCategoryAndChoose("admin");
					product->productStack.Functraverse([&](Product* p) {
						if (p->categoryId == catId && resultCount < MAX_RESULTS) {
							results[resultCount++] = p;
						}
					});
					cout << "Press Enter to continue...";
					cin.get();
					break;
				}
				default:
					throw invalid_argument("Invalid choice!");
			}
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
		int generateProductId() {// generate by chatgpt to auto generate product id
			int lastId = 0;
			product->loadProducts();
			product->productStack.Functraverse([&](Product* p) {
				if(p->id > lastId) lastId = p->id;
			});
			return lastId + 1;
		}
};
class OrderManagement : public AdminOperation {// OrderManagement 
	private:
	    struct OrderItem {
	        int productId;
	        string productName;
	        int quantity;
	        double price;
	        double subtotal;
	        OrderItem* next;
	    };
	    struct Order {
	        int id;
	        int userId;
	        string date;
	        string status;
	        OrderItem* items;
	        double total;
	        Order* next;
	    };
	    Order* ordersHead;
	    Product* productSystem;
		Product product;
	public:
	    OrderManagement(Product* p = nullptr) : productSystem(p), ordersHead(nullptr) {}
	    ~OrderManagement() {
	        clearOrders();
	    }
	    void execute() override {
	        loadOrders();
	        int choice;
	        do {
	            system("cls");
	            cout << "====================================\n";
	            cout << "||        ORDER MANAGEMENT        ||\n";
	            cout << "====================================\n";
	            cout << "1. View All Orders\n";
	            cout << "2. Update Order Status\n";
	            cout << "3. Search Orders\n";
	            cout << "0. Back to Main Menu\n";
	            cout << "====================================\n";
	            cout << "Choice: ";
	            if (!(cin >> choice)) {
	                cin.clear();
	                cin.ignore(numeric_limits<streamsize>::max(), '\n');
	                cout << "Invalid input. Please try again.\n";
	                this_thread::sleep_for(chrono::seconds(1));
	                continue;
	            }
	            switch (choice) {
	                case 1:
	                    displayAllOrders("ALL");
	                    break;
	                case 2:
	                    updateOrderStatus();
	                    break;
	                case 3:
	                    searchOrders();
	                    break;
	                case 0:
	                    cout << "Returning to main menu...\n";
	                    break;
	                default:
	                    cout << "Invalid choice. Please try again.\n";
	                    this_thread::sleep_for(chrono::seconds(1));
	            }
	            if (choice != 0 && choice >= 1 && choice <= 3) {
	                cout << "Press Enter to continue...";
	                cin.ignore();
	                cin.get();
	            }
	        } while (choice != 0);
	    }
	    void clearOrders() {
	        Order* currentOrder = ordersHead;
	        while (currentOrder != nullptr) {
	            OrderItem* currentItem = currentOrder->items;
	            while (currentItem != nullptr) {
	                OrderItem* tempItem = currentItem;
	                currentItem = currentItem->next;
	                delete tempItem;
	            }
	            Order* tempOrder = currentOrder;
	            currentOrder = currentOrder->next;
	            delete tempOrder;
	        }
	        ordersHead = nullptr;
	    }
	    void loadOrders() {
	        clearOrders();
	        ifstream file("orderhistory.txt");
	        if (!file) {
	            cout << "No order history found." << endl;
	            return;
	        }
	        string line;
	        getline(file, line); // Skip header
	        Order* currentOrder = nullptr;
	        int currentOrderId = -1;
	        while (getline(file, line)) {
	            stringstream ss(line);
	            string token;
	            int orderId, userId, productId, quantity;
	            double total;
	            string date, status;
	            getline(ss, token, '|'); orderId = stoi(token);
	            getline(ss, token, '|'); userId = stoi(token);
	            getline(ss, token, '|'); productId = stoi(token);
	            getline(ss, token, '|'); quantity = stoi(token);
	            getline(ss, token, '|'); total = stod(token);
	            getline(ss, date, '|');
	            getline(ss, status);
	            if (orderId != currentOrderId) {
	                Order* newOrder = new Order();// New order
	                newOrder->id = orderId;
	                newOrder->userId = userId;
	                newOrder->date = date;
	                newOrder->status = status;
	                newOrder->total = 0;
	                newOrder->items = nullptr;
	                newOrder->next = nullptr;
	                if (ordersHead == nullptr) {
	                    ordersHead = newOrder;
	                } else {
	                    Order* temp = ordersHead;
	                    while (temp->next != nullptr) {
	                        temp = temp->next;
	                    }
	                    temp->next = newOrder;
	                }
	                currentOrder = newOrder;
	                currentOrderId = orderId;
	            }
	            string productName = "Unknown Product";// Get product name
	            if (productSystem) {
	                productSystem->loadProducts();
	                productSystem->productStack.Functraverse([&](Product* p) {
	                    if (p->id == productId) {
	                        productName = p->name;
	                    }
	                });
	            }
	            OrderItem* newItem = new OrderItem();// Add item to current order
	            newItem->productId = productId;
	            newItem->productName = productName;
	            newItem->quantity = quantity;
	            newItem->price = total / quantity;
	            newItem->subtotal = total;
	            newItem->next = nullptr;
	            if (currentOrder->items == nullptr) {
	                currentOrder->items = newItem;
	            } else {
	                OrderItem* temp = currentOrder->items;
	                while (temp->next != nullptr) {
	                    temp = temp->next;
	                }
	                temp->next = newItem;
	            }
	            currentOrder->total += total;
	        }
	        file.close();
	    }
	    void saveOrders() {
	        ofstream file("orderhistory.txt");
	        file << "Order ID|User ID|Product ID|Quantity|Total Price|Order Date|Status\n";
	        Order* currentOrder = ordersHead;
	        while (currentOrder != nullptr) {
	            OrderItem* currentItem = currentOrder->items;
	            while (currentItem != nullptr) {
	                file << currentOrder->id << "|" << currentOrder->userId << "|" << currentItem->productId << "|"
	                     << currentItem->quantity << "|" << fixed << setprecision(2) << currentItem->subtotal << "|"
	                     << currentOrder->date << "|" << currentOrder->status << "\n";
	                currentItem = currentItem->next;
	            }
	            currentOrder = currentOrder->next;
	        }
	        file.close();
	    }
	    void displayAllOrders(string displaytype) {
	        system("cls");
	        if (ordersHead == nullptr) {
	            cout << "No orders found." << endl;
	            return;
	        }
	        cout << "============================== ORDER HISTORY ==============================\n";
	        Order* currentOrder = ordersHead;
			if(displaytype == "ALL"){
				while (currentOrder != nullptr) {
					cout << "+------------------------------------------------------------------+\n";
					cout << "| Order ID: " << setw(54) << left << currentOrder->id << " |\n";
					cout << "| Date: " << setw(58) << left << currentOrder->date << " |\n";
					cout << "| Status: " << setw(56) << left << currentOrder->status << " |\n";
					cout << "+------------------------------------------------------------------+\n";
					cout << "| Product Name                      | Qty | Price    | Subtotal    |\n";
					cout << "+------------------------------------------------------------------+\n";
					OrderItem* currentItem = currentOrder->items;
					while (currentItem != nullptr) {
						string productName = truncateString(product.getProductNameFromStackById(currentItem->productId), 30);
						cout << "| " << setw(33) << left << productName << " | " 
							<< setw(3) << right << currentItem->quantity << " | " 
							<< setw(8) << right << fixed << setprecision(2) << currentItem->price << " | "
							<< setw(11) << right << fixed << setprecision(2) << currentItem->subtotal << " |\n";
						currentItem = currentItem->next;
					}
					cout << "+------------------------------------------------------------------+\n";
					cout << "| Order Total:                          RM " << setw(23) << fixed << setprecision(2) << currentOrder->total << " |\n";
					cout << "====================================================================\n\n";
					currentOrder = currentOrder->next;
				}
			}else{
				while (currentOrder != nullptr) {
					if(currentOrder->status != "Completed"){
						cout << "+------------------------------------------------------------------+\n";
						cout << "| Order ID: " << setw(54) << left << currentOrder->id << " |\n";
						cout << "| Date: " << setw(58) << left << currentOrder->date << " |\n";
						cout << "| Status: " << setw(56) << left << currentOrder->status << " |\n";
						cout << "+------------------------------------------------------------------+\n";
						cout << "| Product Name                      | Qty | Price    | Subtotal    |\n";
						cout << "+------------------------------------------------------------------+\n";
						OrderItem* currentItem = currentOrder->items;
						while (currentItem != nullptr) {
							// cout << currentItem->productId << endl;
							string productName = truncateString(product.getProductNameFromStackById(currentItem->productId), 30);
							cout << "| " << setw(33) << left << productName << " | " 
								<< setw(3) << right << currentItem->quantity << " | " 
								<< setw(8) << right << fixed << setprecision(2) << currentItem->price << " | "
								<< setw(11) << right << fixed << setprecision(2) << currentItem->subtotal << " |\n";
							currentItem = currentItem->next;
						}
						cout << "+------------------------------------------------------------------+\n";
						cout << "| Order Total:                          RM " << setw(23) << fixed << setprecision(2) << currentOrder->total << " |\n";
						cout << "====================================================================\n\n";
					}
					currentOrder = currentOrder->next;
				}
			}
	    }
	    void updateOrderStatus() {
	        displayAllOrders("PD");
	        if (ordersHead == nullptr) {
	            return;
	        }
	        cout << "Enter Order ID to update status (0 to cancel): ";
	        int orderId;
	        if (!(cin >> orderId) || orderId == 0) {
	            cin.clear();
	            cin.ignore();
	            return;
	        }
	        Order* currentOrder = ordersHead;
	        bool found = false;
	        while (currentOrder != nullptr) {
	            if (currentOrder->id == orderId) {
	                found = true;
	                cout << "\nCurrent Status: " << currentOrder->status << endl;
	                string newStatus;// Determine next status
	                if (currentOrder->status == "Preparing") {
	                    newStatus = "Delivery";
	                } else if (currentOrder->status == "Delivery") {
	                    newStatus = "Complete";
	                } else {
	                    cout << "This order is already completed and cannot be modified.\n";
	                    return;
	                }
	                cout << "Next status will be: " << newStatus << endl;
	                cout << "Confirm update? (Y/N): ";
	                char confirm;
	                cin >> confirm;
	                cin.ignore();
	                if (toupper(confirm) == 'Y') {
	                    currentOrder->status = newStatus;
	                    saveOrders();
	                    cout << "Order status updated successfully!\n";
	                } else {
	                    cout << "Update cancelled.\n";
	                }
	                break;
	            }
	            currentOrder = currentOrder->next;
	        }
	        if (!found) {
	            cout << "Order not found.\n";
	        }
	    }
	    void searchOrders() {
	        system("cls");
	        if (ordersHead == nullptr) {
	            cout << "No orders available to search.\n";
	            return;
	        }
	        cout << "Search Orders By:\n";
	        cout << "1. Order ID\n";
	        cout << "2. Customer ID\n";
	        cout << "3. Status\n";
	        cout << "0. Cancel\n";
	        cout << "Choice: ";
	        int choice;
	        if (!(cin >> choice)) {
	            cin.clear();
	            cin.ignore();
	            cout << "Invalid input.\n";
	            return;
	        }
	        Order* resultsHead = nullptr;
	        Order* resultsTail = nullptr;
	        cin.ignore();
	        switch (choice) {
	            case 1: {
	                cout << "Enter Order ID: ";
	                int id;
	                cin >> id;
	                cin.ignore();
	                Order* current = ordersHead;
	                while (current != nullptr) {
	                    if (current->id == id) {
	                        Order* newNode = copyOrder(current);
	                        if (resultsHead == nullptr) {
	                            resultsHead = resultsTail = newNode;
	                        } else {
	                            resultsTail->next = newNode;
	                            resultsTail = newNode;
	                        }
	                    }
	                    current = current->next;
	                }
	                break;
	            }
	            case 2: {
	                cout << "Enter Customer ID: ";
	                int userId;
	                cin >> userId;
	                cin.ignore();
	                Order* current = ordersHead;
	                while (current != nullptr) {
	                    if (current->userId == userId) {
	                        Order* newNode = copyOrder(current);
	                        if (resultsHead == nullptr) {
	                            resultsHead = resultsTail = newNode;
	                        } else {
	                            resultsTail->next = newNode;
	                            resultsTail = newNode;
	                        }
	                    }
	                    current = current->next;
	                }
	                break;
	            }
	            case 3: {
	                cout << "Enter Status (Preparing/Delivery/Complete): ";
	                string status;
	                getline(cin, status);
	                Order* current = ordersHead;
	                while (current != nullptr) {
	                    if (current->status == status) {
	                        Order* newNode = copyOrder(current);
	                        if (resultsHead == nullptr) {
	                            resultsHead = resultsTail = newNode;
	                        } else {
	                            resultsTail->next = newNode;
	                            resultsTail = newNode;
	                        }
	                    }
	                    current = current->next;
	                }
	                break;
	            }
	            case 0:
	                return;
	            default:
	                cout << "Invalid choice.\n";
	                return;
	        }
	        if (resultsHead == nullptr) {// Display search results
	            cout << "No orders found matching your criteria.\n";
	        } else {
	            cout << "\n=== SEARCH RESULTS ===\n";
	            Order* current = resultsHead;
	            while (current != nullptr) {
	                cout << "+------------------------------------------------------------------+\n";
	                cout << "| Order ID: " << setw(54) << left << current->id << " |\n";
	                cout << "| Date: " << setw(58) << left << current->date << " |\n";
	                cout << "| Status: " << setw(56) << left << current->status << " |\n";
	                cout << "+------------------------------------------------------------------+\n";
	                cout << "| Product Name                      | Qty | Price    | Subtotal    |\n";
	                cout << "+------------------------------------------------------------------+\n";
	                OrderItem* currentItem = current->items;
	                while (currentItem != nullptr) {
	                    cout << "| " << setw(33) << left << truncateString(currentItem->productName, 33) << " | " 
	                         << setw(3) << right << currentItem->quantity << " | " 
	                         << setw(8) << right << fixed << setprecision(2) << currentItem->price << " | "
	                         << setw(11) << right << fixed << setprecision(2) << currentItem->subtotal << " |\n";
	                    currentItem = currentItem->next;
	                }
	                cout << "+------------------------------------------------------------------+\n";
	                cout << "| Order Total:                          RM " << setw(23) << fixed << setprecision(2) << current->total << " |\n";
	                cout << "====================================================================\n\n";
	                current = current->next;
	            }
	        }
	        Order* temp = resultsHead;// Clean up results
	        while (temp != nullptr) {
	            Order* next = temp->next;
	            deleteOrder(temp);
	            temp = next;
	        }
	    }
	    Order* copyOrder(Order* original) {
	        Order* newOrder = new Order();
	        newOrder->id = original->id;
	        newOrder->userId = original->userId;
	        newOrder->date = original->date;
	        newOrder->status = original->status;
	        newOrder->total = original->total;
	        newOrder->items = nullptr;
	        newOrder->next = nullptr;
	        OrderItem* originalItem = original->items;
	        OrderItem* newItemTail = nullptr;
	        while (originalItem != nullptr) {
	            OrderItem* newItem = new OrderItem();
	            newItem->productId = originalItem->productId;
	            newItem->productName = originalItem->productName;
	            newItem->quantity = originalItem->quantity;
	            newItem->price = originalItem->price;
	            newItem->subtotal = originalItem->subtotal;
	            newItem->next = nullptr;
	            if (newOrder->items == nullptr) {
	                newOrder->items = newItem;
	            } else {
	                newItemTail->next = newItem;
	            }
	            newItemTail = newItem;
	            originalItem = originalItem->next;
	        }
	        return newOrder;
	    }
	    void deleteOrder(Order* order) {
	        OrderItem* currentItem = order->items;
	        while (currentItem != nullptr) {
	            OrderItem* temp = currentItem;
	            currentItem = currentItem->next;
	            delete temp;
	        }
	        delete order;
	    }
};
class ReportGeneration : public AdminOperation {// ReportGeneration
private:
    struct SalesData {
        string date;
        double totalSales;
        int orderCount;
    };
    struct OrderDetail {
        string productName;
        double amount;
    };
    static const int MAX_RECORDS = 1000;
    static const int MAX_ORDER_DETAILS = 1000;
    static const int MAX_UNIQUE_ORDERS = 1000;
    Product* product;
    Category* category;
public:
    ReportGeneration(Product* p = nullptr, Category* c = nullptr) : product(p), category(c) {}
    void execute() override {
        int choice;
        do {
            system("cls");
            cout << "====================================\n";
            cout << "||        REPORT MENU             ||\n";
            cout << "====================================\n";
            cout << "1. Daily Order Report\n";
            cout << "2. Monthly Order Report\n";
            cout << "3. Annual Order Report\n";
            cout << "0. Back to Main Menu\n";
            cout << "====================================\n";
            cout << "Choice: ";
            cin >> choice;
            cin.ignore();
            switch(choice) {
                case 1: generateDailyReport(); break;
                case 2: generateMonthlyReport(); break;
                case 3: generateAnnualReport(); break;
                case 0: cout << "Returning to main menu...\n"; break;
                default: cout << "Invalid choice!\n";
            }
            if (choice != 0) {
                cout << "\nPress Enter to continue...";
                cin.get();
            }
        } while(choice != 0);
    }
    bool isOrderIdUnique(int orderId, int orderIds[], int count) {// generate by chatgpt 
        for (int i = 0; i < count; i++) {
            if (orderIds[i] == orderId) {
                return false;
            }
        }
        return true;
    }
    void addUniqueOrderId(int orderId, int orderIds[], int& count) {// generate by chatgpt
        if (count < MAX_UNIQUE_ORDERS && isOrderIdUnique(orderId, orderIds, count)) {
            orderIds[count++] = orderId;
        }
    }
    void generateDailyReport() {
		system("cls");
		cout << "=== DAILY ORDER REPORT ===\n";
		string inputDate;
		cout << "Enter date (YYYY-MM-DD): ";
		getline(cin, inputDate);
		bool validDate = (inputDate.length() == 10) && 
						(inputDate[4] == '-') && 
						(inputDate[7] == '-');
		if (!validDate) {
			cout << "Invalid date format. Please use YYYY-MM-DD.\n";
			cout << "Press Enter to continue...";
			cin.get();
			return;
		}
		ifstream orderFile("orderhistory.txt");
		if (!orderFile) {
			cout << "Error: Cannot open orderhistory.txt file!\n";
			cout << "Press Enter to continue...";
			cin.get();
			return;
		}
		const int MAX_ORDER_DETAILS = 1000;
		const int MAX_UNIQUE_ORDERS = 1000;
		struct OrderDetail {
			char productName[100];
			double amount;
		};
		OrderDetail orderDetails[MAX_ORDER_DETAILS];
		int orderDetailsCount = 0;
		int uniqueOrderIds[MAX_UNIQUE_ORDERS];
		int uniqueOrderCount = 0;
		double dailyTotal = 0.0;
		string line;
		getline(orderFile, line);
		while (getline(orderFile, line) && orderDetailsCount < MAX_ORDER_DETAILS && uniqueOrderCount < MAX_UNIQUE_ORDERS) {
			char* tokens[10];
			int tokenCount = 0;
			char* lineCopy = new char[line.length() + 1];
			strcpy(lineCopy, line.c_str());
			char* token = strtok(lineCopy, "|");
			while (token != nullptr && tokenCount < 10) {
				tokens[tokenCount++] = token;
				token = strtok(nullptr, "|");
			}
			if (tokenCount < 7) {
				delete[] lineCopy;
				continue;
			}
			int orderId = atoi(tokens[0]);
			int productId = atoi(tokens[2]);
			int quantity = atoi(tokens[3]);
			double total = atof(tokens[4]);
			string date = tokens[5];
			if (date.substr(0, 10) == inputDate) {
				bool isUnique = true;
				for (int i = 0; i < uniqueOrderCount; i++) {
					if (uniqueOrderIds[i] == orderId) {
						isUnique = false;
						break;
					}
				}
				if (isUnique) {
					uniqueOrderIds[uniqueOrderCount++] = orderId;
				}
				dailyTotal += total;
			}
			delete[] lineCopy;
		}
		orderFile.close();
		cout << "\nDaily Report for " << inputDate << "\n";
		cout << "==========================================\n";
		cout << "Total Orders: " << uniqueOrderCount << "\n";
		cout << "Total Sales: RM" << fixed << setprecision(2) << dailyTotal << "\n";
		cout << "==========================================\n";
	}
    void generateMonthlyReport() {
        system("cls");
        cout << "=== MONTHLY ORDER REPORT ===\n";
        string inputMonth;
        cout << "Enter month (YYYY-MM): ";
        getline(cin, inputMonth);
        if (inputMonth.length() != 7 || inputMonth[4] != '-') {
            cout << "Invalid month format. Please use YYYY-MM.\n";
            return;
        }
        ifstream orderFile("orderhistory.txt");
        if (!orderFile) {
            cout << "No order data available.\n";
            return;
        }
        string line;
        getline(orderFile, line); // Skip header
        SalesData dailyData[MAX_RECORDS];
        int dailyDataCount = 0;
        double monthlyTotal = 0;
        int uniqueOrderIds[MAX_UNIQUE_ORDERS];
        int uniqueOrderCount = 0;
        while (getline(orderFile, line) && dailyDataCount < MAX_RECORDS) {
            stringstream ss(line);
            string token;
            int orderId;
            double total;
            string date;
            getline(ss, token, '|'); orderId = stoi(token);
            getline(ss, token, '|'); // userId
            getline(ss, token, '|'); // productId
            getline(ss, token, '|'); // quantity
            getline(ss, token, '|'); total = stod(token);
            getline(ss, date, '|');
            if (date.substr(0, 7) == inputMonth) {
                string day = date.substr(0, 10);
                addUniqueOrderId(orderId, uniqueOrderIds, uniqueOrderCount);// Track unique orders
                monthlyTotal += total;// Add to monthly total
                bool found = false;// Find or create daily entry
                for (int i = 0; i < dailyDataCount; i++) {
                    if (dailyData[i].date == day) {
                        dailyData[i].totalSales += total;
                        dailyData[i].orderCount = uniqueOrderCount;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    dailyData[dailyDataCount].date = day;
                    dailyData[dailyDataCount].totalSales = total;
                    dailyData[dailyDataCount].orderCount = 1;
                    dailyDataCount++;
                }
            }
        }
        orderFile.close();
        cout << "\nMonthly Report for " << inputMonth << endl;// Display report
        cout << "+------------+--------------+-----------------+" << endl;
        cout << "| Date       | Total Sales  |  No of Orders   |" << endl;
        cout << "+------------+--------------+-----------------+" << endl;
        for (int i = 0; i < dailyDataCount; i++) {
            cout << "| " << setw(10) << dailyData[i].date << " | RM" 
                 << setw(10) << fixed << setprecision(2) << dailyData[i].totalSales 
                 << " | " << setw(15) << dailyData[i].orderCount << " |\n";
        }
        cout << "+------------+--------------+-----------------+" << endl;
        cout << "| Monthly Total: RM" << setw(10) << monthlyTotal 
             << " | Orders: " << setw(5) << uniqueOrderCount << " |" << endl;
        cout << "+---------------------------------------------+" << endl;
    }
    void generateAnnualReport() {
        system("cls");
        cout << "=== ANNUAL ORDER REPORT ===\n";
        string inputYear;
        cout << "Enter year (YYYY): ";
        getline(cin, inputYear);
        if (inputYear.length() != 4) {
            cout << "Invalid year format. Please use YYYY.\n";
            return;
        }
        ifstream orderFile("orderhistory.txt");
        if (!orderFile) {
            cout << "No order data available.\n";
            return;
        }
        string line;
        getline(orderFile, line); // Skip header
        SalesData monthlyData[12];
        int monthlyDataCount = 0;
        double annualTotal = 0;
        int uniqueOrderIds[MAX_UNIQUE_ORDERS];
        int uniqueOrderCount = 0;
        while (getline(orderFile, line)) {
            stringstream ss(line);
            string token;
            int orderId;
            double total;
            string date;
            getline(ss, token, '|'); orderId = stoi(token);
            getline(ss, token, '|'); // userId
            getline(ss, token, '|'); // productId
            getline(ss, token, '|'); // quantity
            getline(ss, token, '|'); total = stod(token);
            getline(ss, date, '|');
            if (date.substr(0, 4) == inputYear) {
                string month = date.substr(0, 7);
                addUniqueOrderId(orderId, uniqueOrderIds, uniqueOrderCount);// Track unique orders
                annualTotal += total;// Add to annual total
                bool found = false;// Find or create monthly entry
                for (int i = 0; i < monthlyDataCount; i++) {
                    if (monthlyData[i].date == month) {
                        monthlyData[i].totalSales += total;
                        monthlyData[i].orderCount = uniqueOrderCount;
                        found = true;
                        break;
                    }
                }
                if (!found && monthlyDataCount < 12) {
                    monthlyData[monthlyDataCount].date = month;
                    monthlyData[monthlyDataCount].totalSales = total;
                    monthlyData[monthlyDataCount].orderCount = 1;
                    monthlyDataCount++;
                }
            }
        }
        orderFile.close();
        cout << "\nAnnual Report for " << inputYear << endl;
        cout << "+---------+--------------+----------------------+" << endl;
        cout << "| Month   | Total Sales  |     No of Orders     |" << endl;
        cout << "+---------+--------------+----------------------+" << endl;
        for (int i = 0; i < monthlyDataCount; i++) {
            cout << "| " << setw(7) << monthlyData[i].date << " | RM" 
                 << setw(10) << fixed << setprecision(2) << monthlyData[i].totalSales 
                 << " | " << setw(20) << monthlyData[i].orderCount << " |" << endl;
        }
        cout << "+---------+--------------+----------------------+" << endl;
        cout << "| Annual Total: RM" << setw(10) << annualTotal 
             << " | Orders: " << setw(9) << uniqueOrderCount << "|" << endl;
        cout << "+-----------------------------------------------+" << endl;
    }
};
class AdminModule {
	private:
	    Product p;
	    Category c;
	    UserLogin ul;
	public:
	    void admin_menu(int admintype) {
	        cout << string(28, '-') << endl;
	        cout << setw(20) << "Admin Dashboard" << endl;
	        cout << string(28, '-') << endl;
	        cout << "1. Manage Product" << endl;
	        cout << "2. Manage Order" << endl;
	        cout << "3. Report" << endl;
	        cout << "4. Profile" << endl;
			if(admintype == 2){
				cout << "5. Manage Staff" << endl;
			}
	        cout << "0. Logout" << endl;
	    }
	    void admin_main_page(int userId, int admintype) {
	        int choice;
	        do {
	            system("cls");
	            admin_menu(admintype);
	            cout << "Please Select the option: ";
	            cin >> choice;
	            try {
	                AdminOperation* operation = nullptr;
	                switch(choice) {
	                    case 1: 
	                        operation = new ProductManagement(&p, &c, admintype);
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
	                        profileMenu(userId, admintype);
	                        this_thread::sleep_for(chrono::seconds(1));
	                        break;
						case 5: 
	                        staffmanage();
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
	    void profileMenu(int userId, int admintype) {
	        int choice;
	        do {
	            system("cls");
	            cout << "=== PROFILE MENU ===" << endl;
	            cout << "1. View Profile" << endl;
	            cout << "2. Edit Profile" << endl;
	            cout << "0. Back" << endl;
	            cout << "Choice: ";
	            cin >> choice;
	            cin.ignore();
	            switch(choice) {
	                case 1:
	                    ul.viewProfile(userId, admintype);
	                    cout << "\nPress Enter to continue...";
	                    cin.get();
	                    break;
	                case 2:
	                    ul.editProfile(userId, admintype);
	                    break;
	                case 0:
	                    break;
	                default:
	                    cout << "Invalid choice!" << endl;
	                    this_thread::sleep_for(chrono::seconds(1));
	            }
	        } while(choice != 0);
	    }
		void staffmanage() {
	        int choice;
	        do {
	            system("cls");
	            cout << "=== Manage Staff MENU ===" << endl;
	            cout << "1. View Staff" << endl;
	            cout << "2. Add Staff" << endl;
	            cout << "3. Delete Staff" << endl;
	            cout << "0. Back" << endl;
	            cout << "Choice: ";
	            cin >> choice;
	            cin.ignore();
	            switch(choice) {
	                case 1:
	                    viewAllStaff();
						cout << "Press Enter to continue...";
						cin.get();
	                    break;
	                case 2:
	                    addNewStaff();
	                    break;
					case 3:
						deleteStaff();
						break;
	                case 0:
	                    break;
	                default:
	                    cout << "Invalid choice!" << endl;
	                    this_thread::sleep_for(chrono::seconds(1));
	            }
	        } while(choice != 0);
	    }
		void viewAllStaff() {
			system("cls");
			ifstream file("staff.txt");
			if (!file) {
				cout << "No staff records found!" << endl;
				cout << "Press Enter to continue...";
				cin.get();
				return;
			}
			cout << "=== STAFF LIST ===" << endl;
			cout << "+----+------------------+---------------------+----------------+" << endl;
			cout << "| ID | Username         | Email               | Phone          |" << endl;
			cout << "+----+------------------+---------------------+----------------+" << endl;
			string line;
			getline(file, line); // Skip header
			while (getline(file, line)) {
				string fields[6];
				int numFields = split(line, "|", fields, 6);
				if (numFields >= 6) {
					cout << "| " << setw(2) << trim(fields[0]) << " | " 
						<< setw(16) << truncateString(trim(fields[1]), 16) << " | "
						<< setw(19) << truncateString(trim(fields[4]), 19) << " | "
						<< setw(14) << truncateString(trim(fields[5]), 14) << " |" << endl;
				}
			}
			file.close();
			cout << "+----+------------------+---------------------+----------------+" << endl;
		}
		void addNewStaff() {
			system("cls");
			cout << "=== ADD NEW STAFF ===" << endl;
			string username, password, confirmPassword, email, phone;
			cout << "Username: ";
			getline(cin, username);
			cout << "Password: ";
			char ch;
			char pw[30];
			int i = 0;
			while ((ch = getch()) != '\r') {
				if (ch == '\b' && i > 0) {
					--i;
					cout << "\b \b";
				} else if (i < 29 && ch != '\r') {
					pw[i++] = ch;
					cout << '*';
				}
			}
			pw[i] = '\0';
			password = pw;
			cout << "\nConfirm Password: ";
			i = 0;
			while ((ch = getch()) != '\r') {
				if (ch == '\b' && i > 0) {
					--i;
					cout << "\b \b";
				} else if (i < 29 && ch != '\r') {
					pw[i++] = ch;
					cout << '*';
				}
			}
			pw[i] = '\0';
			confirmPassword = pw;
			if (password != confirmPassword) {
				cout << "\nPasswords do not match!" << endl;
				this_thread::sleep_for(chrono::seconds(1));
				return;
			}
			cout << "\nEmail: ";
			getline(cin, email);
			cout << "Phone (+60): ";
			getline(cin, phone);
			int newId = 1;// Generate new staff ID
			ifstream inFile("staff.txt");
			string lastLine;
			while (getline(inFile, lastLine)) {
				if (!lastLine.empty() && lastLine.find("id") == string::npos) {
					stringstream ss(lastLine);
					string token;
					getline(ss, token, '|');
					try {
						newId = stoi(token) + 1;
					} catch (...) {
						newId = 1;
					}
				}
			}
			inFile.close();
			ofstream outFile("staff.txt", ios::app);
			if (!outFile) {
				cout << "Error: Cannot open staff file!" << endl;
				return;
			}
			if (newId == 1) {
				outFile << "id|username|password|address|email|phone\n";
			}
			outFile << newId << "|" << username << "|" << password << "||" << email << "|" << phone << endl;
			outFile.close();
			cout << "\nStaff added successfully with ID: " << newId << endl;
			this_thread::sleep_for(chrono::seconds(1));
		}
		void deleteStaff() {
			viewAllStaff();
			cout << "\nEnter Staff ID to delete (0 to cancel): ";
			int staffId;
			cin >> staffId;
			cin.ignore();
			if (staffId == 0) return;
			ifstream inFile("staff.txt");
			ofstream tempFile("temp_staff.txt");
			string line;
			bool found = false;
			getline(inFile, line); // Skip header
			tempFile << line << endl;
			while (getline(inFile, line)) {
				stringstream ss(line);
				string token;
				getline(ss, token, '|');
				int currentId = stoi(token);
				if (currentId != staffId) {
					tempFile << line << endl;
				} else {
					found = true;
				}
			}
			inFile.close();
			tempFile.close();
			if (found) {
				remove("staff.txt");
				rename("temp_staff.txt", "staff.txt");
				cout << "Staff deleted successfully!" << endl;
			} else {
				remove("temp_staff.txt");
				cout << "Staff ID not found!" << endl;
			}
			this_thread::sleep_for(chrono::seconds(1));
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
            if (l.getpagetype() == 1) { // Customer
                UserModule um;
                um.setLoginUserID(l.getLoginUserID());// check the user id
                um.user_main_page(l.getpagetype());
            }else if(l.getpagetype() == 3 || l.getpagetype() == 2){
            	AdminModule am;
                am.admin_main_page(l.getLoginUserID(), l.getpagetype());
			}
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