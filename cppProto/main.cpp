#include <iostream>
#include <bitset>

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
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=2;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=3;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=4;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=integer << 1;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<8>(integer) << endl;
	integer=4294967295;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<32>(integer) << endl;
	integer=integer+1;
	cout << "Integer in Decimal 	" << integer << endl;
	cout << "Integer in Hexadecimal 	0x" << hex << integer << endl;
	cout << "Integer in Binary 	0b" << bitset<32>(integer) << endl;
	
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
	char characterArray[]={'T','h','i','s','I','s','C','h','a','r'};	
	cout << "Character array element 1 " << characterArray[0] << endl;
	cout << "Character array element 2 " << characterArray[1] << endl;
	cout << "Character array element 3 " << characterArray[2] << endl;
	cout << "Character array element 4 " << characterArray[3] << endl;
	cout << "Character array element 5 " << *(characterArray+4) << endl;
	cout << "Character array " << characterArray << endl;
	char characterArray2[]="AlsoCharacterArray";	
	string characterArray3="AndAlsoCharacterArray";
	cout << "characterArray3 " << characterArray3 << endl;
	cout << "Size of characterArray3 " << sizeof(characterArray3) << endl;

	return 0;

	// functions
	// class
	// struct
} 
