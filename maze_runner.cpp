#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

using namespace std;


// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;
string linha;
string coluna;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE* file = fopen(file_name, "r"); 

	// Le o numero de linhas e colunas (fscanf) 
	fscanf(file, "%d %d", &num_rows, &num_cols);
	maze = (char**)malloc(num_rows * sizeof(char*)); 
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = (char*)malloc(num_cols * sizeof(char)); 
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            // Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]. Se o valor for 'e' salvar o valor em initial_pos
            fscanf(file, " %c", &maze[i][j]); 

            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
    }

    // Fecha o arquivo
    fclose(file);
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos)
{
	pos_t pos_atual;
	pos_atual.j = pos.j;
	pos_atual.i = pos.i;
	int finalizado = 0;

	while (finalizado != 1)
	{
		int i = pos_atual.i;
		int j = pos_atual.j;

		if (j + 1 < num_cols && (maze[i][j + 1] == 'x' || maze[i][j + 1] == 's'))
		{
			pos_t pos_verif;
			pos_verif.i = i;
			pos_verif.j = j + 1;
			maze[i][j + 1] == '.';
			valid_positions.push(pos_verif);
		}

		if (j - 1 > -1 && (maze[i][j - 1] == 'x' || maze[i][j - 1] == 's'))
		{
			pos_t pos_verif;
			pos_verif.i = i;
			pos_verif.j = j - 1;
			maze[i][j - 1] == '.';
			valid_positions.push(pos_verif);
		}

		if (i + 1 < num_rows && (maze[i + 1][j] == 'x' || maze[i + 1][j] == 's'))
		{
			pos_t pos_verif;
			pos_verif.i = i + 1;
			pos_verif.j = j;
			maze[i + 1][j] == '.';
			valid_positions.push(pos_verif);
		}

		if (i - 1 > -1 && (maze[i - 1][j] == 'x' || maze[i - 1][j] == 's'))
		{
			pos_t pos_verif;
			pos_verif.i = i - 1;
			pos_verif.j = j;
			maze[i - 1][j] == '.';
			valid_positions.push(pos_verif);
		}

		if (!valid_positions.empty())
		{
			maze[pos_atual.i][pos_atual.j] = '.';
			pos_atual.j = valid_positions.top().j;
			pos_atual.i = valid_positions.top().i;
			if (maze[pos_atual.i][pos_atual.j] == '.')
			{
				valid_positions.pop();
			}
			else
			{
				if (maze[pos_atual.i][pos_atual.j] == 's')
				{
					finalizado = 1;
				}
				maze[pos_atual.i][pos_atual.j] = 'o';

				system("clear||cls");
				print_maze();
				valid_positions.pop();
			}
		}
		else
		{
			std::cout << "O labirinto não tem saída" << std::endl;
			return false;
		}
	}
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		if (!valid_positions.empty()) {
			pos_t next_position = valid_positions.top();
			valid_positions.pop();
		}
	return false;
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	thread t(walk, initial_pos);
	t.detach();

    while (!exit_found) {
        print_maze();
        system("clear");
    }

    print_maze();
	
    if (!exit_found) {
        cout << "A saída não foi encontrada!" << endl;
    } else {
        cout << "Saída encontrada!" << endl;
    }
	
	return 0;
}
