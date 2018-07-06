
/*
 
 Solving Capacitated Vehicle Routing Problem (CVRP) by Stochastic Hill climbing, Simulated Annealing, and Tabu Search
 In the main function [end of this file], the selected method to solve the problem can be used
 
 Note that the uploaded file in the current repository called  "myDT.h" should be added to the project
 file named "Ank_80.txt" which also can be found in the current repository can be used as test data


*/


//#include "stdafx.h"
#include "myDT.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <Windows.h>


int tw1,tw2;



using namespace std;

int countCustomer;
int tempdemands[MAXCITIES];
int tempdemandsCust[MAXCITIES];

void readInput()
{
	FILE *myfile;		
	if((myfile=fopen(FileName,"r"))==NULL)
		printf( "File could not be opened\n" );
	else
	{
		//TO DO
		//NUMTRUCKS=8;//count Truck
		fscanf(myfile,"%d",&countCustomer);
		fscanf(myfile,"%d",&cap);
		//coordinates[0].x=coordinates[0].y=0;
		int tmp;
		for(int i=0;i < countCustomer;i++)         //@@@@@@  <  na <=
		{		
			fscanf(myfile,"%d%lf%lf",&tmp,&coordinates[i].x,&coordinates[i].y); //tmp
		}
		//demands[0]=-1;//depot demand
		int j;
		for(int i=0;i<countCustomer;i++)
		{		
			fscanf(myfile,"%d%d",&j,&demands[i]);
		}		
	}			
	fclose(myfile);
}

