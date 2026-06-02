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

/* PROTOTIPOS */
void cadastrarVideo();
void cadastrarUsuario();
void listarVideos();
void listarUsuarios();
void buscarVideo();
void buscarUsuario();
void atualizarVideo();
void atualizarUsuario();
void excluirVideo();
void excluirUsuario();
void obterTituloVideo(int idVideo, char titulo[]);
void relatorioUsuariosFavoritos();

/* ========================= VIDEOS ========================= */

void cadastrarVideo() {
    FILE *arq = fopen("videos.dat", "ab");
    if (arq == NULL) return;

    Video v;

    printf("ID: ");
    scanf("%d", &v.id);
    getchar();

    printf("Titulo: ");
    fgets(v.titulo, sizeof(v.titulo), stdin);
    v.titulo[strcspn(v.titulo, "\n")] = '\0';

    printf("Categoria: ");
    fgets(v.categoria, sizeof(v.categoria), stdin);
    v.categoria[strcspn(v.categoria, "\n")] = '\0';

    printf("Duracao (min): ");
    scanf("%d", &v.duracao);

    fwrite(&v, sizeof(Video), 1, arq);
    fclose(arq);

    printf("Video cadastrado com sucesso!\n");
}

void listarVideos() {
    FILE *arq = fopen("videos.dat", "rb");

    if (arq == NULL) {
        printf("Nenhum video cadastrado.\n");
        return;
    }

    Video v;

    while (fread(&v, sizeof(Video), 1, arq)) {
        printf("\nID: %d\n", v.id);
        printf("Titulo: %s\n", v.titulo);
        printf("Categoria: %s\n", v.categoria);
        printf("Duracao: %d min\n", v.duracao);
    }

    fclose(arq);
}

void buscarVideo() {
    FILE *arq = fopen("videos.dat", "rb");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    int id;
    Video v;
    int encontrou = 0;

    printf("ID do video: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(Video), 1, arq)) {
        if (v.id == id) {
            printf("\nTitulo: %s\n", v.titulo);
            printf("Categoria: %s\n", v.categoria);
            printf("Duracao: %d\n", v.duracao);
            encontrou = 1;
            break;
        }
    }

    if (!encontrou)
        printf("Video nao encontrado.\n");

    fclose(arq);
}

void atualizarVideo() {
    FILE *arq = fopen("videos.dat", "r+b");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    int id;
    Video v;

    printf("ID do video: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(Video), 1, arq)) {

        if (v.id == id) {

            getchar();

            printf("Novo titulo: ");
            fgets(v.titulo, sizeof(v.titulo), stdin);
            v.titulo[strcspn(v.titulo, "\n")] = '\0';

            printf("Nova categoria: ");
            fgets(v.categoria, sizeof(v.categoria), stdin);
            v.categoria[strcspn(v.categoria, "\n")] = '\0';

            printf("Nova duracao: ");
            scanf("%d", &v.duracao);

            fseek(arq, -sizeof(Video), SEEK_CUR);
            fwrite(&v, sizeof(Video), 1, arq);

            fclose(arq);

            printf("Video atualizado!\n");
            return;
        }
    }

    fclose(arq);
    printf("Video nao encontrado.\n");
}

/* ========================= USUARIOS ========================= */

void cadastrarUsuario() {
    FILE *arq = fopen("usuarios.dat", "ab");
    if (arq == NULL) return;

    Usuario u;

    printf("ID: ");
    scanf("%d", &u.id);
    getchar();

    printf("Nome: ");
    fgets(u.nome, sizeof(u.nome), stdin);
    u.nome[strcspn(u.nome, "\n")] = '\0';

    do {
        printf("Quantidade de favoritos (0-%d): ", MAX_FAVORITOS);
        scanf("%d", &u.qtdFavoritos);
    } while (u.qtdFavoritos < 0 || u.qtdFavoritos > MAX_FAVORITOS);

    for (int i = 0; i < u.qtdFavoritos; i++) {
        printf("ID do video favorito %d: ", i + 1);
        scanf("%d", &u.favoritos[i]);
    }

    fwrite(&u, sizeof(Usuario), 1, arq);
    fclose(arq);

    printf("Usuario cadastrado com sucesso!\n");
}

void listarUsuarios() {
    FILE *arq = fopen("usuarios.dat", "rb");

    if (arq == NULL) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }

    Usuario u;

    while (fread(&u, sizeof(Usuario), 1, arq)) {

        printf("\nID: %d\n", u.id);
        printf("Nome: %s\n", u.nome);

        printf("Favoritos: ");
        for (int i = 0; i < u.qtdFavoritos; i++)
            printf("%d ", u.favoritos[i]);

        printf("\n");
    }

    fclose(arq);
}

