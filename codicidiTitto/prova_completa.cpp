#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

// --- STRUTTURE ---
struct squadra {
	string nome;
	int numero_giocatori;
	int overall;
};

struct partita {
	string squadra1;
	string squadra2;
	int goal1;
	int goal2;
	int tiri1;
	int tiri2;
	int angolo1;
	int angolo2;
	int gialli1;
	int gialli2;
	int rossi1;
	int rossi2;
	float quota1;
	float quota2;
	float quotaX;
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

struct UserStats {
	string nome_utente;
	int giocate_vinte = 0;
	float vincite_totali = 0;
	float saldo_iniziale = 0;
	float saldo_finale = 0;
	int partite_giocate = 0;
};

// --- GESTIONE STATS ---
void salvaStats(const UserStats& stats) {
	string filename = stats.nome_utente + "_stats.txt";
	ofstream file(filename);
	if (file.is_open()) {
		file << stats.nome_utente << '\n';
		file << stats.giocate_vinte << '\n';
		file << stats.vincite_totali << '\n';
		file << stats.saldo_iniziale << '\n';
		file << stats.saldo_finale << '\n';
		file << stats.partite_giocate << '\n';
		file.close();
	}
}

bool caricaStats(UserStats& stats) {
	string filename = stats.nome_utente + "_stats.txt";
	ifstream file(filename);
	if (!file.is_open()) return false;

	getline(file, stats.nome_utente);
	file >> stats.giocate_vinte;
	file >> stats.vincite_totali;
	file >> stats.saldo_iniziale;
	file >> stats.saldo_finale;
	file >> stats.partite_giocate;
	file.close();
	return true;
}

void creaScriptUpload(const string& nome_utente) {
	string filename = "upload_" + nome_utente + ".bat";
	ofstream script(filename);
	if (script.is_open()) {
		script << "@echo off\n";
		script << "git pull\n";
		script << "git add " << nome_utente << "_stats.txt\n";
		script << "git commit -m \"Aggiorna stats per " << nome_utente << "\"\n";
		script << "git push\n";
		script << "pause\n";
		script.close();
	}
}

void mostraStats(const UserStats& stats) {
	cout << "\n===== STATISTICHE DI " << stats.nome_utente << " =====\n";
	cout << "Giocate vinte:     " << stats.giocate_vinte << endl;
	cout << "Vincite totali:    " << stats.vincite_totali << " euro\n";
	cout << "Saldo iniziale:    " << stats.saldo_iniziale << " euro\n";
	cout << "Saldo finale:      " << stats.saldo_finale << " euro\n";
	cout << "Partite giocate:   " << stats.partite_giocate << endl;
	cout << "=====================================\n";
}

// --- LOGICA GIOCO ---
void calcolaQuote(partita& p, const squadra& s1, const squadra& s2) {
	float overall1 = s1.overall;
	float overall2 = s2.overall;
	float total = overall1 + overall2;
	float prob1 = pow(overall1 / total, 1.35f);
	float prob2 = pow(overall2 / total, 1.35f);
	p.quota1 = min(10.0f, max(1.05f, 1.0f / prob1));
	p.quota2 = min(10.0f, max(1.05f, 1.0f / prob2));
	p.quotaX = min(10.0f, ((p.quota1 + p.quota2) / 2.0f) * 1.05f);
}

void simulaPartita(partita& p, squadra& s1, squadra& s2) {
	p.goal1 = rand() % ((s1.overall / 10) + 3);
	p.goal2 = rand() % ((s2.overall / 10) + 3);
	p.tiri1 = rand() % p.goal1 + rand() % 6;
	p.tiri2 = rand() % p.goal2 + rand() % 6;
	p.angolo1 = rand() % 10;
	p.angolo2 = rand() % 10;
	p.gialli1 = rand() % 4;
	p.gialli2 = rand() % 4;
	p.rossi1 = rand() % 3;
	p.rossi2 = rand() % 3;
}

int main() {
	srand(time(0));
	UserStats user;
	char scelta;

	cout << "Vuoi autenticarti per salvare i tuoi progressi? (s/n): ";
	cin >> scelta;

	if (tolower(scelta) == 's') {
		cout << "Inserisci il tuo nome utente (senza spazi): ";
		cin >> user.nome_utente;

		if (caricaStats(user)) {
			cout << "Utente trovato! Statistiche caricate.\n";
			mostraStats(user);
		} else {
			cout << "Nuovo utente, file di statistiche creato.\n";
			salvaStats(user);
		}
		creaScriptUpload(user.nome_utente);
	}

	int numero_squadre;
	cout << "\nInserisci il numero di squadre: ";
	cin >> numero_squadre;

	vector<squadra> squadre(numero_squadre);
	vector<risultati> stats(numero_squadre);

	for (int i = 0; i < numero_squadre; i++) {
		cout << "\nInserisci il nome della squadra " << i + 1 << ": ";
		if (i == 0) cin.ignore();
		getline(cin, squadre[i].nome);
		squadre[i].numero_giocatori = rand() % 6 + 11;
		squadre[i].overall = rand() % 91 + 10;
		stats[i].nome = squadre[i].nome;
		cout << "Giocatori: " << squadre[i].numero_giocatori
		     << ", Overall: " << squadre[i].overall << endl;
	}

	float saldo;
	cout << "\nSaldo iniziale: ";
	cin >> saldo;
	user.saldo_iniziale = saldo;

	for (int i = 0; i < numero_squadre; i++) {
		for (int j = i + 1; j < numero_squadre; j++) {
			partita p;
			p.squadra1 = squadre[i].nome;
			p.squadra2 = squadre[j].nome;

			calcolaQuote(p, squadre[i], squadre[j]);

			cout << "\n--- " << p.squadra1 << " vs " << p.squadra2 << " ---\n";
			cout << "Quote: 1 = " << p.quota1 << ", X = " << p.quotaX << ", 2 = " << p.quota2 << endl;
			cout << "Saldo attuale: " << saldo << " euro\n";
			cout << "Giocata? (1/X/2 o 0 per saltare): ";
			cin >> p.giocata;
			p.giocata = toupper(p.giocata);

			if (p.giocata != '0') {
				do {
					cout << "Puntata: ";
					cin >> p.puntata;
					if (p.puntata > saldo) cout << "Saldo insufficiente.\n";
				} while (p.puntata > saldo || p.puntata <= 0);
				saldo -= p.puntata;
			}

			simulaPartita(p, squadre[i], squadre[j]);
			cout << "Risultato: " << p.squadra1 << " " << p.goal1 << " - " << p.goal2 << " " << p.squadra2 << endl;

			char risultato;
			if (p.goal1 > p.goal2) risultato = '1';
			else if (p.goal2 > p.goal1) risultato = '2';
			else risultato = 'X';

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

			if (p.giocata != '0') {
				if (p.giocata == risultato) {
					float quota_vinta = (p.giocata == '1') ? p.quota1 : (p.giocata == '2') ? p.quota2 : p.quotaX;
					float vincita = p.puntata * quota_vinta;
					saldo += vincita;
					user.vincite_totali += vincita;
					user.giocate_vinte++;
					cout << "Hai vinto " << vincita << " euro!" << endl;
				} else {
					cout << "Hai perso la scommessa.\n";
				}
			}

			user.partite_giocate++;
		}
	}

	user.saldo_finale = saldo;
	if (!user.nome_utente.empty()) salvaStats(user);

	cout << "\n===== FINE GIOCO =====\n";
	cout << "Saldo finale: " << saldo << " euro\n";
	if (!user.nome_utente.empty()) mostraStats(user);

	return 0;
}
