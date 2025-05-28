#pragma once


#include <vector>
#include <sstream>
#include <cstring>
#include <filesystem>
#include "MyBigInt.h"

///////////////////////////copied////////////////

#include <iostream>
using namespace std;

class BNode {
public:
	int order; //of tree
	Pair* keys;  //keys are integer for now
	BNode** children;
	int counter; //for number of keys right now
	BNode(int ord) { //order is the order of Btree
		order = ord;
		keys = new Pair[order]; //one more so when it goes in excess
		counter = 0; //should max go to order (its for keys only), its number of keys not index, counter = 3 means 3 keys, not till 3 index
		//when have to remove keys, just bring counter down
		children = new BNode * [order + 1]; //it might bring its pointer, if its extra just remove +1
		for (int i = 0; i <= order; i++)
			children[i] = nullptr;
		//bringing counter down will make the extra ptrs useless but still make them nullptr 
	}

	void insert(Pair key) { //for basic insertion in leaf nodes, now in string
		int index = 0; //insertion takes a pair
		for (int i = 0; i < counter; i++) {
			if (keys[i] == key) {
				//insert in next and go back, no chnage in counter whatsoever
				Pair* xtemp = &(keys[i]);
				while (xtemp->next) {
					xtemp = xtemp->next;
				}
				xtemp->next = new Pair;
				xtemp->next->Id = key.Id;
				xtemp->next->value = key.value;
				xtemp->next->next = key.next;
				return; //no counter increased, no values moved forward
			}
			else if (keys[i] > key)
			{
				index = i; //incase it goes somewhere in between
				i = counter; //just to confirm break
				break;
			}
			else if (i == counter - 1)
				index = counter; //incase it is greatest and goes to end
		}
		Pair store;//Now in BigInts because all comparisons are of ids not values
		Pair give = keys[index];  //really important for maintaining the old values , keep testing
		for (int i = index; i < order - 1; i++) {
			store = keys[i + 1];
			keys[i + 1] = give; //shifting ahead
			give = store;

		}
		keys[index] = key;  //value added
		counter++; //the total number of keys in this node incremented
	} //this function just simply enters a  key ,and will be used only if the node is a leaf
	  //which means as long as the 0th index pointer is a null
	// after using this funcion, the max keys will be checked 
	//and while splitting it wont be used (VIMP)

	void deleteKey(BigInt key) { //this function will only be used if its a simple
		//leaf node and wont be used in merging, for main functionality remember
		//non-existing functionality is handled below, here just delete it
		bool done = false; //deletion takes an id
		for (int i = 0; (i < counter && !done); i++) { //counter is number of keys
			if (keys[i] == key) {
				keys[i].deleteLinked(); //just deleting if anylinked
				for (int j = i; j < counter - 1; j++) {
					keys[j] = keys[j + 1]; //here inshAllah no issue as one is removed
					//back there we were just moving ahead, keeping all of them
				}

				done = true;
			}

		}
		counter--; //managing count of keys
	}

	~BNode() {
		//how will it be deleted

	}

};

class Btree {
public:
	int order;
	int minKeys; //for deletion will be useful i guess
	BNode* root;
	Btree(int ord) {
		order = ord;
		root = nullptr;
		minKeys = order / 2;
		if (order % 2 != 0) //ceil function 
			minKeys++;
		--minKeys; //-1 for keys ;)
	}

