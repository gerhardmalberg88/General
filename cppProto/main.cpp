#include <iostream>
#include <bitset>
#include <cstring>
#include "mylibrary.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

using namespace std;

// function declaration
int functionToSumTwoIntegers(int num1, int num2);

// function with multiple return using pointers definition
void functionToSumTwoIntegers(int num1, int num2, int *returnSum, int *returnDiff);

// class decalaration and definition
class MyClass{       		// The class
	public:             	// Access specifier
    	int myNum;        	// Attribute (int variable)
    	string myString;  	// Attribute (string variable)
};

// Declare a structure named "car"
struct car {
  string brand;
  string model;
  int year;
};

int main(int argc, char *argv[]) {
	
	cout << "Basics**************************" << endl;
	cout << "\033[31mFor colored text check defined colors above in main.cpp \033[0m\n";
	cout << "Hello World!" << endl;
	cout << "Total of "<< (argc-1) << " arguments entered" << endl;
	if(argc>=2){
		cout << "First argument is "<< argv[1] << endl;
	}
	else{
		cout << "First argument does not exist "<< endl;
	}
	
	cout << "\nData types**************************" << endl;
	int integerExample=456;
	float floatExample=1.23;
	cout << "Integer type " << integerExample << endl;
	cout << "Float type " << floatExample << endl;
	
	cout << "\nPointers**************************" << endl;
	int integer=8;		// declaration of a variable integer 
 	int *pointer; 		// declaration of a pointer
	pointer=&integer;	// setting pointer to point on the integer address
	cout << "integer value "<< integer << endl;
	cout << "pointer value "<< pointer << endl;
	cout << "integer address "<< &integer << endl;
	cout << "pointer address "<< &pointer << endl;
	cout << "value which is stored in pointer "<< *pointer << endl;
	
	cout << "\nDecimal, Hexadecimal and Binary**************************" << endl;
	integer=1;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=2;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=3;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=4;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << dec  << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=4294967295;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<32>(integer) << endl;
	integer=integer+1;
	cout << "Integer in Decimal 	" << dec << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<32>(integer) << endl;
	cout << dec;
	
	cout << "\nSwitch - Case**************************" << endl;
	integer=1;
	switch(integer){
	  	case 1:
	    		cout << "Integer is 1" << endl;
	    		break;
	  	case 2:
	    		cout << "Integer is 2" << endl;
	    		break;
	  	case 3:
	    		cout << "Integer is 3" << endl;
	    		break;
		default:
	    		cout << "Integer is other than 1,2 or 3" << endl;
	    		break;
	}	

	cout << "\nFor**************************" << endl;
	integer=0;
	// do for loop as long as integer is inside of defined range
	for(integer=0;integer<3;integer++){
		cout << "Integer value " << integer << endl;
	}
	
	cout << "\nWhile**************************" << endl;
	integer=0;
	// do while loop as long as integer is inside of defined range 
	while(integer<3){
		cout << "Integer value " << integer << endl;
		integer=integer+1;
		//break;
	}
	
	cout << "\nCharacter array**************************" << endl;
	char oneCharacter='M';	
	cout << "Character value " << oneCharacter << endl;
	char characterArray[]={'T','h','i','s','I','s','C','h','a','r','\0'};	
	cout << "Character array element 1 " << characterArray[0] << endl;
	cout << "Character array element 2 " << characterArray[1] << endl;
	cout << "Character array element 3 " << characterArray[2] << endl;
	cout << "Character array element 4 " << characterArray[3] << endl;
	cout << "Character array element 5 " << *(characterArray+4) << endl;
	cout << "Character array " << characterArray << endl;
	char characterArray2[]="AlsoCharacterArray";	
	string characterArray3="AndAlsoCharacterArray";
	cout << "characterArray3 " << characterArray3 << endl;
	cout << "size of characterArray3 using sizeof (uncorrect way for string) " << sizeof(characterArray3)/sizeof(characterArray3[0]) << endl;
	cout << "size of characterArray3 using size (correct way for string) " << size(characterArray3) << endl;
	cout << "size of characterArray3[0] " << sizeof(characterArray3[0]) << endl;
	int arrayOfIntegers[]={1,2,3};
	cout << "Size of arrayOfIntegers " << sizeof(arrayOfIntegers) << endl;
	cout << "Size of arrayOfIntegers[0] " << sizeof(arrayOfIntegers[0]) << endl;
	cout << "&arrayOfIntegers[0] " << &arrayOfIntegers[0] << endl;
	cout << "&arrayOfIntegers[1] " << &arrayOfIntegers[1] << endl;
	cout << "&arrayOfIntegers[2] " << &arrayOfIntegers[2] << endl;

	cout << "\nFunctions**************************" << endl;
	int sumResult = functionToSumTwoIntegers(999,1);
	cout << "result " << sumResult << endl;
	int returnSum, returnDiff;
	functionToSumTwoIntegers(999,1, &returnSum, &returnDiff);
	cout << "returnSum " << returnSum << endl;
	cout << "returnDiff " << returnDiff << endl;
	// Other ways to return multiple values from functions are by using pointer to arrays, pointer to structs and STL(c++ standard template library) functions

	cout << "\nClasses and objects the most important property of C++**************************" << endl;
	// Create instances/objects of class
	MyClass person1;
	MyClass person2;
	MyClass person3;
	
	person1.myNum=1111;
	person1.myString="I like dogs";
	
	person2.myNum=2222;
	person2.myString="I like cats";
	
	person3.myNum=3333;
	person3.myString="I like bitcoins";

	cout << "person1 " << person1.myNum << " " << person1.myString  << endl;
	cout << "person2 " << person2.myNum << " " << person2.myString  << endl;
	cout << "person3 " << person3.myNum << " " << person3.myString  << endl;
	
	cout << "\nStructs**************************" << endl;
	// Create a car structure and store it in myCar1;
	car myCar1;
	myCar1.brand = "BMW";
	myCar1.model = "X5";
	myCar1.year = 1999;
	// Create another car structure and store it in myCar2;
	car myCar2;
	myCar2.brand = "Ford";
	myCar2.model = "Mustang";
	myCar2.year = 1969;
	// Print the structure members
	cout << myCar1.brand << " " << myCar1.model << " " << myCar1.year << "\n";
	cout << myCar2.brand << " " << myCar2.model << " " << myCar2.year << "\n";
	
	cout << "\nLibraries**************************" << endl;
	double a = 7.4;
        int b = 99;
    	std::cout << "a + b = " << mylibrary::Arithmetic::Add(a, b) << std::endl;
    	std::cout << "a - b = " << mylibrary::Arithmetic::Subtract(a, b) << std::endl;
    	std::cout << "a * b = " << mylibrary::Arithmetic::Multiply(a, b) << std::endl;
    	std::cout << "a / b = " << mylibrary::Arithmetic::Divide(a, b) << std::endl;
	
	// Instance can also be made but is not necessary if only function is needed 
	mylibrary::Arithmetic newArithmeticInstance;
	std::cout << "a + b = " << newArithmeticInstance.Add(a, b) << std::endl;	

	/*	
	cout << "\nInput/Output**************************" << endl;
	int inputNum; 
	cout << "Type a number between 0 and 4294967295: "; 	// Type a number and press enter
	cin >> inputNum; 					// Get user input from the keyboard
	cout << "Your number is: " << inputNum; 		// Display the input value
	*/

	return 0;

	// libraries
} 

// function definition
int functionToSumTwoIntegers(int num1, int num2){
	cout << "Started function called \"functionToSumTwoIntegers\"" << endl;
	return num1+num2;
}

// function with multiple return using pointers definition
void functionToSumTwoIntegers(int num1, int num2, int *returnSum, int *returnDiff){
	cout << "Started function called \"functionToSumTwoIntegers\"" << endl;
	*returnSum=num1+num2;
	*returnDiff=num1-num2;
}


