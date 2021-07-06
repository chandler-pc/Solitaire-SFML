//Para una óptima visualización se recomiendo una pantalla de al menos 900 pixeles de altura
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
//Números de asteriscos que se va imprimir para cada pila
int asteriscos[13] = { 0,1,2,3,4,5,6,0,0,0,0,0,23 };

//Struct Carta: registro e información de cada carta del mazo
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

//Inicialización de las variables para SFML
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
sf::Text help;
sf::RenderWindow helpWindow;
sf::RenderWindow oneCardWindow;
sf::RenderWindow moreCardWindow;
sf::RenderWindow window;
sf::RectangleShape MakeBtn(float x, float y, sf::Color color, float posX, float posY);
sf::RectangleShape MakeTxtBox(float x, float y, float posX, float posY); 
void JugadaNoValidaVentana();
void OneCardWindow();
void MoreCardWindow(); 
void HelpWindow();
//Variables para el funcionamiento correcto de las ventas
string s1 = "", s2 = "", s3 = "";
bool oneWinTbSel = true;
int moreWinTbSel = 0;

//Prototipo de todas las funciones que se utilizarán en el programa
void inicializarCartas();
void barajear();
void agregarCarta(Pila*&, Carta);
void quitarCarta(Pila*&, Carta&);
void inicializarPilas();
void repartirCartas();
void moverCarta(int a, int b);
void moverVariasCartas(int a, int b, int o);
void mostrarPila(int a, char t);
void mostrarPilas();
int  longitud(int a);
void moverLareserva();
int gano();
void juego();
int jugadaValida(int b, int c, int d);
void barajear();
Pila* intercambio(Pila*);

int main() {
    //Función principal
    juego();
    return 0;
}

