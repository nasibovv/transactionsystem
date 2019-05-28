#include <stdio.h>
#include<stdlib.h>
#include<string.h>

#define ACCOUNTS_FILE_NAME "accounts.txt"
#define TRANSACTIONS_FILE_NAME "transactions.txt"

typedef struct account {
	char name[20];
	char surname[20];
	int number;
	int balance;
} Account;

typedef struct transaction {
	int number;
	int amount;
} Transaction;

void addAccount(void);
void addTransaction(void);

void printAccount(void);
void printTransaction(void);

void printAccounts(Account extraAcc[], int size);
void printTransactions(Transaction extraTrs[], int size);

int getAccountsSize();
int getTransactionsSize();

void readAccounts(Account* accounts, int size);
void readTransactions(Transaction* transactions, int size);

void applyTransactions(Account* accounts, int accSize, Transaction* transactions, int trSize) {
	int i, x;

	for(i = 0; i < accSize; ++i) {		
		for(x = 0; x < trSize; ++x) {												
			if (transactions[x].number != accounts[i].number) {
				/* printf("Transaction No: %d cannot be completed, since the account was not found.\n\n", 
					transactions[x].number); */
			}
			else {
				if ((transactions[x].amount) * -1 > accounts[i].balance && transactions[x].amount < 0) {
					printf("\nTransaction No: %d, Amount: %d cannot be completed, since there is not enough balance.\n", 
						transactions[x].number, transactions[x].amount);
				} 
				else {
					accounts[i].balance += transactions[x].amount; 
				}
			}	
		}
	}
}

void saveChangesToAccount(Account* accounts, int size){

	FILE *fp;

	fp = fopen(ACCOUNTS_FILE_NAME, "w"); /* overwrites existing file */
	if (fp == NULL) {
		printf("File couldn't be opened\n\a\a");
		fclose(fp); exit(1);
	}

	int i; // Save data
	for(i = 0; i < size; i++) {
		fprintf(fp, "%d\t%s\t%s\t%d\n",
		accounts[i].number,
		accounts[i].name,
		accounts[i].surname,
		accounts[i].balance);
	}
	
	fclose(fp);
}

int main(){
	
	addAccount();
	addTransaction();
	printAccount();
	printTransaction();

	printf("\n\nDo you want to add extra accounts? (Y or N): ");

	char acctAnswer[3];
	scanf("%s", acctAnswer);

	if (strcmp(acctAnswer,"Y") == 0){
		int acctSz, n;
		printf("\nHow many extra do you want to add? "); 
		scanf("%d", &acctSz);

		Account extraAcc[acctSz];

		FILE *fp;

		fp = fopen(ACCOUNTS_FILE_NAME, "a+"); // append
		if (fp == NULL) {
			printf("File couldn't be opened\n\a\a");
			fclose(fp);
			exit(1);
		}

		for(n = 0; n < acctSz; n++){

			printf("\n\nEnter %d account info: \n", n+2);
			printf("Account No: "); scanf("%d", &extraAcc[n].number);
			printf("Firstname: "); scanf("%s", extraAcc[n].name);
			printf("Lastname: "); scanf("%s", extraAcc[n].surname);
			printf("Balance: "); scanf("%d", &extraAcc[n].balance);
			printf("\n");

			// Save every account entry
			fprintf(fp, "%d\t%s\t%s\t%d\n", extraAcc[n].number, extraAcc[n].name, extraAcc[n].surname, extraAcc[n].balance);
		}
		
		fclose(fp);

		printAccount(); // printAccounts(extraAcc, size);
	}
	else { 
		puts("Thanks!");
	}

	printf("\n\nDo you want to add transactions? (Y or N): ");

	char tranAnswer[3];
	scanf("%s", tranAnswer);

	if (strcmp(tranAnswer,"Y") == 0){
		FILE *fp;

		fp = fopen(TRANSACTIONS_FILE_NAME, "a+"); // append
		if (fp == NULL) {
			printf("File couldn't be opened\n\a\a");
			fclose(fp);
			exit(1);
		}

		int tranSz, m;
		printf("\nHow many extra transactions do you want to add? "); 
		scanf("%d", &tranSz);

		Transaction extraTrs[tranSz];

		for(m = 0; m < tranSz; ++m){
			
			printf("\n\nEnter %d transaction: \n", m+2);
			printf("Account No: "); scanf("%d", &extraTrs[m].number);
			printf("Amount: "); scanf("%d", &extraTrs[m].amount);
			printf("\n");

			// Save every transaction entry
			fprintf(fp, "%d\t%d\n", extraTrs[m].number, extraTrs[m].amount);
		}
		
		fclose(fp);

		printTransaction(); // printTransactions(extraTrs, size); 
	}
	else { 
		puts("Thanks!");
	}
	
	int accSize = getAccountsSize();
	Account accounts[accSize];
	readAccounts(accounts, accSize);
	printAccounts(accounts, accSize);	
	
	printf("\n\nBefore TRANSACTION processing...\n\n");			
	int tranSize = getTransactionsSize();			
	Transaction transactions[tranSize];
	readTransactions(transactions, tranSize);	
	printTransactions(transactions, tranSize);
	
	applyTransactions(accounts, accSize, transactions, tranSize);	
	printf("\n\nAfter TRANSACTION processing...\n\n");
	
	saveChangesToAccount(accounts, accSize);
	printAccounts(accounts, accSize); 
	
	return 0; 
}

