#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<cstring>
#include<cstdio>
#include<cmath>
#include<ctime>
using namespace std;

typedef struct minefield
{
	char bomba;
	int bprox;
	bool saber;	
}field;

typedef struct ranking
{
	char nome[50];
	char tamanho;
	char modo;
	int jogadas;
}Rank;

int MenuTamanho();
double MenuDificuldade();
void primeiraEscolha(int, int*, int*); //Função para receber os parametros da primeira escolha antes de gerar o campo
field* valorBombs(int, double, int*, int, int); //Função para gerar o Campo Minado
void apagaCampo(field*, int); //Função para apagar o Campo Minado da memória dinâmica
void testCampo(field*, int); //Função para imprimir o Campo Minado gerado com os valores desvendados
int funcaoJogo(field*, int, int, int, int, int*); //Função para imprimir o jogo
bool escolheCasa(field*, int, int, int*); //Função para escolher a casa
void casasProximas(field*, int, int, int, int*); //Função para testar as casas sequenciais da casa escolhida
void atualizaRanking(int, int, double); //Função para atualizar o ranking caso ganhe
void imprimeRanking(int, double); //Função para imprimir o ranking na tela

int main()
{
	system ("clear");
	cout << "   ____                              __  __ _                 _       \n  / ___|__ _ _ __ ___  _ __   ___   |  \\/  (_)_ __   __ _  __| | ___  \n | |   / _` | '_ ` _ \\| '_ \\ / _ \\  | |\\/| | | '_ \\ / _` |/ _` |/ _ \\ \n | |__| (_| | | | | | | |_) | (_) | | |  | | | | | | (_| | (_| | (_) |\n  \\____\\__,_|_| |_| |_| .__/ \\___/  |_|  |_|_|_| |_|\\__,_|\\__,_|\\___/ \n                      |_|                                             \n\n";
	
	cout << "\t\tSelecione o tamanho do tabuleiro:\n [1]   9x9\n [2]  15x15\n [3]  20x20\n [4] Personalizado\n\n";
	
	int tam = MenuTamanho();
	
	cout << "\t\tSelecione a dificuldade:\n [1] Fácil (10% de bombas)\n [2] Intermediário (15% de bombas)\n [3] Difícil (20% de bombas)\n [4] Personalizado\n\n";
	
	double bomb = MenuDificuldade();
	
	int qtdbomba = 0;
	
	cout << "\n\n";
	
	int numx, numy;
	int qtdJogadas = 1;
	
	primeiraEscolha(tam, &numx, &numy);
	
	field *bombas = valorBombs(tam, bomb, &qtdbomba, numx, numy);
	
	int casasAbertas = funcaoJogo(bombas, tam, qtdbomba, numx, numy, &qtdJogadas);
	
	if(casasAbertas == pow(tam,2)-qtdbomba)
		atualizaRanking(tam, qtdJogadas, bomb);
	
	imprimeRanking(tam, bomb);
	
	apagaCampo(bombas, tam);
	
	return 0;
}

int MenuTamanho()
{
	char inputTamanho = '0';
	
	int valorLimite = 0;
	
	cout << ">>";
	cin >> inputTamanho;
	
	cin.ignore();
		
	switch(inputTamanho)
	{
		case '1':
			valorLimite = 9;
			break;
		case '2':
			valorLimite = 15;
			break;
		case '3':
			valorLimite = 20;
			break;
		case '4':
			cout << "Insira o tamanho do campo(min:6): ";
			cin >> valorLimite;
			while (valorLimite < 6)
			{
				cout << "Valor não aceito. Insira outro valor: ";
				cin >> valorLimite;
			}
			break;
		default:
			cout << "Opção inválida. Tente novamente.\n\n";
			MenuTamanho();
			break;
	}
	
	return valorLimite;
}

double MenuDificuldade()
{
	char inputDificuldade = '0';
	
	double valorBombas = 0;
	
	cout << ">>";
	cin >> inputDificuldade;
	
	cin.ignore();
		
	switch(inputDificuldade)
	{
		case '1':
			valorBombas = 0.1;
			break;
		case '2':
			valorBombas = 0.15;
			break;
		case '3':
			valorBombas = 0.2;
			break;
		case '4':
			cout << "Insira a porcentagem de bombas(min: 5%, max: 80%): ";
			cin >> valorBombas;
			while (valorBombas < 5 || valorBombas > 80)
			{
				cout << "Valor não aceito. Insira outro valor: ";
				cin >> valorBombas;
			}
			valorBombas /= 100;
			break;
		default:
			cout << "Opção inválida. Tente novamente.\n\n";
			MenuDificuldade();
			break;
	}
	
	return valorBombas;
}

