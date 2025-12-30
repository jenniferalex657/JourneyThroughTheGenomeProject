#include "Player.h"
#include <sstream>

Player::Player() {
    _name = "Unknown";
    _experience = 0;
    _accuracy = 100;
    _efficiency = 100;
    _insight = 100;
    _discoveryPoints = 20000;
    _advisor = 0;
    _pathType = 0;
    _advisorProtectionActive = false;
}

Player::Player(std::string name, int experience, int accuracy, int efficiency, int insight, int discovery) {
    _name = name;
    _experience = experience;
    _accuracy = accuracy;
    _efficiency = efficiency;
    _insight = insight;
    _discoveryPoints = discovery;
    _advisor = 0;
    _pathType = 0;
    _advisorProtectionActive = false;
}

void Player::setAdvisor(int aid) {
    _advisor = aid;
}
int Player::getAdvisor() const { return _advisor; }

void Player::setPathType(int p) { _pathType = p; }
int Player::getPathType() const { return _pathType; }

std::string Player::getName() const { return _name; }
int Player::getExperience() const { return _experience; }
int Player::getAccuracy() const { return _accuracy; }
int Player::getEfficiency() const { return _efficiency; }
int Player::getInsight() const { return _insight; }
int Player::getDiscoveryPoints() const { return _discoveryPoints; }

void Player::addDiscoveryPoints(int delta) {
    _discoveryPoints += delta;
}

void Player::addAccuracy(int delta) {
    _accuracy += delta;
    if (_accuracy < 100) _accuracy = 100;
}

void Player::addEfficiency(int delta) {
    _efficiency += delta;
    if (_efficiency < 100) _efficiency = 100;
}

void Player::addInsight(int delta) {
    _insight += delta;
    if (_insight < 100) _insight = 100;
}

void Player::enforceMinimumTraits() {
    if (_accuracy < 100) _accuracy = 100;
    if (_efficiency < 100) _efficiency = 100;
    if (_insight < 100) _insight = 100;
}

void Player::enableAdvisorProtection() {
    _advisorProtectionActive = true;
}

bool Player::hasAdvisorProtection() const { return _advisorProtectionActive; }

void Player::disableAdvisorProtection() {
    _advisorProtectionActive = false;
}

std::string Player::toString() const {
    std::ostringstream oss;
    oss << "Name: " << _name << " | Exp: " << _experience << " | Acc: " << _accuracy
        << " | Eff: " << _efficiency << " | Ins: " << _insight << " | DP: " << _discoveryPoints;
    return oss.str();
}

std::string Player::advisorName() const {
    switch (_advisor) {
        case 1: return "Dr. Aliquot";
        case 2: return "Dr. Assembler";
        case 3: return "Dr. Pop-Gen";
        case 4: return "Dr. Bio-Script";
        case 5: return "Dr. Loci";
        default: return "None";
    }
}
