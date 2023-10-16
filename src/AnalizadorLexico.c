#ifndef ANALIZADOR_LEXICO_H
#define ANALIZADOR_LEXICO_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>


// Definicion los tipos de tokens
typedef enum TokenType
{
    OPERADOR_ARITMETICO,
    OPERADOR_LOGICO,
    OPERADOR_ASIGNACION,
    OPERADOR_FINALIZACION,
    OPERADOR_AGRUPACION,
    PALABRA_RESERVADA,
    IDENTIFICADOR,
    NUMERO_ENTERO,
    NUMERO_DECIMAL,
    OTRO
} TokenType;

// Estructura para almacenar informacion de un token
typedef struct Token
{
    TokenType type;
    char lexema[50]; // Tamaño maximo del lexema
} Token;

// Estructura Tabla Hash para los tipos de token
typedef struct HashTable
{
    char key[50];
    TokenType value;
} HashTable;

HashTable hashTable[] = {
    {"+", OPERADOR_ARITMETICO},
    {"-", OPERADOR_ARITMETICO},
    {"*", OPERADOR_ARITMETICO},
    {"/", OPERADOR_ARITMETICO},
    {"^", OPERADOR_ARITMETICO},
    {"and", OPERADOR_LOGICO},
    {"or", OPERADOR_LOGICO},
    {":=", OPERADOR_ASIGNACION},
    {".", OPERADOR_FINALIZACION},
    {"(", OPERADOR_AGRUPACION},
    {")", OPERADOR_AGRUPACION},
    {"[", OPERADOR_AGRUPACION},
    {"]", OPERADOR_AGRUPACION},
    {",", OPERADOR_AGRUPACION},
    {"inicio", PALABRA_RESERVADA},
    {"fin", PALABRA_RESERVADA},
    {"int", PALABRA_RESERVADA},
    {"float", PALABRA_RESERVADA},
    {"double", PALABRA_RESERVADA},
    {"char", PALABRA_RESERVADA},
};

//Verifica si el token es un identificador o no
bool esIdentificador(const char *token)
{
    int i = 0;
    char c = token[i];

    // Verificar el primer carácter
    if (!(isalpha(c) || c == '_' || c == '-'))
    {
        return false;
    }

    // Verificar los caracteres restantes
    while (c != '\0')
    {
        if (!(isalnum(c) || c == '_' || c == '-'))
        {
            return false;
        }

        c = token[++i];
    }

    return true;
}

// Function to get the token type from the hash table
TokenType getTokenType(const char *token)
{
    int numEntries = sizeof(hashTable) / sizeof(hashTable[0]);

    for (int i = 0; i < numEntries; i++)
    {
        if (strcmp(token, hashTable[i].key) == 0)
        {
            return hashTable[i].value;
        }
    }

    if (esIdentificador(token))
    {
        return IDENTIFICADOR;
    }
    else
    {
        bool esNumero = true;
        bool esDecimal = false;

        int tokenLen = strlen(token);
        for (int i = 0; i < tokenLen; i++)
        {
            if ((token[i] < '0' || token[i] > '9') && token[i] != ',')
            {
                esNumero = false;
                break;
            }
            if (token[i] == ',')
            {
                esDecimal = true;
            }
        }

        if (esNumero)
        {
            if (esDecimal)
            {
                return NUMERO_DECIMAL;
            }
            else
            {
                return NUMERO_ENTERO;
            }
        }
    }

    return OTRO;
}

// Funcion para imprimir un token
void imprimirToken(Token token)
{
    const char *tokenType[] = {
        "Operador Aritmetico",
        "Operador Logico",
        "Operador de Asignacion",
        "Operador de Finalizacion",
        "Operador de Agrupacion",
        "Palabra Reservada",
        "Identificador",
        "Numero Entero",
        "Numero Decimal",
        "Otro"};

    printf("Token: %s     Tipo: %s\n", token.lexema, tokenType[token.type]);
}

// Verifica si el token tiene un operador y agrega espacios entre él y los números
void insertarEspacios(char *cadena, const char *operadores)
{
    char resultado[100] = "";
    int len = strlen(cadena);
    int resIndex = 0;

    for (int i = 0; i < len; i++)
    {
        char actual = cadena[i];
        char siguiente = cadena[i + 1];

        // Verificar si el operador es ':='
        if (actual == ':' && siguiente == '=')
        {
            // Verificar si ya existe un espacio antes
            if (resIndex > 0 && resultado[resIndex - 1] != ' ')
            {
                resultado[resIndex++] = ' ';
            }
            resultado[resIndex++] = actual;
            resultado[resIndex++] = siguiente;
            // Verificar si ya existe un espacio después
            if (cadena[i + 2] != ' ')
            {
                resultado[resIndex++] = ' ';
            }
            i++; // Saltar el siguiente caracter
        }
        else
        {
            resultado[resIndex++] = actual;

            if (strchr(operadores, actual) != NULL && !isspace(siguiente))
            {
                resultado[resIndex++] = ' ';
            }
            else if (strchr(operadores, siguiente) != NULL && !isspace(actual))
            {
                resultado[resIndex++] = ' ';
            }
        }
    }

    strcpy(cadena, resultado);
}



// Funcion principal de analisis Lexico
void analisisLexico(const char *nombreArchivo)
{
    FILE *archivo;
    archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }

    char linea[100]; // Tamaño maximo de una linea

    while (fgets(linea, sizeof(linea), archivo))
    {
        int len = strlen(linea);
        if (linea[len - 1] == '\n')
        {
            linea[len - 1] = '\0';
            len--;
        }

        insertarEspacios(linea, "+-*/().[]");
        char *token = strtok(linea, " \t");

        while (token != NULL)
        {
            Token t;
            int tokenLen = strlen(token);

            strcpy(t.lexema, token);

            t.type = getTokenType(token);

            imprimirToken(t);

            token = strtok(NULL, " \t");
        }

    }

    fclose(archivo);
}

#endif // ANALIZADOR_LEXICO_H