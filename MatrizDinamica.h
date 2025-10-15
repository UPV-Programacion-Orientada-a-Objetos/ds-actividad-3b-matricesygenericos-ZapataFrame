#include <stdexcept>

template <typename T>
class MatrizDinamica
{
private:
    T **datos;
    int filas;
    int columnas;

public:
    // Getters
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
    T **getDatos() const { return datos; }

    // Constructor; Es llamado para realizar una instancia. (Constructor)
    MatrizDinamica(int f, int c)
        : datos(nullptr), filas(f), columnas(c)
    {
        // Crear matriz al instanciar clase
        this->datos = createMatiz(this->filas, this->columnas);
    }
    // Destructor; Es llamado al destruirse el objeto (Destructor)
    ~MatrizDinamica()
    {
        this->deleteMatriz(this->datos, this->filas, this->columnas);
    };
    // Copiar mediante constructor (Copy Constructor)

    MatrizDinamica(const MatrizDinamica<T> &other)
        : datos(nullptr), filas(0), columnas(0)
    {
        // 1. Crear copia de la matriz que contiene el objeto source
        this->datos = this->copyMatriz(other.getDatos(), other.getFilas(), other.getColumnas());

        // 2. Actualizar filas y columnas
        this->filas = other.getFilas();
        this->columnas = other.getColumnas();
    }

    // Copia los valores de un objeto al objeto. (Copy Assigment Operator)
    MatrizDinamica &operator=(const MatrizDinamica<T> &other)
    {
        if (this != &other)
        {
            // 1. Eliminar matriz actual
            this->deleteMatriz(this->datos, this->filas, this->columnas);
            this->datos = nullptr;

            // 2. Crear copia de la matriz que contiene el objeto source
            this->datos = this->copyMatriz(other.getDatos(), other.getFilas(), other.getColumnas());

            // 3. Actualizar filas y columnas
            this->filas = other.getFilas();
            this->columnas = other.getColumnas();
        }
        return *this;
    }
    // Mover los datos de un objeto a un nuevo objeto (Move Constructor)
    MatrizDinamica(MatrizDinamica<T> &&other) noexcept
        // 1. Asignar los datos del otro objeot al objeto nuevo.
        : datos(other.datos), filas(other.filas), columnas(other.columnas)
    {
        // 2. Desenlazar/eliminar los datos del otro objeto.
        other.datos = nullptr;
        other.filas = 0;
        other.columnas = 0;
    }

    // Mover los datos de otro objeto a al objeto. (Move Assigment Operator)
    MatrizDinamica &operator=(MatrizDinamica<T> &&other) noexcept
    {
        if (this != &other)
        {
            // 1. Eliminar datos actuales
            deleteMatriz(this->datos, this->filas, this->columnas);

            // 2. Mover los datos del otro objeto a este objeto.
            this->datos = other.datos;
            this->filas = other.filas;
            this->columnas = other.columnas;

            // 3. Desenlazar/eliminar los datos del otro objeto.
            other.datos = nullptr;
            other.filas = 0;
            other.columnas = 0;
        }
        return *this;
    }

    void redimensionar(int new_filas, int new_columnas)
    {
        // validar tamaño positivo
        if (new_filas < 0 || new_columnas < 0)
            throw std::invalid_argument("No es posible una medida negativo");

        // 1. Crear nueva matriz e inicializar con valores 0
        T **new_datos = createMatiz(new_filas, new_columnas);

        // 2. Copiar valores originales a la nueva matriz

        // 2.1 Definir cual es el limite a copiar
        int max_cpy_filas = (new_filas > this->filas) ? this->filas : new_filas;
        int max_cpy_columnas = (new_columnas > this->columnas) ? this->columnas : new_columnas;

        // 2.2 Copiar valores
        for (int i = 0; i < max_cpy_filas; i++)
        {
            for (int j = 0; j < max_cpy_columnas; j++)
                new_datos[i][j] = this->datos[i][j];
        }

        // 4. Eliminar anterior matriz
        this->deleteMatriz(this->datos, this->filas, this->columnas);
        this->datos = nullptr;

        // 5. Remplazamos el puntero
        this->datos = new_datos;

        // 6. Despunteamos el auxiliar
        new_datos = nullptr;

        // 7. actualizamos medidas
        this->filas = new_filas;
        this->columnas = new_columnas;
    }

    template <typename U, typename P>
    static MatrizDinamica<T> *multiplicar(const MatrizDinamica<U> &A, const MatrizDinamica<P> &B)
    {
        // Validar si los tipos de dato se permiten operaciones.

        // 1. Validar si se pueden multiplicar
        if (A.getColumnas() != B.getFilas())
        {
            throw std::invalid_argument("Las matrices no se pueden multiplicar");
        }

        // Formula  A(m x n) * B(n x p) = C(m x p)
        int m = A.getFilas();
        int n = A.getColumnas();
        int p = B.getColumnas();

        // 2. Crear matriz auxiliar con las nuevas medidas
        MatrizDinamica<T> *C = new MatrizDinamica<T>(m, p);

        // 3. Intentar Realizar multiplicacion
        try
        {

            for (int i = 0; i < C->getFilas(); i++)
            {
                for (int j = 0; j < C->getColumnas(); j++)
                {
                    T result = 0;
                    for (int k = 0; k < n; k++)
                    {
                        result += A.getDatos()[i][k] * B.getDatos()[k][j];
                    }
                    C->getDatos()[i][j] = result;
                }
            }
        }
        catch (const std::exception &e)
        {
            // Ante algun fallo Informar del error.
            throw std::invalid_argument("Lost tipos de datos son imposibles de multiplicar");
        }
        return C;
    }

private:
    void deleteMatriz(T **matriz, int filas, int columnas = 0)
    {
        for (int i = 0; i < filas; i++)
            delete[] matriz[i];
        delete[] matriz;
    }

    T **createMatiz(int filas, int columnas)
    {
        // intentar crear una matriz
        try
        {
            // 1. Crear nueva matriz e inicializar con valores 0
            T **matriz = new T *[filas];
            for (int i = 0; i < filas; i++)
            {
                matriz[i] = new T[columnas];
                for (int j = 0; j < columnas; j++)
                    matriz[i][j] = 0;
            };
            return matriz;
        }
        catch (std::bad_alloc)
        {
            // logger->log("Not enough memory to create the video layer.");
            throw;
        }
    };
    T **copyMatriz(T **matriz, int filas, int columnas)
    {
        T **matriz_cpy = createMatiz(filas, columnas);
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                matriz_cpy[i][j] = matriz[i][j];
            }
        }
        return matriz_cpy;
    }
};