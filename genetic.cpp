/*
 
 Solving Capacitated Vehicle Routing Problem (CVRP) by Genetic Algorithm 
 Note that the uploaded file in the current repository called  "myDT3.h" should be added to the project
 file named "Ank_80.txt" which also can be found in the current repository can be used as test data


*/

// genetic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
// genetic & constraint handling
#include "myDT3.h"
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



LARGE_INTEGER frequency; // bara time
LARGE_INTEGER t1, t2;      
double elapsedTime;


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
		for(int i=0;i<countCustomer;i++)// [0] kolan noghteye shru hast ke demandesh ham 0 hast hamishe.
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
bool initialize() //greedy
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

	if(!remaincustomer){
		for(int i=0;i<NUMTRUCKS;i++){
			solution[i].infeasibleCost=0;
		}

	}
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
bool initialize2()//random
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

	for(int i=0;i<NUMTRUCKS;i++)
		solution[i].infeasibleCost=0;
	return true;
}
//
//Truck newSolution[MAXTRUCKS];  in ro tweak mikone(jaye 2ta az shahrash avaz mishe)
//solution ===tweak===>  newSolution
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
		int d=0;
		for(int j=0;j<sol[i].numMembers;j++)
		{
			printf("%d,",sol[i].members[j]);
			d+=tempdemands[sol[i].members[j]];
		}
		//printf("\n%d",sol[i].infeasibleCost+cap);
		printf("\n%d",d);
		printf("\n\n");
	}
	cout<<"result cost: "<<retCost(sol)<<endl;
	//cout<<"tw1: "<<tw1<<"    tw2:"<<tw2<<endl;
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


double retCost(Truck sol[])
{
	double totalcost=0;
	for(int i=0;i<NUMTRUCKS;i++)
		totalcost+=sol[i].totalCost;
	return totalcost;
}


