#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FAVORITOS 10

typedef struct {
    int id;
    char titulo[100];
    char categoria[50];
    int duracao;
} Video;

typedef struct {
    int id;
    char nome[100];
    int favoritos[MAX_FAVORITOS];
    int qtdFavoritos;
} Usuario;

void cadastrarVideo();
void cadastrarUsuario();
void listarVideos();
void listarUsuarios();
void buscarVideo();
void atualizarVideo();
void atualizarUsuario();
void excluirVideo();
void excluirUsuario();
void obterTituloVideo(int idVideo, char titulo[]);
void relatorioUsuariosFavoritos();

int main() {

    int opcao;

    do {

        printf("\n===== STREAMING =====\n");
        printf("1 - Cadastrar Video\n");
        printf("2 - Cadastrar Usuario\n");
        printf("3 - Listar Videos\n");
        printf("4 - Listar Usuarios\n");
        printf("5 - Buscar Video\n");
        printf("6 - Atualizar Video\n");
        printf("7 - Atualizar Usuario\n");
        printf("8 - Excluir Video\n");
        printf("9 - Excluir Usuario\n");
        printf("10 - Relatorio Usuarios x Favoritos\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        scanf("%d",&opcao);

        switch(opcao) {

            case 1: cadastrarVideo(); break;
            case 2: cadastrarUsuario(); break;
            case 3: listarVideos(); break;
            case 4: listarUsuarios(); break;
            case 5: buscarVideo(); break;
            case 6: atualizarVideo(); break;
            case 7: atualizarUsuario(); break;
            case 8: excluirVideo(); break;
            case 9: excluirUsuario(); break;
            case 10: relatorioUsuariosFavoritos(); break;
        }

    } while(opcao != 0);

    return 0;
}

void cadastrarVideo() {
    FILE *arq = fopen("videos.dat", "ab");

    Video v;

    printf("ID: ");
    scanf("%d", &v.id);

    getchar();

    printf("Titulo: ");
    fgets(v.titulo, 100, stdin);
    v.titulo[strcspn(v.titulo, "\n")] = '\0';

    printf("Categoria: ");
    fgets(v.categoria, 50, stdin);
    v.categoria[strcspn(v.categoria, "\n")] = '\0';

    printf("Duracao (min): ");
    scanf("%d", &v.duracao);

    fwrite(&v, sizeof(Video), 1, arq);

    fclose(arq);

    printf("Video cadastrado!\n");
}

void cadastrarUsuario() {
    FILE *arq = fopen("usuarios.dat", "ab");

    Usuario u;

    printf("ID: ");
    scanf("%d", &u.id);

    getchar();

    printf("Nome: ");
    fgets(u.nome, 100, stdin);
    u.nome[strcspn(u.nome, "\n")] = '\0';

    printf("Quantidade de favoritos (max %d): ", MAX_FAVORITOS);
    scanf("%d", &u.qtdFavoritos);

    for(int i=0; i<u.qtdFavoritos; i++) {
        printf("ID do video favorito %d: ", i+1);
        scanf("%d", &u.favoritos[i]);
    }

    fwrite(&u, sizeof(Usuario), 1, arq);

    fclose(arq);

    printf("Usuario cadastrado!\n");
}

void listarVideos() {
    FILE *arq = fopen("videos.dat", "rb");

    Video v;

    printf("\n=== VIDEOS ===\n");

    while(fread(&v, sizeof(Video), 1, arq)) {
        printf("ID: %d\n", v.id);
        printf("Titulo: %s\n", v.titulo);
        printf("Categoria: %s\n", v.categoria);
        printf("Duracao: %d min\n\n", v.duracao);
    }

    fclose(arq);
}

void listarUsuarios() {
    FILE *arq = fopen("usuarios.dat", "rb");

    Usuario u;

    printf("\n=== USUARIOS ===\n");

    while(fread(&u, sizeof(Usuario), 1, arq)) {

        printf("ID: %d\n", u.id);
        printf("Nome: %s\n", u.nome);

        printf("Favoritos: ");

        for(int i=0; i<u.qtdFavoritos; i++) {
            printf("%d ", u.favoritos[i]);
        }

        printf("\n\n");
    }

    fclose(arq);
}

void buscarVideo() {
    FILE *arq = fopen("videos.dat", "rb");

    int id;
    Video v;
    int encontrou = 0;

    printf("ID do video: ");
    scanf("%d", &id);

    while(fread(&v, sizeof(Video), 1, arq)) {
        if(v.id == id) {

            printf("\nTitulo: %s\n", v.titulo);
            printf("Categoria: %s\n", v.categoria);
            printf("Duracao: %d\n", v.duracao);

            encontrou = 1;
            break;
        }
    }

    if(!encontrou)
        printf("Video nao encontrado!\n");

    fclose(arq);
}

void buscarVideo() {
    FILE *arq = fopen("videos.dat", "rb");

    int id;
    Video v;
    int encontrou = 0;

    printf("ID do video: ");
    scanf("%d", &id);

    while(fread(&v, sizeof(Video), 1, arq)) {
        if(v.id == id) {

            printf("\nTitulo: %s\n", v.titulo);
            printf("Categoria: %s\n", v.categoria);
            printf("Duracao: %d\n", v.duracao);

            encontrou = 1;
            break;
        }
    }

    if(!encontrou)
        printf("Video nao encontrado!\n");

    fclose(arq);
}

void atualizarUsuario() {

    FILE *arq = fopen("usuarios.dat", "r+b");

    Usuario u;
    int id;

    printf("ID do usuario: ");
    scanf("%d", &id);

    while(fread(&u, sizeof(Usuario), 1, arq)) {

        if(u.id == id) {

            getchar();

            printf("Novo nome: ");
            fgets(u.nome, 100, stdin);
            u.nome[strcspn(u.nome,"\n")] = '\0';

            printf("Quantidade favoritos: ");
            scanf("%d", &u.qtdFavoritos);

            for(int i=0;i<u.qtdFavoritos;i++) {
                printf("ID video favorito %d: ", i+1);
                scanf("%d",&u.favoritos[i]);
            }

            fseek(arq,-sizeof(Usuario),SEEK_CUR);

            fwrite(&u,sizeof(Usuario),1,arq);

            printf("Atualizado!\n");

            fclose(arq);
            return;
        }
    }

    printf("Usuario nao encontrado!\n");

    fclose(arq);
}

void excluirVideo() {

    FILE *orig = fopen("videos.dat", "rb");
    FILE *novo = fopen("temp.dat", "wb");

    int id;
    Video v;

    printf("ID do video a excluir: ");
    scanf("%d", &id);

    while(fread(&v,sizeof(Video),1,orig)) {

        if(v.id != id)
            fwrite(&v,sizeof(Video),1,novo);
    }

    fclose(orig);
    fclose(novo);

    remove("videos.dat");
    rename("temp.dat","videos.dat");

    printf("Video removido!\n");
}

void excluirUsuario() {

    FILE *orig = fopen("usuarios.dat", "rb");
    FILE *novo = fopen("temp.dat", "wb");

    int id;
    Usuario u;

    printf("ID do usuario a excluir: ");
    scanf("%d", &id);

    while(fread(&u,sizeof(Usuario),1,orig)) {

        if(u.id != id)
            fwrite(&u,sizeof(Usuario),1,novo);
    }

    fclose(orig);
    fclose(novo);

    remove("usuarios.dat");
    rename("temp.dat","usuarios.dat");

    printf("Usuario removido!\n");
}

void obterTituloVideo(int idVideo, char titulo[]) {

    FILE *arq = fopen("videos.dat","rb");

    Video v;

    strcpy(titulo, "Nao encontrado");

    while(fread(&v,sizeof(Video),1,arq)) {

        if(v.id == idVideo) {

            strcpy(titulo,v.titulo);
            break;
        }
    }

    fclose(arq);
}

void relatorioUsuariosFavoritos() {

    FILE *arq = fopen("usuarios.dat","rb");

    Usuario u;
    char titulo[100];

    printf("\n===== RELATORIO =====\n");

    while(fread(&u,sizeof(Usuario),1,arq)) {

        printf("\nUsuario: %s\n",u.nome);

        printf("Videos Favoritados:\n");

        for(int i=0;i<u.qtdFavoritos;i++) {

            obterTituloVideo(u.favoritos[i], titulo);

            printf("- %s\n", titulo);
        }

        printf("--------------------\n");
    }

    fclose(arq);
}