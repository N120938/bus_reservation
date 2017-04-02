#include<stdio.h>
#include<string.h>
#include<malloc.h>
struct sd{
	char destination[20];						//structure of sd,It stores the following data of every station
	float destination_time;		
	int amount;
	struct sd *link;
};
struct node{
	int bus_no;
	int no_seats;								//structure of node,It stores the information about bus
	char type[20];
	struct sd *prev;
	struct node *next;
};
struct user{
	int seat_no;
	char name[20];								//structure of user ,It stores the information about users of particular bus
	int age;
	char gender;
	struct user *next;
};
void display_all(struct node*);													//function declarations 
void assign(int,int,int);
struct node* readbus(struct node*);
void checkbus(struct node*,char source[20],char destination[20]);
int reserve(int); 
void seatsallocation(int,char filename[20]);
void cancelticket(char deleted[20],int seat_no);
int checkinteger(int);								//end of function declarations
struct node *start=NULL,*newnode,*ptr;
struct sd *insert=NULL,*insert_node,*preptr;									
int main(){
	FILE *fp;
	char source[20],destination[20],filename[20],filename2[20],deleted[20];
	int a[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	int n,t_date,t_month,t_year,date,month,year,t,bus_no,seat_no;
	do{
		start=readbus(start);
		printf("*---------------------------------------------------------------------*\n");
		printf("*       1.Information about all buses                                 *\n");
		printf("*       2.Available buses between one station to another station      *\n");					//following operations can be done
		printf("*       3.Reserve ticket for a particular bus                         *\n");										//by using this program						
		printf("*       4.Cancel ticket                                               *\n");
		printf("*       5.exit                                                        *\n");
		printf("*---------------------------------------------------------------------*\n");
		printf("Enter your choice\t-->");
		fflush(stdin);
		scanf("%d",&n);
		n=checkinteger(n);
		t=n;
		if(n==3)
		n=2;
			switch(n){																	//operations have done by using switch case
				case 1:
					display_all(start);												 
					break;
				case 2:	
					fflush(stdin);																		
					fp=fopen("todaydate.txt","r");									//read the today date by using file "todaydate.txt"
					if(fp==NULL)
						printf("can't open file\n");
					else
					while(fscanf(fp,"%d%d%d",&t_date,&t_month,&t_year)!=EOF);		//scan date,month,year by user to check availability
					printf("Enter your source station\t-->");
					scanf("%s",source);
					printf("Enter your destination\t-->");									//scan source,desetination stations by user
					scanf("%s",destination);
					int i=1;
					while(i){
						fflush(stdin);
						printf("Enter your date of journey\t-->");
						scanf("%d",&date);
						date=checkinteger(date);
						printf("Enter your month of journey\t-->");
						fflush(stdin);
						scanf("%d",&month);
						month=checkinteger(month);
						printf("Enter your year of journey\t-->");
						fflush(stdin);
						scanf("%d",&year);
						year=checkinteger(year);
						if(year%4==0)
						a[1]=29;
						if(month<=12&&month>0&&date<=a[month-1]){
							if(year==t_year){
								if(month==t_month){
									if(date>=t_date)
									i=0;
									else
									i=1;
								}
								else if(month>t_month)									//check the date if it is right or wrong
								i=0;
								else
								i=1;
							}
							else if(year>t_year)
							i=0;
							else
							i=1;
						}
					}
					assign(date,month,year);
					checkbus(start,source,destination);
					if(t==2)
					break;
				case 3:
					fflush(stdin);
					printf("Enter busno to reserve\t-->");
					fflush(stdin);
					scanf("%d",&bus_no);
					bus_no=checkinteger(bus_no);
					t=reserve(bus_no);
					if(t!=0){
					fp=fopen("filename.txt","w");
					fprintf(fp,"%d%d%d%d",date,month,year,bus_no);						//file created by "datemonthyearbusno.txt"
					fclose(fp);
					fp=fopen("filename.txt","r");
					fscanf(fp,"%s",filename2);
					fclose(fp);
					strcat(filename2,".txt");
					fp=fopen(filename2,"a");
					if(fp==NULL)
					printf("can't open file\n");
					fclose(fp);
					seatsallocation(bus_no,filename2);
					}
					break;
				case 4:
						printf("Enter your date of journey\t-->");
						fflush(stdin);
						scanf("%d",&date);
						date=checkinteger(date);
						printf("Enter your month of journey\t-->");
						fflush(stdin);
						scanf("%d",&month);
						month=checkinteger(month);
						printf("Enter your year of journey\t-->");
						fflush(stdin);
						scanf("%d",&year);
						year=checkinteger(year);
						printf("enter bus no\t-->");
						fflush(stdin);
						scanf("%d",&bus_no);
						printf("enter seat_no\t-->");
						scanf("%d",&seat_no);
						seat_no=checkinteger(seat_no);
						fp=fopen("filename.txt","w");
						fprintf(fp,"%d%d%d%d",date,month,year,bus_no);			//ticket is cancell on "datemonthyearbusno.txt"
						fclose(fp);
						fp=fopen("filename.txt","r");
						fscanf(fp,"%s",deleted);
						fclose(fp);;
						strcat(deleted,".txt");
						cancelticket(deleted,seat_no);
						printf("\n:::Ticket cancelled on your date:::\n\n");
					break;		
			}
			ptr=start;
			while(ptr!=NULL){
				ptr=start;
				start=start->next;											//free all node in the linkedlist which stores details of "busdetails.txt"
				free(ptr);
				ptr=ptr->next;
			}	
	}while(n!=5);
	return 0;
}
void display_all(struct node *newnode){						//displays all the information about all buses and its timmings
	while(newnode!=NULL){
		printf("******************************************\n");
		printf("bus_no\t%d\tno_seats::%d\t%s\n\n",newnode->bus_no,newnode->no_seats,newnode->type);
		preptr=newnode->prev;
		printf("%s time-->\t%.2f\n",preptr->destination,preptr->destination_time);
		preptr=preptr->link;
		while(preptr!=NULL){
		if(newnode->bus_no<3000){
			if(preptr->amount==0)
			printf("%s time-->\t%.2f\n\n",preptr->destination,preptr->destination_time);
			else
			printf("\t\t\t\t%s  time-->%.2f\tamount-->%d\n\n",preptr->destination,preptr->destination_time,preptr->amount);
			preptr=preptr->link;
		}
		else{
		printf("\t\t\t\t%s  time-->%.2f\tamount-->%d\n\n",preptr->destination,preptr->destination_time,preptr->amount);
		preptr=preptr->link;
		}
		}
		printf("\n");
		newnode=newnode->next;
	}
}
void assign(int date,int month,int year){
	int b,c,d,e,s;
	int a[12]={0,3,3,6,1,4,6,2,5,0,3,5};
	if(year%4==0){
		a[0]=2;
		a[1]=6;
	}
	b=year/100;
	b=(b%4)*5;											//assign the buses according to the date
	c=year%100;
	d=c/4;
	s=a[month+1]+date+b+c+d;
	e=s%7;
	e=e%2;
	if(e==1){
		ptr=start;
		while(ptr!=NULL){
			if(ptr->bus_no>3000){
				s=ptr->bus_no;
				ptr->bus_no=ptr->next->bus_no;
				ptr=ptr->next;
				ptr->bus_no=s;
			}
			ptr=ptr->next;
		}
	}
}
struct node* readbus(struct node *start){
	FILE *fp;
	char destination[20];
	fp=fopen("busdetails.txt","r");
	if(fp==NULL)																// I create two -dimensional linkedlist
		printf("cannot open file\n");
		newnode=(struct node*)malloc(sizeof(struct node));						//main linkedlist stores the information about buses
	while(fscanf(fp,"%d",&newnode->bus_no)!=EOF){	
		 newnode->no_seats=40;
		 fscanf(fp,"%s",newnode->type);
		insert_node=(struct sd*)malloc(sizeof(struct sd));
		fscanf(fp,"%s",destination);
		while(strcmp(destination,"end")!=0){
			strcpy(insert_node->destination,destination);
			fscanf(fp,"%f%d",&insert_node->destination_time,&insert_node->amount);		//sub linkedlist stroes the bus information
			fscanf(fp,"%s",destination);
			if(insert==NULL){
				insert=insert_node;
				insert->link=NULL;
				preptr=insert;
			}
			else{																//every nodes are inserted at the end
				preptr->link=insert_node;
				insert_node->link=NULL;
				preptr=insert_node;
			}
			insert_node=(struct sd*)malloc(sizeof(struct sd));
		}
		newnode->prev=insert;
		if(start==NULL){
			start=newnode;
			newnode->next=NULL;
			ptr=newnode;
		}
		else{
			ptr->next=newnode;
			newnode->next=NULL;
			ptr=newnode;
		}
		newnode=(struct node*)malloc(sizeof(struct node));
		insert=NULL;
	}
	fclose(fp);
	return start;
}
void checkbus(struct node *ptr,char source[20],char destination[20]){
	struct sd *check,*check2;
	FILE *fp;
	fp=fopen("available.txt","w");
	while(ptr!=NULL){
		preptr=ptr->prev;
		while(preptr!=NULL){
			if(strcmp(preptr->destination,source)==0){
				check=preptr->link;													//available buses between source and destination stations
				while(check!=NULL){
					if(check->amount==0){
						break;
					}
					if(strcmp(check->destination,destination)==0){
						if(ptr->bus_no>3000)
						fprintf(fp,"%d %d ",ptr->bus_no,ptr->no_seats);
						printf("*****************************************************\n");
						printf("bus_no\t%d\tno_seats::%d\t%s\n\n",ptr->bus_no,ptr->no_seats,ptr->type);
						printf("%s time-->\t%.2f\n\n",preptr->destination,preptr->destination_time);
						printf("\t\t\t\t%s  time-->%.2f\tamount-->%d\n\n",check->destination,check->destination_time,check->amount-preptr->amount);
						break;
					}
					check=check->link;		
				}
				preptr=check;
			}
			else
			preptr=preptr->link;	
		}
		ptr=ptr->next;
	}
	fclose(fp);
}
int reserve(int bus_no){
	FILE *fp;
	int busno,seats;
	fp=fopen("available.txt","r");
	while(fscanf(fp,"%d%d",&busno,&seats)!=EOF){
		if(bus_no==busno){
			if(seats>0){
				fclose(fp);										//check the status of buses and its seats if seats are available or not
				return bus_no;
			}
			else{
				printf("ALL seats are reserved\n");
				fclose(fp);
				return 0;
			}		
		}
	}
	fclose(fp);
	printf("Bus not found\n");
	return 0;
}
void seatsallocation(int busno,char filename2[20]){
	ptr=start;
	FILE *fp;
	int i=1,seat_no,age,flag=0,availableseats=40,kk=1;
	char gender[20],name[20],c;
	struct user *usernode=NULL,*userinsert,*username,*k=NULL;
	while(ptr!=NULL){
		if(ptr->bus_no==busno){
			fp=fopen(filename2,"r");
			if(fp==NULL){
			printf("cannot open file\n");
			fclose(fp);
			break;
			}	
			userinsert=(struct user*)malloc(sizeof(struct user));
			while(fscanf(fp,"%d %s %d %c",&userinsert->seat_no,userinsert->name,&userinsert->age,&userinsert->gender)!=EOF){
				printf("%d %s %d %c\n",userinsert->seat_no,userinsert->name,userinsert->age,userinsert->gender);
				availableseats--;
				if(usernode==NULL){
					usernode=userinsert;				//scan the user information into the linked list and performing operations
					userinsert->next=NULL;
				}
				else{
					username=usernode;
					while(username!=NULL){
						if(username->seat_no<userinsert->seat_no){
							k=username;
							if(username->next==NULL){
								username->next=userinsert;
								userinsert->next=NULL;
									break;
							}
							username=username->next;
						}
						else{
							if(k==NULL){								//user information sorted by seat_no
								userinsert->next=usernode;
								usernode=userinsert;
								break;
							}
							else if(k!=NULL){
								userinsert->next=username;
								k->next=userinsert;
								break;
							}
						}
					}
				}
				userinsert=(struct user*)malloc(sizeof(struct user));
			}
			fclose(fp);
			break;
			}
			ptr=ptr->next;	
		}
			printf("\navailableseats-->%d\n\n",availableseats);
			username=usernode;
			for(i=1;i<=40;i++){
				if(username!=NULL&&i==username->seat_no){
				printf("%d--%c",i,username->gender);
				username=username->next;	
				}
				else{
				printf("%d--",i);
				}												//print the seat_no and its status of requried bus 
				if(i%4==1||i%4==3)
					printf("\t");
				else if(i%4==2)
					printf("\t\t");
				else
					printf("\n");
			}
			printf("\n\n");
			fp=fopen(filename2,"a");
			do{
					kk=1;
				printf("Enter which seat_no you want\t-->");
				fflush(stdin);
				scanf("%d",&seat_no);
				seat_no=checkinteger(seat_no);
				username=usernode;
				while(username!=NULL){
					if(username->seat_no==seat_no){
						printf("Already reserved--");
						kk=0;														//check user information is correct or not
						break;
					}
					else if(seat_no>40){
						printf("invalid choice--");
						kk=0;													//check user information is correct or not
						break;					
					}
					username=username->next;
				}
			}while(kk!=1);
			printf("Enter Passenger name\t-->");
			fflush(stdin);
			scanf("%s",name);
			printf("ENter passenger age\t-->");
			fflush(stdin);
			scanf("%d",&age);
			age=checkinteger(age);
			while(age<0||age>120){
				printf("ENter passenger age\t-->");
				fflush(stdin);
				scanf("%d",&age);
				age=checkinteger(age);
			}
			do{																			//passenger can be reserved
				printf("Enter passener gender\t-->");
				fflush(stdin);
				fflush(stdin);
				scanf("%s",gender);
				gender[1]='\0';
				username=usernode;
					if(strcmp(gender,"m")==0||strcmp(gender,"M")==0||strcmp(gender,"f")==0||strcmp(gender,"F")==0){
						flag=0;
					}
					else{
						printf("correct your gender--");
						flag=1;
					}
			}while(flag==1);
			fprintf(fp,"%d %s %d %s\n",seat_no,name,age,gender);
			fclose(fp);															//conformation of reservation
			printf("\n:::your reservation is conformed:::\n");
			username=usernode;
			while(usernode!=NULL){
				username=usernode;
				usernode=usernode->next;										//free the linkedlist
				free(username);
			}
}
void cancelticket(char deleted[20],int seat_no){
	FILE *fp,*fs;
	int seatno,age;
	char gender,name[20];
	fp=fopen(deleted,"r");
	if(fp==NULL)
	printf("cannot open file\n");
	fs=fopen("temp.txt","w");
	if(fs==NULL)
	printf("cannot open file\n");
	while(fscanf(fp,"%d %s %d %c",&seatno,name,&age,&gender)!=EOF){				//writes the user information into the other file
		if(seatno!=seat_no)
		fprintf(fs,"%d %s %d %c\n",seatno,name,age,gender);
	}
	fclose(fs);
	fclose(fp);														//delete the original file
	strcpy(name,deleted);
	remove(deleted);												//renamed modified file into original file
	rename("temp.txt",name);
}
int checkinteger(int n){
	n=(int)n;
	fflush(stdin);
	if(n>0 && n<=10000)
		return n;
	do{
		printf("Invalid choice::re enter\t");
		fflush(stdin);
		scanf("%d",&n);
		if(n>0&&n<=10000)
		return n;
	}while(1);
}
