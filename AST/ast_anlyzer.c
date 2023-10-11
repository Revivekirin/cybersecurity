#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Function{
    char name[300];
    char returnType[300];
    char parameters[300];
} Function;

int functionCount = 0;
int ifStatementCount = 0;

Function functions[100]; 

void parseFile(FILE*file);
void countFunctions(const char* line);
void countIfStatements(const char* line);

int main(){
    FILE* file = fopen("controls.c", 'r');
    if (!file){
        printf("파일 열람 실패!\n");
        return 1;
    }

    parseFile(file);

    printf("함수의 개수:  %d\n", functionCount);
    for (int i=0; i< functionCount; i++) {
        printf("함수 이름: %s\n", functions[i].name);
        printf("리턴값: %s\n", functions[i].returnType);
        printf("파라미터 값: %s\n", functions[i].parameters);
    }

    printf("if문 개수: %d\n", ifStatementCount);

    fclose(file);
    return 0;
}

void parseFile(FILE* file) {
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        countFunctions(line);
        countIfStatement(line);
    }
}

void countFunctions(const char* line) {
    if(strstr(line, " int ") || strstr(line, "void")) { 
        if(strstr(line, "(") && strstr(line, ')')) { //함수인지 아닌지 파악
            char* returnTypeEnd = strchr(line, ' '); //linedml 첫 공백 부분 찾아내기(return값 위치)
            char returnType[300];
            strncpy(returnType, line, returnTypeEnd - line); //리턴타입저장
            returnType[returnTypeEnd - line] = '\0'; //리턴타입 0으로 초기화

            char* nameStart = returnTypeEnd +1; //함수 이름 시작 위치
            char* nameEnd = strchr(nameStart, '('); // (ex.)void __function__ (parameters..)
            char name[300];
            strncpy(name, nameStart, nameEnd - nameStart); //name에 함수 이름 저장
            name[nameEnd - nameStart] = '\0';

            char* parameterStart = nameEnd +1; //함수 인수 시작 위치
            char* parameterEnd = strchr(parameterStart, ')');
            char parameters[300];
            strncpy(parameters, parameterStart, parameterEnd - parameterStart);
            parameters[parameterEnd - parameterStart] = '\0'; //parameters에 함수 인수 저장

            strcpy(functions[functionCount].name, name);
            strcpy(functions[functionCount].returnType, returnType);
            strcpy(functions[functionCount].parameters, parameters);

            functionCount++;

        }

    }

}

    void countIfStatements(const char* line) {
        if (strstr(line, "if (") || strstr(line, "else if (")) {
            ifStatementCount++;
        }
    }