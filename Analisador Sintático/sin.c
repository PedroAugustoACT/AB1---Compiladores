#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Tk{FUNC, INIT, RET, PV, DP, VIR, VAR, INT, REL, LET, PAL, BOL, VET, CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, ALIST, FLIST, ABLOCO, FBLOCO, AEXP, FEXP, OPA, OPM, OPCOMP, OPLOG, OPN, IGUAL, SE, SEN, ENQ, PARA, TEC, MON, FDA, INV};

char *nomeTk[38] = {"FUNC", "INIT", "RET", "PV", "DP", "VIR", "VAR", "INT", "REL", "LET", "PAL", "BOL", "VET", "CONSTI", "CONSTR", "CONSTL", "CONSTP", "CONSTB", "ALIST", "FLIST", "ABLOCO", "FBLOCO", "AEXP", "FEXP", "OPA", "OPM", "OPCOMP", "OPLOG", "OPN", "IGUAL", "SE", "SEN", "ENQ", "PARA", "TEC", "MON", "FDA", "INV"};

typedef struct Token{
	int linha, coluna, num;
	char *nome, *lex;
	struct Token *next;
}Token;

Token *headTk = NULL;
Token *tailTk = NULL;
char curChar = '\0';

void creatTk(int linha, int coluna, int num, char* nome, char* lex){
	Token *tk = (Token*) malloc(sizeof(Token));
	tk->linha = linha;
	tk->coluna = coluna;
	tk->num = num;
	tk->nome = nome;
	tk->lex = lex;
	tk->next = NULL;
	if(tailTk == NULL){
		headTk = tk;
	}else{
		tailTk->next = tk;
	}
	tailTk = tk;
}

Token *nextToken(){
	Token *tk = headTk;
	headTk = headTk->next;
	return tk;
}

void printToken(Token *tk){
	printf("              [%04d, %04d] (%04d, %20s) %s\n", tk->linha, tk->coluna, tk->num, tk->nome, tk->lex);
}

enum Nt{Pgm, LDecl, Decl, Tipo, LId, LIdx, LIdxx, Exp, Expx, Fb, Fbx, ExpA, ExpAx, ExpM, ExpMx, ExpS, Id, LFunc, LArg, LArgOb, LArgObx, LArgOp, LBloc, Bloc, IgFunc, ExpLA, ExpLAx, SeN, PaOp, MonOp, TecOp, Ret};

char *nomeNt[32] = {"Pgm", "LDecl", "Decl", "Tipo", "LId", "LIdx", "LIdxx", "Exp", "Expx", "Fb", "Fbx", "ExpA", "ExpAx", "ExpM", "ExpMx", "ExpS", "Id", "LFunc", "LArg", "LArgOb", "LArgObx", "LArgOp", "LBloc", "Bloc", "IgFunc", "ExpLA", "ExpLAx", "SeN", "PaOp", "MonOp", "TecOp", "Ret"};

typedef struct NumTerm{
	int hand, len, lenKey, prod[10], isTk[10], key[15];
}NumTerm;

