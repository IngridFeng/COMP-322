#include<iostream>
#include<algorithm>
#include<cstdlib>

struct ticket
{
  unsigned int numbers[6];
  ticket* next;
};

class SuperDraw
{
public:
  SuperDraw();
  SuperDraw(int num_of_tickets);
  SuperDraw(const SuperDraw &sd);
  ~SuperDraw();
  void newTicket(int verbose = 0);
  void printAllTicketNumbers();
  void verifySequence(unsigned int mySeq[]);
  void deleteSequence(unsigned int mySeq[]);
private:
  ticket* ticketListHead;
  ticket* ticketListTail;
};

//Question 1
SuperDraw::SuperDraw()//initial constructor
{
  this -> ticketListHead = NULL;
  this -> ticketListTail = NULL;
}
//Question 3
SuperDraw::SuperDraw(int num_of_tickets)//a constructor with num_of_tickets as the number of new tickets to be generated
{
  std::cout << num_of_tickets << " new ticket were successfully generated." << std::endl;
  std::cout << "The numbers are: ";
  this -> ticketListHead = NULL;
  this -> ticketListTail = NULL;
  for (int i = 0; i < num_of_tickets; ++i){//iteratively generate the new ticket
    this -> newTicket();//generate one new ticket
    ticket* curr = this -> ticketListTail;//this generated ticket is at the tail
    for (int i = 0; i < 5; ++i){
      std::cout << curr -> numbers[i] << ", ";//printout the numbers to the screen
    }
    std::cout << curr -> numbers[5];
    if (i == num_of_tickets - 1){//the last ticket to print
      std::cout << std::endl;
    }
    else{
      std::cout << " and ";
    }
  }
}
//Question 8
SuperDraw::SuperDraw(const SuperDraw &sd)
{
  //copy head
  ticket* curr_sd = sd.ticketListHead;
  ticket* curr = new ticket;
  for(int i = 0; i < 6; ++i){
    curr -> numbers[i] = curr_sd -> numbers[i];
  }
  curr_sd = curr_sd -> next;//move to the next ticket
  this -> ticketListHead = curr;//put the head pointer at the current ticket
  //iteratively copying tickets after head
  while(curr_sd != NULL){
      ticket* new_t = new ticket;
      for(int i = 0; i < 6; ++i){
          new_t -> numbers[i] = curr_sd -> numbers[i];
      }
      curr -> next = new_t;//put the next of the previous ticket points at new_t
      curr = curr -> next;
      if (curr_sd -> next == NULL){//in the case of this is the tail
        this -> ticketListTail = curr;//update pointer of the tail
        // std::cout << "tail " << curr -> numbers[3] << " " << this -> ticketListTail -> numbers[3] << std::endl;
      }
      curr_sd = curr_sd -> next;
  }
}


//Question 7
SuperDraw::~SuperDraw(){
  ticket* curr = this -> ticketListHead;
  ticket* curr_next;
  while (curr != NULL){//iteratively delete each ticket until the tail
    curr_next = curr -> next;
    delete [] curr;
    curr = curr_next;
  }
  // std::cout << "Destructor gets called" << std::endl;
}

