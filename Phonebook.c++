#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

/* This person class will be the data that is held at each node in the Tree.
   Each person has a last name, first name, and phone number, all represented as strings
*/
class Person {
private:
   std::string firstName{};
   std::string lastName{};
   std::string phoneNumber{}; //The phone number should not have any special characters in it, just raw numbers

public:   
   //Constructor that accepts references to the names and number of a new person upon creation
   Person(const std::string& firstname, const std::string& lastname, const std::string& phonenumber)
      : firstName(firstname), lastName(lastname), phoneNumber(phonenumber) {}

   //constructor that accepts references to only names for find function
   Person(const std::string& firstname, const std::string& lastname)
      : firstName(firstname), lastName(lastname) {}

   std::string getFirstName() const { return firstName; }
   std::string getLastName() const { return lastName; }
   std::string getPhoneNumber() const { return phoneNumber; }

   void setPhoneNumber(const std::string& phone) { phoneNumber = phone; }

   //ensures that names are being entered alphabetically according to last name
   bool operator<(const Person& other) const {
        if (lastName < other.lastName) return true;
        if (lastName == other.lastName && firstName < other.firstName) return true;
        return false;
    }

};

class Book {
   private:

   struct Treenode {
      Person data;
      Treenode* leftChild;
      Treenode* rightChild;
      Treenode(const Person& p) : data(p), leftChild(nullptr), rightChild(nullptr) {}
   };

   Treenode* root;

   void insert(Treenode*& root, const Person& p) {
      if (!root) root = new Treenode(p);
      else if (p < root->data) insert(root->leftChild, p);
      else insert(root->rightChild, p);
   }

    void inorder(Treenode* root) {
      if (!root) return;
      inorder(root->leftChild);
      std::cout << root->data.getFirstName() << " " << root->data.getLastName() << ": " << root->data.getPhoneNumber() << std::endl;
      inorder(root->rightChild);
   }

   Treenode* deleteNode(Treenode* root, const Person& key) {
      if (!root) return root;

      // Recursive cases
      if (key < root->data) 
         root->leftChild = deleteNode(root->leftChild, key);
      else if (root->data < key)
         root->rightChild = deleteNode(root->rightChild, key);
      else {
         // Node with only one child or no child
         if (!root->leftChild) {
            Treenode* temp = root->rightChild;
            delete root;
            return temp;
         } else if (!root->rightChild) {
            Treenode* temp = root->leftChild;
            delete root;
            return temp;
         }

         // Node with two children: get the inorder successor
         Treenode* temp = minValueNode(root->rightChild);
         root->data = temp->data;
         root->rightChild = deleteNode(root->rightChild, temp->data);
      }
      return root;
   }

   Treenode* minValueNode(Treenode* node) {
      Treenode* current = node;
      while (current && current->leftChild) current = current->leftChild;
      return current;
   }

   Treenode* search(Treenode* root, const Person& key) {
      if (!root || (root->data.getFirstName() == key.getFirstName() && root->data.getLastName() == key.getLastName())) 
         return root;
      if (key < root->data)
         return search(root->leftChild, key);
      return search(root->rightChild, key);
   }

   void saveInorderToFile(std::ofstream& file, Treenode* root) {
      if (!root) return;
      saveInorderToFile(file, root->leftChild);
      file << root->data.getFirstName() << " "
         << root->data.getLastName() << " "
         << root->data.getPhoneNumber() << std::endl;
      saveInorderToFile(file, root->rightChild);
   }

   public:
   Book() : root(nullptr) {}

   // Add: Adds a person’s name (first and last) and phone number to the phone book.
   /*
      Checks to see if the given node is null (always start at root)
      if it is then theres the open spot to add the new node
      If it isn't then we compare if person is higher or lower alphabetically using our operator <
      and recursively call the function again with the appropriate child
   */
   void add(const Person& p) { insert(root, p); }

   // Remove: Deletes a given person’s phone number, given only the name. **Formerly delete, but delete is a keyword so we can't call then function that.
   void remove(const std::string& firstName, const std::string& lastName) {
      Person p(firstName, lastName);
      root = deleteNode(root, p);
   }