NumTerm P[76] = {
//1  Pgm = LDecl LFunc 'INIT' 'DP' LBloc 'FDA'    {tipos, 'FUNC', 'INIT'}
{Pgm, 6, 8, {LDecl, LFunc, INIT, DP, LBloc, FDA}, {0, 0, 1, 1, 0, 1}, {INT, REL, LET, PAL, BOL, VET, FUNC, INIT}},
//2  LDecl = Decl 'PV' LDecl                      {tipos}
{LDecl, 3, 6, {Decl, PV, LDecl}, {0, 1, 0}, {INT, REL, LET, PAL, BOL, VET}},
//3  LDecl = $                                    {'FUNC', 'INIT'}
{LDecl, 0, 2, {}, {}, {FUNC, INIT}},
//4  Decl = Tipo LId                              {'tipos'}
{Decl, 2, 6, {Tipo, LId}, {0, 0}, {INT, REL, LET, PAL, BOL, VET}},
//5  Tipo = 'INT'                                 {'INT'}
{Tipo, 1, 1, {INT}, {1}, {INT}},
//6  Tipo = 'REL'                                 {'REL}
{Tipo, 1, 1, {REL}, {1}, {REL}},
//7  Tipo = 'LET'                                 {'LET'}
{Tipo, 1, 1, {LET}, {1}, {LET}},
//8  Tipo = 'PAL'                                 {'PAL'}
{Tipo, 1, 1, {PAL}, {1}, {PAL}},
//9  Tipo = 'BOL'                                 {'BOL'}
{Tipo, 1, 1, {BOL}, {1}, {BOL}},
//10 Tipo = 'VET' 'ALIST' Tipo 'FLIST'            {'VET'}
{Tipo, 4, 1, {VET, ALIST, Tipo, FLIST}, {1, 1, 0, 1}, {VET}},
//11 LId = 'VAR' LIdx                             {'VAR'}
{LId, 2, 1, {VAR, LIdx}, {1, 0}, VAR},
//12 LIdx = 'VIR' LId                             {'VIR'}
{LIdx, 2, 1, {VIR, LId}, {1, 0}, VIR},
//13 LIdx = 'IGUAL' Exp LIdxx                     {'IGUAL'}
{LIdx, 3, 1, {IGUAL, Exp, LIdxx}, {1, 0, 0}, {IGUAL}},
//14 LIdx = $                                     {'PV'}
{LIdx, 0, 1, {}, {}, PV},
//15 LIdxx = 'VIR' LId                            {'VIR'}
{LIdxx, 2, 1, {VIR, LId}, {1, 0}, {VIR}},
//16 LIdxx = $                                    {'PV'}
{LIdxx, 0, 1, {}, {}, {PV}},
//17 Exp = Fb Expx                                {'OPN', 'OPA', 'AEXP', CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, 'ALIST', 'VAR'}
{Exp, 2, 10, {Fb, Expx}, {0, 0}, {OPN, OPA, AEXP, CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, ALIST, VAR}},
//18 Expx = 'OPLOG' Fb Expx                       {'OPLOG'}
{Expx, 3, 1, {OPLOG, Fb, Expx}, {1, 0, 0}, {OPLOG}},
//19 Expx = $                                     {'VIR', 'PV', 'FEXP', 'FLIST', 'ABLOCO'}
{Expx, 0, 5, {}, {}, {VIR, PV, FEXP, FLIST, ABLOCO}},
//20 Fb = 'OPN' Fb                                {'OPN'}
{Fb, 2, 1, {OPN, Fb}, {1, 0}, {OPN}},
//21 Fb = ExpA Fbx                                {'OPA', 'AEXP', const, 'ALIST', 'VAR'}
{Fb, 2, 9, {ExpA, Fbx}, {0, 0}, {OPA, AEXP, CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, ALIST, VAR}},
//22 Fbx = 'OPCOMP' ExpA                          {'OPCOMP'}
{Fbx, 2, 1, {OPCOMP, ExpA}, {1, 0}, {OPCOMP}},
//23 Fbx = $                                      {'OPLOG', 'VIR', 'PV', 'FEXP', 'FLIST', 'ABLOCO'}
{Fbx, 0, 6, {}, {}, {OPLOG, VIR, PV, FEXP, FLIST, ABLOCO}},
//24 ExpA = ExpM ExpAx                            {'OPA', 'AEXP', const, 'ALIST', 'VAR'}
{ExpA, 2, 9, {ExpM, ExpAx}, {0, 0}, {OPA, AEXP, CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, ALIST, VAR}},
//25 ExpAx = 'OPA' ExpM ExpAx                     {'OPA'}
{ExpAx, 3, 1, {OPA, ExpM, ExpAx}, {1, 0, 0}, {OPA}},
//26 ExpAx = $                                    {'OPCOMP', 'OPLOG', 'VIR', 'PV', 'FEXP', 'FLIST', 'ABLOCO'}
{ExpAx, 0, 7, {}, {}, {OPCOMP, OPLOG, VIR, PV, FEXP, FLIST, ABLOCO}},
//27 ExpM = ExpS ExpMx                            {'OPA', 'AEXP', const, 'ALIST', 'VAR'}
{ExpM, 2, 9, {ExpS, ExpMx}, {0, 0}, {OPA, AEXP, CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, ALIST, VAR}},
//28 ExpMx = 'OPM' ExpS ExpMx                     {'OPM'}
{ExpMx, 3, 1, {OPM, ExpS, ExpMx}, {1, 0, 0}, {OPM}},
//29 ExpMx = $                                    {'OPA', 'OPCOMP', 'OPLOG', 'VIR', 'PV', 'FEXP', 'FLIST', 'ABLOCO'}
{ExpMx, 0, 8, {}, {}, {OPA, OPCOMP, OPLOG, VIR, PV, FEXP, FLIST, ABLOCO}},
//30 ExpS = 'OPA' ExpS                            {'OPA'}
{ExpS, 2, 1, {OPA, ExpS}, {1, 0}, {OPA}},
//31 ExpS = 'AEXP' Exp 'FEXP'                     {'AEXP'}
{ExpS, 3, 1, {AEXP, Exp, FEXP}, {1, 0, 1}, {AEXP}},
//32 ExpS = 'CONSTI'                              {'CONSTI'}
{ExpS, 1, 1, {CONSTI}, {1}, {CONSTI}},
//33 ExpS = 'CONSTR'                              {'CONSTR'}
{ExpS, 1, 1, {CONSTR}, {1}, {CONSTR}},
//34 ExpS = 'CONSTL'                              {'CONSTL'}
{ExpS, 1, 1, {CONSTL}, {1}, {CONSTL}},
//35 ExpS = 'CONSTP'                              {'CONSTP'}
{ExpS, 1, 1, {CONSTP}, {1}, {CONSTP}},
//36 ExpS = 'CONSTB'                              {'CONSTB'}
{ExpS, 1, 1, {CONSTB}, {1}, {CONSTB}},
//37 ExpS = 'ALIST' ExpLA 'FLIST'                 {'ALIST'}
{ExpS, 3, 1, {ALIST, ExpLA, FLIST}, {1, 0, 1}, {ALIST}},
//38 ExpS = 'VAR' Id                              {'VAR'}
{ExpS, 2, 1, {VAR, Id}, {1, 0}, {VAR}},
//39 Id = 'AEXP' ExpLA 'FEXP'                      {'AEXP'}
{Id, 3, 1, {AEXP, ExpLA, FEXP}, {1, 0, 1}, {AEXP}},
//40 Id = 'ALIST' Exp 'FLIST'                    {'ALIST'}
{Id, 3, 1, {ALIST, Exp, FLIST}, {1, 0, 1}, {ALIST}},
//41 Id = $                                       {'OPA', 'OPM', 'OPCOMP', 'OPLOG', 'VIR', 'PV', 'FEXP', 'FLIST', 'ABLOCO'}
{Id, 0, 9, {}, {}, {OPA, OPM, OPCOMP, OPLOG, VIR, PV, FEXP, FLIST, ABLOCO}},
//42 LFunc = 'FUNC' 'VAR' 'AEXP' LArg 'ABLOCO' LBloc Ret LFunc {'FUNC'}
{LFunc, 8, 1, {FUNC, VAR, AEXP, LArg, ABLOCO, LBloc, Ret, LFunc}, {1, 1, 1, 0, 1, 0, 0, 0}, {FUNC}},
//43 LFunc = $                                    {'INIT'}
{LFunc, 0, 1, {}, {}, {INIT}},
//44 LArg = LArgOb 'FEXP'                         {tipos}
{LArg, 2, 6, {LArgOb, FEXP}, {0, 1}, {INT, REL, LET, PAL, BOL, VET}},
//45 LArg = 'FEXP'                                {'FEXP'}
{LArg, 1, 1, {FEXP}, {1}, {FEXP}},
//46 LArgOb = Tipo 'VAR' LArgObx                  {tipos}
{LArgOb, 3, 6, {Tipo, VAR, LArgObx}, {0, 1, 0}, {INT, REL, LET, PAL, BOL, VET}},
//47 LArgObx = 'VIR' LArgOb                       {'VIR'}
{LArgObx, 2, 1, {VIR, LArgOb}, {1, 0}, {VIR}},
//48 LArgObx = 'IGUAL' Exp LArgOp                 {'IGUAL'}
{LArgObx, 3, 1, {IGUAL, Exp, LArgOp}, {1, 0, 0}, {IGUAL}},
//49 LArgObx = $                                  {'FEXP'}
{LArgObx, 0, 1, {}, {}, {FEXP}},
//50 LArgOp = 'VIR' Tipo 'VAR' 'IGUAL' Exp LArgOp {'VIR'}
{LArgOp, 6, 6, {VIR, Tipo, VAR, IGUAL, Exp, LArgOp}, {1, 0, 1, 1, 0, 0}, {VIR}},
//51 LArgOp = $                                   {'FEXP'}
{LArgOp, 0, 1, {}, {}, {FEXP}},
//52 LBloc = Bloc LBloc                           {'VAR', 'SE', 'ENQ', 'PARA', 'TEC', 'MON'}
{LBloc, 2, 6, {Bloc, LBloc}, {0, 0}, {VAR, SE, ENQ, PARA, TEC, MON}},
//53 LBloc = $                                    {'FDA', 'RET', 'FBLOCO'}
{LBloc, 0, 3, {}, {}, {FDA, RET, FBLOCO}},
//54 Bloc = 'VAR' IgFunc 'PV'                     {'VAR'}
{Bloc, 3, 1, {VAR, IgFunc, PV}, {1, 0, 1}, {VAR}},
//55 Bloc = 'SE' Exp 'ABLOCO' LBloc 'FBLOCO' SeN  {'SE'}
{Bloc, 6, 1, {SE, Exp, ABLOCO, LBloc, FBLOCO, SeN}, {1, 0, 1, 0, 1, 0}, {SE}},
//56 Bloc = ENQ Exp 'ABLOCO' LBloc 'FBLOCO'       {'ENQ'}
{Bloc, 5, 1, {ENQ, Exp, ABLOCO, LBloc, FBLOCO}, {1, 0, 1, 0, 1}, {ENQ}},
//57 Bloc = 'PARA' 'VAR' 'IGUAL' Exp 'VIR' Exp PaOp 'ABLOCO' LBloc 'FBLOCO' {'PARA'}
{Bloc, 10, 1, {PARA, VAR, IGUAL, Exp, VIR, Exp, PaOp, ABLOCO, LBloc, FBLOCO}, {1, 1, 1, 0, 1, 0, 0, 1, 0, 1}, {PARA}},
//58 Bloc = 'MON' Exp MonOp 'PV'                  {'MON'}
{Bloc, 4, 1, {MON, Exp, MonOp, PV}, {1, 0, 0, 1}, {MON}},
//59 Bloc = 'TEC' 'VAR' TecOp 'PV'                {'TEC'}
{Bloc, 4, 1, {TEC, VAR, TecOp, PV}, {1, 1, 0, 1}, {TEC}},
//60 IgFunc = 'IGUAL' Exp                         {'IGUAL'}
{IgFunc, 2, 1, {IGUAL, Exp}, {1, 0}, {IGUAL}},
//61 IgFunc = 'ALIST' Exp 'FLIST' 'IGUAL' Exp     {'ALIST'}
{IgFunc, 5, 1, {ALIST, Exp, FLIST, IGUAL, Exp}, {1, 0, 1, 1, 0}, {ALIST}},
//62 IgFunc = 'AEXP' ExpLA 'FEXP'                 {'AEXP'}
{IgFunc, 3, 1, {AEXP, ExpLA, FEXP}, {1, 0, 1}, {AEXP}},
//63 ExpLA = Exp ExpLAx                           {'OPN', 'OPA', 'AEXP', const, 'ALIST', 'VAR'}
{ExpLA, 2, 10, {Exp, ExpLAx}, {0, 0}, {OPN, OPA, AEXP, CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, ALIST, VAR}},
//64 ExpLA = $                                    {'FEXP', 'FLIST'}
{ExpLA, 0, 2, {}, {}, {FEXP, FLIST}},
//65 ExpLAx = 'VIR' Exp ExpLAx
{ExpLAx, 3, 1, {VIR, Exp, ExpLAx}, {1, 0, 0}, {VIR}},
//66 ExpLAx = $                                   {'FEXP', 'FLIST'}
{ExpLAx, 0, 2, {}, {}, {FEXP, FLIST}},
//67 SeN = 'SEN' 'ABLOCO' LBloc 'FBLOCO'          {'SEN'}
{SeN, 4, 1, {SEN, ABLOCO, LBloc, FBLOCO}, {1, 1, 0, 1}, {SEN}},
//68 SeN = $                                      {'VAR', 'SE', 'ENQ', 'PARA', 'TEC', 'MON', 'FDA', 'RET', 'FBLOCO'}
{SeN, 0, 9, {}, {}, {VAR, SE, ENQ, PARA, TEC, MON, FDA, RET, FBLOCO}},
//69 PaOp = 'VIR' Exp                             {'VIR'}
{PaOp, 2, 1, {VIR, Exp}, {1, 0}, {VIR}},
//70 PaOp = $                                     {'ABLOCO'}
{PaOp, 0, 1, {}, {}, {ABLOCO}},
//71 MonOp = 'VIR' Exp MonOp                      {'VIR'}
{MonOp, 3, 1, {VIR, Exp, MonOp}, {1, 0, 0}, {VIR}},
//72 MonOp = $                                    {'PV'}
{MonOp, 0, 1, {}, {}, {PV}},
//73 TecOp = 'VIR' 'VAR' TecOp                    {'VIR'}
{TecOp, 3, 1, {VIR, VAR, TecOp}, {1, 1, 0}, {VIR}},
//74 TecOp = $                                    {'PV'}
{TecOp, 0, 1, {}, {}, {PV}},
//75 Ret = 'RET' Exp 'PV' 'FBLOCO'                {'RET'}
{Ret, 4, 1, {RET, Exp, PV, FBLOCO}, {1, 0, 1, 1}, {RET}},
//76 Ret = 'FBLOCO'                               {'FBLOCO'}
{Ret, 1, 1, {FBLOCO}, {1}, {FBLOCO}},
};

