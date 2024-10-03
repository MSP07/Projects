#include <iostream>
using namespace std;
#include <string>
#define IOS                  \
    ios::sync_with_stdio(0); \
    cin.tie(0);              \
    cout.tie(0);
#define endl "\n"
#define MAXN 100001
#define INF 1e18 + 1
#define MAX_WORD_SIZE 500
#define MAX_WORD_SIZE 500
#define header "\033[95m" // pink
#define cyan "\033[96m"
#define green "\033[92m" // green
#define warn "\033[93m"  // yellow
#define fail_ "\033[91m"  // red
#define endc "\033[0m"
#define bold "\033[1m"
#define under "\033[4m"
#define blue "\033[94m" // blue
#define bluend "\033[0m"
#define off "\033[0m"
#define MAX_MEANING_SIZE 10000
#include <fstream>

// COLOR OUTPUS ---/
// RETURN SEARCH,OTHER FUNCTIONS
// MERGE TEXT FILES --/
// DUPLICATE COUNT(TU)
// EDIT SUGGES>>NORMAL SUGESS --/
// What if prefix doesnot exist; --/

int min(int x, int y, int z)
{
    return min(min(x, y), z);
}

int letterToInt(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A';
    }
    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }
    return -1;
}

int levenshtein(string s1, string s2)
{
    int m = s1.size();
    int n = s2.size();
    int dp[m + 1][n + 1];
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0)
                dp[i][j] = j;

            else if (j == 0)
                dp[i][j] = i;

            else if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];

            // If the last character is different, consider all possibilities and find the minimum
            else
                dp[i][j] = 1 + min(dp[i][j - 1],      // Insert
                                   dp[i - 1][j],      // Remove
                                   dp[i - 1][j - 1]); // Replace
        }
    }

    return dp[m][n];
}

struct Node
{
    Node *Trie[26];
    bool flag = false;
    string meaning;
    int count = 0;

    // checks if the reference trie is present or not

    bool containKey(char ch)
    {
        return (Trie[letterToInt(ch)] != NULL);
    }

    // creating reference trie

    void put(char ch, Node *node)
    {
        Trie[letterToInt(ch)] = node;
    }

    Node *get(char ch)
    {
        return Trie[letterToInt(ch)];
    }

    void setEnd()
    {
        flag = true;
    }

    bool isEnd()
    {
        return flag;
    }

    void setEndFalse()
    {
        flag = false;
    }
};
class Trie
{
private:
    Node *root;

public:
    // constructor for creating a new root
    Trie()
    {
        // creating new obejct
        root = new Node();
    }

    bool readWordsFromFile(const char *filename)
    {
        FILE *file = fopen(filename, "r");
        if (!file)
        {
            printf("Could not find/open file \"%s\"\n", filename);
            return false;
        }
        char word[MAX_WORD_SIZE];
        char meaning[MAX_MEANING_SIZE];

        int count = 0;
        while (fscanf(file, "%s %[^\n]", word, meaning) > 1)
        {
            if (!insertFile(word,meaning,0))
            {
                fclose(file);
                return false;
            }
            else
            {
                count++;
            }
        }
        fclose(file);
        cout << blue << bold << "Welcome to The Dictionary. This Dictionary currently has " << count << " words"
             << off << endl;
        return true;
    }
    bool insertFile(string word, string meaning,int user)
    {
        // initializing dummy node pointing to root initially
        Node *node = root;
        for (int i = 0; i < word.size(); i++)
        {
            if (!node->containKey(word[i]))
            {
                node->put(word[i], new Node());
            }

            node = node->get(word[i]);
        }
        node->setEnd();
        // cout << meaning << endl;
        node->meaning = meaning;
        node->count += 1;
        if (user == 1)
        {
            ofstream fout;
            // cout << meaning << endl;
            fout.open("new.txt", ios::app);
            fout << "\n"
                 << word << " " << meaning;
            fout.close();
        }
        return true;
    }

    bool insert(string word, int user)
    {
        // initializing dummy node pointing to root initially
        string meaning;
        cout << "Enter its meaning: ";
        getline(cin,meaning);
        Node *node = root;
        for (int i = 0; i < word.size(); i++)
        {
            if (!node->containKey(word[i]))
            {
                node->put(word[i], new Node());
            }

            node = node->get(word[i]);
        }
        node->setEnd();
        // cout << meaning << endl;
        node->meaning = meaning;
        node->count += 1;
        if (user == 1)
        {
            ofstream fout;
            // cout << meaning << endl;
            fout.open("new.txt", ios::app);
            fout << "\n"
                 << word << " " << meaning;
            fout.close();
        }
        return true;
    }