	/////////////////////////////insertion///////////////////////////////////////////////
private:
	void splitHelper(BNode* parent, BNode* temp, int childIndex) {
		BigInt store, give = parent->keys[childIndex].Id; //ints for now
		for (int i = childIndex; i < order - 1; i++) { //making place for keys and pointers
			store = parent->keys[i + 1].Id;
			parent->keys[i + 1] = give;
			give = store;
		} //index now free for key
		BNode* tempy, * xtemp = parent->children[childIndex + 1]; //for now
		for (int i = childIndex + 1; i < order; i++) {
			tempy = parent->children[i + 1];
			parent->children[i + 1] = xtemp;
			xtemp = tempy;
		} //children space now found
		//remember to increment counter

		int index = (temp->counter / 2);
		if (temp->counter % 2 == 0) { //if even take smaller index like simulation
			index--;
		}
		//will be used as index so no need of +1
		// this choses the point of splitting , this can be changed for left biased and right biased tree
		parent->keys[childIndex] = temp->keys[index];
		parent->children[childIndex + 1] = new BNode(order); //new children hoga ia, leaf node
		if (parent->children[childIndex] == nullptr) { //can only happen if root is split
			parent->children[childIndex] = temp;
		}
		parent->counter++; //a new key arrived along its pointer

		for (int i = index + 1, j = 0; i < temp->counter; i++, j++) {
			parent->children[childIndex + 1]->keys[j] = temp->keys[i]; //this cannot go beyound max
			parent->children[childIndex + 1]->children[j] = temp->children[i];
			temp->children[i] = nullptr;
			if (i == temp->counter - 1)
			{
				parent->children[childIndex + 1]->children[j + 1] = temp->children[i + 1];
				temp->children[i + 1] = nullptr;
			}
			parent->children[childIndex + 1]->counter++;
		}



		temp->counter = index;//for median it worked as index, here as a count, so fine


	}

	void splitRootHelp(BNode* r) { //its called only if the root is the issue
		BNode* temp = root;
		root = new BNode(order);
		splitHelper(root, temp, 0); //insert it in the first index
	}

	void insertHelp(BNode* temp, Pair key, bool& excess) {
		if (temp->children[0] == nullptr) { //if left most is null means its a leaf node
			temp->insert(key);
			//now check if it has gone above max keys
			if (temp->counter == order) { //keys max should be order-1 but are = order

				excess = true;
			}
		}
		else { //still not in a leaf
			int index = 0;
			for (int i = 0; i < temp->counter; i++) {
				if (temp->keys[i] == key) {
					//insert in next and go back, no chnage in counter whatsoever
					Pair* xtemp = &(temp->keys[i]);
					while (xtemp->next) {
						xtemp = xtemp->next;
					}
					xtemp->next = new Pair;
					xtemp->next->Id = key.Id;
					xtemp->next->value = key.value;
					xtemp->next->next = key.next;
					return; //no counter increased, no values moved forward
				}
				else if (temp->keys[i] > key) { //will go to its smaller child
					index = i;
					i = temp->counter; //just to break
					break;
				}
				else if (i == temp->counter - 1) {
					index = temp->counter; //right child of last one
				}
			}
			insertHelp(temp->children[index], key, excess);
			if (excess == true) { //means a leaf has excess
				splitHelper(temp, temp->children[index], index); //filhal to ia return hoa na
				//if after this temp exceeds, excess remains true, if not it is false
				//in this way it can be carried till root
				if (temp->counter < order)
					excess = false; //otherwise will return to previous iteration
				//as true and will run again
			}

		}
	}
public:
	void insert(Pair key) { //Bigint now
		if (root == nullptr) {
			root = new BNode(order);
			root->insert(key); //it is leaf node,thats why using this
		}
		else {
			bool excess = false;
			insertHelp(root, key, excess);
			if (excess == true) { //means root has excess than max and root is leaf
				splitRootHelp(root); //incase root has more
				excess = false;
			}
		}
	}


