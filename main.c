#include <stdio.h>

void displayMainMenu();
void showAvailableDoctors();
void showAvailableBeds(int beds[]);
void patientAdmit(int ids[], int ages[], int wards[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[], int *idCounter, int queueCounts[]);
int patientIndex(int ids[], int searchId);
void registerNewPatient(int ids[], int ages[], int wards[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[], int *idCounter, int queueCounts[]);

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

        switch (admitChoice) {
            case 1:
                registerNewPatient(ids, ages, wards, days, names, admitDates, urgency, specialties, beds, idCounter, queueCounts);
                break;
            case 2:
                printf("Update feature coming soon.\n");
                break;
            case 3:
                printf("Discharge feature coming soon.\n");
                break;
            case 4:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
}

int patientIndex(int ids[], int searchId) {
    int foundIndex = -1;
    for (int i = 0; i < 10; i++) {
        if (ids[i] == searchId) {
            foundIndex = i;
            break;
        }
    }
    return foundIndex;
}

void registerNewPatient(int ids[], int ages[], int wards[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[], int *idCounter, int queueCounts[]) {
    // Moved variables here so the function can access and reset them properly
    int foundIndex = -1;
    int loopControl = 1;
    int isAdmitted = 0;

    for (int i = 0; i < 10 && loopControl; i++) {
        switch (ids[i] == 0) {
            case 1:
                foundIndex = i;
                loopControl = 0;
                break;
            case 0:
                break;
        }
    }

    switch (foundIndex != -1) {
        case 1:
            ids[foundIndex] = *idCounter;
            printf("Patient ID Assigned: %d\n", ids[foundIndex]);
            (*idCounter)++;

            printf("Enter Patient Name: ");
            scanf("%s", names[foundIndex]);

            printf("Enter Age: ");
            scanf("%d", &ages[foundIndex]);

            printf("Is Admitted to Ward? (1 = Yes, 0 = No): ");
            scanf("%d", &isAdmitted);

            switch (isAdmitted) {
                case 1:
                    printf("Enter Admit Date (Day number): ");
                    scanf("%s", admitDates[foundIndex]);

                    printf("Enter Ward ID (1-4): ");
                    scanf("%d", &wards[foundIndex]);

                    switch (wards[foundIndex] >= 1 && wards[foundIndex] <= 4) {
                        case 1:
                            beds[wards[foundIndex]] = 1;
                            break;
                    }
                    break;
            }

            printf("Select Specialty ID (1-4): ");
            scanf("%d", &specialties[foundIndex]);

            printf("Enter Emergency Level (1-3): ");
            scanf("%d", &urgency[foundIndex]);

            printf("Patient successfully registered!\n");
            break;

        case 0:
            printf("Database full!\n");
            break;
    }
}
