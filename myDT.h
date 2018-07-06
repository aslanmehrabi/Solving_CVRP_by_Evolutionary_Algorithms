/*
 
 Solving Capacitated Vehicle Routing Problem (CVRP) by Genetic Algorithm 
 This file contains Data Structures which are used in "SHC-Annealing-Tabu_search.cpp" whcih contains Evolutanioray solutions to the CVRP problem
 file named "Ank_80.txt" which also can be found in the current repository can be used as test data


*/

#include <iostream>
#include <random>
#include <set>
#include <algorithm>
#include <Windows.h>

using namespace std;

#define MAXTRUCKS 200
#define MAXCAP 2210     //in adada bayad daghigh beshan
#define MAXCITIES 484
#define NUMTRUCKS 10 //8
#define MAXITERATION 2000
#define OPTIMALVALUE 1763//784//450
#define FileName "Ank_80.txt"
#define TSHL 3 // T bara stochastic hill climbing
#define TSAMAX 40 // T baraye shrue simulated annealing
#define TSAMIN 5  // T baraye payan simulated annealing
#define TABUSIZE 500


#if (MAXCAP>MAXCITIES)
	#define MAXTRUCKCITIES MAXCITIES
#else
	#define MAXTRUCKCITIES MAXCAP
#endif
	
double EuclidianDistance(int a,int b);

struct Truck{
	int members[MAXTRUCKCITIES]; 
	int numMembers; // tedade shahrhaye dakhele dor
	int remindedCap; // dar akhare masir in meghadar bar ezfe miad
	double totalCost; // hazine kole dor (quality)

	bool clone(Truck t){//copy t to this
		numMembers=t.numMembers;
		remindedCap=t.remindedCap;
		totalCost=t.totalCost;
		for(int i=0;i<t.numMembers;i++)
			members[i]=t.members[i];
		return true;
	}


	double calcTotalCost(){
		double ans=0;
		ans+=EuclidianDistance(0,members[0]);
		for(int i=1;i<numMembers;i++)
			ans+=EuclidianDistance(members[i-1],members[i]);
		ans+=EuclidianDistance(members[numMembers-1],0);
		return ans;
	}



} 
	solution[MAXTRUCKS] , // solutioni ke alan darim hamsayehasho baresi mikonim
	best[MAXTRUCKS] , // behtarin javab ta alan
	newSolution[MAXTRUCKS],
	newSolution2[MAXTRUCKS]
; // hamsayeye solution ke tolid mishe ba momkene jaygozine solution beshe

	double solutionCost,bestCost;


struct Coordinate{
	double x;
	double y;
}
	coordinates[MAXCITIES] ;  //[0]=> mabda  , baghie shahrhaye mokhtalef


int demands[MAXCITIES];  // [0] => khali va alaki
int cap; //zarfiate har kamiun


void readInput(); //meghdar dehie coordinates,demands,cap,k
bool initialize(); //peida kardane javab avalie fisible(tu solution va best)
void tweak1();   // ja	bejai shahrha dakhele yek dor
//in 3ta shoma

void angleSort(Truck * sol,int truckNum); // moratab sazie sharhaye yek dor bar asase zavie
bool tweak2(Truck * sol,int,int); //jabejai shahrha 2ta dor
void tweak3(Truck * sol,int,int,int); //jabejai shahrha bishtar az 2ta dor
// man

double retCost(Truck sol[]);







//tabu list:


struct Ans{
	Truck sol[MAXTRUCKS];
};


bool cmpTruck(Truck t1, Truck t2){ // bara moratab kardane yek dor(tabu)
	if(t1.totalCost < t2.totalCost)
		return true;
	else if(t1.totalCost > t2.totalCost)
		return false;
	return t1.members[0] < t2.members[0];
}


//bool cmpMember(int node1,int node2){//moratab kardane shahrhaye yek dor(tabu)
//	return node1<node2;
//}


struct CmpAns{
	bool operator ()(Ans a,Ans b){
		for(int i=0;i<NUMTRUCKS;i++){
			if(a.sol[i].totalCost != b.sol[i].totalCost)
				return a.sol[i].totalCost < b.sol[i].totalCost;
			if(a.sol[i].totalCost){
				if(a.sol[i].members[0] != b.sol[i].members[0])
					return a.sol[i].members[0] < b.sol[i].members[0];
			}
		}
		return false;
	}
};



struct tabuList{
	Truck arr[TABUSIZE][MAXTRUCKS];
	int ind;

	bool insert(Truck * sol){
		sort(sol,sol+NUMTRUCKS,cmpTruck);//@@@@@@@@@@@@@@@check


		for(int i=0;i<NUMTRUCKS;i++){
			for(int j=0;j<sol[i].numMembers;j++){
				arr[ind][i].members[j]=sol[i].members[j];
			}
			arr[ind][i].numMembers=sol[i].numMembers;
			arr[ind][i].remindedCap=sol[i].remindedCap;
			arr[ind][i].totalCost=sol[i].totalCost;
		}
		ind++;
		if(ind==TABUSIZE)
			ind=0;
		return true;
	}

	tabuList(){
		ind=0;
		memset(arr,sizeof arr,0);
	}


	bool search(Truck * sol){
		sort(sol,sol+NUMTRUCKS,cmpTruck);//

		for(int i=0;i<TABUSIZE;i++){
			for(int j=0;j<NUMTRUCKS;j++){
				if(arr[i][j].totalCost != sol[j].totalCost)
					continue;
				if(arr[i][j].numMembers){
					for(int k=0;k<arr[i][j].numMembers;k++)
						if(arr[i][j].members[k]!=sol[j].members[k])
							continue;
				}
				if(j==NUMTRUCKS-1)
					return true;
			}
		}
		return false;


	}
} tl ;






//struct TabuList{
//	//set <Ans,CmpAns> sttttt;
//	Ans arr[TABUSIZE];
//	int size;
//	int arrInd;
//	int deletInd;//indexe un ke bayad delet she
//
//
//	bool insert(Ans){
//		/*if(size==TABUSIZE){
//			delet(arr[deletInd]);
//			deletInd++;
//			if(deletInd==TABUSIZE)
//				deletInd=0;
//			
//		}*/
//
//
//		for(int i=0;i<MAXTRUCKS;i++){
//			for(int j=0;j<sol[i].
//		}
//
//	}
//
//	bool delet(Ans){
//		
//	}
//
//	TabuList(){
//
//	}
//
//
//};







