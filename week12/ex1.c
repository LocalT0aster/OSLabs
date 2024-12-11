// ex1
// Code by @LocalT0aster (Danil Nesterov d.nesterov@innopolis.university)
// Run ./ex1.sh to compile and launch
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdbool.h>

#define KEYBOARD_PATH "/dev/input/by-path/platform-i8042-serio-0-event-kbd"

// Global varibles
bool* keys; // An array that stores every key position.
struct input_event inputEv; // Latest input event
int keyboardFD; // Keyboard file descriptor

const char* shortcutMsg =
"Available Shortcuts:\n"
"P+E -> \"I passed the Exam!\"\n"
"C+A+P -> \"Get some cappuccino!\"\n"
"K+Y+S -> \"Keep Yourself Safe!\"\n"
"E+X -> Exit program\n";

/** Print input event information
 * @param event event to print
 * @param keycode key code
 */
void print_event(const char* event, int keycode) {
    printf("%s 0x%04x (%d)\n", event, keycode, keycode);
}

int main() {
    // Initialize variables
    keys = calloc(0x100, sizeof(bool));
    keyboardFD = open(KEYBOARD_PATH, O_RDONLY);
    // Handle file opening errors
    if (keyboardFD == -1) {
        perror("Cannot open device");
        exit(EXIT_FAILURE);
    }

    // Print available shortcuts
    printf(shortcutMsg);

    bool isRunning = true;
    while (isRunning) {
        // Read and handle read errors
        if (read(keyboardFD, &inputEv, sizeof(struct input_event)) <= 0) {
            perror("Error while reading from keyboard.");
            isRunning = false;
        }

        if (inputEv.type == EV_KEY) { // The keyboard input event type is EV_KEY
            if (inputEv.value == 1) {
                print_event("PRESSED", inputEv.code);
                keys[inputEv.code] = true; // Update keys[KEY] to PRESSED state
            } else if (inputEv.value == 2)
                print_event("REPEATED", inputEv.code);
            else {
                print_event("RELEASED", inputEv.code);
                keys[inputEv.code] = false; // Update keys[KEY] to RELEASED state
            }
            
            // Exit shortcut check
            if (keys[KEY_E] && keys[KEY_X])
                isRunning = false;

            // "I passed the Exam!" shortcut check
            if (keys[KEY_P] && keys[KEY_E])
                printf("I passed the Exam!\n");

            // "Get some cappuccino!" shortcut check
            if (keys[KEY_C] && keys[KEY_A] && keys[KEY_P])
                printf("Get some cappuccino!\n");

            // "Keep Yourself Safe!" shortcut check
            if (keys[KEY_K] && keys[KEY_Y] && keys[KEY_S])
                printf("Keep Yourself Safe!\n");
        }
    }
    // Close the file before exiting
    free(keys); // Free keys :D
    close(keyboardFD);
    return EXIT_SUCCESS;
}