void primeiraEscolha(int n, int* x, int* y)
{
	cout << "Campo Minado-\n" << n << "x" << n << "\n\n";
	
	cout << "Para jogar, escolha a posição para desvendar;\n";

	cout << setw(4) << setfill(' ') << "|";
	for(int i = 1; i<=n; i++)
		cout << setw(2) << setfill(' ') << i << "|";
	cout << "\n" << setw(5+(n*3)) << setfill('-');
	
	cout << "\n";
	
	for(int i = 0; i<n; i++)
	{
		cout << "|" << setw(2) << setfill(' ') << i+1 << "|";
		for(int j = 0; j<n; j++)
			cout << setw(2) << setfill(' ') << "[]" << '|';
		cout << "\n";
		cout << setw(5+(n*3)) << setfill('-') << "\n";			
	}
	cout << endl;

	cout << "Posição:";
	cin >> *x >> *y;
	cin.ignore();
	while(*x < 1 || *x > n || *y < 1 || *y > n)
	{
		cout << "Casa inexistente, escolha outra:\n\nPosição: ";
		cin >> *x >> *y;
	}
	*x -= 1; 
	*y -= 1;
}
	
field* valorBombs(int n, double b, int* qtdbombas, int x, int y) // Função para gerar o Campo Minado
{
	field* bombas = NULL;
	bombas = new field[n*n];
		
	for(int i = 0; i<n*n; i++)
		bombas[i].bomba = '0';
		
	double totalBombas = round(n*n*b);
	
	*qtdbombas = totalBombas;
	
	srand(time(NULL));
	
	while(totalBombas > 0)
	{
		int i = rand()%(n*n);
		if(bombas[i].bomba != 'B' && i != (x*n)+y)
		{
			bombas[i].bomba = 'B';
			totalBombas--;
		}
		i = 0;
	}
	
	for(int i = 0; i<n; i++)
		for(int j = 0; j<n; j++)
		{
			if(bombas[(i*n)+j].bomba != 'B')
			{
				if(((i-1)*n)+j >= 0)
					if(bombas[((i-1)*n)+j].bomba == 'B')
						bombas[(i*n)+j].bprox++;
				if((i*n)+(j-1) >= 0 && (i*n)+(j-1) >= (i*n))
					if(bombas[(i*n)+(j-1)].bomba == 'B')
						bombas[(i*n)+j].bprox++;
				if(((i+1)*n)+j < n*n)
					if(bombas[((i+1)*n)+j].bomba == 'B')
						bombas[(i*n)+j].bprox++;
				if((j+1) < n)
					if(bombas[(i*n)+(j+1)].bomba == 'B')
						bombas[(i*n)+j].bprox++;
				bombas[(i*n)+j].bomba = bombas[(i*n)+j].bprox + '0';
			}
		}
	
	return bombas;
}

void apagaCampo(field* bombas, int n)
{
	delete[] bombas;
}

void testCampo(field* bombas, int n)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			cout << bombas[(i*n)+j].bomba << " ";
		cout << "\n";
	}
}

int funcaoJogo(field* bombas, int n, int qtdBombas, int x, int y, int* qtdJogadas)
{
	int casasAbertas = 1;
		
	for(int i = 0; i<n; i++)
		for(int j = 0; j<n; j++)
			bombas[(i*n)+j].saber = false;
			
	bombas[(x*n)+y].saber = true;
	casasProximas(bombas, n, x, y, &casasAbertas);
	
	bool gameover = false;
	
	while(gameover != true)
	{
		cout << setw(4) << setfill(' ') << "|";
		for(int i = 1; i<=n; i++)
			cout << setw(2) << setfill(' ') << i << "|";
		cout << "\n" << setw(5+(n*3)) << setfill('-');
		
		cout << "\n";
		
		for(int i = 0; i<n; i++)
		{
			cout << "|" << setw(2) << setfill(' ') << i+1 << "|";
			for(int j = 0; j<n; j++)
			{
				if(!bombas[(i*n)+j].saber)
					cout << setw(2) << setfill(' ') << "[]" << '|';
				else if(bombas[(i*n)+j].bomba == '0')
					cout << setw(3) << setfill(' ') << '|';
				else
					cout << setw(2) << setfill(' ') << bombas[(i*n)+j].bomba << '|';
			}
			cout << "\n";
			cout << setw(5+(n*3)) << setfill('-') << "\n";			
		}
		cout << "\n";
		gameover = escolheCasa(bombas, n, qtdBombas, &casasAbertas);
		(*qtdJogadas)++;
	}
	
	if(casasAbertas < (n*n)-qtdBombas)
	{
		cout << setw(4) << setfill(' ') << "|";
		for(int i = 1; i<=n; i++)
			cout << setw(2) << setfill(' ') << i << "|";
		cout << "\n" << setw(5+(n*3)) << setfill('-');
		
		cout << "\n";
		
		for(int i = 0; i<n; i++)
		{
			cout << "|" << setw(2) << setfill(' ') << i+1 << "|";
			for(int j = 0; j<n; j++)
				if(bombas[(i*n)+j].bomba != '0')
					cout << setw(2) << setfill(' ') << bombas[(i*n)+j].bomba << '|';
				else if(bombas[(i*n)+j].bomba == '0')
					cout << setw(3) << setfill(' ') << '|';
			cout << "\n";
			cout << setw(5+(n*3)) << setfill('-') << "\n";			
		}
		cout << "\n";
		cout << "BOOM! Fim de jogo!\n";
	}
	else if(casasAbertas == (n*n)-qtdBombas)
	{
		cout << setw(4) << setfill(' ') << "|";
		for(int i = 1; i<=n; i++)
			cout << setw(2) << setfill(' ') << i << "|";
		cout << "\n" << setw(5+(n*3)) << setfill('-');
		
		cout << "\n";
		
		for(int i = 0; i<n; i++)
		{
			cout << "|" << setw(2) << setfill(' ') << i+1 << "|";
			for(int j = 0; j<n; j++)
				if(!bombas[(i*n)+j].saber)
					cout << setw(2) << setfill(' ') << "[]" << '|';
				else if(bombas[(i*n)+j].bomba == '0')
					cout << setw(3) << setfill(' ') << '|';
				else
					cout << setw(2) << setfill(' ') << bombas[(i*n)+j].bomba << '|';
			cout << "\n";
			cout << setw(5+(n*3)) << setfill('-') << "\n";			
		}
		cout << "\n";
		cout << "Parabéns, você venceu!\n";
	}
	return casasAbertas;
}

