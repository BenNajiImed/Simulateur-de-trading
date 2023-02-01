#include"trading.h"
#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<fstream>
#include<algorithm>
//definition methodes classe Date
Date::Date(){}
Date::Date(int j,int m,int a){
    if(j<=31 && j>=1){
        Jour=j;
    }
    if(m<=12 && m>=1){
        Mois=m;
    }
    if(a<=2021){
        Annee=a;
    }
    }
int Date::GetJour() const {return Jour;}
int Date::GetMois() const {return Mois;}
int Date::GetAnnee() const {return Annee;}
void Date::DateJourSuivant(){
    if(Jour<31){
        Jour+=1;
    }
    else{
          Jour=1;
        if(Mois<12){
            Mois+=1;
        }
        else{
            Mois=1;
            Annee+=1;
        }
    }
}
void Date::DateJourPrecedent(){
     if(Jour>1){
        Jour-=1;
    }
    else{
          Jour=31;
        if(Mois>1){
            Mois-=1;
        }
        else{
            Mois=12;
            Annee-=1;
        }
    }
}
ostream& operator<<(ostream& flux,const Date& d){
    flux<<d.Jour<<"/"<<d.Mois<<"/"<<d.Annee<<endl;
    return flux;
}
istream& operator>> (istream& flux, Date& d){
    char tmp[10];
      flux.getline(tmp,10,'/');
      d.Jour=atoi(tmp);
      flux.getline(tmp,10,'/');
      d.Mois=atoi(tmp);
      flux.getline(tmp,10,';');
      d.Annee=atoi(tmp);
    return flux;
}
bool operator==(Date const &a, Date const& b){
    if(a.Jour==b.Jour && a.Mois==b.Mois && a.Annee==b.Annee){
        return true;
    }
    else {return false;}
}
//definition methodes classe PrixJournalier
PrixJournalier::PrixJournalier(Date& d ,string n ,double p):date(d),NomAction(n),prix(p){}
Date PrixJournalier::Getdate() const{return date;}
string PrixJournalier::GetNomAction() const{return NomAction;}
double PrixJournalier::Getprix() const{return prix;}
ostream& operator<<(ostream& flux,const PrixJournalier& p){
    flux<<"date: "<<p.date;
    flux<<"nom action: "<<p.NomAction<<endl;
    flux<<"prix: "<<p.prix<<endl;
    return flux;
}
istream& operator>> (istream& flux, PrixJournalier& p){
    flux>>p.date;
    char tmp[10];
    flux.getline(tmp,10,';');
    p.NomAction=tmp;
    flux>>p.prix;
    return flux;
}
bool operator<(PrixJournalier& p1, PrixJournalier& p2){return p1.prix<p2.prix; }
//definition methodes classe Bourse
Bourse::Bourse(){}
Bourse::Bourse(string n,Date& d):Nom(n),DateActuel(d){}
string Bourse::GetNom() const{return Nom;}
Date Bourse::GetDateActuel()const{return DateActuel;}
void Bourse::plusJour(){
     DateActuel.DateJourSuivant();
 }
