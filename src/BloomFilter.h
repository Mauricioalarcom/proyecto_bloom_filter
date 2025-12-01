#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <vector>
#include <string>
#include <cstdint>

// Clase que implementa un Bloom Filter para URLs maliciosas.
// Utiliza un vector de booleanos para almacenar los bits y múltiples funciones hash
// simuladas para verificar la pertenencia de un elemento.
class BloomFilter {

public:
    // Constructor
    // m: tamaño del filtro (número de bits)
    // k: número de funciones hash a utilizar
    BloomFilter(size_t m, size_t k);

    // Inserta una URL en el Bloom Filter.
    // url: la cadena que representa la URL a insertar.
    void insert(const std::string& url);

    // Verifica si una URL posiblemente está en el conjunto.
    // Retorna true si la URL podría estar (posible falso positivo).
    // Retorna false si la URL definitivamente NO está.
    bool possiblyContains(const std::string& url) const;

    // Limpia el filtro (pone todos los bits a 0)
    void clear();

    // Getters para estadísticas
    size_t getSize() const { return m; }
    size_t getHashCount() const { return k; }

private:
    std::vector<bool> bitArray; // Vector de bits para el filtro
    size_t m; // Tamaño del filtro (bits)
    size_t k; // Número de funciones hash

    // Genera los k índices para una URL dada.
    // Utiliza double hashing para simular k funciones hash independientes.
    std::vector<size_t> getHashIndices(const std::string& url) const;
};

#endif // BLOOMFILTER_H
