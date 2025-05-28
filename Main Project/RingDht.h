#pragma once

#include "Machine.h"
using namespace std;


class RingDHT
{
public:


    Machine* root;   //tail is found through traversal
    int IdentifierSpaceee;
    int bOrder;

    //btree order 


    RingDHT(int Identifierbyuser, int border)
    {
        root = NULL;
        IdentifierSpaceee = Identifierbyuser;
        bOrder = border;
    }


    /*             ///////////useless
    bool checkIfMachineExist(string id) {

        Machine* traversal = root->next;

        while (traversal != root) {

            if (traversal->machineID == id) {

                return true;

            }
            traversal = traversal->next;


        }

        return false;


    }
    */

    Machine* returnMachinebyID(string id)   ////machine selection
    {

        Machine* temp = root;


        if (temp->machineID != id)
        {
            temp = temp->next;
            while (temp != root) {

                if (temp->machineID == id)
                {

                    return temp;

                }
                temp = temp->next;

            }


            return nullptr;

        }
        else
        {
            return temp;
        }
    }


    ///initial insertion of machines
    void insertMachine(Machine*& headdd, BigInt id, BigInt name, const string& Mname, const string& directory) {
        Machine* newNode = new Machine(id.num, name.num, bOrder);
        newNode->next = nullptr;
        newNode->Mname = Mname;


        string destinationPath = newNode->name + "\\";
        ///temporary for btree



        newNode->directoryPath = destinationPath;


        string command = "mkdir " + directory + "\\" + newNode->name;
        system(command.c_str());

        if (!headdd) {
            headdd = newNode;
            headdd->next = headdd;

        }
        else {
            Machine* temp = headdd;
            while (temp->next != headdd) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->next = headdd;



        }

        ////////////////////refreshing the ft table//////////////////////
        Machine* traversal = headdd;
        if (traversal->next == headdd)
        {

            this->ftTableforseparateNode();

        }
        else
        {
            while (traversal->next != headdd)
            {
                traversal->head = nullptr;

                traversal = traversal->next;
            }
            this->ftTableforseparateNode();
        }

        ////////////////////review//////////////////////


    }



    ///////////////////////just for insertingmachine at tail///////////

    void insertAtEnd(Machine*& headdd, Machine*& newM) {
        Machine* newNode = newM;

        if (!headdd) {
            headdd = newNode;
            newNode->next = headdd;
        }
        else {
            // Traverse to the last node
            Machine* temp = headdd;
            while (temp->next != headdd) {
                temp = temp->next;
            }

            temp->next = newNode;
            newNode->next = headdd;
        }
    }




    // suppose 2 id's come


    //if (newID.length != this->IdentifierSpaceee)
    //{
    //    // ok
    //    // else
    //    cout << "Enter the id's again";
    //}



    // also check if that machine doesnt exist


    void insertMachineBeforeID(Machine*& headdd, string newID, const string& newName, const string& Mname, const string& directory) {

        // suppose 2 id's come


    //if (newID.length <= this->IdentifierSpaceee)
    //{
    //    // ok
    //    // else
    //    cout << "Enter the id's again";
    //}



    // also check if that machine doesnt exist



        if (isDuplicateIDc(headdd, newID)) {
            cout << "Machine with ID " << newID << " already exists. Enter a different ID." << endl;
            return;
        }

        Machine* newNode = new Machine(newID, newName, bOrder);
        newNode->next = nullptr;
        newNode->Mname = Mname;

        string command = "mkdir " + directory + "\\" + newNode->name;
        system(command.c_str());

        if (!headdd) {
            headdd = newNode;
            headdd->next = headdd;
        }
        else if (headdd->machineID > newNode->machineID) {
            Machine* temp = headdd;
            while (temp->next != headdd) {
                temp = temp->next;
            }

            newNode->next = headdd;
            temp->next = newNode;

            headdd = newNode;

            headdd->bt.leaveInRange(headdd->next->bt,headdd->machineID,headdd->next->machineID);

        }
        else {
            Machine* temp = headdd;
            Machine* pred = nullptr;

            while (temp->next != headdd && temp->machineID < newID) {
                pred = temp;
                temp = temp->next;
            }

            if (temp->next == headdd) {
                newNode->next = headdd;
                temp->next = newNode;
                newNode->bt.takeInRange(newNode->next->bt,newNode->machineID,temp->machineID);
            }
            else {
                newNode->next = pred->next;
                pred->next = newNode;
                newNode->bt.takeInLessThan(newNode->next->bt,newNode->machineID);
            }
        }

        Machine* traversal = headdd;
        if (traversal->next == headdd) {
            ftTableforseparateNode();
        }
        else {
            while (traversal->next != headdd) {
                traversal->head = nullptr;
                traversal = traversal->next;
            }
        }
    }


