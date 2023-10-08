#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

/* This person class will be the data that is held at each node in the Tree.
   Each person has a last name, first name, and phone number, all represented as strings
*/
class Person {
   public:
   std::string firstName{};
   std::string lastName{};
   std::string phoneNumber{}; //The phone number should not have any special characters in it, just raw numbers

   
   
   //Constructor that accepts references to the names and number of a new person upon creation
   Person(const std::string& firstname, const std::string& lastname, const std::string& phonenumber)
    : firstName(firstname), lastName(lastname), phoneNumber(phonenumber) {}

   /*
      This will overload the inequality operators in order to better compare 2 instances of Person
      It makes it so that we can compare Person X and Person Y just by using the instance rather than
      calling their first and lastnames at every comparison
   */
   friend bool operator > (const Person& x, const Person& y) {
      if (x.lastName == y.lastName){
         return x.firstName > y.firstName;
      } else {
         return x.lastName > y.lastName;
      }
   }

   friend bool operator < (const Person& x, const Person& y) {
      if (x.lastName == y.lastName){
         return x.firstName < y.firstName;
      } else {
         return x.lastName < y.lastName;
      }
   }


};

/* The Treenode class is the class to be used for the Binary tree to be constructed as
   part of the Book class. The Tree will be nodes that are people within the phonebook
*/
class Treenode {
   public:
   Person person;
   Treenode* leftChild;
   Treenode* rightChild;

   
   Treenode(const Person& newPerson)
   : person(newPerson), leftChild(nullptr), rightChild(nullptr) {}
};

class Book {
   private:
      Treenode* root{nullptr};
   public:
   
   // Add: Adds a person’s name (first and last) and phone number to the phone book.
   /*
      Checks to see if the given node is null (always start at root)
      if it is then theres the open spot to add the new node
      If it isn't then we compare if person is higher or lower alphabetically using our overloaded > <
      and recursively call the function again with the appropriate child
   */
  void add (const Person& person){
      addPerson(root, person);
   }

   void addPerson(Treenode*& node, const Person& person){
         if (node == nullptr) {
            node = new Treenode(person);
         } else if (person < node->person) {
            addPerson(node->leftChild, person);
         } else if (person > node->person) {
            addPerson(node->rightChild, person);
         }
   }


   // Remove: Deletes a given person’s phone number, given only the name. **Formerly delete, but delete is a keyword so we can't call then function that.
   void remove(){}


   // Find: Locates a person’s phone number, given only the person’s name.
   std::string find(const std::string& firstname, const std::string& lastname){
      Treenode* currentNode = root;
      while (currentNode != nullptr) {
         //If the first and last names match, we found our person, and return their number
         if (firstname == currentNode->person.firstName && lastname == currentNode->person.lastName){
            return(currentNode->person.phoneNumber);
         //If the only the last name matches, then we compare first names to go to the left or right child
         } else if (lastname == currentNode->person.lastName){
            if(firstname < currentNode->person.firstName){
               currentNode = currentNode->leftChild;
            } else {
               currentNode = currentNode->rightChild;
            }
         //If nothing matches, we compare last names and go to the left or right child
         } else if (lastname < currentNode->person.lastName){
            currentNode = currentNode->leftChild;
         } else {
            currentNode = currentNode->rightChild;
         }
      }
   }
   

   // Change: Changes a person’s phone number given the person’s name and new phone number.
      void change(){}


   // Display: Displays (dumps) entire phone book in alphabetical order.
   void display(){
      inorderTraversal(root);
   }

   void inorderTraversal(Treenode* node){
      if (node != nullptr){
         inorderTraversal(node->leftChild);
         std::cout << "INSERT MESSAGE HERE FOR DISPLAY";
         inorderTraversal(node->rightChild);
      }
   }



   void quit(){}


   

};