	///////////////////////////////deletion//////////////////////////////////////////
	//starting step wise
	//first is leaf node deletion
private:
	void merge(BNode* parent, BNode* child, int position) { //neither sibling has an excess
		if (position != parent->counter) { //right is a possiblity,left is the defficient one
			BNode* newOne = new BNode(order);
			if (child->counter != 0) {
				for (int i = 0; i < child->counter; i++) {
					newOne->keys[newOne->counter] = child->keys[i];
					newOne->children[newOne->counter] = child->children[i];
					if (i == child->counter - 1) {
						newOne->children[newOne->counter + 1] = child->children[i + 1];
					}
					newOne->counter++;
				}

			}
			else if (child->children[0]) { //if it has 0 keys yet points to a children
				newOne->children[0] = child->children[0]; //even if nullptr no issue
			}

			newOne->keys[newOne->counter] = parent->keys[position]; //no node from here
			newOne->counter++;

			for (int i = 0; i < parent->children[position + 1]->counter; i++) {
				newOne->keys[newOne->counter] = parent->children[position + 1]->keys[i];
				newOne->children[newOne->counter] = parent->children[position + 1]->children[i];
				if (i == parent->children[position + 1]->counter - 1) {
					newOne->children[newOne->counter + 1] = parent->children[position + 1]->children[i + 1];
				}
				newOne->counter++;
			}

			for (int i = position; i < parent->order - 1; i++) {
				parent->keys[i] = parent->keys[i + 1];
			} //keys handled

			delete parent->children[position];
			delete parent->children[position + 1];
			parent->children[position] = newOne;
			parent->children[position + 1] = nullptr;
			for (int i = position + 1; i < order; i++) {
				parent->children[i] = parent->children[i + 1];
			}
			parent->children[parent->counter] = nullptr; //old counter
			parent->counter--;
		}
		else { //do it with left one
			BNode* newOne = new BNode(order);

			for (int i = 0; i < parent->children[position - 1]->counter; i++) {
				newOne->keys[newOne->counter] = parent->children[position - 1]->keys[i];
				newOne->children[newOne->counter] = parent->children[position - 1]->children[i];
				if (i == parent->children[position - 1]->counter - 1) {
					newOne->children[newOne->counter + 1] = parent->children[position - 1]->children[i + 1];
				}
				newOne->counter++;
			}

			newOne->keys[newOne->counter] = parent->keys[position - 1]; //no node from here
			newOne->counter++;

			if (child->counter != 0) {
				for (int i = 0; i < child->counter; i++) {
					newOne->keys[newOne->counter] = child->keys[i];
					newOne->children[newOne->counter] = child->children[i];
					if (i == child->counter - 1) {
						newOne->children[newOne->counter + 1] = child->children[i + 1];
					}
					newOne->counter++;
				}

			}
			else if (child->children[0]) { //if it has 0 keys yet points to a children
				newOne->children[newOne->counter] = child->children[0]; //even if nullptr no issue
			}

			for (int i = position - 1; i < parent->order - 1; i++) {
				parent->keys[i] = parent->keys[i + 1];
			} //keys handled

			delete parent->children[position];
			delete parent->children[position - 1];
			parent->children[position - 1] = newOne;
			parent->children[position] = nullptr;
			for (int i = position; i < order; i++) {
				parent->children[i] = parent->children[i + 1];
			}
			parent->children[parent->counter] = nullptr; //old counter
			parent->counter--;
		}

		//delete parent respective key and two pointers, then add the newOne
	}

	void borrowOrMerge(BNode* parent, BNode* child, int position) { //child here cam be leaf or parent
		//all childs are leaf node, remember
		if (position != parent->counter && parent->children[position + 1]->counter > minKeys) { //right child can be used
			//middle is defficient , right has excess
			child->keys[child->counter] = parent->keys[position];
			child->counter++;
			child->children[child->counter] = parent->children[position + 1]->children[0];

			parent->keys[position] = parent->children[position + 1]->keys[0]; //parents count doesn'r change

			for (int i = 0; i < parent->children[position + 1]->order - 1; i++) {
				parent->children[position + 1]->keys[i] = parent->children[position + 1]->keys[i + 1];
				parent->children[position + 1]->children[i] = parent->children[position + 1]->children[i + 1];

				if (i == parent->children[position + 1]->counter - 2) {
					parent->children[position + 1]->children[i + 1] = parent->children[position + 1]->children[i + 2];
				}
			}
			parent->children[position + 1]->counter--; //i guess the trasfer in above loop is right

		}
		else if (position != 0 && parent->children[position - 1]->counter > minKeys) { //left child can be used
			//middle is defficient , left has excess
			BNode* gtemp = child->children[0];
			BNode* stemp = nullptr;
			BigInt store;//Bigint now 
			BigInt give = child->keys[0].Id;  //really important for maintaining the old values , keep testing
			for (int i = 0; i < child->order - 1; i++) {
				store = child->keys[i + 1].Id;
				child->keys[i + 1] = give; //shifting ahead
				give = store;

				stemp = child->children[i + 1];
				child->children[i + 1] = gtemp;
				gtemp = stemp;
				if (i == child->order - 2) {
					child->children[i + 2] = gtemp; //stroing the last in counter index
				}

			} //emptying 0th positions //first step to move all of defficient ahead to bring from left excess

			child->counter++;

			child->keys[0] = parent->keys[position - 1];
			child->children[0] = parent->children[position - 1]->children[parent->children[position - 1]->counter]; //its last child

			parent->keys[position - 1] = parent->children[position - 1]->keys[parent->children[position - 1]->counter - 1]; //parents count doesn't change

			parent->children[position - 1]->children[parent->children[position - 1]->counter] = nullptr;

			parent->children[position - 1]->counter--; //i guess the trasfer in above loop is right

		} //borrow done
		else { //must be merged now
			merge(parent, child, position);
		}
	}

