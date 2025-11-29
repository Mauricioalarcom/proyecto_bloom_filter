#include "BloomFilter.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// Función auxiliar para leer líneas de un archivo
vector<string> loadUrls(const string &filename) {
  vector<string> urls;
  ifstream file(filename);
  string line;
  if (file.is_open()) {
    while (getline(file, line)) {
      if (!line.empty()) {
        urls.push_back(line);
      }
    }
    file.close();
  } else {
    cerr << "Error: No se pudo abrir el archivo " << filename << endl;
  }
  return urls;
}

int main(int argc, char *argv[]) {
  // Verificar argumentos
  if (argc < 3) {
    cerr << "Uso: " << argv[0] << " <blacklist_file> <query_file>"
              << endl;
    return 1;
  }

  string blacklistFile = argv[1];
  string queryFile = argv[2];

  cout << "Cargando URLs..." << endl;
  vector<string> blacklist = loadUrls(blacklistFile);
  vector<string> queries = loadUrls(queryFile);

  if (blacklist.empty() || queries.empty()) {
    return 1;
  }

  size_t n = blacklist.size();

  // Parámetros del Bloom Filter
  // m (bits) y k (hashes) se pueden calcular para una tasa de error deseada
  // (p). m = -(n * ln(p)) / (ln(2)^2) k = (m/n) * ln(2) Para p = 0.01 (1%), m
  // ~ 9.58 * n, k ~ 7 Usaremos valores aproximados para n = 50,000
  size_t m = n * 10; // ~500,000 bits
  size_t k = 7;

  cout << "Configuración Bloom Filter: n=" << n << ", m=" << m
            << ", k=" << k << endl;

  // --- BLOOM FILTER ---
  cout << "\n--- Bloom Filter ---" << endl;
  BloomFilter bf(m, k);

  // Medir tiempo de inserción
  auto start = chrono::high_resolution_clock::now();
  for (const auto &url : blacklist) {
    bf.insert(url);
  }
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> insertTimeBF = end - start;
  cout << "Tiempo de inserción: " << insertTimeBF.count() << " ms"
            << endl;

  // Medir tiempo de consulta
  int positivesBF = 0;
  start = chrono::high_resolution_clock::now();
  for (const auto &url : queries) {
    if (bf.possiblyContains(url)) {
      positivesBF++;
    }
  }
  end = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> queryTimeBF = end - start;
  cout << "Tiempo de consulta (" << queries.size()
            << " URLs): " << queryTimeBF.count() << " ms" << endl;
  cout << "Positivos detectados: " << positivesBF <<  endl;

  // --- UNORDERED SET (Ground Truth) ---
  cout << "\n--- unordered_set ---" << endl;
  unordered_set<string> set;

  // Medir tiempo de inserción
  start = chrono::high_resolution_clock::now();
  for (const auto &url : blacklist) {
    set.insert(url);
  }
  end = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> insertTimeSet = end - start;
  cout << "Tiempo de inserción: " << insertTimeSet.count() << " ms"
            << endl;

  // Medir tiempo de consulta y contar verdaderos positivos
  int truePositives = 0;
  start = chrono::high_resolution_clock::now();
  for (const auto &url : queries) {
    if (set.find(url) != set.end()) {
      truePositives++;
    }
  }
  end = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> queryTimeSet = end - start;
  cout << "Tiempo de consulta (" << queries.size()
            << " URLs): " << queryTimeSet.count() << " ms" << endl;
  cout << "Verdaderos positivos: " << truePositives << endl;

  // --- ESTADÍSTICAS ---
  cout << "\n--- Comparación ---" << endl;
  int falsePositives = positivesBF - truePositives;
  double fpRate =
      (double)falsePositives / (queries.size() - truePositives) * 100.0;

  cout << "Falsos Positivos: " << falsePositives << endl;
  cout << "Tasa de Falsos Positivos (Empírica): " << fixed
            << setprecision(4) << fpRate << "%" << endl;

  // Memoria aproximada (muy básica)
  // Bloom Filter: m bits = m / 8 bytes
  // Unordered Set: Overhead por nodo + string size. Muy variable, pero mucho
  // mayor.
  size_t memBF = m / 8;
  cout << "Memoria Bloom Filter (bits): " << memBF << " bytes"
            << endl;
  cout << "Memoria Bloom Filter es significativamente menor que "
               "unordered_set (que guarda copias de los strings)."
            << endl;

  return 0;
}