void sort()//Counting Sort O(n)
{
	int temp[MAXCAP];
	for(int i=1;i<cap;i++) temp[i]=0;
	for(int i=0;i<countCustomer;i++) temp[demands[i]]++;
	for(int i=2;i<cap;i++) temp[i]+=temp[i-1];
	for(int i=countCustomer-1;i>0;i--)
	{
		tempdemands[temp[demands[i]]]=demands[i];
		tempdemandsCust[temp[demands[i]]]=i+1;
		temp[demands[i]]--;
	}

/*	for(int i=1;i<countCustomer;i++){
		tempdemands[i]=*/
	//}


}
double EuclidianDistance(int a,int b)
{
	return (double)(sqrt((coordinates[a].x-coordinates[b].x)*(coordinates[a].x-coordinates[b].x)+(coordinates[a].y-coordinates[b].y)*(coordinates[a].y-coordinates[b].y)));
}
bool assignCustomer[MAXCITIES]={false};
bool remaincustomer()//check all customers covers   
{
	for(int i=0;i<countCustomer;i++)
		if(assignCustomer[i]==false)
			return true;
	return false;
}
int nextcustomer(int trkremaincap)//maximum number(demand) little than equal remainCap && corresponding customer didnt assign
{
	for(int i=countCustomer-1;i>=0;i--)
		if(tempdemands[i]<=trkremaincap && assignCustomer[i]==false)
			return i;
	return -1;
}
bool initialize()
{
	assignCustomer[0]=true;  //@@ man
	sort();		
	for(int i=0;i<NUMTRUCKS;i++)
	{
		struct Truck trk;
		trk.remindedCap=cap;
		trk.numMembers=0;
		trk.totalCost=0;
		//
		int c=0,nc=0;		
		for(int j=0;j<MAXTRUCKCITIES;j++)
			trk.members[j]=0;		
		while((nc=nextcustomer(trk.remindedCap))!=-1)
		{
			trk.remindedCap-=tempdemands[nc];
			if(tempdemandsCust[nc]==0)
				trk.members[c]=0;//@@ 1
			else
				trk.members[c]=tempdemandsCust[nc]-1;//@@ -1  man
			assignCustomer[nc]=true;
			trk.numMembers++;
			if(c==0)			
				trk.totalCost+=EuclidianDistance(trk.members[c],0);
			else
				trk.totalCost+=EuclidianDistance(trk.members[c],trk.members[c-1]);
			c++;			
		}
		//
		if(c!=0)
			trk.totalCost+=EuclidianDistance(trk.members[c-1],0);
		solution[i]=trk;
		//best[i]=trk;
	}
	solutionCost=retCost(solution);
	return (!remaincustomer());
}
//
int nextcustomer2(int trkremaincap)
{
	int r;
	for(int i=countCustomer+1;i>=0;i--)//@@
	{	
		r=(int)rand()%countCustomer;
		if(tempdemands[r]<=trkremaincap && assignCustomer[r]==false)	
			return r;
	}
	return -1;
}
bool initialize2()
{
	
	sort();	
	do{
		for(int j=0;j<MAXCITIES;j++)
			assignCustomer[j]=false;
	assignCustomer[0]=true;
	for(int i=0;i<NUMTRUCKS;i++)
	{
		struct Truck trk;
		trk.remindedCap=cap;
		trk.numMembers=0;
		trk.totalCost=0;
		//
		int c=0,nc=0;		
		for(int j=0;j<MAXTRUCKCITIES;j++)
			trk.members[j]=0;		
		while((nc=nextcustomer2(trk.remindedCap))!=-1)
		{
			trk.remindedCap-=tempdemands[nc];
			if(tempdemandsCust[nc]==0)
				trk.members[c]=0;//@@ 1
			else
				trk.members[c]=tempdemandsCust[nc]-1;			//@@
			assignCustomer[nc]=true;
			trk.numMembers++;
			if(c==0)
				trk.totalCost+=EuclidianDistance(trk.members[c],0);
			else
				trk.totalCost+=EuclidianDistance(trk.members[c],trk.members[c-1]);
			c++;			
		}
		//
		if(c!=0)
			trk.totalCost+=EuclidianDistance(trk.members[c-1],0);
		solution[i]=trk;
		//best[i]=trk;
	}
	solutionCost=retCost(solution);
	
	}while(remaincustomer());
	return true;
}
//
//Truck newSolution[MAXTRUCKS];
void tweak1()
{		
	tw1++;
	for(int ii=0;ii<NUMTRUCKS;ii++)
	{
		newSolution[ii].clone(solution[ii]);
	}
	
	int r1=0,r2=0,t;
	int i;//kamiun ke tush 2ta noghte avaz she.
	for(int j=0;j<NUMTRUCKS;j++){ //@@  shayad yeki mishod
		i=rand()%NUMTRUCKS;
		if(newSolution[i].numMembers>1)
			break;
		if(j==NUMTRUCKS-1)
			return;
	}
	//for(int i=0;i<NUMTRUCKS;i++)
	//{		
	while(r1==r2){
		r1=(int)rand()%(newSolution[i].numMembers);
		r2=(int)rand()%(newSolution[i].numMembers);
	}
		if(r1==0)
			newSolution[i].totalCost-=EuclidianDistance(0,newSolution[i].members[r1]);
		else
			newSolution[i].totalCost-=EuclidianDistance(newSolution[i].members[r1-1],newSolution[i].members[r1]);
		if(r1==newSolution[i].numMembers-1)
			newSolution[i].totalCost-=EuclidianDistance(newSolution[i].members[r1],0);
		else
			newSolution[i].totalCost-=EuclidianDistance(newSolution[i].members[r1],newSolution[i].members[r1+1]);
		if(r2==0)
			newSolution[i].totalCost-=EuclidianDistance(0,newSolution[i].members[r2]);
		else
			newSolution[i].totalCost-=EuclidianDistance(newSolution[i].members[r2-1],newSolution[i].members[r2]);
		if(r2==newSolution[i].numMembers-1)
			newSolution[i].totalCost-=EuclidianDistance(newSolution[i].members[r2],0);
		else
			newSolution[i].totalCost-=EuclidianDistance(newSolution[i].members[r2],newSolution[i].members[r2+1]);
		t=newSolution[i].members[r1];
		newSolution[i].members[r1]=newSolution[i].members[r2];
		newSolution[i].members[r2]=t;
		if(r1==0)
			newSolution[i].totalCost+=EuclidianDistance(0,newSolution[i].members[r1]);
		else			
			newSolution[i].totalCost+=EuclidianDistance(newSolution[i].members[r1-1],newSolution[i].members[r1]);
		if(r1==newSolution[i].numMembers-1)
			newSolution[i].totalCost+=EuclidianDistance(newSolution[i].members[r1],0);
		else
			newSolution[i].totalCost+=EuclidianDistance(newSolution[i].members[r1],newSolution[i].members[r1+1]);
		if(r2==0)
			newSolution[i].totalCost+=EuclidianDistance(0,newSolution[i].members[r2]);
		else
			newSolution[i].totalCost+=EuclidianDistance(newSolution[i].members[r2-1],newSolution[i].members[r2]);
		if(r2==newSolution[i].numMembers-1)
			newSolution[i].totalCost+=EuclidianDistance(newSolution[i].members[r2],0);
		else
			newSolution[i].totalCost+=EuclidianDistance(newSolution[i].members[r2],newSolution[i].members[r2+1]);
	
}



