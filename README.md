# Finding five words for Wordle with no repeating letters

Using Wordle's dictionary, this program searches for sets of five words such that no letter appears twice. Before any of that happens, there is a completely unrelated menu that allows the user to enter information about their pets, but I only wrote any of that to practice using polymorphism.

## Instructions for Build and Use

Steps to build and/or run the software:

1. Clone the repository within an IDE
2. Use the IDE to build/run the code.

Alternatively, downloading Relearn_C_Plus_Plus.exe, five-letter-words.txt, and fraud.txt and putting them in the same folder should allow you to run the program more easily by simply running the .exe file, but I haven't tested this on a different device and it may only work on devices like mine, which has a processor with ARM architecture instead of x86.

## Instructions for using the software:

1. Run Relearn_C_Plus_Plus.exe, either with your IDE's debug tool or by running it directly, depending on how you downloaded it.
2. A terminal should open with a list of commands. In the main menu, the program will compare the first letter of the user's input against the letters in parantheses. Exit (e) or Quit (q) will close the program.
3. Assuming you are not interested in the pets thing, entering "Wordle" will run the main part of the program. You will be be asked whether you want to run the program for yourself, or just display the output. If you say "No", the program will load the original output from fraud.txt. If you say "Yes" it will load Wordle's dictionary from five-letter-words.txt and begin searching for combinations from scratch.

## Development Environment

To recreate the development environment, you need the following software and/or libraries with the specified versions:

* Visual Studio. (NOT Visual Studio Code). If I recall correctly, it should come with its own C++ compiler. Please note that this program makes use of Windows-only libraries for keeping track of elapsed time which should not work on Mac or Linux. If you're really that dedicated, it shouldn't be very hard to remove any lines of code that deal with time entirely.

## Useful Websites to Learn More

I found these websites useful in developing this software:

* [cplusplus.com's tutorial](https://cplusplus.com/doc/tutorial/)
* [geeksforgeeks](https://www.geeksforgeeks.org/cpp/convert-string-to-int-in-cpp/)
* [cppreference.com](https://en.cppreference.com/cpp/numeric/random/rand)
* [W3Schools](https://www.w3schools.com/cpp/cpp_classes.asp)

## Future Work

The following items I plan to fix, improve, and/or add to this project in the future:

* Sort the words in alphabetical order. The dictionary is alphabatized at the start, but once the letters within the words are sorted, it becomes possible to improve performance slightly be alphabetizing the main list a second time, because you can create indices of not just where words that start with A end, but words that contain an A at all.
* Review the code of people who have written faster versions of this. The first program to solve this program was written in Python by [Matt Parker](https://www.youtube.com/watch?v=_-AfhLQfb6w), which took his laptop a month to execute. Several people subsequently made [massive improvements](https://www.youtube.com/watch?v=c33AZBnRHks) to his original code, culminating in a [program written in C](https://github.com/stew675/standup5x5/) by Stew Forster and Landon Kryger which can run in less than a millisecond, allegedly currently running in 0.3 milliseconds on the right hardware. While my program is already about 200 times faster than Matt's, it is currently about 15 times slower than the second-slowest program mentioned in his video.
* I deliberately avoided rewatching Matt's videos during development to see how far I could get just with what I already knew and could come up with on my own. The largest optimization in Forster & Kryger's program that I failed to make appears to be one-hot encoding the letters in words as bits in 32-bit integers and using bitwise AND operations to make comparisons immensely faster. If I do decide to implement some of these optimizations, I plan to avoid looking at any actual code, so that I can come up with an implementation on my own, only using taking strategies from other people on a conceptual level.
