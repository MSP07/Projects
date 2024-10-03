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
#define MAX_MEANING_SIZE 10000

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

struct Node
{
    Node *Trie[26];
    bool flag = false;
    string meaning;

    // checks if the reference trie is present or not

    bool containKey(char ch)
    {
        return (Trie[ch - 'a'] != NULL);
    }

    // creating reference trie

    void put(char ch, Node *node)
    {
        Trie[ch - 'a'] = node;
    }
    // to get the next node for traversal
    Node *get(char ch)
    {
        return Trie[ch - 'a'];
    }
    // Node *getRoot(){
    //     return root;
    // }

    // setting flag to true at the end of the word

    void setEnd()
    {
        flag = true;
    }

    // checking if the word is completed or not

    bool isEnd()
    {
        return flag;
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
            printf("could not find/open file \"%s\"\n", filename);
            return false;
        }
        char word[MAX_WORD_SIZE];
        char meaning[MAX_MEANING_SIZE];

        int count = 0;
        while (fscanf(file, "%s %[^\n]", word, meaning) > 1)
        {
            if (!insert(word, meaning))
            {
                fclose(file);
                return false;
            }
            else
                cout << word << endl, count++;
        }
        fclose(file);
        cout << "Welcome to our Dictionary. Our Dictionary right now contains " << count << " words.\n";
        return true;
    }

    bool insert(string word, string meaning)
    {
        // initializing dummy node pointing to root initially

        Node *node = root;
        for (int i = 0; i < word.size(); i++)
        {
            if (!node->containKey(word[i]))
            {
                node->put(word[i], new Node());
            }

            // moves to reference trie

            node = node->get(word[i]);
        }
        node->setEnd();
        node->meaning = meaning;
        return true;
    }

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
            cout << "Meaning : " << node->meaning << endl;
        }
        return true;
    }
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
            cout << pCrawl->meaning << "\n";
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

        // Iterate over all possible characters of the string
        for (int i = 0; i < 26; i++)
        {

            // If current character present in the Trie
            if (node->Trie[i] != NULL)
            {

                // Insert current character into the stirng res
                res.push_back(i + 'a');
                printSuggestions(node->Trie[i], res);
                // res.pop_back();
            }
        }
    }

    // Function to check if the string is present in Trie or not
    bool checkPresent(string key)
    {

        // Traverse the string
        Node *node = root;
        for (int i = 0; i < key.length(); i++)
        {

            // If current character not present in the Trie
            if (node->get(key[i]) == NULL)
            {

                printSuggestions(node, key.substr(0, i));

                return false;
            }

            // Update root
            node = node->get(key[i]);
        }
        if (node->isEnd() == true)
        {

            return true;
        }
        printSuggestions(node, key);

        return false;
    }
    // void alphabeticalOrder(Node *node, char allWords[], int index)
    // {
    //     Node *pCrawl = root;

    //     if (pCrawl != NULL && pCrawl->isEnd() == true)
    //     {
    //         for (int j = 0; j < index; j++)
    //         {
    //             cout << allWords[j];
    //         }

    //         cout << ": ";
    //         cout << pCrawl->meaning << "\n";
    //     }

    //     for (int i = 0; i < 26; i++)
    //     {
    //         if (pCrawl->Trie[i] != NULL)
    //         {
    //             allWords[index] = 'a' + i;
    //             alphabeticalOrder(pCrawl->Trie[i], allWords, index + 1);
    //         }
    //     }
    // }
};

int main()
{
    Trie Trie;

    cout << "Welcome" << endl;
    Trie.readWordsFromFile("words.txt");

    // FILE *file = fopen("words.txt", "r");

    // if (!file)
    // {
    //     printf("could not find/open file\n");
    //     // return false;
    // }

    // char word[MAX_WORD_SIZE];
    // char mean[MAX_MEANING_SIZE];

    // int count = 0;

    // while (fscanf(file, "%s %[^\n]", word, mean) > 1)
    // {
    //     cout << word << " : " << mean << endl;
    //     Trie.insert(word, mean);
    //     count++;
    // }
    string key;
    cout << "Enter a key to check" << endl;
    cin >> key;

    // Trie.insert("abase", "To lower in position, estimation, or the like; degrade.");
    // Trie.insert("abbess", "The lady superior of a nunnery.");
    // Trie.insert("abbey", "The group of buildings which collectively form the dwelling-place of a society of monks or nuns.");
    // Trie.insert("abbot", "The superior of a community of monks.");
    // Trie.insert("abduction", "A carrying away of a person against his will, or illegally.");

    // if (Trie.checkPresent(key))
    //     cout << "The given string is present in the dictionary" << endl;

    cout << "Enter a word to search" << endl;
    string inword;
    cin >> inword;
    if (!Trie.search(inword))
    {
        cout << "Sorry we coudnt find the word in the dictionary" << endl;
    }
    // Trie.prefixSearch("abb");
}