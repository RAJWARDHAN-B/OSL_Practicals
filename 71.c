#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 100

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

void to_uppercase(char* str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

// Sender program
void sender() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct msg_buffer message;

    while (1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(message.msg_text, MAX_TEXT, stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = 0;  // Remove newline

        if (strcmp(message.msg_text, "exit") == 0)
            break;

        printf("Enter message type (positive integer): ");
        scanf("%ld", &message.msg_type);
        getchar(); // consume newline

        msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0);
        printf("Message sent.\n");
    }
}

// Receiver program
void receiver() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct msg_buffer message;

    printf("Waiting for messages...\n");
    while (1) {
        if (msgrcv(msgid, &message, sizeof(message.msg_text), 0, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        if (strcmp(message.msg_text, "exit") == 0)
            break;

        to_uppercase(message.msg_text);
        printf("Received and converted message: %s\n", message.msg_text);
    }
    msgctl(msgid, IPC_RMID, NULL);
}

int main() {
    int choice;
    printf("1. Sender\n2. Receiver\nEnter choice: ");
    scanf("%d", &choice);
    getchar();  // consume newline

    if (choice == 1) sender();
    else if (choice == 2) receiver();
    else printf("Invalid choice.\n");

    return 0;
}
