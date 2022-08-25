#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <Windows.h>
#include <time.h>

struct Data{
    int dia;
    int mes;
    int ano;
};
struct Usuario{
    long long int cpf;
    char nome[31];
    int codEmpre=-1;
    Data dataEmpre;
    Data dataDev;
};
struct Livro{
    int codigo;
    char titulo[32];
    char autor[31];
    int ano;
    int empre=-1;
};

void data_atual(int &dia, int &mes, int &ano){
    time_t t = time(NULL);
    struct tm lt = *localtime(&t);
    ano = lt.tm_year + 1900;
    mes = lt.tm_mon + 1;
    dia = lt.tm_mday;
}

void data_devolucao(int &dia, int &mes, int &ano){
    bool bissexto=false;
    dia+=7;
    if ((ano % 4==0 && ano % 100!=0) || (ano % 400==0))
        bissexto=true;
    if((mes==1 || mes==3 || mes==5 || mes==7 || mes==8 || mes==10) && dia > 31)
    {
        mes+=1;
        dia-=31;
    }
    else if((mes==4 || mes==6 || mes==9 || mes==11) && dia > 30)
    {
        mes+=1;
        dia-=30;
    }
    else if(mes==12 && dia > 31)
    {
        ano+=1;
        mes=1;
        dia-=31;
    }
    else if(mes==2 && bissexto==true && dia > 29)
    {
        mes+=1;
        dia-=29;
    }
    else if(mes==2 && bissexto==false && dia > 28)
    {
        mes+=1;
        dia-=28;
    }
}
bool eh_bissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}
int numero_dias_mes(int mes, int ano) {
    int dias[] = { 31, eh_bissexto(ano) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    return mes >= 1 && mes <= 12 && ano > 0 ? dias[mes-1] : -1;
}
int diferenca(Data dt_ini, Data dt_fim) {
    int dias;

    if (dt_ini.mes == dt_fim.mes && dt_fim.ano == dt_fim.ano)
        dias = dt_fim.dia - dt_ini.dia;
    else if (dt_fim.ano == dt_fim.ano) {
        // At� o fim do m�s inicial
        dias = numero_dias_mes(dt_ini.mes, dt_ini.ano) - dt_ini.dia;

        for (int mes = dt_ini.mes+1; mes < dt_fim.mes; mes++)
            dias += numero_dias_mes(mes, dt_ini.ano);

        // At� o dia do m�s final
        dias += dt_fim.dia;
    }
    else {
        dias = numero_dias_mes(dt_ini.mes, dt_ini.ano) - dt_ini.dia;

        for (int mes = dt_ini.mes+1; mes <= 12; mes++)
            dias += numero_dias_mes(mes, dt_ini.ano);

        // Do ano inicial+1 at� ano final-1
        for (int ano = dt_ini.ano+1; ano < dt_fim.ano; ano++)
            dias += eh_bissexto(ano) ? 366 : 365;

        // In�cio do ano at� m�s final-1
        for (int mes = 1; mes < dt_fim.mes; mes++)
            dias += numero_dias_mes(mes, dt_fim.ano);

        dias += dt_fim.dia;
    }

    return dias;
}
bool testar_data(char data[], int &dias, Usuario user)
{
    Data dataColocada;
    bool bissexto=false;
    char diaC[3], mesC[3], anoC[5];
    int diaA, mesA, anoA;
    int diasF, mesesF, anosF;
    if(strlen(data)>8)
        return false;
    for(int i = 0; i<8; i++)
    {
        if(data[i]!='0' && data[i]!='1' && data[i]!='2' && data[i]!='3' && data[i]!='4' && data[i]!='5' && data[i]!='6' && data[i]!='7' && data[i]!='8' && data[i]!='9')
            return false;
        if(i==1)
        {
            diaC[0]=data[0];
            diaC[1]=data[1];
            diaC[2]='\0';
            dataColocada.dia = atoi(diaC);
        }
        if(i==3)
        {
            mesC[0]=data[2];
            mesC[1]=data[3];
            mesC[2]='\0';
            dataColocada.mes = atoi(mesC);
        }
        if(i==7)
        {
            anoC[0]=data[4];
            anoC[1]=data[5];
            anoC[2]=data[6];
            anoC[3]=data[7];
            anoC[4]='\0';
            dataColocada.ano = atoi(anoC);
        }
    }
    data_atual(diaA, mesA, anoA);
    if ((dataColocada.ano % 4==0 && dataColocada.ano % 100!=0) || (dataColocada.ano % 400==0))
        bissexto=true;
    if(dataColocada.mes>12 || dataColocada.mes<1 || ((dataColocada.mes==1 || dataColocada.mes==3 || dataColocada.mes==5 || dataColocada.mes==7 || dataColocada.mes==8 || dataColocada.mes==10) && dataColocada.dia>31) || ((dataColocada.mes==4 || dataColocada.mes==6 || dataColocada.mes==9 || dataColocada.mes==11) && dataColocada.dia>30) || (dataColocada.mes==2 && bissexto==true && dataColocada.dia > 29) || (dataColocada.mes==2 && bissexto==false && dataColocada.dia > 28))
        return false;
    if((dataColocada.ano==anoA && dataColocada.mes == mesA && dataColocada.dia > diaA) || (dataColocada.ano==anoA && dataColocada.mes>mesA) || (dataColocada.ano>anoA))
        return false;
    if((dataColocada.ano==user.dataEmpre.ano && dataColocada.mes==user.dataEmpre.mes && dataColocada.dia<user.dataEmpre.dia) || (dataColocada.ano==user.dataEmpre.ano && dataColocada.mes<user.dataEmpre.mes) || (dataColocada.ano<user.dataEmpre.ano))
        return false;
    
    
    dias = diferenca(user.dataDev, dataColocada);

    return true;
}

void imprimeNomeFormatado(char nome[], int tam)
{
    int falta;
    bool continua=true;
    for(int i = 0; i < tam && continua; i++)
    {
        if(nome[i]!='\0')
            printf("%c",nome[i]);
        else
            continua=false;
    }
        
    if(strlen(nome)<tam)
    {
        falta = tam-strlen(nome);
        for(int i = 0; i < falta; i++)
            printf(" ");
    }
}

void bubble_sort_use(Usuario v[], int n) {
    bool trocou = true;
    Usuario aux;
    for (int k = n; k > 0 && trocou; k--)
    {
        trocou = false;
        for (int i = 0; i < k; i++)
            if (v[i].cpf > v[i+1].cpf)
            {
                aux = v[i+1];
                v[i+1] = v[i];
                v[i] = aux;
                trocou = true;
            }
    }
}

void cadUser(Usuario User[], int &contuser)
{
    bool certo,maior;
    char aux[1000];
    do{
        certo=true;
        printf("CPF: ");
        scanf("%lld",&User[contuser].cpf);
        getchar();
        if(contuser>0)
            for(int i = 0; i < contuser; i++)
                if(User[i].cpf==User[contuser].cpf)
                {
                    certo=false;
                    printf("\nErro: CPF ja cadastrado!\n(Digite x para voltar ao menu)\n");
                    if(getchar()=='x')
                    {
                        system("cls");
                        return;
                    }
                    system("cls");
                }
        if(User[contuser].cpf<10000000000 || User[contuser].cpf>99999999999)
        {
            printf("\nErro: CPF invalido\n");
            getchar();
            system("cls");
        }
            
    }while (User[contuser].cpf<10000000000 || User[contuser].cpf>99999999999 || certo==false);
    do{
        certo=false;
        maior=true;
        printf("Nome: ");
        gets(aux);
        strupr(aux);
        fflush(stdin);
        for(int i=0;i<31;i++)
        {
            if(i<4 && aux[i]=='\0')
            {
                printf("\nErro: Nome precisa ter pelo menos 4 digitos\n");
                getchar();
                system("cls");
                printf("CPF: %lld\n",User[contuser].cpf);
                maior=false;
                break;
            }
            if(i>3 && aux[i]=='\0')
            {
                certo=true;
                maior=false;
                break;
            }
        }
        if(maior==true)
        {
            printf("\nErro: Nome precisa ter menos de 30 digitos\n");
            getchar();
            system("cls");
            printf("CPF: %lld\n",User[contuser].cpf);
        }
    }while(certo==false);
    strcpy(User[contuser].nome, aux);
    if(contuser>0)
        bubble_sort_use(User, contuser);
    contuser=contuser+1;
    printf("\nCadastro realizado com sucesso!\n");
    system("pause");
    system("cls");
}
void exclUser(Usuario User[], int &contuser)
{
    long long int cpf;
    int cont;
    bool certo;
    do{
        cont=0;
        certo=false;
        do{
            printf("CPF: ");
            scanf("%lld",&cpf);
            getchar();
            if(cpf<10000000000 || cpf>99999999999)
            {
                printf("Erro: CPF invalido\n");
                getchar();
                system("cls");
            }
        }while(cpf<10000000000 || cpf>99999999999);
        
        for(int i = 0; i< contuser; i++)
        {
            if(User[i].cpf == cpf && User[i].codEmpre == -1)
            {
                certo=true;
                User[i].cpf = 999999999999;
                cont++;
                break;
            }
            else if(User[i].cpf == cpf && User[i].codEmpre != -1)
            {
                printf("\nErro: usuario possui um emprestimo!\n(Digite x para voltar ao menu)\n");
                if(getchar()=='x')
                {
                    system("cls");
                    return;
                }
                system("cls");
                cont++;
                break;
            }
        }
        if(cont==0)
        {
            printf("\nErro: usuario nao cadastrado!\n(Digite x para voltar ao menu)\n");
            if(getchar()=='x')
            {
                system("cls");
                return;
            }
            system("cls");
        }
    }while(certo==false);
    contuser--;
    bubble_sort_use(User, contuser);
    
    printf("\nExclusao realizada com sucesso!\n");
    system("pause");
    system("cls");
}
void listUser(Usuario User[], Livro livro[], int contuser, int contlivro)
{
    printf("------------------------------------------------------------\nCPF         Nome\n------------------------------------------------------------\n");
    for(int i=0;i<contuser;i++)
    {
        printf("%lld %s\n",User[i].cpf,User[i].nome);
        if(User[i].codEmpre != -1)
            for(int j = 0; j < contlivro; j++)
                if(livro[j].codigo==User[i].codEmpre)
                {
                    printf("            Emprestimo: %02d/%02d/%d a %02d/%02d/%d\n", User[i].dataEmpre.dia, User[i].dataEmpre.mes, User[i].dataEmpre.ano, User[i].dataDev.dia, User[i].dataDev.mes, User[i].dataDev.ano);
                    printf("            T�tulo: %s\n            Autor: %s\n", livro[j].titulo, livro[j].autor);
                    break;
                }
    }
    printf("------------------------------------------------------------\n");
    system("pause");
    system("cls");
}

void menuCadUser(Usuario User[], Livro livro[],int &contuser, int contlivro)
{
    int opc;
    do{
        printf("Menu do Cadastro de Usuarios\n1-Cadastrar usuario\n2-Excluir usuario\n3-Listar usuarios\n4-Voltar\n");
        scanf("%d",&opc);
        getchar();
        switch (opc)
        {
            case 1:
                system("cls");
                cadUser(User, contuser);
                break;
            case 2:
                system("cls");
                exclUser(User, contuser);
                break;
            case 3:
                system("cls");
                listUser(User, livro, contuser, contlivro);
                break;
            default:
                system("cls");
                break;
        }
    }while(opc!=4);
    
}

void bubble_sort_liv(Livro v[], int n) {
    bool trocou = true;
    Livro aux;
    for (int k = n; k > 0 && trocou; k--)
    {
        trocou = false;
        for (int i = 0; i < k; i++)
            if (strcmp(v[i].titulo, v[i+1].titulo)>0)
            {
                aux = v[i+1];
                v[i+1] = v[i];
                v[i]=aux;
                trocou = true;
            }
    }
}

void cadLiv(Livro livro[], int &contlivro)
{
    bool certo, maior;
    char aux[1000];
    do{
        certo=true;
        printf("Codigo: ");
        scanf("%d",&livro[contlivro].codigo);
        getchar();
        if(contlivro>0)
            for(int i = 0; i < contlivro; i++)
                if(livro[i].codigo==livro[contlivro].codigo)
                {
                    certo=false;
                    printf("\nErro: Livro ja cadastrado!\n(Digite x para voltar ao menu)\n");
                    if(getchar()=='x')
                    {
                        system("cls");
                        return;
                    }
                    system("cls");
                }
        if(livro[contlivro].codigo<1 || livro[contlivro].codigo>=1000000)
        {
            printf("\nErro: Codigo invalido\n");
            getchar();
            system("cls");
        }
    }while (livro[contlivro].codigo<1 || livro[contlivro].codigo>=1000000 || certo==false);
    do{
        certo=false;
        maior=true;
        printf("Titulo: ");
        gets(aux);
        strupr(aux);
        fflush(stdin);

        for(int i=0;i<31;i++)
        {
            if(i<4 && aux[i]=='\0')
            {
                printf("\nErro: Titulo precisa ter pelo menos 4 digitos\n");
                getchar();
                system("cls");
                printf("Codigo: %d\n",livro[contlivro].codigo);
                maior=false;
                break;
            }
            if(i>3 && aux[i]=='\0')
            {
                certo=true;
                maior=false;
                break;
            }
        }
        if(maior==true)
        {
            printf("\nErro: Titulo precisa ter menos de 30 digitos\n");
            getchar();
            system("cls");
            printf("Codigo: %d\n",livro[contlivro].codigo);
        }
    }while(certo==false);
    strcpy(livro[contlivro].titulo, aux);
    do{
        certo=false;
        maior=true;
        printf("Autor: ");
        gets(aux);
        strupr(aux);
        fflush(stdin);

        for(int i=0;i<31;i++)
        {
            if(i<4 && aux[i]=='\0')
            {
                certo=false;
                printf("\nErro: Nome do autor precisa ter pelo menos 4 digitos\n");
                getchar();
                system("cls");
                printf("Codigo: %d\n",livro[contlivro].codigo);
                printf("Titulo: %s\n",livro[contlivro].titulo);
                maior=false;
                break;
            }
            if(i>3 && aux[i]=='\0')
            {
                certo=true;
                maior=false;
                break;
            }
        }
        if(maior==true)
        {
            printf("\nErro: Nome do autor precisa ter menos de 30 digitos\n");
            getchar();
            system("cls");
            printf("Codigo: %d\n",livro[contlivro].codigo);
            printf("Titulo: %s\n",livro[contlivro].titulo);
        }
    }while(certo==false);
    strcpy(livro[contlivro].autor, aux);
    do{
        certo=true;
        printf("Ano: ");
        scanf("%d",&livro[contlivro].ano);
        getchar();
        if(livro[contlivro].ano<1900 || livro[contlivro].ano>2050)
        {
            printf("\nErro: Ano invalido\n");
            getchar();
            system("cls");
            printf("Codigo: %d\n",livro[contlivro].codigo);
            printf("Titulo: %s\n",livro[contlivro].titulo);
            printf("Autor: %s\n",livro[contlivro].autor);
        }
    }while (livro[contlivro].ano<1900 || livro[contlivro].ano>2050);

    if(contlivro>0)
        bubble_sort_liv(livro, contlivro);
    contlivro= contlivro+1;
    
    printf("\nCadastro realizado com sucesso!\n");
    system("pause");
    system("cls");
}
void excLiv(Livro livro[], int &contlivro)
{
    int codigo, cont;
    bool certo;
    do{
        cont=0;
        certo=false;
        do{
            printf("Codigo: ");
            scanf("%d",&codigo);
            getchar();
            if(codigo>999999 || codigo<0)
            {
                printf("Erro: Codigo invalido\n");
                getchar();
                system("cls");
            }
        }while(codigo>999999 || codigo<0);
        for(int i = 0; i< contlivro; i++)
        {
            if(livro[i].codigo == codigo && livro[i].empre == -1)
            {
                certo=true;
                strcpy(livro[i].titulo, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                cont++;
                break;
            }
            else if(livro[i].codigo == codigo && livro[i].empre != -1)
            {
                printf("\nErro: livro esta emprestado!\n(Digite x para voltar ao menu)\n");
                if(getchar()=='x')
                {
                    system("cls");
                    return;
                }
                system("cls");
                cont++;
                break;
            }
        }
        if(cont==0)
        {
            printf("\nErro: livro nao cadastrado!\n(Digite x para voltar ao menu)\n");
            if(getchar()=='x')
            {
                system("cls");
                return;
            }
            system("cls");
        }
    }while(certo==false);
    contlivro--;
    bubble_sort_liv(livro,contlivro);
    printf("\nExclusao realizada com sucesso!\n");
    system("pause");
    system("cls");
}
void listLiv(Livro livro[], Usuario user[], int contlivro, int contuser)
{
    printf("---------------------------------------------------------------------------------\nCodigo Titulo                         Autor                                 Ano\n---------------------------------------------------------------------------------\n");
    for(int i=0;i<contlivro;i++)
    {
        if(livro[i].codigo!=-1)
        {
            printf("%06d ",livro[i].codigo,livro[i].titulo,livro[i].autor,livro[i].ano);
            imprimeNomeFormatado(livro[i].titulo,30);
            printf(" ");
            imprimeNomeFormatado(livro[i].autor,30);
            printf("       %d\n",livro[i].ano);
            if(livro[i].empre!=-1)
                for(int j = 0; j < contuser; j++)
                    if(user[j].codEmpre==livro[i].codigo)
                    {
                        printf("       Emprestimo: %02d/%02d/%d a %02d/%02d/%d\n", user[j].dataEmpre.dia, user[j].dataEmpre.mes, user[j].dataEmpre.ano, user[j].dataDev.dia, user[j].dataDev.mes, user[j].dataDev.ano);
                        printf("       CPF: %lld\n       Nome: %s\n", user[j].cpf, user[j].nome);
                        break;
                    }
        }
    }
    printf("---------------------------------------------------------------------------------\n");
    system("pause");
    system("cls");
}

void menuCadLiv(Livro livro[], Usuario user[], int &contlivro, int contuser)
{
    int opc;
    do{
        printf("Menu do Cadastro de Livros\n1-Cadastrar livro\n2-Excluir livro\n3-Listar livros\n4-Voltar\n");
        scanf("%d",&opc);
        getchar();
        switch (opc)
        {
            case 1:
                system("cls");
                cadLiv(livro, contlivro);
                break;
            case 2:
                system("cls");
                excLiv(livro, contlivro);
                break;
            case 3:
                system("cls");
                listLiv(livro, user, contlivro, contuser);
                break;
            default:
                system("cls");
                break;
        }
    }while(opc!=4);
}


void cadEmpre(Usuario user[], Livro livro[], int contuser, int contlivro)
{
    long long int cpf;
    int codigo, localUser, localLivro, cont;
    bool certo;
    do{
        cont=0;
        certo=false;
        do{
            printf("CPF: ");
            scanf("%lld",&cpf);
            getchar();
            if(cpf<10000000000 || cpf>99999999999)
            {
                printf("Erro: CPF invalido\n");
                getchar();
                system("cls");
            }
        }while(cpf<10000000000 || cpf>99999999999);
        for(int i=0; i<contuser;i++)
        {
            if(user[i].cpf==cpf && user[i].codEmpre==-1)
            {
                certo=true;
                localUser=i;
                cont++;
            }
            else if(user[i].cpf==cpf && user[i].codEmpre!=-1)
            {
                printf("\nErro: CPF ja tem um emprestimo!\n(Digite x para voltar ao menu)\n");
                if(getchar()=='x')
                {
                    system("cls");
                    return;
                }
                system("cls");
                cont++;
            }
        }
        if(cont==0)
        {
            printf("\nErro: CPF nao cadastrado!\n(Digite x para voltar ao menu)\n");
            if(getchar()=='x')
            {
                system("cls");
                return;
            }
            system("cls");
        }
    }while(certo==false);
    do{
        cont=0;
        certo=false;
        do{
            printf("Codigo: ");
            scanf("%d",&codigo);
            getchar();
            if(codigo>999999 || codigo<0)
            {
                printf("Erro: Codigo invalido\n");
                getchar();
                system("cls");
            }
        }while(codigo>999999 || codigo<0);
        for(int i=0; i<contlivro;i++)
        {
            if(livro[i].codigo==codigo && livro[i].empre==-1)
            {
                certo=true;
                livro[i].empre=1;
                cont++;
                localLivro=i;
            }
            else if(livro[i].codigo==codigo && livro[i].empre!=-1)
            {
                printf("\nErro: Livro ja esta emprestado!\n(Digite x para voltar ao menu)\n");
                if(getchar()=='x')
                {
                    system("cls");
                    return;
                }
                system("cls");
                printf("CPF: %lld\n",cpf);
                cont++;
            }
        }
        if(cont==0)
        {
            printf("\nErro: Livro nao cadastrado!\n(Digite x para voltar ao menu)\n");
            if(getchar()=='x')
            {
                system("cls");
                return;
            }
            system("cls");
            printf("CPF: %lld\n",cpf);
        }
    }while(certo==false);
    user[localUser].codEmpre = codigo;
    data_atual(user[localUser].dataEmpre.dia, user[localUser].dataEmpre.mes, user[localUser].dataEmpre.ano);
    data_atual(user[localUser].dataDev.dia, user[localUser].dataDev.mes, user[localUser].dataDev.ano);
    data_devolucao(user[localUser].dataDev.dia, user[localUser].dataDev.mes, user[localUser].dataDev.ano);
    printf("\nEmpr�stimo concedido!\n");
    printf("Nome: %s\nT�tulo: %s\n",user[localUser].nome,livro[localLivro].titulo);
    printf("Data do empr�stimo: %02d/%02d/%d\nData da devolu��o: %02d/%02d/%d\n",user[localUser].dataEmpre.dia, user[localUser].dataEmpre.mes, user[localUser].dataEmpre.ano, user[localUser].dataDev.dia, user[localUser].dataDev.mes, user[localUser].dataDev.ano);
    system("pause");
    system("cls");
}
void devEmpre(Usuario user[], Livro livro[], int contuser, int contlivro)
{
    long long int cpf;
    int cont, localUser, localLivro, dias;
    bool certo;
    char dataDev[9], aux[1000];
    do{
        cont=0;
        certo=false;
        do{
            printf("CPF: ");
            scanf("%lld",&cpf);
            getchar();
            if(cpf<10000000000 || cpf>99999999999)
            {
                printf("Erro: CPF invalido\n");
                getchar();
                system("cls");
            }
        }while(cpf<10000000000 || cpf>99999999999);
        for(int i=0; i<contuser;i++)
        {
            if(user[i].cpf==cpf && user[i].codEmpre!=-1)
            {
                certo=true;
                localUser=i;
                cont++;
            }
            else if(user[i].cpf==cpf && user[i].codEmpre==-1)
            {
                printf("\nErro: Usuario nao tem emprestimo!\n(Digite x para voltar ao menu)\n");
                if(getchar()=='x')
                {
                    system("cls");
                    return;
                }
                system("cls");
                cont++;
            }
        }
        if(cont==0)
        {
            printf("\nErro: Usuario nao cadastrado!\n(Digite x para voltar ao menu)\n");
            if(getchar()=='x')
            {
                system("cls");
                return;
            }
            system("cls");
        }
    }while(certo==false);
    do{
        certo=false;
        printf("Data da devolucao(DDMMAAAA): ");
        gets(aux);
        fflush(stdin);
        if(testar_data(aux, dias, user[localUser]))
            certo=true;
        else
        {
            printf("\nErro: data de devolucao invalida!(Alem de formatada, a data precisa ser menor ou igual a data do dia de hoje e maior que a data de emprestimo!)\n");
            getchar();
            system("cls");
            printf("CPF: %lld\n",cpf);
        }
    }while(certo==false);
    strcpy(dataDev, aux);
    for(int i = 0; i < contlivro; i++)
    {
        if(livro[i].codigo == user[localUser].codEmpre)
        {
            localLivro = i;
            break;
        }
    }
    user[localUser].codEmpre=-1;
    livro[localLivro].empre=-1;
    printf("\nNome: %s\nTitulo: %s\n\n",user[localUser].nome, livro[localLivro].titulo);
    if(dias>0)
        printf("\nDevolucao realizada com atraso de %d dia(s)!\n\n",dias);
    else
        printf("\nDevolucao realizada com sucesso no prazo!\n\n");
    system("pause");
    system("cls");
}
void listEmpre(Usuario user[], Livro livro[], int contuser, int contlivro)
{
    printf("----------------------------------------------------------------\nCPF         Nome      C�digo  T�tulo    Empr�stimo  Devolucao\n----------------------------------------------------------------\n");
    for(int i = 0; i<contuser; i++)
    {
        if(user[i].codEmpre!=-1)
        {
            for(int j = 0; j<contlivro; j++)
            {
                if(livro[j].codigo == user[i].codEmpre)
                {
                    printf("%lld ",user[i].cpf);
                    imprimeNomeFormatado(user[i].nome,9);
                    printf(" %06d  ",livro[j].codigo);
                    imprimeNomeFormatado(livro[j].titulo,9);
                    printf(" %02d/%02d/%d  %02d/%02d/%d\n",user[i].dataEmpre.dia, user[i].dataEmpre.mes, user[i].dataEmpre.ano, user[i].dataDev.dia, user[i].dataDev.mes, user[i].dataDev.ano);
                    break;
                }
            }
        }
    }
    printf("----------------------------------------------------------------\n");
    system("pause");
    system("cls");
}

void menuEmpre(Usuario user[], Livro livro[], int contuser, int contlivro)
{
    int opc;
    do{
        printf("Menu do Emprestimo/Devolucao\n1-Emprestar livro\n2-Devolver livro\n3-Listar emprestimos\n4-Voltar\n");
        scanf("%d",&opc);
        getchar();
        switch (opc)
        {
            case 1:
                system("cls");
                cadEmpre(user, livro, contuser, contlivro);
                break;
            case 2:
                system("cls");
                devEmpre(user, livro, contuser, contlivro);
                break;
            case 3:
                system("cls");
                listEmpre(user, livro, contuser, contlivro);
                break;
            default:
                system("cls");
                break;
        }
    }while(opc!=4);
}

int main()
{
    setlocale(LC_ALL,"");
    Usuario user[100];
    Livro livro[100];
    int opc,contuser=0, contlivro=0;
    do
    {
        printf("Menu Principal\n1-Cadastro de usuarios\n2-Cadastro de livros\n3-Emprestimo/devolucao\n4-Fim\n");
        scanf("%d",&opc);
        switch (opc)
        {
        case 1:
            system("cls");
            menuCadUser(user, livro, contuser, contlivro);
            break;
        case 2:
            system("cls");
            menuCadLiv(livro, user, contlivro, contuser);
            break;
        case 3:
            system("cls");
            menuEmpre(user, livro, contuser, contlivro);
            break;
        case 4:
            system("cls");
            printf("Obrigado, volte sempre!\n");
            break;
        default:
            system("cls");
            break;
        }
    } while (opc!=4);
}