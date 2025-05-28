#pragma once


#include <sha.h>
#include <filters.h>
#include <hex.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace CryptoPP;



class BigInt { //this big int is for strings containing binary bits
	//use strings to save the binary numbers, use string operations frequently
public:

	string num;

	//operator overloading , plus operations

	BigInt() {
		num = "";
	}

	BigInt(string n) {
		this->num = n;
	}

	BigInt(BigInt& n) {
		this->num = n.num;
	}


	string hexToBin() { // any hex into binary -> 1D = 0001 1100 each hex in a 4 bit binary
		string result = "";

		for (int i = 0; i < num.length(); i++) {
			if (num[i] == '0') {
				result += "0000";
			}
			else if (num[i] == '1') {
				result += "0001";
			}
			else if (num[i] == '2') {
				result += "0010";
			}
			else if (num[i] == '3') {
				result += "0011";
			}
			else if (num[i] == '4') {
				result += "0100";
			}
			else if (num[i] == '5') {
				result += "0101";
			}
			else if (num[i] == '6') {
				result += "0110";
			}
			else if (num[i] == '7') {
				result += "0111";
			}
			else if (num[i] == '8') {
				result += "1000";
			}
			else if (num[i] == '9') {
				result += "1001";
			}
			else if (num[i] == 'A') { //hex only has capital alphabets from A-F
				result += "1010";
			}
			else if (num[i] == 'B') {
				result += "1011";
			}
			else if (num[i] == 'C') {
				result += "1100";
			}
			else if (num[i] == 'D') {
				result += "1101";
			}
			else if (num[i] == 'E') {
				result += "1110";
			}
			else if (num[i] == 'F') {
				result += "1111";
			}
			else {
				cout << "Its not hex string i guess" << endl;
			}
		}

		this->num = result;
		return result;
	}



	string operator+(BigInt& n) { //length of parameters in addition will always be same, their answers length could be different (one increased may be)
		string result = num;
		result += '1'; //just making it length n+1

		char carry = '0'; //starting
		for (int i = num.length() - 1, j = result.length() - 1; j >= 0; i--, j--) {

			if (i < 0) { //the two same length strings have finished
				result[j] = carry;
				continue;
			}

			if (num[i] == '1' && n.num[i] == '1') {
				if (carry == '1') {
					result[j] = '1';
					//carry remains 1
				}
				else {
					result[j] = '0';
					carry = '1';
				}
			}
			else if (num[i] == '0' && n.num[i] == '1') {
				if (carry == '1') {
					result[j] = '0';
					carry = '1';
					//carry remains 1
				}
				else {
					result[j] = '1';
				}

			}
			else if (num[i] == '1' && n.num[i] == '0') {
				if (carry == '1') {
					result[j] = '0';
					carry = '1';
					//carry remains 1
				}
				else {
					result[j] = '1';
				}
			}
			else if (num[i] == '0' && n.num[i] == '0') {
				if (carry == '1') {
					result[j] = '1';
					carry = '0';
					//carry remains 1
				}
				else {
					result[j] = '0';
				}
			}
			else {
				cout << "Its not a binary perhaps" << endl;
			}
		}


		return result;
	}

	//a function to generate 2 ki power in bits equal to identifier space

	string operator-(BigInt& n) { //basically the desired ft id is now one greater than range, and we are subtracting fom it
		//this is already bigger, b is smaller 
		/*
		subtraction will be between strings of same length

		*/

		//subtracting same length ids
		string result = num; //just making it this size
		char carry = '0';
		for (int i = num.length() - 1; i >= 0; i--) { //after subtraction the number comes in range of ids 
			//but its length is still one bigger
			if (num[i] == '1' && n.num[i] == '1') { //num is the upper string, cause its bigger
				if (carry == '1') {
					carry = '1'; //keep it on, there is someone ahead
					result[i] = '1';
				}
				else {
					result[i] = '0';
				}
			}
			else if (num[i] == '0' && n.num[i] == '1') {
				if (carry == '1') {
					result[i] = '0';
				}
				else {
					carry = '1';
					result[i] = '1';
				}

			}
			else if (num[i] == '1' && n.num[i] == '0') {
				if (carry == '1') {
					carry = '0';
					result[i] = '0';
				}
				else {
					result[i] = '1';
				}
			}
			else if (num[i] == '0' && n.num[i] == '0') {
				if (carry == '1') {
					result[i] = '1';
				}
				else {
					result[i] = '0';
				}
			}
			else {
				cout << "Its not a binary perhaps" << endl;
			}
		}

		return result;

	}