    void DisplayBTree(BigInt machineID)
    {
        Machine* target = returnMachinebyID(machineID.num);
        target->bt.print();


    }




    void deleteFileFromMachine(Machine*& targetMachine,BigInt fileID , const string& directory) {
        /*Machine* targetMachine = nullptr;
        Machine* temp = machines;
        do {
            if (temp->machineID == targetMachineId) {
                targetMachine = temp;
                break;
            }
            temp = temp->next;
        } while (temp != machines);*/

        Pair* pai = targetMachine->bt.searchbyId(fileID);

        

        if (targetMachine != nullptr && pai) {
            string fullPath = pai->value;
            size_t lastSlash = fullPath.find_last_of("\\/");
            string fileName = (lastSlash != string::npos) ? fullPath.substr(lastSlash + 1) : fullPath;

            string filePath = directory + "\\" + targetMachine->name + "\\" + fileName;

            // Try to remove the file
            if (remove(filePath.c_str()) == 0) {
                cout << "File '" << fileName << "' deleted from machine '" << targetMachine->name << "'" << endl;
                targetMachine->bt.deleteIt(pai->Id);
            }
            else {
                cerr << "Error deleting file '" << fileName << "' from machine '" << targetMachine->name << endl;
            }
        }
        else {
            cout << "Machine not found\n";
        }
    }

    void addFileToMachine(Machine*& targetMachine, const string& filePath, const string& directory) {
        /*Machine* targetMachine = nullptr;
        Machine* temp = machines;
        do {
            if (temp->machineID == targetMachineId) {
                targetMachine = temp;
                break;
            }
            temp = temp->next;
        } while (temp != machines);*/


        size_t lastSlash = filePath.find_last_of("\\/");
        string fileName = (lastSlash != string::npos) ? filePath.substr(lastSlash + 1) : filePath;

        string destinationPath = directory + "\\" + targetMachine->name + "\\" + fileName;

        targetMachine->directoryPath = destinationPath;

        string command = "copy \"" + filePath + "\" \"" + destinationPath + "\"";
        system(command.c_str());


        ////////////////////inserting file in btree //////////////////
        BigInt IdOfFile;
        IdOfFile = FilePathtoFileID(destinationPath);
        Pair* pai = new Pair;
        pai->Id = IdOfFile;
        pai->value = destinationPath;
        targetMachine->bt.insert((*pai));
        ////////////////////inserting file in btree //////////////////
       
        //// Create file in the directory
        //string command = "echo " + fileName + " > " + targetMachine->name + "\\" + fileName;
        //system(command.c_str());

        cout << "File added to machine: " << targetMachine->name << endl;


    }

    bool isDuplicateIDc(Machine* head, string targetID) {
        Machine* temp = head;
        do {
            if (temp->machineID == targetID) {
                return true;  // ID already exists
            }
            temp = temp->next;
        } while (temp != head);

        return false;  // ID doesn't exist
    }
    /*
        string readAndDisplayTextFile(const std::string& filePath) {
            std::ifstream file(filePath);

            if (!file.is_open()) {
                std::cerr << "Error opening file: " << filePath << std::endl;
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                return line;
            }

            file.close();
        }
    */

