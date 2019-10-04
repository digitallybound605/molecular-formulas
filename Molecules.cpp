/*********************************
 * Name: Wilder Queen
 * Student#: 00083435
 * Class: CS 318
 * Details: Molecular Weights
 * Date: 9/30/19
 *********************************/

#include<iostream>
#include<fstream>
#include<iomanip>
#include<cctype>
using namespace std;


//===STRUCTURE DEFINITION=====================================================
struct element {
    
    string symbol;
    float weight;
    
};


//===FUNCTION PROTOTYPES======================================================
// data handling
void getElements(element elements[], int length);                               //reads in element array
void printElements(element elements[], int length);                             //prints element array
void printAtomicWeight(string formula, float weight);                           //prints formula and weight

// sorting and searching
void sortElements(element elements[], int length);                              //bubble sort element array
int searchElements(const element elements[], int length, string value);         //binary search

// calculating
float atomicWeight(const element elements[], int length, string symbol, int number);    //returns weight of element
void formulas(const element elements[], int length);                      //parses file and calls weight and print

//===GLOBAL VARIABLES=========================================================
ofstream outFile;
int MAX = 30;                                   //for elements in array


int main()
{
    
    //===VARIABLES=============================================================
    element elems[MAX];                                     //struct array for elements
    
    //===EXECUTION=============================================================
    outFile.open("MolecularWeight.out");
    
    // get elements from file and sort them alphabetically by symbol
    getElements(elems, MAX);
    sortElements(elems, MAX);
    
    // go through formulas file and calculate and print
    formulas(elems, MAX);
    
    outFile << endl;
    outFile.close();
    return 0;
}


//===FUNCTION DEFINITIONS=======================================================

//=========================================
// Function: getElements()
// Description: reads in and stores elements
//=========================================
void getElements(element elements[], int length)
{
    ifstream inFile;
    inFile.open("Element.dat");
    
    for(int i = 0; i < length; i++)
    {
        inFile >> elements[i].symbol;
        inFile >> elements[i].weight;
    }
    
    
    inFile.close();
}


//=========================================
// Function: printElements()
// Description: prints elements
//=========================================
void printElements(element elements[], int length)
{
    cout << "Symbol " << setw(10) << "Weight" << endl;
    for(int i = 0; i < length; i++)
        cout << elements[i].symbol << " " << setw(15) << elements[i].weight << endl;
    
}


//=========================================
// Function: sortElements()
// Description: sorts element array with bubble sort
//=========================================
void sortElements(element elements[], int length)
{
    string tempSym;                                          //variable to store temp symbol
    float tempW;                                             //variable to store temp weight
    
    //total passes through array
    for(int pass = 0; pass < (length - 1); pass++)
    {
        //each pass through elements
        for(int i = 0; i < (length - 1); i++)
        {
            // If they need swapped
            if(elements[i].symbol > elements[i+1].symbol)
            {
                tempSym = elements[i].symbol;
                tempW = elements[i].weight;
                elements[i].symbol = elements[i+1].symbol;
                elements[i].weight = elements[i+1].weight;
                elements[i+1].symbol = tempSym;
                elements[i+1].weight = tempW;
            }
        }
    }
}


//=========================================
// Function: searchElements()
// Description: searches element array with binary search
//=========================================
int searchElements(const element elements[], int length, string value)
{
    // Variables for position and pointers
    int position = -1;
    int first = 0;
    int last = length -1;
    int middle;
    
    // Flag to stop loop
    bool found = false;
    
    // While not found and pointers havent passed each other
    while(!found && first <= last)
    {
        // Find middle
        middle = (first + last) / 2;
        
        // If value found
        if(elements[middle].symbol == value)
        {
            found = true;
            position = middle;
        }
        // If value could be in first half
        else if(elements[middle].symbol > value)
            last = middle - 1;
        // If value could be in last half
        else if(elements[middle].symbol < value)
            first = middle + 1;
    }
    
    // Return where the symbol was located in the array
    return position;
}


//=========================================
// Function: atomicWeight()
// Description: returns atomic weight of element
//=========================================
float atomicWeight(const element elements[], int length, string symbol, int number)
{
    float atomWeight;                       //weight to return
    int position;                           //where element is in element array
    
    // Get the position
    position = searchElements(elements, length, symbol);
    
    // Get the weight of the number of that element in formula
    atomWeight = elements[position].weight * number;
    
    return atomWeight;
}

//=========================================
// Function: printAtomicWeight()
// Description: prints atomic weight
//=========================================
void printAtomicWeight(string formula, float weight)
{
    // Get how many chars are in the formula to scale setw()
    int charCount = formula.length();
    
    outFile << fixed  << setprecision(3);
    
    // Adjust the setw() to scale based on formula size
    outFile << formula << " " << setw(50-charCount) << weight << endl;
}

//=========================================
// Function: formulas()
// Description: read in data, get weight and print
//=========================================
void formulas(const element elements[], int length)
{
    ifstream inFile;
    inFile.open("Formula.dat");
    
    char symChar, nextChar;                     //holds the char to read, and the next
    string symString, formString;               //the string to hold current symbol, and the formula
    int numOf, secNum;                          //holds the num of atoms of element present
    float totalWeight;                          //formula running weight counter
    
    outFile << setw(10) << "Formula" << setw(43) << "Atomic Weight" << endl << endl;
    
    // While you can still read a character
    while(inFile.get(symChar))
    {
        // Initialize all variables
        totalWeight = 0;
        numOf = 1;
        formString = "";
        symString = "";
        
        // Add character read in to symbol and formula strings. + overloaded for chars!
        symString += symChar;
        formString += symChar;
        
        // Check next character, make sure it's not a newline and infile is still good
        // This goes through and seperates each formula
        while(inFile.peek() != '\n' && inFile)
        {
            // Store next char to check
            nextChar = inFile.peek();
        
            // If you are done with element symbol, and it has a subscriopt
            if(nextChar == '(')
            {
                // Get the char, add to formula but not the symbol string
                inFile.get(symChar);
                formString += symChar;
                // Get the next char, should be a #. Add to formula
                inFile.get(symChar);
                formString += symChar;
                // Convert the char to an int (shifts by 0 to get right ascii answer)
                numOf = symChar - '0';
                
                // Check if the subscript is double digit #
                // If it is, get it, shift the first # to ten's spot and add new one
                if(inFile.peek() != ')')
                {
                    inFile.get(symChar);
                    formString += symChar;
                    secNum = symChar - '0';
                    numOf *= 10;
                    numOf += secNum;
                }
                
                // Get the ) and add to formula but not symbol string
                inFile.get(symChar);
                formString += symChar;
            }
            
            // If the next character is lower, meaning a symbol with 2 chars
            else if(islower(nextChar))
            {
                // Get char and add to symbol string and formula string
                inFile.get(symChar);
                symString += symChar;
                formString += symChar;
            }
            
            // If next char is upper, meaning a new element
            else if(isupper(nextChar))
            {
                // Get the total weight of the previous element in formula
                totalWeight += atomicWeight(elements, length, symString, numOf);
                
                // Reset symbol string and number of atoms in current element
                symString = "";
                numOf = 1;
                
                // Get the new element symbol char and add to formula and symbol strings
                inFile.get(symChar);
                symString += symChar;
                formString += symChar;
            }
        }
        
        // Accumulate weight for formula
        totalWeight += atomicWeight(elements, length, symString, numOf);
        
        // Print out the formula string and weight
        printAtomicWeight(formString, totalWeight);
    }
    
    
    inFile.close();
}
