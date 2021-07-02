#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "list"
#include "queue"
#include "map"
#include "vector"
using std::__fs::filesystem::directory_iterator;
using namespace std;
#include "AVL_ADT.h"

struct DATA
{
    int frequency;
    double frequencyPerThousandWords;
    double frequencyPhrasePerThousandWords;
    string key;
    string path;
    int uniquePosition;
    list<int> positions;
    map<string,double> includeKey;

    bool operator<(const DATA& d) const {
        return frequencyPerThousandWords < d.frequencyPerThousandWords;
    }
};



void print(DATA& ss)
{
    cout<<ss.key <<" ("<<ss.frequencyPerThousandWords<<" per thousand words)"<<endl;
}

void printPhrases(DATA& ss)
{
    map<string,double>::iterator it;
    for (it = ss.includeKey.begin(); it != ss.includeKey.end(); it++)
    {
        cout<<(*it).first<<" ("<<(*it).second<<" per thousand words)"<<endl;
    }
}

void printIndex(DATA& ss) {
    cout<<"Word <"<<ss.key<<"> is found in the following "<<ss.frequency<<" locations:"<<endl;
    list<int>::iterator it;
    for (it = ss.positions.begin(); it != ss.positions.end(); it++)
    {
        cout<<"("<<ss.path<<","<<(*it)<<")"<<endl;
    }
}

string removeSpecialCharacter(string& s)
{
    for (int i = 0; i < s.size(); i++) {

        // Finding the character whose
        // ASCII value fall under this
        // range
        if (s[i] < 'A' || s[i] > 'Z' && s[i] < 'a' || s[i] > 'z' || ispunct(s[i]))
        {
            // erase function to erase
            // the character
            s.erase(i, 1);
            i--;
        }
    }
    return s;
}

