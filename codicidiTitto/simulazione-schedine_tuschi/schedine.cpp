#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <unistd.h>
using namespace std;

struct squadra {
	string nome;
	int numero_giocatori;
	int overall;
};

struct partita {
	string squadra1;
	string squadra2;
	int goal1 = 0;
	int goal2 = 0;
	int tiri1 = 0;
	int tiri2 = 0;
	int angolo1 = 0;
	int angolo2 = 0;
	int gialli1 = 0;
	int gialli2 = 0;
	int rossi1 = 0;
	int rossi2 = 0;
	float quota1 = 0;
	float quota2 = 0;
	float quotaX = 0;
	char giocata = '0'; // '1', '2', 'X', '0' 
	float puntata = 0;
	bool vinta = false;
};

struct risultati {
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

void stampaCoppa() {
	cout << R"(
       ___________
      '._==_==_=_.'
      .-\:      /-.
     | (|:.     |) |
      '-|:.     |-'
        \::.    /
         '::. .'
           ) (
         _.' '._
        `"""""""`

 COMPLIMENTI! HAI VINTO LA SCOMMESSA!
    )" << endl;
}


void calcolaQuote(partita& p, const squadra& s1, const squadra& s2) {
	float overall1 = s1.overall;
	float overall2 = s2.overall;

	float total = overall1 + overall2;

	// Usa potenza per esaltare le differenze
	float prob1 = pow(overall1 / total, 1.35f);
	float prob2 = pow(overall2 / total, 1.35f);

	// Quote inverse delle probabilitC
	p.quota1 = min(10.0f, max(1.05f, 1.0f / prob1));
	p.quota2 = min(10.0f, max(1.05f, 1.0f / prob2));
	p.quotaX = min(10.0f, ((p.quota1 + p.quota2) / 2.0f) * 1.05f); // pareggio leggermente meno probabile
}


void gestioneEventi(int minuto, partita& p, squadra& s1, squadra& s2) {
	int evento = rand() % 100;

	if (evento < 5) {
		sleep(1);
		cout << "[" << minuto << "''] " << s1.nome << " guadagna un calcio d'angolo." << endl;
		p.angolo1++;
	} else if (evento < 10) {
		sleep(1);
		cout << "[" << minuto << "''] " << s2.nome << " conquista un corner pericoloso!" << endl;
		p.angolo2++;
	} else if (evento < 12) {
		sleep(1);
		cout << "[" << minuto << "''] Ammonizione per un giocatore del " << s1.nome << "." << endl;
		p.gialli1++;
	} else if (evento < 14) {
		sleep(1);
		cout << "[" << minuto << "''] Cartellino giallo ad un giocatore di " << s2.nome << " per gioco scorretto." << endl;
		p.gialli2++;
	} else if (evento < 15) {
		sleep(1);
		cout << "[" << minuto << "''] Espulsione diretta per " << s1.nome << "!" << endl;
		p.rossi1++;
	} else if (evento < 16) {
		sleep(1);
		cout << "[" << minuto << "''] " << s2.nome << " prende un cartellino rosso!" << endl;
		p.rossi2++;
	} else if (evento < 30) {
		sleep(1);
		cout << "[" << minuto << "''] " << s1.nome << " tenta un tiro dalla distanza..." << flush;
		sleep(1);
		p.tiri1++;
		float chance = (float)(rand() % 100) / 100.0f;
		float goalProb = s1.overall / (s1.overall + s2.overall + 1.0f);
		if (chance < goalProb * 0.4f) {
			cout << " GOL!" << endl;
			p.goal1++;
		} else if (chance < goalProb * 0.6f) {
			cout << " ...palo clamoroso!" << endl;
		} else {
			cout << " ...fuori di poco!" << endl;
		}
	} else if (evento < 44) {
		cout << "[" << minuto << "''] Azione veloce del " << s2.nome << ", tiro improvviso..." << flush;
		sleep(1);
		p.tiri2++;
		sleep(1);
		float chance = (float)(rand() % 100) / 100.0f;
		float goalProb = s2.overall / (s1.overall + s2.overall + 1.0f);
		if (chance < goalProb * 0.4f) {
			cout << " RETE!" << endl;
			p.goal2++;
		} else if (chance < goalProb * 0.6f) {
			cout << " ...traversa!" << endl;
		} else {
			cout << " ...il portiere salva tutto!" << endl;
		}
	} else if (evento < 52) {
		sleep(1);
		cout << "[" << minuto << "''] Il ritmo si abbassa, le squadre si studiano." << endl;
	} else if (evento < 58) {
		sleep(1);
		cout << "[" << minuto << "''] Che azione spettacolare del " << s1.nome << ", ma manca l'ultimo passaggio!" << endl;
	} else if (evento < 64) {
		sleep(1);
		cout << "[" << minuto << "''] Giocata tecnica del " << s2.nome << ", ma c'è fuorigioco!" << endl;
	} else if (evento < 68) {
		sleep(1);
		cout << "[" << minuto << "''] Il portiere del " << s1.nome << " compie un miracolo!" << endl;
	} else if (evento < 72) {
		sleep(1);
		cout << "[" << minuto << "''] Sostituzione nel " << s2.nome << ", entra un volto nuovo per cambiare la partita." << endl;
	} else if (evento < 74) {
		sleep(1);
		cout << "[" << minuto << "''] Infortunio a un giocatore del " << s1.nome << ", gioco momentaneamente fermo." << endl;
	}
	// 26% di probabilità che non succeda nulla
}


