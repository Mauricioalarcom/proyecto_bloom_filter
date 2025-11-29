# Detector de URLs Maliciosas con Bloom Filter

Este proyecto implementa un **Bloom Filter** en C++ para detectar URLs maliciosas de manera eficiente en términos de memoria y tiempo. Se compara su rendimiento contra una estructura de datos estándar (`std::unordered_set`).

## Descripción del Proyecto
El programa carga una lista de URLs "maliciosas" (blacklist) en un Bloom Filter. Luego, procesa una lista de consultas para verificar si cada URL está en la lista negra.
- **Bloom Filter**: Estructura probabilística que puede dar falsos positivos pero nunca falsos negativos. Muy eficiente en memoria.
- **std::unordered_set**: Estructura exacta que almacena todas las claves. Mayor consumo de memoria.

## Requisitos
- Compilador compatible con C++17 (g++, clang++, MSVC).
- CMake 3.17 o superior (opcional, se puede compilar manualmente).
- Python 3 (para generar datos de prueba).

## Estructura del Repositorio
```
bloom-filter-malicious-urls/
├─ src/
│  ├─ BloomFilter.h      // Definición de la clase BloomFilter
│  ├─ BloomFilter.cpp    // Implementación de la clase
│  └─ main.cpp           // Programa principal y pruebas
├─ tests/
│  ├─ generate_data.py   // Script para generar URLs de prueba
│  ├─ blacklist_urls.txt // (Generado) Lista de URLs maliciosas
│  └─ test_queries.txt   // (Generado) Lista de consultas
├─ results/              // Carpeta para resultados (opcional)
├─ CMakeLists.txt        // Configuración de CMake
└─ README.md             // Este archivo
```

## Compilación y Ejecución

### 1. Generar datos de prueba
Primero, ejecuta el script de Python para crear los archivos de texto en `tests/`:
```bash
python3 tests/generate_data.py
```

### 2. Compilar con CMake
```bash
mkdir build
cd build
cmake ..
make
```
O compilar manualmente con g++:
```bash
g++ src/*.cpp -std=c++17 -O2 -o bloom_filter
```

### 3. Ejecutar
El programa recibe dos argumentos: el archivo de blacklist y el archivo de consultas.
```bash
./bloom_filter tests/blacklist_urls.txt tests/test_queries.txt
```
(Si usaste CMake y build, ajusta la ruta a `../tests/...`)

## Interpretación de Resultados
El programa mostrará:
1.  **Tiempo de inserción**: Comparación entre Bloom Filter y unordered_set.
2.  **Tiempo de consulta**: Comparación de velocidad de búsqueda.
3.  **Falsos Positivos**: Cuántas veces el Bloom Filter dijo "posiblemente" para una URL que no estaba en la lista.
4.  **Memoria**: Estimación del uso de memoria del Bloom Filter.

## Autores
Implementado por: 
Diego Alarcon,
Valentino Contreras,
Lucia Castro,
Benjamin Suarez.
# proyecto_bloom_filter
