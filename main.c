#include <stdio.h>

void displayMainMenu();
void showAvailableDoctors();
void showAvailableBeds(int beds[]);
void patientAdmit(int ids[], int ages[], int wards[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[], int *idCounter, int queueCounts[]); // Added missing semicolon here

int main() {
    int mainChoice = 0;
    int patientIds[10] = {0};
    int patientAges[10] = {0};
    int patientWards[10] = {0};
    int patientDays[10] = {0};
    char patientNames[10][30] = {{0}};
    char patientAdmitDates[10][15] = {{0}};
    int patientUrgency[10] = {0};
    int patientSpecialties[10] = {0};
    int bedOccupancy[5] = {0};
    int bookingCounts[5] = {0};
    int idCounter = 1;

    while (1) {
        displayMainMenu();
        scanf("%d", &mainChoice);

        switch (mainChoice) {
            case 1:
                // Calling your patientAdmit function here
                patientAdmit(patientIds, patientAges, patientWards, patientDays, patientNames, patientAdmitDates, patientUrgency, patientSpecialties, bedOccupancy, &idCounter, bookingCounts);
                break;
            case 2:
                printf("Doctor Channeling Selected\n");
                break;
            case 3:
                showAvailableBeds(bedOccupancy);
                break;
            case 4:
                showAvailableDoctors();
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
    return 0;
}

void displayMainMenu() {
    printf("\n--- Yasaro Hospital ---\n");
    printf("1. Patient Admit\n");
    printf("2. Doctor Channeling\n");
    printf("3. Available Beds\n");
    printf("4. Available Doctors\n");
    printf("Enter your choice: ");
}

void showAvailableBeds(int beds[]) {
    printf("\n--- Available Beds Status ---\n");
    printf("1. General Ward Status (0 = Free, 1 = Occupied): %d\n", beds[1]);
    printf("2. Paediatric Ward Status (0 = Free, 1 = Occupied): %d\n", beds[2]);
    printf("3. Surgical Ward Status (0 = Free, 1 = Occupied): %d\n", beds[3]);
    printf("4. ICU Status (0 = Free, 1 = Occupied): %d\n", beds[4]);
}

void showAvailableDoctors() {
    printf("\n--- Available Doctors ---\n");
    printf("1. General Practice (Available)\n");
    printf("2. Paediatrics (Available)\n");
    printf("3. Cardiology (Available)\n");
    printf("4. Neurology (Available)\n");
}

void patientAdmit(int ids[], int ages[], int wards[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[], int *idCounter, int queueCounts[]) {
    int admitChoice = 0;
    while (admitChoice != 4) {
        printf("\n--- Admit Menu ---\n");
        printf("1. New Admit\n");
        printf("2. Update\n");
        printf("3. Discharge\n");
        printf("4. Back\n");
        printf("Choice: ");
        scanf("%d", &admitChoice);
    }
}