bool escolheCasa(field* bombas, int n, int totalBombas, int* casasAbertas)
{	
	int x = 0, y = 0;
	
	bool explosao = false, ganhou = false;
	
	cout << "Posição:";
	cin >> x >> y;
	cin.ignore();
	int numx = x-1;
	int numy = y-1;
	if(numx < 0 || numx > n-1 || numy < 0 || numy > n-1)
		cout << "Casa inexistente, escolha outra:\n";
	else if(bombas[(numx*n)+numy].saber == false)
	{
		bombas[(numx*n)+numy].saber = true;
		if(bombas[(numx*n)+numy].bomba == 'B')
		{
			explosao = true;
			return explosao;
		}
		else
			(*casasAbertas)++;
		if(bombas[(numx*n)+numy].bprox == 0)
		{
			if(((numx-1)*n)+numy >= 0)
				if(bombas[((numx-1)*n)+numy].saber == false)
					if(bombas[((numx-1)*n)+numy].bomba != 'B')
					{
						bombas[((numx-1)*n)+numy].saber = true;
						(*casasAbertas)++;
						if(bombas[((numx-1)*n)+numy].bprox == 0)
							casasProximas(bombas, n, numx-1, numy, casasAbertas);
					}
			if((numx*n)+(numy-1) >= 0 && (numx*n)+(numy-1) >= (numx*n))
				if(bombas[(numx*n)+numy-1].saber == false)
					if(bombas[(numx*n)+(numy-1)].bomba != 'B')
					{
						bombas[(numx*n)+(numy-1)].saber = true;
						(*casasAbertas)++;
						if(bombas[(numx*n)+(numy-1)].bprox == 0)
							casasProximas(bombas, n, numx, numy-1, casasAbertas);
					}
			if(((numx+1)*n)+numy < n*n)
				if(bombas[((numx+1)*n)+numy].saber == false)	
					if(bombas[((numx+1)*n)+numy].bomba != 'B')
					{
						bombas[((numx+1)*n)+numy].saber = true;
						(*casasAbertas)++;
						if(bombas[((numx+1)*n)+numy].bprox == 0)
							casasProximas(bombas, n, numx+1, numy, casasAbertas);
					}
			if((numy+1) < n)
				if(bombas[(numx*n)+(numy+1)].saber == false)
					if(bombas[(numx*n)+(numy+1)].bomba != 'B')
					{
						bombas[(numx*n)+(numy+1)].saber = true;
						(*casasAbertas)++;
						if(bombas[(numx*n)+(numy+1)].bprox == 0)
							casasProximas(bombas, n, numx, numy+1, casasAbertas);
					}
		}
	}
	else if(bombas[(numx*n)+numy].saber == true)
		cout << "Casa já desvendada, escolha outra:\n";
	
	if(*casasAbertas >= (n*n)-totalBombas)
		ganhou = true;
	
	return ganhou;
}

