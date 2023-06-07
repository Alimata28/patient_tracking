#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[12];  // TC Kimlik No
    char name[50];
    int age;
    char disease[50];
} Patient;

void addPatient(FILE *patientFile, FILE *indexFile) {
    Patient newPatient;
    printf("Enter patient ID (TC Kimlik No): ");
    scanf("%s", newPatient.id);
    printf("Enter patient name: ");
    scanf(" %[^\n]", newPatient.name);
    printf("Enter patient age: ");
    scanf("%d", &newPatient.age);
    printf("Enter patient disease: ");
    scanf(" %[^\n]", newPatient.disease);

    fseek(patientFile, 0, SEEK_END);
    fwrite(&newPatient, sizeof(Patient), 1, patientFile);
    fprintf(indexFile, "%s %ld\n", newPatient.id, ftell(patientFile) - sizeof(Patient));
    printf("Patient record added successfully!\n");
}

void searchPatient(FILE *indexFile, FILE *patientFile) {
    char searchID[12];
    printf("Enter the patient ID to search: ");
    scanf("%s", searchID);

    char currentID[12];
    long int recordPosition;
    int found = 0;

    rewind(indexFile);
    while (fscanf(indexFile, "%s %ld", currentID, &recordPosition) == 2) {
        if (strcmp(currentID, searchID) == 0) {
            found = 1;
            break;
        }
    }

    if (found) {
        fseek(patientFile, recordPosition, SEEK_SET);
        Patient patient;
        fread(&patient, sizeof(Patient), 1, patientFile);

        printf("\nPatient found!\n");
        printf("ID: %s\n", patient.id);
        printf("Name: %s\n", patient.name);
        printf("Age: %d\n", patient.age);
        printf("Disease: %s\n", patient.disease);
    } else {
        printf("Patient not found!\n");
    }
}

void deletePatient(FILE *indexFile, FILE *patientFile) {
    char deleteID[12];
    printf("Enter the patient ID to delete: ");
    scanf("%s", deleteID);

    char currentID[12];
    long int recordPosition;
    int found = 0;

    rewind(indexFile);
    FILE *tempIndexFile = fopen("temp_index.txt", "w");

    while (fscanf(indexFile, "%s %ld", currentID, &recordPosition) == 2) {
        if (strcmp(currentID, deleteID) == 0) {
            found = 1;
            continue;
        }

        fprintf(tempIndexFile, "%s %ld\n", currentID, recordPosition);
    }

    fclose(indexFile);
    fclose(tempIndexFile);

    remove("index.txt");
    rename("temp_index.txt", "index.txt");

    if (found) {
        printf("Patient record deleted successfully!\n");
    } else {
        printf("Patient not found!\n");
    }
}

void displayPatients(FILE *indexFile, FILE *patientFile) {
    char currentID[12];
    long int recordPosition;

    rewind(indexFile);
    while (fscanf(indexFile, "%s %ld", currentID, &recordPosition) == 2) {
        fseek(patientFile, recordPosition, SEEK_SET);
        Patient patient;
        fread(&patient, sizeof(Patient), 1, patientFile);

        printf("ID: %s\n", patient.id);
        printf("Name: %s\n", patient.name);
        printf("Age: %d\n", patient.age);
        printf("Disease: %s\n\n", patient.disease);
    }
}

int main() {
    FILE *patientFile = fopen("patients.dat", "ab+");
    FILE *indexFile = fopen("index.txt", "a+");

    if (patientFile == NULL || indexFile == NULL) {
        printf("Error opening files!\n");
        return 1;
    }

    int choice;
    do {
        printf("Patient Tracking System\n");
        printf("1. Add patient\n");
        printf("2. Search patient\n");
        printf("3. Delete patient\n");
        printf("4. Display all patients\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient(patientFile, indexFile);
                break;
            case 2:
                searchPatient(indexFile, patientFile);
                break;
            case 3:
                deletePatient(indexFile, patientFile);
                break;
            case 4:
                displayPatients(indexFile, patientFile);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

        printf("\n");
    } while (choice != 5);

    fclose(patientFile);
    fclose(indexFile);

    return 0;
}

