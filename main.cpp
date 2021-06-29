#include<iostream>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace std;
//Constantes:
const int N_Cartas = 52;
const int N_Pilas = 13;

//Colores:
char C[2] = { 'N','R' };
//Palos:
char P[4] = { 'E','D','T','C' };
//N�meros de asteriscos que se va imprimir para cada pila
int asteriscos[13] = { 0,1,2,3,4,5,6,0,0,0,0,0,23 };

//Struct Carta: registro e informaci�n de cada carta del mazo
struct Carta {
    char Palo; //Palo
    int color; //Color
    int Ncarta; //Numero de carta
}CARTAS[N_Cartas];

//Struct Pila
struct Pila {
    Carta carta;
    Pila* siguiente;
}*PILA[N_Pilas]; //Pilas del 1-7 son de juego, Pilas del 8-11 son de salida, la Pila 12 es de reserva y la Pila 13 es de descarte.

//Inicializaci�n de las variables para SFML
sf::RectangleShape card(sf::Vector2f(80, 100));
sf::Texture reverseCard;
sf::Texture ECard;
sf::Texture DCard;
sf::Texture TCard;
sf::Texture CCard;
sf::Texture TempCard;
sf::Texture emptyTexture;
sf::Font letra;
sf::Text numeroCarta;
sf::RenderWindow window;

//Prototipo de todas las funciones que se utilizar�n en el programa
void inicializarCartas();
void barajear();
void agregarCarta(Pila*&, Carta);
void quitarCarta(Pila*&, Carta&);
void inicializarPilas();
void repartirCartas();
void moverCarta(int a, int b);
void moverVariasCartas(int a, int b, int o);
void mostrarPila(int a);
void mostrarPilas();
int  longitud(int a);
void moverLareserva();
int gano();
void juego();
int jugadaValida(int a, int b, int c, int d);
void barajear();
Pila* intercambio(Pila*);

int main() {
    //Funci�n principal
    juego();
    return 0;
}

void inicializarCartas() {
    //Funci�n para inicializar el array de CARTAS
    int t = 0, m = 1, c = 1;
    for (int i = 0; i < N_Cartas; i++) {
        //Establecemos el n�mero y el palo
        CARTAS[i].Ncarta = m;
        CARTAS[i].Palo = P[t];
        //Establecemos su color de acuerdo al i-esimo elemento del vector C[2]
        if (c % 2 == 0) {
            CARTAS[i].color = 1;//Color rojo
        }
        else {
            CARTAS[i].color = 0;//Color negro
        }
        //Reestablecemos condiciones para un nuevo bloque de cartas (13)
        if (m % 13 == 0) {
            t++;//Cambiamos el palo
            m = 0;//Se reinicia el numero de cartas
            c++;//Cambiamos el valor de c para cambiar el color del siguiente bloque de cartas
        }
        m++;//Se hace un incremento, ya que las cartas incializan en 1
        //cout<< CARTAS[i].Ncarta <<" "<<CARTAS[i].Palo<<endl; //Prueba de mostrar las cartas inicializadas
    }
}

void barajear() {
    //Funci�n para barajear los elementos del array CARTAS
    srand(time(NULL));//Inicializamos el random
    for (int i = 0; i < 52; i++) {
        //Variable que guarda una carta de forma temporal
        Carta t = CARTAS[i];
        //Variable con un n�mero aleatorio 
        int x = rand() % 52;
        //Intercambiamos las cartas
        CARTAS[i] = CARTAS[x];
        CARTAS[x] = t;
    }
}

void agregarCarta(Pila*& pila, Carta n) {
    Pila* nueva_pila = new Pila();
    //Se crea una nueva pila, es decir, se reserva memoria
    nueva_pila->carta = n;
    //Se guarda un elemento de la pila dentro de 'carta'
    nueva_pila->siguiente = pila;
    //Para poder agregar m�s elementos a la pila 
    pila = nueva_pila;
    //A�ade o agrega una carta a la pila
}

void quitarCarta(Pila*& pila, Carta& n) {
    Pila* aux = pila;
    //Se crea una variable auxiliar de tipo nodo
    n = aux->carta;
    //Para 'saber' el dato al que queremos eliminar
    pila = aux->siguiente;
    //Para que pila deje de apuntar al elemento que se queria eliminar
    delete aux;
    //Elimina o extrae una carta de la pila
}

void inicializarPilas() {
    //Funci�n para inicializar el array de pilas(PILA)
    for (int i = 0; i < N_Pilas; i++) {
        //Las 13 pilas se inicializan vac�as: "Sin elementos en su interior"
        PILA[i] = NULL;
    }
}

