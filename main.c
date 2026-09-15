#include <stdio.h>

void displayMainMenu() ;


int main() {
    int mainChoice = 0;

    while (1) {
        displayMainMenu();
        scanf("%d", &mainChoice);

        switch (mainChoice) {
            case 1: printf("Patient Admit Menu Selected\n");
            break;
            case 2: printf("Doctor Channeling Selected\n");
            break;
            case 3: printf("Available Beds Selected\n");
            break;
            case 4: printf("Available Doctors Selected\n");
            break;
            default: printf("Invalid choice!\n");
            break;
        }
    }
    return 0;
}
void displayMainMenu() {
    printf(" Yasaro Hospital\n");
    printf("1. Patient Admit\n");
    printf("2. Doctor Channeling\n");
    printf("3. Available Beds\n");
    printf("4. Available Doctors\n");
    printf("Enter your choice: ");
}