void tweak1Tabu()
{		
	tw1++;
	for(int ii=0;ii<NUMTRUCKS;ii++)
	{
		newSolution2[ii].clone(solution[ii]);
	}
	
	int r1=0,r2=0,t;
	int i;//kamiun ke tush 2ta noghte avaz she.
	for(int j=0;j<NUMTRUCKS;j++){ //@@  shayad yeki mishod
		i=rand()%NUMTRUCKS;
		if(newSolution2[i].numMembers>1)
			break;
		if(j==NUMTRUCKS-1)
			return;
	}
	//for(int i=0;i<NUMTRUCKS;i++)
	//{		
	while(r1==r2){
		r1=(int)rand()%(newSolution2[i].numMembers);
		r2=(int)rand()%(newSolution2[i].numMembers);
	}
		if(r1==0)
			newSolution2[i].totalCost-=EuclidianDistance(0,newSolution2[i].members[r1]);
		else
			newSolution2[i].totalCost-=EuclidianDistance(newSolution2[i].members[r1-1],newSolution2[i].members[r1]);
		if(r1==newSolution2[i].numMembers-1)
			newSolution2[i].totalCost-=EuclidianDistance(newSolution2[i].members[r1],0);
		else
			newSolution2[i].totalCost-=EuclidianDistance(newSolution2[i].members[r1],newSolution2[i].members[r1+1]);
		if(r2==0)
			newSolution2[i].totalCost-=EuclidianDistance(0,newSolution2[i].members[r2]);
		else
			newSolution2[i].totalCost-=EuclidianDistance(newSolution2[i].members[r2-1],newSolution2[i].members[r2]);
		if(r2==newSolution2[i].numMembers-1)
			newSolution2[i].totalCost-=EuclidianDistance(newSolution2[i].members[r2],0);
		else
			newSolution2[i].totalCost-=EuclidianDistance(newSolution2[i].members[r2],newSolution2[i].members[r2+1]);
		t=newSolution2[i].members[r1];
		newSolution2[i].members[r1]=newSolution2[i].members[r2];
		newSolution2[i].members[r2]=t;
		if(r1==0)
			newSolution2[i].totalCost+=EuclidianDistance(0,newSolution2[i].members[r1]);
		else			
			newSolution2[i].totalCost+=EuclidianDistance(newSolution2[i].members[r1-1],newSolution2[i].members[r1]);
		if(r1==newSolution2[i].numMembers-1)
			newSolution2[i].totalCost+=EuclidianDistance(newSolution2[i].members[r1],0);
		else
			newSolution2[i].totalCost+=EuclidianDistance(newSolution2[i].members[r1],newSolution2[i].members[r1+1]);
		if(r2==0)
			newSolution2[i].totalCost+=EuclidianDistance(0,newSolution2[i].members[r2]);
		else
			newSolution2[i].totalCost+=EuclidianDistance(newSolution2[i].members[r2-1],newSolution2[i].members[r2]);
		if(r2==newSolution2[i].numMembers-1)
			newSolution2[i].totalCost+=EuclidianDistance(newSolution2[i].members[r2],0);
		else
			newSolution2[i].totalCost+=EuclidianDistance(newSolution2[i].members[r2],newSolution2[i].members[r2+1]);
	
}



bool assessment(Truck solu1[],Truck solu2[])//if cost first solution <= cost second solution return true
{
	double cost1=0,cost2=0;
	for(int i=0;i<NUMTRUCKS;i++)
	{
		cost1+=solu1[i].totalCost;
		cost2+=solu2[i].totalCost;
	}
	if(cost1<cost2)
		return true;
	return false;
}
//
int calcRegion(int point){
	if(coordinates[point].x==0 && coordinates[point].y==0)
		return 1;
	if(coordinates[point].x==0 && coordinates[point].y>0)
		return 2;
	if(coordinates[point].x>0 && coordinates[point].y>=0)
		return 3;
	if(coordinates[point].x>0 && coordinates[point].y<0)
		return 4;
	if(coordinates[point].x==0 && coordinates[point].y<0)
		return 5;
	if(coordinates[point].x<0 && coordinates[point].y<0)
		return 6;
	if(coordinates[point].x<0 && coordinates[point].y>=0)
		return 7;	
	cout<<"error1\n";
}

