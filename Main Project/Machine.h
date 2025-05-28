#pragma once

#include "BTREE.h"
using namespace std;

class Machine;

class FtTableNode {
public:

    FtTableNode* next;
    FtTableNode* prev;
    Machine* machine;

    FtTableNode(Machine* node) {

        machine = node;
        next = nullptr;
        prev = nullptr;

    }

};


class Machine {
public:
    
    ///////////////////////file handling///////////////////
    string name; // default name    (folder name)
    string directoryPath;
    bool isOn;
    ///////////////////////file handling///////////////////

    BigInt machineID;////////////bigint
    string Mname; // own name   (handle this)
    Machine* next;
    FtTableNode* head;
    Btree bt;
    
    Machine(string id, string nam,int btorder):bt(btorder) {
        this->machineID = id;
        this->name = nam;
        isOn = true;
        next = nullptr;
    }

    void insert(Machine* Node) {   /////ft table insertion function

        FtTableNode* newNode = new FtTableNode(Node);

        if (head == NULL) {

            head = newNode;
            head->next = NULL;
            head->prev = NULL;
            return;
        }
        else {

            FtTableNode* traversal = head;
            while (traversal->next != NULL) {

                traversal = traversal->next;

            }

            traversal->next = newNode;
            newNode->prev = traversal;
            newNode->next = NULL;
            return;

        }
    }

    bool checkMachinestring(BigInt Gid) {

        Machine* temp = this;

        if (temp->machineID == Gid)
        {
            return true;
        }

        temp = temp->next;


        while (temp->next != this->next)
        {

            if (temp->machineID == Gid)
            {
                return true;
            }
            temp = temp->next;
        }

        return false;

    }

    Machine* returnMachinestring(BigInt Gid)
    {

        Machine* temp = this;

        if (temp->machineID == Gid)
        {
            return temp;
        }

        temp = temp->next;

        while (temp->next != this->next)
        {

            if (temp->machineID == Gid)
            {
                return temp;
            }
            temp = temp->next;
        }

        return nullptr;


    }











    

    //check this
    void makeFtTablestring(int NumberOfMachinesInRoutingTable, Machine* root) {
                               //max number of machines
        if (this->next == this) {  //if its alone in the ring (circular)

            for (int i = 0; i < NumberOfMachinesInRoutingTable; i++) {


                insert(returnMachinestring(this->machineID));


            }
            return;

        }

                                  //identifier space in power of 2   ///comes in  bits one more than identifier space
        BigInt upperBound(powerof2(NumberOfMachinesInRoutingTable, NumberOfMachinesInRoutingTable+1));
        //is used in subtraction thats why one more than identifier space
        for (int i = 0; i < NumberOfMachinesInRoutingTable; i++) {


            bool FTtableChecker = false;
            BigInt ft(powerof2(i, NumberOfMachinesInRoutingTable));
            BigInt checker(ft + this->machineID);
            while (checker > upperBound) {
                checker = checker - upperBound;
            }
        

        checker.removeMSB();

        if (checkMachinestring(checker)) {                                 // bool(tosearch for node in ring dht with id )
            insert(returnMachinestring(checker));                          // simply we get that node by id and insert in ft table node --> machine member ..
        }
        else {

            Machine* traversal = this->next;
            if (this->machineID > checker) {
                traversal = root; //if after subtraction, no restrcition on making ft table
            }

            while (traversal != this) {

                if (traversal->machineID > checker || traversal->machineID == checker) {

                    insert(traversal);
                    FTtableChecker = true;
                    break;

                }
                traversal = traversal->next;

            }

            if (FTtableChecker == false) {

                insert(root);  ///////why this?
            }
        }


       }

    }


};
