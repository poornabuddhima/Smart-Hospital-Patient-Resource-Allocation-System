#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 45

void displayMainMenu();
void showAvailableDoctors(char* specialtyNames[], float specialtyFees[], int specialtyTimes[], int specialtyCaps[]);
void showAvailableBeds(int beds[4][20], char* wardNames[], int wardCapacities[]);
int patientIndex(int patientId[], int searchId);
void dischargePatient(int wards[], int bedsAssigned[], int beds[4][20], int index, int isAdmitted, int wardCapacities[]);
void patientCosts(int ids[], int days[], char history[][50], float medCosts[], float surgeryCosts[]);
void registerNewPatient(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char phones[][15], char history[][50], char admitDates[][25], int urgency[], int specialties[], float medCosts[], float surgeryCosts[], int beds[4][20], int *idCounter, int wardCapacities[], char* wardNames[], int *totalRegistered, int urgencyStats[]);
void patientAdmit(int patientId[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char phones[][15], char history[][50], char admitDates[][25], char dischargeDates[][25], int urgency[], int specialties[], float medCosts[], float surgeryCosts[], int beds[4][20], int *idCounter, char* wardNames[], int wardCapacities[], float specFees[], float wardRates[], char* specNames[], int *totalRegistered, int urgencyStats[], float *totalRevenue, float *totalDiscounts, char highestName[], float *highestBill);
float baseConsultationFee(int specialty, float specFees[]);
float emergencySurcharge(int urgency, float baseFee);
float wardDailyRate(int wardId, float wardRates[]);
float wardCost(int isAdmitted, int wardId, int days, float wardRates[]);
float grossTotal(float baseFee, float surcharge, float wardCostVal, float medCostVal, float surgeryCostVal);
float discount(int age, float grossTotalVal);
void printPatientBill(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char phones[][15], char history[][50], char admitDates[][25], char dischargeDates[][25], int urgency[], int specialties[], float medCosts[], float surgeryCosts[], int recordIdx, float specFees[], float wardRates[], char* specNames[], char* wardNames[], float *totalRevenue, float *totalDiscounts, char highestName[], float *highestBill);
void doctorChanneling(char* specNames[], float specFees[], int specTimes[], int specCaps[], int specialtyNumbers[]);
void performanceReports(int totalRegistered, int urgencyStats[], float totalRevenue, float totalDiscounts, int beds[4][20], int wardCapacities[], char* wardNames[], char highestName[], float highestBill);
int extractDay(char* dateStr);
int dateToDays(char* dateStr);
void saveBedStatus(int beds[4][20], int wardCapacities[]);
void loadBedStatus(int beds[4][20], int wardCapacities[]);
void appendPatientRecord(int id, char* name, char* admitDate, char* dischargeDate, float finalPayable);

int main() {
    int mainCodeChoice = 0;

    int patientId[MAX_PATIENTS] = {0};
    int patientAges[MAX_PATIENTS] = {0};
    int patientWards[MAX_PATIENTS] = {0};
    int patientBeds[MAX_PATIENTS] = {0};
    int patientDays[MAX_PATIENTS] = {0};
    char patientNames[MAX_PATIENTS][30] = { {0} };
    char patientPhones[MAX_PATIENTS][15] = { {0} };
    char patientHistory[MAX_PATIENTS][50] = { {0} };
    char patientAdmitDates[MAX_PATIENTS][25] = { {0} };
    char patientDischargeDates[MAX_PATIENTS][25] = { {0} };
    int patientUrgency[MAX_PATIENTS] = {0};
    int patientSpecs[MAX_PATIENTS] = {0};
    float patientMedicalCost[MAX_PATIENTS] = {0};
    float patientSurgeryCost[MAX_PATIENTS] = {0};

    // Hospital Configuration Constants
    char* SPEC_NAMES[] = {"", "General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
    float SPEC_FEES[] = {0, 1500, 2500, 4500, 5000};
    int SPEC_TIMES[] = {0, 15, 20, 30, 30};
    int SPEC_CAPS[] = {0, 30, 20, 12, 10};

    char* WARD_NAMES[] = {"", "General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"};
    float WARD_RATES[] = {0, 3000, 6000, 12000, 25000};
    int WARD_CAPACITIES[] = {0, 20, 10, 10, 5};

    int specialtyNumbers[5] = {0};
    int bedOccupancy[4][20] = {0};
    int idCounter = 1;


    loadBedStatus(bedOccupancy, WARD_CAPACITIES);


    int totalRegisteredPatients = 0;
    int urgencyStats[4] = {0};
    float totalRevenueEarned = 0.0f;
    float totalDiscountsGranted = 0.0f;
    char highestPayingName[30] = "None";
    float highestPatientBill = 0.0f;

    // Main Control
    while (1) {
        displayMainMenu();
        scanf("%d", &mainCodeChoice);

        switch (mainCodeChoice) {
            case 1:
                patientAdmit(patientId, patientAges, patientWards, patientBeds, patientDays, patientNames, patientPhones, patientHistory, patientAdmitDates, patientDischargeDates, patientUrgency, patientSpecs, patientMedicalCost, patientSurgeryCost, bedOccupancy, &idCounter, WARD_NAMES, WARD_CAPACITIES, SPEC_FEES, WARD_RATES, SPEC_NAMES, &totalRegisteredPatients, urgencyStats, &totalRevenueEarned, &totalDiscountsGranted, highestPayingName, &highestPatientBill);
                break;
            case 2:
                doctorChanneling(SPEC_NAMES, SPEC_FEES, SPEC_TIMES, SPEC_CAPS, specialtyNumbers);
                break;
            case 3:
                showAvailableBeds(bedOccupancy, WARD_NAMES, WARD_CAPACITIES);
                break;
            case 4:
                showAvailableDoctors(SPEC_NAMES, SPEC_FEES, SPEC_TIMES, SPEC_CAPS);
                break;
            case 5:
                performanceReports(totalRegisteredPatients, urgencyStats, totalRevenueEarned, totalDiscountsGranted, bedOccupancy, WARD_CAPACITIES, WARD_NAMES, highestPayingName, highestPatientBill);
                break;
            default:
                printf("Wrong choice! Please select between 1 and 5.\n");
                break;
        }
    }
    return 0;
}

// Display Main Menu
void displayMainMenu() {
    printf("==================================================== \n");
    printf(" Yasaro Hospital Management System\n");
    printf("==================================================== \n");
    printf("1. Patient Admit \n");
    printf("2. Doctor Channeling\n");
    printf("3. Available Beds\n");
    printf("4. Available Doctors\n");
    printf("5. Performance Reports & Analytics\n");
    printf("Enter your choice: ");
}

int extractDay(char* dateStr) {
    int day = 0;
    int foundDigit = 0;
    for (int i = 0; dateStr[i] != '\0'; i++) {
        if (dateStr[i] >= '0' && dateStr[i] <= '9') {
            day = day * 10 + (dateStr[i] - '0');
            foundDigit = 1;
        } else if (foundDigit) {
            break;
        }
    }
    return day > 0 ? day : 1;
}

int dateToDays(char* dateStr) {
    int year = 0, month = 0, day = 0;
    sscanf(dateStr, "%d-%d-%d", &year, &month, &day);
    int daysInMonths[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int totalDays = day;
    for (int m = 1; m < month && m <= 12; m++) {
        totalDays += daysInMonths[m];
    }
    return totalDays;
}

// Display Current Bed Available
void showAvailableBeds(int beds[4][20], char* wardNames[], int wardCapacities[]) {
    printf("\n--- Available Beds ---\n");
    for (int i = 0; i < 4; i++) {
        int wardId = i + 1;
        printf("%d. %s (Capacity: %d):\n     ", wardId, wardNames[wardId], wardCapacities[wardId]);
        for (int j = 0; j < wardCapacities[wardId]; j++) {
            printf("%d ", beds[i][j]);
        }
        printf("\n");
    }
}

// Display list of medical specialties, consultation fees, and caps
void showAvailableDoctors(char* specialtyNames[], float specialtyFees[], int specialtyTimes[], int specialtyCaps[]) {
    printf("\n--- Available Doctors & Specialties ---\n");
    for (int i = 1; i <= 4; i++) {
        printf("%d. %s | Fee: LKR %.2f | Time: %d mins | Daily Cap: %d\n",
               i, specialtyNames[i], specialtyFees[i], specialtyTimes[i], specialtyCaps[i]);
    }
}

// Patient ID
int patientIndex(int patientId[], int searchId) {
    int foundIndex = -1;
    for (int i = 0; i < MAX_PATIENTS; i++) {
        if (patientId[i] == searchId) {
            foundIndex = i;
            break;
        }
    }
    return foundIndex;
}

// Patient Admit
void patientAdmit(int patientId[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char phones[][15], char history[][50], char admitDates[][25], char dischargeDates[][25], int urgency[], int specialties[], float medCosts[], float surgeryCosts[], int beds[4][20], int *idCounter, char* wardNames[], int wardCapacities[], float specFees[], float wardRates[], char* specNames[], int *totalRegistered, int urgencyStats[], float *totalRevenue, float *totalDiscounts, char highestName[], float *highestBill) {
    int admitChoice = 0;
    while (admitChoice != 4) {
        printf("\n--- Admit Patient Menu ---\n");
        printf("1. New Admit Patient\n");
        printf("2. Update Admitted Patient \n");
        printf("3. Discharge & Print Patient Bill\n");
        printf("4. Return to Main Menu\n");
        printf("Choice: ");
        scanf("%d", &admitChoice);

        switch (admitChoice) {
            case 1:
                registerNewPatient(patientId, ages, wards, bedsAssigned, days, names, phones, history, admitDates, urgency, specialties, medCosts, surgeryCosts, beds, idCounter, wardCapacities, wardNames, totalRegistered, urgencyStats);
                break;
            case 2:
                patientCosts(patientId, days, history, medCosts, surgeryCosts);
                break;
            case 3: {
                int searchId = 0;
                int foundIndex = -1;
                printf("Enter Patient ID to discharge: ");
                scanf("%d", &searchId);
                foundIndex = patientIndex(patientId, searchId);

                if (foundIndex != -1) {
                    char dischargeDateStr[25];
                    printf("Enter Discharge Date (e.g., 2026-06-15): ");
                    scanf("%s", dischargeDateStr);
                    strcpy(dischargeDates[foundIndex], dischargeDateStr);

                    // Fixed calculation using dateToDays to safely handle stays across month boundaries
                    int dischargeTotalDays = dateToDays(dischargeDates[foundIndex]);
                    int admitTotalDays = dateToDays(admitDates[foundIndex]);

                    days[foundIndex] = dischargeTotalDays - admitTotalDays;
                    if (days[foundIndex] < 1) {
                        days[foundIndex] = 1;
                    }

                    // Print final bill, update analytics and permanent file records
                    printPatientBill(patientId, ages, wards, bedsAssigned, days, names, phones, history, admitDates, dischargeDates, urgency, specialties, medCosts, surgeryCosts, foundIndex, specFees, wardRates, specNames, wardNames, totalRevenue, totalDiscounts, highestName, highestBill);

                    int adm_check = (wards[foundIndex] > 0) ? 1 : 0;
                    dischargePatient(wards, bedsAssigned, beds, foundIndex, adm_check, wardCapacities);

                    // Clear patient slot after discharge
                    patientId[foundIndex] = 0;
                    wards[foundIndex] = 0;
                    days[foundIndex] = 0;

                    printf("Patient discharged successfully!\n");
                } else {
                    printf("NO ID found!\n");
                }
                break;
            }
            case 4:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Wrong choice!\n");
                break;
        }
    }
}

// Register a New Patient
void registerNewPatient(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char phones[][15], char history[][50], char admitDates[][25], int urgency[], int specialties[], float medCosts[], float surgeryCosts[], int beds[4][20], int *idCounter, int wardCapacities[], char* wardNames[], int *totalRegistered, int urgencyStats[]) {
    int foundIndex = -1;
    int run = 1;
    int isAdmitted = 0;

    for (int i = 0; i < MAX_PATIENTS && run; i++) {
        if (ids[i] == 0) {
            foundIndex = i;
            run = 0;
        }
    }

    if (foundIndex != -1) {
        printf("\n--- New Patient Admit ---\n");

        printf("Enter Patient Name: ");
        scanf("%s", names[foundIndex]);

        printf("Enter Age: ");
        scanf("%d", &ages[foundIndex]);

        printf("Enter Phone Number: ");
        scanf("%s", phones[foundIndex]);

        printf("Is Admitted to Ward? (1 = Yes, 0 = No): ");
        scanf("%d", &isAdmitted);

        ids[foundIndex] = *idCounter;
        printf("Patient ID : PAT-%d\n", ids[foundIndex]);
        (*idCounter)++;

        if (isAdmitted == 1) {
            printf("Enter Ward ID (1: General, 2: Paediatric, 3: Surgical, 4: ICU): ");
            scanf("%d", &wards[foundIndex]);

            if (wards[foundIndex] >= 1 && wards[foundIndex] <= 4) {
                int wardIdx = wards[foundIndex] - 1;
                int bedFound = 0;
                // Search for an available bed in the selected ward
                for (int b = 0; b < wardCapacities[wards[foundIndex]]; b++) {
                    if (beds[wardIdx][b] == 0) {
                        beds[wardIdx][b] = 1;
                        bedsAssigned[foundIndex] = b + 1;
                        bedFound = 1;
                        printf("Bed Assigned: %d\n", bedsAssigned[foundIndex]);
                        break;
                    }
                }

                if (!bedFound) {
                    printf("No beds available!\n");
                    wards[foundIndex] = 0;
                } else {
                    // Automatically save bed status to file
                    saveBedStatus(beds, wardCapacities);
                }
            } else {
                printf("Invalid Ward ID\n");
                wards[foundIndex] = 0;
            }
        } else {
            wards[foundIndex] = 0;
        }

        printf("Enter Patient Medical History: ");
        scanf(" %[^\n]", history[foundIndex]);

        printf("Enter Triage Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
        scanf("%d", &urgency[foundIndex]);

        // Update statistics for analytics
        (*totalRegistered)++;
        if (urgency[foundIndex] >= 1 && urgency[foundIndex] <= 3) {
            urgencyStats[urgency[foundIndex]]++;
        } else {
            urgencyStats[1]++;
        }

        printf("Select Specialty ID (1: OPD, 2: Paediatrics, 3: Cardiology, 4: Neurology): ");
        scanf("%d", &specialties[foundIndex]);

        printf("Enter Admit Date (e.g., 2026-06-01): ");
        scanf("%s", admitDates[foundIndex]);

        days[foundIndex] = 1;
        medCosts[foundIndex] = 0;
        surgeryCosts[foundIndex] = 0;

        printf("Patient admitted successfully!\n");
    } else {
        printf("Hospital full! Maximum patient capacity reached.\n");
    }
}

// Patient Discharge
void dischargePatient(int wards[], int bedsAssigned[], int beds[4][20], int index, int isAdmitted, int wardCapacities[]) {
    if (isAdmitted == 1 && wards[index] >= 1 && wards[index] <= 4) {
        int wardIdx = wards[index] - 1;
        int bedNum = bedsAssigned[index] - 1;
        if (bedNum >= 0 && bedNum < 20) {
            beds[wardIdx][bedNum] = 0;
            // Update and save bed statuses
            saveBedStatus(beds, wardCapacities);
        }
    }
}

// Update medical costs and patient history for admitted patients
void patientCosts(int ids[], int days[], char history[][50], float medCosts[], float surgeryCosts[]) {
    int searchId = 0;
    int foundIndex = -1;

    printf("Enter Patient ID to update records: ");
    scanf("%d", &searchId);

    foundIndex = patientIndex(ids, searchId);

    if (foundIndex != -1) {
        printf("Updating Records for Patient PAT-%d ---\n", ids[foundIndex]);

        printf("Enter updated Days : ");
        scanf("%d", &days[foundIndex]);

        printf("Medicine Cost: ");
        scanf("%f", &medCosts[foundIndex]);

        printf("Surgery Cost: ");
        scanf("%f", &surgeryCosts[foundIndex]);

        printf("Update Medical History: ");
        scanf(" %[^\n]", history[foundIndex]);

        printf("Patient record updated\n");
    } else {
        printf("NO Patient\n");
    }
}

// Calculate base consultation fee based on selected specialty
float baseConsultationFee(int specialty, float specFees[]) {
    if (specialty >= 1 && specialty <= 4) {
        return specFees[specialty];
    }
    return 1500;
}

// Calculate emergency surcharge based on triage level
float emergencySurcharge(int urgency, float baseFee) {
    float surcharge = 0;
    if (urgency == 2) {
        surcharge = baseFee * 0.20;
    } else if (urgency == 3) {
        surcharge = baseFee * 0.50;
    }
    return surcharge;
}

// Get daily rate of the ward
float wardDailyRate(int wardId, float wardRates[]) {
    if (wardId >= 1 && wardId <= 4) {
        return wardRates[wardId];
    }
    return 0;
}

// Calculate total ward stay cost
float wardCost(int isAdmitted, int wardId, int days, float wardRates[]) {
    if (isAdmitted > 0) {
        return days * wardDailyRate(wardId, wardRates);
    }
    return 0;
}

// Calculate gross total before discounts
float grossTotal(float baseFee, float surcharge, float wardCostVal, float medCostVal, float surgeryCostVal) {
    return baseFee + surcharge + wardCostVal + medCostVal + surgeryCostVal;
}

// Calculate age-based subsidy discount (15% for age < 5 or > 65)
float discount(int age, float grossTotalVal) {
    float discountAmount = 0;
    if (age < 5 || age > 65) {
        discountAmount = grossTotalVal * 0.15f;
    }
    return discountAmount;
}

// Print  patient bill
void printPatientBill(int ids[], int ages[], int wards[], int bedsAssigned[], int days[], char names[][30], char phones[][15], char history[][50], char admitDates[][25], char dischargeDates[][25], int urgency[], int specialties[], float medCosts[], float surgeryCosts[], int recordIdx, float specFees[], float wardRates[], char* specNames[], char* wardNames[], float *totalRevenue, float *totalDiscounts, char highestName[], float *highestBill) {

    float baseFee = baseConsultationFee(specialties[recordIdx], specFees);
    float surcharge = emergencySurcharge(urgency[recordIdx], baseFee);

    int isAdmitted = (wards[recordIdx] > 0) ? 1 : 0;

    float wCost = wardCost(isAdmitted, wards[recordIdx], days[recordIdx], wardRates);
    float gross = grossTotal(baseFee, surcharge, wCost, medCosts[recordIdx], surgeryCosts[recordIdx]);
    float disc = discount(ages[recordIdx], gross);
    float finalPayable = gross - disc;


    *totalRevenue += finalPayable;
    *totalDiscounts += disc;
    if (finalPayable > *highestBill) {
        *highestBill = finalPayable;
        strcpy(highestName, names[recordIdx]);
    }

    appendPatientRecord(ids[recordIdx], names[recordIdx], admitDates[recordIdx], dischargeDates[recordIdx], finalPayable);

    char specName[30];
    if (specialties[recordIdx] >= 1 && specialties[recordIdx] <= 4) {
        strcpy(specName, specNames[specialties[recordIdx]]);
    } else {
        strcpy(specName, "General Practice");
    }

    char wardName[50] = "Not Admitted";
    if (wards[recordIdx] >= 1 && wards[recordIdx] <= 4) {
        sprintf(wardName, "%s (Bed %d)", wardNames[wards[recordIdx]], bedsAssigned[recordIdx]);
    }

    char urgencyText[30];
    int surchargePct = 0;
    if (urgency[recordIdx] == 2) {
        surchargePct = 20;
        strcpy(urgencyText, "Level 2 (Urgent)");
    } else if (urgency[recordIdx] == 3) {
        surchargePct = 50;
        strcpy(urgencyText, "Level 3 (Critical)");
    } else {
        surchargePct = 0;
        strcpy(urgencyText, "Level 1 (Routine)");
    }

    printf("==================================================== \n");
    printf(" YASARO HOSPITAL BILL\n");
    printf("----------------------------------------------------\n");
    printf(" ID          : PAT-%d\n", ids[recordIdx]);
    printf(" Name        : %s\n", names[recordIdx]);
    if (ages[recordIdx] < 5 || ages[recordIdx] > 65) {
        printf("Age         : %d Years (15%% Subsidy Eligible)\n", ages[recordIdx]);
    } else {
        printf("Age         : %d Years (Not Eligible)\n", ages[recordIdx]);
    }
    printf("Specialty    : %s\n", specName);
    printf("Assigned Ward: %s\n", wardName);
    printf("Admit Date   : %s\n", admitDates[recordIdx]);
    printf("Discharge Date: %s\n", dischargeDates[recordIdx]);
    if (wards[recordIdx] > 0) {
        printf("Ward Stay    : %d Days\n", days[recordIdx]);
    }
    printf("Urgency Level: %s\n", urgencyText);
    printf("----------------------------------------------------\n");
    printf("Base Consultation Fee : LKR %.2f\n", baseFee);
    printf("Emergency Surcharge   : LKR %.2f (%d%%)\n", surcharge, surchargePct);
    if (wards[recordIdx] > 0) {
        printf("Ward Stay Cost (%d Days) : LKR %.2f\n", days[recordIdx], wCost);
    } else {
        printf("Ward Stay Cost (0 Days)  : LKR    0.00\n");
    }
    printf("Medicine Cost         : LKR %.2f\n", medCosts[recordIdx]);
    printf("Surgery Cost          : LKR %.2f\n", surgeryCosts[recordIdx]);
    printf("----------------------------------------------------\n");
    printf("Gross Total Bill      : LKR %.2f\n", gross);
    if (disc > 0) {
        printf("Age Subsidy Discount  : LKR -%.2f (15%%)\n", disc);
    } else {
        printf("Age Subsidy Discount  : LKR        0.00 \n");
    }
    printf("----------------------------------------------------\n");
    printf("Final Payable Amount  : LKR %.2f\n", finalPayable);
    printf("==================================================== \n");
}

//  Doctor Channeling and Appointment
void doctorChanneling(char* specNames[], float specFees[], int specTimes[], int specCaps[], int specialtyNumbers[]) {
    int specialtyId = 0;
    showAvailableDoctors(specNames, specFees, specTimes, specCaps);
    printf("Select Consultant Specialty ID (1-4): ");
    scanf("%d", &specialtyId);

    if (specialtyId < 1 || specialtyId > 4) {
        printf("Invalid Specialty ID!\n");
        return;
    }

    if (specialtyNumbers[specialtyId] >= specCaps[specialtyId]) {
        printf("Booking Full! Daily capacity for %s reached.\n", specNames[specialtyId]);
        return;
    }

    char name[30];
    int age = 0;
    char phone[15];

    printf("Enter Patient Name: ");
    scanf("%s", name);
    printf("Enter Age: ");
    scanf("%d", &age);
    printf("Enter Phone Number: ");
    scanf("%s", phone);

    specialtyNumbers[specialtyId]++;
    int queueNumber = specialtyNumbers[specialtyId];

    int startMinutes = 16 * 60; // 4:00 PM start time
    int avgLateTime = 15;
    int consultationDuration = specTimes[specialtyId];

    int totalMinutes = startMinutes + avgLateTime + ((queueNumber - 1) * consultationDuration);
    int apptHour = (totalMinutes / 60) % 24;
    int apptMin = totalMinutes % 60;

    int displayHour;
    if (apptHour > 12) {
        displayHour = apptHour - 12;
    } else if (apptHour == 0) {
        displayHour = 12;
    } else {
        displayHour = apptHour;
    }

    char* timePeriod;
    if (apptHour >= 12) {
        timePeriod = "PM";
    } else {
        timePeriod = "AM";
    }

    printf("=================================================== \n");
    printf(" DOCTOR CHANNELING APPOINTMENT TICKET\n");
    printf("----------------------------------------------------\n");
    printf("Specialty    : %s\n", specNames[specialtyId]);
    printf("Patient Name : %s\n", name);
    printf("Age & Phone  : %d Years | %s\n", age, phone);
    printf("Booking Order: %02d\n", queueNumber);
    printf("Consultation Start : 04:00 PM\n");
    printf("Average Late Time  : %d mins\n", avgLateTime);
    printf("Estimated Arrival  : %02d:%02d %s\n", displayHour, apptMin, timePeriod);
    printf("Consultation Fee   : LKR %.2f\n", specFees[specialtyId]);
    printf("==================================================== \n");
}

// Generate Performance Reports & Analytics
void performanceReports(int totalRegistered, int urgencyStats[], float totalRevenue, float totalDiscounts, int beds[4][20], int wardCapacities[], char* wardNames[], char highestName[], float highestBill) {
    printf("==================================================== \n");
    printf(" PERFORMANCE REPORTS & ANALYTICS\n");
    printf("==================================================== \n");

    // 1. Patient Urgency Breakdown
    printf("1. Patient Urgency Breakdown:\n");
    printf("   - Total Registered Patients : %d\n", totalRegistered);
    printf("   - Level 1 (Routine)         : %d\n", urgencyStats[1]);
    printf("   - Level 2 (Urgent)          : %d\n", urgencyStats[2]);
    printf("   - Level 3 (Critical)        : %d\n", urgencyStats[3]);
    printf("----------------------------------------------------\n");

    // 2. Financial Summary
    printf("2. Financial Summary:\n");
    printf("   - Total Revenue Earned      : LKR %.2f\n", totalRevenue);
    printf("   - Total Discounts Granted   : LKR %.2f\n", totalDiscounts);
    printf("----------------------------------------------------\n");

    // 3. Bed Occupancy Percentage Per Ward
    printf("3. Bed Occupancy Percentage Per Ward:\n");
    for (int i = 0; i < 4; i++) {
        int wardId = i + 1;
        int capacity = wardCapacities[wardId];
        int occupiedCount = 0;
        for (int j = 0; j < capacity; j++) {
            if (beds[i][j] == 1) {
                occupiedCount++;
            }
        }

        float occupancyPercentage = 0.0f;
        if (capacity > 0) {
            occupancyPercentage = ((float)occupiedCount / capacity) * 100.0f;
        }

        printf("   - %s: %.1f%% (%d/%d Beds Occupied)\n",
               wardNames[wardId], occupancyPercentage, occupiedCount, capacity);
    }
    printf("----------------------------------------------------\n");

    // 4. Highest-Paying Patient Record
    printf("4. Highest-Paying Patient Record:\n");
    if (highestBill > 0.0f) {
        printf("   - Patient Name : %s\n", highestName);
        printf("   - Total Bill   : LKR %.2f\n", highestBill);
    } else {
        printf("   - No billed patients recorded yet.\n");
    }
    printf("==================================================== \n");
}

// File Handling: Save current bed occupancy status to disk
void saveBedStatus(int beds[4][20], int wardCapacities[]) {
    FILE *f = fopen("beds_status.txt", "w");
    if (f == NULL) {
        printf("Error: Could not open beds_status.txt for writing.\n");
        return;
    }
    for (int i = 0; i < 4; i++) {
        int wardId = i + 1;
        for (int j = 0; j < wardCapacities[wardId]; j++) {
            fprintf(f, "%d ", beds[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

// File Handling: Load bed occupancy status from disk on startup
void loadBedStatus(int beds[4][20], int wardCapacities[]) {
    FILE *f = fopen("beds_status.txt", "r");
    if (f == NULL) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        int wardId = i + 1;
        for (int j = 0; j < wardCapacities[wardId]; j++) {
            if (fscanf(f, "%d", &beds[i][j]) != 1) {
                break;
            }
        }
    }
    fclose(f);
}

// File Handling: Append permanent billing logs to text file
void appendPatientRecord(int id, char* name, char* admitDate, char* dischargeDate, float finalPayable) {
    FILE *f = fopen("patient_records.txt", "a");
    if (f == NULL) {
        printf("Error: Could not open patient_records.txt for appending.\n");
        return;
    }
    fprintf(f, "ID: PAT-%d | Name: %s | Admit Date: %s | Discharge Date: %s | Final Paid: LKR %.2f\n",
            id, name, admitDate, dischargeDate, finalPayable);
    fclose(f);
}
