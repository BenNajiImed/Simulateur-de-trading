#ifndef TRADING_HPP
#define TRADING_HPP
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
using namespace std;
class Date{
    private:
       int Jour;
       int Mois;
       int Annee;
    public:
       Date();
       Date(int,int,int);
       int GetJour() const ;
       int GetMois() const ;
       int GetAnnee() const;
       void DateJourSuivant();
       void DateJourPrecedent();
       void LireDate();
       friend ostream& operator<<(ostream&,const Date&) ;
       friend istream& operator>> (istream& flux, Date& d);
       friend bool operator==(Date const &a, Date const& b);
};

class PrixJournalier{
    private:
       Date date;
       string NomAction;
       double prix;
    public:
       PrixJournalier(){};
       PrixJournalier(Date&,string,double);
       Date Getdate() const;
       string GetNomAction() const;
       double Getprix() const;
       friend ostream& operator<<(ostream& flux, const PrixJournalier& p);
       friend istream& operator>> (istream& flux, PrixJournalier& p);
       friend bool operator<(PrixJournalier& , PrixJournalier&);
};

class Bourse{
    protected:
       string Nom;
       Date DateActuel;
    public:
       Bourse();
       Bourse(string, Date&);
       string GetNom()const;
       Date GetDateActuel()const;
       void plusJour();
       virtual PrixJournalier* ChercherUneAction(string,Date&)=0;
       virtual vector<PrixJournalier> ChercherHistoriqueAction(string,int)=0;
       virtual vector<PrixJournalier> ActionsJourActuel()=0;
       virtual vector<PrixJournalier> ChercherHistoriqueJour(Date&)=0;
       friend ostream& operator<<(ostream&,const Bourse&) ;
       virtual ~Bourse(){}
};
class BourseVecteur: public Bourse{
    private:
        vector<PrixJournalier> Historique;
    public:
        BourseVecteur();
        BourseVecteur(string, Date&, vector<PrixJournalier>&);
        PrixJournalier* ChercherUneAction(string,Date&);
        vector<PrixJournalier> ChercherHistoriqueAction(string,int);
        vector<PrixJournalier> ActionsJourActuel();
        vector<PrixJournalier> ChercherHistoriqueJour(Date&);
};
class BourseFichier : public Bourse{
    private:
        ifstream Historique;
    public:
        BourseFichier(string, Date&, const char*);
        PrixJournalier* ChercherUneAction(string,Date&);
        vector<PrixJournalier> ChercherHistoriqueAction(string,int);
        vector<PrixJournalier> ActionsJourActuel();
        vector<PrixJournalier> ChercherHistoriqueJour(Date&);
};
enum TypeTransaction{RIEN=0,VENTE=1,ACHAT=2};
class Transaction{
    private:
        TypeTransaction Type;
        string NomAction;
        int Quantite;
    public:
        Transaction();
        Transaction(TypeTransaction,string,int);
        TypeTransaction GetType()const;
        string GetNomAction()const;
        int GetQuantite()const;
        friend ostream& operator<<(ostream&,const Transaction&) ;
};
class Titre{
    private:
        string Nom;
        int Quantite;
    public:
        Titre();
        Titre(string,int);
        string GetNom()const;
        int GetQuantite()const;
        void AjouterQuantite(int);
        void RetirerQuantite(int);
        friend ostream& operator<<(ostream&,const Titre&) ;
};
class Portefeuille{
    private:
        string NomProp;
        vector<Titre*> Titres;
        int NbTitres;
    public:
        Portefeuille(string,vector<Titre*>);
        string GetNomProp()const;
        int GetNbTitres()const;
        vector<Titre*> GetTitres()const;
        int ChercherUnTitre(string)const;
        void AjouterTitre(Titre&);
        int RetirerTitre(Titre&);
        friend ostream& operator<<(ostream&,const Portefeuille&);
};


///////////////////////////////////utilise pour le sort de vecteur/////////////////////////////////////////
struct Pgreater{
    bool operator()(const PrixJournalier& P1,const PrixJournalier& P2){
        return P1.Getprix()<P2.Getprix();
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum TypeTrader{HUMAIN=0,ALGO=1,ALEA=2};
class Trader{
    protected:
        string NomProp;
        TypeTrader Type;
    public:
        Trader();
        Trader(string,TypeTrader);
        TypeTrader GetType()const;
        string GetNomProp() const;
        virtual Transaction PrendreDecision(Bourse&,const Portefeuille&)=0  ;
        friend ostream& operator<<(ostream&,Trader&) ;
};
class TraderHumain:public Trader{
    public:
        TraderHumain(string,TypeTrader);
        Transaction PrendreDecision(Bourse&,const Portefeuille&);
};
class TraderAlgo:public Trader{
    private:
        int nbDecision;
    public:
        TraderAlgo(string,TypeTrader);
        Transaction PrendreDecision(Bourse&,const Portefeuille&);
};
class TraderAlea:public Trader{
    public:
        TraderAlea(string,TypeTrader);
        Transaction PrendreDecision(Bourse&,const Portefeuille&);
};

class Simulation{
    private:
        Bourse* bourse;
        Trader* Tr;
        double SoldeInitial;
        double Solde;
        Portefeuille P;
        int NbJour;
        Date DateDepart;
        Date DateActuel;
     public:
        Simulation(Bourse*,Trader*,double,Portefeuille,int,Date);
        Bourse* GetBourse() const;
        Trader* GetTr() const;
        double GetSoldeInitial() const;
        double GetSolde() const;
        Portefeuille GetP() const;
        int GetNbJour() const;
        Date GetDateActuel() const;
        Date GetDateDepart() const;
        void AcheterUneAction(Titre&);
        void VendreUneAction(Titre&);
        void IncrementerJour();
        void LancerSimulation();
        double CalculerGain();
        double GainPourcentage();
        ~Simulation(){};
};

#endif
