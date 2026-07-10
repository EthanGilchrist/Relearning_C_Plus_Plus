// To whom it may concern:
// sins.txt was created to debug an issue where some words were not being alphabatized correctly.
// It is no longer used, or even mentioned, anywhere in the code, but I kept it and gave fraud.txt
// its name because I thought it was kind of funny how it sounded like something out of Ultrakill.
// Speaking of which, fraud.txt is the output this program produced when I ran it properly, before
// I tacked on all the polymorphism stuff, which I simply copied from the terminal and pasted into it.
// By selecting Wordle and then No from the main menu, you can see the program output without actually
// waiting four hours for it to run.
#pragma region notmain
#pragma region wordlestuff
#include <iostream>
#include <string> // bruh
// ok no seriously, did they leave C++ this way as a sort of warning shot or what?
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;

// oh no, I unironically have to have these prototypes here
// if I want to keep main() at the top of the program and
// still be able to call them.
void syntax();
inline string concatenate(const string& a, const string& b);
int pause()
{
    //cin >> age;   // If you press enter without typing anything with this one, 
    string age;     // it keeps waiting until you enter something for real.
    getline(cin, age); // This one is more like Console.ReadLine();
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
#pragma region polymorphism
class Animal {
public:
    void speak() {
        cout << "edward\n";
    }
    string name;
    float weight;
    float height;
    int age;
    int legs = -1;
    bool upsideDown = true;
    void amputate() {
        if (legs == 0)
            return;
        legs--;
    }
    Animal(string nameIn, int ageIn, float weightIn, float heightIn) : name(nameIn), age(ageIn), weight(weightIn), height(heightIn), legs(-1) {}
};

class Cat : public Animal {
public:
    void speak() {
        cout << "Meow\n";
    }
    int legs = 4;
    Cat(string nameIn, int ageIn, float weightIn = 10.0, float heightIn = 10.0) : Animal(nameIn, ageIn, weightIn, heightIn) {}
    bool declawed = false;
};

class Dog : public Animal {
public:
    void speak() {
        int noise = rand() % 9;
        if (noise < 4)
            cout << "Bark\n";
        else if (noise < 8)
            cout << "Woof\n";
        else
            cout << "Bow Wow\n";
    }
    int legs = 4;
    Dog(string nameIn, int ageIn, float weightIn = 10.0, float heightIn = 10.0) : Animal(nameIn, ageIn, weightIn, heightIn) {}
    string breed = "Unknown";
    // polymorphism seems relatively useless in C++...
};

class Fish : public Animal {
public:
    void speak() {
        cout << "Blub\n";
    }
    int legs = 0;
    bool freshwater = true;
    Fish(string nameIn, int ageIn, float weightIn = 10.0, float heightIn = 10.0) : Animal(nameIn, ageIn, weightIn, heightIn) {}
};

class Bird : public Animal {
public:
    void speak() {
        int noise = rand() % 2;
        if (noise < 1)
            cout << "Tweet\n";
        else
            cout << "Chirp\n";
    }
    int legs = 2;
    Bird(string nameIn, int ageIn, float weightIn, float heightIn) : Animal(nameIn, ageIn, weightIn, heightIn) {}
};

class Pets {
public:
    int total = 0;
    vector<Dog> dogs = {};
    vector<Cat> cats = {};
    vector<Bird> birds = {};
    vector<Fish> fishes = {};
    void speak(int index)
    {
        if (index < dogs.size())
            dogs[index].speak();
        index -= dogs.size();
        if (index < cats.size())
            cats[index].speak();
        index -= cats.size();
        if (index < birds.size())
            birds[index].speak();
        index -= birds.size();
        if (index < fishes.size())
            fishes[index].speak();
    }
    void flip(int index)
    {
        if (index < dogs.size())
            dogs[index].upsideDown = !dogs[index].upsideDown;
        index -= dogs.size();
        if (index < cats.size())
            cats[index].upsideDown = !cats[index].upsideDown;
        index -= cats.size();
        if (index < birds.size())
            birds[index].upsideDown = !birds[index].upsideDown;
        index -= birds.size();
        if (index < fishes.size())
            fishes[index].upsideDown = !fishes[index].upsideDown;
    }
    string getName(int index)
    {
        if (index < dogs.size())
            return dogs[index].name;
        index -= dogs.size();
        if (index < cats.size())
            return cats[index].name;
        index -= cats.size();
        if (index < birds.size())
            return birds[index].name;
        index -= birds.size();
        if (index < fishes.size())
            return fishes[index].name;
        return "null";
    }
    int getLegs(int index)
    {
        if (index < dogs.size())
            return dogs[index].legs;
        index -= dogs.size();
        if (index < cats.size())
            return cats[index].legs;
        index -= cats.size();
        if (index < birds.size())
            return birds[index].legs;
        index -= birds.size();
        if (index < fishes.size())
            return fishes[index].legs;
        return -1;
    }
    float getHeight(int index)
    {
        if (index < dogs.size())
            return dogs[index].height;
        index -= dogs.size();
        if (index < cats.size())
            return cats[index].height;
        index -= cats.size();
        if (index < birds.size())
            return birds[index].height;
        index -= birds.size();
        if (index < fishes.size())
            return fishes[index].height;
        return -1;
    }
    float getWeight(int index)
    {
        if (index < dogs.size())
            return dogs[index].weight;
        index -= dogs.size();
        if (index < cats.size())
            return cats[index].weight;
        index -= cats.size();
        if (index < birds.size())
            return birds[index].weight;
        index -= birds.size();
        if (index < fishes.size())
            return fishes[index].weight;
        return -1;
    }
    int getAge(int index)
    {
        if (index < dogs.size())
            return dogs[index].age;
        index -= dogs.size();
        if (index < cats.size())
            return cats[index].age;
        index -= cats.size();
        if (index < birds.size())
            return birds[index].age;
        index -= birds.size();
        if (index < fishes.size())
            return fishes[index].age;
        return -1;
    }
    string getType(int index)
    {
        if (index < dogs.size())
            return "dog";
        index -= dogs.size();
        if (index < cats.size())
            return "cat";
        index -= cats.size();
        if (index < birds.size())
            return "bird";
        index -= birds.size();
        if (index < fishes.size())
            return "fish";
        return "Unknown";
    }
};

void newerPet(string& type, Animal* newPet)
{
    string temp;
    cout << "\n\nWhat is the " << type << "'s name?\n";
    getline(cin, (*newPet).name);
    cout << "How old is " << (*newPet).name << "?\n";
    getline(cin, temp);
    (*newPet).age = stoi(temp);
    cout << "How many pounds does " << (*newPet).name << " weigh?\n";
    getline(cin, temp);
    (*newPet).weight = stof(temp);
    cout << "How many inches tall is " << (*newPet).name << "?\n";
    getline(cin, temp);
    (*newPet).height = stof(temp);
}

void newPet(string& type, Pets& pets)
{
    // all of this code used to actually make sense, but it was dependent
    // on some false assumptions about how classes work in C++
    //Animal newPet = Animal("", 0, 0.0, 0.0);
    Dog* newDog = new Dog("", 0, 0.0, 0.0);
    Cat* newCat = new Cat("", 0, 0.0, 0.0);
    Bird* newBird = new Bird("", 0, 0.0, 0.0);
    Fish* newFish = new Fish("", 0, 0.0, 0.0);
    string temp;
    if (type == "dog")
    {
        newerPet(type, newDog);
        pets.dogs.push_back(*newDog);
        pets.total++;
    }
    else if (type == "cat")
    {
        newerPet(type, newCat);
        pets.cats.push_back(*newCat);
        pets.total++;
    }
    else if (type == "fish")
    {
        newerPet(type, newFish);
        pets.fishes.push_back(*newFish);
        pets.total++;
    }
    else if (type == "bird")
    {
        newerPet(type, newBird);
        pets.birds.push_back(*newBird);
        pets.total++;
    }
    else
    {
        cout << "Error: what on earth is a \"" << type << "\"?\n";
        throw "Animal not implemented";
        // the fun thing about throwing an error is that as long as the
        // code compiles, it doesn't matter what happens at runtime!
    }
}
#pragma endregion polymorphism

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

void displayMenu()
{
    clear_screen();
    cout << "What would you like to do?\n";
    cout << "(W)ordle results\n";
    cout << "Add (d)og\n";
    cout << "Add (c)at\n";
    cout << "Add (b)ird\n";
    cout << "Add (f)ish\n";
    cout << "Tell your pets to (s)peak\n";
    cout << "Tell your pets to (r)oll over\n";
    cout << "(L)ist all of your pets\n";
}

#pragma endregion notmain

int main()
{
    string input;
    string type;
    Pets pets;
    bool forreal = false;
    while (input[0] != 'q')
    {
        displayMenu();
        getline(cin, input);
        if (input[0] == 'e' || input[0] == 'E' || input[0] == '-' || input[0] == '0' || input[0] == 'q' || input[0] == 'Q')
        {
            return 0;
        }
        else if (input[0] == 'w' || input[0] == 'W' || input[0] == '1')
        {
            cout << "\nRun it for real? Will take about four hours! (Y/N)\n";
            getline(cin, input);
            if (input[0] == 'Y' || input[0] == 'y')
                forreal = true;
            break;
        }
        else if (input[0] == 'd' || input[0] == 'D' || input[0] == '2')
        {
            type = "dog";
            newPet(type, pets);
        }
        else if (input[0] == 'c' || input[0] == 'C' || input[0] == '3')
        {
            type = "cat";
            newPet(type, pets);
        }
        else if (input[0] == 'b' || input[0] == 'B' || input[0] == '4')
        {
            type = "bird";
            newPet(type, pets);
        }
        else if (input[0] == 'f' || input[0] == 'F' || input[0] == '5')
        {
            type = "fish";
            newPet(type, pets);
        }
        else if (input[0] == 's' || input[0] == 'S' || input[0] == '6') 
        {
            cout << endl << endl;
            for (int i = 0; i < pets.total; i++)
            {
                cout << pets.getName(i) << " says ";
                pets.speak(i);
            }
            cout << "Press enter to continue...";
            pause();
        }
        else if (input[0] == 'r' || input[0] == 'R' || input[0] == '7')
        {
            cout << "\n\nYou know this is a terminal, right? (Y/N)\n";
            pause();
            cout << "Ok, here they go...\n";
            Sleep(2500);
            for (int i = 0; i < pets.total; i++)
            {
                pets.flip(i);
            }
            cout << "They're all upside-down right now, just trust me bro\n";
            Sleep(1500);
            for (int i = 0; i < pets.total; i++)
            {
                pets.flip(i);
            }
            cout << "...and done!\n";
            Sleep(2500);
        }
        else if (input[0] == 'l' || input[0] == 'L' || input[0] == '8')
        {
            cout << endl << endl;
            for (int i = 0; i < pets.total; i++)
            {
                cout << pets.getName(i) << " the " << pets.getType(i) << " has " << pets.getLegs(i) << " legs, ";
                cout << "weighs " << pets.getWeight(i) << " pounds, " << "is " << pets.getHeight(i);
                cout << " inches tall, and is " << pets.getAge(i) << " years old.\n";
            }
            cout << "Press enter to continue...";
            pause();
        }
        else
        {
            cout << "Dude, what are you talking about?\n";
            cout << "(Press enter to continue...)";
            pause();
        }
    }
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
    // This DOES copy the entire thing. Just because it's a bad idea, doesn't mean C++ will stop you. 
    // (To be clear, I'm doing this because we need to remember what the words 
    // were before I jumbled them so that we can have an actual output.)
    std::cout << "Sorting..." << endl;
    auto sorted = raw;
    for (int i = 0; i < fileLength; i++)
    mySort(sorted[i]);
    std::cout << "Sorted!" << endl;

    // prune stupid words and anagrams
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
                if (sameWord(sorted[word], pruned[existingWord])) // h**k this is expensive. O(n^2/2) in total.
                {
                    unique = false;
                    anagrams++;
                    existingWord = pruned.size();
                }
                // the alternative is to run a sorting algorithm on sorted to sort the sorted words against each other
                // there's a chance I'll end up needing to do that regardless... But what algorithm would I use???
                // probably merge sort?
                // Update: In hindsight, this is definitely worth it, although I doubt
                // that this will speed up the program this by more than maybe double.
            }
        }

        // add the word
        if (unique)
        {
            copyWord(sorted[word], temp);
            pruned.push_back(temp); // ran a test, this pushes a copy as intended.
            rawIndex.push_back(word); // that's an int, mind you
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
                    // testing with only four words
                    //i++;
                    //if (i % 1337 == 0)
                    //{

                    //    std::cout << i << " ";
                    //    print(raw[rawIndex[wordOne]]);
                    //    std::cout << " " << wordOne << " ";
                    //    print(raw[rawIndex[wordTwo]]);
                    //    std::cout << " " << wordTwo << " ";
                    //    print(raw[rawIndex[wordThree]]);
                    //    std::cout << " " << wordThree << " ";
                    //    print(raw[rawIndex[wordFour]]);
                    //    std::cout << " " << wordFour << endl;
                    //}
                }
            }
        }
        // benchmarking with a subset of four-word combinations:
        // 99 seconds to do 30 on ascending logical operator order
        // 149 seconds on descending order
        // I think the first word tends to hog more common letters, so it's better at eliminating
        // word 3 in particular. I still think word 5 should be checked against word 4 first, but that's it.
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


    //vector<int> indices = { 0, 1, 2, 3, 4 };
    // this strategy was too complicated; I thought it could somehow save me from having a O(n^5) nested loop
    //while(i < 7663)
    //{
    //    while (matchLetter(pruned[indices[0]], pruned[indices[1]]))
    //    {
    //        // this line of code is going to appear a LOT...
    //        // (unless I refactor it)
    //        // but the ONLY character that can change is that 1, which can also be 2, 3, or 4.
    //        // but I have to copy the whole line, or add way more if statements elsewhere, 
    //        // or learn some more advanced technique, or make some variables global, if C++ even allows that.
    //        if (bump(indices, size, 1, i > 7660)) return 0;
    //    }
    //    if (indices[2] < indices[1] + 1)
    //        indices[2] = indices[1] + 1;
    //    while (
    //        matchLetter(pruned[indices[0]], pruned[indices[2]]) || 
    //        matchLetter(pruned[indices[1]], pruned[indices[2]]))
    //    {
    //        if (bump(indices, size, 2, i > 7660)) return 0;
    //    }
    //    if (i > 7660)
    //    {
    //        print(raw[rawIndex[indices[0]]]);
    //        std::cout << " " << indices[0] << " ";
    //        print(raw[rawIndex[indices[1]]]);
    //        std::cout << " " << indices[1] << " ";
    //        print(raw[rawIndex[indices[2]]]);
    //        std::cout << " " << indices[2] << endl;
    //    }
    //    if (bump(indices, size, 2, i > 7660)) return 0;
    //    i++;
    //}
    //print(pruned[indices[0]]);
    //print(pruned[indices[1]]);


    return pause();
}