ostream& operator<<(ostream& flux,const Bourse& b){
     flux<<"nom de la bourse "<<b.Nom<<endl;
     flux<<"la date du jour: "<<b.DateActuel<<endl;
     return flux;
}
//definition methodes classe BourseVecteur
BourseVecteur::BourseVecteur(){}
BourseVecteur::BourseVecteur(string n, Date& d, vector<PrixJournalier>& h):Bourse(n,d), Historique(h){}
PrixJournalier* BourseVecteur::ChercherUneAction(string n,Date& d){
    unsigned int i=0;
    PrixJournalier *A=NULL;
    while(i<Historique.size()){
        if(Historique.at(i).Getdate()==d && Historique.at(i).GetNomAction()==n){
            A=new PrixJournalier;
            *A=Historique.at(i);
            break;
        }
        else {i++;}
    }
    return A;
}
vector<PrixJournalier> BourseVecteur::ChercherHistoriqueAction(string n,int nb){
    Date d=DateActuel;
    vector<PrixJournalier> v;
    for(int j=0;j<nb;j++){
        for(unsigned int i=0;i<Historique.size();i++){
             if(Historique.at(i).Getdate()==d && Historique.at(i).GetNomAction()==n){
                 v.push_back(Historique.at(i));
                }
        }
        d.DateJourPrecedent();
    }
    return v;
}
vector<PrixJournalier> BourseVecteur::ActionsJourActuel(){
    unsigned int i=0;
    vector<PrixJournalier> v;
    for(i=0;i<Historique.size();i++){
        if(Historique.at(i).Getdate()==DateActuel){
         v.push_back(Historique.at(i));
        }
    }
    return v;
}
vector<PrixJournalier> BourseVecteur::ChercherHistoriqueJour(Date& d){
    unsigned int i=0;
    vector<PrixJournalier> v;
    for(i=0;i<Historique.size();i++){
        if(Historique.at(i).Getdate()==d){
         v.push_back(Historique.at(i));
        }
    }
    return v;
}
//definition methodes classe BourseFichier
BourseFichier::BourseFichier(string n, Date& d,const char* iniFile):Bourse(n,d),Historique(iniFile){}
PrixJournalier* BourseFichier::ChercherUneAction(string n,Date& d){
    string ligne;
    if(Historique.is_open()){
        getline(Historique,ligne);
        while(!Historique.eof()){
               PrixJournalier* pj=new PrixJournalier ;
               Historique>>*pj;
               if(pj->GetNomAction()==n && pj->Getdate()==d){
                   return pj;
               }
        }
        Historique.clear();
        Historique.seekg(0,Historique.beg);
    }
    return NULL;
}
vector<PrixJournalier> BourseFichier::ChercherHistoriqueAction(string n,int nb){
    Date d=DateActuel;
    vector<PrixJournalier> v;
    PrixJournalier p;
    string ligne;
    if(Historique.is_open()){
        for(int i=0;i<nb;i++){
            getline(Historique,ligne);
            while(!Historique.eof()){
                Historique>>p;
                if(p.GetNomAction()==n && p.Getdate()==d){
                    v.push_back(p);
                }
            }
            Historique.clear();
            Historique.seekg(0,Historique.beg);
            d.DateJourPrecedent();
        }
    }
    return v;
}
vector<PrixJournalier> BourseFichier::ActionsJourActuel(){
    PrixJournalier p;
    vector<PrixJournalier> v;
    string ligne;
    if(Historique.is_open()){
        getline(Historique,ligne);
        while(!Historique.eof()){
            Historique>>p;
            if(p.Getdate()==DateActuel){
                v.push_back(p);
            }
        }
        Historique.clear();
        Historique.seekg(0,Historique.beg);
    }
    return v;
}
vector<PrixJournalier> BourseFichier::ChercherHistoriqueJour(Date& d){
    vector<PrixJournalier> v;
    PrixJournalier p;
    string ligne;
    if(Historique.is_open()){
        getline(Historique,ligne);
        while(!Historique.eof()){
            Historique>>p;
            if(p.Getdate()==d){
                v.push_back(p);
            }
        }
        Historique.clear();
        Historique.seekg(0,Historique.beg);
    }
    return v;
}
//definition methodes classe Transaction
Transaction::Transaction(){}
Transaction::Transaction(TypeTransaction t,string n,int q):Type(t), NomAction(n), Quantite(q){}
TypeTransaction Transaction::GetType() const{return Type;}
string Transaction::GetNomAction() const{return NomAction;}
int Transaction::GetQuantite() const{return Quantite;}
ostream& operator<<(ostream& flux,const Transaction& t){
    flux<<"type de la transaction:  "<<t.Type<<endl;
    flux<<"nom action : "<<t.NomAction<<endl;
    flux<<"quantite : "<<t.Quantite<<endl;
    return flux;
}
// definition methodes classe Titre
int Titre::GetQuantite()const{return Quantite;}
string Titre::GetNom()const{return Nom;}
Titre::Titre(){}
Titre::Titre(string n,int q){
    Nom=n;
    Quantite=q;
}
void Titre :: AjouterQuantite(int q){
    Quantite+=q;
}
void Titre :: RetirerQuantite(int q){
    Quantite-=q;
}
ostream& operator<<(ostream& flux,const Titre& t){
	flux << "(" <<t.Nom << "," <<t.Quantite << ")"<<endl;
    return flux;
}
//definition methodes class Portefeuille
Portefeuille::Portefeuille(string n,vector<Titre*> t){
    NomProp=n;
    Titres=t;
    NbTitres=0;
}
string Portefeuille::GetNomProp()const{return NomProp;}
int Portefeuille::GetNbTitres()const{return NbTitres;}
vector<Titre*> Portefeuille::GetTitres()const{return Titres;}
int Portefeuille::ChercherUnTitre(string n)const{
    int i=0;
    while(i<NbTitres){
        if (Titres[i]->GetNom()==n){
            return i;
        }
        else {i++;}
    }
    return -1;
}
void Portefeuille::AjouterTitre(Titre& T){
    int i= ChercherUnTitre(T.GetNom());
    if(i!=-1)
        Titres[i]->AjouterQuantite(T.GetQuantite());
    else{
        Titre* t = new Titre;
        *t=T;
        Titres.push_back(t);
        NbTitres+=1;
    }
}
int Portefeuille :: RetirerTitre(Titre& T){
    int i=ChercherUnTitre(T.GetNom());
    if(i!=-1){
        if(T.GetQuantite()>=Titres[i]->GetQuantite()){
            Titres.erase(Titres.begin()+i);
            NbTitres--;
            return Titres[i]->GetQuantite();
        }
        else{
            Titres[i]->RetirerQuantite(T.GetQuantite());
            return T.GetQuantite();
        }
    }
    return 0;
}
ostream& operator<<(ostream& flux,const Portefeuille& P){
	flux<<"La portefeuille de "<<P.NomProp<<" comporte "<<P.NbTitres<<" titres:"<<endl;
    for(int i=0;i<P.NbTitres;i++){
        cout<<i<<"- "<<*(P.Titres[i])<<endl;
    }
    return flux;
}

