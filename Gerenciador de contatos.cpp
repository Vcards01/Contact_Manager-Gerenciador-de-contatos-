/*Autoria: Victor Hugo Cardoso - Data: Maio de 2018. Este programa gerencia uma agenda de contatos,onde o usuario
pode inserir,pesquisar,listar e modificar dados de um contato, alem de poder selecionar os aniversariantes de um 
determinado mês.Para cada contato sera armazenado o nome completo, o telefone, o e-mail e a data de nascimento 
(dia, mês e ano).Todos os dados serão armazenados em estruturas alocadas dinamicamente e sempre que o programa 
for encerrado,os contatos serão armazenados em um arquivo binário. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

//Definição dos dados que cada contato deve possuir.
struct data
	{
		int dia;
		int mes;
		int ano;
	};
struct Contatos
{
	char nome[50];
	char telefone[15];
	char email[60];
	data datanasc;
};

// Protótipo das Funções
void menu(int &op);
void inserirContato(Contatos *p,int &qtdd,int vet);
void pesquisarContato(Contatos *p, int qtdd);
void exbirContatos(Contatos *p,int qtdd);
void alterarDados(Contatos *p, int qtdd);
void aniversariantes(Contatos *p,int qtdd);
int idade(int dia,int mes,int ano);
int meses();
void aloca(Contatos **agenda, int vet);
void realoca(Contatos **agenda,int vet);
void liberar(Contatos *agenda);

//Função Principal.
main()
{
	setlocale(LC_ALL,"Portuguese");
	Contatos *agenda;//Ponteiro para struct de contatos, que sera o vetor alocado dinamicamente.
	int vet; //Tamanho do vetor.
	int qtdd;//Numero de contatos cadastrados. 
	int opcao=-1;//Controla as opções do menu.
	FILE *fp;
	//Criando o arquivo binario. 
  	fp = fopen("agenda.dat","rb");
  	//Arquivo não foi encontrado,e será criado,isso acontecerá na primeira vez q o programa for aberto.
	if(fp==NULL)
	{
		qtdd = 0;//Quantidade incial de contatos .
   		vet = 10;//Tamanho incial do vetor.
   		aloca(&agenda,vet);//Alocando vetor Dinamincamente.
    }
    //Arquio encontrado
    else
	{
	 	//Lendo a quantidade de contatos já gravados no arquivo.
    	fread(&qtdd, sizeof(int), 1, fp);
    	/*Definindo o tamanho do vetor.Ele tem q ser maior que o numero de contatos ja gravados,
		para que assim novos contatos possam ser gravados.*/
 	   	vet = qtdd*2;
		aloca(&agenda,vet);//Alocando vetor Dinamincamente.    
		//Lendo os dados arquivo para o vetor.
    	fread(agenda, sizeof( struct Contatos), qtdd, fp);
    	//Fechando arquivo apos leitura.
    	fclose(fp);
	}
	//Rodando menu.
	while(opcao!=0)
	{
		//Se a capacidade de memoria chegar ao limite, aumentar o vetor usando o "realloc".
		if(vet==qtdd)
		{
		 vet =vet*2;
	     realoca(&agenda,vet);
		}  
		system("cls");
		menu(opcao);
	 	switch ( opcao )
    	{
    	  case 1 :
    	  	//Inserir novo contato.
      	  system("cls"); 
	  	  inserirContato(agenda,qtdd,vet);
     	 break;
 
    	  case 2 :
    	  	//Pesquisar contato.
 	  	   system("cls");
       	   pesquisarContato(agenda,qtdd);
    	 break;
 
    	 case 3 :
    	 	//Listar contato.
       		system("cls"); 
			exbirContatos(agenda,qtdd);
 	 	 break;
 
     	 case 4 :
     	 	//Alterar dados.
        	system("cls");
        	alterarDados(agenda,qtdd);
     	 break;
 
     	 case 5 :
      	  system("cls");
      	  //Aniversariantes do mes.
      	  aniversariantes(agenda,qtdd);
    	 break;
 	 	 
 	 	 case 0 :
 	 		EXIT_SUCCESS;
 	 	 break;	
     
  	   	default :
       	   printf ("Valor invalido!\n");
       	   system("pause");
		}
			
	}
	//Se existir contatos cadastrados,ira gravar no arquivo
	if(qtdd>0)
	{
		//Abre arquivo em modo de gravacao.
    	fp = fopen("agenda.dat","wb");
    	//Grava o numero de contatos no inicio do arquivo.
    	fwrite(&qtdd, sizeof(int), 1, fp);
    	//Grava os dados do vetor no arquivo.
    	fwrite(agenda, sizeof(Contatos), qtdd, fp);
    	//Fecha arquivo apos a gravacao.
    	fclose(fp);
	}
	//libera a memoria alocada.
	liberar(agenda);
}
// Declaração das funções