void simulaPartita(partita& p, squadra& s1, squadra& s2) {
	cout << "\nInizia la partita tra " << s1.nome << " e " << s2.nome << "!" << endl;
	sleep(2);

	for (int minuto = 1; minuto <= 90; minuto += 5) {
		gestioneEventi(minuto, p, s1, s2);
		sleep(1);
	}

	cout << "\nFischio finale! La partita termina con il risultato:" << endl;
	cout << s1.nome << " " << p.goal1 << " - " << p.goal2 << " " << s2.nome << endl;
}


int main() {
	srand(time(0));

	int numero_squadre;
	cout << "Inserisci il numero di squadre: ";
	cin >> numero_squadre;

	vector<squadra> squadre(numero_squadre);
	vector<risultati> stats(numero_squadre);

	for (int i = 0; i < numero_squadre; i++) {
		cout << "\nInserisci il nome della squadra " << i + 1 << ": ";
		if (i == 0)cin.ignore();
		getline(cin, squadre[i].nome);

		squadre[i].numero_giocatori = rand() % 6 + 11;
		squadre[i].overall = rand() % 91 + 10;

		stats[i].nome = squadre[i].nome;

		cout << "Calcolo informazioni..." << endl;
		sleep(1);
		cout << "Numero giocatori: " << squadre[i].numero_giocatori << flush;
		sleep(1);
		cout << ", Overall: " << squadre[i].overall << endl;
	}

	float saldo;
	cout << "\nCon quanti euro vuoi iniziare? ";
	cin >> saldo;

	int giocate_vinte = 0;
	float vincite_totali = 0;

	for (int i = 0; i < numero_squadre; i++) {
		for (int j = i + 1; j < numero_squadre; j++) {
			partita p;
			p.squadra1 = squadre[i].nome;
			p.squadra2 = squadre[j].nome;

			calcolaQuote(p, squadre[i], squadre[j]);

			// Pre-partita
			cout << "\n-----------------------------" << endl;
			cout << p.squadra1 << " vs " << p.squadra2 << endl << endl;
			cout << "Quote: " << setw(3)
			     << "1 = " << p.quota1 << endl << setw (11)
			     << "X = " << p.quotaX << endl << setw(11)
			     << "2 = " << p.quota2 << endl;
			cout << "Saldo attuale: " << saldo << " euro" << endl;
			cout << "-----------------------------\n";

			cout << "Inserisci la tua giocata (1/X/2 o 0 per saltare): ";
			cin >> p.giocata;
			p.giocata = toupper(p.giocata);

			while (p.giocata != '1' && p.giocata != '2' && p.giocata != 'X' && p.giocata != '0') {
				cout << "Scelta non valida. Inserisci 1, X, 2 o 0: ";
				cin >> p.giocata;
				p.giocata = toupper(p.giocata);
			}

			if (p.giocata != '0') {
				do {
					cout << "Quanto vuoi puntare? ";
					cin >> p.puntata;
					if (p.puntata > saldo) {
						cout << "Non hai abbastanza saldo!" << endl;
					}
				} while (p.puntata > saldo || p.puntata <= 0);
				saldo -= p.puntata;
			}

			// Simula partita
			simulaPartita(p, squadre[i], squadre[j]);

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

			stats[i].rossi += p.rossi1;
			stats[j].rossi += p.rossi2;

			if (risultato == '1') {
				stats[i].vittorie++;
				stats[j].perdite++;
			} else if (risultato == '2') {
				stats[j].vittorie++;
				stats[i].perdite++;
			} else {
				stats[i].pareggi++;
				stats[j].pareggi++;
			}

			// Controlla scommessa
			if (p.giocata != '0') {
				if (p.giocata == risultato) {
					float quota_vinta;
					if (p.giocata == '1')
						quota_vinta = p.quota1;
					else if (p.giocata == '2')
						quota_vinta = p.quota2;
					else
						quota_vinta = p.quotaX;


					float vincita = p.puntata * quota_vinta;
					saldo += vincita;
					vincite_totali += vincita;
					giocate_vinte++;

					stampaCoppa();
				} else {
					cout << "Hai perso la scommessa." << endl;
				}
				cout << "Saldo aggiornato: " << saldo << " euro" << endl;
			}
		}
	}

	sleep(2);
	cout << "Sono finite tutte le partite, vediamo le statistiche: " << endl;
	sleep(2);

	// Riepilogo finale
	cout << "\n===== FINE GIOCO =====" << endl;
	cout << "Giocate vinte: " << giocate_vinte << endl;
	cout << "Vincite totali: " << vincite_totali << " euro" << endl;
	cout << "Saldo finale: " << saldo << " euro" << endl;

	cout << "\n===== STATISTICHE SQUADRE =====" << endl;
	for (const auto& r : stats) {
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