//definition methodes classe Trader
Trader::Trader(){}
Trader::Trader(string n,TypeTrader t):NomProp(n),Type(t){}
TypeTrader Trader::GetType() const{return Type;}
string Trader::GetNomProp() const{return NomProp;}
ostream& operator<<(ostream& flux,Trader& T){
    flux<<"nom proprietaire "<<T.NomProp<<endl;
    flux<<"type trader"<<T.Type<<endl;
    return flux;
}
//definition methodes classe TraderHumain
TraderHumain::TraderHumain(string n,TypeTrader t):Trader(n,t){}
Transaction TraderHumain::PrendreDecision(Bourse& b,const Portefeuille& p){
    TypeTransaction t;
    int i,k,q;
    string n,s="oui",n1;
    vector<PrixJournalier> v;
    vector<Titre*> T;
    PrixJournalier* a;
    Date d;
    while(s!="non"){
        cout<<"voulez-vous avoir des infos sur la bourse?(oui/non)"<<endl;
        cin>>s;
        if(s=="oui"){
            cout<<"1-chercher une action"<<endl<<"2-chercher l'historique d'une action"<<endl<<"3-afficher les actions du jour"<<endl<<"4-chercher l'historique d'un jour"<<endl;
            cin>>i;
            switch(i){
              case 1:
                cout<<"entre nom action"<<endl;
                cin>>n1;
                cout<<"entrer la date"<<endl;
                cin>>d;
                a=b.ChercherUneAction(n1,d);
                cout<<*a;
                break;
              case 2:
                cout<<"entre nom action"<<endl;
                cin>>n1;
                cout<<"entrer le nombre de jours"<<endl;
                cin>>k;
                v=b.ChercherHistoriqueAction(n1,k);
                for(unsigned int i=0;i<v.size();i++){
                   cout<<v.at(i);
                }
                break;
             case 3:
                v=b.ActionsJourActuel();
                for(unsigned int i=0;i<v.size();i++){
                   cout<<v.at(i);
                }
                break;
             case 4:
                cout<<"entrer la date"<<endl;
                cin>>d;
                v=b.ChercherHistoriqueJour(d);
                for(unsigned int i=0;i<v.size();i++){
                  cout<<v.at(i);
                }
                break;
            }
        }
    }
    cout<<"voulez vous consulter votre portefeuille?(oui/non)"<<endl;
    cin>>s;
    if(s=="oui"){
        cout<<p;
         /*T=p.GetTitres();
         for(unsigned int i=0;i<T.size();i++){
                cout<<(*T[i]);
            } */
        }
    cout<<"entrer le type de la transaction"<<endl;
    cout<<"0-rien faire"<<endl;
    cout<<"1-vente"<<endl;
    cout<<"2-achat"<<endl;
    cin>>i;
    switch(i){
        case 0:
           t=RIEN;
           break;
        case 1:
           t=VENTE;
           break;
        case 2:
           t=ACHAT;
           break;
    }

    if(t==VENTE || t==ACHAT){
       cout<<"entrer nom action"<<endl;
       cin>>n;
       cout<<"entrer la quantite d'actions"<<endl;
       cin>>q;
       Transaction T(t,n,q);
       return T;
    }
    else{
       Transaction T(t,"",0);
       return T;
    }
}
//definition methodes classe TraderAlea
TraderAlea::TraderAlea(string n,TypeTrader t):Trader(n,t){}
Transaction TraderAlea::PrendreDecision(Bourse& b,const Portefeuille& p){
    TypeTransaction t;
    int i,q;
    string n;
    vector<PrixJournalier> v;
    srand((unsigned int) time(0));
    i=rand()%3;
    switch(i){
        case 0:
           t=RIEN;
           break;
        case 1:
           t=VENTE;
           break;
        case 2:
           t=ACHAT;
           break;
    }
    if(t==VENTE || t==ACHAT){
        v=b.ActionsJourActuel();
        i=rand()%v.size();
        n=v[i].GetNomAction();
       q=rand()%100;
       Transaction T(t,n,q);
       return T;
    }
    else{
       Transaction T(t,"",0);
       return T;
    }
}
//definition methodes classe traderalgo
TraderAlgo::TraderAlgo(string n, TypeTrader t):Trader(n,t){
    nbDecision=0;
}
Transaction TraderAlgo::PrendreDecision(Bourse& b,const Portefeuille& p){
    TypeTransaction t;
    int q;
    string n;
    int i=nbDecision%3;
    vector<PrixJournalier> v(b.ActionsJourActuel());
    switch (i){
        case 0:
        t=ACHAT;
        sort(v.begin(),v.end());
        n=v[0].GetNomAction();

        q=10;
        break;
        case 1:
        t=RIEN;
        n="";
        q=0;
        break;
        case 2:
           t=VENTE;
           for(unsigned int j=0;j<p.GetTitres().size();j++){
              Date d=b.GetDateActuel();
              PrixJournalier* pr=b.ChercherUneAction(p.GetTitres()[j]->GetNom(),d);
              v.push_back(*pr);
            }
            sort(v.begin(), v.end());
            reverse(v.begin(), v.end());
            n=v[0].GetNomAction();
            int x=p.ChercherUnTitre(n);
            if(x!=-1){
                q=p.GetTitres()[x]->GetQuantite()/2;
            }
            else{q=0;}
            break;

    }
    nbDecision++;
    Transaction T(t,n,q);
    return T;
}
//definition methodes classe Simulation
Simulation::Simulation(Bourse* b, Trader* t,double si,Portefeuille p,int n, Date d):bourse(b),Tr(t),P(p),DateDepart(d){
    if(si>=0){SoldeInitial=si;}
    Solde=si;
    if(n>0){NbJour=n;}
    DateActuel=DateDepart;
}
Bourse* Simulation::GetBourse() const{return bourse;}
Trader* Simulation::GetTr() const{return Tr;}
double Simulation::GetSoldeInitial() const{return SoldeInitial;}
double Simulation::GetSolde() const{return Solde;}
Portefeuille Simulation::GetP() const{return P;}
int Simulation::GetNbJour() const{return NbJour;}
Date Simulation::GetDateActuel() const{return DateActuel;}
Date Simulation::GetDateDepart() const{return DateDepart;}
void Simulation::AcheterUneAction(Titre& t){
    PrixJournalier *A ;
    A= bourse->ChercherUneAction(t.GetNom(),DateActuel);
    if(A!=NULL ){
       if(Solde >= t.GetQuantite()*A->Getprix()){
            Solde-=(t.GetQuantite()*A->Getprix());
            P.AjouterTitre(t);
       }
    }
}
void Simulation::VendreUneAction(Titre& T){
    PrixJournalier *A ;
    int i=0;
    A= bourse->ChercherUneAction(T.GetNom(),DateActuel);
    if(A!=NULL ){
            i=P.RetirerTitre(T);
            Solde+= i*A->Getprix();
    }
}
 void Simulation::IncrementerJour(){
     DateActuel.DateJourSuivant();
 }
