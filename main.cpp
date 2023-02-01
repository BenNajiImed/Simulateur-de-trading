#include"trading.h"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
int main(){
   vector<PrixJournalier> h;
   ifstream f("prices_simple.csv");
    int nbpj= 0,i=0;
    string ligne;
    if(f.is_open()){
        getline(f,ligne);
        while(!f.eof()){
               PrixJournalier pj ;
               f>>pj;
               h.push_back(pj);
               nbpj++;
        }
        f.clear();
        f.seekg(0,f.beg);
    }
   int j;
   double s;
   Trader* T;
   string n;
   Date d;
   cout<<"entrer le type du trader"<<endl<<"0-trader humain"<<endl<<"1-trader algorithmique"<<endl<<"2-trader aleatoire"<<endl;
   cin>>j;
   cout<<"entrer le nom du trader"<<endl;
   cin>>n;
   switch(j){
           case 0:
           T=new TraderHumain(n,HUMAIN);
           break;
           case 1:
           T=new TraderAlgo(n,ALGO);
           break;
           case 2:
           T=new TraderAlea(n,ALEA);
   }
   vector<Titre*> v;
   Portefeuille P(n,v);
   cout<<"voulez vous lancer une simulation?(oui/non)"<<endl;
   cin>>n;
   Bourse* b;
   Simulation* S;
   while(n=="oui"){
        cout<<"entrer le solde initial"<<endl;
        cin>>s;
        cout<<"entrer la date de depart(jj/mm/aa;)"<<endl;
        cin>>d;
        cout<<"entrer le nombre de jours"<<endl;
        cin>>i;
        b=new BourseVecteur("NewYork",d,h);
        S=new Simulation(b,T,s,P,i,d);
        S->LancerSimulation();
        s=S->CalculerGain();
        double s1=S->GainPourcentage();
        cout<<"votre gain : "<<s<<endl;
        cout<<"gain en pourcentage : "<<s1<<"%"<<endl;
        cout<<"voulez vous lancer une simulation?(oui/non)"<<endl;
        cin>>n;
   };
   return 0;
}
/*int main(){
   vector<PrixJournalier> h;
    int i=0;
   int j;
   double s;
   Trader* T;
   string n;
   Date d;
   cout<<"entrer le type du trader"<<endl<<"0-trader humain"<<endl<<"1-trader algorithmique"<<endl<<"2-trader aleatoire"<<endl;
   cin>>j;
   cout<<"entrer le nom du trader"<<endl;
   cin>>n;
   switch(j){
           case 0:
           T=new TraderHumain(n,HUMAIN);
           break;
           case 1:
           T=new TraderAlgo(n,ALGO);
           break;
           case 2:
           T=new TraderAlea(n,ALEA);
   }
   vector<Titre*> v;
   Portefeuille P(n,v);
   cout<<"voulez vous lancer une simulation?(oui/non)"<<endl;
   cin>>n;
   Bourse* b;
   Simulation* S;
   if(n=="oui"){
        cout<<"entrer le solde initial"<<endl;
        cin>>s;
        cout<<"entrer la date de depart(jj/mm/aa;)"<<endl;
        cin>>d;
        cout<<"entrer le nombre de jours"<<endl;
        cin>>i;
        b=new BourseFichier("NewYork",d,"prices_simple.csv");
        S=new Simulation(b,T,s,P,i,d);
        S->LancerSimulation();
        s=S->CalculerGain();
        double s1=S->GainPourcentage();
        cout<<"votre gain : "<<s<<endl;
        cout<<"gain en pourcentage : "<<s1<<"%"<<endl;
   };
   return 0;
}*/