	BNode* predecessor(BNode* temp) {
		if (temp) {
			if (!temp->children[0]) {
				return temp;
			}
			else {
				return predecessor(temp->children[temp->counter]); //key cannot be at counter index but if its not a leaf, a child can be
			}
		}
	}

	BNode* search(BNode* temp, BigInt key, bool& done) {
		if (temp && temp->counter > 0) {
			for (int i = 0; (i < temp->counter && !done); i++) {
				if (temp->keys[i] == key) {
					done = true;
					return temp;
				}
				else if (temp->keys[i] > key) {
					if (temp->children[0]) {
						return search(temp->children[i], key, done);
					}
					else {
						cout << "File not found" << endl;
						done = true;
						return nullptr;
					}
				}
				else if (i == temp->counter - 1) {
					if (temp->children[0]) {
						return search(temp->children[i + 1], key, done);
					}
					else {
						cout << "File not found" << endl;
						done = true;
						return nullptr;
					}
				}
			}
		}
		else {
			cout << "File not found" << endl;
			return nullptr;
		}
	}

	void searchReplace(Pair& keyRep, Pair& keyReq) {
		bool done = false;
		BNode* temp = search(root, keyRep.Id, done);
		if (temp) {
			for (int i = 0; i < temp->counter; i++) {
				if (temp->keys[i] == keyRep) {
					temp->keys[i] = keyReq; //replacing everything also the pointers nullptr -> whatever the other has
					break;
				}
			}
		}
		else
			cout << "Not working";
	}

