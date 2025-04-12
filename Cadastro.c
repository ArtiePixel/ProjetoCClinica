#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct paciente
typedef struct {
    char nome[50];
    char inscricao[21];
    int idade;
} Paciente;

void cadastrarPacientes(Paciente *pacientes, int quantidade){
    for (int i = 0; i < quantidade; i++) {

        printf("\nCadastro do Paciente %d:\n", i+1);
        printf("Nome: ");
        scanf("%49s", pacientes[i].nome);
        printf("Número de Inscrição: ");
        scanf("%20s", pacientes[i].inscricao);
        printf("Idade: ");
        scanf("%d", &pacientes[i].idade);

        char nomeArquivo[60];
        snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.txt", pacientes[i].nome);

        FILE *arquivo = fopen(nomeArquivo, "w");
        if (arquivo == NULL) {
            printf("Erro ao criar arquivo para %s!\n", pacientes[i].nome);
            continue;
        }
        
        fprintf(arquivo, "nome: %s, inscricao: %s, idade: %d", 
                pacientes[i].nome, 
                pacientes[i].inscricao, 
                pacientes[i].idade);

        fclose(arquivo);
        printf("Arquivo %s criado com sucesso!\n", nomeArquivo);
    }
}

int main() {
    int quantidade;

    printf("Quantos pacientes deseja cadastrar? ");
    scanf("%d", &quantidade);

    Paciente *pacientes = (Paciente *)malloc(quantidade * sizeof(Paciente));
    if (pacientes == NULL) {
        printf("Erro na alocação de memória!\n");
        return 1;
    }

    cadastrarPacientes(pacientes, quantidade);

    free(pacientes);

    return 0;
}