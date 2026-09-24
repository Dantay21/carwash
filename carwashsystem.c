//Group Members: Alyssa Joesephs, Daniel Burrie, Michael Johnson, Samanda Bradley
//Date: March 20, 2023
//Instruction: Programming 2 Car Wash System Project
//Note Briefly: Please test the code using the online gdb compiler or Code Blocks, but preferably GDB.

//Headers
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_SERVICES 6
#define MAX_CUSTOMERS 20
#define MAX_WASHBAY 3
#define CARD_FEE 0.03
#define FILENAME "customers.txt" //FILENAME as a global constant//MACRO
#define MAX_SERVICE_NAME_LENGTH 50

struct Customer;
void saveCustomerToFile(struct Customer customer);
//function prototypes
void customerInfo();
void washCar();
void returningCustomer();
void salesReport();
void exitProgram();
void saveCustomerToFile(struct Customer customer);
int isDateValid(int day, int month, int year);
int isLicensePlateValid(char *licensePlate, char **existingPlates, int numPlates);

//START OF ENUM
enum ServiceType {
WASH_VACUUM,
ENGINE_WASH,
POLISHING,
BUFFING,
ROOF_CLEANING,
DETAILING }; //END OF ENUM

//START OF STRUCTURE
struct Service { 
    enum ServiceType type;
    float cost;
}; //END OF STRUCTURE

//START OF STRUCTURE
struct Customer {
	int day;
    int month;
    int year;
    char firstName[25];
    char lastName[25];
    char licensePlate[7];
    char payMethod[11];
    struct Service services[MAX_SERVICES];
    float tip;
    float totalCost;
    float grandTotal;
}; //END OF STRUCTURE

struct Customer customers[MAX_CUSTOMERS];
struct Customer washbay[MAX_WASHBAY];
struct Customer permCustomer[100];
int inWashBay=0;
int totalCustomers = 0;
static int numCustomers = 0;

//storing the cost of ther services
//START OF STRUCTURE
struct Service getServiceInfo(enum ServiceType type) {
    struct Service service;
    switch (type) {
        case WASH_VACUUM:
            service.cost = 2500.00;
            break;
        case ENGINE_WASH:
            service.cost = 2000.00;
            break;
        case POLISHING:
            service.cost = 3500.00;
            break;
        case BUFFING:
            service.cost = 5500.00;
            break;
        case ROOF_CLEANING:
            service.cost = 3200.00;
            break;
        case DETAILING:
            service.cost = 7500.00;
            break;
        default:
            printf("Invalid service type.\n");
            service.cost = 0.00;
    }
    service.type = type;
    return service;
} //END OF STRUCTURE 