	void deleteHelper(BNode* temp, BigInt key, bool& done, bool& check) {
		if (temp) {
			if (temp->counter == 0) { //no keys in this node
				cout << "Key doesn't exists" << endl;
				done = true;
				return;
			}

			for (int i = 0; (i < temp->counter && !done); i++) {
				if (temp->keys[i] == key) {
					if (temp->children[0] != nullptr) { //if it is an internal node

						BNode* predChild = predecessor(temp->children[i]); //sending values smaller than it
						Pair keyReq;
						keyReq = predChild->keys[predChild->counter - 1]; //ints for now
						//this now has the things
						//key req has pointers now as well
						predChild->keys[predChild->counter - 1].next = nullptr; //so delete linked doesnt deletes them


						Pair keyRep;
						keyRep = temp->keys[i];
						keyRep.deleteLinked(); //deleting its stuff after saving above's

						//temp deleted and replaced by the value, now calling deleteHelp on it
						deleteIt(keyReq.Id);//just delete the id and the value, which i have copied
						//save delete and ive it to the place
						searchReplace(keyRep, keyReq); //keys for now, values later
						//  //its greatest, as it is itself the greatest node on the left side(from the smaller values)
						//giving it first will cause errors as it will try to delete it from the parent
					}
					else { // its a leaf nod
						temp->deleteKey(key);
						if (temp->counter < minKeys) {
							check = true; //if it was called by root(leaf) then no need to do anything just delete, otherwise if root or anyother is a parent to it, you need to inform them
						} //otherwise no need
					}


					done = true; //no need to run the loop further
					//only deal with the parent if needed
				}
				else if (temp->keys[i] > key) {
					if (temp->children[0] == nullptr) { //is a leaf node
						cout << "Key doesn't exists" << endl;
						done = true;
						return;
					}
					else {
						deleteHelper(temp->children[i], key, done, check);
						if (check == true) {
							//parent  that child    its index
							borrowOrMerge(temp, temp->children[i], i);
							if (temp->counter >= minKeys) { //temp is parent to leaf here
								check = false; //only then
							}//how to handle height shrink
						}
						//same index as it is smaller than the value, so same index of children has smaller values already in it
					}
				}
				else if (i == temp->counter - 1) {
					if (temp->children[0] == nullptr) { //is a leaf node
						cout << "Key doesn't exists" << endl;
						done = true;
						return;
					}
					else {
						deleteHelper(temp->children[temp->counter], key, done, check);
						if (check == true) {
							//parent  that child    its index
							borrowOrMerge(temp, temp->children[temp->counter], temp->counter);
							if (temp->counter >= minKeys) { //temp is parent to leaf here
								check = false; //only then
							}//how to handle height shrink
						}
						//counter is not possible to be inserted in keys as index, but is possible for children as they are one greater
					}
				}
			}
		}
	}
public:
	void deleteIt(BigInt key) {
		if (!root) {
			return;
		}

		bool done = false;
		bool check = false;
		deleteHelper(root, key, done, check);

		if (root && root->counter == 0) {
			if (root->children[0]) {
				BNode* temp = root;
				root = root->children[0];
				delete temp;
				temp = nullptr;
			}
			else {
				delete root;
				root = nullptr; //tree got restarted
			}
		}

		//no need to check if check is on meaning if root is defficient
		//root is allowed to have less keys
	}


	//////////////////////////////////searching for a file/////////////////////////////
	bool search(BigInt key) { //for now returns bool, later inshALlah value
		bool done = false;
		BNode* temp = search(root, key, done);
		if (temp) {
			for (int i = 0; i < temp->counter; i++) {
				if (temp->keys[i] == key) {
					cout << "File found" << endl;
					cout << "ID : " << temp->keys[i].Id << endl;
					cout << "Value : " << temp->keys[i].value << endl;
					Pair* xtemp = temp->keys[i].next;
					while (xtemp != nullptr) {
						cout << "Value : " << xtemp->value << endl;
						xtemp = xtemp->next;
					}
					return true; //we can return value too inshAllah
					break;
				}
			}
		}
		else
		{   //it already couts above
			return false;
		}
	}

	Pair* searchbyId(BigInt key) { //for now returns bool, later inshALlah value
		bool done = false;
		BNode* temp = search(root, key, done);
		if (temp) {
			for (int i = 0; i < temp->counter; i++) {
				if (temp->keys[i] == key) {
					cout << "File found" << endl;
					cout << "ID : " << temp->keys[i].Id << endl;
					cout << "Value : " << temp->keys[i].value << endl;
					Pair* xtemp = temp->keys[i].next;
					while (xtemp != nullptr) {
						cout << "Value : " << xtemp->value << endl;
						xtemp = xtemp->next;
					}
					return &(temp->keys[i]); //we can return value too inshAllah
					break;
				}
			}
		}
		else
		{   //it already couts above
			return nullptr;
		}
	}


	/////////////////////////////////printing a btree///////////////////////////////////
private:
	void printHelper(BNode* temp) {
		if (temp && temp->counter > 0) {

			for (int i = 0; i < temp->counter; i++) {
				if (temp->children[i]) {
					printHelper(temp->children[i]);
				}

				Pair* xtemp = &(temp->keys[i]);
				cout << "Id: " << xtemp->Id << endl;
				while (xtemp) {
					cout << "Path: " << xtemp->value << endl;
					xtemp = xtemp->next;
				}

				if (i == temp->counter - 1 && temp->children[i + 1]) {
					printHelper(temp->children[i + 1]);
				}
			}

		}
		return;
	}
public:
	void print() {
		printHelper(root);
	}