void buscarUsuario() {
    FILE *arq = fopen("usuarios.dat", "rb");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    Usuario u;
    int id, encontrou = 0;

    printf("ID do usuario: ");
    scanf("%d", &id);

    while (fread(&u, sizeof(Usuario), 1, arq)) {
        if (u.id == id) {

            printf("\nID: %d\n", u.id);
            printf("Nome: %s\n", u.nome);

            encontrou = 1;
            break;
        }
    }

    fclose(arq);

    if (!encontrou)
        printf("Usuario nao encontrado.\n");
}

void atualizarUsuario() {
    FILE *arq = fopen("usuarios.dat", "r+b");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    Usuario u;
    int id;

    printf("ID do usuario: ");
    scanf("%d", &id);

    while (fread(&u, sizeof(Usuario), 1, arq)) {

        if (u.id == id) {

            getchar();

            printf("Novo nome: ");
            fgets(u.nome, sizeof(u.nome), stdin);
            u.nome[strcspn(u.nome, "\n")] = '\0';

            do {
                printf("Quantidade favoritos (0-%d): ", MAX_FAVORITOS);
                scanf("%d", &u.qtdFavoritos);
            } while (u.qtdFavoritos < 0 || u.qtdFavoritos > MAX_FAVORITOS);

            for (int i = 0; i < u.qtdFavoritos; i++) {
                printf("ID video favorito %d: ", i + 1);
                scanf("%d", &u.favoritos[i]);
            }

            fseek(arq, -sizeof(Usuario), SEEK_CUR);
            fwrite(&u, sizeof(Usuario), 1, arq);

            fclose(arq);

            printf("Usuario atualizado!\n");
            return;
        }
    }

    fclose(arq);
    printf("Usuario nao encontrado.\n");
}

/* ========================= EXCLUSAO ========================= */

void excluirVideo() {
    FILE *orig = fopen("videos.dat", "rb");
    if (orig == NULL) return;

    FILE *novo = fopen("temp.dat", "wb");

    Video v;
    int id;

    printf("ID do video: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(Video), 1, orig)) {
        if (v.id != id)
            fwrite(&v, sizeof(Video), 1, novo);
    }

    fclose(orig);
    fclose(novo);

    remove("videos.dat");
    rename("temp.dat", "videos.dat");

    printf("Video removido.\n");
}

void excluirUsuario() {
    FILE *orig = fopen("usuarios.dat", "rb");
    if (orig == NULL) return;

    FILE *novo = fopen("temp.dat", "wb");

    Usuario u;
    int id;

    printf("ID do usuario: ");
    scanf("%d", &id);

    while (fread(&u, sizeof(Usuario), 1, orig)) {
        if (u.id != id)
            fwrite(&u, sizeof(Usuario), 1, novo);
    }

    fclose(orig);
    fclose(novo);

    remove("usuarios.dat");
    rename("temp.dat", "usuarios.dat");

    printf("Usuario removido.\n");
}

/* ========================= RELATORIO ========================= */

void obterTituloVideo(int idVideo, char titulo[]) {
    FILE *arq = fopen("videos.dat", "rb");

    strcpy(titulo, "Nao encontrado");

    if (arq == NULL)
        return;

    Video v;

    while (fread(&v, sizeof(Video), 1, arq)) {
        if (v.id == idVideo) {
            strcpy(titulo, v.titulo);
            break;
        }
    }

    fclose(arq);
}

void relatorioUsuariosFavoritos() {
    FILE *arq = fopen("usuarios.dat", "rb");

    if (arq == NULL) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }

    Usuario u;
    char titulo[100];

    printf("\n===== RELATORIO =====\n");

    while (fread(&u, sizeof(Usuario), 1, arq)) {

        printf("\nUsuario: %s\n", u.nome);
        printf("Videos Favoritados:\n");

        for (int i = 0; i < u.qtdFavoritos; i++) {
            obterTituloVideo(u.favoritos[i], titulo);
            printf("- %s\n", titulo);
        }
    }

    fclose(arq);
}

/* ========================= MAIN ========================= */

int main() {

    int opcao;

    do {

        printf("\n===== STREAMING =====\n");
        printf("1 - Cadastrar Video\n");
        printf("2 - Cadastrar Usuario\n");
        printf("3 - Listar Videos\n");
        printf("4 - Listar Usuarios\n");
        printf("5 - Buscar Video\n");
        printf("6 - Buscar Usuario\n");
        printf("7 - Atualizar Video\n");
        printf("8 - Atualizar Usuario\n");
        printf("9 - Excluir Video\n");
        printf("10 - Excluir Usuario\n");
        printf("11 - Relatorio\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        scanf("%d", &opcao);

        switch(opcao) {
            case 1: cadastrarVideo(); break;
            case 2: cadastrarUsuario(); break;
            case 3: listarVideos(); break;
            case 4: listarUsuarios(); break;
            case 5: buscarVideo(); break;
            case 6: buscarUsuario(); break;
            case 7: atualizarVideo(); break;
            case 8: atualizarUsuario(); break;
            case 9: excluirVideo(); break;
            case 10: excluirUsuario(); break;
            case 11: relatorioUsuariosFavoritos(); break;
        }

    } while(opcao != 0);

    return 0;
}