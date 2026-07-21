// To whom it may concern:
// fraud.txt contains a copy of what the program produced when I did the first
// successful run, with only the optimizations I could think of on my own.
// At the time of writing, there is no meaningful difference between that output and
// what you would see if you ran the code for real, but I currently plan to keep
// that functionality in place even when I do add more optimizations, for posterity.
#pragma region notmain
#pragma region wordlestuff
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;

inline string concatenate(const string& a, const string& b)
{
    return a + b;
}

int pause()
{
    //cin >> age;   // If you press enter without typing anything with this one, 
    string foo;     // it keeps waiting until you enter something for real.
    getline(cin, foo); // This one is more like Console.ReadLine();
    return 1;
}

void print(vector<char>& word, int newLine = 0, string filename = "a")
{
    if (filename == "a")
    {
        for (int i = 0; i < 5; i++)
            std::cout << word[i];
        for (int i = 0; i < newLine; i++)
            std::cout << endl;
    }
    else
    {
        ofstream fout(filename, std::ios::app);
        if (fout.is_open())
        {
            for (int i = 0; i < 5; i++)
                fout << word[i];
            for (int i = 0; i < newLine; i++)
                fout << endl;
            fout.close();
        }
        else
        {
            std::cout << "Unable to open file";
            pause();
        }
    }
}

bool mySwap(vector<char>& word, int a)
{
    if (word[a] > word[a + 1])
    {
        char temp = word[a];
        word[a] = word[a + 1];
        word[a + 1] = temp;
        return false;
    }
    return true;
}

void mySort(vector<char>& word)
{
    bool ordered = false;
    int streak = 0;
    for (int stop = 4; stop > 0; stop--)
    {
        for (int i = 0; i < stop; i++)
        {
            ordered = mySwap(word, i);
            if (ordered) streak++;
            else streak = 0;
        }
        stop -= streak;
    }
}