	////////////////////getting another btree's all nodes in////////////////////////
				 ////this is for when one machine gets deleted//////////////
			/////its entire b tree goes to its successor ///////////////////////
		  //////////all the files get copied there into the successor//////////////

	BNode* getLeftMost(BNode* temp) {
		if (temp) {
			if (temp->children[0]) {
				return getLeftMost(temp->children[0]);
			}
			else {
				return temp;
			}
		}
		else {
			return nullptr;
		}
	}

	void takeIn(Btree& a) { //obviously same orders
		//how will it be taken
		BNode* temp;
		while (a.root) {
			temp = getLeftMost(a.root);
			Pair* xtemp = new Pair;
			xtemp->next = temp->keys[0].next; //copying the entire string
			xtemp->value = temp->keys[0].value;
			xtemp->Id = temp->keys[0].Id;
			//change them here
			//change them here
			this->insert((*xtemp)); //chnge values of all ofthem, a chain or a key
			xtemp = nullptr;
			temp->keys[0].next = nullptr; //so it doesnt deletes the chain 
			a.deleteIt(temp->keys[0].Id);
		} //correct it properly
	}
private:
	void insertInThisTable(BNode* temp, BigInt k) {
		if (temp && temp->counter > 0) {

			for (int i = 0; i < temp->counter; i++) {
				if (temp->children[i]) {
					insertInThisTable(temp->children[i], k);
				}

				if (temp->keys[i] < k || temp->keys[i] == k) {
					Pair* xtemp = new Pair;
					xtemp->next = temp->keys[i].next; //copying the entire string
					xtemp->value = temp->keys[i].value;
					xtemp->Id = temp->keys[i].Id;
					//change them here
					//change them here
					this->insert((*xtemp)); //chnge values of all ofthem, a chain or a key
					xtemp = nullptr;
					temp->keys[i].next = nullptr; //so it doesnt deletes the chain later
					temp->keys[i].value = "-1"; ///indication to delete them
				}

				if (i == temp->counter - 1 && temp->children[i + 1]) {
					insertInThisTable(temp->children[i + 1], k);
				}
			}

		}
		return;
	}

	void deleteFromThisTable(Btree& a, BNode* temp, bool& done) {
		if (done && temp && temp->counter > 0) {

			for (int i = 0; (done && i < temp->counter); i++) {
				if (temp->children[i]) {
					deleteFromThisTable(a, temp->children[i], done);
				}

				if (done && temp->keys[i].value == "-1") { //didn't overload
					a.deleteIt(temp->keys[i].Id);
					done = false;
					return;
				}

				if (done && i == temp->counter - 1 && temp->children[i + 1]) {
					deleteFromThisTable(a, temp->children[i + 1], done);
				}
			}

		}
		return;
	}

	void insertInThisTableRange(BNode* temp, BigInt up, BigInt down) {
		if (temp && temp->counter > 0) {

			for (int i = 0; i < temp->counter; i++) {
				if (temp->children[i]) {
					insertInThisTableRange(temp->children[i], up, down);
				}

				if ((temp->keys[i] < up || temp->keys[i] == up) && temp->keys[i] > down) {
					Pair* xtemp = new Pair;
					xtemp->next = temp->keys[i].next; //copying the entire string
					xtemp->value = temp->keys[i].value;
					xtemp->Id = temp->keys[i].Id;
					//change them here
					//change them here
					this->insert((*xtemp)); //chnge values of all ofthem, a chain or a key
					xtemp = nullptr;
					temp->keys[i].next = nullptr; //so it doesnt deletes the chain later
					temp->keys[i].value = "-1"; ///indication to delete them
				}

				if (i == temp->counter - 1 && temp->children[i + 1]) {
					insertInThisTableRange(temp->children[i + 1], up, down);
				}
			}

		}
		return;
	}

