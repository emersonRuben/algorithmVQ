#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time() y clock()
 
using namespace std;
 
struct Vector {
    vector<double> coordinates;
    Vector(int k) : coordinates(k, 0) {}
    double distanceTo(const Vector& other) const {
        double sum = 0;
        for (size_t i = 0; i < coordinates.size(); ++i) {
            sum += pow(coordinates[i] - other.coordinates[i], 2);
         }
        return sqrt(sum);
    }
};

class VQ {
    vector<Vector> codebook;
 
public:
    VQ(int k, int numPrototypes) {
        // Inicializa la semilla solo una vez al crear un objeto VQ.
        srand(time(0));
 
        for (int i = 0; i < numPrototypes; ++i) {
            Vector prototype(k);
            // Generar valores aleatorios para los prototipos.
            for (int j = 0; j < k; ++j) {
                prototype.coordinates[j] = rand() % 100; // Valores entre 0 y 100.
            }
            codebook.push_back(prototype);
        }
    }
 
    int findNearestPrototype(const Vector& dataPoint) const {
        int nearest = 0;
        double minDist = dataPoint.distanceTo(codebook[0]);
        for (size_t i = 1; i < codebook.size(); ++i) {
            double dist = dataPoint.distanceTo(codebook[i]);
            if (dist < minDist) {
                minDist = dist;
                nearest = i;
            }
        }
        return nearest;
    }
 
    Vector decode(int prototypeId) const {
        // Retorna el prototipo correspondiente al identificador dado
        return codebook[prototypeId];
    }
 
    void printCodebook() const {
        for (const auto& vec : codebook) {
            for (double coord : vec.coordinates) {
                cout << coord << " ";
            }
            cout << endl;
        }
    }
};
 
int main() {
    srand(time(0)); // Inicializa la semilla del generador de números aleatorios.
 
    int k = 2; // Dimensiones
    int numPrototypes = 20; // Cantidad de prototipos en el codebook
    VQ vq(k, numPrototypes);
    vq.printCodebook();
 
    Vector dataPoint(k);
    dataPoint.coordinates[0] = 23.4;
    dataPoint.coordinates[1] = 45.7;
 
    // Medir el tiempo de codificación (búsqueda del prototipo más cercano)
    clock_t startEncode = clock();
    int nearest = vq.findNearestPrototype(dataPoint);
    clock_t endEncode = clock();
    double encodeTime = double(endEncode - startEncode) / CLOCKS_PER_SEC;
    cout << "Nearest prototype: " << nearest << endl;
 
    // Medir el tiempo de decodificación (recuperación del prototipo)
    clock_t startDecode = clock();
    Vector decodedPoint = vq.decode(nearest);
    clock_t endDecode = clock();
    double decodeTime = double(endDecode - startDecode) / CLOCKS_PER_SEC;
 
    cout << "Decoded data point: ";
    for (double coord : decodedPoint.coordinates) {
        cout << coord << " ";
    }
    cout << endl;
 
    // Mostrar los tiempos de codificación y decodificación
    cout << "Tiempo de codificación: " << encodeTime << " segundos." << endl;
    cout << "Tiempo de decodificación: " << decodeTime << " segundos." << endl;
 
    return 0;
}
