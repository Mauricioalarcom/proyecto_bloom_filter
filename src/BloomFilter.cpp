#include "BloomFilter.h"
#include <cmath>
#include <functional>

// Constructor: Inicializa el array de bits y guarda los parámetros m y k.
BloomFilter::BloomFilter(size_t m, size_t k) : m(m), k(k) {
  bitArray.resize(m, false); // Inicializamos todos los bits en falso (0)
}

// Inserta una URL marcando los bits correspondientes en true.
void BloomFilter::insert(const std::string &url) {
  std::vector<size_t> indices = getHashIndices(url);
  for (size_t index : indices) {
    bitArray[index] = true;
  }
}

// Verifica si una URL está en el filtro.
// Si todos los bits correspondientes están en true, retorna true (posiblemente
// contiene). Si al menos un bit está en false, retorna false (definitivamente
// no contiene).
bool BloomFilter::possiblyContains(const std::string &url) const {
  std::vector<size_t> indices = getHashIndices(url);
  for (size_t index : indices) {
    if (!bitArray[index]) {
      return false; // Si un bit es 0, el elemento seguro no está.
    }
  }
  return true; // Todos los bits son 1, el elemento podría estar.
}

// Genera k índices usando double hashing.
// h_i(x) = (h1(x) + i * h2(x)) % m
// Esto evita tener que calcular k hashes costosos diferentes.
std::vector<size_t> BloomFilter::getHashIndices(const std::string &url) const {
  std::vector<size_t> indices;
  indices.reserve(k);

  // Hash 1: std::hash estándar
  size_t h1 = std::hash<std::string>{}(url);

  // Hash 2: Una variación simple para obtener un segundo valor
  // pseudo-independiente. Podemos usar otro algoritmo o simplemente manipular
  // h1. Aquí usaremos una segunda pasada con un "salt" simple o un algoritmo
  // ligero. Para simplicidad y eficiencia, haremos un hash de la longitud + el
  // primer caracter + h1.
  size_t h2 = h1 ^ (h1 >> 16); // Un simple shift-xor como segundo componente

  for (size_t i = 0; i < k; ++i) {
    // Fórmula de double hashing
    size_t combinedHash = h1 + i * h2;
    indices.push_back(combinedHash % m);
  }

  return indices;
}