void printProd(int i){
	printf("          %s =", nomeNt[P[i].hand]);
	int j = -1;
	while(++j < P[i].len){
		if(P[i].isTk[j])
			printf(" '%s'", nomeTk[P[i].prod[j]]);
		else
			printf(" %s", nomeNt[P[i].prod[j]]);
	}
	if(P[i].len == 0)
		printf("'épsilon'");
	printf("\n");
}

typedef struct PILHA{
	int hand, isTk;
	struct PILHA *next;
}PILHA;

PILHA *criaPilha(){
	PILHA *p = (PILHA*) malloc(sizeof(PILHA));
	p->hand = Pgm; p->isTk = 0; p->next = NULL;
	return p;
}

PILHA *pop(PILHA *p){
	PILHA *aux = p->next;
	free(p);
	return aux;
} 

PILHA *popP(PILHA *p, int i){
	if(p->hand != P[i].hand){
		printf("Não terminal da Produção %d, não identificado\n", i);
		exit(2);
	}
	PILHA *aux = p->next;
	free(p);
	int j = P[i].len;
	while(j--){
		p = (PILHA*) malloc(sizeof(PILHA));
		p->hand = P[i].prod[j];
		p->isTk = P[i].isTk[j];
		p->next = aux;
		aux = p;
	}
	//p = aux;
	//while(aux != NULL){
	//	if(aux->isTk)
	//		printf("%s ", nomeTk[aux->hand]);
	//	else
	//		printf("%s ", nomeNt[aux->hand]);
	//	printf("\n");
	//	aux = aux->next;
	//}
	//aux = p;
	return aux;
}

