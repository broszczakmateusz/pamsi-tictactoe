#include <iostream>
#include <cstdlib>

#define INFINITY 2147483647

using std::cout, std::cin , std::endl;

class Game {
private:
    int size; //rozmiar planszy
    int victoryCondition; //liczba znaków potrzebnych do zwyciestwa
    int depth; //glebokosc przeszukan algorytmu

    int ** board; //pola planszy
    int moves = 0; //liczba wykonanych ruchow

public:
    void setSize(int newSize) { size = newSize; }
    void setVictoryCondition(int newVictoryCondition) { victoryCondition = newVictoryCondition; }
    void setDepth(int newDepth) { depth = newDepth; }
    void makeBoard(){
        board = new int *[size];
        for (int i = 0; i < size; i++) {
            board[i] = new int[size];
        }
    };
    void fillWithZeros();
    void print();

    bool notAllowedToPlay(int x, int y);
    void aiMove();
    void playerMove();

    int minMaxAlgorithm(int algoDepth, bool isMaximizing, int alpha, int beta);

    void play();
    int isWin();
};

// wypelnia cala plansze zerami
void Game::fillWithZeros() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
        }
    }
}

// wyswietla plansze w terminalu
void Game::print() {

    system("clear");
    cout << "   \t";

    for (int i = 0; i < size; i++) { cout <<  i << "\t"; } //nr kolumny
    for (int i = 0; i < size; i++) { //pola w jednym wierszu

        cout << endl;
        cout << " " << i << " \t"; //nr wiersza

        for (int j = 0; j < size; j++) {
            switch (board[i][j]) {
                case 1: //kolko
                    if (j==size-1)    cout << "  O \t"; //dla ostatniej kolumny
                    else cout << "  O |\t"; //dla pozostalych
                    break;

                case -1: //krzyzyk
                    if (j==size-1)    cout << "  X \t"; //dla ostatniej kolumny
                    else cout << "  X |\t"; //dla pozostalych
                    break;

                default: //puste pole
                    if (j==size-1) cout << "    \t"; //dla ostatniej kolumny
                    else cout << "    |\t"; //dla pozostalych
                    break;
            }
        }
        cout << endl;
        cout << "   \t";
        if (i == size -1)   for (int l = 0; l < size; l++) { cout << "\t"; } //dla ostatniego wiersza nie rysuj kreski
        else  for (int l = 0; l < size; l++) { cout << "----\t"; } //kreska miedzy wierszami
    }
    cout << endl;

}

// sprawdza mozliwosc wykonania ruchu
bool Game::notAllowedToPlay(int x, int y) {
    //przekroczone zakresy
    if (x < 0 || x >= size)  return true;
    if (y < 0 || y >= size) return true;
    //czy wykonano juz ruch na tym polu
    if (board[x][y] != 0) return true;
    return false;
}


// ruch gracza
void Game::playerMove() {
    int x, y;
    // pobranie wsporzednych ruchu
    do {
        cout << endl;
        cout << "Twoja kolej!\n";
        cout << "wiersz: ";
        cin >> x;
        cout << "kolumna: ";
        cin >> y;
    } while (notAllowedToPlay(x, y)); //czy mozna zagrac ten ruch

    board[x][y] = -1; // ustawienie ze zagrano
    moves++; //zwiekszenie liczby wykonanych ruchow
}

// ruch sztucznej inteligencji
void Game::aiMove() {
    int value;
    int bestMove = -INFINITY;
    int bestMoveX = 0;
    int bestMoveY = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                board[i][j] = 1; //ustawnie ze zagrano ruch
                if (isWin() == 1) { // jesli od razu wygrana
                    value = INFINITY;
                } else {
                    value = minMaxAlgorithm(0, false, -INFINITY, INFINITY); //wywolanie minimax
                }
                board[i][j] = 0; //odznaczenie ze zagrano
                if (value > bestMove) {
                    bestMove = value;
                    bestMoveX = i;
                    bestMoveY = j;
                }
            }
        }
    }
    board[bestMoveX][bestMoveY] = 1; //wykonanie najlepszego ruchu
    moves++; //zwiekszenie liczby wykonanych ruchow
}