void repartirCartas() {
    //Funci�n para repartir las cartas
    int m = 0;
    //Inicializamos las pilas
    inicializarPilas();
    //Llenamos las pilas 1-7
    for (int i = 0; i < 7; i++) {
        for (int j = i; j < 7; j++) {
            //Agregamos una carta en la pila correspondiente
            agregarCarta(PILA[j], CARTAS[m]);
            m++;
        }
    }
    //Llenamos la pila 12
    for (int i = 28; i < N_Cartas; i++) {
        agregarCarta(PILA[12], CARTAS[i]);
    }
}

void moverCarta(int a, int b) {
    //Funci�n para mover una sola carta
    if (longitud(a) == asteriscos[a] + 1 && asteriscos[a] > 0) {
        asteriscos[a]--;//Si la pila tiene m�s de un elemento, decrementa la cantidad de cartas boca abajo
    }
    //Quitamos una carta de la primera pila y la agregamos a la segunda pila
    agregarCarta(PILA[b], PILA[a]->carta);
    quitarCarta(PILA[a], PILA[a]->carta);
}

void moverVariasCartas(int a, int b, int o) {
    //Funci�n para mover varias cartas
    Carta aux[60];
    if (longitud(a) == asteriscos[a] + o && asteriscos[a] > 0) {
        asteriscos[a]--;//Si la pila tiene m�s de un elemento, decrementa la cantidad de cartas boca abajo
    }
    for (int i = 0; i < o; i++) {
        //Guardamos las cartas en un array auxiliar y las quitamos de la primera pila
        aux[i] = PILA[a]->carta;
        quitarCarta(PILA[a], PILA[a]->carta);
    }
    for (int i = 0; i < o; i++) {
        //Quitamos las cartas del auxiliar y las guardamos en la nueva pila
        agregarCarta(PILA[b], aux[o - i - 1]);
    }
}

//Funci�n que calcula el tama�o de una pila
int longitud(int a) {
    Carta aux[N_Cartas];
    int cont = 0;
    while (PILA[a] != NULL) {
        //Mientras la pila sea distinta de NULL, guarda los elementos en un array auxiliar y suma al contador
        aux[cont] = PILA[a]->carta;
        quitarCarta(PILA[a], PILA[a]->carta);
        cont++;
    }
    for (int i = 0; i < cont; i++) {
        //Devuelve los elementos al array PILA[a]
        agregarCarta(PILA[a], aux[cont - i - 1]);
    }
    //Retorna la longitud de la pila
    return cont;
}

void moverLareserva() {
    //Funci�n para mover cartas entre la reserva y descarte
    moverVariasCartas(12, 11, longitud(12));
    moverCarta(11, 12);
    moverVariasCartas(11, 12, longitud(11));
}

void mostrarPila(int a, char t) {
    //Funci�n para mostrar una pila
    Pila* pointer = PILA[a];
    pointer = intercambio(pointer);
    int count = 0;
    if (longitud(a) == 0) {
        if (t == 's') {
            emptyTexture.create(80, 100);
            card.setOutlineThickness(1.0f);
            card.setOutlineColor(sf::Color::White);
            card.setPosition(sf::Vector2f(60 + 100 * (a - 4), 20));
            card.setTexture(&emptyTexture);
            window.draw(card);
        }
        if (t == 'j') {
            emptyTexture.create(80, 100);
            card.setOutlineThickness(1.0f);
            card.setOutlineColor(sf::Color::White);
            card.setPosition(sf::Vector2f(60 + 100 * a, 150));
            card.setTexture(&emptyTexture);
            window.draw(card);
        }
    }
    //Si la longitud de la pila no es 0 entonces se imprime
    if (longitud(a) != 0) {
        if (t == 'j') {
            //Imprime las cartas que no est�n en el tope
            for (int i = 0; i < asteriscos[a]; i++) {
                cout << "[**]";
                card.setPosition(sf::Vector2f(60 + 100 * a, 150 + 80 * i));
                card.setOutlineThickness(1.0f);
                card.setOutlineColor(sf::Color::Black);
                card.setTexture(&reverseCard);
                window.draw(card);
                pointer = pointer->siguiente;
            }
            //Imprime la carta del tope y las cartas que se han movido
            while (count != (longitud(a) - asteriscos[a])) {
                count++;
                char x = pointer->carta.Palo;
                int n = pointer->carta.Ncarta;
                ostringstream auxstr;
                auxstr << x << ".png";
                string texture = auxstr.str();
                TempCard.loadFromFile(texture);
                card.setOutlineThickness(1.0f);
                card.setOutlineColor(sf::Color::Red);
                card.setTexture(&TempCard);
                card.setPosition(sf::Vector2f(60 + 100 * a, 150 + 80 * (longitud(a) - 1)));
                numeroCarta.setString(to_string(n));
                numeroCarta.setPosition(sf::Vector2f(5 + 60 + 100 * a, 150 + 80 * (longitud(a) - 1)));
                char cartColor = C[pointer->carta.color];
                if (cartColor == 'N') {
                    numeroCarta.setFillColor(sf::Color::Black);
                }
                else {
                    numeroCarta.setFillColor(sf::Color::Red);
                }
                window.draw(card);
                window.draw(numeroCarta);
                cout << pointer->carta.Ncarta << pointer->carta.Palo << "(" << C[pointer->carta.color] << ")";
                pointer = pointer->siguiente;
            }
            cout << endl;
        }
        if (t == 's') {
            char x = pointer->carta.Palo;
            int n = pointer->carta.Ncarta;
            ostringstream auxstr;
            auxstr << x << ".png";
            string texture = auxstr.str();
            TempCard.loadFromFile(texture);
            card.setOutlineThickness(1.0f);
            card.setOutlineColor(sf::Color::Black);
            card.setTexture(&TempCard);
            card.setPosition(sf::Vector2f(60 + 100 * (a-4), 20));
            numeroCarta.setString(to_string(n));
            numeroCarta.setPosition(sf::Vector2f(5 + 60 + 100 * (a - 4), 20));
            window.draw(card);
            window.draw(numeroCarta);

        }
        if (t == 'r') {

        }
    }
    else {
        cout << endl;
    }
}

