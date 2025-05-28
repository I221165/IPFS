#include "RingDht.h"

#include <sha.h>
#include <filters.h>
#include <hex.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace CryptoPP;
using namespace std;

int main() {






    const string dName = "Machine";

    string command = "mkdir " + dName;
    system(command.c_str());


    int mainCHOICES = -1;

    char choice;


    cout << "InterPlanaary FIle System  " << endl;


    int IdentifierSpace;
    cout << "Input Identifier Space please!   ";
    cin >> IdentifierSpace;


    while (IdentifierSpace > 160 || IdentifierSpace <= 0)
    {
        cout << "Input Identifier Space please!   ";
        cin >> IdentifierSpace;
    }

    int totalNumberOfmachines = 0;
    totalNumberOfmachines = pow(2, IdentifierSpace);



    int BtreeOrder;

    cout << "Enter the order of Btree:  ";
    cin >> BtreeOrder;



    while (BtreeOrder < 1)
    {


        cout << "Enter the order of Btree:  ";
        cin >> BtreeOrder;

    }



    int userDemandedMachinesNo = 9; //will be over written




    cout << " Enter how many machines you want to create:  ";
    cin >> userDemandedMachinesNo;

    /*
    while (userDemandedMachinesNo < 0 || userDemandedMachinesNo > totalNumberOfmachines)
    {

        cout << " Invalid. Type again";

        cin >> userDemandedMachinesNo;

    }
    */

    BigInt* dupA = new BigInt[userDemandedMachinesNo];
    BigInt* dupAR = new BigInt[userDemandedMachinesNo];
    //
    //for (int i = 0; i < userDemandedMachinesNo; i++)
    //{
    //    dupA[i] = -1;
    //}




    char choicee;
    // error add p or r
    cout << "Do you want to give random id's to the machine or your personal id's  ";
    while (true) {
        cin >> choice;

        // Check if the choice is valid
        if (choice == 'P' || choice == 'p' || choice == 'R' || choice == 'r') {
            break;  // Valid input, exit the loop
        }
        else {
            cout << "Wrong Choice. Enter 'P' or 'R' please" << endl;
        }
    }


    BigInt userDesiredID;      // BigInt
    string NameOfMachine;
    RingDHT Object(IdentifierSpace, BtreeOrder);


    int  duplicateindex = 0;
    int  duplicateindexR = 0;

    BigInt* inputIDs = new BigInt[userDemandedMachinesNo];
    BigInt* inputIDsR = new BigInt[userDemandedMachinesNo];




    if (choice == 'p' || choice == 'P')
    {

        for (int i = 0; i < userDemandedMachinesNo; ++i) {
            cout << "Enter ID: ";
            cin >> userDesiredID;

            while (userDesiredID.num.length() != IdentifierSpace)
            {
                cout << "Wrong ";
                cin >> userDesiredID;


            }


            // Check for duplicates
            // function me bigint daalo
            while (Object.isDuplicateID(userDesiredID, dupA, duplicateindex)) {
                cout << "ID is already present. Enter a different ID: ";
                cin >> userDesiredID;
            }
            inputIDs[i] = userDesiredID;
            dupA[duplicateindex] = userDesiredID;
            ++duplicateindex;


            // ascending order please



            /*
            if (userDesiredID.num.length() != IdentifierSpace)
            {
                // Object.insertMachine(Object.root, userDesiredID , "Machine" + to_string(userDesiredID ), NameOfMachine, dName);


                inputIDs[i] = userDesiredID;

            }
           else
            {
                while (userDesiredID > pow(2, IdentifierSpace))
                {
                    cout << "Wrong ID";
                    cout << endl;
                    cout << "Enter ID :";
                    cin >> userDesiredID;
                }
                cout << "Machine name" << endl;
                cin >> NameOfMachine;
                Object.insertMachine(Object.root, userDesiredID , "Machine" + to_string(userDesiredID ), NameOfMachine, dName);

            }*/
        }

        for (int i = 1; i < userDemandedMachinesNo; ++i) {
            BigInt key = inputIDs[i];
            int j = i - 1;
            while (j >= 0 && inputIDs[j] > key) {
                inputIDs[j + 1] = inputIDs[j];
                --j;
            }
            inputIDs[j + 1] = key;
        }


        for (int i = 0; i < userDemandedMachinesNo; ++i) {

            cout << "Enter Machine name for id" << inputIDs[i] << endl;
            cin >> NameOfMachine;

            Object.insertMachine(Object.root, inputIDs[i], inputIDs[i], NameOfMachine, dName);


        }

    }
    else if (choice == 'r' || choice == 'R')
    {
        for (int i = 0; i < userDemandedMachinesNo; ++i) {
            cout << "Machines name " << endl;
            cout << "Note that they are in order " << endl;
            cin >> NameOfMachine;




            string MachineId = Object.filenameTOMachineID(NameOfMachine);
            cout << MachineId << endl;



            while (Object.isDuplicateID(MachineId, dupAR, duplicateindexR)) {
                cout << "Name is already present. Enter a different Name: ";
                cin >> NameOfMachine;


                MachineId = Object.filenameTOMachineID(NameOfMachine);
                cout << MachineId << endl;

            }
            inputIDsR[i] = MachineId;
            dupAR[duplicateindexR] = MachineId;
            ++duplicateindexR;



            //  Object.insertMachine(Object.root, i, "Machine" + to_string(i), NameOfMachine, dName);
        }


        for (int i = 1; i < userDemandedMachinesNo; ++i) {
            BigInt key = inputIDsR[i];
            int j = i - 1;
            while (j >= 0 && inputIDsR[j] > key) {
                inputIDsR[j + 1] = inputIDsR[j];
                --j;
            }
            inputIDsR[j + 1] = key;
        }

        for (int i = 0; i < userDemandedMachinesNo; ++i) {



            Object.insertMachine(Object.root, inputIDsR[i], inputIDsR[i], NameOfMachine, dName);


        }
    }
    cout << "Initial state of machines:\n";
    Object.displayMachines(Object.root);





    do {
        do {






            cout << "Which work do you want to perform: " << endl;

            cout << "   (1)   File insertion  " << endl;
            cout << "   (2)   File deletion   " << endl;
            cout << "   (3)   File search  " << endl;

            cout << "   (4)   Machine Insertion" << endl;
            cout << "   (5)   Machine Deletion" << endl;

            cout << "   (6)   Display FT Table " << endl;
            cout << "   (7)   Display B-Trees" << endl;




            cin >> mainCHOICES;



            while (mainCHOICES > 5 && mainCHOICES < 0)
            {
                cout << "Wrong Enter again please:  ";
                cin >> mainCHOICES;
            }

            if (mainCHOICES == 1)
            {
                Object.FTtable_traversalstring(mainCHOICES, dName);
            }
            else if (mainCHOICES == 2)
            {
                Object.FTtable_traversalstring(mainCHOICES, dName);
            }
            else if (mainCHOICES == 3)
            {
                Object.FTtable_traversalstring(mainCHOICES, dName);
            }
            else if (mainCHOICES == 5)
            {

                string deleteMachineId;
                cout << "Enter the machine ID to be deleted: ";
                cin >> deleteMachineId;

                // add check

                Object.deleteMachine(Object.root, deleteMachineId , dName);

                cout << "\nState of machines after deleting machine:\n";
                Object.displayMachines(Object.root);

            }
            else if (mainCHOICES == 4)
            {

                string newMachineID;
                string newMachineName;


                cout << "Enter Machine ID and then NAME : ";

                cin >> newMachineID;
                cin >> newMachineName;

                Object.insertMachineBeforeID(Object.root, newMachineID, newMachineID, newMachineName, dName);

                // chcek for files that need to be here



               // Display the state of machines after inserting the new machine
                cout << "\nState of machines after inserting the new machine:\n";
                Object.displayMachines(Object.root);

            }
            else if (mainCHOICES == 6)
            {




                string marziKeMAchineID;

                cout << "Enter id of machine you want to display ft table for:  ";
                cin >> marziKeMAchineID;

                while (marziKeMAchineID.length() != IdentifierSpace)
                {

                    cout << "Enter id of machine you want to display ft table for correctly:  ";
                    cin >> marziKeMAchineID;


                }
                while (!Object.returnMachinebyID(marziKeMAchineID))
                {
                    cout << "there exist no such thing. Kindly enter a valid one";
                    cin >> marziKeMAchineID;

                }
                Object.displayFtTable(Object.returnMachinebyID(marziKeMAchineID));

            }
            else if (mainCHOICES == 7)
            {
               
                string machineIDd;
                cout << "Enter MAchine ID  ";
                cin >> machineIDd;
                Object.DisplayBTree(machineIDd);
            }


            char exitchoice;

            cout << "Do you want to continue?  press y for YES and n for NO" << endl;
            cin >> exitchoice;

            if (exitchoice == 'n')
            {
                break;
            }



        } while (true);



        string commandDeleteDir = "rmdir /s /q \"" + dName;
        system(commandDeleteDir.c_str());



        cout << "Press 'x' to exit or any other key to continue: ";
        cin >> choice;
    } while (choice != 'x');








    return 0;
}
