/*
    Problem Statemenr : https://docs.google.com/document/d/1MRqiEa1NgAQOR403ZYDcrVDwsW67k7taXCgDr0Z_CDI/edit?usp=sharing
*/

#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

class QNS {
    private:
        int powerLevel, durability, energyStorage, heatLevel;
    public:
        QNS(int P, int D, int E, int H) 
            : powerLevel(P), durability(D), energyStorage(E), heatLevel(H) {
                if(powerLevel > 5000) powerLevel = 5000;
                if(heatLevel < 0) heatLevel = 0;
            }
        QNS(const QNS& suit) {
            powerLevel = suit.powerLevel;
            durability = suit.durability;
            energyStorage = suit.energyStorage;
            heatLevel = suit.heatLevel;
        }
        QNS() : powerLevel(1000), durability(500), energyStorage(300), heatLevel(0) {}

        QNS& operator+(const QNS& Q) {
            powerLevel += Q.energyStorage;
            durability += Q.durability;
            energyStorage += Q.powerLevel;
           
            if(powerLevel > 5000) powerLevel = 5000;
            return *this;
        }

        QNS& operator-(int X) {
            durability -= X;
            energyStorage += X;
            heatLevel += X;
            return *this;
        }

        QNS& operator*(int X) {
            powerLevel += (powerLevel*X)/100;
            energyStorage += 5*X;
            heatLevel += X;
            
            if(powerLevel > 5000) powerLevel = 5000;
            return *this;
        }

        QNS& operator/(int X) {
            durability += X;
            heatLevel -= X;
           
            if(heatLevel < 0) heatLevel = 0;
            return *this;
        }

        void boostPower(int factor) {
            (*this) * factor;
        }

        void boostPower(const QNS& othersuit) {
            (*this) + othersuit;
        }

        bool operator==(const QNS& othersuit) {
            return (powerLevel = othersuit.powerLevel && durability == othersuit.durability);
        }

        bool operator<(const QNS& othersuit) {
            return (powerLevel + durability < othersuit.powerLevel + othersuit.durability);
        }

        int getPowerLevel() { return powerLevel; }
        int getDurability() { return durability; }
        int getEnergyStored() { return energyStorage; }
        int getHeatLevel() { return heatLevel; }

        bool isDestroyed() { return durability <= 0; }
        bool shutDown() { return heatLevel > 500; }

};

class Avenger {
    private:
        string name;
        QNS suit;
        int attackStrength;
    public:
        Avenger(string avName, QNS avSuit, int strength) 
            : name(avName), suit(avSuit), attackStrength(strength) {}
        Avenger() {}

        void attack(Avenger& enemy) {
            enemy.suit - attackStrength;
        }

        bool upgradeSuit(queue<QNS>& suits) {
            if(suits.empty())  return false; 
            
            suit + suits.front();
            suits.pop();
            
            return true;
        }

        void repair(int X) {
            suit/X; 
        }

        void printStatus() {
            cout << name << " " << suit.getPowerLevel() << " " << suit.getDurability() << " " << suit.getEnergyStored() << " " << suit.getHeatLevel() << endl;
        }

        int getPowerLevel() { return suit.getPowerLevel(); }
        int getDurability() { return suit.getDurability(); }
        int getHeatLevel() { return suit.getHeatLevel(); }

        QNS& getSuit() { return suit; }
        string getname() { return name; }
        bool isActive() { return (!suit.isDestroyed() && !suit.shutDown()); }

};