    // Function to return the meaning of the given word
    bool search(string word)
    {
        Node *node = root;
        for (int i = 0; i < word.size(); i++)
        {
            if (!node->containKey(word[i]))
            {
                return false;
            }
            node = node->get(word[i]);
        }
        if (node && node->isEnd())
        {
            node->count += 1;
            cout << header << "Meaning : " << node->meaning << bluend << " " << endl;
        }
        else
            return false;
        return true;
    }

    // Function to print words starting with the given prefix
    void printPrefixSearch(Node *node, char all[], int index, string prefix)
    {
        Node *pCrawl = node;
        if (pCrawl != NULL && pCrawl->isEnd() == true)
        {
            cout << header << prefix;
            for (int j = 0; j < index; j++)
            {
                cout << all[j];
            }
            cout << ": " << off;
            cout << bold << pCrawl->meaning << off << endl;
        }

        for (int i = 0; i < 26; i++)
        {
            if (pCrawl->Trie[i] != NULL)
            {
                all[index] = 'a' + i;
                printPrefixSearch(pCrawl->Trie[i], all, index + 1, prefix);
            }
        }
    }
    // Function to search using prefix

    int prefixSearch(string prefix)
    {
        Node *node = root;
        for (int i = 0; i < prefix.size(); i++)
        {
            node = node->get(prefix[i]);
            if (!node)
            {
                cout << fail_ << "Sorry , There is no words with the given prefix in the dictionary" << off << endl;
                return 0;
            }
        }
        char all[100];
        printPrefixSearch(node, all, 0, prefix);
        return 1;
    }
    int checkWordinLine(string prefix)
    {
        Node *node = root;
        for (int i = 0; i < prefix.size(); i++)
        {
            node = node->get(prefix[i]);
            if (!node){ return 0; }
        }
        return 1;
    }

    // Function to print suggestions of the string

    void printSuggestions(Node *node, string res)
    {
        // If current character is the last character of a string
        if (node->isEnd() == true)
        {
            cout << header << res << endl;
        }
        // Iterate over all possible characters
        for (int i = 0; i < 26; i++)
        {
            if (node->Trie[i] != NULL)
            {
                // Insert current character into the stirng res
                res.push_back(i + 'a');
                printSuggestions(node->Trie[i], res);
                res.pop_back();
            }
        }
    }

    // Function to check if the string is present in Trie or not if not print suggesstions...
    bool checkPresent(string key)
    {
        // Traverse the string
        Node *node = root;
        for (int i = 0; i < key.length(); i++)
        {
            if (node->get(key[i]) == NULL)
            {
                printSuggestions(node, key.substr(0, i));

                return false;
            }
            node = node->get(key[i]);
        }
        if (node->isEnd() == true)
        {
            return true;
        }
        printSuggestions(node, key);
        cout << "\n";
        return false;
    }
    void printAlphabeticalOrder(Node *node, char allWords[], int index)
    {
        Node *pCrawl = node;

        if (pCrawl && pCrawl->isEnd() == true)
        {
            string t = "";
            for (int i = 0; i < index; i++)
            {
                t += allWords[i];
            }
            cout << warn << t;
            cout << ": ";
            cout << cyan << pCrawl->meaning << "\n"
                 << off;
        }

        for (int i = 0; i < 26; i++)
        {
            if (pCrawl->Trie[i])
            {
                allWords[index] = 'a' + i;
                printAlphabeticalOrder(pCrawl->Trie[i], allWords, index + 1);
            }
        }
    }
    void alphabeticalOrder()
    {
        char allWords[50];
        printAlphabeticalOrder(root, allWords, 0);
        cout << "\n";
    }
    bool deleteWord(string word)
    {

        Node *pCrawl = root;

        for (int i = 0; i < word.length(); i++)
        {
            int index = letterToInt(word[i]);

            if (!pCrawl->get(word[i]))
                return false;

            pCrawl = pCrawl->Trie[index];
        }

        if (pCrawl && pCrawl->isEnd())
        {
            pCrawl->setEndFalse();
            return true;
        }
        return true;
    }
    bool updateWord(string oldWord, string newWord) // return from search
    {
        deleteWord(oldWord);
        //insert(newWord, " ", 1); //
    }
    void printSuggestedOrder(Node *node, char allWords[], int index, string s, vector<string> &res)
    {
        Node *pCrawl = node;

        if (pCrawl && pCrawl->isEnd() == true)
        {
            string t = "";
            for (int i = 0; i < index; i++)
            {
                t += allWords[i];
            }
            if (levenshtein(s, t) < 3)
            {
                res.push_back(t);
                cout << warn << t;
                cout << ": ";
                cout << cyan << pCrawl->meaning << "\n"
                     << off;
            }
        }

        for (int i = 0; i < 26; i++)
        {
            if (pCrawl->Trie[i])
            {
                allWords[index] = 'a' + i;
                printSuggestedOrder(pCrawl->Trie[i], allWords, index + 1, s, res);
            }
        }
    }
    vector<string> suggestWords(string s)
    {
        char allWords[50];
        vector<string> res;
        printSuggestedOrder(root, allWords, 0, s, res);
        cout << "\n";
        return res;
    }
};

