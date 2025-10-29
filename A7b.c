// ================================================
// Server.cpp - Shared Memory Writer
// ================================================

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <cstdlib>
#define SHM_SIZE 1024
#define SHM_KEY 1234
using namespace std;

int main() {
    int shm_id;
    char *shm_ptr;

    // --- Create shared memory ---
    shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget failed");
        exit(1);
    }

    cout << "Server: Shared memory created with ID " << shm_id << "\n";

    // --- Attach shared memory ---
    shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // --- Write message to shared memory ---
    cout << "Server: Enter message to write: ";
    string msg;
    getline(cin, msg);
    strncpy(shm_ptr, msg.c_str(), SHM_SIZE - 1);
    shm_ptr[SHM_SIZE - 1] = '\0';

    cout << "Server: Message written.\n";

    // --- Detach shared memory ---
    shmdt(shm_ptr);

    return 0;
}


// ================================================
// Client.cpp - Shared Memory Reader
// ================================================

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <cstdlib>
#define SHM_SIZE 1024
#define SHM_KEY 1234
using namespace std;

int main() {
    int shm_id;
    char *shm_ptr;

    // --- Access existing shared memory ---
    shm_id = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shm_id < 0) {
        perror("shmget failed");
        exit(1);
    }

    // --- Attach shared memory ---
    shm_ptr = (char *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // --- Read message ---
    cout << "Client: Message from shared memory:\n" << shm_ptr << "\n";

    // --- Detach shared memory ---
    shmdt(shm_ptr);

    // Optional cleanup:
    // shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
