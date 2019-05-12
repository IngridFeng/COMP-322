#include<iostream>
#include<exception>
#include<typeinfo>
using namespace std;
/*
Question 1:
Smart Pointers are designed such that "resource acquisition occurs at the same
time that the object is initialized." When a pointer goes out of scope, the memory
it points to is deallocated automatically and there is no need to free the memory
again.

The smart pointers provided by the standard library are: unique_ptr, shared_ptr,
and weak_ptr.

Differences between them are:
Unique_ptr allows exactly one owner (can be moved to a new owner) while shared_ptr
allows multiple ownders. A unique_ptr has the size of one pointer while a shared_ptr
has the size of two pointers -- one for the object and the other for the reference
count. The raw pointer is deleted for unique_ptr automatically when its only owner
goes out of scope while the raw pointer for shared_ptr is only deleted when all of
its owners go out of scope.

Weak_ptr is used together with shared_ptr; it "provides access to an object owned
by one or more shared_ptr instances, but does not participate in reference counting."
Weak_ptr models temporary ownership -- it is used to track the object and in order to
access the object it is converted to a shared_ptr temporarily. "If the original
shared_ptr is destroyed at this time, the object's lifetime is extended until the
temporary shared_ptr is destroyed as well." In addition, "weak_ptr is used to break
reference cycles formed by objects managed by share_ptr" in order to prevent memory
leak.

Reference:
1). https://docs.microsoft.com/en-us/cpp/cpp/smart-pointers-modern-cpp?view=vs-2017
2). https://en.cppreference.com/w/cpp/memory/weak_ptr
*/

template <typename T>
class SmartPointer
{
private:
  T *ptr;//pointer for single variable
  int length;//length of the array, default to 1 --> single variable
public:
  //default constructor, single variable, setting ptr value to 0
  SmartPointer(){
    T num = 0;
    try{
      this -> ptr = new T(num);//initialize the pointer
      this -> length = 1;
    }
    catch (bad_alloc& e){//running out of memory
      cout << "Allocation failed for smart pointer: " << e.what() << endl;
    }
  }
  //constructor for single variable
  SmartPointer(T num){
    try{
      string msg = "Initialization failed! SmartPointer class does not handle negative numbers.";
      if (num < 0) {throw msg;}//if is passing in negative number, error
      else {
        this -> ptr = new T(num);
        this -> length = 1;
      }//initialize the pointer
    }
    catch (bad_alloc& e){//running out of memory
      cout << "Allocation failed for smart pointer: " << e.what() << endl;
    }
    catch (string& message){
      cout << message << endl;//print the error message
    }
  }
  //constructor for array pointer
  SmartPointer(T num[], int lg){
    try{
      this -> length = lg;
      this -> ptr = new T[this -> length];//allocate array space
    }
    catch (bad_alloc& e){//running out of memory
      cout << "Allocation failed for smart pointer: " << e.what() << endl;
    }
    try{
      for (int i = 0; i < this -> length; i++){
        string msg = "Initialization failed! SmartPointer class does not handle negative numbers.";
        if (num[i] < 0) {//if this entry of array is negative
          throw msg;
        }
        else {*(this -> ptr + i) = num[i];}//o.w. set the value
      }
    }
    catch (string& message){
      cout << message << endl;//print the error message
    }
  }

  //destructor
  ~SmartPointer(){
    try{
      string msg = "The smart pointer is not initialized!";
      if (this -> ptr == NULL) {throw msg;}
      else {//only free the memory if it was allocated
        if (length == 1){//single variable case
          delete this -> ptr;
        }
        else{//array case
          delete[] this -> ptr;
        }
      }
    }
    catch (string& message){}
  }
  //get the value of the single variable pointer
  T getValue(){
    try{
      string msg = "\nGetValue failed! The smart pointer is not initialized.";
      if (this -> ptr == NULL) {throw msg;}
      else {return *(this -> ptr);}//only get value if the pointer was initialized
    }
    catch (string& message){
      cout << message << endl;
      return 0;//return default value
    }
  }

  //get value in the smart array with index id
  T getValueAt(int id){//id is the index [0, index-1]
    try{
      string msg1 = "\nGetValue failed! The smart pointer is not initialized.";
      string msg2 = "\nError! Array index out of bound.";
      if (this -> ptr == NULL) {throw msg1;}
      else if (id >= this -> length) {throw msg2;}
      else {return *(this -> ptr + id);}//only get value if the pointer was initialized and the index is valid
    }
    catch (string& message){
      cout << message << endl;
      return 0;//return default value
    }
  }