	void takeOtherThanRange(BNode* temp, BigInt up, BigInt down) {
		if (temp && temp->counter > 0) {

			for (int i = 0; i < temp->counter; i++) {
				if (temp->children[i]) {
					takeOtherThanRange(temp->children[i], up, down);
				}

				if ((temp->keys[i] < down || temp->keys[i] == down) || temp->keys[i] > up) {
					Pair* xtemp = new Pair;
					xtemp->next = temp->keys[i].next; //copying the entire string
					xtemp->value = temp->keys[i].value;
					xtemp->Id = temp->keys[i].Id;
					//change them here
					//change them here
					this->insert((*xtemp)); //chnge values of all ofthem, a chain or a key
					xtemp = nullptr;
					temp->keys[i].next = nullptr; //so it doesnt deletes the chain later
					temp->keys[i].value = "-1"; ///indication to delete them
				}

				if (i == temp->counter - 1 && temp->children[i + 1]) {
					takeOtherThanRange(temp->children[i + 1], up, down);
				}
			}

		}
		return;
	}

public:
	//actually takes in less than or equal to
	void takeInLessThan(Btree& a, BigInt k) { //this tree, takes all the files from the parameter tree (along chaining) tat have id less than or equal to k
		//first just take these Pairs into this tree and make their pointers null in that pointer
		insertInThisTable(a.root, k);
		bool done = false;
		while (!done) {
			done = true;
			deleteFromThisTable(a, a.root, done);
		}
		//k is actually the machine id of this btree's machine
	}


	void takeInRange(Btree& a, BigInt up, BigInt down) {
		//first just take these Pairs into this tree and make their pointers null in that pointer
		insertInThisTableRange(a.root, up, down);
		bool done = false;
		while (!done) {
			done = true;
			deleteFromThisTable(a, a.root, done);
		}
		//k is actually the machine id of this btree's machine
	}
	//up is old head   //down is new head
	void leaveInRange(Btree& oldhead, BigInt up, BigInt down) { //this will leave a range of values in the old head
		//new head will leav files > itslef and less than equal to old head's id in the old head

		//first just take these Pairs into this tree and make their pointers null in that pointer
		takeOtherThanRange(oldhead.root, up, down);
		bool done = false;
		while (!done) {
			done = true;
			deleteFromThisTable(oldhead, oldhead.root, done);
		}
		//k is actually the machine id of this btree's machine

	}

	~Btree() {
		//how will it be deleted
	}

};





















