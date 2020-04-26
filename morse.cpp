#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h> 

const int COUNT=10; //number of spaces in printing 
//binary tree
const int HASH_SIZE=20; //number of buckets in hash table
struct hashNode //seperate chaining
{
char key;
std::string value;
};

struct bNode //for binary tree
{
struct bNode* left;
struct bNode* right;
char data;
};

int hashFunction(char alph);
char findNode(std::string morse,bNode* root);
struct bNode* newNode(char data); 
struct hashNode newHashNode(char key,std::string value);
std::string findHashTable(char alph,std::vector<struct hashNode>* hashTable);
std::string translateToAlphabet(std::string morseLine,bNode* root);
std::string translateToMorse(std::string alphLine, std::vector<struct hashNode>* hashTable);
void insertHashTable(char alph, std::string morse,std::vector<struct hashNode>* hashTable);
void insertNode(std::string morse, bNode* root,int i,char data);
void fillAssociativeArray(char alph,std::string morse,std::vector<struct hashNode>* hashTable);
void readDatabase(std::string fileName,bNode* root,std::vector<struct hashNode>* hashTable );
void readAndAttachToFile(std::string fileName,bNode* root,std::vector<struct hashNode>* hashTable);
void insertAtTheEndOfFile(std::string fileName, std::string message);
void print2DUtil(bNode *root, int space); 
void print2D(bNode *root); 
int main()
{
char choice='y';
while(choice=='Y' || choice=='y')
{	
std::string dataBaseFileName="morse.txt";
std::string ioFileName="";
std::cout<<"please enter the database name"<<std::endl;
std::cin>>dataBaseFileName;
bNode* root= newNode('\0'); //passed with null character, because root should be empty
std::vector<hashNode>* hashTable= new std::vector<hashNode>[HASH_SIZE];
//a pointer to vector of structs used as hash table with size of HASH_SIZE
readDatabase(dataBaseFileName,root,hashTable); // read the data base and make a binary tree 
//for converting morse to alphabet. and pass root to hash table to convert alphabet to morse
std::cout<<"read database"<<std::endl;
std::cout<<"please enter the file name"<<std::endl;
std::cin>>ioFileName;
readAndAttachToFile(ioFileName,root,hashTable); //attach the result of convert
//to the end of the file 
std::cout<<"read file"<<std::endl;
std::cout<<"do you want to continue using the program? if yes, enter Y or y , if not , enter n or N"<<std::endl;
std::cin>>choice;
}
std::cout<<"thank you for using the program"<<std::endl;
return 0;
}
/**
function to make a new node for hash table from key and value
@param - character, string
@return - structure
*/
hashNode newHashNode(char key,std::string value)
{
//key is alphabet character
//value is equivalant morse code
hashNode node;
node.key= key;
node.value=value;
return (node); 
}
/**
function to calculate hash code
@param - character,
@return - integer
*/
int hashFunction(char alph)
{
	int iAlph= (int)toupper(alph); //since the database includes uppercase
	//character I used to upper to get the number of the following character
	return iAlph%HASH_SIZE; //compress, and make sure it's on table range
}
/**
function to insert a new hash node (alphabet and morse) to the hash table
@param - character, string, pointer to vectore, 
@return - none
*/
void insertHashTable(char alph, std::string morse,std::vector<struct hashNode>* hashTable)
{
int hashedAlph=hashFunction(alph); //to find the index in hash table
//hashedAlph is the index
hashTable[hashedAlph].push_back(newHashNode(alph,morse));
//instert alphabet and morse code as hash node to the vector of the hash table
}
/**
function to find the morse equivalant of a character
@param - character, pointer to vector
@return - string
*/
std::string findHashTable(char alph,std::vector<struct hashNode>* hashTable)
{
	int hashedAlph=hashFunction(alph); //find index by hash function
	for(int i=0; i<hashTable[hashedAlph].size(); i++)
	{
		hashNode hashNode=hashTable[hashedAlph][i];
		if(hashNode.key==alph || hashNode.key==toupper(alph))
		//used to upper to avoid having lower case characters
		//if the character is equal to the key
		{
			return hashNode.value;
		}
	}
	//in case nothing was found in the hashtable, replace by space.
	return " ";
}

/**
function to make a new node for binary tree
@param - character
@return - pointer to bNode struct
*/
bNode* newNode(char data)
{
	bNode* node = (bNode*)malloc(sizeof(bNode)); 
	//malloc assign a block of memory to bNode and return a pointer to it
  
    // Assign data to this node 
	node->data = data; //assign alphabet
  
  // Initialize left and right pointers as NULL 
	node->left = NULL; 
	node->right = NULL; 
	return(node); 
}


/**
function to insert Node for binary tree 
@param - string, pointer to bNode, integer, data
@return - none
*/
void insertNode(std::string morse, bNode* root,int i,char data)
{
	bNode* temp=root; //set root to temp
	while(i<morse.length())
	{
		//choose the right flag  when it's '.'
		if(morse[i]=='.')
		{
			if(temp->right==NULL) //check if there's a leaf in right side
			{
				temp->right=newNode(' '); //make temp right by a space
			}
			temp=temp->right;		
		}
		else if(morse[i]=='-')
		{
			if(temp->left==NULL)
			{
				temp->left=newNode(' ');
			}
			temp=temp->left;		
		}
	i=i+1;
	}
	temp->data=data;
	return;
}