bool cmpAngle(int point1,int point2){
	int pos1=calcRegion(point1);
	int pos2=calcRegion(point2);

	if(pos1 < pos2)
		return true;
	if(pos1 > pos2)
		return false;
	
	if(pos1==1)
		return true;
	
	if(pos1==2)
		return coordinates[pos1].y < coordinates[pos2].y;

	if(pos1==3)
		return coordinates[pos1].y / coordinates[pos1].x >= coordinates[pos2].y / coordinates[pos2].x;

	if(pos1==4)
		return coordinates[pos1].y / coordinates[pos1].x >= coordinates[pos2].y / coordinates[pos2].x;

	if(pos1==5)
		return coordinates[pos1].y < coordinates[pos2].y ;

	if(pos1==6)
		return coordinates[pos1].y / coordinates[pos1].x >= coordinates[pos2].y / coordinates[pos2].x;

	if(pos1==7)
		return coordinates[pos1].y / coordinates[pos1].x >= coordinates[pos2].y / coordinates[pos2].x;
		

}

bool cmpX(int point1,int point2){
	return coordinates[point1].x < coordinates[point2].x;
}
bool cmpY(int point1 , int point2){
	return coordinates[point1].y < coordinates[point2].y;
}

void tweak1AngleSort(Truck * sol,int truckNum){
	sort(sol[truckNum].members,sol[truckNum].members+sol[truckNum].numMembers,cmpAngle);
	sol[truckNum].totalCost=sol[truckNum].calcTotalCost();
}

void tweak1xSort(Truck * sol,int truckNum){
	sort(sol[truckNum].members,sol[truckNum].members+sol[truckNum].numMembers,cmpX);
	sol[truckNum].totalCost=sol[truckNum].calcTotalCost();
}

void tweak1ySort(Truck * sol,int truckNum){
	sort(sol[truckNum].members,sol[truckNum].members+sol[truckNum].numMembers,cmpY);
	sol[truckNum].totalCost=sol[truckNum].calcTotalCost();
}




