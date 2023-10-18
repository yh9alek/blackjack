#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define ROJO 252
#define NEGRO 240

#define CORAZON 3
#define DIAMANTE 4
#define TREBOL 5
#define PICA 6

#define POS_Y 7
#define POS_CARTAS_JUGADOR 9
#define POS_MASO_CARTAS 36
#define POS_CARTAS_CPU 50

std::string baraja[52];
std::string carta;
int pJ = 0, pC = 0, numCartasJugador = 0, numCartasCPU;
int incremento = 0;

//Lógica de juego
void gotoxy(int, int);
void inicio();
void final();
void crearCartas(std::string);
void crearBaraja();
void mostrarBaraja();
int generarAleatorio(int, int);
void mezclarBaraja();
std::string pedirCarta();
int obtenerValor(std::string);
void reiniciarJuego();
void determinarGanador();
bool continuar();
int valorAS();
void blackjack();
void asignarCarta(int);
void mensaje();
void limpiarMensaje();
void comenzarJuego();

//Dibujado de Cartas
void cambiarColor(int);
void resetearColor();
void dibujarMarco(int, int);
void dibujarVolteada(int, int);
void dibujar2(char, int, int);
void dibujar3(char, int, int);
void dibujar4(char, int, int);
void dibujar5(char, int, int);
void dibujar6(char, int, int);
void dibujar7(char, int, int);
void dibujar8(char, int, int);
void dibujar9(char, int, int);
void dibujar10(char, int, int);
void dibujarEspecial(char, char, int, int);
char obtenerTipo(char);
void dibujarCarta(std::string, int);
void mostrarGUI();
void ocultar();

int main()
{
	srand(time(NULL));
	inicio();
	comenzar:
		reiniciarJuego();
		crearBaraja();
		system("cls");
		mezclarBaraja();
		comenzarJuego();
		if(continuar())
			goto comenzar;
	final();
	return 0;
}

void gotoxy(int X, int Y)
{
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = X;
	dwPos.Y = Y;
	SetConsoleCursorPosition(hcon,dwPos);
}

void inicio()
{
	gotoxy(35, 6);
	printf("BLACKJACK %c", 6);
	gotoxy(12, 8);
	printf("El objetivo del juego es llegar a 21 puntos sin pasarse");
	gotoxy(3, 12);
	printf("Como jugar?");
	gotoxy(6, 14);
	printf("El jugador comienza con 2 cartas, y puede tomar mas cartas.");
	gotoxy(6, 15);
	printf("El Crupier (CPU) comienza con 2 cartas, pero se muestra solo una de ellas.");
	gotoxy(6, 16);
	printf("El jugador al tomar mas cartas debe estar conciente de no pasarse de 21");
	gotoxy(6, 17);
	printf("puntos, de lo contrario, pierde automaticamente.");
	gotoxy(6, 18);
	printf("Las cartas del 2 al 10 valen ese numero, el AS puede valer 1 o 11.");
	gotoxy(6, 19);
	printf("Las cartas J, K y Q valen 10");
	gotoxy(6, 21);
	printf("Al final, gana el jugador que mas se haya acercado al numero 21.");
	gotoxy(3, 23);
	printf("EMPEZAR...");
	getch();
}

void final()
{
	system("cls");
	gotoxy(30, 10);
	printf("GRACIAS POR JUGAR %c", 5);
	gotoxy(15, 21);
	printf("Copyright %c17/04/2023 YOHAN ALEK PLAZOLA ARANGURE", 169);
	getch();
}

//    - Lógica de juego -
void crearCartas(std::string denominacion)
{
	static int incremento = 0;
	if(incremento >= 50)
		incremento = 0;
	std::string tipos = "CDHS";
	for(int d = 0; d < denominacion.length(); d++)
	{
		for(int t = 0; t < tipos.length(); t++)
		{
			baraja[t + incremento] = denominacion[d];
			baraja[t + incremento] += tipos[t];
		}
		incremento += 4;
	}
	if(denominacion.length() > 4)
	{
		for(int t = 0; t < tipos.length(); t++)
		{
			baraja[t + incremento] = "10";
			baraja[t + incremento] += tipos[t];
		}
		incremento += 4;
	}
	std::cout << incremento << std::endl;
}

void crearBaraja()
{
	crearCartas("23456789");
	crearCartas("AJKQ");
}

void mostrarBaraja()
{
	for(int x = 0; x < 52; x++)
	{
		std::cout << x << ".-  " << baraja[x] << std::endl;
	}
}

int generarAleatorio(int inicio, int final)
{	
	return rand() % (inicio, final);
}