/**
function to check which morse code belong to which character
@param - string, pointer to bNode
@return - character
*/
char findNode(std::string morse,bNode* root)
{
	int i=0;
	while(i<morse.length())
	{
		if(morse[i]=='.')
		{
			root=root->right;
		}
		else if(morse[i]=='-')
		{
			root=root->left;
		}
		if(root==NULL) //empty
		{
		// cause the morse doesn't exist in the binary tree and the database
		return ' ';
		}
		i=i+1;
	}
	return root->data; //return character
}
/**
function to convert morse code to alphabet
@param - string, pointer to bNode
@return - string
*/
std::string translateToAlphabet(std::string morseLine,bNode* root)
{
std::string translatedLine;
std::string delimiter = " ";
size_t pos = 0; 
std::string morse;
//npos is null position of the string
while ((pos = morseLine.find(delimiter)) != std::string::npos) {
    //when a double space happends
     if(pos==morseLine.find("  ")) //double space: word
    {
    morse = morseLine.substr(0, pos); //seperate every 
    //characters before space
    translatedLine=translatedLine+findNode(morse,root);
    translatedLine=translatedLine+' ';
	morseLine.erase(0, pos + delimiter.length()+1);
	//erase whatever before to be able to check again without considering
	//what we checked already. +1 for space
    }
    else //word is not completed yet
    {
    morse = morseLine.substr(0, pos);
    translatedLine=translatedLine+findNode(morse,root);
    morseLine.erase(0, pos + delimiter.length());
    }
}
//	translatedLine=translatedLine+findNode(morseLine,root);
return translatedLine;
}
void fillAssociativeArray(char alph,std::string morse,std::vector<struct hashNode>* hashTable)
{
	insertHashTable(alph,morse,hashTable);
}
/**
function to convert alphabet to morse code
@param - string, pointer to vector of structs
@return - string
*/
std::string translateToMorse(std::string alphLine, std::vector<struct hashNode>* hashTable)
{
	std::string translatedLine;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string word;
	while ((pos = alphLine.find(delimiter)) != std::string::npos) 
	{
	    word = alphLine.substr(0, pos);
	    for(int i=0; i<word.length();i++)
	    	{
	    		translatedLine=translatedLine+ findHashTable(word[i],hashTable); //ith character
	    		//calculate morse of ith character and added to the line
	    	}
	    alphLine.erase(0, pos + delimiter.length());
	   	translatedLine=translatedLine+" ";
	}
	for(int i=0;i<alphLine.length();i++) //for the last word since the end
	//of the last word is not with space
	{
	translatedLine=translatedLine+ findHashTable(alphLine[i],hashTable);
	}
	translatedLine=translatedLine+" ";
return translatedLine;
}
/**
function to read from database
@param - string, pointer to vector of structs, pointer to bNode struct
@return - none
*/
void readDatabase(std::string fileName,bNode* root,std::vector<struct hashNode>* hashTable)
{
	std::ifstream in_file;
	std::string delimiter = " ";
	size_t pos = 0;
	in_file.open(fileName);
	std::string line;
	if(in_file.is_open())
	{
		

		while(std::getline(in_file,line))
		{
			pos = line.find(delimiter); //find space 
			std::string alphStr=line.substr(0,pos);
			char alph=alphStr[0]; //showing character
			std::string morse=line.substr(pos,line.length()); //showing morse code
			fillAssociativeArray(alph,morse,hashTable);
			insertNode(morse,root,0,alph);
		}	
	}	
		
	else if(in_file.fail())
	{
		std::cout <<"Error opening file: "<<std::endl;
	}
	in_file.close();
	return;
}
/**
read and attach to the end of file
@param - string, pointer to vector of structs, pointer to bNode struct
@return - none
*/
void readAndAttachToFile(std::string fileName,bNode* root,std::vector<struct hashNode>* hashTable)
{
	std::ifstream in_file;
	in_file.open(fileName);
	std::string translatedString;
	std::string line;
	if(in_file.is_open())
	{
		while(std::getline(in_file,line))
		{
			std::cout<<"...the line to be translated is :"<<std::endl;
			std::cout<<line<<std::endl;
			std::cout<<"...translating the line to ";
			if(line[0] !='.' && line[0] !='-') //if it's not - and . then it should be alphbet
			{
				std::cout<< "MORSE CODE "<<std::endl;
				translatedString=translateToMorse(line,hashTable);
			}
			else
			{
				std::cout<< "ALPHABETS"<<std::endl;
			 	translatedString=translateToAlphabet(line,root);
			}
		}	
	}
	else if(in_file.fail())
	{
		std::cout <<"Error opening file: "<<std::endl;
	}
	in_file.close();
	std::cout<<"...the translated line is : "<<std::endl;	
	std::cout<<translatedString<<std::endl;
	insertAtTheEndOfFile(fileName,translatedString);
	return;
}
/**
function to add at the end of file
@param - string, pointer to vector of structs, string
@return - none
*/
void insertAtTheEndOfFile(std::string fileName, std::string message)
{
	std::ofstream out; //output stream from program to file

 // std::ios::app is the open mode "append" meaning
 // new data will be written to the end of the file.
	out.open(fileName, std::ios::app); 
	out<<message;
	out.close();
}
// Function to print binary tree in 2D  
// It does reverse inorder traversal  
/**
function to print
@param -integer, pointer to bNode struct
@return - none
*/
void print2DUtil(bNode *root, int space)  
{  
    // Base case  
    if (root == NULL)  
        return;  
  
    // Increase distance between levels  
    space += COUNT;  
  
    // Process right child first  
    print2DUtil(root->right, space);  
  
    // Print current node after space  
    // count  
    std::cout<< std::endl;  
    for (int i = COUNT; i < space; i++)  
        std::cout<<" ";  
    std::cout<<root->data<<"\n";  
  
    // Process left child  
    print2DUtil(root->left, space);  
}  
  
// Wrapper over print2DUtil()  
void print2D(bNode *root)  
{  
    // Pass initial space count as 0  
    print2DUtil(root, 0);  
}  
