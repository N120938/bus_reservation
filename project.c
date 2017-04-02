#include<stdio.h>
#include<string.h>
#include<malloc.h>
struct sd{
	char source[20];
	float source_time;
	char destination[];
	float destination_time;
	int amount;
};
struct node{
	int bus_no;
	int no_seats;
	struct sd *link;
	struct node *next;
};
struct node *start=NULL,*newnode,*ptr;
void display_all(struct node *newnode){
	while(newnode!=NULL){
		printf("%d %s %s %d ",newnode->bus_no,newnode->source,newnode->destination,newnode->amount);
		printf("%d %.2f %.2f\n",newnode->no_seats,newnode->source_time,newnode->destination_time);
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
	b=(b%4)*5;
	c=year%100;
	d=c/4;
	s=a[month+1]+date+b+c+d;
	e=s%7;
	e=e%2;
		ptr=start;
		while(ptr->next!=NULL){
			if(ptr->bus_no%2==e && e==0){
				char swap[20];
				strcpy(swap,ptr->source);
				strcpy(ptr->source,ptr->destination);
				strcpy(ptr->destination,swap);
			}
			else if(ptr->bus_no%2==e && e==1){
					char swap[20];
				strcpy(swap,ptr->source);
				strcpy(ptr->source,ptr->destination);
				strcpy(ptr->destination,swap);
			}
			ptr=ptr->next;
		}
}
main(){
	FILE *fp;
	int n,date,month,year;
	char source[20],destination[20],c;
	fp=fopen("busdetails.txt","r");
	if(fp==NULL)
		printf("cannot open file\n");
		newnode=(struct node*)malloc(sizeof(struct node));	
	while(fscanf(fp,"%d %s %s %d %f %f",&newnode->bus_no,newnode->source,newnode->destination,&newnode->amount,&newnode->source_time,&newnode->destination_time)!=EOF){	
		newnode->no_seats=40;
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
	}
	fclose(fp);
	display_all(start);
	do{
		printf("1.check Available buses between source and destination stations and timings\n2.reserve ticket for bus");
		printf("3.cancel ticket\n4.reservation status\n5.exit\nenter your choice\n");
		scanf("%d",&n);
		switch(n){
			case 1:
				printf("enter your source station\n");
				scanf("%s",source);
				printf("enter your destination station\n");
				scanf("%s",destination);
				printf("enter your date month,year");
				scanf("%d %d %d",&date,&month,&year);
				assign(date,month,year);
				ptr=start;
				while(ptr->next!=NULL){
					if(strcmp(ptr->source,source)==0&&strcmp(ptr->destination,destination)==0){
						printf("%d %s %s %d ",ptr->bus_no,ptr->source,ptr->destination,ptr->amount);
						printf("%d %.2f %.2f\n",ptr->no_seats,ptr->source_time,ptr->destination_time);
					}
					ptr=ptr->next;	
				}
				printf("do you want reserve ticket or not y/n \n");
				scanf("%c",&c);
				if(c=='y')
				printf("siva");
				else
				break;
			default:
				printf("not valid\n");
		}
	}while(n!=5);
}