  //setting the value of the single variable pointer
  //if setValue takes in a negative value, then does not change the original value of the smartpointer
  void setValue(T newNum){
    try{
      string msg1 = "SetValue failed! SmartPointer class does not handle negative numbers.";
      string msg2 = "SetValue failed! The smart pointer is not initialized.";
      if (newNum < 0) {throw msg1;}//if requesting to set a negative value
      else if (this -> ptr == NULL) {throw msg2;}//if the pointer was not initialized
      else {*(this -> ptr) = newNum;}//set value if no error
    }
    catch (string& message){
      cout << message << endl;
    }
  }

  //set value in the smart array with index id
  void setValue(T newNum, int id){//id is the index [0, index-1]
    try{
      string msg1 = "SetValue failed! SmartPointer class does not handle negative numbers.";
      string msg2 = "SetValue failed! The smart pointer is not initialized.";
      string msg3 = "\nError! Array index out of bound.";
      if (newNum < 0) {throw msg1;}//if requesting to set a negative value
      else if (this -> ptr == NULL) {throw msg2;}//if the pointer was not initialized
      else if (id >= this -> length) {throw msg3;}
      else {*(this -> ptr + id) = newNum;}//set value if no error
    }
    catch (string& message){
      cout << message << endl;
    }
  }

  //overloading dereferencing and arrow operators
  T & operator * (){
    assert(this -> ptr != NULL);
    return *(this -> ptr);
  }
  T * operator -> (){
    assert(this -> ptr != NULL);
    return this -> ptr;
  }

  //friend functions for arithmetic operations

  //for addition
  friend SmartPointer operator+(SmartPointer<T> &num1, SmartPointer<T> &num2){
    try{
      string msg = "Cannot perform arithmetic operations on smart pointers of different types.";
      string msg0 = "Addition failed! Cannot perform arithmetic operations on smart pointer arrays of different lengths.";
      string msg1 = "Addition failed! SmartPointer class does not handle negative numbers.";
      string msg2 = "Addition failed! The smart pointer is not initialized.";
      if (typeid(num1).name() != typeid(num2).name()) {throw msg;}//different type
      else if (num1.length != num2.length) {throw msg0;}//different length
      else if (*(num1.ptr) < 0 || *(num2.ptr) < 0) {throw msg1;}//negative number
      else if ((num1.ptr) == NULL || (num2.ptr) == NULL) {throw msg2;}//null pointer
      else {
        SmartPointer<T> *num3 = new SmartPointer<T>(new T[num1.length], num1.length);//allocate space
        for (int i = 0; i < num1.length; i++){
          num3 -> setValue(*(num1.ptr + i) + *(num2.ptr + i), i);//set value to addition
        }
        return *num3;
      }
    }
    catch (string& message){
      cout << message << endl;
      return 0;
    }
  }

  friend SmartPointer operator-(SmartPointer<T> &num1, SmartPointer<T> &num2){
    try{
      string msg = "Cannot perform arithmetic operations on smart pointers of different types.";
      string msg0 = "Subtraction failed! Cannot perform arithmetic operations on smart pointer arrays of different lengths.";
      string msg1 = "Subtraction failed! SmartPointer class does not handle negative numbers.";
      string msg2 = "Subtraction failed! The smart pointer is not initialized.";
      if (typeid(num1).name() != typeid(num2).name()) {throw msg;}//different type
      else if (num1.length != num2.length) {throw msg0;}//different length
      else if (*(num1.ptr) < 0 || *(num2.ptr) < 0 || *(num1.ptr) < *(num2.ptr)) {throw msg1;}//negative value
      else if ((num1.ptr) == NULL || (num2.ptr) == NULL) {throw msg2;}//null pointer
      else {
        SmartPointer<T> *num3 = new SmartPointer<T>(new T[num1.length], num1.length);//allocate space
        for (int i = 0; i < num1.length; i++){
          num3 -> setValue(*(num1.ptr + i) - *(num2.ptr + i), i);//set value to subtraction
        }
        return *num3;
      }
    }
    catch (string& message){
      cout << message << endl;
      return 0;
    }
  }

