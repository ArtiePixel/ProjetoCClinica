#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// struct paciente
typedef struct {
    char nome[50];
    char inscricao[11];
    int idade;
} Paciente;

//diretiva para mudança de função a depender do sistema operacional
#ifdef _WIN32
    #define criarDiretorio _mkdir
#else
    #define criarDiretorio mkdir

#endif

void cadastrarPacientes(Paciente *pacientes, int quantidade) {
    criarDiretorio("./pacientes", 0777);

    for (int i = 0; i < quantidade; i++) {

        printf("\nCadastro do Paciente %d:\n", i+1);
        printf("Nome: ");
        scanf("%49s", pacientes[i].nome);
        printf("Número de Inscrição (10 digitos): ");
        scanf("%10s", pacientes[i].inscricao);
        printf("Idade: ");
        scanf("%d", &pacientes[i].idade);

        char nomePasta[200];
        snprintf(nomePasta, sizeof(nomePasta), "./pacientes/%s", (pacientes[i].nome));
        criarDiretorio(nomePasta, 0777);

        char nomeArquivo[300];
        snprintf(nomeArquivo, sizeof(nomeArquivo), "%s/%s.txt", nomePasta, pacientes[i].nome);

        
        FILE *arquivo = fopen(nomeArquivo, "w");
        if (arquivo == NULL) {
            printf("Erro ao criar arquivo para %s!\n", pacientes[i].nome);
            continue;
        }

        // Escreve os dados no arquivo
        fprintf(arquivo, "nome: %s, inscricao: %s, idade: %d", 
                pacientes[i].nome, 
                pacientes[i].inscricao, 
                pacientes[i].idade);

        // Fecha o arquivo
        fclose(arquivo);
        printf("Arquivo %s criado com sucesso!\n", nomeArquivo);
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