	//we need addition and subtraction function, addition of same length leads to 1 greater length,
	//subtraction is done after verifying the first number is greater - 2 power identifier space

	string removeMSB() {
		for (int i = 0; i < num.length() - 1; i++) {
			num[i] = num[i + 1];
		}

		num[num.length() - 1] = '\0';
		return num;
	}

	string operator=(string a) {
		this->num = a;
		return num;
	}

	string operator=(BigInt& n) {
		this->num = n.num;
		return num;
	}



	bool operator<(BigInt& n) { //remmeber these are for only same lenght strings
		for (int i = 0; i < num.length(); i++) {
			if (num[i] != n.num[i]) {
				if (num[i] == '1' && n.num[i] == '0') {
					return false;
				}
				else
					return true;
			}
		}
	}


	bool operator<(string n) { //remmeber these are for only same lenght strings
		for (int i = 0; i < num.length(); i++) {
			if (num[i] != n[i]) {
				if (num[i] == '1' && n[i] == '0') {
					return false;
				}
				else
					return true;
			}
		}
	}



	bool operator>(BigInt& n) {
		return !((*this) < n);
	}

	bool operator>(string n) {
		return !((*this) < n);
	}

	bool operator==(BigInt& n) {
		for (int i = 0; i < num.length(); i++) {
			if (num[i] != n.num[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator==(string a) {
		for (int i = 0; i < num.length(); i++) {
			if (num[i] != a[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(BigInt& n) {
		return !((*this) == n);
	}

	bool operator!=(string n) {
		return !((*this) == n);
	}

};

ostream& operator<<(ostream& out, BigInt& n) {
	out << n.num;
	return out;
}

istream& operator>>(istream& in, BigInt& n) {
	in >> n.num;
	return in;
}

string getTheseLSBs(string b, int identifierSpace) { //a binary string and the number of bits to get from its right, max ca be 160 so its fine
	string result = "";
	for (int i = b.length() - (identifierSpace); i < b.length(); i++) {
		result += b[i];
	}
	return result;
}

string powerof2(int n, int identifierSpace) { //n max can be equal to max identifier space -1 i-e 160 -1
	// the number of bits it should be in
//suppose i is power of 2
	string result = "";
	for (int i = 1; i <= identifierSpace; i++) { //stringsize should be equal to identifierspace selected by user
		result += '0'; //just making it that size
	}

	for (int i = 0, j = result.length() - 1; i <= n; i++, j--) {
		if (i == 0) {
			result[j] = '1';
		}
		else {
			result[j] = '1';
			result[j + 1] = '0';   ///keeep check is it right?
		}
	}
	return result;
}


string giveSha(string s) { //give anything, machine name, txt file content in a string(after being read, png file read
	SHA1 Sconverter;
	string result;
	//s is the string                               result is the 40 Hex digit key
	StringSource(s, true, new HashFilter(Sconverter, new HexEncoder(new StringSink(result))));

	return result;
}


////////////////pair class///////////////////////

class Pair {
public:
	BigInt Id;
	string value;
	Pair* next;

	Pair() {
		Id = ""; //no condition on their base values, so doesn't matters
		value = ""; //no condition on their base values, so doesn't matters
		next = nullptr;
	}

	bool operator<(Pair& a) {
		return (this->Id < a.Id); //using the already overloaded operators
	}

	bool operator<(BigInt& a) {
		return (this->Id < a); //using the already overloaded operators
	}

	bool operator>(Pair& a) {
		return (this->Id > a.Id); //using the already overloaded operators
	}

	bool operator>(BigInt& a) {
		return (this->Id > a); //using the already overloaded operators
	}

	bool operator==(Pair& a) {
		return (this->Id == a.Id); //using the already overloaded operators
	}

	bool operator==(BigInt& a) {
		return (this->Id == a); //using the already overloaded operators
	}

	bool operator!=(Pair& a) {
		return (this->Id != a.Id); //using the already overloaded operators
	}

	string operator=(Pair& a) { //just return value incase
		this->value = a.value; //remember to do this everywhere
		this->next = a.next; //bringing the entire node, shallow copy for now
		return (this->Id = a.Id);
	}

	string operator=(BigInt& a) { //just return value incase
		return (this->Id = a);
	}

	void deleteLinked() {
		Pair* temp = next;
		while (next) {
			temp = next;
			next = next->next;
			delete temp;
			temp = nullptr;
		}
		temp = nullptr;
	}

	~Pair() {
		
	}

};



////////////////pair class///////////////////////



/*
int main()
{
	BigInt ft(powerof2(4 - 1, 4));//identifierspace-1,identifierspace
	BigInt machineId("1111");//we on 31 machine, add 16 to it ->47th machine - 32
	BigInt checker(ft + machineId);
	BigInt upperBound(powerof2(4, 5)); //identifierspace,identifierspace+1 (max it can handle is allowed, one greater length is not)
	if (checker > upperBound) {
		while (checker > upperBound) {
			checker = checker - upperBound;
		}
	}

	checker.removeMSB();
	cout << checker << endl;
	*/
	/*
	string a;
	BigInt b;
	b.num = "11111111111000000000001111110100101010101010";
	BigInt c;
	c.num = "01010101010101111100000001101010101010001001";
	*/
	/*101010101001101111100010001011111010100110011*/
//a = b + c;
//cout << a << endl;

/*01010101010101111100000001101010101010001001*/
/*101010101001101111100010001011111010100110011*/
/*101010101001101111100010001011111010100110011*/
/*
cout << endl << endl;
b = "011100011100";
c = "111100011100";
BigInt d(c); //copy constructor automatic, no need of deep copy

cout << d<<endl;

d = c - b;
cout << d; //this function is only when first one is greater

cout <<endl<<endl<< d.removeMSB();

cout <<endl<<endl<< (b==c) << endl;


cin >> d;

cout << d;


cout << endl << endl;
string ar = "ADEF98EC";
BigInt prog(ar);
cout << prog;
cout << endl;
prog.hexToBin();
cout << prog;



	return 0;
}
*/









/*     ///////////////old main
	string a = binaryAddition("11011","000111"); //can be different lengths


	string hehe;
	ifstream myfile;
	myfile.open("C:\!Downloads\Semester 3\Data Structure\PROJECT\My coding part\j.txt");
	if (!file.is_open()) {
		cerr << "Error opening file: " << file_path << endl;
		return 1; // Return an error code
	}
	else{
		myfile >> hehe;

	myfile.close();
	string source = "H";  //This will be randomly generated somehow
	string hash = giveSha(source);
	  }
	*/

	/*
	string result = giveSha("Faiz inshAllah you will do it");
	cout << result << endl;

	cout << endl << endl;
	cout << result.length();


	result = hexToBin(result);

	cout << endl << endl<<result<<endl;
	cout << endl << endl << result.length();
	//make functions for text and png files , and work on how to read and write to files, set the b tree as well, after b tree go to ft table


	result = getTheseLSBs(result,6); //result should be binary string

	cout << endl << endl << result<<endl;
	cout << endl << endl << result.length() << endl;

	*/