//Função que exibe o menu na tela.
void menu(int &op)
{
	printf("\n============MENU============");
	printf("\n[1]Inserir contatos.");
	printf("\n[2]Pesquisar contato.");
	printf("\n[3]Listar contatos.");
	printf("\n[4]Alterar dados.");
	printf("\n[5]Aniversariantes.");
	printf("\n[0]Sair.\nR:");
	scanf("%d",&op);
	
}
//Função que faz o cadastro de um novo contato na agenda.
void inserirContato(Contatos *p,int &qtdd,int vet)
{
	 
	printf("============Inserir Contato============");
	printf("\n\nNome Completo:");
	fflush(stdin);
	gets(p[qtdd].nome);
	printf("\nTelefone:");
	fflush(stdin);
	gets(p[qtdd].telefone);
	printf("\nEmail:");
	fflush(stdin);
	gets(p[qtdd].email);
	printf("\nData de Nascimento:");
	printf("\n\tDia:");
	scanf("%d",&p[qtdd].datanasc.dia);
	while(p[qtdd].datanasc.dia>31)//Evita que o usuario digite mais dias do que tem um mês.
	{
		printf("\n\tDia invalido,tente novamente:");
		printf("\n\tDia:");
		scanf("%d",&p[qtdd].datanasc.dia);
	}
	printf("\tMes:");
	scanf("%d",&p[qtdd].datanasc.mes);
	while(p[qtdd].datanasc.mes>12)//Evita que o usuario digite meses inexistentes.
	{
		printf("\n\tMes invalido,tente novamente:");
		printf("\n\tMes:");
		scanf("%d",&p[qtdd].datanasc.mes);
	}
	printf("\tAno:");
	scanf("%d",&p[qtdd].datanasc.ano);
	while((p[qtdd].datanasc.ano<1918)||(p[qtdd].datanasc.ano>2018))//Evita que o usuario digite anos absurdos.
	{
		printf("\n\tAno invalido,tente novamente:");
		printf("\n\tAno:");
		scanf("%d",&p[qtdd].datanasc.ano);
	}	
	qtdd++;
}

//Função que faz a pesquisa de um contato utilizando seu nome.
void pesquisarContato(Contatos *p, int qtdd)
{
	printf("============Pesquisa de contato============");
	char nome[50];
	int flag=0;//Flag de controle.Se o contato existe = True.Se não = False.
	printf("\n\nDigite o nome do contato:");
	fflush(stdin);
	gets(nome);
	for(int i=0;i<qtdd;i++)
	{
		if(strcmp(nome,p[i].nome)==0)
		{
			int id=idade(p[i].datanasc.dia,p[i].datanasc.mes,p[i].datanasc.ano);//Recebe da função a idade do contato.
			printf("\nNome:%s   | Telefone:%s   | Email:%s | Idade:%d\n\n",p[i].nome,p[i].telefone,p[i].email,id);
			system("pause");
			flag=1;
		}
	}
	if(flag==0)
	{
		printf("\nO contato solicitado não se encontra na agenda.\n");
		printf("\nVerifique se o nome do contato foi escrito corretamente, e tente novamente.\n\n");
		system("pause");
	}
}
  