/*
int main() {


	BigInt n01;
	n01 = "0001";
	BigInt n02;
	n02 = "0010";
	BigInt n03;
	n03 = "0011";
	BigInt n04;
	n04 = "0100";
	BigInt n05;
	n05 = "0101";
	BigInt n06;
	n06 = "0110";
	BigInt n07;
	n07 = "0111";
	BigInt n00;
	n00 = "0000";
	BigInt n15;
	n00 = "1111";

	Pair n0;
	n0 = n00;
	Pair n1;
	n1 = n01;
	Pair n2;
	n2 = n02;
	Pair n3;
	n3 = n03;
	Pair n4;
	n4 = n04;
	Pair n5;
	n5 = n05;
	Pair n6;
	n6 = n06;
	Pair n7;
	n7 = n07;
	Pair n015;
	n015 = n15;


	Btree a(3);


	a.insert(n0);
	a.insert(n2);
	a.insert(n1);
	a.insert(n2);
	a.insert(n3);
	a.insert(n3);
	a.insert(n4);
	a.insert(n5);
	a.insert(n2);
	a.insert(n6);
	a.insert(n2);
	a.insert(n7);
	a.insert(n2);
	a.insert(n3);
	a.insert(n015);


	Btree b(3);
	BigInt idOfNewHead("0010"); //upper will be
	BigInt idOfOldHead("0111"); //lower wont be included
	b.leaveInRange(a, idOfOldHead, idOfNewHead);
	//target to leave nodes in a only in the range of new head id > && oldhead <= 
	cout << "A:" << endl;
	a.print();
	cout << "B:" << endl;
	b.print();
	*/
	/*
	BigInt fileId("10AD");//came from a hash
	string value = "C:/fahhf/fsjfh"; //lets say its path

	Pair a;
	a.Id = fileId;
	a.value = value;

	cout << a.Id << endl;
	a.Id.hexToBin();
	cout << a.Id << endl;
	*/

	/*

	Btree a(4);

	a.insert(1);
	a.insert(2);
	a.insert(3);
	a.insert(4);
	a.insert(5);
	a.insert(6);
	a.insert(7);
	a.insert(8);
	a.insert(9);
	a.insert(22);
	a.insert(23);
	a.insert(12);

	cout << a.root->keys[0] <<" "<<endl;

	cout << a.root->children[0]->keys[0] << " " << endl;
	cout << a.root->children[0]->counter << " " << endl;

	cout << a.root->children[1]->keys[0] << " " << endl;
	cout << a.root->children[1]->keys[1] << " " << endl;
	cout << a.root->children[1]->keys[2] << " " << endl;


	cout << endl << endl << a.root->children[1]->children[0]->keys[0];
	cout << endl << endl << a.root->children[1]->children[1]->keys[0];
	cout << endl << endl << a.root->children[1]->children[2]->keys[0];
	cout << endl << endl << a.root->children[1]->children[3]->keys[0];
	*/
	/*
	a.insert('D');

	a.insert('H');

	a.insert('Z');
	a.insert('K');

	a.insert('B');

	a.insert('P');

	a.insert('Q');

	a.insert('E');

	a.insert('A');

	a.insert('S');

	a.insert('W');

	a.insert('T');

	a.insert('C');

	a.insert('L');

	a.insert('N');

	a.insert('Y');

	a.insert('M');
	a.deleteIt('M');

	cout << char(a.root->keys[1]) << endl;

	cout << char(a.root->children[0]->keys[1]) << endl;
	*/
	/*
		Btree B(4);
		B.insert(1);
		B.insert(2);
		B.insert(3);
		B.insert(4);
		B.insert(5);
		B.insert(6);
		B.insert(7);
		B.insert(8);
		B.insert(9);
		B.insert(10);
		B.insert(11);
		B.insert(12);
		B.insert(13);
		B.insert(14);
		B.insert(15);
		B.insert(16);
		B.insert(17);
		B.insert(18);
		B.insert(19);
		B.insert(20);

		cout << B.root->children[B.root->counter]->children[B.root->children[B.root->counter]->counter]->keys[0] << endl;
		cout << B.root->children[B.root->counter]->children[B.root->children[B.root->counter]->counter]->keys[1] << endl;
		cout << B.root->children[B.root->counter]->children[B.root->children[B.root->counter]->counter]->keys[2] << endl;
		*/
		/*
		BTreePrinter printer;
		Btree t(5);




		t.insert(1);
		t.insert(2);
		t.insert(3);
		t.insert(4);
		t.insert(5);
		t.insert(6);
		t.insert(7);
		t.insert(8);
		t.insert(9);
		t.insert(10);

		t.insert(11);
		t.insert(12);
		t.insert(13);
		t.insert(14);
		t.insert(15);
		t.insert(16);
		t.insert(17);
		t.insert(18);
		t.insert(19);
		t.insert(20);

		t.insert(21);
		t.insert(22);
		t.insert(23);
		t.insert(24);
		t.insert(25);
		t.insert(26);
		t.insert(27);
		t.insert(28);
		t.insert(29);
		t.insert(30);

		t.insert(31);
		t.insert(32);
		t.insert(33);
		t.insert(34);
		t.insert(35);
		t.insert(36);
		t.insert(37);
		t.insert(38);
		t.insert(39);
		t.insert(40);

		t.insert(41);
		t.insert(42);
		t.insert(43);
		t.insert(44);
		t.insert(45);
		t.insert(46);
		t.insert(47);
		t.insert(48);
		t.insert(49);
		t.insert(50);

		t.insert(51);
		t.insert(52);
		t.insert(53);
		t.insert(54);
		t.insert(55);
		t.insert(56);
		t.insert(57);
		t.insert(58);
		t.insert(59);
		*/

		/*
		t.deleteIt(36);

		printer.print(t);
		*/

/*


	return 0;
}

*/