   // Find: Locates a person’s phone number, given only the person’s name.
   std::string find(const std::string& firstName, const std::string& lastName) {
    Person p(firstName, lastName);
    Treenode* result = search(root, p);
    if (result)
        return result->data.getPhoneNumber();
    else
        return "Not Found";
   }

   // Change: Changes a person’s phone number given the person’s name and new phone number.
   void changePhoneNumber(const std::string& firstName, const std::string& lastName, const std::string& newPhone) {
      Person p(firstName, lastName);
      Treenode* result = search(root, p); // Using the search function we added earlier
      if (result) {
         result->data.setPhoneNumber(newPhone);
         std::cout << "Phone number updated successfully!" << std::endl;
      } else {
         std::cout << "Person not found in the phonebook." << std::endl;
   }
}

   // Display: Dumps entire phone book in alphabetical order.
   void display(){
      inorderTraversal(root);
   }

   void inorderTraversal(Treenode* node){
      if (!root) return;
        inorder(root->leftChild);
        std::cout << root->data.getFirstName() << " " << root->data.getLastName() << ": " << root->data.getPhoneNumber() << std::endl;
        inorder(root->rightChild);
   }

   void saveToFile(const std::string& filename) {
      std::ofstream file(filename);
      if (!file.is_open()) {
         std::cout << "Failed to open file" << std::endl;
         return;
      }
      saveInorderToFile(file, root);
      file.close();
      std::cout << "Phonebook saved to " << filename << std::endl;
   }

   void loadFromFile(const std::string& filename) {
      std::ifstream file(filename);
      if (!file.is_open()) {
         std::cout << "Failed to open the file or current phonebook does not exist. Starting with an empty phonebook." << std::endl;
         return;
      }
      std::string firstName, lastName, phoneNumber;
      while (file >> firstName >> lastName >> phoneNumber) {
         add(Person(firstName, lastName, phoneNumber));
      }
      file.close();
      std::cout << "Phonebook loaded from " << filename << std::endl;
   }
};

class UserInterface {
private:
    Book phonebook;

public:
   void displayMenu() {
    std::cout << "\nMenu:" << std::endl;
    std::cout << "1. Add" << std::endl;
    std::cout << "2. Display" << std::endl;
    std::cout << "3. Delete" << std::endl;
    std::cout << "4. Find" << std::endl;
    std::cout << "5. Change Phone Number" << std::endl;
    std::cout << "6. Quit" << std::endl;
}


   void executeChoice(int choice) {
      std::string fName, lName, phone;

      switch(choice) {
         case 1:
            std::cout << "Enter first name, last name, and phone number:" << std::endl;
            std::cin >> fName >> lName >> phone;
            phonebook.add(Person(fName, lName, phone));
            break;
         case 2:
            phonebook.display();
            break;
         case 3:
            std::cout << "Enter first name and last name of the person to delete:" << std::endl;
            std::cin >> fName >> lName;
            phonebook.remove(fName, lName);
            std::cout << "Person deleted, if they were in the book." << std::endl;
            break;
         case 4:
            std::cout << "Enter first name and last name of the person to find:" << std::endl;
            std::cin >> fName >> lName;
            phone = phonebook.find(fName, lName);
            if(phone != "Not Found") {
                  std::cout << fName << " " << lName << "'s phone number: " << phone << std::endl;
            } else {
                  std::cout << "Person not found in the phonebook." << std::endl;
            }
            break;
         case 5:
            std::cout << "Enter first name and last name of the person:" << std::endl;
            std::cin >> fName >> lName;
            std::cout << "Enter the new phone number:" << std::endl;
            std::cin >> phone;
            phonebook.changePhoneNumber(fName, lName, phone);
            break;
         case 6:
            phonebook.saveToFile("phonebook.txt");
            std::cout << "Exiting the application" << std::endl;
            exit(0);
            break;
         default:
            std::cout << "Invalid choice. Please try again." << std::endl;
      }
   }

   void start(){
      // Load phonebook entries from file
      phonebook.loadFromFile("phonebook.txt"); 
      int choice;
      do {
         displayMenu();
         std::cin >> choice;
         executeChoice(choice);
      } while (choice != 6);
   }
};

int main() {
    UserInterface ui;
    ui.start();

    return 0;
};
