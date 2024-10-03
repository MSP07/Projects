#include <iostream>
using namespace std;
#define IOS                  \
    ios::sync_with_stdio(0); \
    cin.tie(0);              \
    cout.tie(0);
#define endl "\n"
#define MAXN 100001
#define INF 1e18 + 1
#define MAX_WORD_SIZE 500
#define under "\033[4m"
#define blue "\033[94m"
#define bluend "\033[0m"
#define underOff "\033[0m"
#define MAX_MEANING_SIZE 10000
#include <fstream>

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
            if (!insert(word, meaning, 0))
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
        cout << "Welcome to The Dictionary. This Dictionary currently has " << count << " words.\n";
        return true;
    }

    bool insert(string word, string meaning, int user)
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
        node->meaning = meaning;
        if (user == 1)
        {
            ofstream fout;
            fout.open("dictionary.txt", ios::app);
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
            cout << "Meaning : " << blue << node->meaning << bluend << endl;
        }
        return true;
    }

    // Function to print words starting with the given prefix
    void printPrefixSearch(Node *node, char all[], int index, string prefix)
    {
        Node *pCrawl = node;
        if (pCrawl != NULL && pCrawl->isEnd() == true)
        {
            cout << prefix;
            for (int j = 0; j < index; j++)
            {
                cout << all[j];
            }
            cout << ": ";
            cout << pCrawl->meaning << endl;
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

    void prefixSearch(string prefix)
    {
        Node *node = root;
        for (int i = 0; i < prefix.size(); i++)
        {
            node = node->get(prefix[i]);
        }
        char all[100];
        printPrefixSearch(node, all, 0, prefix);
    }

    // Function to print suggestions of the string

    void printSuggestions(Node *node, string res)
    {
        // If current character is the last character of a string
        if (node->isEnd() == true)
        {
            cout << res << " ";
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
            cout << t;
            cout << ": ";
            cout << pCrawl->meaning << "\n";
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
    bool updateWord(string oldWord, string newWord)
    {
        deleteWord(oldWord);
        insert(newWord, " ", 1); //
    }
    void printSuggestedOrder(Node *node, char allWords[], int index, string s)
    {
        Node *pCrawl = node;

        if (pCrawl && pCrawl->isEnd() == true)
        {
            string t = "";
            for (int i = 0; i < index; i++)
            {
                t += allWords[i];
            }
            if (levenshtein(s, t) <3)
            {
                cout << t;
                cout << ": ";
                cout << pCrawl->meaning << "\n";
            }
        }

        for (int i = 0; i < 26; i++)
        {
            if (pCrawl->Trie[i])
            {
                allWords[index] = 'a' + i;
                printSuggestedOrder(pCrawl->Trie[i], allWords, index + 1, s);
            }
        }
    }
    void suggestOrder(string s)
    {
        char allWords[50];
        printSuggestedOrder(root, allWords, 0, s);
        cout << "\n";
    }
};

void spellChecker(Trie Tree)
{
    string s;
    while (char c = getchar())
    {
        if (c == '\n')
        {
            Tree.search(s) ? cout << s : cout << under << s << underOff << endl;
            break;
        }
        else if (c == ' ')
        {
            Tree.search(s) ? cout << s : cout << under << s << underOff << endl;
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
    cout << "Welcome" << endl;
    Trie Tree;
    Tree.readWordsFromFile("words.txt");
    string call;
    do
    {
        cout << "\nWhat would you like to do?\n";
        cout << "1. Insert a word.\n";
        cout << "2. Search a word.\n";
        cout << "3. Delete a word.\n";
        cout << "4. Print dictionary in alphabetical order.\n";
        cout << "5. Prefix search.\n";
        cout << "6. Show suggested words.\n";
        cout << "7. Show suggested words using levenshtein distance.\n";
        cout << "\n";
        fflush(stdin);
        int choice;
        cin >> choice;
        cout << "\n";
        string word;
        string meaning;

        switch (choice)
        {

        case 1:
            cout << "Enter the word you would like to insert: ";
            cin >> word;
            cout << "Enter its meaning: ";
            cin >> meaning;

            if (Tree.insert(word, meaning, 1))
            {
                cout << word << " has been added to the Dictionary.\n";
            }

            break;

        case 2:
            cout << "Enter the word you would like to search: ";
            cin >> word;
            if (!Tree.search(word))
            {
                cout << under << word << underOff << endl;
                cout << "Sorry, the word you searched for doesn't exist in the dictionary. Would you like to add it or see suggestions for it.(1/2) ";
                string add;
                cin >> add;
                if (add == "1")
                {
                    cout << "Enter its meaning: ";
                    cin >> meaning;

                    if (Tree.insert(word, meaning, 1))
                    {
                        cout << word << " has been added to the Dictionary.\n";
                    }
                }
                else if (add == "2")
                {
                    Tree.suggestOrder(word);
                }
            }

            break;

        case 3:
            cout << "Which word would you like to delete? ";
            cin >> word;

            if (Tree.deleteWord(word))
            {
                cout << word << " has been successfully deleted from the Dictionary.\n";
            }
            else
            {
                cout << "No such word exists in the Dictionary.\n";
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
            Tree.suggestOrder(word);
            break;

        default:
            cout << "Enter a valid entry.";
        }
        fflush(stdin);
        cout << "\n\nWould you like to continue or exit?(Yes/No) ";
        cin >> call;
    } while (call == "yes" || call == "Yes" || call == "YES");
    return 0;
}