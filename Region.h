#ifndef REGION_H
#define REGION_H
#include <vector>
#include <string>
using namespace std;
class Region
{

    std::vector<int> neighbors;
    int id;
    int nbNeighbors;
    int superRegion;
    string owner;
    int armies;
    double startPriority;
    double roundThreat;
    double importance;
    public:
        Region();
        Region(int pId,int superRegion, double startPriority, double importance);
        virtual ~Region();
        void addNeighbors(int Neighbors);
        void setArmies(int nbArmies);
        void setOwner(string owner);
        int getArmies();
        string getOwner();
        int getSuperRegion();
        vector<int>& getNeighbors();
        void setStartPriority(double priority);
        double getStartPriority(void);
        double getThreat();
        void setThreat(double);
        void setImportance(double Importance);
        double getImportance();

    protected:
    private:
};

#endif // REGION_H