bool changePos(Truck* sol,int truckNum1,int s1,int f1,int truckNum2,int s2,int f2){//ta khune ghabl az f1 o f2

	/*if(truckNum1==6 && truckNum2==3 && s1==0 && s2==1)
		int x=0;*/

	int len1 = s1<f1 ? f1-s1 : sol[truckNum1].numMembers-s1 + f1;
	int len2 = s2<f2 ? f2-s2 : sol[truckNum2].numMembers-s2 + f2;

	int cnt1=s1,cnt2=s2,tmp;

	if(len1==len2){
		while(cnt2!=f2 ){
			tmp=sol[truckNum1].members[cnt1];
			sol[truckNum1].members[cnt1]=sol[truckNum2].members[cnt2];
			sol[truckNum2].members[cnt2]=tmp;
			//total cost joda hesab she =>   mishe haminja gozosht,kheili farghi nadare
			sol[truckNum1].remindedCap+= demands[sol[truckNum2].members[cnt2]]-demands[sol[truckNum1].members[cnt1]];
			sol[truckNum2].remindedCap+= demands[sol[truckNum1].members[cnt1]]-demands[sol[truckNum2].members[cnt2]];
			
			cnt1++;
			cnt2++;
			if(cnt1==sol[truckNum1].numMembers)
				cnt1=0;
			if(cnt2==sol[truckNum2].numMembers)
				cnt2=0;
		}
	}

	else if(len1>len2){
		while(cnt2!=f2 ){
			tmp=sol[truckNum1].members[cnt1];
			sol[truckNum1].members[cnt1]=sol[truckNum2].members[cnt2];
			sol[truckNum2].members[cnt2]=tmp;
			//total cost joda hesab she =>   mishe haminja gozosht,kheili farghi nadare
			sol[truckNum1].remindedCap+= demands[sol[truckNum2].members[cnt2]]-demands[sol[truckNum1].members[cnt1]];
			sol[truckNum2].remindedCap+= demands[sol[truckNum1].members[cnt1]]-demands[sol[truckNum2].members[cnt2]];
			
			cnt1++;
			cnt2++;
			if(cnt1==sol[truckNum1].numMembers)
				cnt1=0;
			if(cnt2==sol[truckNum2].numMembers)
				cnt2=0;
		}
		cnt2=sol[truckNum2].numMembers;
		int tmp=cnt1;
		while(cnt1!=f1){
			sol[truckNum2].members[cnt2++]=sol[truckNum1].members[cnt1++];
			sol[truckNum2].remindedCap -= demands[sol[truckNum1].members[cnt1-1]]; //! -1
			sol[truckNum1].remindedCap += demands[sol[truckNum1].members[cnt1-1]]; 
			if(cnt1==sol[truckNum1].numMembers)//cnt2 dare az akhar por mikone +
				cnt1=0;
		}
		sol[truckNum2].numMembers=cnt2;
		if(cnt1==0)
			;
		else if(tmp<cnt1){
			int firstTmp=tmp;
			while(1){//cnt1!=firstTmp){
				sol[truckNum1].members[tmp++]=sol[truckNum1].members[cnt1++];
				if(cnt1==sol[truckNum1].numMembers)
					break;
			}
		}
		else if(tmp>cnt1){
			for(int i=cnt1;i<tmp;i++){
				sol[truckNum1].members[i-cnt1]=sol[truckNum1].members[i];
			}
		}
		else{
			cout<<"error2\n";
		}
		sol[truckNum1].numMembers += len2-len1;
		

	}
	else{//len2>len1

		while(cnt1!=f1 ){
			tmp=sol[truckNum1].members[cnt1];
			sol[truckNum1].members[cnt1]=sol[truckNum2].members[cnt2];
			sol[truckNum2].members[cnt2]=tmp;
			//total cost joda hesab she =>   mishe haminja gozosht,kheili farghi nadare
			sol[truckNum1].remindedCap+= demands[sol[truckNum2].members[cnt2]]-demands[sol[truckNum1].members[cnt1]];
			sol[truckNum2].remindedCap+= demands[sol[truckNum1].members[cnt1]]-demands[sol[truckNum2].members[cnt2]];
			
			cnt1++;
			cnt2++;
			if(cnt1==sol[truckNum1].numMembers)
				cnt1=0;
			if(cnt2==sol[truckNum2].numMembers)
				cnt2=0;
		}
		cnt1=sol[truckNum1].numMembers;
		int tmp=cnt2;
		while(cnt2!=f2){
			sol[truckNum1].members[cnt1++]=sol[truckNum2].members[cnt2++];
			sol[truckNum1].remindedCap -= demands[sol[truckNum2].members[cnt2-1]]; //! -1
			sol[truckNum2].remindedCap += demands[sol[truckNum2].members[cnt2-1]];
			if(cnt2==sol[truckNum2].numMembers)//cnt1 dare az akhar por mikone +
				cnt2=0;
		}
		sol[truckNum1].numMembers=cnt1;
		if(cnt2==0)
			;
		else if(tmp<cnt2){
			int firstTmp=tmp;
			while(1){//cnt1!=firstTmp){
				sol[truckNum2].members[tmp++]=sol[truckNum2].members[cnt2++];
				if(cnt2==sol[truckNum2].numMembers)
					break;
			}
		}
		else if(tmp>cnt2){
			for(int i=cnt2;i<tmp;i++){
				sol[truckNum2].members[i-cnt2]=sol[truckNum2].members[i];
			}
		}
		else{
			cout<<"error3\n";
		}
		sol[truckNum2].numMembers += len1-len2;

	}
	return true;	
}


