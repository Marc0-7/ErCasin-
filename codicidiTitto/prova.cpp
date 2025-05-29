#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cctype>
using namespace std;
namespace fs = std::filesystem;

struct UserStats {
    string nome_utente;
    int giocate_vinte = 0;
    float vincite_totali = 0;
    float saldo_iniziale = 0;
    float saldo_finale = 0;
    int partite_giocate = 0;
};

void salvaStats(const UserStats& stats) {
    string filename = stats.nome_utente + "_stats.txt";
    ofstream file(filename);
    if (!file) {
        cerr << "Errore nella scrittura del file " << filename << endl;
        return;
    }
    file << stats.nome_utente << endl;
    file << stats.giocate_vinte << endl;
    file << stats.vincite_totali << endl;
    file << stats.saldo_iniziale << endl;
    file << stats.saldo_finale << endl;
    file << stats.partite_giocate << endl;
    file.close();
}

bool caricaStats(UserStats& stats) {
    string filename = stats.nome_utente + "_stats.txt";
    ifstream file(filename);
    if (!file) {
        return false;
    }
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
    if (!script) {
        cerr << "Errore nella creazione dello script .bat" << endl;
        return;
    }
    script << "@echo off\n";
    script << "echo Caricamento statistiche su GitHub...\n";
    script << "git pull\n";
    script << "git add " << nome_utente << "_stats.txt\n";
    script << "git commit -m \"Aggiornamento stats utente: " << nome_utente << "\"\n";
    script << "git push\n";
    script << "pause\n";
    script.close();
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

int main() {
    cout << "Vuoi accedere come utente autenticato? (s/n): ";
    char scelta;
    cin >> scelta;

    UserStats stats;

    if (tolower(scelta) == 's') {
        cout << "Inserisci il tuo nome utente (senza spazi): ";
        cin >> stats.nome_utente;

        bool esiste = caricaStats(stats);
        if (esiste) {
            cout << "\nAccesso riuscito! Dati caricati da file.\n";
            mostraStats(stats);
        } else {
            cout << "\nNuovo utente! Verrà creato un file per salvare i tuoi dati.\n";
            salvaStats(stats); // Salva i dati vuoti iniziali
        }

        creaScriptUpload(stats.nome_utente);
        cout << "Script per upload generato: upload_" << stats.nome_utente << ".bat\n";
    } else {
        cout << "Hai scelto di giocare senza autenticazione.\n";
    }

    // Qui inserirai la logica del gioco (simulazioni, aggiornamento stats ecc.)

    return 0;
}