void addAccount(void) {

	FILE *fp;
	Account acct;

	fp = fopen(ACCOUNTS_FILE_NAME, "w+");
	if (fp == NULL) {
		printf("File couldn't be opened\n\a\a");
		fclose(fp); exit(1);
	}
	
	printf("Enter new account info: \n");
	printf("Account No: "); scanf("%d", &acct.number);
	printf("Firstname: "); scanf("%s", acct.name);
	printf("Lastname: "); scanf("%s", acct.surname);
	printf("Balance: "); scanf("%d", &acct.balance);
	printf("\n");

	// Save data
	fprintf(fp, "%d\t%s\t%s\t%d\n", 
		acct.number, acct.name, 
		acct.surname, acct.balance);

	fclose(fp);
}

void addTransaction(void) {
	
	FILE *fp;
	Transaction trans;

	fp = fopen(TRANSACTIONS_FILE_NAME, "w+");
	if (fp == NULL) {
		printf("File couldn't be opened\n\a\a");
		fclose(fp); exit(1);
	}
	
	printf("Enter new transaction: \n");
	printf("Account No: "); scanf("%d", &trans.number);
	printf("Amount: "); scanf("%d", &trans.amount);
	printf("\n");

	// Save data
	fprintf(fp, "%d\t%d\n", trans.number, trans.amount);
	
	fclose(fp);
}

void printAccount() {
	
	FILE * fp = fopen(ACCOUNTS_FILE_NAME, "r");

	if(fp == NULL){	//could not open
		puts("Error opening the file!");
		exit(1);
	}

	Account tempAcct;

	printf("\n\nACCOUNTS from file %s\n\n\n", ACCOUNTS_FILE_NAME);
	printf("---------------------------------ACCOUNTS---------------------------------\n");
	while (fscanf(fp, "%5d %20s %20s %7d\n",  &tempAcct.number, tempAcct.name, 
		tempAcct.surname, &tempAcct.balance) != EOF) {
		printf("\n%5d %-20s %-20s %7d\n", tempAcct.number, tempAcct.name, 
		tempAcct.surname, tempAcct.balance);	
	}
	printf("\n--------------------------------------------------------------------------");

	fclose(fp);	//close the file
}

void printTransaction() {
	
	FILE * fp = fopen(TRANSACTIONS_FILE_NAME, "r");
	
	if(fp == NULL){	//could not open
		puts("Error opening the file!");
		exit(1);
	}

	Transaction trans;

	printf("\n\n\nTRANSACTIONS from file %s\n\n\n", TRANSACTIONS_FILE_NAME);
	printf("-------------------------------TRANSACTIONS-------------------------------");	
	while ( fscanf(fp, "%d%d", &trans.number, &trans.amount) != EOF){
		printf("\n%5d\t%7d\n", trans.number, trans.amount);
	}
	printf("--------------------------------------------------------------------------");

	fclose(fp);	//close the file
}

void printAccounts(Account extraAcct[], int size){

	printf("---------------------------------ACCOUNTS---------------------------------");
	int x;
	for(x = 0; x < size; ++x) {
		printf("\n%5d\t%20s\t%20s\t%7d\n", extraAcct[x].number, extraAcct[x].name, extraAcct[x].surname, extraAcct[x].balance);		
	}
	printf("--------------------------------------------------------------------------");
}

void printTransactions(Transaction extraTrans[], int size){

	printf("-------------------------------TRANSACTIONS-------------------------------");	
	int x;
	for(x = 0; x < size; ++x) {		
		printf("\n%5d\t%7d\n", extraTrans[x].number, extraTrans[x].amount);		
	}
	printf("--------------------------------------------------------------------------");
}

int getAccountsSize(){

	FILE* fp = fopen(ACCOUNTS_FILE_NAME, "r");
	if (fp == NULL){
		puts("Error opening the file!");
		exit(1);
	}

	size_t count_a = 0;
	Account acct;
	
	while(fscanf(fp, "%d%s%s%d", 
		&acct.number, acct.name, acct.surname, &acct.balance) != EOF)
		++count_a;
	
	fclose(fp);
	return count_a;
}

int getTransactionsSize(){

	FILE* fp = fopen(TRANSACTIONS_FILE_NAME, "r");

	if (fp == NULL){
		puts("Error opening the file!");
		exit(1);
	}

	size_t count_t = 0;
	Transaction t;

	while(fscanf(fp, "%d%d", &t.number, &t.amount) != EOF) 
		++count_t;	
	
	fclose(fp);
	return count_t;
}

void readAccounts(Account * accounts, int size){
	
	FILE* fp = fopen(ACCOUNTS_FILE_NAME, "r");
	if (fp == NULL){
		puts("Error opening the file!");
		exit(1);
	}	
	
	int i;
	for(i=0; i < size; ++i) {		
		fscanf(fp, "%d%s%s%d", 
		&accounts[i].number, 
		accounts[i].name, 
		accounts[i].surname, 
		&accounts[i].balance);
	}
	
	fclose(fp);	//close the file
}

void readTransactions(Transaction* transactions, int size) {
	
	FILE* fp = fopen(TRANSACTIONS_FILE_NAME, "r");
	if (fp == NULL){
		puts("Error opening the file!");
		exit(1);
	}	

	int i;
	for(i=0; i < size; ++i) {	
		fscanf(fp, "%d%d", 
		&transactions[i].number, 
		&transactions[i].amount);
	}
	
	fclose(fp);
}