bool tweak2(Truck * sol,int truckNum1,int truckNum2){ //return: javab peida karde ya na
	tw2++;
	int startPoint1;
	int startPoint2;
	int sum1;
	int sum2;
	int cnt1,cnt2;

	bool find=false;

	for(int i=0;i< (sol[truckNum1].numMembers*sol[truckNum2].numMembers)/4 || i<sol[truckNum1].numMembers+sol[truckNum2].numMembers ; i++){ //@@@@ tedade dafaate tekarar

		startPoint1=rand()%sol[truckNum1].numMembers;  // shru az 2ta noghteye dige
		startPoint2=rand()%sol[truckNum2].numMembers;
		cnt1=startPoint1+1 ;
		cnt2=startPoint2+1;
		cnt1 %= sol[truckNum1].numMembers;
		cnt2 %= sol[truckNum2].numMembers;
		sum1=demands[sol[truckNum1].members[startPoint1]];
		sum2=demands[sol[truckNum2].members[startPoint2]];

		bool valid=true;
		while( sum2 + sol[truckNum2].remindedCap < sum1 || sum1 + sol[truckNum1].remindedCap < sum2 ){
			//if(sum2 + sol[truckNum2].remindedCap > sum1 + sol[truckNum1].remindedCap){
			if(sum1 + sol[truckNum1].remindedCap < sum2 ){
				if(cnt1==startPoint1){
					valid=false;
					break;
				}
				sum1+=demands[sol[truckNum1].members[cnt1++]];
				cnt1 %= sol[truckNum1].numMembers;
			}
			else{
				if(cnt2==startPoint2){
					valid=false;
					break;
				}
				sum2+=demands[sol[truckNum2].members[cnt2++]];
				cnt2 %= sol[truckNum2].numMembers;
			}
		}//while() tu

		if(valid){//@@@@@@@@@@@@@   int alamat behtaree || bashe va tu tweak bayad handle she
			if(cnt1!=startPoint1 && cnt2!=startPoint2){  //@@  2ta dor kamel ba ham avaz nemishan
				changePos(sol,truckNum1,startPoint1,cnt1,truckNum2,startPoint2,cnt2);
				sol[truckNum1].totalCost=sol[truckNum1].calcTotalCost();
				sol[truckNum2].totalCost=sol[truckNum2].calcTotalCost();
				return true;
			}
		}

		


	}
	return false;
}
//




void show(Truck * sol)
{
	cout<<"\n==================Best Solution====================\n";
	for(int i=0;i<NUMTRUCKS;i++)
	{
		printf("number of cust:%d\n",sol[i].numMembers);
		printf("remain Cap:%d\n",sol[i].remindedCap);
		printf("total cost:%lf\n",sol[i].totalCost);
		printf("Path:");
		for(int j=0;j<sol[i].numMembers;j++)
			printf("%d,",sol[i].members[j]);
		printf("\n\n");
	}
	cout<<"result cost: "<<retCost(sol)<<endl;
	cout<<"tw1: "<<tw1<<"    tw2:"<<tw2<<endl;
}


bool tweak(int current,int end)
{
	double level = (double)current/end; //@@ 
	double wTweak3=4+0.5*level;
	double wTweak2=5+1.5*level;
	double wTweak1=3+3*level;
	double wSort=0.5+2*level;

	double sum=wTweak1+wTweak2+wTweak3+wSort;
	double rnd=rand()%(int)(sum*1000);
	rnd/=1000;

	if(rnd<=wTweak3){
		//tweak3();
		for(int i=0;i<NUMTRUCKS;i++)
			newSolution[i].clone(solution[i]);
		int r1=rand()%NUMTRUCKS;
		int r2=rand()%NUMTRUCKS;
		if(NUMTRUCKS<2)//@@
			return false;
		while(r1==r2)
			r2=rand()%NUMTRUCKS;
		tweak2(newSolution,r1,r2);
	}
	else if(rnd<=wTweak3+wTweak2){
		for(int i=0;i<NUMTRUCKS;i++)
			newSolution[i].clone(solution[i]);
		int r1=rand()%NUMTRUCKS;
		int r2=rand()%NUMTRUCKS;
		while(r1==r2)
			r2=rand()%NUMTRUCKS;
		tweak2(newSolution,r1,r2);
	}
	else if(rnd<=wTweak3+wTweak2+wTweak1){
		tweak1();
	}
	else{
		//tweakSort();
		/*for(int i=0;i<NUMTRUCKS;i++)
			newSolution[i].clone(solution[i]);
		tweak1AngleSort(newSolution,rand()%NUMTRUCKS);*/
	}

	return true;
}




