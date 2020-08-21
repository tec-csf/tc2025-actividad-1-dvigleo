/*
    Alumno: Daniela Vignau León
    Fecha: 20 de agosto de 2020
    Materia: Programación Avanzada
    Profesor: Vicente Cubells
    Tarea 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5

typedef struct {
    char * name;
    char * lastnames;
    int age;
    char * phone_number;
    int id;
    char * available;
} patient;

void admit_patient(patient *, patient *, int, int *, int *, int *, int *, int *);
void list_patients(patient *, patient *);
void discharge_patient(patient *, patient *, int *, int *);
void general_info(int *, int *, int *);
void search_bed_info(patient *, patient *, int *, int *, int *);
void exit_program(patient *, patient *);

int main(int argc, char const *argv[]) {
    patient * hospital;
    patient * p;
    int n_patients = 0;

    hospital = (patient *) malloc(sizeof(patient));
    patient * total_patients = hospital + n_patients;

    // Bed ID
    int bed_id;
    int * bid_ptr = &bed_id;
    *bid_ptr = 0;

    int total_beds;
    int *t_beds = &total_beds;
    *t_beds = N;

    int available_beds;
    int *a_beds = &available_beds;
    *a_beds = N;

    int used_beds;
    int *u_beds = &used_beds;
    *u_beds = 0;

    int saved_id;
    int *s_id = &saved_id;

    int option = 0;
    while(option != 6) {
        printf("\n\n*************** HOSPITAL MANAGEMENT SYSTEM ***************\n\n");
        printf("1 - View hospital general information\n2 - Admit new patient \n3 - Discharge patient \n4 - See all admitted patients\n5 - Search bed information \n6 - Exit program"); 
        printf("\nSelect an option (1-6): ");
        scanf("%d", &option);   

        
        switch (option) {
        case 1: // View hospital info
            general_info(t_beds, a_beds, u_beds);
            break;
        case 2: // Admit new patient
            if(used_beds == 0) {
                n_patients ++;
                hospital = (patient *) realloc(hospital, sizeof(patient) * n_patients);
                total_patients = hospital + n_patients;
                admit_patient(hospital, total_patients, n_patients, bid_ptr, u_beds, a_beds, t_beds, NULL);
            } else {
                for(patient * aux = hospital; aux < total_patients; ++aux) {
                    if(aux->name == NULL) {
                        int temp_id = aux->id;
                        * s_id = temp_id;
                        printf("I have found an empty bed at %d\n", aux->id);
                        admit_patient(hospital, total_patients, n_patients - 1,  bid_ptr, u_beds, a_beds, t_beds, s_id);
                        break;
                    } else {
                        n_patients ++;
                        hospital = (patient *) realloc(hospital, sizeof(patient) * n_patients);
                        total_patients = hospital + n_patients;
                        admit_patient(hospital, total_patients, n_patients, bid_ptr, u_beds, a_beds, t_beds, NULL);
                        break;
                    }
                }   
            }
            break;
        case 3: 
            discharge_patient(hospital, total_patients, u_beds, a_beds);
            break;   
        case 4:
            list_patients(hospital, total_patients);  
            break;      
        case 5:
            search_bed_info(hospital, total_patients, a_beds, u_beds, t_beds);
            break;
        case 6: 
            exit_program(hospital, total_patients);
            break;
        default:
            break;
        }

    }

    return 0;
}

void admit_patient(patient * hospital, patient * total_patients, int n_patients, int * id, int * used_beds, int * available_beds, int * total_beds, int * s_id) {
    // printf("n_patients: %d\n", n_patients);
    patient * p = hospital + n_patients - 1;

    printf("\n\n*************** ADMIT PATIENT ***************\n\n");
    printf("Please enter the data below\n");
    
    p->name = (char *) malloc(sizeof(char) * 30);
    printf("\nName (s): ");
    scanf(" %[^\n]", p->name);

    p->lastnames = (char *) malloc(sizeof(char) * 30);
    printf("Lastnames: ");
    scanf(" %[^\n]", p->lastnames);

    printf("Age: ");
    scanf("%d", &p->age);

    p->phone_number = (char *) malloc(sizeof(char) * 30);
    printf("Phone number: ");
    scanf("%s", p->phone_number);
    
    p->available = (char *) malloc(sizeof(char) * 3);
    p->available = "No";

    if(s_id == NULL){        
        *id += 1;
        p->id = *id;
    } else {
        p->id = *s_id;
    }
    
    if(*available_beds == 0) {
        *available_beds += N;
        *total_beds += N;
    }

    *used_beds += 1;
    *available_beds -= 1;

    printf("\n\n*************** PATIENT SAVED ***************\n\n");
    printf("Name: %s %s\n   Age: %d\n   Phone number: %s\n   Bed ID: %d", p->lastnames, p->name, p->age, p->phone_number, p->id);
}

void list_patients(patient * hospital, patient * total_patients){
    patient * p;
    printf("\n\n*************** LIST ADMITTED PATIENTS ***************");
    for(p = hospital; p < total_patients; ++p) {
        if(p->name != NULL) {
            printf("\n-------------------------------------------------------\n");
            printf("\nName: %s %s\n   Age: %d\n   Phone number: %s\n   Bed ID: %d\n", p->lastnames, p->name, p->age, p->phone_number, p->id);      
        }
    }
}

void discharge_patient(patient * hospital, patient * total_patients, int * used_beds, int * available_beds) {
    patient * p;
    int discharge;
    int available_id;
    
    printf("\n\n*************** DISCHARGE PATIENT BY BED ID ***************\n\n");
    if(*used_beds == 0){
        printf("\nThere are no patients in the system to be discharged\n");
    } else {
        printf("Type in the ID of the patient you wish to discharge.\nType in -1 to cancel: ");
        scanf("%d", &discharge);
        if(discharge == -1) {
            printf("\nCanceling operation...\n");
        } else {
            for(p = hospital; p < total_patients; ++p) {
                if(p->id == discharge) {
                    available_id = p->id;
                    printf("\nPatient %s %s has been sucessfully ", p->lastnames, p-> name);
                    free(p->lastnames);
                    free(p->name);
                    p->name = NULL;
                    // free(p->phone_number);
                    printf("discharged\nBed %d is now available\n", available_id);
                    *used_beds -= 1;
                    *available_beds += 1;
                }
            }
        }
    }
}

void general_info(int * total_beds, int * available_beds, int * used_beds) {
    printf("\n\n*************** GENERAL INFORMATION ***************\n\n");
    printf("Admitted of patients: %d\nUsed beds: %d\nAvailable beds: %d\nTotal beds: %d\n", *used_beds, *used_beds, *available_beds, *total_beds);
}

void search_bed_info(patient * hospital, patient * total_patients, int * available_beds, int * used_beds, int * total_beds) {
    patient * p;
    int bed_info;

    printf("\n\n*************** SEE BED INFO BY ID ***************\n\n");
    printf("Type in the Bed ID to see its information.\nType in -1 to cancel: ");
    scanf("%d", &bed_info);
    printf("\nNote: If the name of the patient is not mentioned,\nit means that bed is AVAILABLE\n");

    if(bed_info == -1) {
        printf("\nCanceling operation...\n\n");
    } else {
        if(bed_info > *total_beds) {
            printf("\nBed %d does not exist\n\n", bed_info);
        } else if(bed_info > 0 && bed_info <= *total_beds){
            printf("\nBed %d does exist ", bed_info);
            for(p = hospital; p < total_patients; ++p) {
                if(p->available == "No") {
                    if(p->id == bed_info){
                        printf("and it is currently assigned to patient %s %s\n\n", p->lastnames, p->name);
                        break;
                    } 
                } 
            }
            
        } 
        
    }
}

void exit_program(patient * hospital, patient * final) {
    patient * p;

    for(p = hospital; p < final; ++p) {
        free(p->name);
        free(p->lastnames);
        free(p->phone_number);
    }
    free(hospital);

    printf("\n\nExiting program...\n\n");
    exit(0);
}