void mostrarPilas() {
    //Funci�n para mostrar todas las pilas
    //Muestra las pilas del 1 al 7
    cout << "Pilas de juego: " << endl;
    for (int i = 0; i < 7; i++) {
        cout << "Pila " << i + 1 << ": ";
        mostrarPila(i,'j');
    }
    //Muestra las pilas del 8 al 11
    cout << "\nPilas de salida: " << endl;
    for (int i = 7; i < 11; i++) {
        cout << "Pila " << i + 1 << ": ";
        mostrarPila(i,'s');
    }
    //Muestra la pila 12 y 13 (reserva y descarte)
    cout << "\nPila de reserva: " << endl;
    cout << "Pila " << 12 << ": " << "[**]" << endl;
    mostrarPila(11,'r');
    cout << "Pila de descarte: " << endl;
    cout << "Pila " << 13 << ": ";
    cout << PILA[12]->carta.Ncarta << PILA[12]->carta.Palo << "(" << C[PILA[12]->carta.color] << ")";
}


int gano() { // 1: ganar - 0: perder
    Carta aux[13];
    int a[4] = { 1,1,1,1 }; //estado de cada pila de salida
    //1:que esa pila del mismo palo esa ordenado de manera creciente 
    if (longitud(7) == 13 && longitud(8) == 13 && longitud(9) == 13 && longitud(10) == 13) {
        for (int i = 7; i < 11; i++) { //recorro cada pila de salida
            for (int j = 0; j < 13; j++) { //recorro cada elemento de la pila
                aux[j] = PILA[i]->carta; //guardo una carta del top pila
                quitarCarta(PILA[i], PILA[i]->carta);//quito el top pila para acceder a una posicion anterior
                if (j > 0) { //para poder comparar las cartas. Si j=0 entonces j-1=-1   -><-
                  //verifico si todas las cartas que estan en la pila
                  //si son de diferentes color o diferente palo al elemento anterior 
                    if (aux[j].color != aux[j - 1].color || aux[j].Palo != aux[j - 1].Palo) {
                        a[i - 7] = 0; //i-7=0,1,2,3 de a[4]
                    }
                }
                //esta en 12-j pero es 13-j
                if (aux[j].Ncarta != 13 - j) { // indica que son consecutivas 13,12,11,10,9,8,7,6,5,4,3,2,1
                    a[i - 7] = 0;
                }
            }
            for (int j = 0; j < 13; j++) { //regresa la pila a la normalidad la pila que hemos estado quitando
                agregarCarta(PILA[i], aux[12 - j]);
            }
        }
        // si todos arreglo a[] es 1 entonces hemos ganado
        if (a[0] == 1 && a[1] == 1 && a[2] == 1 && a[3] == 1) {
            return 1;
        }
        else {
            return 0; //caso contrario hemos perdido
        }
    }
    else {
        return 0; // si ninguna pila es de tama�o 13 tampoco hemos ganado
    }
}

//Funci�n que retorna una pila nueva, donde su base es es el top y su top es la base de la pila anterior
Pila* intercambio(Pila* pointer) {
    Pila* pilareturn = NULL;
    while (pointer != NULL) {
        agregarCarta(pilareturn, pointer->carta);
        pointer = pointer->siguiente;
    }
    return pilareturn;
}


/*
  a: 1,2,3. Donde cada numero es la opci�n a realizar.
  b: de donde se va extraer.
  c: a donde se va llevar.
  d: cuantas cartas vamos a mover.
*/