bool tweakTabu(int current,int end)
{
	double level = (double)current/end; //@@ 
	double wTweak3=4+0.5*level;
	double wTweak2=5+1.5*level;
	double wTweak1=3+3*level;
	double wSort=0.5+2*level;

	double sum=wTweak1+wTweak2+wTweak3;//+wSort;
	double rnd=rand()%(int)(sum*1000);
	rnd/=1000;

	if(rnd<=wTweak3){
		//tweak3();
		for(int i=0;i<NUMTRUCKS;i++)
			newSolution2[i].clone(solution[i]);
		int r1=rand()%NUMTRUCKS;
		int r2=rand()%NUMTRUCKS;
		if(NUMTRUCKS<2)//@@
			return false;
		while(r1==r2)
			r2=rand()%NUMTRUCKS;
		tweak2(newSolution2,r1,r2);
	}
	else if(rnd<=wTweak3+wTweak2){
		for(int i=0;i<NUMTRUCKS;i++)
			newSolution2[i].clone(solution[i]);
		int r1=rand()%NUMTRUCKS;
		int r2=rand()%NUMTRUCKS;
		while(r1==r2)
			r2=rand()%NUMTRUCKS;
		tweak2(newSolution2,r1,r2);
	}
	else if(rnd<=wTweak3+wTweak2+wTweak1){
		tweak1Tabu();
	}
	else{
		//tweakSort();
		/*for(int i=0;i<NUMTRUCKS;i++)
			newSolution2[i].clone(solution[i]);
		tweak1AngleSort(newSolution2,rand()%NUMTRUCKS);*/
	}

	return true;
}



double retCost(Truck sol[])
{
	double totalcost=0;
	for(int i=0;i<NUMTRUCKS;i++)
		totalcost+=sol[i].totalCost;
	return totalcost;
}


bool checkValid(Truck * sol){
	int bood[10000+1]={};
	for(int i=0;i<NUMTRUCKS;i++){
		for(int j=0;j<sol[i].numMembers;j++)
			bood[sol[i].members[j]]++;
	}

	for(int i=1;i<countCustomer;i++){
		if(bood[i]!=1)
			return false;
	}
	return true;

}

bool iterHill(int numStart)
{
	//if(!initialize())
		initialize2();
	int iter=0,countStart=0;
	for(int i=0;i<NUMTRUCKS;i++){
		best[i].clone(solution[i]);
		bestCost=solutionCost;
	}
	do{			
		countStart++;
		//cout<<"start: "<<countStart<<endl;
		iter=0;
		do{
			iter++;
			/*if(iter==258 && countStart==2)
				int zz=0;*/
			tweak(iter,MAXITERATION/numStart);
			
			/*if(!checkValid(newSolution))
			{
				int as=0;
			}*/


			/*for(int i=0;i<NUMTRUCKS;i++)
				newSolution[i].*/
			if(retCost(newSolution)<solutionCost){
				for(int i=0;i<NUMTRUCKS;i++)				
					solution[i].clone(newSolution[i]);
				solutionCost=retCost(newSolution);
				//cout<<"tweak khub   "<<solutionCost<<endl;

				
			}
		}while(solutionCost>OPTIMALVALUE && iter < MAXITERATION/numStart);
		if(solutionCost<bestCost){
			for(int i=0;i<NUMTRUCKS;i++)
				best[i].clone(solution[i]);
			bestCost=retCost(solution);
			//show();
			//cout<<"find    "<<bestCost<<endl;
		}
		initialize2();
	}while(countStart<numStart && bestCost>OPTIMALVALUE);

	show(best);
return true;

}


bool stcHill(){   ///ba  best
	/*if(!initialize())
		initialize2();*/
	initialize2();

	int newCost;

	int iter=0;
	for(int i=0;i<NUMTRUCKS;i++)
		best[i].clone(solution[i]);
	bestCost=solutionCost;

	do{			
		iter++;

		/*if(iter%100000==0)
			cout<<iter<<endl;*/
		
		tweak(iter,MAXITERATION);

		newCost=retCost(newSolution);

		double prb=1/(1+exp((solutionCost-newCost)/TSHL));

		double rnd=rand()% 10000;
		rnd/= 10000;
		if(prb<=rnd){
			for(int i=0;i<NUMTRUCKS;i++)
				solution[i].clone(newSolution[i]);
			solutionCost=retCost(newSolution);
			//cout<<"find    "<<solutionCost<<endl;
			if(solutionCost<bestCost){
				for(int i=0;i<NUMTRUCKS;i++)
					best[i].clone(solution[i]);
				bestCost=solutionCost;
			}
		}

	}while(iter<MAXITERATION && bestCost > OPTIMALVALUE);
	show(best);

return true;
}


int updateT(int iter,int t)
{
	return (TSAMIN + (iter/MAXITERATION )*(iter/MAXITERATION)*(TSAMAX-TSAMIN));
}


