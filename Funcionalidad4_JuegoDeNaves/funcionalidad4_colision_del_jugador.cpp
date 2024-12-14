#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <chrono>

using namespace std;

const int anchoPantalla = 30;
const int altoPantalla = 25;
int posicionX = anchoPantalla / 2;
int posicionY = altoPantalla - 2;
int vidasJugador = 3;
vector<pair<int, int>> enemigos;
vector<int> enemigosX, enemigosY;
vector<int> proyectilesX, proyectilesY;

void dibujarPantalla() {
    system("cls");
    cout << "Vidas Restantes: " << vidasJugador << endl;
    for (int i = 0; i < altoPantalla; i++) {
        for (int j = 0; j < anchoPantalla; j++) {
            if (i == posicionY && j == posicionX) {
                cout << "=$="; // Nave
            } else {
                bool esEnemigo = false;
                for (int e = 0; e < enemigosX.size(); e++) {
                    if (i == enemigosY[e] && j == enemigosX[e]) {
                        cout << "V";
                        esEnemigo = true;
                    }
                }
                if (!esEnemigo) cout << " ";
            } 
            if (i == posicionY-1 && j == posicionX) {
                cout << "^"; // Nave
            } else {
                bool proyectil = false;
                for (int p = 0; p < proyectilesX.size(); p++) {
                    if (i == proyectilesY[p] && j == proyectilesX[p]) {
                        cout << "."; // Proyectil
                        proyectil = true;
                    }
                }
                if (!proyectil) cout << " ";
            }
        }
        cout << endl;
    }
}

//funcion para los botones y acciones del juego
void botones(char tecla) {
    switch(tecla) {

        //mueve la nave hacia la izquierda
        case 'a':
            if (posicionX > 0) posicionX--;
            break;

        //mueve la nave hacia la derecha
        case 'd':
            if (posicionX < anchoPantalla - 1) posicionX++;
            break;

        //la nave dispara un proyectil
        case 's':
            proyectilesX.push_back(posicionX);
            proyectilesY.push_back(posicionY - 1);
    }
}

//funcion para generar aleatoriamente enemigos desde arriba
void generarEnemigo() {
    enemigosX.push_back(rand() % anchoPantalla);
    enemigosY.push_back(0);
}

//funcion para mover los enemigos hacia abajo
void moverEnemigos() {
    for (int e = 0; e < enemigosX.size(); e++) {
        enemigosY[e]++;
        if (enemigosY[e] >= altoPantalla) {
            enemigosX.erase(enemigosX.begin() + e);
            enemigosY.erase(enemigosY.begin() + e);
        }
    }
}

//verifica la colision de la nave con los enemigos
void verificarColisionJugador() {
    for (int e = 0; e < enemigosX.size(); e++) {
        if (posicionX == enemigosX[e] && posicionY == enemigosY[e]) {
            vidasJugador--;
        
            enemigosX.erase(enemigosX.begin() + e);
            enemigosY.erase(enemigosY.begin() + e);
            
            if (vidasJugador <= 0) {
                cout << "Game Over!" << endl;
                system("pause");
                exit(0);
            }
            return;
        }
    }
}

//verifica la colision de los proyectiles con los enemigos
void verificarColisiones() {
    bool colision = false;
    
    for (int p = 0; p < proyectilesX.size(); p++) {
        for (int e = 0; e < enemigosX.size(); e++) {
            if (proyectilesX[p] == enemigosX[e] && proyectilesY[p] == enemigosY[e]) {
                // Eliminar el proyectil y el enemigo
                proyectilesX.erase(proyectilesX.begin() + p);
                proyectilesY.erase(proyectilesY.begin() + p);
                enemigosX.erase(enemigosX.begin() + e);
                enemigosY.erase(enemigosY.begin() + e);
                
                // Marcar que hubo una colisión
                colision = true;
                
                // Salir del bucle interno para evitar comprobaciones innecesarias
                break;
            }
        }
        
        // Si ya se detectó una colisión, podemos salir del bucle externo
        if (colision) break;
    }
}


//funcion para mover los proyectiles hacia arriba
void moverProyectiles() {
    for (int p = 0; p < proyectilesX.size(); p++) {
        proyectilesY[p]--;
        if (proyectilesY[p] < 0) {
            proyectilesX.erase(proyectilesX.begin() + p);
            proyectilesY.erase(proyectilesY.begin() + p);
        }
    }
}

//inicio del juego
int main() {
    srand(time(0));
    char tecla;
    int contador = 0;
    do {
        if (_kbhit()) {
            tecla = _getch();
            if (tecla == 'x') break;
            botones(tecla);
        }
        if (contador % 10 == 0) generarEnemigo();
        verificarColisiones();
        moverProyectiles();
        moverEnemigos();
        verificarColisionJugador();
        dibujarPantalla();
        contador++;
    } while(true);
    return 0;
}