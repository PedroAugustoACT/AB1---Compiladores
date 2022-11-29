#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Tk{FUNC, INIT, RET, PV, DP, VIR, VAR, INT, REL, LET, PAL, BOL, VET, CONSTI, CONSTR, CONSTL, CONSTP, CONSTB, ALIST, FLIST, ABLOCO, FBLOCO, AEXP, FEXP, OPA, OPM, OPCOMP, OPLOG, OPN, IGUAL, SE, SEN, ENQ, PARA, TEC, MON, FDA, INV};

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
	while((read = getline(&line, &len, code)) != -1){
		printf("%4d  %s", ++linha, line);
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
					creatTk(linha, coluna, OPCOMP, "OPN", "!");
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
	}
	creatTk(linha, 0, FDA, "FDA", "FDA");
	fclose(code);
	if(line)
		free(line);
	Token *tk = headTk->next;
	while(tk != NULL){
		printToken(headTk);
		free(headTk);
		headTk = tk;
		tk = headTk->next;
	}
	printToken(headTk);
	free(headTk);
	return 0;
}