#pragma region stillnotmain
// `string& a` means pass a pointer to a, which is much faster for long strings
// const forbids the function from modifying the originals. This allows them
// to functionally be copies without sacrificing performance. And you'll
// be able to see that they are const from the other side.
// `inline` means that the compiler injects the code directly into where the
// call goes, rather than doing a traditional function call... but, um,
// if I'm reading this right... the compiler will made functions inline
// automatically if it detects that it would be faster?
// and I'm really not sure about this part, but it looks like it will
// also IGNORE an inline if it would make it slower? So you really don't
// need to know about this?
// quote from the website:
// "In C++, optimization is a task delegated to the compiler, which is free to generate 
// any code for as long as the resulting behavior is the one specified by the code."
inline string concatenate(const string& a, const string& b)
{
    return a + b;
}

void syntax()
{
    std::cout << "Hello World!\n";
    // declare three ints a, b, and c. Initialize a to 2 and b to 1.
    int intA = 2, intB = 1, intC;
    int intD(42); // a space between the variable name and the parantheses is optional
    int intE{ 1337 }; // at least as far as ints are concerned, these syntaxes are equal.
    auto intF = intE; // C++ is old... but that doesn't mean it hasn't been updated
    decltype(intF) intG; // declare g as having the same type as f without initializing ... ngl, idk when you would use that
    std::cout << intA << intB << intD << intE;
    std::cout << '£';
    string strA = "foo";
    string strB("foo");
    string strC{ "foo" };
    auto doubleA = 1.5;
    auto doubleB = 1.5e50;
    auto doubleC = 1.6e-19;
    auto floatA = 1.5F;
    auto floatB = 1.5f;
    auto longA = 1.5L;
    auto longB = 1.5l;
    auto strD = "This forms " "one big ""string!";
    // there is a limit of 16 characters on how long the sequence at the beginning and end can be.
    string strE = R"something(now *I +can -use $symbols ^wherever &I \want, ::even )something ;and !nothing @will %break)something";
    auto strF = u"this string is encoded with UTF-16, I think";
    auto strG = U"this string is encoded with UTF-32, I think";
    auto strH = u8"this string is enocded with UTF-8, but surely it was going to do that anyway, right?";
    string strI = "this £ will render as ú. Unfortunate for the British, but excellent for performance.";

    // prints ú
    std::cout << '£';
    // prints -93
    // wait, what?
    std::cout << int('£');

    do {
        std::cout << "This will happen exactly once.";
    } while (false);

    std::cout << false; // prints 0
    std::cout << true; // prints 1

    // switch statements are limited to literals/constants! That's why they're faster!

    // behold, a valid program, in which two functions
    // contain references to EACH OTHER.
    //void odd(int x);
    //void even(int x);
    //
    //int main()
    //{
    //    int i;
    //    do {
    //        std::cout << "Please, enter number (0 to exit): ";
    //        cin >> i;
    //        odd(i);
    //    } while (i != 0);
    //    return 0;
    //}
    //
    //void odd(int x)
    //{
    //    if ((x % 2) != 0) std::cout << "It is odd.\n";
    //    else even(x);
    //}
    //
    //void even(int x)
    //{
    //    if ((x % 2) == 0) std::cout << "It is even.\n";
    //    else odd(x);
    //}

    int arrayA[5];
    int arrayB[5] = {1,2,3}; // ...0, 0
    int arrayC[5] = {}; // oops, all zeros!
    int arrayD[3]{4,5,6}; // the = is optional

    // example reading from a file code:
    // reading a text file
    //#include <iostream>
    //#include <fstream>
    //#include <string>
    //using namespace std;
    //
    //int main() {
    //    string line;
    //    ifstream myfile("example.txt");
    //    if (myfile.is_open())
    //    {
    //        while (getline(myfile, line))
    //        {
    //            std::cout << line << '\n';
    //        }
    //        myfile.close();
    //    }
    //
    //    else std::cout << "Unable to open file";
    //
    //    return 0;
    //}

    int edwardSnowden = 5;
    edwardSnowden;   // value of edwardSnowden
    &edwardSnowden;  // address of edwardSnowden
    *&edwardSnowden; // value of edwardSnowden

    // access forbidden memory!
    int a[1]{};
    for (int i = 1; i <= 10; i++)
        std::cout << a[i] << endl;

    // min is 0, max is 32767
    int rando = rand();
    // random number between 0 and 9, slight bias towards 0-7
    int practicalRand = rand() % 10;
    // random number between 0 and 9, serious bias towards 0-2767
    int biasRand = rand() % 10000;

    // Run program: Ctrl + F5 or Debug > Start Without Debugging menu
    // Debug program: F5 or Debug > Start Debugging menu

    // Tips for Getting Started: 
    //   1. Use the Solution Explorer window to add/manage files
    //   2. Use the Team Explorer window to connect to source control
    //   3. Use the Output window to see build output and other messages
    //   4. Use the Error List window to view errors
    //   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
    //   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
}
#pragma endregion stillnotmain
