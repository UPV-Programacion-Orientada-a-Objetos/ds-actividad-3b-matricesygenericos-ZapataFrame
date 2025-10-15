#include <iostream>
#include <iomanip>
#include <limits>
#include "MatrizDinamica.h"

// Imprimir matriz
template <typename T>
void print_matriz(MatrizDinamica<T> matriz);

// Imprimir mensaje
void print_message(std::string msg);

// Obtener Inputs de un tipo de dato flexible:
template <typename T>
T getInput(const std::string prompt);

template <>
std::string getInput<std::string>(const std::string prompt);

int main()
{
    print_message("--- Sistema de Análisis de Transformaciones Lineales ---");
    print_message(">> Prueba de Matriz ENTERA (Matriz A) <<");
    print_message("Creando Matriz A (Tipo INT) de 2x3...");
    print_message("Ingresar valores para A (2x3):");
    MatrizDinamica<int> *matrizA = new MatrizDinamica<int>(2, 3);
    for (int i = 0; i < matrizA->getFilas(); i++)
    {
        for (int j = 0; j < matrizA->getColumnas(); j++)
        {
            std::string prompt = "A";
            prompt.append("[" + std::to_string(i) + "]"); // filas
            prompt.append("[" + std::to_string(j) + "]"); // columnas
            prompt.append(":");
            int value = getInput<int>(prompt);
            matrizA->getDatos()[i][j] = value;
        }
    }
    print_message(">> Redimensionando Matriz A <<");
    matrizA->redimensionar(3, 3);
    print_message("Redimensionando A a 3x3. Datos conservados:");
    print_matriz(*matrizA);

    print_message(">> Prueba de Multiplicación (Tipo FLOAT) <<");
    print_message("Ingresar valores para B (Tipo FLOAT) de 3x2...");
    MatrizDinamica<float> *matrizB = new MatrizDinamica<float>(3, 2);
    for (int i = 0; i < matrizB->getFilas(); i++)
    {
        for (int j = 0; j < matrizB->getColumnas(); j++)
        {
            std::string prompt = "B";
            prompt.append("[" + std::to_string(i) + "]"); // filas
            prompt.append("[" + std::to_string(j) + "]"); // columnas
            prompt.append(":");
            float value = getInput<float>(prompt);
            matrizB->getDatos()[i][j] = value;
        }
    }
    print_matriz(*matrizB);
    MatrizDinamica<float> *matrizC = MatrizDinamica<float>::multiplicar(*matrizA, *matrizB);

    print_message("Matriz C = A(3x3) x B(3x2) ...");
    print_message("Matriz C (Resultado 3x2, Tipo FLOAT):");
    print_matriz(*matrizC);

    print_message("Liberando memoria de todas las matrices...");

    delete matrizA;
    delete matrizB;
    delete matrizC;
    print_message("Memoria liberada exitosamente!, Bye!");
    return 0;
}

template <typename T>
void print_matriz(MatrizDinamica<T> matriz)
{
    for (int i = 0; i < matriz.getFilas(); i++)
    {
        for (int j = 0; j < matriz.getColumnas(); j++)
        {
            std::string celda = (j != matriz.getColumnas() - 1) ? "|" : "";
            celda.append(std::to_string(matriz.getDatos()[i][j]));
            celda.append("|");
            std::cout << celda;
        }
        std::cout << std::endl;
    }
}

template <typename T>
T getInput(const std::string prompt)
{
    T input;
    std::cout << prompt;
    while (!(std::cin >> input))
    {
        std::cout << "Ingrese un input valido!" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << prompt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

template <>
std::string getInput(const std::string prompt)
{
    std::cout << prompt;
    std::string temp;
    std::getline(std::cin, temp);
    return temp;
}

void print_message(std::string msg)
{
    const std::string MENU_TOP_ROUND = "╭─────────────────────────────────────────────────────╮";
    const std::string MENU_BORDER = "│";
    const std::string MENU_BOT_ROUND = "╰─────────────────────────────────────────────────────╯";
    const int content_width = 50;
    std::cout << MENU_TOP_ROUND << std::endl;
    std::string formatted_msg = " " + msg + " ";
    std::cout << MENU_BORDER << std::left << std::setw(content_width) << formatted_msg << std::endl;
    std::cout << MENU_BOT_ROUND << std::endl;
}