class Battle {
    private:
        vector<Avenger> heroes, enemies;
        vector<string> battleLog;
        queue<QNS> suits;
    public:
        void startBattle() {
            string s;
            cin >> s;
            while(s != "End") {
                if(s == "Attack") {
                    string av1, av2;
                    cin >> av1 >> av2;
                    Avenger *attacker = getAvenger(av1);
                    Avenger *defender = getAvenger(av2);
                    
                    if(!attacker->isActive() && (defender->getDurability()>0)) continue;
                    attacker->attack(*defender);
                    
                    battleLog.push_back(av1 + " attacks " + av2);
                    if(defender->getDurability() < 0) battleLog.push_back(av2 + " suit destroyed");
                    else if(defender->getHeatLevel() > 500) battleLog.push_back(av2 + " suit overheated");
                }
                else if(s == "Repair") {
                    string name;
                    int x;
                    cin >> name >> x;

                    Avenger *AV = getAvenger(name);
                    AV->repair(x);
                    
                    battleLog.push_back(name + " repaired");
                }
                else if(s == "BoostPowerByFactor") {
                    string name;
                    int y;
                    cin >> name >> y;

                    Avenger *AV = getAvenger(name);
                    QNS *suit = &AV->getSuit();
                    suit->boostPower(y);
                   
                    battleLog.push_back(name + " boosted");
                    if(AV->getHeatLevel() > 500) battleLog.push_back(name + " suit overheated");
                }
                else if(s == "BoostPower") {
                    string name;
                    int P,D,E,H;
                    cin >> name >> P >> D >> E >> H;

                    Avenger *AV = getAvenger(name);
                    QNS *suit = &AV->getSuit();
                    suit->boostPower(QNS(P,D,E,H));

                    battleLog.push_back(name + " boosted");
                    if(AV->getHeatLevel() > 500) battleLog.push_back(name + " suit overheated");
                }
                else if(s == "AvengerStatus") {
                    string name;
                    cin >> name;
                    Avenger *AV = getAvenger(name);
                    AV->printStatus();
                }
                else if(s == "Upgrade") {
                    string name;
                    cin >> name;
                    Avenger *AV = getAvenger(name);

                    if(AV->upgradeSuit(suits)) battleLog.push_back(name + " upgraded");
                    else battleLog.push_back(name + " upgrade Fail");
                }
                else if(s == "PrintBattleLog") {
                    printBattleLog();
                }
                else if(s == "BattleStatus") {
                    if(Result() == 1) cout << "heroes are winning" << endl;
                    else if(Result() == -1) cout << "enemies are winning" << endl;
                    else cout << "tie" << endl;
                }
                cin >> s;
            }
        }
        void printBattleLog() {
            for(auto& x : battleLog) { cout << x << endl; }
        }

        int Result() {
            int heroes_win = 0, enemies_win = 0;
            for(auto& x : heroes) {
                if(x.getDurability() > 0)
                heroes_win += x.getPowerLevel() + x.getDurability();
            }
            for(auto& y : enemies) {
                if(y.getDurability() > 0)
                enemies_win += y.getPowerLevel() + y.getDurability();
            }

            if(heroes_win > enemies_win) return 1;
            else if(heroes_win < enemies_win) return -1;
            else return 0;
        }

        void addHero(Avenger hero) { heroes.push_back(hero); }
        void addEnemy(Avenger enemy) { enemies.push_back(enemy); }
        void addSuit(QNS suit) { suits.push(suit); }

        int getNoofSuits() { return suits.size(); }
        Avenger* getAvenger(string name) {
            for(auto& it : heroes) { if(it.getname() == name) return &it; }
            for(auto& it : enemies) { if(it.getname() == name) return &it; }
            return nullptr;
        }
        QNS* getSuit() {
            if(suits.empty()) return nullptr;
            QNS *temp = &suits.front();
            suits.pop();
            return temp;
        }
        
};

int main()
{
    Battle war;

    int k,n,m;
    cin >> k >> n >> m;

    for(int i=0; i<k; ++i) {
        int P,D,E,H;
        cin >> P >> D >> E >> H;
        war.addSuit(QNS(P,D,E,H));
    }

    for(int i=0; i<n+m; ++i) {
        string name;
        int attackStrength;
        cin >> name >> attackStrength;
        if(war.getNoofSuits() == 0) {
            cout << name << " is out of fight" << endl;
            continue;
        }
        QNS *suit = war.getSuit();
        
        if(i < n) { war.addHero(Avenger(name,*suit,attackStrength)); }
        else war.addEnemy(Avenger(name,*suit,attackStrength));
    }

    string s;
    cin >> s;
    if(s == "BattleBegin") { war.startBattle(); }

}