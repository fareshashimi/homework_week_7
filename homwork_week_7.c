#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the struct for patient information
struct Patient {
    char name[50];
    int age;
    char illness[100];
};

int main() {
    FILE *file;
    int choice;

    // Open the file in binary read/write mode
    file = fopen("patient_records.dat", "r+");

    if (file == NULL) {
        printf("File does not exist. Creating a new file...\n");
        file = fopen("patient_records.dat", "w+");
        if (file == NULL) {
            printf("Error opening the file.\n");
            return 1;
        }
    }

    while (1) {
        printf("\nMenu:\n");
        printf("1. Insert Patient\n");
        printf("2. Find Patient by Key Data\n");
        printf("3. Delete Patient Record\n");
        printf("4. Modify Patient Record\n");
        printf("5. List All Patients\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Insert Patient
                {
                    struct Patient patient;
                    printf("Enter Patient Name: ");
                    scanf("%s", patient.name);
                    printf("Enter Patient Age: ");
                    scanf("%d", &patient.age);
                    printf("Enter Patient Illness: ");
                    scanf("%s", patient.illness);

                    // Write the patient record to the file
                    fseek(file, 0, SEEK_END);
                    fwrite(&patient, sizeof(struct Patient), 1, file);
                    printf("Patient added successfully!\n");
                }
                break;
            case 2:
                // Find Patient by Key Data
                {
                    char keyData[100];
                    printf("Enter Key Data to Find Patient: ");
                    scanf("%s", keyData);

                    struct Patient patient;
                    int found = 0;

                    rewind(file);
                    while (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
                        if (strstr(patient.name, keyData) || strstr(patient.illness, keyData)) {
                            printf("Patient Found:\n");
                            printf("Name: %s\n", patient.name);
                            printf("Age: %d\n", patient.age);
                            printf("Illness: %s\n", patient.illness);
                            found = 1;
                        }
                    }

                    if (!found) {
                        printf("Patient not found.\n");
                    }
                }
                break;
            case 3:
                // Delete Patient Record
                {
                    char patientName[50];
                    printf("Enter Patient Name to Delete: ");
                    scanf("%s", patientName);

                    struct Patient patient;
                    int found = 0;
                    FILE *tempFile = fopen("temp.dat", "w+");

                    rewind(file);
                    while (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
                        if (strcmp(patient.name, patientName) == 0) {
                            found = 1;
                            printf("Patient Deleted:\n");
                            printf("Name: %s\n", patient.name);
                            printf("Age: %d\n", patient.age);
                            printf("Illness: %s\n", patient.illness);
                        } else {
                            fwrite(&patient, sizeof(struct Patient), 1, tempFile);
                        }
                    }

                    fclose(file);
                    fclose(tempFile);
                    remove("patient_records.dat");
                    rename("temp.dat", "patient_records.dat");

                    if (!found) {
                        printf("Patient not found.\n");
                    }

                    // Reopen the file for further operations
                    file = fopen("patient_records.dat", "r+");
                }
                break;
            case 4:
                // Modify Patient Record
                {
                    char patientName[50];
                    printf("Enter Patient Name to Modify: ");
                    scanf("%s", patientName);

                    struct Patient patient;
                    int found = 0;

                    rewind(file);
                    while (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
                        if (strcmp(patient.name, patientName) == 0) {
                            found = 1;
                            printf("Enter New Patient Name: ");
                            scanf("%s", patient.name);
                            printf("Enter New Patient Age: ");
                            scanf("%d", &patient.age);
                            printf("Enter New Patient Illness: ");
                            scanf("%s", patient.illness);
                            fseek(file, sizeof(struct Patient), SEEK_CUR);
                            fwrite(&patient, sizeof(struct Patient), 1, file);
                            printf("Patient Record Modified.\n");
                        }
                    }

                    if (!found) {
                        printf("Patient not found.\n");
                    }
                }
                break;
            case 5:
                // List All Patients
                {
                    struct Patient patient;

                    rewind(file);
                    printf("List of All Patients:\n");
                    while (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
                        printf("Name: %s\n", patient.name);
                        printf("Age: %d\n", patient.age);
                        printf("Illness: %s\n", patient.illness);
                        printf("-----------------------\n");
                    }
                }
                break;
            case 6:
                fclose(file);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
