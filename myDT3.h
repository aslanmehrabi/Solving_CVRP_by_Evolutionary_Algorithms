/*
 
 Solving Capacitated Vehicle Routing Problem (CVRP) by Genetic Algorithm 
 This file contains Data Structures which are used in "genetic.cpp" whcih is a genetic algorithm solution to the CVRP problem
 file named "Ank_80.txt" which also can be found in the current repository can be used as test data


*/

//gentic algorithm & constraint handling

#include <algorithm>
#include <Windows.h>

using namespace std;

#define MAXTRUCKS 200
#define MAXCAP 2210     //in adada bayad daghigh beshan
#define MAXCITIES 484
#define NUMTRUCKS 10//5 //8
#define OPTIMALVALUE 784//784//450

#define INFSBLCAPRATIO 1.2 // constraint handling  zarfiat kol chan barabare CAP
#define PENALTY 5

#define FileName "Ank_32.txt"

#define POPSIZE 100 // population size genetic
#define OFFSPRINGRATIO 2// 2 barabare POSIZE offspring ijad kone -- khode popSize ham jozeshe
#define	OFFSPRINGSIZE ((int)POPSIZE*OFFSPRINGRATIO)

#define MUTATIONPROB 0.7  // ehtemale inke badaz xover, mutation kone

#define TIMELIMIT 179 //zamane ejra
#define NUMITTERATION 1000 // mahdudiate 
#define NUMEVALUATION 10000000000

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
	int numNotUsed;//tu cross over
	int infeasibleCost;// 0: feasisible  >0: meghdare infeasible ke bare ezafe mibare

	bool clone(Truck t){//copy t to this
		numMembers=t.numMembers;
		remindedCap=t.remindedCap;
		infeasibleCost=t.infeasibleCost;
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

double solutionCost,bestCost;//behtarin javan ke ta alan peida shod(feasible) => best

Truck population[POPSIZE][MAXTRUCKS];
double popCost[POPSIZE];
int popInFsblCost[POPSIZE];

Truck offSprings[OFFSPRINGSIZE][MAXTRUCKS];
double offSpringCost[OFFSPRINGSIZE];
int offSpringInFsblCost[OFFSPRINGSIZE];
bool offSpringChoosed[OFFSPRINGSIZE];


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

struct Ans{
	Truck sol[MAXTRUCKS];
};




bool cmpTruckCost(Truck t1, Truck t2){ // bara moratab kardane yek dor(tabu)soudi
	if(t1.totalCost < t2.totalCost)
		return true;
	else if(t1.totalCost > t2.totalCost)
		return false;
	return t1.members[0] < t2.members[0];
}

bool cmpTruckNotUsed(Truck t1, Truck t2){ // moratab kardane 1 dor Xover , nozuli
	return t1.numNotUsed > t2.numNotUsed;
}

bool cmpTruckRCap(Truck t1,Truck t2){
	return t1.remindedCap < t2.remindedCap;
}


struct CmpAns{
	bool operator ()(Truck * a,Truck * b){
		for(int i=0;i<NUMTRUCKS;i++){
			if(a[i].totalCost != b[i].totalCost)
				return a[i].totalCost < b[i].totalCost;
			if(a[i].totalCost){
				if(a[i].members[0] != b[i].members[0])
					return a[i].members[0] < b[i].members[0];
			}
		}
		return false;
	}
};