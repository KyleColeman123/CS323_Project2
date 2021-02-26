#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class listNode {
	public:
		string firstName;
		string lastName;
		listNode *next;
		
	listNode(string fn, string ln){
		firstName = fn;
		lastName = ln;
	}	

	void printNode(listNode *node, ofstream &outFile){
	    if (this->next == NULL)
	        outFile << "(" << this->firstName <<" "<< this->lastName <<" NULL)-->NULL";
	    if (this->next != NULL)
	        outFile << "(" << this->firstName <<" "<<this->lastName <<" , "
	        << this->next->firstName << ")-->" ;
	}
};

class hashTable {
	public:
		char op;
		int bucketSize;
		listNode* hashT[1000];


    hashTable(int n){
        bucketSize = n;
    }
	
	int Doit (string lastName){
		unsigned int val = 1;
		for (int i = 0; i<lastName.length(); i++){
			val = val*32 + (int)lastName[i];
		}
		return val%bucketSize;
	}
	
	void createHashTable (){	
		for (int i = 0; i < bucketSize; i++){
			listNode* dummy = new listNode("dummyfirst", "dummylast");
			hashT[i] = dummy;
		}	
	}
	
	void informationProcessing (ifstream &infile, ofstream &outFile2){
		string firstName;
		string lastName;
		int index;
		while (infile >> op >> firstName >> lastName){
			outFile2<<"Processing "<<op<<" "<<firstName<<" "<<lastName<<"..."<<endl;
			index = Doit(lastName);
			outFile2<<"Target Index: "<<index<<endl;
			printList(index, outFile2);
		    if (op == '+'){
		   	    hashInsert (index, firstName, lastName, outFile2);
		   	}
		   	if (op == '-'){
		   	    hashDelete (index, firstName, lastName, outFile2);	 
		   	}
		   	if (op == '?'){
		   	    hashRetrieval (index, firstName, lastName, outFile2);	    	
			}		
		}
	}
	
	listNode* findSpot (int index, string firstName, string lastName){
		listNode* spot = hashT[index];
		while (spot->next != NULL
		 && spot->next->lastName < lastName)
			spot = spot->next;
		while (spot->next != NULL
		 && spot->next->lastName == lastName
		 && spot->next->firstName < firstName)
		 	spot = spot->next;
			return spot;
	}
	void hashInsert(int index, string firstName, string lastName, ofstream &outFile2){
		outFile2 << "***Performing hashInsert on "<<firstName<<" "<< lastName<< endl;
		listNode* spot = findSpot(index, firstName, lastName);
		if (spot->next != NULL && spot->next->lastName == lastName && spot->next->firstName == firstName)
			outFile2<<"*** Warning, the record "<<firstName<< " " <<lastName<<" is in the database!"<<endl<<endl;
		else{
			listNode* newNode;
			newNode = new listNode(firstName, lastName);
			newNode->next = spot->next;
			spot->next = newNode;
			outFile2<<firstName<<" "<<lastName<<" has been INSERTED on "<<"HashTable["<<index<<"]: ";
			printList(index, outFile2);
			outFile2<<endl;		
		}
	}
	void hashDelete(int index, string firstName, string lastName, ofstream &outFile2){
		outFile2 << "***Performing hashDelete on "<<firstName<<" "<< lastName<< endl;
		listNode* spot = findSpot(index, firstName, lastName);
		if (spot->next != NULL && spot->next->lastName == lastName && spot->next->firstName == firstName){
			listNode* junk;
			junk = spot->next;
			spot->next = spot->next->next;
			junk->next = NULL;
			free(junk);
			outFile2<<firstName<<" "<<lastName<<" has been DELETED on "<<"HashTable["<<index<<"]: ";
			printList(index, outFile2);
			outFile2<<endl;
		}
		else outFile2<<"*** Warning, the record "<<firstName<< " " <<lastName<<" is NOT in the database!"<<endl<<endl;
	}
	void hashRetrieval(int index, string firstName, string lastName, ofstream &outFile2){
		outFile2 << "***Performing hashRetrieval on "<<firstName<<" "<< lastName<< endl;
		listNode* spot = findSpot(index, firstName, lastName);
		if (spot->next != NULL && spot->next->lastName == lastName && spot->next->firstName == firstName){
			outFile2<<"Yes, the record "<<firstName<< " " <<lastName<<" is in the database!"<<endl<<endl;
		}
		else outFile2<<"No, the record "<<firstName<<" "<<lastName<<" is NOT in the database!"<<endl<<endl;
	}
	void printList(int index, ofstream &outFile){
		listNode* tmp = hashT[index];
	 	while (tmp != NULL){
				tmp->printNode(tmp, outFile);
				tmp = tmp->next;
		}
		outFile<<endl;
	}
	void printHashTable (ofstream &outFile){
		for (int i; i<bucketSize; i++){
			outFile<<"HashTable["<<i<<"]: ";
			printList(i, outFile);
		}
	}
};

int main(int argc, char *argv[]){
	ifstream input;
	input.open(argv[1]);
	
	ofstream output1, output2;
	output1.open(argv[3]);
	output2.open(argv[4]);
	
	int i = atoi(argv[2]);
    hashTable myHash(i); 
    myHash.createHashTable();
    myHash.informationProcessing(input, output2);
    myHash.printHashTable(output1);
    input.close();
	
	output1.close();
	output2.close();
	cout<<"DONE";
	return 0;
}
