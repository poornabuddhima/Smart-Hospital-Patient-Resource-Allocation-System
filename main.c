#include <stdio.h>
#include <string.h>

void displayMainMenu();
void showAvailableDoctors();
void showAvailableBeds(int beds[]);
void patientAdmit(int ids[], int ages[], int wards[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[], int *idCounter, int queueCounts[]);
int patientIndex(int ids[], int searchId);
void registerNewPatient(int ids[], int ages[], int wards[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[], int *idCounter, int queueCounts[]);
void patientCosts(int ids[], int days[]);
float baseConsultationFee(int specialty);
float emergencySurcharge(int urgency, float baseFee);
float wardDailyRate(int wardId);
float wardCost(int isAdmitted, int wardId, int days);
float grossTotal(float baseFee, float surcharge, float wardCostVal);
float discount(int age, float grossTotalVal);
void dischargePatient(int wards[], int beds[], int index, int isAdmitted);
void printPatientBill(int ids[], int ages[], int wards[], int days[], char names[][30], int urgency[], int specialties[], int foundIndex);

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
                patientCosts(ids, days);
                break;
            case 3: {
                int searchId = 0;
                int foundIndex = -1;
                printf("Enter Patient ID to discharge: ");
                scanf("%d", &searchId);
                foundIndex = patientIndex(ids, searchId);

                if (foundIndex != -1) {
                    int admitDayNum = 0;
                    for (int i = 0; admitDates[foundIndex][i] != '\0'; i++) {
                        if (admitDates[foundIndex][i] >= '0' && admitDates[foundIndex][i] <= '9') {
                            admitDayNum = admitDayNum * 10 + (admitDates[foundIndex][i] - '0');
                        }
                    }

                    int dischargeDayNum = 0;
                    printf("Enter Discharge Date (Day number): ");
                    scanf("%d", &dischargeDayNum);

                    days[foundIndex] = dischargeDayNum - admitDayNum;
                    if (days[foundIndex] < 1) {
                        days[foundIndex] = 1;
                    }

                    // Automatically print the bill upon discharge
                    printPatientBill(ids, ages, wards, days, names, urgency, specialties, foundIndex);

                    dischargePatient(wards, beds, foundIndex, (wards[foundIndex] > 0 ? 1 : 0));
                    printf("Patient discharged successfully!\n");
                } else {
                    printf("Patient ID not found!\n");
                }
                break;
            }
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

void patientCosts(int ids[], int days[]) {
    int searchId = 0;
    int foundIndex = -1;
    float medCost = 0;
    float surgeryCost = 0;

    printf("Enter Patient ID to update: ");
    scanf("%d", &searchId);

    foundIndex = patientIndex(ids, searchId);

    switch (foundIndex != -1) {
        case 1:
            printf("Input Medicine Cost: ");
            scanf("%f", &medCost);
            printf("Input Surgery Cost: ");
            scanf("%f", &surgeryCost);
            printf("Costs updated successfully for patient index %d!\n", foundIndex);
            break;
        case 0:
            printf("Patient ID not found!\n");
            break;
    }
}

float baseConsultationFee(int specialty) {
    float fee = 1500.0;
    switch (specialty) {
        case 1: fee = 1500.0; break;
        case 2: fee = 2500.0; break;
        case 3: fee = 4500.0; break;
        case 4: fee = 5000.0; break;
    }
    return fee;
}

float emergencySurcharge(int urgency, float baseFee) {
    float surcharge = 0.0;
    switch (urgency) {
        case 2: surcharge = baseFee * 0.20; break;
        case 3: surcharge = baseFee * 0.50; break;
    }
    return surcharge;
}

float wardDailyRate(int wardId) {
    float rate = 0.0;
    switch (wardId) {
        case 1: rate = 3000.0; break;
        case 2: rate = 6000.0; break;
        case 3: rate = 12000.0; break;
        case 4: rate = 25000.0; break;
    }
    return rate;
}

float wardCost(int isAdmitted, int wardId, int days) {
    if (isAdmitted > 0) {
        return days * wardDailyRate(wardId);
    }
    return 0.0;
}

float grossTotal(float baseFee, float surcharge, float wardCostVal) {
    return baseFee + surcharge + wardCostVal;
}

float discount(int age, float grossTotalVal) {
    float discountAmount = 0.0;
    if (age < 5 || age > 65) {
        discountAmount = grossTotalVal * 0.15;
    }
    return discountAmount;
}

void dischargePatient(int wards[], int beds[], int index, int isAdmitted) {
    if (isAdmitted == 1 && wards[index] >= 1 && wards[index] <= 4) {
        beds[wards[index]] = 0;
    }
}

void printPatientBill(int ids[], int ages[], int wards[], int days[], char names[][30], int urgency[], int specialties[], int foundIndex) {
    float baseFee = baseConsultationFee(specialties[foundIndex]);
    float surcharge = emergencySurcharge(urgency[foundIndex], baseFee);
    int isAdmitted = (wards[foundIndex] > 0 ? 1 : 0);
    float wCost = wardCost(isAdmitted, wards[foundIndex], days[foundIndex]);
    float gross = grossTotal(baseFee, surcharge, wCost);
    float disc = discount(ages[foundIndex], gross);
    float finalPayable = gross - disc;

    char specName[30];
    switch (specialties[foundIndex]) {
        case 1: strcpy(specName, "General Practice");
         break;
        case 2: strcpy(specName, "Paediatrics");
         break;
        case 3: strcpy(specName, "Cardiology");
         break;
        case 4: strcpy(specName, "Neurology");
        break;
        default: strcpy(specName, "General");
         break;
    }

    char wardName[30];
    switch (wards[foundIndex]) {
        case 1: strcpy(wardName, "General Ward (Bed #01)");
        break;
        case 2: strcpy(wardName, "Paediatric Ward (Bed #01)");
         break;
        case 3: strcpy(wardName, "Surgical Ward (Bed #01)");
         break;
        case 4: strcpy(wardName, "ICU (Bed #01)");
        break;
        default: strcpy(wardName, "Not Admitted");
         break;
    }

    char urgencyText[30];
    int surchargePct = 0;
    if (urgency[foundIndex] == 2) {
        surchargePct = 20;
        strcpy(urgencyText, "Level 2 (Urgent)");
    } else if (urgency[foundIndex] == 3) {
        surchargePct = 50;
        strcpy(urgencyText, "Level 3 (Critical)");
    } else {
        surchargePct = 0;
        strcpy(urgencyText, "Level 1 (Routine)");
    }

    printf("\n====================================================\n");
    printf(" HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Patient ID   : PAT-%d\n", ids[foundIndex]);
    printf("Patient Name : %s\n", names[foundIndex]);
    if (ages[foundIndex] < 5 || ages[foundIndex] > 65) {
        printf("Age          : %d Years (15% Eligible)\n", ages[foundIndex]);
    } else {
        printf("Age          : %d Years (Not Eligible)\n", ages[foundIndex]);
    }
    printf("Specialty    : %s\n", specName);
    printf("Assigned Ward: %s\n", wardName);
    if (wards[foundIndex] > 0) {
        printf("Ward Stay    : %d Days\n", days[foundIndex]);
    }
    printf("Urgency Level: %s\n", urgencyText);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Base Consultation Fee : LKR %5\n", baseFee);
    printf("Emergency Surcharge   : LKR %5f (%d%%)\n", surcharge, surchargePct);
    if (wards[foundIndex] > 0) {
        printf("Ward Stay Cost (%d Days) : LKR %5f\n", days[foundIndex], wCost);
    } else {
        printf("Ward Stay Cost (0 Days)  : LKR   0.00\n");
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Gross Total Bill      : LKR %5f\n", gross);
    if (disc > 0) {
        printf("Age  Discount  : LKR -%5f (15%%)\n", disc);
    } else {
        printf("Age Subsidy Discount  : LKR    0.00 (0%%)\n");
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Final Payable Amount  : LKR %5f\n", finalPayable);
    printf("Estimated Waiting Time: 0.00 mins (Immediate Attention)\n");
    printf("====================================================\n");
}