bool sameWord(vector<char>& a, vector<char>& b)
{
    for (int i = 0; i < 5; i++)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

bool matchLetter(vector<char>& a, vector<char>& b)
{
    //print(a);
    //print(b, 1);
    int indexA = 0, indexB = 0;
    while (indexA < 5 && indexB < 5 && (a[indexA] ^ b[indexB]))
    {
        //std::cout << a[indexA] << a[indexB] << endl;
        if (a[indexA] > b[indexB])
            indexB++;
        else indexA++;
    }
    return indexA < 5 && indexB < 5;
}

void copyWord(vector<char>& from, vector<char>& to)
{
    for (int i = 0; i < 5; i++)
    {
        to[i] = from[i];
    }
}

bool bump(vector<int>& indices, int size, int index, bool debug = false)
{
    if(debug)std::cout<<"index: "<<index<<", start: "<<indices[0]<<" "<<indices[1]<<" "<<indices[2];
    indices[index]++;
    if (indices[index] == size - (4 - index))
    {
        indices[index - 1]++;
        for (int i = index; i < 5; i++)
        {
            indices[i] = indices[i - 1] + 1;
        }
    }
    // just one way to catch a bug
    if (index != 0 && indices[index] == size - (4 - index))
    {
        indices[index - 1]--;
        bump(indices, size, index - 1, debug);
    }
    if(debug)std::cout<<", end: "<<indices[0]<<" "<<indices[1]<<" "<<indices[2]<<endl;
    return indices[0] == size - 4;
}
#pragma endregion wordlestuff

// found this on stackoverflow: https://stackoverflow.com/questions/5866529/how-do-we-clear-the-console-in-assembly/5866648#5866648
void clear_screen(char fill = ' ')
{
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

#pragma endregion notmain

int main()
{
    string input;
    string type;
    bool forreal = false;
    cout << "\nRun it for real? Will take about four hours! (Y/N)\n";
    getline(cin, input);
    if (input[0] == 'Y' || input[0] == 'y')
        forreal = true;
    clear_screen();

    if (!forreal)
    {
        ifstream fraud("fraud.txt");
        string fline;
        if (fraud.is_open())
        {
            cout << "Disclaimer: this is only a recreation of the output. I can't just run this thing for 4 hours on a whim every time I need to make a change.\n";
            while (getline(fraud, fline))
            {
                cout << fline << endl;
            }
            fraud.close();
        }
        else
        {
            std::cout << "Unable to open file";
            return pause();
        }
        return pause();
    }

    const int fileLength = 12972;
    vector<vector<char>> raw;
    string line;
    int lineNum = 0;
    ifstream myfile("five-letter-words.txt");
    if (myfile.is_open())
    {
        std::cout << "Ready...";
        //pause();
        std::cout << "Loading...\n";
        for (int i = 0; i < fileLength; i++)
        {
            raw.push_back({ ' ',' ',' ',' ',' ' });
        }
        while (getline(myfile, line))
        {
            for (int i = 0; i < 5; i++)
            {
                raw[lineNum][i] = line[i];
            }
            lineNum++;
        }
        myfile.close();
        std::cout << "Loaded! lineNum: " << lineNum << endl;
    }
    else 
    { 
        std::cout << "Unable to open file"; 
        return pause();
    }
    
    // Sort all of the words
    // This DOES copy the entire thing, which in most cases is bad practice, 
    // but this is necessary here because we need to remember what the words 
    // were pre-alphabitizing so that there can be a meaningful output.
    std::cout << "Sorting..." << endl;
    auto sorted = raw;
    for (int i = 0; i < fileLength; i++)
        mySort(sorted[i]);
    std::cout << "Sorted!" << endl;

    // prune words with duplicate letters and anagrams
    std::cout << "Pruning..." << endl;
    vector<vector<char>> pruned;
    vector<int> rawIndex;
    vector<char> temp;
    for (int i = 0; i < 5; i++)
        temp.push_back(' ');
    bool unique;
    int anagrams = 0; // just for fun
    for (int word = 1; word < fileLength; word++)
    {
        // check for duplicate letters
        unique = true;
        for (int letter = 0; letter < 4; letter++)
        {
            if (sorted[word][letter] == sorted[word][letter + 1])
                unique = false;
        }

        // now check if an anagram of the word has been added already
        if (unique)
        {
            copyWord(sorted[word], temp);
            for (int existingWord = 0; existingWord < pruned.size(); existingWord++)
            {
                // I think this is O(n^2/2) in total. Expensive, but not nearly as expensive as the main loop.
                if (sameWord(sorted[word], pruned[existingWord]))
                {
                    unique = false;
                    anagrams++;
                    existingWord = pruned.size();
                }
            }
        }

        // add the word
        if (unique)
        {
            copyWord(sorted[word], temp);
            pruned.push_back(temp); // ran a test, this pushes a copy as intended.
            // Remember which word this used to be. (keep in mind that word is an int, not a string)
            rawIndex.push_back(word);
        }
    }
    std::cout << "Pruned!" << endl;
    // 12972 -> 6222, that is HUGE! If this really is O(n^5), then this should run ~39x faster as a result!

    const int size = pruned.size();
    std::cout << pruned.size() << endl;
    //pause();

    int i = 0;
    chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();
    chrono::high_resolution_clock::time_point current_time = chrono::high_resolution_clock::now();
    chrono::high_resolution_clock::time_point previous_time = chrono::high_resolution_clock::now();
    // This size - 4 business shouldn't *really* be necessary, but better safe than sorry.
    for (int wordOne = 0; wordOne < size - 4; wordOne++)
    {
        // We don't ever have to increment wordOne beyond the loop definition,
        // because there is nothing for it to conflict with.
        // Removing the words with double letters earlier already took care of that.
        for (int wordTwo = wordOne + 1; wordTwo < size - 3; wordTwo++)
        {
            while (
                matchLetter(pruned[wordOne], pruned[wordTwo]))
            {
                wordTwo++;
                if (wordTwo == size - 3)
                    break;
            }
            for (int wordThree = wordTwo + 1; wordThree < size - 2; wordThree++)
            {
                while (
                    matchLetter(pruned[wordOne], pruned[wordThree]) ||
                    matchLetter(pruned[wordTwo], pruned[wordThree]))
                {
                    wordThree++;
                    if (wordThree == size - 2)
                        break;
                }
                // this layer slows down the program the most. It may be worth concatenating words 1-3
                // and comparing wordFour (and wordFive) to the whole thing at once.
                // UPDATE: one-hot encoding will do that for free
                for (int wordFour = wordThree + 1; wordFour < size - 1; wordFour++)
                {
                    while (
                        matchLetter(pruned[wordOne],   pruned[wordFour]) ||
                        matchLetter(pruned[wordTwo],   pruned[wordFour]) ||
                        matchLetter(pruned[wordThree], pruned[wordFour]))
                    {
                        wordFour++;
                        if (wordFour == size - 1)
                            break;
                    }
                    for (int wordFive = wordFour + 1; wordFive < size; wordFive++)
                    {
                        while (
                            matchLetter(pruned[wordOne],   pruned[wordFive]) ||
                            matchLetter(pruned[wordTwo],   pruned[wordFive]) ||
                            matchLetter(pruned[wordThree], pruned[wordFive]) ||
                            matchLetter(pruned[wordFour],  pruned[wordFive]))
                        {
                            wordFive++;
                            if (wordFive == size)
                                break;
                        }
                        if (wordFive != size)
                        {
                            i++;
                            std::cout << i << " ";
                            print(raw[rawIndex[wordOne]]);
                            std::cout << " " << wordOne << " ";
                            print(raw[rawIndex[wordTwo]]);
                            std::cout << " " << wordTwo << " ";
                            print(raw[rawIndex[wordThree]]);
                            std::cout << " " << wordThree << " ";
                            print(raw[rawIndex[wordFour]]);
                            std::cout << " " << wordFour << " ";
                            print(raw[rawIndex[wordFive]]);
                            std::cout << " " << wordFive << endl;
                            std::cout << "Discovered after: "
                                << chrono::duration_cast<chrono::seconds>(
                                    chrono::high_resolution_clock::now() - start_time).count()
                                << " seconds\n\n";
                        }
                    }
                }
            }
        }
        // benchmarking with a subset of four-word combinations:
        // 99 seconds to do 30 on ascending logical operator order
        // 149 seconds on descending order
        // I think the first word tends to hog more common letters, so it's better at eliminating
        // word 3 in particular. I still think word 5 should be checked against word 4 first, but that's it.
        // (I didn't end up committing to this strategy, it will become irrelevant anyway)
        if ((wordOne + 1) % 50 == 0)
        {
            current_time = chrono::high_resolution_clock::now();
            std::cout << "Time elapsed this loop: "
                << chrono::duration_cast<chrono::seconds>(current_time - previous_time).count()
                << " seconds\n";
            std::cout << "Total time elapsed so far: "
                << chrono::duration_cast<chrono::seconds>(current_time - start_time).count()
                << " seconds\n\n";
            previous_time = chrono::high_resolution_clock::now();
        }
    }
    std::cout << "Done!" << endl;

    return pause();
}