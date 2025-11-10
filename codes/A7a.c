// ================================================
// Process1.cpp - Writer & Sender
// ================================================

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
using namespace std;

#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"

int main() {
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    char buffer[1024];

    cout << "Enter a sentence (type 'exit' to quit): ";
    cin.getline(buffer, sizeof(buffer));

    while (strcmp(buffer, "exit") != 0) {
        // --- Write to FIFO1 ---
        int fd1 = open(FIFO1, O_WRONLY);
        write(fd1, buffer, strlen(buffer) + 1);
        close(fd1);

        // --- Read processed result from FIFO2 ---
        int fd2 = open(FIFO2, O_RDONLY);
        read(fd2, buffer, sizeof(buffer));
        close(fd2);

        cout << "\nResult from Process2:\n" << buffer << endl;

        cout << "\nEnter a sentence (type 'exit' to quit): ";
        cin.getline(buffer, sizeof(buffer));
    }

    // --- Send exit message ---
    int fd1 = open(FIFO1, O_WRONLY);
    write(fd1, buffer, strlen(buffer) + 1);
    close(fd1);

    return 0;
}


// ================================================
// Process2.cpp - Reader & Counts Analyzer
// ================================================

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
using namespace std;

#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"

int main() {
    char buffer[1024];

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    while (true) {
        // --- Read message from FIFO1 ---
        int fd1 = open(FIFO1, O_RDONLY);
        read(fd1, buffer, sizeof(buffer));
        close(fd1);

        if (strcmp(buffer, "exit") == 0)
            break;

        string sentence(buffer);
        int charCount = sentence.size();
        int wordCount = 0, lineCount = 1;
        bool inWord = false;

        // --- Count characters, words, and lines ---
        for (char c : sentence) {
            if (c == ' ' || c == '\t' || c == '\n') {
                if (inWord) {
                    wordCount++;
                    inWord = false;
                }
                if (c == '\n')
                    lineCount++;
            } else {
                inWord = true;
            }
        }
        if (inWord)
            wordCount++;

        // --- Write results to a file ---
        ofstream out("output.txt");
        out << "Characters: " << charCount << endl;
        out << "Words: " << wordCount << endl;
        out << "Lines: " << lineCount << endl;
        out.close();

        // --- Read result back from file ---
        ifstream in("output.txt");
        string result((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        in.close();

        // --- Send results through FIFO2 ---
        int fd2 = open(FIFO2, O_WRONLY);
        write(fd2, result.c_str(), result.size() + 1);
        close(fd2);
    }

    return 0;
}