void mezclarBaraja()
{
	bool cartas[52];
	int numero = 0, ind1 = 0, ind2 = 0;
	std::string aux;
	for(int mezclas = 0; mezclas < 26; mezclas++)
	{
		for(int x = 0; x < 2; x++)
		{
			generar:
				numero = generarAleatorio(0, 52);
				if(!cartas[numero])
				{
					cartas[numero] = true;
					if(x < 1)
					{
						ind1 = numero;
						aux = baraja[numero];	
					}
					else
						ind2 = numero;
				}
				else
					goto generar;
		}
		baraja[ind1] = baraja[ind2];
		baraja[ind2] = aux;
	}
}

std::string pedirCarta()
{
	static int indice = 52;
	if(numCartasJugador <= 0)
		indice = 52;
	indice--;
	std::string carta = baraja[indice];
	return carta;
}

int obtenerValor(std::string carta)
{
	if(carta.length() > 2)
	{
		return 10;
	}
	switch(carta[0])
	{
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'A':
			return 11;
		case 'J':
			return 10;
		case 'K':
			return 10;
		case 'Q':
			return 10;
	}
}

void reiniciarJuego()
{
	pJ = 0;
	numCartasJugador = 0;
	pC = 0;
	numCartasCPU = 0;
	incremento = 0;
	for(int x = 0; x < 52; x++)
		baraja[x] = "";
}

void determinarGanador()
{
	bool ganador = false;
	gotoxy(29, 7);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 191);
	gotoxy(29, 8);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 9);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 10);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 11);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 12);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 13);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 217);
	gotoxy(30, 9);
	if(pJ == pC)
	{
		if(pJ < 21)
		{
			printf("      EMPATE...");
			return;
		}
		else
		{
			if(numCartasCPU == 2 && numCartasJugador > 2)
				ganador = false;
			else if(numCartasJugador == 2 && numCartasCPU > 2)
				ganador = true;
			else
			{
				printf("      EMPATE...");
				return;
			}
		}
	}
	else if(pJ > pC)
	{
		if(pJ > 21)
			ganador = false;
		else
			ganador = true;
	}
	else
	{
		if(pC > 21)
			ganador = true;
		else
			ganador = false;
	}
	if(ganador)
		printf("      GANASTE...");
	else
		printf("     PERDISTE...");
}

bool continuar()
{
	int op = 0;
	gotoxy(30, 10);
	printf("   Volver a jugar?");
	gotoxy(30, 11);
	printf("     1.- SI");
	gotoxy(30, 12);
	printf("     2.- NO   >> ");
	std::cin >> op;
	return op < 2;
}

int valorAS()
{
	int op = 0;
	gotoxy(29, 7);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 191);
	gotoxy(29, 8);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 9);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 10);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 11);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 12);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 13);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 217);
	gotoxy(30, 10);
	printf("   Valor para tu AS?");
	gotoxy(30, 11);
	printf("     1.- 1");
	gotoxy(30, 12);
	printf("     2.- 11   >> ");
	std::cin >> op;
	limpiarMensaje();
	dibujarVolteada(POS_MASO_CARTAS, POS_Y);
	if(op < 2)
	    return 1;
	else
		return 11;
}

void blackjack()
{
	if(numCartasJugador == 2 && pJ == 21)
	{
		gotoxy(10, 20);
		HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
		SetConsoleTextAttribute(hConsole, 3);
		printf("BLACKJACK");
		resetearColor();
	}
	if(numCartasCPU == 2 && pC == 21)
	{
		gotoxy(51, 20);
		HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
		SetConsoleTextAttribute(hConsole, 3);
		printf("BLACKJACK");
		resetearColor();
	}
}

void asignarCarta(int jugador)
{
	carta = pedirCarta();
	if(jugador == 1)
	{
		dibujarCarta(carta, POS_CARTAS_JUGADOR);
		if(carta[0] == 'A')
			pJ += valorAS();
		else
			pJ += obtenerValor(carta);
		gotoxy(18, 19);
		printf("%d", pJ);
		numCartasJugador++;
	}
	else
	{
		pC += obtenerValor(carta);
		gotoxy(59, 19);
		printf("%d", pC);
		dibujarCarta(carta, POS_CARTAS_CPU);
		numCartasCPU++;
	}
	blackjack();
}

void mensaje()
{
	gotoxy(29, 7);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 218, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 191);
	gotoxy(29, 8);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 9);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 10);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 11);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 12);
	printf("%c                   %c", 179, 179);
	gotoxy(29, 13);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 192, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 217);
	gotoxy(30, 9);
	printf("Debes tomar almenos");
	gotoxy(30, 10);
	printf("2 cartas...");
	getch();
}

void limpiarMensaje()
{
	gotoxy(29, 7);
	printf("                     ");
	gotoxy(29, 8);
	printf("                     ");
	gotoxy(29, 9);
	printf("                     ");
	gotoxy(29, 10);
	printf("                     ");
	gotoxy(29, 11);
	printf("                     ");
	gotoxy(29, 12);
	printf("                     ");
	gotoxy(29, 13);
	printf("                     ");
}