    void displayMachines(Machine* machines) {
        if (!machines) {
            cout << "No machines to display." << endl;
            return;
        }

        Machine* temp = machines;
        do {
            cout << "Machine ID: " << temp->machineID << ", Name: " << temp->name << ", Status: " << (temp->isOn ? "On" : "Off") << endl;
            temp = temp->next;
        } while (temp != machines);
        cout << endl;
    }


    void deleteMachineAtHead(Machine*& machines , const string direct) {
        if (machines != nullptr) {
            // Check if there is only one machine in the list
            if (machines->next == machines) {
                char confirmDelete;
                cout << "This is the last machine in the list. Are you sure you want to delete it? (y/n): ";
                cin >> confirmDelete;

                if (confirmDelete == 'y' || confirmDelete == 'Y') {
                    // Delete the machine directory
                    string commandDeleteDir = "rmdir /s /q \"" + direct + "\\"+ machines->name + "\"";
                    system(commandDeleteDir.c_str());

                    // Set machines to nullptr after deletion
                    machines = nullptr;

                    cout << "Last machine deleted: " << endl;
                }
                else {
                    cout << "Deletion canceled. Last machine not deleted.\n";
                }
            }
            else {
                char confirmDelete;
                cout << "This is the first machine in the list. Are you sure you want to delete it? (y/n): ";
                cin >> confirmDelete;

                if (confirmDelete == 'y' || confirmDelete == 'Y') {
                    Machine* lastMachine = machines;
                    while (lastMachine->next != machines) {
                        lastMachine = lastMachine->next;
                    }

                    // Move files to the last machine


                    string commandMoveFiles = "move " + machines->name + "\\* " + lastMachine->name;
                    system(commandMoveFiles.c_str());

                    lastMachine->next = machines->next;

                    string commandDeleteDir = "rmdir /s /q \"" + direct +"\\" + machines->name + "\"";
                    system(commandDeleteDir.c_str());

                    // Set machines to the successor after deletion
                    machines = machines->next;

                    cout << "First machine deleted: " << machines->name << ", Files shifted to successor: " << lastMachine->name << endl;
                }
                else {
                    cout << "Deletion canceled. First machine not deleted.\n";
                }
            }
        }
        else {
            cout << "No machines to delete.\n";
        }
    }




    void deleteMachine(Machine*& machines, string targetMachineId , const string& directry) {
        Machine* targetMachine = nullptr;
        Machine* temp = machines;


        do {
            if (temp->machineID == targetMachineId) {
                targetMachine = temp;
                break;
            }
            temp = temp->next;
        } while (temp != machines);

        if (targetMachine != nullptr) {

            if (targetMachine == machines) {
                deleteMachineAtHead(machines , directry);
            }

            else {
                Machine* pred = machines;
                while (pred->next != targetMachine) {
                    pred = pred->next;
                }

                pred->next = targetMachine->next;
                Machine* tempnext = targetMachine->next;
                targetMachine->next = nullptr;

                tempnext->bt.takeIn(tempnext->bt); //btree transfrred
                

                Machine* traversal = machines;
                if (traversal->next == machines) {
                    this->ftTableforseparateNode();
                }
                else {
                    while (traversal->next != machines) {
                        traversal->head = nullptr;
                        traversal = traversal->next;
                    }
                    this->ftTableforseparateNode();
                }

                // Move files to successor
                string command = "move " +directry + "\\" + targetMachine->name + "\\* " + pred->next->name;
                system(command.c_str());



                // Delete machine directory
                string commandDeleteDir = "rmdir /s /q \"" + directry+"\\" + targetMachine->name + "\"";
                system(commandDeleteDir.c_str());



                cout << "Machine deleted: " << targetMachine->name << ", Files shifted to successor: " << pred->next->name << endl;

                delete targetMachine;

            }
        }
        else {
            cout << "Machine not found\n";
        }
    }



    void ftTableforseparateNode()  /////ft table referesh node
    {

        Machine* temp = this->root;


        temp->makeFtTablestring(this->IdentifierSpaceee, this->root);

        temp = temp->next;

        while (temp != this->root)
        {

            temp->makeFtTablestring(this->IdentifierSpaceee, this->root);
            temp = temp->next;
        }

    }