// algorytm minmax (wersja alfa-beta)
int Game::minMaxAlgorithm(int algoDepth, bool isMaximizing, int alpha, int beta) {
    int temp = isWin();

    // warunki stopu
    if (temp == -1) return -100 / depth;
    if (temp == 1) return 100 / depth;
    if (algoDepth == depth) return temp;

    int value;
    // Dla max
    if (isMaximizing) {
        int bestValue = -INFINITY;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) { //dla niezagranych ruchow
                    board[i][j] = 1; // ustawienie ze zagrano ruch
                    value = minMaxAlgorithm(algoDepth + 1, false, alpha, beta); //wywolanie rekurencyjne dla przeciwnika
                    board[i][j] = 0; // odznaczenie ze zagrano
                    bestValue = std::max(value, bestValue); // porownanie i przypisanie wiekszej z wartosci
                    alpha = std::max(alpha, value); // obliczenie nowego alfa
                    if (beta <= alpha) break; // unikniecie dodatkowych obliczen
                }
            }
        }
        return bestValue;
    } else { // Dla min
        int bestValue = INFINITY;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) { //dla niezagranych ruchow
                    board[i][j] = -1;  // ustawienie ze zagrano ruch
                    value = minMaxAlgorithm(algoDepth + 1, true, alpha, beta); //wywolanie rekurencyjne dla przeciwnika
                    board[i][j] = 0; // odznaczenie ze zagrano
                    bestValue = std::min(value, bestValue); // porownanie i przypisanie mniejszej z wartosci
                    beta = std::min(beta, value); // obliczneie nowego beta
                    if (beta <= alpha) break; // unikniecie dodatkowych obliczen
                }
            }
        }
        return bestValue;
    }
}

// przeprowadzenie rozgrywki
void Game::play() {
    bool moveForAI = true; //zaczyna AI

    int tmp;
    cout << "OX *** Gra kolko i krzyzyk *** OX" << endl;
    //    wczytanie parametrow rozgrywki
    cout <<" Podaj rozmiar planszy" << endl;
    cin >> tmp;
    setSize(tmp);
    makeBoard();
    cout << " Podaj liczbe znakow w rzedzie potrzebnych do wygranej" << endl;
    do {
        cin >> tmp;
    } while (tmp < 0 || tmp > size);
    setVictoryCondition(tmp);
    cout << " Podaj glebokosc poszukiwan algorytmu" << endl;
    cin >> tmp;
    setDepth(tmp);

    fillWithZeros();

//    przyznawanie ruchow graczom oraz wyswietlanie planszy
    while (isWin() == 0 && moves < size * size) {
        if (moveForAI) {
            aiMove();
            moveForAI = false;
            cout << "Ruch AI"<< endl;
        }
        else {
            playerMove();
            moveForAI = true;
        }
        print();
    }
//  komunikat o wyniku
    switch (isWin()) {
        case 1:
            cout << endl << "\t*** Przegrana! ***"<< endl;
            break;
        case -1:
            cout << endl << "\t*** Wygrana! ***"<< endl;
            break;
        case 0:
            cout << endl << "\t*** Remis! ***"<< endl;
            break;
    }
}

// sprawdzenie czy ktorys z graczy wygral, zwraca 0 jesli remis, 1 jesli wygralo AI, -1 jesli wygral gracz
int Game::isWin() {
    int counter; //zliczone wystapienia w aktualnym sprawdzeniu
    int players[2] = { 1,-1 }; // 1 - AI, -1 - gracz

    for (int p = 0; p < 2; p++) { /* petla dla sprawdzenia gracza i AI*/
        for (int i = 0; i < size; i++) { /* wiersze lub kolumny*/
            for (int j = 0; j < size; j++) { /* wiersze lub kolumny*/
                // w poziomie
                counter = 0;
                for (int k = 0; k < victoryCondition; k++) { /* zliczanie wystapien X lub O*/
                    if (j + k == size)
                        break;
                    if (board[i][j + k] == players[p])
                        counter++;
                }
                if (counter == victoryCondition)
                    return players[p];

                // w pionie
                counter = 0;
                for (int k = 0; k < victoryCondition; k++) { /* zliczanie wystapien X lub O*/
                    if (j + k == size)
                        break;
                    if (board[j + k][i] == players[p])
                        counter++;
                }
                if (counter == victoryCondition)
                    return players[p];

                // skos od prawej do lewej
                counter = 0;
                for (int k = 0; k < victoryCondition; k++) { /* zliczanie wystapien X lub O*/
                    if (i + k == size)
                        break;
                    if (j + k == size)
                        break;
                    if (board[i + k][size -1 - j - k] == players[p])
                        counter++;
                }
                if (counter == victoryCondition)
                    return players[p];

                // skos od lewej do prawej
                counter = 0;
                for (int k = 0; k < victoryCondition; k++) { /* zliczanie wystapien X lub O*/
                    if (i + k == size)
                        break;
                    if (j + k == size)
                        break;
                    if (board[i + k][j + k] == players[p])
                        counter++;
                }
                if (counter == victoryCondition)
                    return players[p];
            }
        }
    }
    return 0;
}

int main() {
    Game G;
    G.play();
}
