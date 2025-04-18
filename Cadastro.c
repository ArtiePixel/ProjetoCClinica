#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>

typedef struct {
    char nome[50];
    char inscricao[11];
    char procedimento[100];
    int idade;
} Paciente;

//funcao decidida a partir de sistema operacional especifico
#ifdef _WIN32
    #define criarDiretorio _mkdir
#else
    #define criarDiretorio mkdir
#endif

// funcao para substituir espaços por underlines
void removerespacosNomePasta(char *nome) {
    for (int i = 0; nome[i]; i++) {
        if (isspace(nome[i])) {
            nome[i] = '_';
        }
    }
}

void cadastrarPacientes(Paciente *pacientes, int quantidade) {
    criarDiretorio("./pacientes", 0777);

    for (int i = 0; i < quantidade; i++) {
        printf("\nCadastro do Paciente %d:\n", i+1);
        
        printf("Nome: ");
        fgets(pacientes[i].nome, sizeof(pacientes[i].nome), stdin);
        pacientes[i].nome[strcspn(pacientes[i].nome, "\n")] = '\0'; 
        
        printf("Número de Inscrição (10 digitos): ");
        fgets(pacientes[i].inscricao, sizeof(pacientes[i].inscricao), stdin);
        pacientes[i].inscricao[strcspn(pacientes[i].inscricao, "\n")] = '\0';

        printf("Procedimento a ser realizado: ");
        fgets(pacientes[i].procedimento, sizeof(pacientes[i].procedimento), stdin);
        pacientes[i].procedimento[strcspn(pacientes[i].procedimento, "\n")] = '\0';
        
        printf("Idade: ");
        scanf("%d", &pacientes[i].idade);
        while (getchar() != '\n'); 

        char nomeAlterado[50];
        strcpy(nomeAlterado, pacientes[i].nome);
        removerespacosNomePasta(nomeAlterado);

        char nomePasta[200];
        snprintf(nomePasta, sizeof(nomePasta), "./pacientes/%s", nomeAlterado);
        criarDiretorio(nomePasta, 0777);

        char nomeArquivo[300];
        snprintf(nomeArquivo, sizeof(nomeArquivo), "%s/dados.txt", nomePasta);

        FILE *arquivo = fopen(nomeArquivo, "w");
        if (arquivo == NULL) {
            printf("Erro ao criar arquivo para %s!\n", pacientes[i].nome);
            continue;
        }
        
        fprintf(arquivo, "nome: %s, inscricao: %s, idade: %d, procedimento: %s", 
                pacientes[i].nome,  
                pacientes[i].inscricao, 
                pacientes[i].idade,
                pacientes[i].procedimento);

        fclose(arquivo);
        printf("Arquivo criado com sucesso em %s!\n", nomeArquivo);
    }
}

int main() {
    int quantidade;

    printf("Quantos pacientes deseja cadastrar? ");
    scanf("%d", &quantidade);
    while (getchar() != '\n'); // limpeza do buffer

    Paciente *pacientes = (Paciente *)malloc(quantidade * sizeof(Paciente));
    if (pacientes == NULL) {
        printf("Erro na alocação de memória!\n");
        return 1;
    }

    cadastrarPacientes(pacientes, quantidade);

    free(pacientes);
    return 0;
}