void comenzarJuego()
{
	bool continuar = true;
	system("cls");
	mostrarGUI();
	dibujarVolteada(POS_MASO_CARTAS, POS_Y);
	gotoxy(36, 4);
	printf("TU TURNO");
	char input;
	for(int x = 0; x < 2; x++)
		asignarCarta(1);
	if(pJ == 21)
		continuar = false;
	incremento = 0;
	asignarCarta(2);
	incremento = 6;
	dibujarVolteada(POS_CARTAS_CPU + 3, POS_Y);
	while(continuar)
	{
		input = getch();
		switch(tolower(input))
		{
			case 'a':
				asignarCarta(1);
				if(pJ > 21)
				{
					determinarGanador();
					return;
				}
				if(pJ == 21)
					continuar = false;
				break;
			case 'd':
				if(numCartasJugador < 2)
				{
					mensaje();
					limpiarMensaje();
					dibujarVolteada(POS_MASO_CARTAS, POS_Y);
				}
				else
					continuar = false;
				break;
		}
	}
	incremento = 3;
	ocultar();
	gotoxy(34, 4);
	printf("TURNO DE CPU");
	Sleep(1000);
	do
	{
		Sleep(1000);
		asignarCarta(2);
		if(pC == 21)
			break;
	}while(pC < 17);
	determinarGanador();	
}
//    -  Dibujado de Cartas -
void cambiarColor(int tipo)
{
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	if(tipo == CORAZON || tipo == DIAMANTE)
		SetConsoleTextAttribute(hConsole, ROJO);
	else
		SetConsoleTextAttribute(hConsole, NEGRO);
}

void resetearColor()
{
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute(hConsole, 7);
}

void dibujarMarco(int x, int y)
{
	gotoxy(x, y);
	printf("%c%c%c%c%c%c%c%c%c", 218, 196, 196, 196, 196, 196, 196, 196, 191);
	gotoxy(x, y + 1);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 2);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 3);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 4);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 5);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 6);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 7);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 8);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 9);
	printf("%c       %c", 179, 179);
	gotoxy(x, y + 10);
	printf("%c%c%c%c%c%c%c%c%c", 192, 196, 196, 196, 196, 196, 196, 196, 217);
}

void dibujarVolteada(int x, int y)
{
	cambiarColor(240);
	gotoxy(x, y);
	printf("%c%c%c%c%c%c%c%c%c", 218, 196, 196, 196, 196, 196, 196, 196, 191);
	gotoxy(x, y + 1);
	printf("%c%c%c%c%c%c%c%c%c", 179, 176, 176, 177, 178, 177, 176, 176, 179);
	gotoxy(x, y + 2);
	printf("%c%c%c%c%c%c%c%c%c", 179, 176, 177, 178, 178, 178, 177, 176, 179);
	gotoxy(x, y + 3);
	printf("%c%c%c%c%c%c%c%c%c", 179, 176, 177, 178, 178, 178, 177, 176, 179);
	gotoxy(x, y + 4);
	printf("%c%c%c%c%c%c%c%c%c", 179, 177, 178, 178, 178, 178, 178, 177, 179);
	gotoxy(x, y + 5);
	printf("%c%c%c%c%c%c%c%c%c", 179, 177, 178, 178, 176, 178, 178, 177, 179);
	gotoxy(x, y + 6);
	printf("%c%c%c%c%c%c%c%c%c", 179, 177, 178, 178, 178, 178, 178, 177, 179);
	gotoxy(x, y + 7);
	printf("%c%c%c%c%c%c%c%c%c", 179, 176, 177, 178, 178, 178, 177, 176, 179);
	gotoxy(x, y + 8);
	printf("%c%c%c%c%c%c%c%c%c", 179, 176, 177, 178, 178, 178, 177, 176, 179);
	gotoxy(x, y + 9);
	printf("%c%c%c%c%c%c%c%c%c", 179, 176, 176, 177, 178, 177, 176, 176, 179);
	gotoxy(x, y + 10);
	printf("%c%c%c%c%c%c%c%c%c", 192, 196, 196, 196, 196, 196, 196, 196, 217);
	resetearColor();
}

void dibujar2(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 2);
	gotoxy(x + 2, y + 2);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 3);
	printf("     ");
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("     ");
	gotoxy(x + 2, y + 6);
	printf("     ");
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("  %c  ", tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 2);
	resetearColor();
}

void dibujar3(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 3);
	gotoxy(x + 2, y + 2);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 3);
	printf("     ");
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 6);
	printf("     ");
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("  %c  ", tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 3);
	resetearColor();
}

void dibujar4(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 4);
	gotoxy(x + 2, y + 2);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 3);
	printf("     ");
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("     ");
	gotoxy(x + 2, y + 6);
	printf("     ");
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 4);
	resetearColor();
}