void inicializarCartas() {
    //Función para inicializar el array de CARTAS
    int t = 0, m = 1, c = 1;
    for (int i = 0; i < N_Cartas; i++) {
        //Establecemos el número y el palo
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
    //Función para barajear los elementos del array CARTAS
    srand(time(NULL));//Inicializamos el random
    for (int i = 0; i < 52; i++) {
        //Variable que guarda una carta de forma temporal
        Carta t = CARTAS[i];
        //Variable con un número aleatorio 
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
    //Para poder agregar más elementos a la pila 
    pila = nueva_pila;
    //Añade o agrega una carta a la pila
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
    //Función para inicializar el array de pilas(PILA)
    for (int i = 0; i < N_Pilas; i++) {
        //Las 13 pilas se inicializan vacías: "Sin elementos en su interior"
        PILA[i] = NULL;
    }
}

void repartirCartas() {
    //Función para repartir las cartas
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
    //Función para mover una sola carta
    if (longitud(a) == asteriscos[a] + 1 && asteriscos[a] > 0) {
        asteriscos[a]--;//Si la pila tiene más de un elemento, decrementa la cantidad de cartas boca abajo
    }
    //Quitamos una carta de la primera pila y la agregamos a la segunda pila
    agregarCarta(PILA[b], PILA[a]->carta);
    quitarCarta(PILA[a], PILA[a]->carta);
}

void moverVariasCartas(int a, int b, int o) {
    //Función para mover varias cartas
    Carta aux[60];
    if (longitud(a) == asteriscos[a] + o && asteriscos[a] > 0) {
        asteriscos[a]--;//Si la pila tiene más de un elemento, decrementa la cantidad de cartas boca abajo
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

//Función que calcula el tamaño de una pila
int longitud(int a) {
    Carta aux[N_Cartas];
    int cont = 0;
    if (a > 12) {
        return 0;
    }
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
    //Función para mover cartas entre la reserva y descarte
    moverVariasCartas(12, 11, longitud(12));
    moverCarta(11, 12);
    moverVariasCartas(11, 12, longitud(11));
}

void mostrarPila(int a, char t) {
    //Función para mostrar una pila
    Pila* pointer = PILA[a];
    pointer = intercambio(pointer);
    int count = 0;
    if (longitud(a) == 0) {
        if (t == 's') {
            //
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
        if (t == 'r' && longitud(12)!=0) {
            card.setPosition(sf::Vector2f(60, 20));
            card.setOutlineThickness(1.0f);
            card.setOutlineColor(sf::Color::Black);
            card.setTexture(&reverseCard);
            window.draw(card);
        }
    }
    //Si la longitud de la pila no es 0 entonces se imprime
    if (longitud(a) != 0) {
        if (t == 'j') {
            //Imprime las cartas que no están en el tope
            for (int i = 0; i < asteriscos[a]; i++) {
                //cout << "[**]";
                card.setPosition(sf::Vector2f(60 + 100 * a, 150 + 80 * i));
                card.setOutlineThickness(1.0f);
                card.setOutlineColor(sf::Color::Black);
                card.setTexture(&reverseCard);
                window.draw(card);
                pointer = pointer->siguiente;
            }
            //Imprime la carta del tope y las cartas que se han movido
            int tempLong = longitud(a) - asteriscos[a];
            while (count != tempLong) {
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
                card.setPosition(sf::Vector2f(60 + 100 * a, 150 + 80 * (longitud(a) - tempLong + count -1)));
                numeroCarta.setString(to_string(n));
                numeroCarta.setPosition(sf::Vector2f(5 + 60 + 100 * a, 150 + 80 * (longitud(a) - tempLong + count - 1)));
                char cartColor = C[pointer->carta.color];
                if (cartColor == 'N') {
                    numeroCarta.setFillColor(sf::Color::Black);
                }
                else {
                    numeroCarta.setFillColor(sf::Color::Red);
                }
                window.draw(card);
                window.draw(numeroCarta);
                //cout << pointer->carta.Ncarta << pointer->carta.Palo << "(" << C[pointer->carta.color] << ")";
                pointer = pointer->siguiente;
            }
            //cout << endl;
        }
        if (t == 's') {
            int tempLong = longitud(a) - asteriscos[a];
            while (count != tempLong) {
                count++;
                window.draw(card);
                window.draw(numeroCarta);
                char x = pointer->carta.Palo;
                int n = pointer->carta.Ncarta;
                char cartColor = C[pointer->carta.color];
                ostringstream auxstr;
                auxstr << x << ".png";
                string texture = auxstr.str();
                TempCard.loadFromFile(texture);
                card.setOutlineThickness(1.0f);
                card.setOutlineColor(sf::Color::Black);
                card.setTexture(&TempCard);
                card.setPosition(sf::Vector2f(60 + 100 * (a - 4), 20));
                numeroCarta.setString(to_string(n));
                numeroCarta.setPosition(sf::Vector2f(5 + 60 + 100 * (a - 4), 20));
                if (cartColor == 'N') {
                    numeroCarta.setFillColor(sf::Color::Black);
                }
                else {
                    numeroCarta.setFillColor(sf::Color::Red);
                }
                window.draw(card);
                window.draw(numeroCarta);
                //cout << pointer->carta.Ncarta << pointer->carta.Palo << "(" << C[pointer->carta.color] << ")";
                pointer = pointer->siguiente;
            }
            //cout << endl;
        }
        if (t == 'd') {
            while (count != longitud(a)-1) {
                count++;
                pointer = pointer->siguiente;
            }
            char x = pointer->carta.Palo;
            int n = pointer->carta.Ncarta;
            ostringstream auxstr;
            auxstr << x << ".png";
            string texture = auxstr.str();
            TempCard.loadFromFile(texture);
            card.setOutlineThickness(1.0f);
            card.setOutlineColor(sf::Color::Red);
            card.setTexture(&TempCard);
            card.setPosition(sf::Vector2f(160, 20));
            numeroCarta.setString(to_string(n));
            numeroCarta.setPosition(sf::Vector2f(165,20));
            char cartColor = C[pointer->carta.color];
            if (cartColor == 'N') {
                numeroCarta.setFillColor(sf::Color::Black);
            }
            else {
                numeroCarta.setFillColor(sf::Color::Red);
            }
            window.draw(card);
            window.draw(numeroCarta);
        }
    }
    else {
        //cout << endl;
    }
}

void mostrarPilas() {
    //Función para mostrar todas las pilas
    //Muestra las pilas del 1 al 7
    //cout << "Pilas de juego: " << endl;
    for (int i = 0; i < 7; i++) {
        //cout << "Pila " << i + 1 << ": ";
        mostrarPila(i,'j');
    }
    //Muestra las pilas del 8 al 11
    //cout << "\nPilas de salida: " << endl;
    for (int i = 7; i < 11; i++) {
        //cout << "Pila " << i + 1 << ": ";
        mostrarPila(i,'s');
    }
    //Muestra la pila 12 y 13 (reserva y descarte)
    /*cout << "\nPila de reserva: " << endl;
    cout << "Pila " << 12 << ": " << "[**]" << endl;*/
    mostrarPila(11,'r');
    /*cout << "Pila de descarte: " << endl;
    cout << "Pila " << 13 << ": ";
    cout << PILA[12]->carta.Ncarta << PILA[12]->carta.Palo << "(" << C[PILA[12]->carta.color] << ")";*/
    mostrarPila(12, 'd');
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
        return 0; // si ninguna pila es de tamaño 13 tampoco hemos ganado
    }
}

//Función que retorna una pila nueva, donde su base es es el top y su top es la base de la pila anterior
Pila* intercambio(Pila* pointer) {
    Pila* pilareturn = NULL;
    while (pointer != NULL) {
        agregarCarta(pilareturn, pointer->carta);
        pointer = pointer->siguiente;
    }
    return pilareturn;
}


/*
  a: 1,2,3. Donde cada numero es la opción a realizar.
  b: de donde se va extraer.
  c: a donde se va llevar.
  d: cuantas cartas vamos a mover.
*/

int jugadaValida(int b, int c, int d) {
    int valor = 1; // 1: jugada valida y  0: jugada invalida
    Carta aux[60];
    //b: salida , c:llegada
    if (d > longitud(b)) { //si el numero de cartas que vamos a mover es mayor al tamaño de la pila de donde vamos a extraer
        valor = 0;//No se realiza acciones
    }
    else {
        if (c < 7) { //Mover cartas hacia las pilas de juego
            if (d == 1) { //si solo se mueve una carta
                if (longitud(c) == 0) { // si la pila de llegada esta vacia solo debe aceptar una carta de numero 13
                    if (PILA[b]->carta.Ncarta < 13) {
                        valor = 0;
                    }
                }
                else { // si en la pila de llegada hay 1 o mas elementos
                 //si no es consecutiva o es del mismo color
                    if (PILA[b]->carta.Ncarta + 1 != PILA[c]->carta.Ncarta || PILA[b]->carta.color == PILA[c]->carta.color) {
                        valor = 0;
                    }
                }
            }
            else { // si se mueve más de una carta
             //guardo las cartas de mi pila en un array
             //pila a la que se mueve: vacia
                if (longitud(c) == 0) {
                    for (int i = 0; i < d; i++) {
                        aux[i] = PILA[b]->carta;
                        quitarCarta(PILA[b], PILA[b]->carta);
                    }
                    if (aux[d - 1].Ncarta == 13) {
                        //vuelvo a llenar mi pila como estaba
                        for (int i = 0; i < d; i++) {
                            agregarCarta(PILA[b], aux[d - i - 1]);
                        }
                    }
                    else {
                        valor = 0;
                    }
                }
                else {//pila a la que se mueve: con cartas
                    //guardo las cartas de mi pila en un array
                    for (int i = 0; i < d; i++) {
                        aux[i] = PILA[b]->carta;
                        quitarCarta(PILA[b], PILA[b]->carta);
                    }
                    //vuelvo a llenar mi pila como estaba
                    for (int i = 0; i < d; i++) {
                        agregarCarta(PILA[b], aux[d - i - 1]);
                    }
                    //si la carta no es consecutiva o no es del mismo color
                    if (aux[d - 1].Ncarta + 1 != PILA[c]->carta.Ncarta || aux[d - 1].color == PILA[c]->carta.color) {
                        valor = 0;
                    }
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
             //deben ser consecutivas y mismo color
                if (PILA[b]->carta.Ncarta - 1 != PILA[c]->carta.Ncarta || PILA[b]->carta.color != PILA[c]->carta.color) {
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

sf::RectangleShape MakeBtn(float x, float y, sf::Color color, float posX, float posY) {
    sf::RectangleShape btn;
    btn.setSize(sf::Vector2f(x, y));
    btn.setFillColor(color);
    btn.setPosition(sf::Vector2f((posX - btn.getGlobalBounds().width)/2, posY));
    return btn;
}

sf::RectangleShape MakeTxtBox(float x, float y, float posX, float posY) {
    sf::RectangleShape tb;
    tb.setFillColor(sf::Color::White);
    tb.setSize(sf::Vector2f(x, y));
    tb.setPosition(sf::Vector2f(posX,posY));
    return tb;
}

void JugadaNoValidaVentana() {
    sf::RenderWindow notValid;
    sf::Text textNV;
    sf::Image icon;
    icon.loadFromFile("./error.png");
    textNV.setFont(letra);
    textNV.setCharacterSize(25);
    textNV.setFillColor(sf::Color::White);
    textNV.setString("Esa jugada no es válida, inténtelo nuevamente o cierre la ventana.");
    sf::FloatRect textRect = textNV.getLocalBounds();
    textNV.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    textNV.setPosition(sf::Vector2f(305, 50));
    notValid.create(sf::VideoMode(610, 100), "Jugada inválida", sf::Style::Close);
    notValid.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    notValid.setFramerateLimit(60);
    while (notValid.isOpen()) {
        sf::Event eventNV;
        while (notValid.pollEvent(eventNV)) {
            if (eventNV.type == sf::Event::Closed) {
                notValid.close();
            }
        }
        notValid.clear(sf::Color(118, 178, 146, 255));
        notValid.draw(textNV);
        notValid.display();
    }
}
//Render de mover una carta
void OneCardWindow() {
    oneCardWindow.create(sf::VideoMode(500, 200), "Mover una carta", sf::Style::Close);
    sf::Image icon;
    icon.loadFromFile("./iconCard.png");
    oneCardWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    oneCardWindow.setFramerateLimit(60);
    sf::Text btnText;
    btnText.setFont(letra);
    btnText.setFillColor(sf::Color::Black);
    btnText.setCharacterSize(20);
    btnText.setPosition(sf::Vector2f(223,155));
    btnText.setString("Aceptar");
    sf::Text t1, t2;
    t1.setFont(letra); t2.setFont(letra);
    t1.setCharacterSize(20); t2.setCharacterSize(20);
    t1.setPosition(sf::Vector2f(20, 20));
    t2.setPosition(sf::Vector2f(20, 80));
    t1.setString("Escribe la pila de donde sacar la carta : ");
    t2.setString("Escribe la fila a la cual mover la carta : ");
    sf::RectangleShape btn = MakeBtn(80, 40, sf::Color(191, 195, 203), oneCardWindow.getSize().x, 150);
    sf::RectangleShape tb1 = MakeTxtBox(100, 20, 350, 25);
    sf::RectangleShape tb2 = MakeTxtBox(100, 20, 350, 85);
    sf::Text n1, n2;
    long long  int in1 = 0, in2 = 0;
    n1.setFont(letra);
    n1.setFillColor(sf::Color::Black);
    n1.setCharacterSize(20);
    n1.setPosition(352, 20);
    n2.setFont(letra);
    n2.setFillColor(sf::Color::Black);
    n2.setCharacterSize(20);
    n2.setPosition(352, 80);
    while (oneCardWindow.isOpen()) {
        sf::Event oneEvent;
        while (oneCardWindow.pollEvent(oneEvent)) {
            if (oneEvent.type == sf::Event::Closed) {
                in1 = 0, in2 = 0;
                s1 = "", s2 = "";
                oneCardWindow.close();
            }
            if (oneEvent.type == sf::Event::TextEntered) { 
                if ((oneEvent.text.unicode <= 57 && oneEvent.text.unicode >= 48) || oneEvent.text.unicode == 8) {
                    
                    if (oneEvent.text.unicode == 8) {
                        if (oneWinTbSel) {
                            if (s1.size() <= 1) {
                                s1 = "";
                                in1 = 0;
                            }
                            else{
                                if (s1.size() > 2) {
                                    s1 = s1.substr(0, 1);
                                }
                                else {
                                    s1 = s1.substr(0, s1.size() - 1);
                                }
                                in1 = stoi(s1);
                            }
                            n1.setString(s1);
                        }
                        if (!oneWinTbSel) {
                            if (s2.size() <= 1) {
                                s2 = "";
                                in2 = 0;
                            }
                            else{
                                if (s2.size() > 2) {
                                    s2 = s2.substr(0, 1);
                                }
                                else {
                                    s2 = s2.substr(0, s2.size() - 1);
                                }
                                in2 = stoi(s2);
                            }
                            n2.setString(s2);
                        }
                    }
                    else {
                        if (oneWinTbSel) {
                            s1 += static_cast<char>(oneEvent.text.unicode);
                            if (s1.size() > 0 && s1.size() <= 2) {
                                in1 = stoi(s1);
                                n1.setString(s1);
                            }
                        }
                        else {
                            s2 += static_cast<char>(oneEvent.text.unicode);
                            if (s2.size() > 0 && s2.size() <= 2) {
                                in2 = stoi(s2);
                                n2.setString(s2);
                            }
                            
                        }
                    }
                }
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(oneCardWindow);
            if (mousePos.x <= btn.getSize().x + btn.getPosition().x && mousePos.x >= btn.getPosition().x && mousePos.y <= btn.getSize().y + btn.getPosition().y && mousePos.y >= btn.getPosition().y) {
                if (to_string(in1) == "0") {
                    in1 = 1;
                }
                if (to_string(in2) == "0") {
                    in2 = 1;
                }
                if (jugadaValida(in1 - 1, in2 - 1, 1) == 1) {
                    oneCardWindow.close();
                    moverCarta(in1 - 1, in2 - 1);
                    in1 = 0, in2 = 0;
                    s1 = "", s2 = "";
                }
                else {
                    JugadaNoValidaVentana();
                    in1 = 0, in2 = 0;
                    s1 = "", s2 = "";
                    n1.setString(s1);
                    n2.setString(s2);
                }

            }
            if (mousePos.x <= tb1.getSize().x + tb1.getPosition().x && mousePos.x >= tb1.getPosition().x && mousePos.y <= tb1.getSize().y + tb1.getPosition().y && mousePos.y >= tb1.getPosition().y) {
                oneWinTbSel = true;
            }
            if (mousePos.x <= tb2.getSize().x + tb2.getPosition().x && mousePos.x >= tb2.getPosition().x && mousePos.y <= tb2.getSize().y + tb2.getPosition().y && mousePos.y >= tb2.getPosition().y) {
                oneWinTbSel = false;
            }
        }
        if (oneWinTbSel) {
            tb1.setFillColor(sf::Color(186, 243, 247, 255));
            tb2.setFillColor(sf::Color::White);
        }
        else {
            tb2.setFillColor(sf::Color(186, 243, 247, 255));
            tb1.setFillColor(sf::Color::White);
        }
        oneCardWindow.clear(sf::Color(118, 178, 146, 255));
        oneCardWindow.draw(t1);
        oneCardWindow.draw(t2);
        oneCardWindow.draw(tb1);
        oneCardWindow.draw(tb2);
        oneCardWindow.draw(btn);
        oneCardWindow.draw(n1);
        oneCardWindow.draw(n2);
        oneCardWindow.draw(btnText);
        oneCardWindow.display();
    }
}
//Render de mover varias cartas
void MoreCardWindow() {
    moreCardWindow.create(sf::VideoMode(500, 250), "Mover varias cartas", sf::Style::Close);
    sf::Image icon;
    icon.loadFromFile("./iconCard.png");
    moreCardWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    moreCardWindow.setFramerateLimit(60);
    sf::Text btnText;
    btnText.setFont(letra);
    btnText.setFillColor(sf::Color::Black);
    btnText.setCharacterSize(20);
    btnText.setPosition(sf::Vector2f(223, 205));
    btnText.setString("Aceptar");
    sf::Text t1, t2, t3;
    t1.setFont(letra); t2.setFont(letra); t3.setFont(letra);
    t1.setCharacterSize(20); t2.setCharacterSize(20); t3.setCharacterSize(20);
    t1.setPosition(sf::Vector2f(20, 20));
    t2.setPosition(sf::Vector2f(20, 80));
    t3.setPosition(sf::Vector2f(20, 140));
    t1.setString("Escribe la pila de donde sacar la carta : ");
    t2.setString("Escribe la fila a la cual mover la carta : ");
    t3.setString("Escribe el numero de cartas a mover : ");
    sf::RectangleShape btn = MakeBtn(80, 40, sf::Color(191, 195, 203), moreCardWindow.getSize().x, 200);
    sf::RectangleShape tb1 = MakeTxtBox(100, 20, 350, 25);
    sf::RectangleShape tb2 = MakeTxtBox(100, 20, 350, 85);
    sf::RectangleShape tb3 = MakeTxtBox(100, 20, 350, 145);
    sf::Text n1, n2, n3;
    int in1 = 0, in2 = 0, in3 = 0;
    n1.setFont(letra);
    n1.setFillColor(sf::Color::Black);
    n1.setCharacterSize(20);
    n1.setPosition(352, 20);
    n2.setFont(letra);
    n2.setFillColor(sf::Color::Black);
    n2.setCharacterSize(20);
    n2.setPosition(352, 80);
    n3.setFont(letra);
    n3.setFillColor(sf::Color::Black);
    n3.setCharacterSize(20);
    n3.setPosition(352, 140);
    while (moreCardWindow.isOpen()) {
        sf::Event moreEvent;
        while (moreCardWindow.pollEvent(moreEvent)) {
            if (moreEvent.type == sf::Event::Closed) {
                in1 = 0; in2 = 0; in3 = 0;
                s1 = "", s2 = ""; s3 = "";
                moreCardWindow.close();
            }
            if (moreEvent.type == sf::Event::TextEntered) {
                if (moreEvent.text.unicode <= 57 && moreEvent.text.unicode >= 48 || moreEvent.text.unicode == 8) {
                    if (moreEvent.text.unicode == 8) {
                        if (moreWinTbSel == 0) {
                            if (s1.size() <= 1) {
                                s1 = "";
                                in1 = 0;
                            }
                            else {
                                if (s1.size() > 2) {
                                    s1 = s1.substr(0, 1);
                                }
                                else {
                                    s1 = s1.substr(0, s1.size() - 1);
                                }
                                in1 = stoi(s1);
                            }
                            n1.setString(s1);
                        }
                        else if (moreWinTbSel == 1) {
                            if (s2.size() <= 1) {
                                s2 = "";
                                in2 = 0;
                            }
                            else {
                                if (s2.size() > 2) {
                                    s2 = s2.substr(0, 1);
                                }
                                else {
                                    s2 = s2.substr(0, s2.size() - 1);
                                }
                                in2 = stoi(s2);
                            }
                            n2.setString(s2);
                        }
                        else {
                            if (s3.size() <= 1) {
                                s3 = "";
                                in3 = 0;
                            }
                            else {
                                if (s3.size() > 2) {
                                    s3 = s3.substr(0, 1);
                                }
                                else {
                                    s3 = s3.substr(0, s3.size() - 1);
                                }
                                in2 = stoi(s3);
                            }
                            n3.setString(s3);
                        }
                    }
                    else {
                        if (moreWinTbSel == 0) {
                            s1 += static_cast<char>(moreEvent.text.unicode);
                            if (s1.size() > 0 && s1.size() <= 2) {
                                in1 = stoi(s1);
                                n1.setString(s1);
                            }
                            
                        }
                        else if (moreWinTbSel == 1) {
                            s2 += static_cast<char>(moreEvent.text.unicode);
                            if (s2.size() > 0 && s2.size() <= 2) {
                                in2 = stoi(s2);
                                n2.setString(s2);
                            }
                            
                        }
                        else {
                            s3 += static_cast<char>(moreEvent.text.unicode);
                            if (s3.size() > 0 && s3.size() <= 2) {
                                in3 = stoi(s3);
                                n3.setString(s3);
                            }
                        }
                    }
                }
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(moreCardWindow);
            if (mousePos.x <= btn.getSize().x + btn.getPosition().x && mousePos.x >= btn.getPosition().x && mousePos.y <= btn.getSize().y + btn.getPosition().y && mousePos.y >= btn.getPosition().y) {
                if (to_string(in1) == "0") {
                    in1 = 1;
                }
                if (to_string(in2) == "0") {
                    in2 = 1;
                }
                if (to_string(in3) == "0") {
                    in3 = 1;
                }
                if (jugadaValida(in1 - 1, in2 - 1, in3) == 1) {
                    moreCardWindow.close();
                    moverVariasCartas(in1 - 1, in2 - 1,in3);
                    in1 = 0; in2 = 0; in3 = 0;
                    s1 = "", s2 = ""; s3 = "";
                }
                else {
                    JugadaNoValidaVentana();
                    in1 = 0; in2 = 0; in3 = 0;
                    s1 = "", s2 = ""; s3 = "";
                    n1.setString(s1);
                    n2.setString(s2);
                    n3.setString(s3);
                }

            }
            if (mousePos.x <= tb1.getSize().x + tb1.getPosition().x && mousePos.x >= tb1.getPosition().x && mousePos.y <= tb1.getSize().y + tb1.getPosition().y && mousePos.y >= tb1.getPosition().y) {
                moreWinTbSel = 0;
            }
            if (mousePos.x <= tb2.getSize().x + tb2.getPosition().x && mousePos.x >= tb2.getPosition().x && mousePos.y <= tb2.getSize().y + tb2.getPosition().y && mousePos.y >= tb2.getPosition().y) {
                moreWinTbSel = 1;
            }
            if (mousePos.x <= tb3.getSize().x + tb3.getPosition().x && mousePos.x >= tb3.getPosition().x && mousePos.y <= tb3.getSize().y + tb3.getPosition().y && mousePos.y >= tb3.getPosition().y) {
                moreWinTbSel = 2;
            }
        }
        if (moreWinTbSel==0) {
            tb1.setFillColor(sf::Color(186, 243, 247, 255));
            tb2.setFillColor(sf::Color::White);
            tb3.setFillColor(sf::Color::White);
        }
        else if (moreWinTbSel == 1) {
            tb2.setFillColor(sf::Color(186, 243, 247, 255));
            tb1.setFillColor(sf::Color::White);
            tb3.setFillColor(sf::Color::White);
        }
        else {
            tb3.setFillColor(sf::Color(186, 243, 247, 255));
            tb2.setFillColor(sf::Color::White);
            tb1.setFillColor(sf::Color::White);
        }
        moreCardWindow.clear(sf::Color(118, 178, 146, 255));
        moreCardWindow.draw(t1);
        moreCardWindow.draw(t2);
        moreCardWindow.draw(t3);
        moreCardWindow.draw(tb1);
        moreCardWindow.draw(tb2);
        moreCardWindow.draw(tb3);
        moreCardWindow.draw(btn);
        moreCardWindow.draw(n1);
        moreCardWindow.draw(n2);
        moreCardWindow.draw(n3);
        moreCardWindow.draw(btnText);
        moreCardWindow.display();
    }

}
void HelpWindow() {
    sf::Image icon;
    icon.loadFromFile("./help.png");
    helpWindow.create(sf::VideoMode(500, 300), "Ayuda", sf::Style::Close);
    helpWindow.setIcon(icon.getSize().x,icon.getSize().y, icon.getPixelsPtr());
    helpWindow.setFramerateLimit(60);
    sf::Text helpText1, helpText2, helpText3;
    helpText1.setFont(letra); helpText2.setFont(letra); helpText3.setFont(letra);
    helpText1.setString("- Presiona 1 para mover una carta.");
    helpText2.setString("- Presiona 2 para mover varias cartas.");
    helpText3.setString("- Presiona 3 para mover la reserva.");
    sf::FloatRect rectT1 = helpText1.getLocalBounds(), rectT2 = helpText2.getLocalBounds(), rectT3 = helpText3.getLocalBounds();
    helpText1.setOrigin(sf::Vector2f((rectT1.width + rectT1.left) / 2, (rectT1.height + rectT1.top) / 2));
    helpText2.setOrigin(sf::Vector2f((rectT2.width + rectT2.left) / 2, (rectT2.height + rectT2.top) / 2));
    helpText3.setOrigin(sf::Vector2f((rectT3.width + rectT3.left) / 2, (rectT3.height + rectT3.top) / 2));
    helpText1.setPosition(250, 50); helpText2.setPosition(250, 150); helpText3.setPosition(250, 250);
    while (helpWindow.isOpen()) {
        sf::Event helpEvent;
        while (helpWindow.pollEvent(helpEvent)) {
            if (helpEvent.type == sf::Event::Closed) {
                helpWindow.close();
            }
        }
        helpWindow.clear(sf::Color(118, 178, 146, 255));
        helpWindow.draw(helpText1);
        helpWindow.draw(helpText2);
        helpWindow.draw(helpText3);
        helpWindow.display();
    }
}
void InfoWindow() {
    sf::Image icon;
    icon.loadFromFile("./help.png");
    helpWindow.create(sf::VideoMode(600, 300), "Ayuda", sf::Style::Close);
    helpWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    helpWindow.setFramerateLimit(60);
    sf::Text infoText1, infoText2, infoText3, infoText4, infoText5, infoText6, infoText7, infoText8, infoTextTitulo;
    infoText1.setFont(letra); infoText2.setFont(letra); infoText3.setFont(letra);
    infoText4.setFont(letra); infoText5.setFont(letra); infoText6.setFont(letra);
    infoText7.setFont(letra); infoText8.setFont(letra); infoTextTitulo.setFont(letra);
    infoText1.setCharacterSize(21); infoText2.setCharacterSize(21); infoText3.setCharacterSize(21);
    infoText4.setCharacterSize(21); infoText5.setCharacterSize(21); infoText6.setCharacterSize(21);
    infoText7.setCharacterSize(21); infoText8.setCharacterSize(21); infoTextTitulo.setCharacterSize(30);
    infoTextTitulo.setFillColor(sf::Color(0,0,0));
    infoTextTitulo.setString("JUEGO EL SOLITARIO");
    infoText1.setString("Hay 7 pilas de juego, en estas solo se pueden colocar cartas que ");
    infoText2.setString("formen una secuencia decreciente consecutiva y de colores alternantes");
    infoText3.setString("1 Pila de reserva y de descarte, de la cual podemos tomar una carta");
    infoText4.setString("de la pila de Reserva, una por cada vez y moverla a las pilas de juego");
    infoText5.setString("o a las Pilas de Salida. 4 Pilas de Salida, en estas pilas se iran");
    infoText6.setString("apilando cada carta de un mismo palo. Si se logra llenar cada una de");
    infoText7.setString("las 13 cartas de un mismo palo en cada pila de salida, se gana el juego,");
    infoText8.setString(" y se pierde cuando no se puede hacer ningun movimiento.");
    infoTextTitulo.setPosition(190, 10);
    infoText1.setPosition(10, 50); infoText2.setPosition(10, 81); infoText3.setPosition(10, 112);
    infoText4.setPosition(10, 143); infoText5.setPosition(10, 174); infoText6.setPosition(10, 205);
    infoText7.setPosition(10, 236);
    while (helpWindow.isOpen()) {
        sf::Event helpEvent;
        while (helpWindow.pollEvent(helpEvent)) {
            if (helpEvent.type == sf::Event::Closed) {
                helpWindow.close();
            }
        }
        helpWindow.clear(sf::Color(118, 178, 146, 255));
        helpWindow.draw(infoTextTitulo);
        helpWindow.draw(infoText1);
        helpWindow.draw(infoText2);
        helpWindow.draw(infoText3);
        helpWindow.draw(infoText4);
        helpWindow.draw(infoText5);
        helpWindow.draw(infoText6);
        helpWindow.draw(infoText7);
        helpWindow.display();
    }
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
    help.setString("Presiona H para ayuda.");
    help.setFont(letra);
    help.setCharacterSize(30);
    help.setPosition(sf::Vector2f(20, 750));
    reverseCard.loadFromFile("reverse.png");
    ECard.loadFromFile("E.png");
    DCard.loadFromFile("D.png");
    TCard.loadFromFile("T.png");
    CCard.loadFromFile("C.png");
    window.create(sf::VideoMode(800, 800), "Solitaire Game", sf::Style::Close);
    sf::Image icon;
    icon.loadFromFile("./iconCard.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);
    bool twoWin = false;
    /*cout << "\n\nElija una de las opciones: " << endl;
    cout << "\n1.-Mover una carta.";
    cout << "\n2.-Mover varias cartas.";
    cout << "\n3.-Mover la reserva." << endl;*/
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyReleased) {
                if ((event.key.code == sf::Keyboard::Numpad1 || event.key.code == sf::Keyboard::Num1) && !twoWin) {
                    twoWin = true;
                    /*int t1 = 0, t2 = 0;
                    cout << "Escribe la fila de donde sacar la carta : ";cin >> t1;
                    cout << "Escribe la fila a la cual mover la carta : ";cin >> t2; 
                    if (jugadaValida(1, t1 - 1, t2 - 1, 1) == 1) {
                        moverCarta(t1 - 1, t2 - 1);
                    }
                    cout << endl;*/
                    OneCardWindow();
                }
                if ((event.key.code == sf::Keyboard::Numpad2 || event.key.code == sf::Keyboard::Num2) && !twoWin) {
                    twoWin = true;
                    /*int t1 = 0, t2 = 0, nc = 0;
                    cout << "Escribe la fila de donde sacar la carta : "; cin >> t1;
                    cout << "Escribe la fila a la cual mover la carta : "; cin >> t2;
                    cout << "Ingrese la cantidad de cartas que desea mover: "; cin >> nc;
                    if (jugadaValida(2, t1 - 1, t2 - 1, nc) == 1) {
                        moverVariasCartas(t1 - 1, t2 - 1, nc);
                    }*/
                    MoreCardWindow();
                }
                if ((event.key.code == sf::Keyboard::Numpad3 || event.key.code == sf::Keyboard::Num3)) {
                    moverLareserva();
                }
                if (event.key.code == sf::Keyboard::H && !twoWin) {
                    twoWin = true;
                    HelpWindow();
                }
            }
        }
        window.clear(sf::Color(15, 185, 74, 255));
        mostrarPilas();
        /*cout << "\n\nElija una de las opciones: " << endl;
        cout << "\n1.-Mover una carta.";
        cout << "\n2.-Mover varias cartas.";
        cout << "\n3.-Mover la reserva." << endl;
        system("cls");*/
        twoWin = false;
        window.draw(help);
        window.display();
        /*cout << "\n\nElija una de las opciones: " << endl;
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
        cin.get();*/
    }
    //cout << "Felicidades ganaste" << endl;
}