    void displayFtTable(Machine* node) {



        FtTableNode* traversal = node->head;



        while (traversal != NULL) {


            cout << traversal->machine->machineID << "   ";
            cout << endl;

            traversal = traversal->next;
        }

    }

    bool isDuplicateID(BigInt id, BigInt* duplicatechecker, int size) {
        for (int i = 0; i < size; ++i) {
            if (id == duplicatechecker[i]) {
                return true;
            }
        }
        return false;
    }


    bool isValidFilePath(const std::string& filePath) {
        std::ifstream fileStream(filePath);
        return fileStream.good();
    }



    string FilePathtoFileID(const string& newFilePath)
    {

        if (isValidFilePath(newFilePath)) {

            // Open the file
            ifstream file(newFilePath);

            // Check if the file is open
            if (!file.is_open()) {
                cerr << "Error opening file: " << newFilePath << endl;

            }

            // Read the file content into a string
            stringstream buffer;
            buffer << file.rdbuf();
            string file_content = buffer.str();

            // Close the file
            file.close();


            file_content = giveSha(file_content);


            BigInt k;

            k.num = file_content;


            k.hexToBin();


            k.num = getTheseLSBs(k.num, this->IdentifierSpaceee);


            //   k normal 5 bit me aagaya
             //  this is file id

            return k.num;
        }
    }


    /////////////////////////////////////////review///////////////////////////////////