//Função para exbir todos os contatos registrados.
void exbirContatos(Contatos *p,int qtdd)
{
	printf("============Exibir Contatos============");
	if(qtdd==0)
	{
		printf("\n\nSem contatos na agenda,adicione novos contatos e tente novamente");
	}
	else
	{
		for(int i=0;i<qtdd;i++)
	{
		int id=idade(p[i].datanasc.dia,p[i].datanasc.mes,p[i].datanasc.ano);//Recebe da função a idade do contato.
		printf("\n\n------------------------Contato %d------------------------",i+1);
		printf("\n\nNome:%s   | Telefone:%s   | Email:%s | Idade:%d",p[i].nome,p[i].telefone,p[i].email,id);
	}
		
	}
	printf("\n\n");
	system("pause");
   
}

//Função que altera os dados do contato selecionado pelo nome.
void alterarDados(Contatos *p, int qtdd)
{
	printf("============Alterar Dados============");
	char nome[50];
	int menu;//Controla as opções do menu interno da função.
	int flag=0;//Flag de controle.Se o contato existe = True.Se não = False.
	printf("\n\nDigite o nome do contato:");
	fflush(stdin);
	gets(nome);
	for(int i=0;i<qtdd;i++)
	{
		if(strcmp(nome,p[i].nome)==0)
		{
			flag=1;
			printf("\nO que deseja altera?");
			printf("\n[1]Nome.");
			printf("\n[2]Telefone.");
			printf("\n[3]Email.");
			printf("\n[4]Data de nascimento.\nR:");
			scanf("%d",&menu);
			switch(menu)
			{
			case 1:
				//Altera o nome.
				printf("\n\nNome Completo:");
	   	   	   	fflush(stdin);
				gets(p[i].nome);
				break;
			case 2:
				//Altera o telefone.
				printf("\nTelefone:");
		 	 	fflush(stdin);
				gets(p[qtdd].telefone);
				break;
			case 3:
				//Altera o email.
				printf("\nEmail:");
				fflush(stdin);
				gets(p[i].email);
				break;
			case 4:
				//Altera a data de nascimento.
				printf("\nData de Nascimento:");
				printf("\n\tDia:");
				scanf("%d",&p[qtdd].datanasc.dia);
				while(p[qtdd].datanasc.dia>31)//Evita que o usuario digite mais dias do que tem um mês.
				{
					printf("\n\tDia invalido,tente novamente:");
					printf("\n\tDia:");
					scanf("%d",&p[qtdd].datanasc.dia);
	   	   	   	}
				printf("\tMes:");
				scanf("%d",&p[qtdd].datanasc.mes);
				while(p[qtdd].datanasc.mes>12)//Evita que o usuario digite meses inexistentes.
				{
					printf("\n\tMes invalido,tente novamente:");
					printf("\n\tMes:");
					scanf("%d",&p[qtdd].datanasc.mes);
				}
				printf("\tAno:");
				scanf("%d",&p[qtdd].datanasc.ano);
				while((p[qtdd].datanasc.ano<1918)||(p[qtdd].datanasc.ano>2018))//Evita que o usuario digite anos absurdos.
				{
					printf("\n\tAno invalido,tente novamente:");
		   	   	   	printf("\n\tAno:");
					scanf("%d",&p[qtdd].datanasc.ano);
				}	
				break;
			default:
				printf ("Valor invalido!\n");
   	   	   	    system("pause");
				break;
			}
			printf("\nALTERACAO CONCLUIDA COM SUCESSO\n\n");
			system("pause");
		}
	}
	if(flag==0)
	{
		printf("\nO contato solicitado não se encontra na agenda\n\n");
		system("pause");
	}
}
//Função que printa os aniversariantes no mes em que o usuario escolher.
void aniversariantes(Contatos *p,int qtdd)
{
	printf("\n============Aniversariantes do mês============");
	int mes= meses();//Recebe da função o numero do mês,levando varias possibilidades de digitação em conta.
	int flag=0;//Flag de controle.Se o contato existe = True.Se não = False.
	for(int i=0;i<qtdd;i++)
	{
		if(mes==p[i].datanasc.mes)
		{
			flag=1;
			printf("\n\n------------------------Parabéns!------------------------",i+1);
			printf("\nContato Aniversariante :%s\n\n",p[i].nome);
		}
		
	}  
	if(flag==0)
	{
		printf("\nSem aniversariantes esse mês\n\n");
	} 
	  system("pause"); 	
}
//Função que faz o calculo da idade dos contatos.
int idade(int dia,int mes,int ano)
{
	int idade;
	time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);	
   	if(mes>(tm.tm_mon+1))
	{
		idade=(tm.tm_year + 1900)-ano-1;
		return idade;
	}
	else if(mes==(tm.tm_mon+1))
	{
		if(dia<=tm.tm_mday)
		{
			idade=(tm.tm_year + 1900)-ano;
			return idade;
		}
		else
		{
			idade=(tm.tm_year + 1900)-ano-1;
			return idade;
		}
	}
	else if(mes<(tm.tm_mon+1))
	{
		idade=(tm.tm_year + 1900)-ano;
		return idade;
	}
}
//Função que determina o mes, de acordo com o que o usuario digitar, levando varias possibilidades em consideração.
int meses()
{
	char mes[10];
	printf("\n\nDigite um mês:");
	fflush(stdin);
	gets(mes);
	if((strcmp(mes,"1")==0)||(strcmp(mes,"01")==0)||(strcmp(mes,"Janeiro")==0)||(strcmp(mes,"janeiro")==0))
	{
		return 1;
	}
	if((strcmp(mes,"2")==0)||(strcmp(mes,"02")==0)||(strcmp(mes,"Fevereiro")==0)||(strcmp(mes,"fevereiro")==0))
	{
		return 2;
	}
	if((strcmp(mes,"3")==0)||(strcmp(mes,"03")==0)||(strcmp(mes,"Março")==0)||(strcmp(mes,"marco")==0))
	{
		return 3;
	}
	if((strcmp(mes,"4")==0)||(strcmp(mes,"04")==0)||(strcmp(mes,"Abril")==0)||(strcmp(mes,"abril")==0))
	{
		return 4;
	}
	if((strcmp(mes,"5")==0)||(strcmp(mes,"05")==0)||(strcmp(mes,"Maio")==0)||(strcmp(mes,"maio")==0))
	{
		return 5;
	}
	if((strcmp(mes,"6")==0)||(strcmp(mes,"06")==0)||(strcmp(mes,"Junho")==0)||(strcmp(mes,"junho")==0))
	{
		return 6;
	}
	if((strcmp(mes,"7")==0)||(strcmp(mes,"07")==0)||(strcmp(mes,"Julho")==0)||(strcmp(mes,"julho")==0))
	{
		return 7;
	}
	if((strcmp(mes,"8")==0)||(strcmp(mes,"08")==0)||(strcmp(mes,"Agosto")==0)||(strcmp(mes,"agosto")==0))
	{
		return 8;
	}
	if((strcmp(mes,"9")==0)||(strcmp(mes,"09")==0)||(strcmp(mes,"Setembro")==0)||(strcmp(mes,"setembro")==0))
	{
		return 9;
	}
	if((strcmp(mes,"10")==0)||(strcmp(mes,"Outubro")==0)||(strcmp(mes,"outubro")==0))
	{
		return 10;
	}
	if((strcmp(mes,"11")==0)||(strcmp(mes,"Novembro")==0)||(strcmp(mes,"novembro")==0))
	{
		return 11;
	}
	if((strcmp(mes,"12")==0)||(strcmp(mes,"Dezembro")==0)||(strcmp(mes,"dezembro")==0))
	{
		return 12;
	}
}
//Função que faz a alocação de memoria do vetor dinamicamente.
void aloca(Contatos **agenda, int vet)
{
	*agenda = (Contatos*) malloc (vet*sizeof (Contatos));
	if (*agenda == NULL)
	{
		printf ("\nMemoria insuficiente!");
		exit (1);
	}
}
//Função q realoca a memoria caso o vetor chegue no limite maximo.
void realoca(Contatos **agenda,int vet)
{
	 *agenda = (Contatos*)realloc(*agenda,vet*sizeof(Contatos));
}
//Função que libera a memoria alocada, apos o uso.
void liberar(Contatos *agenda)
{
	free(agenda);
}