  friend SmartPointer operator*(SmartPointer<T> &num1, SmartPointer<T> &num2){
    try{
      string msg = "Cannot perform arithmetic operations on smart pointers of different types.";
      string msg0 = "Multiplication failed! Cannot perform arithmetic operations on smart pointer arrays of different lengths.";
      string msg1 = "Multiplication failed! SmartPointer class does not handle negative numbers.";
      string msg2 = "Multiplication failed! The smart pointer is not initialized.";
      if (typeid(num1).name() != typeid(num2).name()) {throw msg;}//different type
      else if (num1.length != num2.length) {throw msg0;}//different length
      else if (*(num1.ptr) < 0 || *(num2.ptr) < 0) {throw msg1;}//negative value
      else if ((num1.ptr) == NULL || (num2.ptr) == NULL) {throw msg2;}//null pointer
      else {
        SmartPointer<T> *num3 = new SmartPointer<T>(new T[num1.length], num1.length);//allocate space
        for (int i = 0; i < num1.length; i++){
          num3 -> setValue(*(num1.ptr + i) * *(num2.ptr + i), i);//set value to multiplication
        }
        return *num3;
      }
    }
    catch (string& message){
      cout << message << endl;
      return 0;
    }
  }
};



int main() {
// For SmartPointer class
cout << "----------------Testing SmartPointer class----------------" << endl;

// Testing Constructors
cout << "---------Creating SmartIntPointer1 with value 11----------" << endl;
SmartPointer<int> SmartIntPointer1(11);
cout << "SmartIntPointer1 = " << SmartIntPointer1.getValue() << endl;

cout << "---------Creating SmartIntPointer with value -1-----------" << endl;
SmartPointer<int> SmartIntPointer(-1);

cout << "-----Creating SmartIntPointer2 with no value provided-----" << endl;
SmartPointer<int> SmartIntPointer2;
cout << "SmartIntPointer2 = " << SmartIntPointer2.getValue() << endl;
cout << "Type of SmartIntPointer2 is " << typeid(SmartIntPointer2.getValue()).name() << endl;

// Testing Setter & Getter
cout << "----------Setting value of SmartIntPointer2 to 5----------" << endl;
SmartIntPointer2.setValue(5);
cout << "SmartIntPointer2 = " << SmartIntPointer2.getValue() << endl;

//float
cout << "-------------------Testing float--------------------------" << endl;
cout << "----Creating SmartFloatPointer1 with no value provided----" << endl;
SmartPointer<float> SmartFloatPointer1;
cout << "SmartFloatPointer1 = " << SmartFloatPointer1.getValue() << endl;
cout << "Type of SmartFloatPointer1 is " << typeid(SmartFloatPointer1.getValue()).name() << endl;

cout << "--------Setting value of SmartFloatPointer1 to 1.5--------" << endl;
SmartFloatPointer1.setValue(1.5);
cout << "SmartFloatPointer1 = " << SmartFloatPointer1.getValue() << endl;

cout << "----Creating SmartFloatPointer2 with no value provided----" << endl;
SmartPointer<float> SmartFloatPointer2;
cout << "--------Setting value of SmartFloatPointer2 to 2.5--------" << endl;
SmartFloatPointer2.setValue(2.5);
cout << "SmartFloatPointer2 = " << SmartFloatPointer2.getValue() << endl;

//double
cout << "-------------------Testing double-------------------------" << endl;
cout << "----Creating SmartDoublePointer1 with no value provided---" << endl;
SmartPointer<double> SmartDoublePointer1;
cout << "SmartDoublePointer1 = " << SmartDoublePointer1.getValue() << endl;
cout << "Type of SmartDoublePointer1 is " << typeid(SmartDoublePointer1.getValue()).name() << endl;

cout << "--------Setting value of SmartDoublePointer1 to 13.31-----" << endl;
SmartDoublePointer1.setValue(13.31);
cout << "SmartDoublePointer1 = " << SmartDoublePointer1.getValue() << endl;
//arithmetic operations
cout << "-----------------Arithmetic Operations--------------------" << endl;
cout << "------Adding SmartFloatPointer1 to SmartFloatPointer2-----" << endl;
SmartPointer<float> SmartFloatPointer3 = SmartFloatPointer1 + SmartFloatPointer2;
cout << "SmartFloatPointer1 + SmartFloatPointer2 = " << SmartFloatPointer3.getValue() << endl;

cout << "--Subtracting SmartFloatPointer1 from SmartFloatPointer2--" << endl;
SmartPointer<float> SmartFloatPointer4 = SmartFloatPointer2 - SmartFloatPointer1;
cout << "SmartFloatPointer2 - SmartFloatPointer1 = " << SmartFloatPointer4.getValue() << endl;

cout << "--Subtracting SmartFloatPointer2 from SmartFloatPointer1--" << endl;
cout << "SmartFloatPointer1 - SmartFloatPointer2 = ";
SmartPointer<float> SmartFloatPointer5 = SmartFloatPointer1 - SmartFloatPointer2;

cout << "---Multiplying SmartFloatPointer1 to SmartFloatPointer2---" << endl;
SmartPointer<float> SmartFloatPointer6 = SmartFloatPointer1 * SmartFloatPointer2;
cout << "SmartFloatPointer1 * SmartFloatPointer2 = " << SmartFloatPointer6.getValue() << endl;

// For handling arrays
cout << "-----------------------------Testing arrays-----------------------------" << endl;

//
// add the needed code that shows how you use your class to create an array of multiple elements of a certain type.
// provide all the necessary test code that shows the different use cases of your code

// Testing Constructors
int array1[6] = {3, 2, 1, 6, 8, 10};
cout << "---------------Creating SmartIntArrayPointer1 of length 6---------------" << endl;
SmartPointer<int> SmartIntArrayPointer1(array1, 6);
cout << "SmartIntArrayPointer1[0] = " << SmartIntArrayPointer1.getValueAt(0) << endl;
cout << "SmartIntArrayPointer1[1] = " << SmartIntArrayPointer1.getValueAt(1) << endl;
cout << "SmartIntArrayPointer1[2] = " << SmartIntArrayPointer1.getValueAt(2) << endl;
cout << "SmartIntArrayPointer1[3] = " << SmartIntArrayPointer1.getValueAt(3) << endl;
cout << "SmartIntArrayPointer1[4] = " << SmartIntArrayPointer1.getValueAt(4) << endl;
cout << "SmartIntArrayPointer1[5] = " << SmartIntArrayPointer1.getValueAt(5) << endl;
cout << "SmartIntArrayPointer1[6] = " << SmartIntArrayPointer1.getValueAt(6) << endl;

int array2[5] = {3, 6, 9, -1, -2};
cout << "-----Creating SmartIntArrayPointer2 of length 5 w/ negative numbers-----" << endl;
SmartPointer<int> SmartIntArrayPointer2(array2, 6);

// Testing Setter & Getter
cout << "----------------------Testing getting and setting-----------------------" << endl;
cout << "----------Setting 3rd element of SmartIntArrayPointer1 to 5-------------" << endl;
SmartIntArrayPointer1.setValue(5, 2);
cout << "SmartIntArrayPointer1[2] = " << SmartIntArrayPointer1.getValueAt(2) << endl;
cout << "Type of SmartIntArrayPointer1[2] is " << typeid(SmartIntArrayPointer1.getValueAt(2)).name() << endl;

//float
cout << "----------------------Testing with float--------------------------------" << endl;
float array3[5] = {3.3, 6.5, 9.8, 10.1, 13.5};
cout << "--------------Creating SmartFloatArrayPointer1 of length 5--------------" << endl;
SmartPointer<float> SmartFloatArrayPointer1(array3, 5);
cout << "SmartFloatArrayPointer1[4] = " << SmartFloatArrayPointer1.getValueAt(4) << endl;
cout << "Type of SmartFloatArrayPointer1[4] is " << typeid(SmartFloatArrayPointer1.getValueAt(4)).name() << endl;

cout << "----------Setting 5th element of SmartFloatArrayPointer1 to 11.3--------" << endl;
SmartFloatArrayPointer1 .setValue(11.3, 4);
cout << "SmartFloatArrayPointer1[4] = " << SmartFloatArrayPointer1.getValueAt(4) << endl;

float array5[5] = {8.8, 34.5, 23.7, 66.7, 130.1};
cout << "--------------Creating SmartFloatArrayPointer2 of length 5--------------" << endl;
SmartPointer<float> SmartFloatArrayPointer2(array5, 5);

//double
cout << "----------------------Testing with double-------------------------------" << endl;
double array4[3] = {15.51, 34.23, 122.34};
cout << "--------------Creating SmartDoubleArrayPointer1 of length 3-------------" << endl;
SmartPointer<double> SmartDoubleArrayPointer1(array4, 3);
cout << "SmartDoubleArrayPointer1[2] = " << SmartDoubleArrayPointer1.getValueAt(2) << endl;
cout << "Type of SmartDoubleArrayPointer1[2] is " << typeid(SmartDoubleArrayPointer1.getValueAt(2)).name() << endl;

cout << "----------Setting 2nd element of SmartDoubleArrayPointer1 to 66.66-------" << endl;
SmartDoubleArrayPointer1.setValue(66.66, 1);
cout << "SmartDoubleArrayPointer1[1] = " << SmartDoubleArrayPointer1.getValueAt(1) << endl;

//arithmetic operations
cout << "----------------------Arithmetic Operations-----------------------------" << endl;

cout << "SmartFloatArrayPointer1 = {3.3, 6.5, 9.8, 10.1, 11.3}" << endl;
cout << "SmartFloatArrayPointer2 = {8.8, 34.5, 23.7, 66.7, 130.1}" << endl;

cout << "---------Adding SmartFloatArrayPointer1 to SmartFloatArrayPointer2------" << endl;
SmartPointer<float> SmartFloatArrayPointer3 = SmartFloatArrayPointer1 + SmartFloatArrayPointer2;
cout << "SmartFloatArrayPointer1[0] + SmartFloatArrayPointer2[0] = " << SmartFloatArrayPointer3.getValueAt(0) << endl;
cout << "SmartFloatArrayPointer1[1] + SmartFloatArrayPointer2[1] = " << SmartFloatArrayPointer3.getValueAt(1) << endl;
cout << "SmartFloatArrayPointer1[2] + SmartFloatArrayPointer2[2] = " << SmartFloatArrayPointer3.getValueAt(2) << endl;
cout << "SmartFloatArrayPointer1[3] + SmartFloatArrayPointer2[3] = " << SmartFloatArrayPointer3.getValueAt(3) << endl;
cout << "SmartFloatArrayPointer1[4] + SmartFloatArrayPointer2[4] = " << SmartFloatArrayPointer3.getValueAt(4) << endl;
cout << "SmartFloatArrayPointer1[5] + SmartFloatArrayPointer2[5] = " << SmartFloatArrayPointer3.getValueAt(5) << endl;

cout << "---Subtracting SmartFloatArrayPointer2 from SmartFloatArrayPointer1-----" << endl;
cout << "SmartFloatArrayPointer1[3] - SmartFloatArrayPointer2[3] = ";
SmartPointer<float> SmartFloatArrayPointer4 = SmartFloatArrayPointer1 - SmartFloatArrayPointer2;

cout << "----Subtracting SmartFloatArrayPointer1 from SmartFloatArrayPointer2----" << endl;
SmartPointer<float> SmartFloatArrayPointer5 = SmartFloatArrayPointer2 - SmartFloatArrayPointer1;
cout << "SmartFloatArrayPointer2[3] - SmartFloatArrayPointer1[3] = " << SmartFloatArrayPointer5.getValueAt(3) << endl;

cout << "-----Multiplying SmartFloatArrayPointer1 to SmartFloatArrayPointer2-----" << endl;
SmartPointer<float> SmartFloatArrayPointer6 = SmartFloatArrayPointer1 * SmartFloatArrayPointer2;
cout << "SmartFloatArrayPointer1[3] * SmartFloatArrayPointer2[3] = " << SmartFloatArrayPointer6.getValueAt(3) << endl;

cout << "----------Arithmetic Operations with arrays of different lengths--------" << endl;

float array7[2] = {45.5, 2.7};
SmartPointer<float> SmartFloatArrayPointer7(array7, 2);

cout << "SmartFloatArrayPointer1 = {3.3, 6.5, 9.8, 10.1, 11.3}" << endl;
cout << "SmartFloatArrayPointer7 = {45.5, 2.7}" << endl;

cout << "SmartFloatArrayPointer1[0] + SmartFloatArrayPointer7[0] = ";
SmartPointer<float> SmartFloatArrayPointer8 = SmartFloatArrayPointer1 + SmartFloatArrayPointer7;

cout << "SmartFloatArrayPointer1[0] - SmartFloatArrayPointer7[0] = ";
SmartPointer<float> SmartFloatArrayPointer9 = SmartFloatArrayPointer1 - SmartFloatArrayPointer7;

cout << "SmartFloatArrayPointer1[0] * SmartFloatArrayPointer7[0] = ";
SmartPointer<float> SmartFloatArrayPointer10 = SmartFloatArrayPointer1 * SmartFloatArrayPointer7;
cout << "----------------------All test cases ended------------------------------" << endl;
return 0;
}
