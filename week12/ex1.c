// ex1
#include <stdio.h>
#include <stdlib.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdbool.h>

/**
 * @brief Function to print event information
 *
 * @param event_type Type of event to print
 * @param code The key code
 */
void print_event(const char* event_type, int code) {
    printf("%s 0x%04x (%d)\n", event_type, code, code);
}

/**
 * @brief Program entry point function
 *
 * @return int exit code
 */
int main() {
    // Stores latest event data
    struct input_event ev;
    // Flag that prevents the program from exiting
    bool running = 1;
    // Array that stores every state of the keybooard keys. 1 is PRESSED 2 is RELEASED
    bool* keystate = calloc(256, sizeof(bool)); // P.S. The implementation is not memory optimal, but fast.
    // Keyboard file stream
    FILE* kbfile = fopen("/dev/input/by-path/platform-i8042-serio-0-event-kbd", "r");
    // Exit the program if we cannot open the file
    if (kbfile == NULL) {
        perror("Cannot open device");
        exit(EXIT_FAILURE);
    }

    // Print available shortcuts
    printf("Available Shortcuts:\n");
    printf("P+E -> 'I passed the Exam!'\n");
    printf("C+A+P -> 'Get some cappuccino!'\n");
    printf("LSHIFT+RSHIFT -> 'Turbo Boost!'\n");

    // Main program loop
    while (running) {
        // If the read from keybord returns an error, exit the program
        if (fread(&ev, sizeof(struct input_event), 1, kbfile) == 0) {
            perror("Error while reading from keyboard.");
            running = 0;
        }
        // If event type is EV_KEY type
        if (ev.type == EV_KEY) {
            // Event value switch
            switch (ev.value) {
            case 0: // Key Released
                print_event("RELEASED", ev.code);
                keystate[ev.code] = 0; // Change keystate[KEY] to RELEASED state
                break;
            case 1: // Key Pressed
                print_event("PRESSED", ev.code);
                keystate[ev.code] = 1; // Change keystate[KEY] to PRESSED state
                break;
            case 2: // Key Repeated
                print_event("REPEATED", ev.code);
                break;
            }
            // Check for the exit shortcut
            if (keystate[KEY_E] && keystate[KEY_X])
                running = 0;

            // Check for 'I passed the Exam!' shortcut
            if (keystate[KEY_P] && keystate[KEY_E])
                printf("I passed the Exam!\n");

            // Check for 'Get some cappuccino!' shortcut
            if (keystate[KEY_C] && keystate[KEY_A] && keystate[KEY_P])
                printf("Get some cappuccino!\n");

            // Check for 'Turbo Boost!' shortcut
            if (keystate[KEY_LEFTSHIFT] && keystate[KEY_RIGHTSHIFT])
                printf("Turbo boost!\n");
        }
    }
    // Close the file before exiting
    fclose(kbfile);
    return 0;
}
