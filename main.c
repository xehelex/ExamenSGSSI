#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

//======================== Estructuras ========================
typedef struct {
    char simbolo;
    int veces;
} LetraContada;

typedef struct {
    char original;
    char reemplazo;
} ParLetra;

//======================== Declaraciones ======================
void mostrarArchivoContenido(char *nombre);
void analizarFrecuencias(char *nombre, LetraContada lista[]);
int compararFrecuencias(const void *x, const void *y);
void generarAbecedarioNuevo(LetraContada lista[], ParLetra pares[]);
void verAbecedario(ParLetra pares[]);
void traducirArchivo(char *origen, char *destino, ParLetra pares[]);
void ajustarSustituciones(char *origen, char *destino, ParLetra pares[]);

//======================== Programa principal =================
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso correcto: %s archivoEntrada archivoSalida\n", argv[0]);
        return 1;
    }

    printf("=== CONTENIDO ORIGINAL ===\n\n");
    mostrarArchivoContenido(argv[1]);

    LetraContada letras[26];
    analizarFrecuencias(argv[1], letras);

    ParLetra abecedario[26];
    generarAbecedarioNuevo(letras, abecedario);

    traducirArchivo(argv[1], argv[2], abecedario);
    ajustarSustituciones(argv[1], argv[2], abecedario);

    return 0;
}

//======================== Mostrar texto ======================
void mostrarArchivoContenido(char *nombre) {
    FILE *arch = fopen(nombre, "r");
    if (!arch) {
        perror("No se pudo abrir el archivo");
        return;
    }

    int c;
    while ((c = fgetc(arch)) != EOF)
        putchar(c);

    fclose(arch);
}

//======================== Contar letras ======================
void analizarFrecuencias(char *nombre, LetraContada lista[]) {
    for (int i = 0; i < 26; i++) {
        lista[i].simbolo = 'a' + i;
        lista[i].veces = 0;
    }

    FILE *arch = fopen(nombre, "r");
    if (!arch) {
        perror("Error al abrir archivo para lectura");
        return;
    }

    int ch;
    while ((ch = fgetc(arch)) != EOF) {
        if (isalpha(ch)) {
            ch = tolower(ch);
            lista[ch - 'a'].veces++;
        }
    }

    fclose(arch);

    qsort(lista, 26, sizeof(LetraContada), compararFrecuencias);

    printf("\nFrecuencia de aparición:\n");
    for (int i = 0; i < 26; i++)
        printf("%c: %d\n", lista[i].simbolo, lista[i].veces);
}

//======================== Comparador ======================
int compararFrecuencias(const void *x, const void *y) {
    LetraContada *a = (LetraContada *)x;
    LetraContada *b = (LetraContada *)y;
    return b->veces - a->veces;
}

//======================== Crear abecedario ======================
void generarAbecedarioNuevo(LetraContada lista[], ParLetra pares[]) {
    char referencia[] = "eaolsndruitcpmyqbhgfvjnzxkw";
    int i = 0;
    while (i < 26) {
        pares[i].original = lista[i].simbolo;
        pares[i].reemplazo = referencia[i];
        i++;
    }
    verAbecedario(pares);
}

//======================== Mostrar nuevo abecedario ======================
void verAbecedario(ParLetra pares[]) {
    printf("\nAbecedario generado:\n");
    int i = 0;
    while (i < 26) {
        printf("%c -> %c\n", pares[i].original, pares[i].reemplazo);
        i++;
    }
}

//======================== Descifrar ======================
void traducirArchivo(char *origen, char *destino, ParLetra pares[]) {
    FILE *entrada = fopen(origen, "r");
    FILE *salida = fopen(destino, "w");

    if (!entrada || !salida) {
        perror("Error al abrir los archivos");
        if (entrada) fclose(entrada);
        if (salida) fclose(salida);
        return;
    }

    int c;
    while ((c = fgetc(entrada)) != EOF) {
        char letra = tolower(c);
        int encontrado = 0, i = 0;

        while (i < 26 && encontrado == 0) {
            if (letra == pares[i].original) {
                fputc(pares[i].reemplazo, salida);
                encontrado = 1;
            }
            i++;
        }

        if (!encontrado)
            fputc(c, salida);
    }

    fclose(entrada);
    fclose(salida);

    printf("\n=== TEXTO DESCIFRADO ===\n\n");
    mostrarArchivoContenido(destino);
}

//======================== Modificar sustituciones ======================
void ajustarSustituciones(char *origen, char *destino, ParLetra pares[]) {
    char opcion;
    printf("\n¿Deseas cambiar alguna sustitución? (S/N)\n>> ");
    scanf(" %c", &opcion);

    while (tolower(opcion) == 's') {
        char vieja, nueva;
        printf("Letra descifrada a cambiar: ");
        scanf(" %c", &vieja);
        printf("Reemplazar por: ");
        scanf(" %c", &nueva);

        int i = 0;
        while (i < 26) {
            if (tolower(vieja) == pares[i].reemplazo) {
                pares[i].reemplazo = tolower(nueva);
                break;
            }
            i++;
        }

        traducirArchivo(origen, destino, pares);

        printf("\n¿Quieres hacer otro cambio? (S/N)\n>> ");
        scanf(" %c", &opcion);
    }
}

