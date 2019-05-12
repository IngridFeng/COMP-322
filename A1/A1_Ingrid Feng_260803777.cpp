#include<iostream>
using namespace std;
/*Question 3
A function is tail recursive if every recursive call is a tail call. This means
that no additional computation is done after every recursive call. It is better to
design recursive function as a tail recursive one because the computer can forget
about the caller when a call is a tail call. It only need to remember the
newly-called function. This saves time and space. Tail call elimination can usually
reduce the space complexity from O(n) to O(1). Therefore, some programming languages
like Scheme requires its functions to be properly tail recursive.

Not any recursive functions can be designed to be tail recursive. But in continuation
passing style, all function calls can be tail calls, although might not be recursive.
This means we are going to make the recursion stack explicit. We are going to split
out procedure into multiple procedures. One procedure ends at the next procedure's
starting point. By explicitly specifying the starting and ending points, every call
would be a tail call.

Reference:
1).https://en.wikipedia.org/wiki/Tail_call
2).https://cs.stackexchange.com/questions/41768/can-any-recursion-implementation-be-written-as-tail-recursion
*/

//function decalaration
void countLetter();
void convertPhonetic();
void factorial();
int helper(int num, int total);
int enhancedHelper(int num, int total);
void enhancedFactorial();


//main function
int main(){
  countLetter();
  convertPhonetic();
  factorial();
  enhancedFactorial();
  return 0;
}

void countLetter(){
  string sent;
  char lett;
  int count;
  //take in inputs
  cout << "Please enter a sentence: ";
  getline(cin, sent);
  cout << "Please enter a letter: ";
  cin >> lett;
  //recursively count letter
  for (int i = 0; i < sent.length(); ++i){
    if (tolower(lett) == tolower(sent[i]))
      count++;
  }
  //print to the terminal
  cout << "The letter " << lett << " is repeated " << count << " times in your sentence" << endl;
}

void convertPhonetic(){
  string word;
  string phonetic[26] = {"Alfa", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot",
  "Golf", "Hotel", "India", "Juliett", "Kilo", "Lima", "Mike", "November", "Oscar",
  "Papa", "Quebec", "Romeo", "Sierra", "Tango", "Uniform", "Victor", "Whiskey", "X-ray", "Yankee", "Zulu"};
  cout << "Please enter a word: ";
  cin >> word;
  //check for error if inputs are all letters
  for (int i = 0; i < word.length(); ++i){
    char l = tolower(word[i]);
    if (l < 'a' || l > 'z'){
      cout << "Error! Please enter a word with only letters." << endl;
    }
  }
  //recursively print the corresponding word for each letter
  for (int i = 0; i < word.length(); ++i){
    int id = int(tolower(word[i]))-97;
    cout << phonetic[id] << ' ';
  }
  cout << endl;
}

int helper(int num, int total){
  if (num == 0)
    return total;
  else
    return helper(num-1, total*num);
}

void factorial(){
  int n;
  int res;
  //input
  cout << "Please enter a number: ";
  cin >> n;
  res = helper(n, 1);
  //output
  cout << "The factorial of " << n << " is " << res << endl;
}

int enhancedHelper(int num, int total){
  if (num == 6)
    return 720*total;
  else
    return enhancedHelper(num-1, total*num);
}

void enhancedFactorial(){
  int arr[6] = {1, 2, 6, 24, 120, 720};
  int n;
  int res;
  //input
  cout << "Please enter a number: ";
  cin >> n;
  if (n <= 6)
    res = arr[n-1];
  else
    res = enhancedHelper(n, 1);
  //output
  cout << "The factorial of " << n << " is " << res << endl;
}