void casasProximas (field* bombas, int n, int numx, int numy, int* casasAbertas)
{
	if(bombas[(numx*n)+numy].bprox == 0)
	{
		if(((numx-1)*n)+numy >= 0)
			if(bombas[((numx-1)*n)+numy].saber == false)
				if(bombas[((numx-1)*n)+numy].bomba != 'B')
				{
					bombas[((numx-1)*n)+numy].saber = true;
					(*casasAbertas)++;
					if(bombas[((numx-1)*n)+numy].bprox == 0)
						casasProximas(bombas, n, numx-1, numy, casasAbertas);
				}
		if((numx*n)+(numy-1) >= 0 && (numx*n)+(numy-1) >= (numx*n))
			if(bombas[(numx*n)+numy-1].saber == false)
				if(bombas[(numx*n)+(numy-1)].bomba != 'B')
				{
					bombas[(numx*n)+(numy-1)].saber = true;
					(*casasAbertas)++;
					if(bombas[(numx*n)+(numy-1)].bprox == 0)
						casasProximas(bombas, n, numx, numy-1, casasAbertas);
				}
		if(((numx+1)*n)+numy < n*n)
			if(bombas[((numx+1)*n)+numy].saber == false)	
				if(bombas[((numx+1)*n)+numy].bomba != 'B')
				{
					bombas[((numx+1)*n)+numy].saber = true;
					(*casasAbertas)++;
					if(bombas[((numx+1)*n)+numy].bprox == 0)
						casasProximas(bombas, n, numx+1, numy, casasAbertas);
				}
		if((numy+1) < n)
			if(bombas[(numx*n)+(numy+1)].saber == false)
				if(bombas[(numx*n)+(numy+1)].bomba != 'B')
				{
					bombas[(numx*n)+(numy+1)].saber = true;
					(*casasAbertas)++;
					if(bombas[(numx*n)+(numy+1)].bprox == 0)
							casasProximas(bombas, n, numx, numy+1, casasAbertas);
				}
	}
}

void atualizaRanking(int n, int qtdJogadas, double bomb)
{
		int dificuldade = bomb*100;
		Rank vencedor;
		
		switch(n)
		{
			case 9:
				vencedor.tamanho = '1';
				break;
			case 15:
				vencedor.tamanho = '2';
				break;
			case 20:
				vencedor.tamanho = '3';
				break;
		}
		switch(dificuldade)
		{
			case 10:
				vencedor.modo = 'F';
				break;
			case 15:
				vencedor.modo = 'M';
				break;
			case 20:
				vencedor.modo = 'D';
				break;
		}
		
		Rank rank[90];
		
		vencedor.jogadas = qtdJogadas;
		
		fstream RankFile;
		
		RankFile.open("ranking.dat",ios::binary|ios::in);
		
		RankFile.read((char*)rank, sizeof(Rank)*90);
		
		cout << "Insira seu nome para o Ranking: ";
		fgets(vencedor.nome, 50, stdin);
		
		char* replace = strchr(vencedor.nome, '\n');
		*replace = '\0';
		
		for(int i = 0; i<90; i++)
			if(rank[i].tamanho == vencedor.tamanho && rank[i].modo == vencedor.modo)
				if(vencedor.jogadas < rank[i].jogadas)
				{
					int ranksAbaixo = 9-i%10;
					for(int j = i+ranksAbaixo; j>i; j--)
					{
						strcpy(rank[j].nome, rank[j-1].nome);
						rank[j].jogadas = rank[j-1].jogadas;
					}
					strcpy(rank[i].nome, vencedor.nome);
					rank[i].jogadas = vencedor.jogadas;
					break;
				}
		RankFile.close();
		
		RankFile.open("ranking.dat",ios::binary|ios::out);
		if(RankFile.fail())
			cout << "Falha na abertura de arquivo de ranking." << endl;
		else
			RankFile.write((char*)rank, sizeof(Rank)*90);
			
		RankFile.close();
}

void imprimeRanking(int n, double bomb)
{
	int dificuldade;
	char tamanho, modo;
	dificuldade = bomb*100;
	
	Rank rank[90];
	
	ifstream RankFile;
	
	RankFile.open("ranking.dat",ios::binary);
	RankFile.read((char*)rank, sizeof(Rank)*90);		
	
	switch(n)
	{
		case 9:
			tamanho = '1';
			break;
		case 15:
			tamanho = '2';
			break;
		case 20:
			tamanho = '3';
			break;
	}
	switch(dificuldade)
	{
		case 10:
			modo = 'F';
			break;
		case 15:
			modo = 'M';
			break;
		case 20:
			modo = 'D';
			break;
	}
		
	int pos = 1;
	cout << endl;
	cout << "\tRanking" << endl;
	for(int i = 0; i<90; i++)
		if(rank[i].tamanho == tamanho && rank[i].modo == modo)
		{
			cout << pos << "\t" << rank[i].nome << " " << rank[i].tamanho << " " << rank[i].modo << " " << rank[i].jogadas << endl;
			pos++;
		}
			
	RankFile.close();
}
