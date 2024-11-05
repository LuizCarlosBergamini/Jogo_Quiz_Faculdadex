#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <conio.h>

#define TAM 100
#define MAX_QUESTOES 50

#define GREEN   "\033[38;2;255;255;255;1m\033[48;2;106;170;100;1m"
#define RED     "\033[38;2;255;255;255;1m\033[48;2;220;20;60;1m"
#define RESET   "\033[0m"
#define TIME_LIMIT 10 // segundos

typedef struct{
    char pergunta[TAM];
    char resp1[TAM];
    char resp2[TAM];
    char resp3[TAM];
    int correta;
}tQuestao;

void limparQuestao(tQuestao *q)
{
    for(int i=0; i<TAM; i++)
    {
        q->pergunta[i]='\0';
        q->resp1[i]='\0';
        q->resp2[i]='\0';
        q->resp3[i]='\0';
    }
    q->correta = 0;
}

void gravarQuestaoNoArquivo(tQuestao q)
{
    FILE *arq;

    arq = fopen("bancoquestoes.qz", "ab");

    if(arq == NULL)
    {
        printf("\n\nERRO - Não foi possível abrir o arquivo.");
    }
    else
    {
        fwrite(&q, sizeof(tQuestao),1, arq);

        fclose(arq);

        printf("gravado");
    }
}

void configurarQuestao()
{
    tQuestao nova_questao;

    limparQuestao(&nova_questao);

    printf("\nInserindo NOVA questão no jogo...\n");

    printf("\n\nDigite a pergunta: ");
    fflush(stdin);
    fgets(nova_questao.pergunta, TAM, stdin);

    printf("\n\nDigite a 1a opção de resposta: ");
    fflush(stdin);
    fgets(nova_questao.resp1, TAM, stdin);

    printf("\n\nDigite a 2a opção de resposta: ");
    fflush(stdin);
    fgets(nova_questao.resp2, TAM, stdin);

    printf("\n\nDigite a 3a opção de resposta: ");
    fflush(stdin);
    fgets(nova_questao.resp3, TAM, stdin);

    do{
        printf("\n\nQual das respostas é a correta (1, 2 ou 3): ");
        scanf("%d", &nova_questao.correta);

    }while(!((nova_questao.correta == 1) || (nova_questao.correta == 2) || (nova_questao.correta == 3)));

    gravarQuestaoNoArquivo(nova_questao);
}

void configurar()
{
    char opcaoConfig;

    while(!(opcaoConfig=='B' || opcaoConfig=='b'))
    {
        system("cls");
        printf("\nConfigurando o Jogo...");

        printf("\nA) Configurar uma nova questão no banco de questões.");
        printf("\nB) Voltar para o menu principal.");

        printf("\nEscolha uma das opções acima: ");
        fflush(stdin);
        scanf("%c", &opcaoConfig);

        switch (opcaoConfig)
        {
            case 'a':
            case 'A':
                configurarQuestao();
                break;
            case 'b':
            case 'B':
                break;
            default:
                printf("\n\nOpção inválida.");
        }
        fflush(stdin);
        getchar();
    }
}

void carregarQuestoes(tQuestao *vet, int *qtdVet)
{
    FILE * arq;
    *qtdVet = 0;

    arq = fopen("bancoquestoes.qz", "rb");

    if(arq==NULL)
    {
        printf("\n\nERRO - Não foi possível abrir o arquivo.");
    }
    else
    {
        fread(&vet[*qtdVet], sizeof(tQuestao), 1, arq);

        while(!feof(arq) && (*qtdVet)<MAX_QUESTOES )
        {
            (*qtdVet)++;
            fread(&vet[*qtdVet], sizeof(tQuestao), 1, arq);
        }

        fclose(arq);
    }

}

int responderQuestao(tQuestao q)
{
    int acertou = 0, resp;
    time_t start, end;
    double elapsed;
    int remaining_time = TIME_LIMIT;

    printf("\n%s", q.pergunta);
    printf("\n1) %s", q.resp1);
    printf("\n2) %s", q.resp2);
    printf("\n3) %s\n", q.resp3);

    while (remaining_time > 0) {
        printf("\rTempo restante: %d segundos     ", remaining_time);
        fflush(stdout);
        sleep(1);
        remaining_time--;

        // Essa parte foi sugestão do ChatGPT, com algumas alterações minhas, pois não sabia como fazer o scanf não parar o tempo, fazendo assim com que o timer não travasse pro usuário
        // Check if the user has already provided an answer using kbhit
        if (_kbhit()) {
            char ch = _getch();  // Read character without waiting for Enter

            if (ch == '1' || ch == '2' || ch == '3') {
                resp = ch - '0';  // Convert char to int (e.g., '1' to 1)
                printf("\nResposta recebida: %d\n", resp);
                break;
            }
        }
    }

    printf("\r                              \r");
    printf("\n");

    if (remaining_time == 0) {
        printf(RED "\nTempo esgotado! A resposta foi registrada como incorreta." RESET "\n");
        return 0;
    }

    if (resp == q.correta) {
        acertou = 1;
        printf(GREEN "\nParabéns! Você acertou a questão!" RESET "\n");
    } else {
        printf(RED "\nSinto muito.... você errou a questão." RESET "\n");
    }

    return acertou;
}

void jogar()
{
    tQuestao questoes[MAX_QUESTOES];
    int qtdQuestoesConfig = 0;
    int nroAcertos = 0;

    printf("\nApenas clique o valor da resposta desejada,\nnão é necessário o uso da tecla enter ao entrar com a resposta!\n");
    printf("\nJogando...");

    carregarQuestoes(questoes, &qtdQuestoesConfig);

    for(int i=0; i<qtdQuestoesConfig; i++)
    {
        if(responderQuestao(questoes[i]))
        {
            nroAcertos++;
        }
    }

    printf("\n\n\nFIM DE JOGO! - Seu percentual de acertos = %.1f%%", ((float)nroAcertos/(float)qtdQuestoesConfig)*100.0f);

    fflush(stdin);
    getchar();
}

void menu()
{
    int opcao=0;

    while(opcao!=3)
    {
        system("cls");
        printf("\n\n*************** JOGO QUIZ ***************\n\n");

        printf("1) Configurar questões\n");
        printf("2) Jogar\n");
        printf("3) Sair\n");

        printf("\n\nEscolha uma das opções acima: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                configurar();
                break;
            case 2:
                jogar();
                break;
            case 3:
                printf("\nAté mais! Volte novamente quando puder...");
                break;
            default:
                printf("\nOpção Inválida!\n");
        }
    }


}

int main()
{
    setlocale(LC_ALL, "portuguese");

    menu();

    return 0;
}
