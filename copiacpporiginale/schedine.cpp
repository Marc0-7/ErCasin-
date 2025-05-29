#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

struct squadra 
{
    string nome;
    int numero_giocatori, overall;
};

struct partita 
{
    string squadra1, squadra2;
    int goal1, goal2, tiri1, tiri2, angolo1, angolo2, gialli1, gialli2;
    float quota1, quota2;
    float quotaX;
    char giocata = '0'; // '1', '2', 'X', '0'
    float puntata = 0;
    bool vinta = false;
};

struct risultati 
{
    string nome;
    int vittorie = 0;
    int perdite = 0;
    int pareggi = 0;
    int goal = 0;
    int goal_presi = 0;
    int gialli = 0;
    int rossi = 0;
    int tiri = 0;
};

void calcolaQuote(partita& p, const squadra& s1, const squadra& s2) {
    float overall1 = s1.overall;
    float overall2 = s2.overall;

    float total = overall1 + overall2;

    // Usa potenza per esaltare le differenze
    float prob1 = pow(overall1 / total, 1.35f);
    float prob2 = pow(overall2 / total, 1.35f);

    // Quote inverse delle probabilità
    p.quota1 = min(10.0f, max(1.05f, 1.0f / prob1));
    p.quota2 = min(10.0f, max(1.05f, 1.0f / prob2));
    p.quotaX = min(10.0f, ((p.quota1 + p.quota2) / 2.0f) * 1.05f); // pareggio leggermente meno probabile
}

void simulaPartita(partita& p) {
    p.goal1 = rand() % 6;
    p.goal2 = rand() % 6;
    p.tiri1 = rand() % 16 + 5;
    p.tiri2 = rand() % 16 + 5;
    p.angolo1 = rand() % 10;
    p.angolo2 = rand() % 10;
    p.gialli1 = rand() % 4;
    p.gialli2 = rand() % 4;
}

int main() 
{
    srand(time(0));

    int numero_squadre;
    cout << "Inserisci il numero di squadre: ";
    cin >> numero_squadre;

    vector<squadra> squadre(numero_squadre);
    vector<risultati> stats(numero_squadre);

    for (int i = 0; i < numero_squadre; i++)
	{
        cout << "\nInserisci il nome della squadra " << i + 1 << ": ";
        cin.ignore();
        getline(cin, squadre[i].nome);

        squadre[i].numero_giocatori = rand() % 6 + 11;
        squadre[i].overall = rand() % 91 + 10;

        stats[i].nome = squadre[i].nome;

        cout << "Numero giocatori: " << squadre[i].numero_giocatori
             << ", Overall: " << squadre[i].overall << endl;
    }

    float saldo;
    cout << "\nCon quanti euro vuoi iniziare? ";
    cin >> saldo;

    int giocate_vinte = 0;
    float vincite_totali = 0;

    for (int i = 0; i < numero_squadre; i++) 
	{
        for (int j = i + 1; j < numero_squadre; j++) 
		{
            partita p;
            p.squadra1 = squadre[i].nome;
            p.squadra2 = squadre[j].nome;

            calcolaQuote(p, squadre[i], squadre[j]);

            // Pre-partita
            cout << "\n-----------------------------" << endl;
            cout << "Partita: " << p.squadra1 << " vs " << p.squadra2 << endl;
            cout << p.squadra1 << " - Overall: " << squadre[i].overall << endl;
            cout << p.squadra2 << " - Overall: " << squadre[j].overall << endl;
            cout << "Quote: 1 = " << p.quota1 << ", X = " << p.quotaX << ", 2 = " << p.quota2 << endl;
            cout << "Saldo attuale: " << saldo << " euro" << endl;

            cout << "Inserisci la tua giocata (1/X/2 o 0 per saltare): ";
            cin >> p.giocata;
            p.giocata = toupper(p.giocata);

            while (p.giocata != '1' && p.giocata != '2' && p.giocata != 'X' && p.giocata != '0') 
			{
                cout << "Scelta non valida. Inserisci 1, X, 2 o 0: ";
                cin >> p.giocata;
                p.giocata = toupper(p.giocata);
            }

            if (p.giocata != '0') 
			{
                do 
				{
                    cout << "Quanto vuoi puntare? ";
                    cin >> p.puntata;
                    if (p.puntata > saldo) 
                        cout << "Non hai abbastanza saldo!" << endl;
                } 
				while (p.puntata > saldo || p.puntata <= 0);
                saldo -= p.puntata;
            }

            // Simula partita
            simulaPartita(p);
            cout << "\nRisultato: " << p.squadra1 << " " << p.goal1 << " - " << p.goal2 << " " << p.squadra2 << endl;

            // Calcola risultato
            char risultato;
            if (p.goal1 > p.goal2) risultato = '1';
            else if (p.goal2 > p.goal1) risultato = '2';
            else risultato = 'X';

            // Aggiorna statistiche
            stats[i].goal += p.goal1;
            stats[j].goal += p.goal2;

            stats[i].goal_presi += p.goal2;
            stats[j].goal_presi += p.goal1;

            stats[i].tiri += p.tiri1;
            stats[j].tiri += p.tiri2;

            stats[i].gialli += p.gialli1;
            stats[j].gialli += p.gialli2;

            stats[i].rossi = stats[i].gialli / 2;
            stats[j].rossi = stats[j].gialli / 2;

            if (risultato == '1') 
			{
                stats[i].vittorie++;
                stats[j].perdite++;
            } 
			else if (risultato == '2') 
			{
                stats[j].vittorie++;
                stats[i].perdite++;
            } 
			else 
			{
                stats[i].pareggi++;
                stats[j].pareggi++;
            }

            // Controlla scommessa
            if (p.giocata != '0') 
			{
                if (p.giocata == risultato) 
				{
                    float quota_vinta = (p.giocata == '1') ? p.quota1 :
                                        (p.giocata == '2') ? p.quota2 : p.quotaX;

                    float vincita = p.puntata * quota_vinta;
                    saldo += vincita;
                    vincite_totali += vincita;
                    giocate_vinte++;

                    cout << "? Hai vinto! Guadagni: " << vincita << " euro" << endl;
                } 
				else
                    cout << "? Hai perso la scommessa." << endl;
                cout << "Saldo aggiornato: " << saldo << " euro" << endl;
            }
        }
    }

    // Riepilogo finale
    cout << "\n===== FINE GIOCO =====" << endl;
    cout << "Giocate vinte: " << giocate_vinte << endl;
    cout << "Vincite totali: " << vincite_totali << " euro" << endl;
    cout << "Saldo finale: " << saldo << " euro" << endl;

    cout << "\n===== STATISTICHE SQUADRE =====" << endl;
    for (const auto& r : stats) 
	{
        cout << "\nSquadra: " << r.nome << endl;
        cout << " Vittorie: " << r.vittorie
             << " | Pareggi: " << r.pareggi
             << " | Sconfitte: " << r.perdite << endl;
        cout << " Goal fatti: " << r.goal
             << " | Goal subiti: " << r.goal_presi << endl;
        cout << " Tiri totali: " << r.tiri << endl;
        cout << " Gialli: " << r.gialli << " | Rossi: " << r.rossi << endl;
    }

    return 0;
}