bool checkValid(Truck * sol){//check mikone tu sol hameye shahra bashan
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


bool xOverINFsbl(Truck * sol1,Truck * sol2,Truck * res){//sol1 & 2 momkene inFsbl bashan va res ham momkene inFsbl she.
	//define darsad ke be cost ezafe she

	if(rand()%10<1)
		sort(sol1,sol1+NUMTRUCKS,cmpTruckRCap);//soudi cap ,  mishod barasase cost

	if(NUMTRUCKS==1)
		return false;
	int cutInd=(rand()%(NUMTRUCKS-1))+1; // ta ghabl az in az sol1, azin bebad sol2

	for(int i=0;i<cutInd;i++)
		res[i].clone(sol1[i]);
	
	for(int i=cutInd;i<NUMTRUCKS;i++){
		res[i].remindedCap=cap;
		res[i].numMembers=0;
		res[i].infeasibleCost=0;
	}
	

	bool used[MAXCITIES];
	memset(used,0,countCustomer);

	for(int i=0;i<cutInd;i++)
		for(int j=0;j<res[i].numMembers;j++)
			used[res[i].members[j]]=true;
	
	
	for(int i=0;i<NUMTRUCKS;i++){
		sol2[i].numNotUsed=0;
		for(int j=0;j<sol2[i].numMembers;j++)
			if(!used[sol2[i].members[j]])
				sol2[i].numNotUsed++;
	}

	sort(sol2,sol2+NUMTRUCKS,cmpTruckNotUsed);

	cutInd--;
	int resInd=0;//index dakhele yek dor
	for(int i=0;i<NUMTRUCKS;i++){
		for(int j=0;j<sol2[i].numMembers;j++){
			if(used[sol2[i].members[j]]==false){
				if(res[cutInd].remindedCap+(INFSBLCAPRATIO-1)*cap/**/ < demands[sol2[i].members[j]]){
					cutInd++;//@@@@@@@ mishe inja gasht va avalin shahri ke ja misharo bezarim dakhel
					resInd=0;
					if(cutInd==NUMTRUCKS)
						return false;
				}
				if(res[cutInd].remindedCap +(INFSBLCAPRATIO-1)*cap >= demands[sol2[i].members[j]]){
					res[cutInd].members[resInd++]=sol2[i].members[j];
					res[cutInd].remindedCap -= demands[sol2[i].members[j]];
					if(res[cutInd].remindedCap<0){// cap az 0 kamtar nemishe
						res[cutInd].infeasibleCost += -res[cutInd].remindedCap ;
						res[cutInd].remindedCap =0;
					}
					res[cutInd].numMembers++;
					used[sol2[i].members[j]]=true;
				}
				else
					return false;//in halat pish nemiad
			}
		}
	}
	
	for(int i=0;i<NUMTRUCKS;i++)
		res[i].totalCost=res[i].calcTotalCost();

	return true;
}

//sol1,sol2 xover mishan,mire tu res  ,  hame feasible
bool xOverFsbl(Truck  * sol1 , Truck * sol2 , Truck * res){
	//@@@@@@@@@@@   mishe ba ye ehtemale random ino gozosht       @@ ya 2ta noghteye random tush entekhab she
	
	if(rand()%10<1)
		sort(sol1,sol1+NUMTRUCKS,cmpTruckRCap);//soudi cap ,  mishod barasase cost

	if(NUMTRUCKS==1)
		return false;
	int cutInd=(rand()%(NUMTRUCKS-1))+1; // ta ghabl az in az sol1, azin bebad sol2

	for(int i=0;i<cutInd;i++)
		res[i].clone(sol1[i]);
	
	for(int i=cutInd;i<NUMTRUCKS;i++){
		res[i].remindedCap=cap;
		res[i].numMembers=0;
		res[i].infeasibleCost=0;
	}
	

	bool used[MAXCITIES];
	memset(used,0,countCustomer);

	for(int i=0;i<cutInd;i++)
		for(int j=0;j<res[i].numMembers;j++)
			used[res[i].members[j]]=true;
	
	
	for(int i=0;i<NUMTRUCKS;i++){
		sol2[i].numNotUsed=0;
		for(int j=0;j<sol2[i].numMembers;j++)
			if(!used[sol2[i].members[j]])
				sol2[i].numNotUsed++;
	}

	sort(sol2,sol2+NUMTRUCKS,cmpTruckNotUsed);

	cutInd--;
	int resInd=0;//index dakhele yek dor
	for(int i=0;i<NUMTRUCKS;i++){
		for(int j=0;j<sol2[i].numMembers;j++){
			if(used[sol2[i].members[j]]==false){
				if(res[cutInd].remindedCap < demands[sol2[i].members[j]]){
					cutInd++;//@@@@@@@ mishe inja gasht va avalin shahri ke ja misharo bezarim dakhel
					resInd=0;
					if(cutInd==NUMTRUCKS)
						return false;
				}
				if(res[cutInd].remindedCap >= demands[sol2[i].members[j]]){
					res[cutInd].members[resInd++]=sol2[i].members[j];
					res[cutInd].remindedCap -= demands[sol2[i].members[j]];
					res[cutInd].numMembers++;
					used[sol2[i].members[j]]=true;
				}
				else
					return false;//in halat pish nemiad
			}
		}
	}
	
	for(int i=0;i<NUMTRUCKS;i++)
		res[i].totalCost=res[i].calcTotalCost();


	return true;


}



//Truck population[POPSIZE][MAXTRUCKS];
//double popCost[POPSIZE];
//int inFsblCost[POPSIZE];
//
//
//Truck offSprings[POPSIZE*OFFSPRINGRATIO][MAXTRUCKS];
//double offSpringCost[POPSIZE*OFFSPRINGRATIO];
//int offSpringInFsblCost[POPSIZE*OFFSPRINGRATIO];
//bool offSpringChoosed[POPSIZE*OFFSPRINGRATIO];


bool terminalCondition(int numIteration,int numEvaluation){

	QueryPerformanceCounter(&t2);
	elapsedTime = (t2.QuadPart - t1.QuadPart) / frequency.QuadPart;
	if(/*elapsedTime > TIMELIMIT ||*/ numIteration>NUMITTERATION /*|| numEvaluation > NUMEVALUATION*/)
		return true;
	return false;
}


int binarySearchDbl(double key,double arr[],int start,int end){//indexe khunei ke key <= une
	if(start>=end)//> pish nemiad
		return start;
	int mid=(start+end)/2;
	if(arr[mid]==key || mid==0 ||  (arr[mid]>key && arr[mid-1]<key))
		return mid;
	if(arr[mid]<key)
		return binarySearchDbl(key,arr,mid+1,end);
	if(arr[mid]>key)
		return binarySearchDbl(key,arr,start,mid-1);
}


void constraintHandling(){

	int numIteration=0,numEvaluation=0;
	int parentsCandidate[1000];
	int numParentCandidate=4;//4 bara inja
	double sumProbOff[OFFSPRINGSIZE]={}; // initialize 0

	//  @@@@@ best

	do{
		/*
		if(numIteration==10)
		{
			double max=retCost(population[0]);
			int k=0;
			for(int i=1;i<POPSIZE;i++)
			{
				if(retCost(population[i])<max)
				{
					max=retCost(population[i]);
					k=i;
				}
			}
			show(population[k]);
		}
		*/
		numIteration++;
		//    ===> numEvaluation++;    @@@@@@
		for(int i=0;i<POPSIZE;i++){//copy pop => off
			for(int j=0;j<NUMTRUCKS;j++)
				offSprings[i][j].clone(population[i][j]);
			offSpringCost[i]=popCost[i];
			offSpringChoosed[i]=false;
			for(int j=0;j<NUMTRUCKS;j++){
				offSpringInFsblCost[i]+=offSprings[i][j].infeasibleCost;
			}
		}

		for(int i=POPSIZE;i<OFFSPRINGSIZE;i++){
			//entekhab 4 ta random, 2ta pedar
			bool fsbl;
			offSpringInFsblCost[i]=0;
			do{
				for(int j=0;j<numParentCandidate;j++)
					parentsCandidate[j]=rand()%POPSIZE;  //@ hazfe tekrariha

				int firstInd=0,firstCost=popCost[parentsCandidate[0]];
				for(int j=1;j<numParentCandidate;j++){
					if(popCost[parentsCandidate[j]] < firstCost){
						firstCost=popCost[parentsCandidate[j]];
						firstInd=j;
					}
				}

				int secInd = firstInd==0 ? 1:0;
				int secCost = firstInd==0 ? popCost[parentsCandidate[1]] : parentsCandidate[0];
				for(int j=1;j<numParentCandidate;j++){
					if(j!=firstInd && popCost[parentsCandidate[j] < secCost]){
						secCost=popCost[parentsCandidate[j]];
						secInd=j;
					}
				}

				fsbl=xOverINFsbl(population[parentsCandidate[firstInd]],population[parentsCandidate[secInd]],offSprings[i]);//@@@
			}while(!fsbl);

			offSpringCost[i]=retCost(offSprings[i]);
			offSpringInFsblCost[i]=0;
			for(int j=0;j<NUMTRUCKS;j++)
				offSpringInFsblCost[i]+=offSprings[i][j].infeasibleCost;

			
			if(offSpringCost[i] < bestCost && offSpringInFsblCost[i]==0){
				bestCost=offSpringCost[i];
				for(int k=0;k<NUMTRUCKS;k++)
					best[k].clone(offSprings[i][k]);
			}

			if((rand()%1000) > MUTATIONPROB*1000){
				//tweak(offSpring[i]);
				;
				offSpringCost[i]=retCost(offSprings[i]);
				offSpringInFsblCost[i]=0;
				for(int k=0;k<NUMTRUCKS;k++){
					offSpringInFsblCost[i]+=offSprings[i][k].infeasibleCost;
				}
				if(offSpringCost[i] < bestCost && offSpringInFsblCost[i]==0){
					bestCost=offSpringCost[i];
					for(int k=0;k<NUMTRUCKS;k++)
						best[k].clone(offSprings[i][k]);
				}
			}
			offSpringChoosed[i]=false;//dune dune false konim
		}

		//entekhab az offSpringha va gozoshtan tu population:

		sumProbOff[0]=1/((offSpringCost[0])*(1+(double)offSpringInFsblCost[0]/cap)*PENALTY); // tu har khune jame 1/cost[i] ta unja baraye rolet wheel
		for(int i=1;i<OFFSPRINGSIZE;i++)
			sumProbOff[i]=sumProbOff[i-1]+1/ ((offSpringCost[i])*(1+(double)offSpringInFsblCost[i]/cap)*PENALTY); 
			
		//sumProbOff[0]=1/((offSpringCost[0])*/*(1+(double)offSpringInFsblCost[0]/cap)**/PENALTY); // tu har khune jame 1/cost[i] ta unja baraye rolet wheel
		//for(int i=1;i<OFFSPRINGSIZE;i++)
	//		sumProbOff[i]=sumProbOff[i-1]+1/ ((offSpringCost[i])*/*(1+(double)offSpringInFsblCost[i]/cap)**/PENALTY); 
		
		double randRes;
		int indexRes;
		for(int i=0;i<POPSIZE;i++){			
			do{
				randRes=((double)(rand()%10000))/(10000*sumProbOff[OFFSPRINGSIZE-1]);
				indexRes=binarySearchDbl(randRes,sumProbOff,0,OFFSPRINGSIZE-1);
			}while(offSpringChoosed[indexRes]);

			for(int j=0;j<NUMTRUCKS;j++)
				population[i][j].clone(offSprings[i][j]);
			popCost[i]=offSpringCost[i];
			popInFsblCost[i]=offSpringInFsblCost[i];
			offSpringChoosed[i]=true;
			
		}		

		
		
	}while( !terminalCondition(numIteration,numEvaluation));

	show(best);



}


void genetic(){
	int numIteration=0,numEvaluation=0;
	int parentsCandidate[1000];
	int numParentCandidate=5;
	double sumProbOff[OFFSPRINGSIZE]={}; // initialize 0

	//  @@@@@ best

	do{
		numIteration++;
		//    ===> numEvaluation++;    @@@@@@
		for(int i=0;i<POPSIZE;i++){//copy pop => off
			for(int j=0;j<NUMTRUCKS;j++)
				offSprings[i][j].clone(population[i][j]);
			offSpringCost[i]=popCost[i];
			offSpringChoosed[i]=false;
		}

		for(int i=POPSIZE;i<OFFSPRINGSIZE;i++){
			//entekhab 5 ta random, 2ta pedar
			bool fsbl;
			do{
				for(int j=0;j<numParentCandidate;j++)
					parentsCandidate[j]=rand()%POPSIZE;  //@ hazfe tekrariha

				int firstInd=0,firstCost=popCost[parentsCandidate[0]];
				for(int j=1;j<numParentCandidate;j++){
					if(popCost[parentsCandidate[j]] < firstCost){
						firstCost=popCost[parentsCandidate[j]];
						firstInd=j;
					}
				}

				int secInd = firstInd==0 ? 1:0;
				int secCost = firstInd==0 ? popCost[parentsCandidate[1]] : parentsCandidate[0];
				for(int j=1;j<numParentCandidate;j++){
					if(j!=firstInd && popCost[parentsCandidate[j] < secCost]){
						secCost=popCost[parentsCandidate[j]];
						secInd=j;
					}
				}


				fsbl=xOverFsbl(population[parentsCandidate[firstInd]],population[parentsCandidate[secInd]],offSprings[i]);//@@@
			}while(!fsbl);

			offSpringCost[i]=retCost(offSprings[i]);
			if(offSpringCost[i] < bestCost){
				bestCost=offSpringCost[i];
				for(int k=0;k<NUMTRUCKS;k++)
					best[k].clone(offSprings[i][k]);
			}

			if((rand()%1000) > MUTATIONPROB*1000){
				//tweak(offSpring[i]);;
				;
				offSpringCost[i]=retCost(offSprings[i]);
				if(offSpringCost[i] < bestCost){
					bestCost=offSpringCost[i];
					for(int k=0;k<NUMTRUCKS;k++)
						best[k].clone(offSprings[i][k]);
				}
			}
			offSpringChoosed[i]=false;
		}

		//entekhab az offSpringha va gozoshtan tu population:

		sumProbOff[0]=1/offSpringCost[0]; // tu har khune jame 1/cost[i] ta unja baraye rolet wheel
		for(int i=1;i<OFFSPRINGSIZE;i++)
			sumProbOff[i]=sumProbOff[i-1]+1/offSpringCost[i];
		
		double randRes;
		int indexRes;
		for(int i=0;i<POPSIZE;i++){
			do{
				randRes=((double)(rand()%10000))/(10000*sumProbOff[OFFSPRINGSIZE-1]);
				indexRes=binarySearchDbl(randRes,sumProbOff,0,OFFSPRINGSIZE-1);
			}while(offSpringChoosed[indexRes]);

			for(int j=0;j<NUMTRUCKS;j++)
				population[i][j].clone(offSprings[i][j]);
			popCost[i]=offSpringCost[i];
			offSpringChoosed[i]=true;
			
		}
		
		
	}while( !terminalCondition(numIteration,numEvaluation));

	show(best);

}


int main()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&t1);


	//srand ( time(NULL) ); //tabe e rand har bar yejur javab mide

	readInput();	
	

	for(int i=0;i<POPSIZE;i++){ // initialize randome avalie tu population
		while(!initialize2());
		for(int j=0;j<NUMTRUCKS;j++){
			population[i][j].clone(solution[j]);
		}
		popInFsblCost[i]=0;
		popCost[i]=retCost(solution);			
		if(i==0 || popCost[i]<bestCost){
			for(int j=0;j<NUMTRUCKS;j++)
				best[j].clone(population[i][j]);
			bestCost=popCost[i];
		}
	}

	//genetic();
	constraintHandling();



	/*while(!initialize2());
	for(int i=0;i<NUMTRUCKS;i++)
		newSolution[i].clone(solution[i]);
	while(!initialize2);
	xOverFsbl(solution,newSolution,newSolution2);*/
	


	QueryPerformanceCounter(&t2);
	elapsedTime = (t2.QuadPart - t1.QuadPart) / frequency.QuadPart;
	cout<<"time: "<<elapsedTime<<endl;

	getchar();
	return 0;
}