double Simulation::CalculerGain(){
    double s=0;
    PrixJournalier* A;
    Date d=DateActuel;
    for(unsigned int i=0;i<P.GetTitres().size();i++){
        A=NULL;
       while(A==NULL){
           A=bourse->ChercherUneAction(P.GetTitres()[i]->GetNom(),d);
           d.DateJourPrecedent();
       }
       s+=P.GetTitres()[i]->GetQuantite()*A->Getprix();
    }
    return s+Solde-SoldeInitial;
}
double Simulation::GainPourcentage(){
    double s=CalculerGain();

    return (s/SoldeInitial)*100;
}
void Simulation::LancerSimulation(){
    int j=0;
    while(j<NbJour){
            vector<PrixJournalier> v(bourse->ActionsJourActuel());
            if(v.empty()){
                bourse->plusJour();
                IncrementerJour();
                j++;
            }
            else{
                 Transaction tr;
                 tr=Tr->PrendreDecision(*bourse,P);
                 cout<<tr;
                 Titre tit(tr.GetNomAction(),tr.GetQuantite());
                 if(tr.GetType()==ACHAT){AcheterUneAction(tit);}
                 if(tr.GetType()==VENTE){VendreUneAction(tit);}
                 if(tr.GetType()==RIEN){
                    IncrementerJour();
                    bourse->plusJour();
                    j++;}
            }
    }
}
