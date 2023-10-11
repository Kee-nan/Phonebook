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

   Person(const std::string& firstname, const std::string& lastname)
   : firstName(firstname), lastName(lastname) {}

   std::string getFirstName() const { return firstName; }
   std::string getLastName() const { return lastName; }
   std::string getPhoneNumber() const { return phoneNumber; }

   void setPhoneNumber(const std::string& phone) { phoneNumber = phone; }

   bool operator<(const Person& other) const {
        if (lastName < other.lastName) return true;
        if (lastName == other.lastName && firstName < other.firstName) return true;
        return false;
    }
   /*
      This will overload the inequality operators in order to better compare 2 instances of Person
      It makes it so that we can compare Person X and Person Y just by using the instance rather than
      calling their first and lastnames at every comparison
   */
   // friend bool operator > (const Person& x, const Person& y) {
   //    if (x.lastName == y.lastName){
   //       return x.firstName > y.firstName;
   //    } else {
   //       return x.lastName > y.lastName;
   //    }
   // }

   // friend bool operator < (const Person& x, const Person& y) {
   //    if (x.lastName == y.lastName){
   //       return x.firstName < y.firstName;
   //    } else {
   //       return x.lastName < y.lastName;
   //    }
   // }

};

/* The Treenode class is the class to be used for the Binary tree to be constructed as
   part of the Book class. The Tree will be nodes that are people within the phonebook
*/
// class Treenode {
//    public:
//    Person person;
//    Treenode* leftChild;
//    Treenode* rightChild;

   
//    Treenode(const Person& newPerson)
//    : person(newPerson), leftChild(nullptr), rightChild(nullptr) {}
// };

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

//    Treenode* root{nullptr};

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

   public:
      Book() : root(nullptr) {}

   // Add: Adds a person’s name (first and last) and phone number to the phone book.
   /*
      Checks to see if the given node is null (always start at root)
      if it is then theres the open spot to add the new node
      If it isn't then we compare if person is higher or lower alphabetically using our overloaded > <
      and recursively call the function again with the appropriate child
   */
   void add(const Person& p) { insert(root, p); }

   // void addPerson(Treenode*& node, const Person& person){
   //       if (node == nullptr) {
   //          node = new Treenode(person);
   //       } else if (person < node->person) {
   //          addPerson(node->leftChild, person);
   //       } else {
   //          addPerson(node->rightChild, person);
   //       }
   // }


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
      void change(){}


   // Display: Displays (dumps) entire phone book in alphabetical order.
   void display(){
      inorderTraversal(root);
   }

   void inorderTraversal(Treenode* node){
      if (!root) return;
        inorder(root->leftChild);
        std::cout << root->data.getFirstName() << " " << root->data.getLastName() << ": " << root->data.getPhoneNumber() << std::endl;
        inorder(root->rightChild);
      // if (node != nullptr){
      //    inorderTraversal(node->leftChild);
      //    std::cout << root->person.getFirstName() << " " << root->person.getLastName() << ": " << root->person.getPhoneNumber() << std::endl;
      //    inorderTraversal(node->rightChild);
      // }
   }

   void saveToFile(const std::string& filename) {
         // Save BST to a text file using any tree traversal method.
      }

   void loadFromFile(const std::string& filename) {
      // Load from file and build BST.
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
    std::cout << "5. Quit" << std::endl;
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
            phonebook.saveToFile("phonebook.txt");
            break;
         default:
            std::cout << "Invalid choice. Please try again." << std::endl;
      }
   }

   void start(){
      int choice;
      do {
         displayMenu();
         std::cin >> choice;
         executeChoice(choice);
      } while (choice != 5);
   }
};

int main() {
    UserInterface ui;
    ui.start();

    return 0;
};