void dibujar5(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 5);
	gotoxy(x + 2, y + 2);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 3);
	printf("     ");
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 6);
	printf("     ");
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 5);
	resetearColor();
}

void dibujar6(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 6);
	gotoxy(x + 2, y + 2);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 3);
	printf("     ");
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 6);
	printf("     ");
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 6);
	resetearColor();
}

void dibujar7(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 7);
	gotoxy(x + 2, y + 2);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 3);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 6);
	printf("     ");
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 7);
	resetearColor();
}

void dibujar8(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 8);
	gotoxy(x + 2, y + 2);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 3);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 6);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 8);
	resetearColor();
}

void dibujar9(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 9);
	gotoxy(x + 2, y + 2);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 3);
	printf("     ");
	gotoxy(x + 2, y + 4);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 5);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 6);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 7, y + 9);
	printf("%d", 9);
	resetearColor();
}

void dibujar10(char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%d", 10);
	gotoxy(x + 2, y + 2);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 3);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 4);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 5);
	printf("     ");
	gotoxy(x + 2, y + 6);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 2, y + 7);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 8);
	printf("%c   %c", tipo, tipo);
	gotoxy(x + 6, y + 9);
	printf("%d", 10);
	resetearColor();
}

void dibujarEspecial(char esp, char tipo, int x, int y)
{
	//Dibujar Marco
	cambiarColor(tipo);
	dibujarMarco(x, y);
	//Dibujar detalles
	gotoxy(x + 1, y + 1);
	printf("%c", esp);
	gotoxy(x + 2, y + 2);
	printf("     ");
	gotoxy(x + 2, y + 3);
	printf("     ");
	gotoxy(x + 2, y + 4);
	printf("     ");
	gotoxy(x + 2, y + 5);
	printf("  %c  ", tipo);
	gotoxy(x + 2, y + 6);
	printf("     ");
	gotoxy(x + 2, y + 7);
	printf("     ");
	gotoxy(x + 2, y + 8);
	printf("     ");
	gotoxy(x + 7, y + 9);
	printf("%c", esp);
	resetearColor();
}


char obtenerTipo(char tipo)
{
	switch(tipo)
	{
		case 'H':
			return 3;
		case 'D':
			return 4;
		case 'C':
			return 5;
		case 'S':
			return 6;
	}
}

void dibujarCarta(std::string dato, int pos)
{
	char tipo;
	if(dato.length() > 2)
	{
		tipo = obtenerTipo(dato[2]);
		dibujar10(tipo, pos + incremento, POS_Y);
		incremento += 3;
		return;
	}
	tipo = obtenerTipo(dato[1]);
	switch(dato[0])
	{
		case '2':
			dibujar2(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case '3':
			dibujar3(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case '4':
			dibujar4(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case '5':
			dibujar5(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case '6':
			dibujar6(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case '7':
			dibujar7(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case '8':
			dibujar8(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case '9':
			dibujar9(tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case 'A':
			dibujarEspecial(dato[0], tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case 'J':
			dibujarEspecial(dato[0], tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case 'K':
			dibujarEspecial(dato[0], tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
		case 'Q':
			dibujarEspecial(dato[0], tipo, pos + incremento, POS_Y);
			incremento += 3;
			break;
	}
}

void mostrarGUI()
{
	gotoxy(10, 5);
	printf("===== JUGADOR =====");
	gotoxy(10, 19);
	printf("PUNTOS: %d", pJ);
	
	gotoxy(51, 5);
	printf("=====   CPU   =====");
	gotoxy(51, 19);
	printf("PUNTOS: %d", pC);
	
	gotoxy(35, 19);
	printf("%c%c%c%c%c", 218, 196, 196, 196, 191);
	gotoxy(35, 20);
	printf("%c A %c", 179, 179);
	gotoxy(35, 21);
	printf("%c%c%c%c%c", 192, 196, 196, 196, 217);
	gotoxy(35, 22);
	printf("Pedir");
	gotoxy(41, 19);
	printf("%c%c%c%c%c", 218, 196, 196, 196, 191);
	gotoxy(41, 20);
	printf("%c D %c", 179, 179);
	gotoxy(41, 21);
	printf("%c%c%c%c%c", 192, 196, 196, 196, 217);
	gotoxy(41, 22);
	printf("Plantar");
}

void ocultar()
{
	gotoxy(35, 19);
	printf("     ");
	gotoxy(35, 20);
	printf("     ");
	gotoxy(35, 21);
	printf("     ");
	gotoxy(35, 22);
	printf("     ");
	gotoxy(41, 19);
	printf("     ");
	gotoxy(41, 20);
	printf("     ");
	gotoxy(41, 21);
	printf("     ");
	gotoxy(41, 22);
	printf("       ");
}