//saving the customer information to the file
//FILE OPEN
void saveCustomerToFile(struct Customer customer) {
    FILE *fp;
    fp = fopen(FILENAME, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
	fprintf(fp, "%s,%s,%s,%s,%d/%d/%d,%.2f,%.2f\n", customer.firstName, customer.lastName, customer.licensePlate, customer.payMethod,customer.day, customer.month, customer.year, customer.totalCost,customer.grandTotal);
	for (int i = 0; i < MAX_SERVICES; i++) {
        if (customer.services[i].type != -1) {
            fprintf(fp, "%d,", customer.services[i].type);
        }
    }
    fclose(fp);
} //FILE CLOSE 


// FUNCTION isDateValid
//VALIDATE THE DATA ENTERED BY USER 
int isDateValid(int day, int month, int year) {
    // Check if month is valid
    if (month < 1 || month > 12 || year!=2023 ) {
    return 0;
    }

    // Check if day is valid for the given month
    int maxDays = 31;
    if (month == 2) {
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
            maxDays = 29; // Leap year
        } else {
            maxDays = 28; // Non-leap year
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDays = 30;
    }

    if (day < 1 || day > maxDays) {
        return 0;
    }

return 1;
} //END OF FUNCTION isDateValid


// Function to validate license plate number
//FUNCTION isLicensePlateDuplicate
int isLicensePlateDuplicate(char *licensePlate, struct Customer *customers, int numCustomers) {
    int len = strlen(licensePlate);
    if (len != 6) {
        return 1;  // Error code for invalid length
    }

    // Check if the first 4 characters are digits
    for (int i = 0; i < 4; i++) {
        if (!isdigit(licensePlate[i])) {
            return 2;   // Error code for non-digit character
        }
    }

    // Check if license plate already exists
    for (int i = 0; i < numCustomers; i++) {
        if (strcmp(licensePlate, customers[i].licensePlate) == 0) {
            return 4;  // License plate already exists
        }
    } //END FUNCTION isLicensePLateDuplicate 



    // Check if last 2 characters are capital letters
    if (!isupper(licensePlate[len-1]) || !isupper(licensePlate[len-2])) {
        return 3;   // Error code for non-capital letter character
    }

    return 0;  // Success code
}

//MENU FUNCTION 1
//FUNCTION customerInfo
void customerInfo() {
    //checking to see if max has been reached 
    if (numCustomers == MAX_CUSTOMERS) {
        printf("Sorry, we can't accept any more customers at this time. Please try again later.\n");
        return;} //ENDIF

    struct Customer newCustomer; //struct instance newCustomer
    newCustomer.totalCost = 0.0;
    srand(time(NULL)); // Initialize random number generator with current time
    int random_num = rand() % 2; // Generate a random number between 0 and 1
    newCustomer.grandTotal=0.0;
    float cardPercentage;
    

    //Validate Date 
    int dateValid = 0;
	while (!dateValid)
	{
		printf("Enter transaction date [dd/mm/2023]: ");
	    scanf("%d/%d/%d", &newCustomer.day, &newCustomer.month, &newCustomer.year);

	    // Validate date entered by user
	    dateValid = isDateValid(newCustomer.day, newCustomer.month, newCustomer.year);

	    if (!dateValid) {
	        printf("Invalid date entered. Please enter a valid date.\n");
		}
	} //ENDWHILE

	//Validate License Plate


	int licensePlateValid = 0;

	while (licensePlateValid == 0)
	{
	    printf("Enter a license plate with 4 number and 2 capital letters: ");
	    scanf("%s", newCustomer.licensePlate);

	    // Validate license plate number entered by user and check for duplicates
		int isValid= isLicensePlateDuplicate(newCustomer.licensePlate, customers, numCustomers);
	    if (isValid == 1) {
	        printf("Invalid license plate length. Enter a 6-character license plate number.\n");
	    } else if (isValid == 2) {
	        printf("Invalid license plate characters. Enter a license plate with digits as the first 4 characters.\n");
	    } else if (isValid == 3) {
	        printf("Invalid license plate characters. Enter a license plate number with capital letters as the last 2 characters.\n");
	    } else if (isValid == 4) {
	        printf("License plate number already exists. Enter a unique license plate number.\n");
	    } else {
	        licensePlateValid = 1;
	    }
	} //ENDWHILE

    
    printf("Enter customer's first name: ");
    scanf("%s", newCustomer.firstName);
    printf("Enter customer's last name: ");
    scanf("%s", newCustomer.lastName);

    //PAYMENT METHOD
	while(1)
	{
	    printf("Enter customer's payment method [cash/card]: ");
	    scanf("%s", newCustomer.payMethod);

	    // Validate payment method entered by user
	    if (strcmp(newCustomer.payMethod, "cash") == 0 || strcmp(newCustomer.payMethod, "card") == 0) {
	        break; // Exit loop if payment method is valid
	    } else
		{
	        printf("Invalid payment method entered. Please enter a valid payment method.\n");
		}
	} //ENDWHILE

    // Services offered menu
    printf("Select services required:\n");
    printf("1. Wash and Vacuum\n");
    printf("2. Engine Wash\n");
    printf("3. Polishing\n");
    printf("4. Buffing\n");
    printf("5. Roof Cleaning\n");
    printf("6. Detailing/Interior Shampooing\n");
    //DECLARATIONS
    int numServices = 0;
    int serviceOption;
    int i;

    do {
        printf("Enter service option (1-6) or 0 to finish: ");
        scanf("%d", &serviceOption);
        if (serviceOption >= 1 && serviceOption <= 6) {
            // Check if the service option has already been entered
            int alreadyEntered = 0;
            for (i = 0; i < numServices; i++) {
                if (newCustomer.services[i].type == (enum ServiceType)(serviceOption - 1)) {
                    alreadyEntered = 1;
                    break;
                }
            }
            if (!alreadyEntered) {
                // Convert the integer input to an appropriate ServiceType value
                newCustomer.services[numServices] = getServiceInfo((enum ServiceType)(serviceOption - 1));
                numServices++;
            } else {
                printf("\nService option %d has already been entered.\n", serviceOption);
            }
        }
        else if (serviceOption != 0) {
            printf("\nInvalid input. Please enter a number from 1 to 6.\n");
        }

    } while (serviceOption != 0 && numServices < MAX_SERVICES); //END DO WHILE LOOP



	// Calculate total cost
	for (int i = 0; i < numServices; i++) {
    	newCustomer.totalCost += newCustomer.services[i].cost;
	}

	 // Add card payment fee if applicable
    if (strcmp(newCustomer.payMethod, "card") == 0)
	{
        cardPercentage= newCustomer.totalCost *CARD_FEE;
    }

	//DISPLAY RECEIPT
	printf("\n=======RECEIPT========");
	printf("\nName: %s %s",newCustomer.firstName,newCustomer.lastName);
	printf("\nCard Fee: %.2f",cardPercentage);
	// Display total service cost to user
	printf("\nTotal Service Cost: $%.2f\n", newCustomer.totalCost);

	char response;
	 // Generate a random response
    if (random_num == 0)
	{
        response = 'y';
    }
	else
	{
        response = 'n';
    }
    // If user wants to give a tip, add 2% of total cost to the total cost
    if (response == 'y') {
        newCustomer.tip=newCustomer.totalCost*0.02;
	}
	else{
	     newCustomer.tip=0;
	}
	printf("Tip Charged: %.2f",newCustomer.tip);
	//calculation of grand total stored in newCustomer.grandTotal variables
	newCustomer.grandTotal=newCustomer.tip+newCustomer.totalCost+cardPercentage;
	printf("\nGrand Total: %.2f",newCustomer.grandTotal);
    
    //assigning the information of newCustomer to customers using numCustomers as index
    customers[numCustomers] = newCustomer;
    //assigning the information of customers to permCustomers 
    permCustomer[totalCustomers]=customers[numCustomers];
    numCustomers++; //increment the number of current customers
    totalCustomers++; //increment the total number of customers
    printf("\nCustomer information added.\n");

    //append customer information to file
    //OPEN FILE customer.txt
    FILE *filePtr;
    filePtr = fopen("customers.txt", "a");
    if (filePtr == NULL) {
        printf("Error opening file.");
        return;
    }
    
    fprintf(filePtr, "%s %s %s %s  %d/%d/%d %.2f %.2f\n", newCustomer.firstName, newCustomer.lastName, newCustomer.licensePlate, newCustomer.payMethod, newCustomer.day, newCustomer.month, newCustomer.year,newCustomer.totalCost,newCustomer.grandTotal);
	fclose(filePtr); //CLOSE File customer.txt
} //END OF FUNCTION customerInfo

//print the customers license plate 
//FUNCTION printcustomers
void printcustomers() {
    for (int i = 0; i < numCustomers; i++) {
        printf("%d\t%s\n",i+1, customers[i].licensePlate);
    }
} //END OF FUCNTION printcustomers

//print the customer's license plate in the washbay
//FUNCTION printcustomersWasgBay
void printcustomersWashBay() {
    for (int i = 0; i < inWashBay; i++) {
        printf("%d\t%s\n",i+1, washbay[i].licensePlate);
    }
} //END OF FUNCTION printcustomersWasgBay

//START OF FUNCTION
void moveToWashBay(struct Customer customers[], int* numCustomers, int index, struct Customer washbay[], int* inWashBay) {
    // Move the customer at the specified index to the wash bay
    washbay[*inWashBay] = customers[index];
    // Clear the original customer from the customers array
    memset(&customers[index], 0, sizeof(struct Customer));

    // Shift all elements after index one position to the left
    for (int i = index; i < (*numCustomers) - 1; i++) {
        customers[i] = customers[i+1];
    }
    // Clear the last element in the customers array
    memset(&customers[*numCustomers-1],0, sizeof(struct Customer));
    (*numCustomers)--;
	(*inWashBay)++;
} //END OF FUNCTION
//START OF FUNCTION
void removeCustomer(struct Customer washbay[], int index, int* inWashBay) {
    // Shift all elements after index one position to the left
    for (int i = index; i < (*inWashBay) -1; i++) {
        washbay[i] = washbay[i+1];
    }
    // Clear the last element in the customers array
    //if(index>1){
    memset(&washbay[*inWashBay-1], 0, sizeof(struct Customer));
    (*inWashBay)--;
} //END OF FUNCTION

//MENU FUNCTION 2
//START OF FUNCTION washCar
void washCar()
{
    int position = 0, pay = 0.,washCarChoice;
    do
    {
        //output menu option 
        printf("\n1. Move customer to washbay");
        printf("\n2. Wash car");
        printf("\n3. Return to main menu");
        printf("\n Enter your choice: ");
        scanf("%d",&washCarChoice);
        switch (washCarChoice) //START OF CASE
        {
        case 1:
            if(numCustomers<1){
                printf("\nThere are no customers currently to move to the washbay");
                break;
            }
            else if (inWashBay==MAX_WASHBAY){
                printf("\nSorry the washbay is currently full");
                break;
            }
            
            //validate the position entered by the user for the car to be moved
            do{
            printcustomers(); //CALL FUNCTION
            printf("Enter the position of the car you wish to move to the washbay: ");
            scanf("%d", &position);
            if (1>position || position>numCustomers)
            {
                printf("Invalid input enter a number within the range\n");
            }
            }while (1>position || position>numCustomers);
            
            //validate the option for if the customer paid entered by the user
            do{
            printf("Have they paid? enter 1 for yes 0 for no: \n ");
            scanf("%d", &pay);
            if (pay < 0 || pay > 1)
            {
            	printf("Invalid input enter 0 or 1\n");
			}
            }while (pay < 0 || pay > 1);

            if(pay){
            moveToWashBay(customers, &numCustomers, position-1, washbay, &inWashBay);}
            break;
        case 2:
            if (inWashBay==0){
                printf("\nThe washbay is currently empty");
                break;
            }
            //validate the position entered by the user for washBay
            do
            {
                printcustomersWashBay();
                printf("\nWhat car would you like to wash:");
                scanf("%d",&position);
                if ( position < 1 || position > inWashBay)
                {
                    printf("Invalid input enter a number within the range\n");
                }
            }while (position < 1 || position > inWashBay);
            removeCustomer(washbay, position-1, &inWashBay);
            break;
        case 3:
            return;
            break;

        default:
            printf("INVALID INPUT ENTER OPTION PROVIDED (1-3)");
            break;
        } //ENDCASE

    } while (position != -1000);
} //END OF FUNCTION washCar


void returningCustomer()
{
    //FILE OPEN customers.txt
    FILE *fp = fopen("customers.txt", "r");
    if (fp == NULL) //check if the file is empty 
    {
        printf("Unable to open file");
        return;
    }

    char line[100];
    char licenseplates[100][7]; // array to store all license plates
    char firNames[100][20]; // array to store all first names
    char lasNames[100][20]; // array to store all last names
    float totalAmtSpent[100] = {0.0};
    int freqVisits[100] = {0};
    int count = 0;
    int mostRetCustomer=0;
    int numLicensePlates = 0;

    // read each line in the file if it is not empty
    while (fgets(line, 100, fp) != NULL)
    {
        char tempLicense[7];
        char tempFirName[20];
        char tempLasName[20];
        float tempAmtSpent;
        //scanning the line of the file for the necessary information
        //the astericks are used to ignore the irrelevant in the line of the file
        if (sscanf(line, "%s %s %s %*s %*d/%*d/%*d %*f %f",tempFirName, tempLasName, tempLicense, &tempAmtSpent) != 4)
        //e.gFIRSTNAME LASTNAME 1111AA card  1/1/2 2500.00 2575.00

        {
            continue; // skip invalid lines
        }

        int found = 0;
        // check if the license plate is already in the array
        for (int i = 0; i < numLicensePlates; i++)
        {
            if (strcmp(tempLicense, licenseplates[i]) == 0)
            {
                freqVisits[i]++;
                totalAmtSpent[i] += tempAmtSpent; //putting the tempAmtSpent into the array index
                found = 1;
                break;
            }
        }

        // if the license plate is not in the array, add it
        if (!found)
        {
            strcpy(licenseplates[numLicensePlates], tempLicense);
            strcpy(firNames[numLicensePlates], tempFirName);
            strcpy(lasNames[numLicensePlates], tempLasName);
            totalAmtSpent[numLicensePlates] = tempAmtSpent;
            freqVisits[numLicensePlates] = 1; //initialize freqVisits[numLicensePlate] to 1
            numLicensePlates++;
        }
    }//ENDWHILE

    //output showing the customer who appeared more than once
    printf("Customers who have visited more than once:\n");
    printf("First Name\tLast Name\tLicense Plate\tTotal Amount Spent\tFrequency of Visits\n");
    printf("------------------------------------------------------------------------------\n");

    // print results for customers who have visited more than once
    for (int i = 0; i < numLicensePlates; i++)
    {
        if (freqVisits[i] > 1)
        {
            printf("%s\t\t%s\t\t%s\t\t%.2f\t\t %d\n", firNames[i], lasNames[i], licenseplates[i], totalAmtSpent[i], freqVisits[i]);
            count++;
        }
    }

    for (int i = 0; i < numLicensePlates; i++)
	{
        if (freqVisits[i] > freqVisits[mostRetCustomer])
        {
            mostRetCustomer = i; 
        }
	}
    
    
    
	float disAmount = 0.2 * customers[mostRetCustomer].totalCost;
	float disTotal = customers[mostRetCustomer].totalCost - disAmount;

	printf("\nThe most  returning customer is %s %s and their total is %.2f. \n%s gets a 20 percent discount of %.2f. The new total is %.2f.", firNames[mostRetCustomer], lasNames[mostRetCustomer],customers[mostRetCustomer].totalCost, firNames[mostRetCustomer],disAmount,disTotal);
    

    if (count == 0)
    {
        printf("No returning customers found.\n");
    }

    fclose(fp); //CLOSE FILE customers.txt
} //END OF FUCNTION for returningCustomer

//START OF FUNCTION salesReport
void salesReport() {
    char serviceNames[MAX_SERVICES][MAX_SERVICE_NAME_LENGTH] = {"Wash & Vacuum", "Engine Wash", "Polishing", "Buffing", "Roof Cleaning", "Detailing"};
    float totalSales[MAX_SERVICES] = {0},tSale,totalTips; // initialize to 0

     tSale=0;
     totalTips=0;
     int i=0, j =0;
    for (i=0; i < totalCustomers; i++) {
        for (j=0; j < MAX_SERVICES; j++) {
            if (permCustomer[i].services[j].type <=MAX_SERVICES ) { // check if service exists
                //adding the cost of a specific service provided by a customer to the total sales of that service type.
                totalSales[permCustomer[i].services[j].type] += permCustomer[i].services[j].cost;
               // "i" used to access the service type of a specific service provided by the customer.
                //
            } //ENDIF
        } //ENDFOR
    
        totalTips+= permCustomer[i].tip;
    } //ENDFOR
    for (i = 0; i < MAX_SERVICES; i++) {
    tSale += totalSales[i];
}
    // Print the final report
    printf("Final report:\n");
    printf("Total number of customers: %d\n", totalCustomers);
    printf("Total sales for each service:%.2f\n",tSale);
    printf("Total tips: %.2f\n",totalTips);
    fflush(stdout);
    for (int i = 0; i < MAX_SERVICES; i++) {
        printf("%s: $%.2f\n", serviceNames[i], totalSales[i]);
        fflush(stdout); //forces the output out
    }
    
} //END OF FUNCTION salesReport

//START OF FUNCTION exitProgrom
void exitProgrom(){

    system("cls"); 

    printf("\n\n\t\t\t\t\tEXITING THE SYSTEM...");
    printf("\n\t\t\t\t\t***********************************************");
    printf("\n\t\t\t\t\t**                                           **");
    printf("\n\t\t\t\t\t**              TISH AND SHAUN AUTOWASH      **");
    printf("\n\t\t\t\t\t**              HAVE A GOOD DAY              **");
    printf("\n\t\t\t\t\t**               ______________              **");
    printf("\n\t\t\t\t\t**         # #  #    #  # # #                **");
    printf("\n\t\t\t\t\t**        #   #  #  #   #                    **");
    printf("\n\t\t\t\t\t**        # #     #     # # #                **");
    printf("\n\t\t\t\t\t**        #   #   #     #                    **");
    printf("\n\t\t\t\t\t**        # # #   #     # # #                **");
    printf("\n\t\t\t\t\t**              ______________               **");
    printf("\n\t\t\t\t\t**                                           **");
    printf("\n\t\t\t\t\t**               THANK YOU!                  **");
    printf("\n\t\t\t\t\t**                                           **");
    printf("\n\t\t\t\t\t***********************************************");
    exit(1);
} //END OF FUNCTION exitProgrom

int main ()
{
	int option;
	do
	{
		printf("\nTRISH AND SHAUN AUTO WASH SERVICES\n");
		printf("1. Accept Customer's Information & Payment\n");
		printf("2. Wash Car\n");
		printf("3. Display Returning Customers\n");
		printf("4. Display Day's Sales Report\n");
		printf("5. Exit Program\n");
		printf("=> Enter your choice: ");
		scanf("%d", &option);
		switch (option)
		{
		//CALL FUNCTION
		case 1:
			customerInfo();
		break;
		case 2:
			washCar();
		break;
		case 3:
			returningCustomer();
		break;
		case 4:
			salesReport();
		break;
		case 5:
			exitProgrom();
		break;
		default:
		printf("Invalid choice. Please enter a number between 1 and 5.\n");
		}
		} while (option != 5);

 return 0;
} //END OF FUNCTION main