int main(int argc, char *argv[ ]) {
	FILE *code = fopen(argv[1], "r");
	char var[102];
	char *p;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int linha = 0;
	int c;
	if(code == NULL){
		printf("Arquivo code.txt não existe\n");
		return -1;
	}
	Token *aux;
	while((read = getline(&line, &len, code)) != -1){
		linha++;
		//printf("%4d  %s", ++linha, line);
		int coluna = -1;
		while(++coluna < read){
			switch(line[coluna]){
				case '\n':
				case '\0':
				case '\t':
				case ' ':
					break;
				case ';':
					creatTk(linha, coluna, PV, "PV", ";");
					break;
				case ':':
					creatTk(linha, coluna, DP, "DP", ":");
					break;
				case ',':
					creatTk(linha, coluna, VIR, "VIR", ",");
					break;
				case '[':
					creatTk(linha, coluna, ALIST, "ALIST", "[");
					break;
				case ']':
					creatTk(linha, coluna, FLIST, "FLIST", "]");
					break;
				case '{':
					creatTk(linha, coluna, ABLOCO, "ABLOCO", "{");
					break;
				case '}':
					creatTk(linha, coluna, FBLOCO, "FBLOCO", "}");
					break;
				case '(':
					creatTk(linha, coluna, AEXP, "AEXP", "(");
					break;
				case ')':
					creatTk(linha, coluna, FEXP, "FEXP", ")");
					break;
				case '+':
					creatTk(linha, coluna, OPA, "OPA", "+");
					break;
				case '*':
					creatTk(linha, coluna, OPM, "OPM", "*");
					break;
				case '/':
					creatTk(linha, coluna, OPM, "OPM", "/");
					break;
				case '>':
					if(coluna + 1 < read && line[coluna + 1] == '='){
						creatTk(linha, coluna++, OPCOMP, "OPCOMP", ">=");
						break;
					}
					creatTk(linha, coluna, OPCOMP, "OPCOMP", ">");
					break;
				case '<':
					if(coluna + 1 < read && line[coluna + 1] == '='){
						creatTk(linha, coluna++, OPCOMP, "OPCOMP", "<=");
						break;
					}
					creatTk(linha, coluna, OPCOMP, "OPCOMP", "<");
					break;
				case '!':
					if(coluna + 1 < read && line[coluna + 1] == '='){
						creatTk(linha, coluna++, OPCOMP, "OPCOMP", "!=");
						break;
					}
					creatTk(linha, coluna, OPN, "OPN", "!");
					break;
				case '=':
					if(coluna + 1 < read && line[coluna + 1] == '='){
						creatTk(linha, coluna++, OPCOMP, "OPCOMP", "==");
						break;
					}
					creatTk(linha, coluna, IGUAL, "IGUAL", "=");
					break;
				case '^':
					creatTk(linha, coluna, OPLOG, "OPLOG", "^");
					break;
				case '|':
					creatTk(linha, coluna, OPLOG, "OPLOG", "|");
					break;
				case '\'':
					if(line[coluna + 1] == '\\' && line[coluna + 3] == '\''){
						p = malloc(3 * sizeof(char));
						p[0] = line[coluna + 1];
						p[1] = line[coluna + 2];
						p[2] = '\0';
						creatTk(linha, coluna, CONSTL, "CONSTL", p);
						coluna += 3;
						break;
					}
					if(line[coluna + 2] != '\''){
						printf("char detected but not between '.'\n");
						creatTk(linha, coluna, INV, "INV", "\'");
						break;
					}
					p = malloc(2 * sizeof(char));
					p[0] = line[coluna + 1];
					p[1] = '\0';
					creatTk(linha, coluna, CONSTL, "CONSTL", p);
					coluna += 2;
					break;
				case '\"':
					c = 1;
					while(line[coluna + c] != '\0' || (line[coluna + c] == '\"' && line[coluna + c + 1] == '\"')){
						if(line[coluna + c] == '\"'){
							if(line[coluna + c + 1] == '\"'){
								var[c++ - 1] = '\"';
							}else{
								break;
							}
						}
						var[c - 1] = line[coluna + c];
						c++;
					}
					var[c - 1] = '\0';
					p = malloc(102 * sizeof(char));
					strcpy(p, var);
					creatTk(linha, coluna, CONSTP, "CONSTP", p);
					coluna += c;
					break;
				case '-':
					if(line[coluna + 1] < '0' || line[coluna + 1] > '9'){
						creatTk(linha, coluna, OPA, "OPA", "-");
						break;
					}
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					c = 0;
					var[c++] = line[coluna];
					while(line[coluna + c] >= '0' && line[coluna + c] <= '9'){
						var[c] = line[coluna + c];
						c++;
					}
					if(line[coluna + c] == '.'){
						var[c++] = '.';
						while(line[coluna + c] >= '0' && line[coluna + c] <= '9'){
							var[c] = line[coluna + c];
							c++;
						}
						if(line[coluna + c] != 'E'){
							var[c] = '\0';
							p = malloc(102 * sizeof(char));
							strcpy(p, var);
							creatTk(linha, coluna, CONSTR, "CONSTR", p);
							coluna += c - 1;
							break;
						}
					}
					if(line[coluna + c] == 'E'){
						var[c++] = 'E';
						if(line[coluna + c] == '-')
							var[c++] = '-';
						while(line[coluna + c] >= '0' && line[coluna + c] <= '9'){
							var[c] = line[coluna + c];
							c++;
						}
						var[c] = '\0';
						p = malloc(102 * sizeof(char));
						strcpy(p, var);
						creatTk(linha, coluna, CONSTR, "CONSTR", p);
						coluna += c - 1;
						break;
					}
					var[c] = '\0';
					p = malloc(102 * sizeof(char));
					strcpy(p, var);
					creatTk(linha, coluna, CONSTI, "CONSTI", p);
					coluna += c - 1;
					break;
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
				case 'o':
				case 'p':
				case 'q':
				case 'r':
				case 's':
				case 't':
				case 'u':
				case 'v':
				case 'x':
				case 'w':
				case 'y':
				case 'z':
					c = 0;
					var[c++] = line[coluna];
					while(line[coluna + c] >= 'a' && line[coluna + c] <= 'z'){
						var[c] = line[coluna + c];
						c++;
					}
					var[c] = '\0';
					p = malloc(102 * sizeof(char));
					strcpy(p, var);
					creatTk(linha, coluna, VAR, "VAR", p);
					coluna += c - 1;
					break;
				case 'F':
					if(coluna + 3 < read && line[coluna + 1] == 'U' && line[coluna + 2] == 'N' && line[coluna + 3] == 'C'){
						creatTk(linha, coluna, FUNC, "FUNC", "FUNC");
						coluna += 3;
						break;
					}
					creatTk(linha, coluna, CONSTB, "CONSTB", "F");
					break;
				case 'V':
					if(coluna + 2 < read && line[coluna + 1] == 'E' && line[coluna + 2] == 'T'){
						creatTk(linha, coluna, VET, "VET", "VET");
						coluna += 2;
						break;
					}
					creatTk(linha, coluna, CONSTB, "CONSTB", "V");
					break;
				case 'S':
					if(coluna + 1 < read && line[coluna + 1] == 'E'){
						if(coluna + 2 < read && line[coluna + 2] == 'N'){
							creatTk(linha, coluna, SEN, "SEN", "SEN");
							coluna += 2;
							break;
						}
						creatTk(linha, coluna, SE, "SE", "SE");
						coluna += 1;
						break;
					}
				case 'I':
					if(coluna + 3 < read && line[coluna + 1] == 'N'){
						if(line[coluna + 2] == 'T'){
							creatTk(linha, coluna, INT, "INT", "INT");
							coluna += 2;
							break;
						}
						if(line[coluna + 2] == 'I' && line[coluna + 3] == 'T'){
							creatTk(linha, coluna, INIT, "INIT", "INIT");
							coluna += 3;
							break;
						}
					}
				case 'L':
					if(coluna + 2 < read && line[coluna + 1] == 'E' && line[coluna + 2] == 'T'){
						creatTk(linha, coluna, LET, "LET", "LET");
						coluna += 2;
						break;
					}
				case 'P':
					if(coluna + 3 < read && line[coluna + 1] == 'A'){
						if(line[coluna + 2] == 'L'){
							creatTk(linha, coluna, PAL, "PAL", "PAL");
							coluna += 2;
							break;
						}
						if(line[coluna + 2] == 'R' && line[coluna + 3] == 'A'){
							creatTk(linha, coluna, PARA, "PARA", "PARA");
							coluna += 3;
							break;
						}
					}
				case 'E':
					if(coluna + 2 < read && line[coluna + 1] == 'N' && line[coluna + 2] == 'Q'){
						creatTk(linha, coluna, ENQ, "ENQ", "ENQ");
						coluna += 2;
						break;
					}
				case 'B':
					if(coluna + 2 < read && line[coluna + 1] == 'O' && line[coluna + 2] == 'L'){
						creatTk(linha, coluna, BOL, "BOL", "BOL");
						coluna += 2;
						break;
					}
				case 'T':
					if(coluna + 2 < read && line[coluna + 1] == 'E' && line[coluna + 2] == 'C'){
						creatTk(linha, coluna, TEC, "TEC", "TEC");
						coluna += 2;
						break;
					}
				case 'M':
					if(coluna + 2 < read && line[coluna + 1] == 'O' && line[coluna + 2] == 'N'){
						creatTk(linha, coluna, MON, "MON", "MON");
						coluna += 2;
						break;
					}
				case 'R':
					if(coluna + 2 < read && line[coluna + 1] == 'E'){
						if(line[coluna + 2] == 'T'){
							creatTk(linha, coluna, RET, "RET", "RET");
							coluna += 2;
							break;
						}
						if(line[coluna + 2] == 'L'){
							creatTk(linha, coluna, REL, "REL", "REL");
							coluna += 2;
							break;
						}
					}
				default:
					p = malloc(2 * sizeof(char));
					p[0] = line[coluna];
					p[1] = '\0'; 
					creatTk(linha, coluna, INV, "INV", p);
			}
		}
		if(linha == 1){
		    aux = headTk;
			//printToken(aux);
		}
		if(aux->next != NULL){
			aux = aux->next;
			while(aux->next != NULL){
				//printToken(aux);
				aux = aux->next;
			}
			//printToken(aux);
		}
	}
	creatTk(linha, 0, FDA, "FDA", "FDA");
	//printToken(aux->next);
	fclose(code);
	if(line)
		free(line);
	int n = 32, m = 38, i = -1, j;
	int tabela[n][m];
	while(++i < n){
		j = -1;
		while(++j < m)
			tabela[i][j] = -1;
	}
	i = -1;
	while(++i < 76){
		//printProd(i);
		j = -1;
		//printf("j = %d\n", P[i].lenKey);
		while(++j < P[i].lenKey){
			tabela[P[i].hand][P[i].key[j]] = i;
			//printf("tabela[%s][%s] = %d\n", nomeNt[P[i].hand], nomeTk[P[i].key[j]], i);
		}
	}
	i = -1;
	while(++i < n){
		j = -1;
		while(++j < m){
			//printf("%2d,",tabela[i][j] + 1);
		}
		//printf("\n");
	}
	PILHA *pilha = criaPilha();
	Token *tk;
	i = 0;
	while(pilha != NULL && headTk != NULL){
		//printf("Topo: ");
		if(pilha->isTk){
			//printf(" %s\n", nomeTk[pilha->hand]);
			if(pilha->hand != headTk->num){
				printf("Erro, token mal posicionado\n");
			}
			pilha = pop(pilha);
			printToken(headTk);
			tk = headTk->next;
			free(headTk);
			headTk = tk;
		}else{
			//printf(" %s\n", nomeNt[pilha->hand]);
			int pd = tabela[pilha->hand][headTk->num];
			//printf("tabela[%s][%s] = %d\n", nomeNt[pilha->hand], nomeTk[headTk->num], pd);
			if(pd != -1){
				printProd(pd);
				pilha = popP(pilha, pd);
			}else{
				printf("Erro ao ler produção\n");
				exit(3);
			}
		}
	}
	if(pilha == NULL && headTk == NULL)
		printf("programa sem erros sintáticos\n");
	return 0;
}

