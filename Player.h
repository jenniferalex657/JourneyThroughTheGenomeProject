#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
    private:
        std::string _name;
        int _experience;
        int _accuracy;
        int _efficiency;
        int _insight;
        int _discoveryPoints;
        int _advisor; 
        int _pathType; 
        bool _advisorProtectionActive;

    public:
        Player();
        Player(std::string name, int experience, int accuracy, int efficiency, int insight, int discovery);

        void setAdvisor(int aid);
        int getAdvisor() const;
        void setPathType(int p);
        int getPathType() const;

        std::string getName() const;
        int getExperience() const;
        int getAccuracy() const;
        int getEfficiency() const;
        int getInsight() const;
        int getDiscoveryPoints() const;

        void addDiscoveryPoints(int delta);
        void addAccuracy(int delta);
        void addEfficiency(int delta);
        void addInsight(int delta);

        void enforceMinimumTraits();
        void enableAdvisorProtection();
        bool hasAdvisorProtection() const;
        void disableAdvisorProtection();

        std::string toString() const;
        std::string advisorName() const;
};

#endif
