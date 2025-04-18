#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


// struct paciente
typedef struct {
    char nome[50];
    char inscricao[21];
    int idade;
} Paciente;

//diretiva para mudança de função a depender do sistema operacional
#ifdef _WIN32
    #include <direct.h>
    #define criarDiretorio(path, mode) _mkdir(path)
#else
    #define criarDiretorio(path, mode) mkdir(path, mode)

#endif

void listarPacientes();

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
    listarPacientes();
    return 0;
}
void listarPacientes() {
    DIR *dir;
    struct dirent *entrada;

    printf("\n--- LISTA DE PACIENTES ---\n");

    if ((dir = opendir("./pacientes")) == NULL) {
        printf("Nenhum paciente cadastrado!\n");
        return;
    }

    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        char caminho[300];
        snprintf(caminho, sizeof(caminho), "./pacientes/%s/%s.txt", entrada->d_name, entrada->d_name);
        
        FILE *arquivo = fopen(caminho, "r");
        if (arquivo) {
            char linha[256];
            printf("\n--- Dados do Paciente ---\n");
            while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                printf("%s", linha);
            }
            fclose(arquivo);
        }
    }
    closedir(dir);
}