void spellChecker(Trie Tree)
{
    string s;
    while (char c = getchar())
    {
        if (c == '\n')
        {
            Tree.search(s) ? cout << s : cout << under << s << off << endl;
            break;
        }
        else if (c == ' ')
        {
            Tree.search(s) ? cout << s : cout << under << s << off << endl;
            s = "";
            cout << c;
        }
        else
        {
            s += c;
        }
    }
}

int main()
{
    // cout << "Welcome" << endl;
    Trie Tree;
    
    Tree.readWordsFromFile("new.txt");
    string call;
    do
    {
        cout << green;
        cout << "\nWhat would you like to do?\n";
        cout << "1. Insert a word.\n";
        cout << "2. Search a word.\n";
        cout << "3. Delete a word.\n";
        cout << "4. Print dictionary in alphabetical order.\n";
        cout << "5. Prefix search.\n";
        cout << "6. Show suggested words.\n";
        cout << "7. Show suggested words using levenshtein distance.\n";
        cout << "8. Print all the words starting with an alphabet.\n";
        cout << "\n";
        cout << off;
        fflush(stdin);
        string word;
        int choice;
        cin >> choice;
        cout << "\n";
        fstream f;
        f.open("new.txt",ios_base::in);
        switch (choice)
        {

        case 1:
            cout << "Enter the word you would like to insert: ";
            cin >> word;
            

            if (Tree.insert(word, 1))
            {
                cout << warn << word << " has been added to the Dictionary.\n"
                     << off;
            }

            break;

        case 2:
            cout << "Enter the word you would like to search: ";
            cin >> word;
            if (!Tree.search(word))
            {
                cout << under << word << off << endl;
                cout << fail_ << "Sorry, the word you searched for doesn't exist in the dictionary. Would you like to add it or see suggestions for it.(1/2) : " << off;
                string add;
                cin >> add;
                if (add == "1")
                {
                    if (Tree.insert(word,1))
                    {
                        cout << green << word << " has been added to the Dictionary.\n"
                             << off;
                    }
                }
                else if (add == "2")
                {
                    if (Tree.suggestWords(word).empty())
                    {
                        Tree.checkPresent(word);
                    }
                }
            }

            break;

        case 3:
            cout << "Which word would you like to delete? ";
            cin >> word;
            if(f.fail()){
                cout<<"Error opening file"<<endl;
            }
            else{
                vector<string> lines;
                string line;
                while(getline(f,line)){
                    if (line.substr(0,word.length())!=word){
                        lines.push_back(line);
                    }
                }
                f.close();
                f.open("new.txt",ios_base::out);
                for(int i=0;i<lines.size();i++){
                    f<<lines[i]<<endl;
                }
                f.close();
            }
            
            if (Tree.deleteWord(word))
            {
                cout << green << word << " has been successfully deleted from the Dictionary.\n"
                     << off;
            }
            else
            {
                cout << fail_ << "No such word exists in the Dictionary.\n"
                     << off;
            }
            break;

        case 4:
            char allWords[50];
            Tree.alphabeticalOrder();
            cout << "\n";
            break;

        case 5:
            cout << "Enter the word you would like to use as a prefix: ";
            cin >> word;
            cout << "\n";
            Tree.prefixSearch(word);
            break;

        case 6:
            cout << "Enter the word for whose suggested words you want to see: ";
            cin >> word;
            Tree.checkPresent(word);
            break;
        case 7:
            cout << "Enter the word for which suggested words you wanna see: ";
            cin >> word;
            Tree.suggestWords(word);
            break;
        case 8:
            cout << "Enter an alphabet from a to z : ";
            cin >> word;
            cout << "\n";
            Tree.prefixSearch(word);
            break;

        default:
            cout << fail_ << "Enter a valid entry." << off;
        }
        fflush(stdin);
        cout << green << "\n\nWould you like to continue ?(Yes/No) " << off;
        cin >> call;
    } while (call == "yes" || call == "Yes" || call == "YES");
    return 0;
}