int main()
{
    int option = 10;
    ifstream input;
    ifstream input1;
    string path;
    string path1;
    AvlTree<DATA,string> tree;
    DATA item;
    vector<string> words;
    while(option != 0) {
        cout<<"1. Learn an index dictionary from a file (without phrases)."<<endl;
        cout<<"2. Print the index dictionary."<<endl;
        cout<<"3. Print the AVL tree of the dictionary."<<endl;
        cout<<"4. Output the dictionary to a text file."<<endl;
        cout<<"5. Load dictionary from a file."<<endl;
        cout<<"6. Learn an index dictionary from a set of files (without phrases)."<<endl;
        cout<<"7. Input a word with input suggestions."<<endl;
        cout<<"8. Learn an index dictionary from a file (with phrases)."<<endl;
        cout<<"9. Delete low frequency words from the dictionary."<<endl;
        cout<<"0. Quit."<<endl;
        cout<<"Choose your option: ";
        cin>>option;
        if(option == 0) {
            return 0;
        } else if(option == 1) {
            cout<<"Input a file name: "<<endl;
            cin>>path;
            input.open(path.c_str());
            if(!input)
            {
                cout<<"Error in opening file data.txt\n";
            }
            string tok;
            while (true)
            {
                input>>tok;
                if (input)
                {
                    list<int> listIndex;
                    removeSpecialCharacter(tok);
                    item.key = tok;
                    item.frequency = 1;
                    item.path = path;
                    item.frequencyPerThousandWords = 1.0;
                    item.frequencyPhrasePerThousandWords = 1.0;
                    item.positions = listIndex;
                    tree.AVL_Insert(item);
                } else {
                    break;
                }
            }
            input.close();
            tree.updateFrequencyPerThousandWords();
            tree.AVL_Traverse(print);
            cout<<"The total number of words/phrases in the Index Dictionary is "<<tree.AVL_Count()<<endl;
        } else if(option == 2) {
            tree.AVL_Traverse(printIndex);
        } else if(option == 3) {
            tree.AVL_Print();
        } else if(option == 4) {
            cout<<"Input a file name: "<<endl;
            cin>>path;
            //tree.printNodeInOrder(path);
            tree.printNodeBFS(path);
        } else if(option == 5) {
            string path2;
            AvlTree<DATA,string> treeDictionary;
            DATA itemOfDictionary;
            cout<<"Input a file name (dictionary.txt): "<<endl;
            cin>>path2;
            string filename(path2.c_str());
            string line;

            ifstream input_file(filename);
            if (!input_file.is_open()) {
                cerr<< "Could not open the file - '"<< filename << "'" << endl;
                return EXIT_FAILURE;
            }

            while (getline(input_file, line)){
                string str = "";
                vector<char> vector(line.begin(),line.end());
                for (auto i = vector.begin(); i != vector.end(); ++i) {
                    if(isdigit(*i)) {
                        str.push_back((*i));
                        break;
                    }
                }
                int frequency = atoi(str.c_str());
                string key = line.substr(0,line.find(' '));
                itemOfDictionary.key = key;
                itemOfDictionary.frequency = frequency;
                itemOfDictionary.frequencyPerThousandWords = (double) frequency;
                treeDictionary.AVL_Insert(itemOfDictionary);
            }
            treeDictionary.updateFrequencyPerThousandWords();
            treeDictionary.AVL_Traverse(print);
            cout<<"The total number of words/phrases in the Index Dictionary is "<<treeDictionary.AVL_Count()<<endl;
            input_file.close();
        } else if(option == 6) {
                string path = "Textfiles";
                for (const auto & file : directory_iterator(path)) {
                    AvlTree<DATA,string> AITree;
                    DATA itemAI;
                    cout<<"Learning from file: "<<file.path()<<endl;
                    input.open(file.path());
                    if(!input)
                    {
                        cout<<"Error in opening file data.txt\n";
                    }
                    string tokAI;
                    while (true)
                    {
                        input>>tokAI;
                        if (input)
                        {
                            list<int> listIndexAI;
                            removeSpecialCharacter(tokAI);
                            itemAI.key = tokAI;
                            itemAI.frequency = 1;
                            itemAI.path = file.path();
                            itemAI.frequencyPerThousandWords = 1.0;
                            itemAI.frequencyPhrasePerThousandWords = 1.0;
                            itemAI.positions = listIndexAI;
                            AITree.AVL_Insert(itemAI);
                        } else {
                            break;
                        }
                    }
                    input.close();
                    //AITree.updateFrequencyPerThousandWords();
                    //AITree.AVL_Traverse(print);
                    cout<<"The total number of words/phrases in the Index Dictionary is "<<AITree.AVL_Count()<<endl;
                }

        } else if(option == 7) {
            priority_queue<DATA> priorityQueue;
            vector<char> vectorOfChars;
            char c;
            string str = "";
            cout<<"Input a word or part of the word. Press enter to start search. Press a number to choose a word/phrase: "<<endl;
            cin>>c;
            str.push_back(c);
            vectorOfChars.push_back(c);
            tree.AVL_TraverseFindCharacter(vectorOfChars,priorityQueue);
            while(priorityQueue.size() != 0) {
                vector<DATA> vc;
                int maxSize = priorityQueue.size();
                if(priorityQueue.size() == 0) {
                    break;
                }
                int index = 0;
                while (!priorityQueue.empty() ) {
                    index++;
                    if(index > 5) {
                        cout<<"   ...and "<<maxSize-index+1<<" more words"<<endl;
                        break;
                    }
                    vc.push_back(priorityQueue.top());
                    cout<<index<<": "<<priorityQueue.top().key<<" ("<<priorityQueue.top().frequencyPerThousandWords<<" per thousand words)"<<endl;
                    priorityQueue.pop();
                }
                cout<<str;
                cin>>c;
                if(isalpha(c)) {
                    str.push_back(c);
                    vectorOfChars.push_back(c);
                    tree.AVL_TraverseFindCharacter(vectorOfChars,priorityQueue);
                    cout<<priorityQueue.size()<<endl;
                } else {
                    int index = (int)c-48;
                    cout<<"Word <"<<vc.at(index-1).key<<"> is found in the following "<<vc.at(index-1).frequency<<" locations:"<<endl;
                    list<int>::iterator it;
                    for (it = vc.at(index-1).positions.begin(); it != vc.at(index-1).positions.end(); it++)
                    {
                        cout<<"("<<vc.at(index-1).path<<","<<(*it)<<")"<<endl;
                    }
                    break;
                }
            }
        } else if(option == 8) {
            cout<<"Input a file name: "<<endl;
            cin>>path1;
            input1.open(path1.c_str());
            if(!input1)
            {
                cout<<"Error in opening file data.txt\n";
            }
            vector<string> words;
            vector<string> phrases;
            string tok1;
            while (input1>>tok1)
            {
                removeSpecialCharacter(tok1);
                words.push_back(tok1);
            }
            tree.AVL_TraverseUpdate(words,phrases);
            tree.AVL_Traverse(printPhrases);
            cout<<"The total number of words/phrases in the Index Dictionary is: "<<tree.getCountPhrases()<<endl;
        } else if(option == 9) {
            double threshold;
            cout<<"Input a threshold (say 0.5 ptw): ";
            cin>>threshold;
            map<string,double> newMap;
            tree.AVL_TraverseThreshold(threshold,newMap);
            tree.AVL_Traverse(printPhrases);
            cout<<"The total number of words/phrases in the Index Dictionary is: "<<tree.getUpdateCountPhrases()<<endl;
        }
    }
    return 0;
}