//Question 2
void SuperDraw::newTicket(int verbose){
  ticket* new_t = new ticket;//allocate memory for new_t
  new_t -> next = NULL;//it will be at the tail so points to NULL

  int curr_num = std::rand() % 49 + 1;//generate the first number of the ticket
  new_t -> numbers[0] = curr_num;

  for (int i = 1; i < 6; ++i){//iteratively generate numbers of the ticket
    while (std::find(new_t -> numbers, new_t -> numbers+6, curr_num) != new_t -> numbers+6){
      //make sure this number is not the same as the previous numbers
      curr_num = std::rand() % 49 + 1;
    }
    new_t -> numbers[i] = curr_num;
  }

  std::sort(new_t -> numbers, new_t -> numbers+6);//sort the numbers

  if (this -> ticketListHead == NULL){//update pointers when this is the first ticket
    this -> ticketListHead = new_t;
    this -> ticketListTail = new_t;
  }
  else {//otherwise this ticket must be the new tail
    this -> ticketListTail -> next = new_t;
    this -> ticketListTail = this -> ticketListTail -> next;
  }

  if (verbose == 1){//then needs to print to the screen
    std::cout << "A new ticket was successfully generated. The numbers are: ";
    for (int i = 0; i < 5; ++i){//iteratively print the numbers of the new ticket
      std::cout << new_t -> numbers[i] << ", ";
    }
    std::cout << new_t -> numbers[5] << std::endl;
  }
}
//Question 4
void SuperDraw::printAllTicketNumbers(){
  int count = 0;//maintains a count of the number of tickets
  ticket* curr = this -> ticketListHead;
  while (curr != NULL){//calculate the count of the ticket
    count ++;
    curr = curr -> next;
  }
  std::cout << "We found " << count << " generated tickets:" << std::endl;
  curr = this -> ticketListHead;
  while (curr != NULL){//iteratively print the numbers of each ticket
    for (int i = 0; i < 5; ++i){
      std::cout << curr -> numbers[i] << ", ";
    }
    std::cout << curr -> numbers[5] << std::endl;
    curr = curr -> next;
  }
}
//Question 5
void SuperDraw::verifySequence(unsigned int mySeq[]){
  std::sort(mySeq, mySeq+6);//make sure the input sequence is sorted for comparison convenience
  ticket* curr = this -> ticketListHead;
  while (curr != NULL){//loop through each ticket
    for (int i = 0; i < 6; ++i){//compare each of the six numbers in the ticket
      if (curr -> numbers[i] != mySeq[i]){//this ticket is not the same
        break;
      }
      if (i == 5){//this ticket is the same as mySeq
        std::cout << "The provided sequence of numbers was already generated" << std::endl;
        return;
      }
    }
    curr = curr -> next;//go to the next ticket
  }
  //if didn't find a ticket that is the same as mySeq
  std::cout << "The provided sequence of numbers was never generated before" << std::endl;
}
//Question 6
void SuperDraw::deleteSequence(unsigned int mySeq[]){
  std::sort(mySeq, mySeq+6);//make sure the input sequence is sorted for comparison convenience
  ticket* curr = this -> ticketListHead;
  ticket* prev = NULL;
  bool found = false;
  bool printed = false;

  while (curr != NULL){//loop through each ticket
    found = false;
    for (int i = 0; i < 6; ++i){//compare each of the six numbers in the ticket
      if (curr -> numbers[i] != mySeq[i]){//this ticket is not the same
        break;
      }
      if (i == 5){//this ticket is the same as mySeq
        found = true;
        if (printed == false){//print the statement if this is the first ticket that is the same as mySeq
          std::cout << "The provided sequence of numbers was successfully deleted." << std::endl;
          printed = true;
        }
        if (prev == NULL){//this ticket is the head, update the pointers and free the memory
          this -> ticketListHead = curr -> next;
          delete curr;
          curr = this -> ticketListHead;
          // std::cout << "head " << curr -> numbers[3] << " " << prev << std::endl;
        }
        else if (curr -> next == NULL){//this ticket is the tail, update the pointers and free the memory
          prev -> next = NULL;
          this -> ticketListTail = prev;
          delete curr;
          curr = NULL;
          // std::cout << "tail " << curr << " " << prev -> numbers[3] << std::endl;
        }
        else{//this ticket is not the head nor the tail, update the pointers and free the memory
          prev -> next = curr -> next;
          delete curr;
          curr = prev -> next;
          // std::cout << "middle " << curr -> numbers[3] << " " << prev -> numbers[3] << std::endl;
        }
      }
    }
    if (found == false){//if this ticket is not the same
      prev = curr;
      curr = curr -> next;//go to the next ticket
    }
  }
  if (printed == false){//if didn't find a ticket that is the same as mySeq
    std::cout << "The provided sequence of numbers was never generated before" << std::endl;
  }
}

int main(){
  SuperDraw sd;
  srand(time(NULL));//in order to create random tickets

  //Question 2 testing
  std::cout << "----Question 2 Testing Start----" << std::endl;
  sd.newTicket(1);
  std::cout << "----Question 2 Testing End------" << std::endl << std::endl;

  //Question 3 testing
  std::cout << "----Question 3 Testing Start----" << std::endl;
  SuperDraw sd1(5);
  std::cout << "----Question 3 Testing End------" << std::endl << std::endl;

  //Question 4 testing
  std::cout << "----Question 4 Testing Start----" << std::endl;
  sd.newTicket();
  sd.newTicket();
  sd.newTicket();
  //as many sd.newTicket() as you like
  sd.printAllTicketNumbers();
  std::cout << "----Question 4 Testing End------" << std::endl << std::endl;

  //Question 5 testing
  std::cout << "----Question 5 Testing Start----" << std::endl;
  unsigned int myNumbers[6] = {2, 4, 17, 29, 31, 34};
  sd.verifySequence(myNumbers);
  std::cout << "----Question 5 Testing End------" << std::endl << std::endl;

  //Question 6 testing
  std::cout << "----Question 6 Testing Start----" << std::endl;
  sd.deleteSequence(myNumbers);
  std::cout << "----Question 6 Testing End------" << std::endl << std::endl;

  //Question 8 testing
  std::cout << "----Question 8 Testing Start----" << std::endl;
  SuperDraw sd2(sd);
  sd2.printAllTicketNumbers();
  std::cout << "----Question 8 Testing End------" << std::endl;
  return 0;
}
