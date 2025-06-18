/*
    Problem Statement : https://docs.google.com/document/d/14ssqJdFFEo14lEqXX9LzPdUVfoJ7hzSklzsSMrkWu_I/edit?usp=sharing
*/

#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class book
{
    private:
        string isbn;
        int copiesAvailable;
        int totalCopies;
    public:
        string title;
        string author;

        //constructors
        book(string Ititle,string author,string isbn,int IcopiesAvailable,int ItotalCopies);
        book(book& reference,string new_isbn);        

        //getters
        string getISBN() { return isbn; } 
        int getCopiesAvailable() { return copiesAvailable; }
        int getTotalCopies() { return totalCopies; }

        //utilities
        void updateCopies(int count);
        bool borrowBook();
        bool returnBook();
        void printDetails();          
};

class member
{
    private:
        string memberID;
        map<string,int> borrowedBooks;
        int borrowLimit;
        //int booksBorrowed = 0;
    public:
        string name;

        member(string ID,string Iname,int limit = 3) {
            memberID = ID;
            name = Iname;
            borrowLimit = limit;
        }

        //getters
        string getMemberID() { return memberID; }
        int getBorrowLimit() { return borrowLimit; }

        //utilities
        bool borrowBook(string isbn);
        bool returnBook(string isbn);
        void printDetails();
};

class Library
{
    private:
        vector<book> AllBooks;
        vector<member> AllMembers;
        map<string,book*> Allbooks;
        map<string,member*> Allmembers;
    public:
        Library() {
            AllBooks.reserve(50);         //reserving space so that using push_back doesn't change the address of the elements
            AllMembers.reserve(150);
        }

        bool addBook(book& A);
        bool registerMember(member& M);
        bool borrowBook(string memberID, string isbn);
        bool returnBook(string memberID, string isbn); 
        void printLibrary();

        book* searchBook(string isbn);
        member* searchMember(string ID);
};

book::book(string Ititle = "UnknownTitle",string Iauthor = "UnknownAuthor",string Iisbn = "ISBN", int IcopiesAvailable = 0, int ItotalCopies = 5) {
            title = Ititle;
            author = Iauthor;
            isbn = Iisbn;
            copiesAvailable = IcopiesAvailable;
            totalCopies = ItotalCopies;
        } 
book::book(book& reference, string new_isbn) {      
            title = reference.title;
            author = reference.author;
            isbn = new_isbn;
            copiesAvailable = reference.copiesAvailable;
            totalCopies = reference.totalCopies;
        }
void book::updateCopies(int count) {
            if(count + copiesAvailable >= 0) {
                copiesAvailable += count;
                totalCopies += count;
            }
            else cout << "Invalid request! Count becomes negative" << endl; 
        }

bool book::borrowBook() {
            if(copiesAvailable == 0) {
                cout << "Invalid request! Copy of book not available" << endl;
                return false;
            }
            else {
                --copiesAvailable;  
                return true;
            }
        }

bool book::returnBook() {
            if(copiesAvailable == totalCopies) {
                cout << "Invalid request! Copy of book exceeds total copies" << endl; 
                return false;
            }
            else {
                ++copiesAvailable;  
                return true;
            }
        }
void book::printDetails() {
            cout << title << " " << author << endl;
        }

bool member::borrowBook(string isbn) {
            if(borrowLimit == 0) {                      //BorrowLimit is the number of addional books which can be borrowed 
                cout << "Invalid request! Borrow limit exceeded" << endl;
                return false;
            }
            else  {
                borrowedBooks[isbn]++;
                    --borrowLimit;                      //keeping track of borrowed books by increasing and deccreasing borrowed limit
                    return true;
            }
        }
bool member::returnBook(string isbn) {
            auto it = borrowedBooks.find(isbn);
            if(it == borrowedBooks.end() || it->second == 0) {          //books which are borrowed and returned still exist in map 
                cout << "Invalid request! Book not borrowed" << endl;   //but the number of copies borrowed will be zero 
                return false;                                           //should check the number of borrowed copies if book exists in map
            }
            else {
                --it->second;                           //Decreases the number of copies borrowed until it is zero
                ++borrowLimit;
                return true;  
            }
        }
void member::printDetails() {
            for(auto x : borrowedBooks) {
                if(x.second == 0) continue;
                cout << memberID << " "<< name << " " << x.first << " " << x.second << endl;
            }
        }

bool Library::addBook(book& A) {
            string tempISBN = A.getISBN();
            auto itr = Allbooks.find(tempISBN);
            
            if(itr != Allbooks.end()) {
                cout << "Invalid request! Book with same isbn already exists" << endl;
                return false;
            } 
            
            AllBooks.push_back(A);
            Allbooks[tempISBN] = &AllBooks.back();
           
            return true;
        }

bool Library::registerMember(member& M) {
            string tempID = M.getMemberID();
            auto itr = Allmembers.find(tempID);
            
            if(itr != Allmembers.end()) {
                cout << "Invalid request! Member with same id already exists" << endl;
                return false;
            }
           
            AllMembers.push_back(M);
            Allmembers[tempID] = &AllMembers.back();
           
            return true;
        }
        
