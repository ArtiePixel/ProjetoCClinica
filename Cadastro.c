#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


// struct paciente
typedef struct {
    char nome[50];
    char inscricao[21];
    int idade;
} Paciente;

void cadastrarPacientes(Paciente *pacientes, int quantidade){
    mkdir("./pacientes", 0777);

    for (int i = 0; i < quantidade; i++) {

        printf("\nCadastro do Paciente %d:\n", i+1);
        printf("Nome: ");
        scanf("%49s", pacientes[i].nome);
        printf("Número de Inscrição: ");
        scanf("%20s", pacientes[i].inscricao);
        printf("Idade: ");
        scanf("%d", &pacientes[i].idade);

        char nomePasta[300];
        snprintf(nomePasta, sizeof(nomePasta)*2, "./pacientes/%s", pacientes[i].nome);
        mkdir(nomePasta, 0777);


        char nomeArquivo[300];
        snprintf(nomeArquivo, sizeof(nomeArquivo)*5, "%s/%s.txt", nomePasta, pacientes[i].nome);

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
        printf("Arquivo criado com sucesso em %s!\n", nomeArquivo);
    }
}

int main() {
    int quantidade, continuar = 0;

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