#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <cstdlib>
#include<ctime>
 
using namespace std;
 
struct KDNode {
    vector<double> point;
    int id;
    KDNode* left;
    KDNode* right;
    KDNode(const vector<double>& point, int identificador) : point(point), id(identificador), left(nullptr), right(nullptr) {}
};
 
class KDTreeVQ {
    KDNode* root;
    unordered_map<int, vector<double>> codebook;
 
    KDNode* insertRec(KDNode* node, const vector<double>& point, int id, unsigned depth) {
        if (node == nullptr) return new KDNode(point, id);
        unsigned cd = depth % point.size();
        if (point[cd] < node->point[cd])
            node->left = insertRec(node->left, point, id, depth + 1);
        else
            node->right = insertRec(node->right, point, id, depth + 1);
        return node;
    }
 
    KDNode* nearestRec(KDNode* node, const vector<double>& point, unsigned depth, KDNode* best, double& bestDist) {
        if (node == nullptr) return best;
        double dist = distanceSquared(point, node->point);
        if (dist < bestDist) {
            bestDist = dist;
            best = node;
        }
        unsigned cd = depth % point.size();
        KDNode* goodSide = (point[cd] < node->point[cd]) ? node->left : node->right;
        KDNode* badSide = (point[cd] < node->point[cd]) ? node->right : node->left;
        best = nearestRec(goodSide, point, depth + 1, best, bestDist);
        if (bestDist > pow(point[cd] - node->point[cd], 2)) {
            best = nearestRec(badSide, point, depth + 1, best, bestDist);
        }
        return best;
    }
 
    double distanceSquared(const vector<double>& p1, const vector<double>& p2) {
        double dist = 0;
        for (size_t i = 0; i < p1.size(); ++i) {
            dist += pow(p1[i] - p2[i], 2);
        }
        return dist;
    }
 
public:
    KDTreeVQ() : root(nullptr) {}
 
    void insert(const vector<double>& point, int id) {
        root = insertRec(root, point, id, 0);
        codebook[id] = point;
    }
 
    KDNode* nearestNeighbour(const vector<double>& point) {
        double bestDist = numeric_limits<double>::max();
        return nearestRec(root, point, 0, nullptr, bestDist);
    }
 
    int encode(const vector<double>& point) {
        KDNode* nearest = nearestNeighbour(point);
        return nearest->id;
    }
 
    vector<double> decode(int id) {
        if (codebook.find(id) != codebook.end()) {
            return codebook[id];
        }
        return {};
    }
};
 
int main() {
    srand(static_cast<unsigned>(time(0)));
 
    KDTreeVQ tree;
 
    const int numPoints = 10000;
    for (int i = 0; i < numPoints; ++i) {
        vector<double> point = { static_cast<double>(rand() % 100), static_cast<double>(rand() % 100) };
        tree.insert(point, i);
        cout << "id " << i << ": (" << point[0] << ", " << point[1] << ")" << endl;
    }
 
    vector<double> target = {23.4, 45.7};
 
    int idPrototipo = tree.encode(target);
 
    vector<double> puntoDecodificado = tree.decode(idPrototipo);
 
    cout << "Identificador del prototipo más cercano para (" << target[0] << ", " << target[1] << ") es: " << idPrototipo << endl;
    cout << "Prototipo decodificado para el id " << idPrototipo << " es: ("
         << puntoDecodificado[0] << ", " << puntoDecodificado[1] << ")" << endl;
 
    return 0;
}