bool Library::borrowBook(string memberID, string isbn) {
            auto itr = Allbooks.find(isbn);
            if(itr == Allbooks.end()) {
                cout << "Invalid request! Book not found" << endl;
                return false;
            }
            
            if(!Allbooks[isbn]->getCopiesAvailable())  {
                cout << "Invalid request! Copy of book not available" << endl;
                return false;
            }
            
            auto mem = Allmembers.find(memberID);
            if(mem == Allmembers.end()) {
                cout << "Invalid request! No such member exists" << endl;
                return false;
            }
            
            if(Allmembers[memberID]->borrowBook(isbn)) { 
                Allbooks[isbn]->borrowBook();
                return true;
            }
            
            return false;
        }

bool Library::returnBook(string memberID, string isbn) {
            auto itr = Allbooks.find(isbn);
            if(itr == Allbooks.end()) {
                cout << "Invalid request! Book not found" << endl; 
                return false;
            }
            
            auto mem = Allmembers.find(memberID);
            if(mem == Allmembers.end()) {
                cout << "Invalid request! Member is not registered" << endl;
                return false;
            }
            
            if(itr->second->getCopiesAvailable() == itr->second->getTotalCopies()) 
                return Allbooks[isbn]->returnBook();        //if the if condition is satisfied then this funcion prints error meassage and returns false
            
            if(Allmembers[memberID]->returnBook(isbn)) {
                Allbooks[isbn]->returnBook();                   
                return true;
            }
    
            return false;
        }

void Library::printLibrary() {
            for(auto x : AllBooks) {
                cout << x.title << " " << x.author << " " << x.getCopiesAvailable() << endl; 
            }
            for(auto x : AllMembers) {
                cout << x.getMemberID() << " " << x.name << endl;
            }
        }

book* Library::searchBook(string isbn) {
            auto itr = Allbooks.find(isbn);
            if(itr == Allbooks.end()) return NULL;
            return Allbooks[isbn];
        }

member* Library::searchMember(string ID) {
            auto itr = Allmembers.find(ID);
            if(itr == Allmembers.end()) return NULL;
            return Allmembers[ID];
        }

int main()
{
    Library L;
    string s;
    cin >> s;
    while(s != "Done") {
        if(s == "Book") {
            string title;
            cin >> title;
           
            if(title == "ExistingBook") {
                string curISBN,newISBN;
                cin >> curISBN >> newISBN;
                
                book* old = L.searchBook(curISBN);
                if(old == NULL) { 
                    cout << "Invald request! Book with given ISBN does not exist" << endl; 
                    continue; 
                }

                book newBook(*old,newISBN);     
                L.addBook(newBook);
            }          
            else if(title == "None") {
                book newBook;
                L.addBook(newBook);
            }
            else {
                string author,isbn; 
                int copiesAvailable,totalCopies;
                cin >> author >> isbn >> copiesAvailable >> totalCopies;

                book newBook(title,author,isbn,copiesAvailable,totalCopies);
                L.addBook(newBook);
            }
        }
        else if(s == "UpdateCopiesCount") {
            string isbn;
            int countToChange;
            cin >> isbn >> countToChange;
           
            book* toChange = L.searchBook(isbn);
            if(toChange == NULL) {
                 cout << "Invalid request! No such Book exists" << endl; 
                 continue; 
            }
            
            toChange->updateCopies(countToChange);
        }
        else if(s == "Member") {
            string LimitChecker;
            cin >> LimitChecker;
           
            if(LimitChecker == "NoBorrowLimit") {
                string name,ID;
                cin >> ID >> name;
                
                member newMember(ID,name);
                L.registerMember(newMember);
            }
            else {
                string ID = LimitChecker;
                string name;
                int borrowLimit;
                cin >> name >> borrowLimit;
                
                member newMember(ID,name,borrowLimit);
                L.registerMember(newMember);
            }
        }
        else if(s == "Borrow") {
            string ID,ISBN;
            cin >> ID >> ISBN;
            L.borrowBook(ID,ISBN);
        }
        else if(s == "Return") {
            string ID,ISBN;
            cin >> ID >> ISBN;
            L.returnBook(ID,ISBN);
        }
        else if(s == "PrintBook") {
            string ISBN;
            cin >> ISBN;
            
            book* toPrint = L.searchBook(ISBN);
            if(toPrint == NULL) {
                cout << "Invalid request! Book with given ISBN does not exist" << endl;
                continue;
            }
            
            toPrint->printDetails();
        }
        else if(s == "PrintMember") {
            string ID;
            cin >> ID;
            
            member* toPrint = L.searchMember(ID);
            if(toPrint == NULL) {
                cout << "Invallid request! No such member is registered" << endl; 
                continue;
            }
            
            toPrint->printDetails();
        }
        else if(s == "PrintLibrary") {
            L.printLibrary();
        }
        
        cin >> s;
    }
}