bool simulatedAnnealing(int numChangeT)
{
	int totalIt=0;
	if(!initialize())
		initialize2();
	int iter=0,countChange=0;
	double t=TSAMIN,newCost;

	for(int i=0;i<NUMTRUCKS;i++){
		best[i].clone(solution[i]);
		bestCost=solutionCost;
	}
	do{			
		countChange++;
		//cout<<"start: "<<countStart<<endl;
		iter=0;
		do{
			iter++;
			totalIt++;
			/*if(iter==258 && countStart==2)
				int zz=0;*/
			tweak(totalIt,MAXITERATION);  //@@@@@
			
			/*if(!checkValid(newSolution))
			{
				int as=0;
			}*/
			/*for(int i=0;i<NUMTRUCKS;i++)
				newSolution[i].*/
			newCost=retCost(newSolution);
			if(newCost<solutionCost){
				for(int i=0;i<NUMTRUCKS;i++)				
					solution[i].clone(newSolution[i]);
				solutionCost=newCost;
				if(solutionCost < bestCost){
					for(int i=0;i<NUMTRUCKS;i++)
						best[i].clone(solution[i]);
					bestCost=solutionCost;
					//show();
					//cout<<"find    "<<bestCost<<endl;
				}
				//cout<<"tweak khub   "<<solutionCost<<endl;
			}
			//halate == moshkel ijad mishe.(loop)
			else if(newCost < solutionCost)
			{
				double prb=exp((newCost-solutionCost)/t);
				//cout<<prb<<endl;

				double rnd=rand()% 10000;
				rnd/= 10000;
				if(prb<=rnd){
					for(int i=0;i<NUMTRUCKS;i++)
						solution[i].clone(newSolution[i]);
					solutionCost=newCost;
				}

			}				

		}while(solutionCost > OPTIMALVALUE && iter < MAXITERATION/numChangeT);
		t=updateT(totalIt,t);
		//initialize2();
	}while( totalIt < MAXITERATION && bestCost > OPTIMALVALUE);

	show(best);
return true;

}








double goal;

bool tabuSearch(int numHillit)//n
{
	int totalIt=0;
	//if(!initialize())
		initialize2();
	int iter=0,countHill=0;
	double newCost,new2Cost;

	for(int i=0;i<NUMTRUCKS;i++){
		best[i].clone(solution[i]);
		bestCost=solutionCost;
	}

	tl.insert(best);

	do{			
		//cout<<"start: "<<countStart<<endl;
		iter++;
		tweak(iter,MAXITERATION);  //@@@@@
		newCost=retCost(newSolution);

		bool newsToList=tl.search(newSolution);
		countHill=0;
		do{
			/*if(iter==258 && countStart==2)
				int zz=0;*/
			countHill++;
			
			tweakTabu(iter,MAXITERATION);
			new2Cost=retCost(newSolution2);
			
			if((tl.search(newSolution2)==false)  &&( new2Cost<newCost || newsToList)){
				newsToList=false;
				for(int i=0;i<NUMTRUCKS;i++){
					newSolution[i].clone(newSolution2[i]);
				}
				newCost=new2Cost;
			}	

		}while(newCost > OPTIMALVALUE && countHill <numHillit);


		if(newsToList==false && newCost<solutionCost){
			for(int i=0;i<NUMTRUCKS;i++){
				solution[i].clone(newSolution[i]);
			}
			solutionCost=newCost;
			tl.insert(newSolution);
			goal=(double)iter/MAXITERATION;
		}

		
	}while( iter < MAXITERATION && bestCost > OPTIMALVALUE);

	show(solution);
	cout<<"sol cost: "<<solutionCost<<endl;
	cout<<"goal :"<<goal<<endl;
return true;

}










int main()
{
	//cout<<"wer\n";
	readInput();

	

	LARGE_INTEGER frequency;
		LARGE_INTEGER t1, t2;      
		double elapsedTime;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&t1);


	//initialize();
	

	//sort(solution,solution+NUMTRUCKS,cmpTruck);



	srand ( time(NULL) );        

	
	// ==>>>   Here the method to solve the problem can be chose
	tabuSearch(300);
	//simulatedAnnealing(20);
	//stcHill();
	//iterHill(100);


	QueryPerformanceCounter(&t2);
		elapsedTime = (t2.QuadPart - t1.QuadPart) / frequency.QuadPart;
		cout<<"time: "<<elapsedTime<<endl;

	getchar();
	return 0;
}