    void FTtable_traversalstring(int choicefromMAIN, string directory)
    {
        string newFileID;
        string newFilePath;
        BigInt temp;
        BigInt temp1;
        Machine* targettedMachine = nullptr;
        string tempPath;
        BigInt newFileIDBigInt;
        string targetMachineId;
        BigInt targetMachineIdBInt;
        Machine* foundMachineIDM = nullptr;
        BigInt K;
        if (choicefromMAIN == 1)
        {

            
            cout << "Enter the machine ID from where the file is to be inserted ";
            cin >> targetMachineId;
            targetMachineIdBInt =targetMachineId;
            while (!returnMachinebyID(targetMachineId))
            {


                cout << "Enter the machine ID from where the file is to be inserted again please ";
                cin >> targetMachineId;
                BigInt targetMachineIdBInt(targetMachineId);


            }
            targettedMachine = returnMachinebyID(targetMachineId);
            

            cout << "Enter file path ";
            cin >> newFilePath;

            tempPath = newFilePath;
            
            newFileIDBigInt = FilePathtoFileID(newFilePath);

            cout <<"FileHash is " << newFileIDBigInt << endl;
            temp = newFileIDBigInt;

            temp1 = targetMachineIdBInt;


        }
        else if (choicefromMAIN == 2)
        {

            cout << "Enter the machine ID from where the file is to be deleted ";
            cin >> targetMachineId;
            targetMachineIdBInt = targetMachineId;
            while (!returnMachinebyID(targetMachineId))
            {


                cout << "Enter the machine ID from where the file is to be deleted again please ";
                cin >> targetMachineId;
                BigInt targetMachineIdBInt(targetMachineId);


            }
            targettedMachine = returnMachinebyID(targetMachineId);



            

            cout << "Enter hashID :";
            cin >> newFileIDBigInt;


            temp = newFileIDBigInt;

            temp1 = targetMachineIdBInt;

            // //k =  btree.search(k);

             // k = fileinput else -1


             // k is basically our path


            K = newFileIDBigInt;

           

        }
        else if (choicefromMAIN == 3)
        {

            string hashID;
            cout << "Enter hashID :";
            cin >> hashID;

            string input = hashID;

            BigInt k;

            k = input;

            K = k;

            Machine* searchedM = returnMachinebyID(k.num);
            Pair* searcher = searchedM->bt.searchbyId(k);

            cout << "The file Id :" << searcher->Id << endl;
            cout << "Its path is :" << searcher->value << endl;

            // //k =  btree.search(k);

             // k = fileinput else -1



          

        }
        else
        {

            return;

        }


        Machine* traversaltotail = this->root->next;

        while (traversaltotail->next != this->root) {


            traversaltotail = traversaltotail->next;
        }


        BigInt foundMachineID ;
        FtTableNode* traversal = targettedMachine->head;
        cout << traversal->machine->machineID << "->";
        bool foundMachine = false;
        if (targettedMachine->machineID == newFileIDBigInt) {
            foundMachineID = targettedMachine->machineID;
            foundMachineIDM = targettedMachine;
            cout << foundMachineID;
            foundMachine = true;
        }
        else if (((traversal->machine->machineID > newFileIDBigInt || traversal->machine->machineID == newFileIDBigInt) && targettedMachine->machineID < newFileIDBigInt) || traversal->machine->machineID == newFileIDBigInt) {

            foundMachineID = traversal->machine->machineID;
            foundMachineIDM = traversal->machine;

            cout << foundMachineID;
            foundMachine = true;

        }

        else if (newFileID < targetMachineId && targetMachineId != this->root->machineID.num) {//

            traversal = root->head;
            targetMachineId = this->root->machineID.num;
            cout << traversal->machine->machineID << "->";


        }
        else if (newFileID < this->root->machineID.num || newFileIDBigInt > traversaltotail->machineID.num) {

            foundMachineID = this->root->machineID;
            foundMachineIDM = this->root;
            cout << foundMachineID;
            foundMachine = true;

        }




        if (!foundMachine) {
            if (targetMachineId == newFileIDBigInt.num) {


                foundMachineID = targetMachineId;
            }
            else {


                Machine* traversalMachine = nullptr;


                while (traversal->machine->machineID < newFileID && foundMachine != true) {


                    while (traversal->next != NULL) {//correct it



                        if (traversal->next->machine->machineID == newFileIDBigInt ) {


                            traversalMachine = traversal->machine;

                            break;

                        }
                        else {


                            if (traversal->machine->machineID < newFileIDBigInt)
                            {

                                traversalMachine = traversal->machine;

                            }
                            else if (traversal->machine->machineID > newFileIDBigInt || traversal->machine->machineID == newFileIDBigInt) {
                                foundMachineID = traversal->machine->machineID;
                                foundMachineIDM = traversal->machine;
                                foundMachine = true;
                                break;
                            }

                        }

                        traversal = traversal->next;
                        cout << traversal->machine->machineID << "->";

                    }
                    if (foundMachine == true) {
                        break;
                    }
                    traversal = traversalMachine->head;
                    cout << traversal->machine->machineID << "->";
                    if (((traversal->machine->machineID > newFileIDBigInt || traversal->machine->machineID == newFileIDBigInt) && targettedMachine->machineID < newFileIDBigInt) || traversal->machine->machineID == newFileIDBigInt) {

                        foundMachineID = traversal->machine->machineID;
                        foundMachineIDM = traversal->machine;
                        cout << foundMachineID ;
                        foundMachine = true;
                        break;

                    }
                   
                    if (traversal->machine->machineID == newFileIDBigInt) {
                        foundMachineID = traversal->machine->machineID;
                        foundMachineIDM = traversal->machine;
                        foundMachine = true;
                    }

                }




                if (foundMachine != true) {
                    foundMachineID = traversal->machine->machineID;
                }


            }
        }

        if (choicefromMAIN == 1)
        {
           //  cout << "the machine hs" << foundMachineIDM->name << foundMachineIDM->Mname << endl;
            addFileToMachine(foundMachineIDM, tempPath, directory);
            // add vala part

        }
        else if (choicefromMAIN == 2)
        {
             deleteFileFromMachine(foundMachineIDM,K.num, directory);
        }
    }


    






    /////////////////////////////////////////review///////////////////////////////////


    string filenameTOMachineID(string Mname)
    {


        Mname = giveSha(Mname);


        BigInt k;

        k.num = Mname;


        k.hexToBin();


        k.num = getTheseLSBs(k.num, IdentifierSpaceee);


        //   k normal 5 bit me aagaya
         //  this is file id

        return k.num;

    }


    };



