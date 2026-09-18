#include <stdio.h>
#include <string.h>

void displayMainMenu();
void showAvailableDoctors(char* specNames[], float specFees[], int specTimes[], int specCaps[]);
void showAvailableBeds(int beds[4][20], char* wardNames[], int wardCapacities[]);
void patientAdmit(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[4][20], int *idCounter, char* wardNames[], int wardCapacities[], float specFees[], float wardRates[], char* specNames[]);
int patientIndex(int ids[], int searchId);
void registerNewPatient(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[4][20], int *idCounter, int wardCapacities[], char* wardNames[]);
void patientCosts(int ids[], int days[]);
float baseConsultationFee(int specialty, float specFees[]);
float emergencySurcharge(int urgency, float baseFee);
float wardDailyRate(int wardId, float wardRates[]);
float wardCost(int isAdmitted, int wardId, int days, float wardRates[]);
float grossTotal(float baseFee, float surcharge, float wardCostVal);
float discount(int age, float grossTotalVal);
void dischargePatient(int wards[], int bedsAssigned[], int beds[4][20], int index, int isAdmitted);
void printPatientBill(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], int urgency[], int specialties[], int foundIndex, float specFees[], float wardRates[], char* specNames[], char* wardNames[]);

int main() {
    int mainChoice = 0;
    int patientIds[10] = {0};
    int patientAges[10] = {0};
    int patientWards[10] = {0};
    int patientBeds[10] = {0};
    int patientDays[10] = {0};
    char patientNames[10][30] = {{0}};
    char patientAdmitDates[10][15] = {{0}};
    int patientUrgency[10] = {0};
    int patientSpecialties[10] = {0};

    // Lookup arrays placed inside main
    char* SPEC_NAMES[] = {"", "General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
    float SPEC_FEES[] = {0.0f, 1500.0f, 2500.0f, 4500.0f, 5000.0f};
    int SPEC_TIMES[] = {0, 15, 20, 30, 30};
    int SPEC_CAPS[] = {0, 30, 20, 12, 10};

    char* WARD_NAMES[] = {"", "General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"};
    float WARD_RATES[] = {0.0f, 3000.0f, 6000.0f, 12000.0f, 25000.0f};
    int WARD_CAPACITIES[] = {0, 20, 10, 10, 5};

    // 2D integer array for bed occupancy matrix [4 Wards][20 Max Beds]
    int bedOccupancy[4][20] = {0};
    int idCounter = 1;

    while (1) {
        displayMainMenu();
        scanf("%d", &mainChoice);

        switch (mainChoice) {
            case 1:
                patientAdmit(patientIds, patientAges, patientWards, patientBeds, patientDays, patientNames, patientAdmitDates, patientUrgency, patientSpecialties, bedOccupancy, &idCounter, WARD_NAMES, WARD_CAPACITIES, SPEC_FEES, WARD_RATES, SPEC_NAMES);
                break;
            case 2:
                printf("Doctor Channeling Selected\n");
                break;
            case 3:
                showAvailableBeds(bedOccupancy, WARD_NAMES, WARD_CAPACITIES);
                break;
            case 4:
                showAvailableDoctors(SPEC_NAMES, SPEC_FEES, SPEC_TIMES, SPEC_CAPS);
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

void showAvailableBeds(int beds[4][20], char* wardNames[], int wardCapacities[]) {
    printf("\n--- Available Beds Status Matrix (0 = Free, 1 = Occupied) ---\n");
    for (int i = 0; i < 4; i++) {
        int wardId = i + 1;
        printf("%d. %s (Capacity: %d):\n   [ ", wardId, wardNames[wardId], wardCapacities[wardId]);
        for (int j = 0; j < wardCapacities[wardId]; j++) {
            printf("%d ", beds[i][j]);
        }
        printf("]\n");
    }
}

void showAvailableDoctors(char* specNames[], float specFees[], int specTimes[], int specCaps[]) {
    printf("\n--- Available Doctors & Specialties ---\n");
    for (int i = 1; i <= 4; i++) {
        printf("%d. %s | Fee: LKR %.2f | Time: %d mins | Daily Cap: %d\n",
               i, specNames[i], specFees[i], specTimes[i], specCaps[i]);
    }
}

void patientAdmit(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[4][20], int *idCounter, char* wardNames[], int wardCapacities[], float specFees[], float wardRates[], char* specNames[]) {
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
                registerNewPatient(ids, ages, wards, bedsAssigned, days, names, admitDates, urgency, specialties, beds, idCounter, wardCapacities, wardNames);
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

                    printPatientBill(ids, ages, wards, bedsAssigned, days, names, urgency, specialties, foundIndex, specFees, wardRates, specNames, wardNames);
                    dischargePatient(wards, bedsAssigned, beds, foundIndex, (wards[foundIndex] > 0 ? 1 : 0));
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

void registerNewPatient(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char admitDates[][15], int urgency[], int specialties[], int beds[4][20], int *idCounter, int wardCapacities[], char* wardNames[]) {
    int foundIndex = -1;
    int loopControl = 1;
    int isAdmitted = 0;

    for (int i = 0; i < 10 && loopControl; i++) {
        if (ids[i] == 0) {
            foundIndex = i;
            loopControl = 0;
        }
    }

    if (foundIndex != -1) {
        ids[foundIndex] = *idCounter;
        printf("Patient ID Assigned: PAT-%d\n", ids[foundIndex]);
        (*idCounter)++;

        printf("Enter Patient Name: ");
        scanf("%s", names[foundIndex]);

        printf("Enter Age: ");
        scanf("%d", &ages[foundIndex]);

        printf("Is Admitted to Ward? (1 = Yes, 0 = No): ");
        scanf("%d", &isAdmitted);

        if (isAdmitted == 1) {
            printf("Enter Admit Date (Day number): ");
            scanf("%s", admitDates[foundIndex]);

            printf("Enter Ward ID (1-4): ");
            scanf("%d", &wards[foundIndex]);

            if (wards[foundIndex] >= 1 && wards[foundIndex] <= 4) {
                int wardIdx = wards[foundIndex] - 1;
                int bedFound = 0;
                for (int b = 0; b < wardCapacities[wards[foundIndex]]; b++) {
                    if (beds[wardIdx][b] == 0) {
                        beds[wardIdx][b] = 1;
                        bedsAssigned[foundIndex] = b + 1;
                        bedFound = 1;
                        printf("Bed #%02d assigned successfully in %s.\n", b + 1, wardNames[wards[foundIndex]]);
                        break;
                    }
                }
                if (!bedFound) {
                    printf("Warning: No available beds in this ward! Admission marked without bed.\n");
                    wards[foundIndex] = 0;
                }
            } else {
                printf("Invalid Ward ID!\n");
                wards[foundIndex] = 0;
            }
        }

        printf("Select Specialty ID (1-4): ");
        scanf("%d", &specialties[foundIndex]);

        printf("Enter Emergency Level (1-3): ");
        scanf("%d", &urgency[foundIndex]);

        printf("Patient successfully registered!\n");
    } else {
        printf("Database full!\n");
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

    if (foundIndex != -1) {
        printf("Input Medicine Cost: ");
        scanf("%f", &medCost);
        printf("Input Surgery Cost: ");
        scanf("%f", &surgeryCost);
        printf("Costs updated successfully for patient index %d!\n", foundIndex);
    } else {
        printf("Patient ID not found!\n");
    }
}

float baseConsultationFee(int specialty, float specFees[]) {
    if (specialty >= 1 && specialty <= 4) {
        return specFees[specialty];
    }
    return 1500.0f;
}

float emergencySurcharge(int urgency, float baseFee) {
    float surcharge = 0.0f;
    switch (urgency) {
        case 2: surcharge = baseFee * 0.20f; break;
        case 3: surcharge = baseFee * 0.50f; break;
    }
    return surcharge;
}

float wardDailyRate(int wardId, float wardRates[]) {
    if (wardId >= 1 && wardId <= 4) {
        return wardRates[wardId];
    }
    return 0.0f;
}

float wardCost(int isAdmitted, int wardId, int days, float wardRates[]) {
    if (isAdmitted > 0) {
        return days * wardDailyRate(wardId, wardRates);
    }
    return 0.0f;
}

float grossTotal(float baseFee, float surcharge, float wardCostVal) {
    return baseFee + surcharge + wardCostVal;
}

float discount(int age, float grossTotalVal) {
    float discountAmount = 0.0f;
    if (age < 5 || age > 65) {
        discountAmount = grossTotalVal * 0.15f;
    }
    return discountAmount;
}

void dischargePatient(int wards[], int bedsAssigned[], int beds[4][20], int index, int isAdmitted) {
    if (isAdmitted == 1 && wards[index] >= 1 && wards[index] <= 4) {
        int wardIdx = wards[index] - 1;
        int bedNum = bedsAssigned[index] - 1;
        if (bedNum >= 0 && bedNum < 20) {
            beds[wardIdx][bedNum] = 0;
        }
    }
}

void printPatientBill(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], int urgency[], int specialties[], int foundIndex, float specFees[], float wardRates[], char* specNames[], char* wardNames[]) {
    float baseFee = baseConsultationFee(specialties[foundIndex], specFees);
    float surcharge = emergencySurcharge(urgency[foundIndex], baseFee);
    int isAdmitted = (wards[foundIndex] > 0 ? 1 : 0);
    float wCost = wardCost(isAdmitted, wards[foundIndex], days[foundIndex], wardRates);
    float gross = grossTotal(baseFee, surcharge, wCost);
    float disc = discount(ages[foundIndex], gross);
    float finalPayable = gross - disc;

    char specName[30];
    if (specialties[foundIndex] >= 1 && specialties[foundIndex] <= 4) {
        strcpy(specName, specNames[specialties[foundIndex]]);
    } else {
        strcpy(specName, "General Practice");
    }

    char wardName[50] = "Not Admitted";
    if (wards[foundIndex] >= 1 && wards[foundIndex] <= 4) {
        sprintf(wardName, "%s (Bed #%02d)", wardNames[wards[foundIndex]], bedsAssigned[foundIndex]);
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
        printf("Age          : %d Years (15%% Subsidy Eligible)\n", ages[foundIndex]);
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
    printf("Base Consultation Fee : LKR %10.2f\n", baseFee);
    printf("Emergency Surcharge   : LKR %10.2f (%d%%)\n", surcharge, surchargePct);
    if (wards[foundIndex] > 0) {
        printf("Ward Stay Cost (%d Days) : LKR %10.2f\n", days[foundIndex], wCost);
    } else {
        printf("Ward Stay Cost (0 Days)  : LKR   0.00\n");
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Gross Total Bill      : LKR %10.2f\n", gross);
    if (disc > 0) {
        printf("Age Subsidy Discount  : LKR -%10.2f (15%%)\n", disc);
    } else {
        printf("Age Subsidy Discount  : LKR    0.00 (0%%)\n");
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Final Payable Amount  : LKR %10.2f\n", finalPayable);
    printf("Estimated Waiting Time: 0.00 mins (Immediate Attention)\n");
    printf("====================================================\n");
}