int jugadaValida(int a, int b, int c, int d) {
    int valor = 1; // 1: jugada valida y  0: jugada invalida
    Carta aux[60];

    if (d > longitud(b)) { //las cartas que vamos a mover deben ser mayor al tama�o de la pila de donde vamos a extraer
        valor = 0;
    }
    else {
        if (c < 7) { //Mover cartas hascias las pilas de juego
            if (d == 1) { //si solo se mueve una carta
                if (longitud(c) == 0) { // si esta vacia la pila de llegada solo debe aceptar 13
                    if (PILA[b]->carta.Ncarta != 13) {
                        valor = 0;
                    }
                }
                else { // si en la pila de llegada hay mas de un elemento
                 //si no es consecutiva
                    if (PILA[b]->carta.Ncarta + 1 != PILA[c]->carta.Ncarta) {
                        valor = 0;
                    }
                    //si no es del mismo color
                    if (PILA[b]->carta.color != 1 - PILA[c]->carta.color) {
                        valor = 0;
                    }
                }
            }
            else { // si se mueve m�s de una carta
             //guardo las cartas de mi pila en un array
                for (int i = 1; i < d; i++) {
                    aux[i] = PILA[b]->carta;
                    quitarCarta(PILA[b], PILA[b]->carta);
                }
                //vuelvo a llenar mi pila como estaba
                for (int i = 1; i < d; i++) {
                    agregarCarta(PILA[b], aux[d - i - 1]);
                }
                //si la carta no es consecutiva
                if (aux[d - 1].Ncarta + 1 != PILA[c]->carta.Ncarta) {
                    valor = 0;
                }
                //si la carta no es del mismo color
                if (aux[d - 1].color != 1 - PILA[c]->carta.color) {
                    valor = 0;
                }
            }
        }
        else if (c >= 7 && c < 11) { //Mover cartas hacia las pilas de de salida
            if (longitud(c) == 0) {
                //si la longitud es 0 solo debe aceptar la carta 1
                if (PILA[b]->carta.Ncarta != 1) {
                    valor = 0;
                }
            }
            else {  //si la longitud mayor a 0
             //deben ser consecutibas 
                if (PILA[b]->carta.Ncarta - 1 != PILA[c]->carta.Ncarta) {
                    valor = 0;
                }
                //mismo color
                if (PILA[b]->carta.color != PILA[c]->carta.color) {
                    valor = 0;
                }
            }
        }
        else { //no se puede mover cartas hacia la pila de reserva o descarte
         //por es la jugada es invalida
            valor = 0;
        }
    }
    return valor;
}

void juego() {
    //Mostrar el juego
    int a, b, c, d;
    //Inicializamos, barajeamos y repartimos las cartas
    inicializarCartas();
    barajear();
    repartirCartas();
    letra.loadFromFile("PumpkinPancakes.ttf");
    numeroCarta.setFont(letra);
    numeroCarta.setCharacterSize(20);
    reverseCard.loadFromFile("reverse.png");
    ECard.loadFromFile("E.png");
    DCard.loadFromFile("D.png");
    TCard.loadFromFile("T.png");
    CCard.loadFromFile("C.png");
    window.setFramerateLimit(60);
    window.create(sf::VideoMode(800, 800), "Solitaire Game", sf::Style::Close);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.key.code == '1') {
                cout << "XDDDDDDDDDDDD" << endl;
            }
        }
        window.clear(sf::Color(15, 185, 74, 255));
        mostrarPilas();
        window.display();
        cout << "\n\nElija una de las opciones: " << endl;
        cout << "\n1.-Mover una carta.";
        cout << "\n2.-Mover varias cartas.";
        cout << "\n3.-Mover la reserva." << endl;
        cin >> a;
        if (a == 1) {
            d = 1;
            cout << "Ingrese la pila de donde sacara la carta: "; cin >> b;
            cout << "Ingrese la pila de llegada de la carta: "; cin >> c;
            if (jugadaValida(a, b - 1, c - 1, d) == 1) {
                moverCarta(b - 1, c - 1);
            }
        }
        else if (a == 2) {
            cout << "Ingrese la pila de donde sacara la carta: "; cin >> b;
            cout << "Ingrese la pila de llegada de la carta: "; cin >> c;
            cout << "Ingrese el numero de cartas que desea mover: "; cin >> d;
            if (jugadaValida(a, b - 1, c - 1, d) == 1) {
                moverVariasCartas(b - 1, c - 1, d);
            }
        }
        else if (a == 3) {
            moverLareserva();
        }
        if (gano() != 0) {
            break;
        }
        cin.get();
        system("cls");
    }
    cout << "Felicidades ganaste" << endl;
}
