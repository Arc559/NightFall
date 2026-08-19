#include "nightfall.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace nightfall {
namespace {

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

std::vector<WeaponData> makeWeapons() {
    return {
        {"Falcon-9", WeaponClass::AssaultRifle, {38, 720, 30, 2.4f, 250.0f, "Low", 35, 0, 0, "2.4 s"}},
        {"Tempest-11", WeaponClass::AssaultRifle, {34, 820, 30, 2.2f, 230.0f, "Medium", 30, 0, 0, "2.2 s"}},
        {"Sentinel-14", WeaponClass::AssaultRifle, {36, 760, 30, 2.3f, 240.0f, "Very Low", 38, 0, 0, "2.3 s"}},
        {"Titan-12", WeaponClass::BattleRifle, {52, 420, 20, 2.8f, 320.0f, "High", 50, 0, 0, "2.8 s"}},
        {"Vanguard-21", WeaponClass::BattleRifle, {46, 520, 20, 2.7f, 300.0f, "Medium", 45, 0, 0, "2.7 s"}},
        {"Longstrike-18", WeaponClass::BattleRifle, {49, 460, 20, 2.9f, 310.0f, "Low", 55, 0, 0, "2.9 s"}},
        {"Hammer-90", WeaponClass::HeavyRifle, {58, 360, 15, 3.0f, 340.0f, "Very High", 60, 0, 0, "3.0 s"}},
        {"Comet-9", WeaponClass::Carbine, {34, 780, 30, 2.2f, 220.0f, "Low", 30, 0, 0, "2.2 s"}},
        {"Ranger-11", WeaponClass::Carbine, {37, 690, 30, 2.4f, 240.0f, "Medium", 40, 0, 0, "2.4 s"}},
        {"Scout-5", WeaponClass::Carbine, {35, 750, 30, 2.3f, 220.0f, "Very Low", 34, 0, 0, "2.3 s"}},
        {"Spectre-7", WeaponClass::SMG, {29, 930, 35, 2.0f, 180.0f, "Medium", 20, 0, 0, "2.0 s"}},
        {"Phantom-30", WeaponClass::SMG, {31, 820, 30, 2.1f, 190.0f, "Low", 24, 0, 0, "2.1 s"}},
        {"Viper-6", WeaponClass::SMG, {25, 1050, 40, 2.3f, 170.0f, "High", 18, 0, 0, "2.3 s"}},
        {"Raptor-16", WeaponClass::SMG, {20, 1270, 50, 2.3f, 170.0f, "Very High", 20, 0, 0, "2.3 s"}},
        {"Atlas-80", WeaponClass::LMG, {41, 700, 75, 5.5f, 420.0f, "Medium", 45, 0, 0, "5.5 s"}},
        {"Bulldog-12", WeaponClass::Shotgun, {18, 85, 7, 0.0f, 0.0f, "N/A", 9, 8, 18, "Shell by shell"}},
        {"Warden-8", WeaponClass::Shotgun, {16, 70, 8, 0.0f, 0.0f, "N/A", 11, 10, 16, "Shell by shell"}},
        {"Vex-45", WeaponClass::Pistol, {48, 320, 12, 1.7f, 150.0f, "N/A", 0, 0, 0, "1.7 s"}},
        {"Slate-12", WeaponClass::Pistol, {40, 420, 15, 1.6f, 150.0f, "N/A", 0, 0, 0, "1.6 s"}},
        {"Brim-22", WeaponClass::Pistol, {35, 500, 18, 1.5f, 140.0f, "N/A", 0, 0, 0, "1.5 s"}},
        {"Forge-9", WeaponClass::Pistol, {55, 250, 8, 2.0f, 170.0f, "N/A", 0, 0, 0, "2.0 s"}},
        {"Dusk-19", WeaponClass::Pistol, {38, 450, 16, 1.6f, 145.0f, "N/A", 0, 0, 0, "1.6 s"}},
        {"Sable-11", WeaponClass::Pistol, {42, 380, 13, 1.7f, 150.0f, "N/A", 0, 0, 0, "1.7 s"}},
        {"Echo-5", WeaponClass::Pistol, {36, 480, 15, 1.5f, 140.0f, "N/A", 0, 0, 0, "1.5 s"}},
        {"Ember-7", WeaponClass::Pistol, {44, 340, 12, 1.7f, 150.0f, "N/A", 0, 0, 0, "1.7 s"}},
        {"Vector-8", WeaponClass::MachinePistol, {24, 1100, 25, 1.8f, 170.0f, "N/A", 15, 0, 0, "1.8 s"}},
        {"Marshal-6", WeaponClass::Revolver, {78, 180, 6, 2.5f, 180.0f, "N/A", 0, 0, 0, "2.5 s"}}
    };
}

std::vector<OperatorLoadout> makeRaiders() {
    return {
        {"Ashen", "Raiders", {"Falcon-9", "Bulldog-12"}, {"Vex-45", "Vector-8"}},
        {"Morrow", "Raiders", {"Spectre-7", "Comet-9"}, {"Slate-12", "Dusk-19"}},
        {"Vex", "Raiders", {"Tempest-11", "Vanguard-21"}, {"Marshal-6", "Brim-22"}},
        {"Wisp", "Raiders", {"Comet-9", "Phantom-30"}, {"Brim-22", "Echo-5"}},
        {"Flint", "Raiders", {"Raptor-16", "Atlas-80"}, {"Forge-9", "Ember-7"}}
    };
}

std::vector<OperatorLoadout> makeRebellions() {
    return {
        {"Bastion", "Rebellions", {"Hammer-90", "Warden-8"}, {"Forge-9", "Dusk-19"}},
        {"Frostbyte", "Rebellions", {"Sentinel-14", "Viper-6"}, {"Sable-11", "Slate-12"}},
        {"Echo", "Rebellions", {"Viper-6", "Scout-5"}, {"Echo-5", "Brim-22"}},
        {"Oracle", "Rebellions", {"Scout-5", "Longstrike-18"}, {"Dusk-19", "Vex-45"}},
        {"Graviton", "Rebellions", {"Titan-12", "Ranger-11"}, {"Vector-8", "Marshal-6"}},
        {"Sentry", "Rebellions", {"Raptor-16", "Phantom-30"}, {"Vector-8", "Echo-5"}}
    };
}

GlobalStats makeGlobalStats() {
    return GlobalStats{};
}

}  // namespace

Weapon::Weapon(const WeaponData& weaponData)
    : data_(weaponData), ammoInMagazine_(weaponData.stats.magazine), reserveAmmo_(weaponData.stats.magazine * 2) {}

const std::string& Weapon::name() const { return data_.name; }
WeaponClass Weapon::weaponClass() const { return data_.weaponClass; }
int Weapon::ammoInMagazine() const { return ammoInMagazine_; }
int Weapon::reserveAmmo() const { return reserveAmmo_; }
bool Weapon::isAds() const { return ads_; }
void Weapon::setAds(bool ads) { ads_ = ads; }

bool Weapon::fire() {
    if (ammoInMagazine_ <= 0) {
        return false;
    }
    --ammoInMagazine_;
    return true;
}

void Weapon::reload() {
    if (ammoInMagazine_ == data_.stats.magazine || reserveAmmo_ <= 0) {
        return;
    }

    int needed = data_.stats.magazine - ammoInMagazine_;
    int loaded = std::min(needed, reserveAmmo_);
    ammoInMagazine_ += loaded;
    reserveAmmo_ -= loaded;
}

DamageResult Weapon::simulateDamage(HitRegion region, double distanceMeters, int currentHealth) const {
    DamageResult result;
    result.headshot = region == HitRegion::Head;
    result.limbHit = region == HitRegion::Limb;

    double distanceFalloff = 1.0;
    if (distanceMeters > data_.stats.effectiveRange) {
        distanceFalloff = 0.45;
    } else if (distanceMeters > data_.stats.effectiveRange * 0.75) {
        distanceFalloff = 0.7;
    }

    double baseDamage = static_cast<double>(data_.stats.damage) * distanceFalloff;
    const GlobalStats& stats = globalStats();

    if (result.headshot) {
        baseDamage *= stats.headshotMultiplier;
    } else if (result.limbHit) {
        baseDamage *= stats.limbMultiplier;
    }

    result.damage = static_cast<int>(std::round(baseDamage));
    result.remainingHealth = std::max(0, currentHealth - result.damage);
    result.kill = result.remainingHealth <= 0;
    return result;
}

const WeaponData& Weapon::data() const { return data_; }

NightfallGame::NightfallGame() : stats_(globalStats()) {}

std::vector<OperatorLoadout> NightfallGame::raidRoster() const {
    return std::vector<OperatorLoadout>(raiders().begin(), raiders().end());
}

std::vector<OperatorLoadout> NightfallGame::rebellionRoster() const {
    return std::vector<OperatorLoadout>(rebellions().begin(), rebellions().end());
}

std::optional<Weapon> NightfallGame::makeWeapon(const std::string& name) const {
    auto weaponData = findWeapon(name);
    if (!weaponData.has_value()) {
        return std::nullopt;
    }
    return Weapon(*weaponData);
}

std::optional<OperatorLoadout> NightfallGame::getOperator(const std::string& name, const std::string& faction) const {
    const auto& roster = faction == "Raiders" ? raiders() : rebellions();
    for (const auto& oper : roster) {
        if (oper.name == name) {
            return oper;
        }
    }
    return std::nullopt;
}

std::string NightfallGame::regionName(HitRegion region) const {
    return toString(region);
}

const std::vector<WeaponData>& weapons() {
    static const std::vector<WeaponData> list = makeWeapons();
    return list;
}

const std::vector<OperatorLoadout>& raiders() {
    static const std::vector<OperatorLoadout> list = makeRaiders();
    return list;
}

const std::vector<OperatorLoadout>& rebellions() {
    static const std::vector<OperatorLoadout> list = makeRebellions();
    return list;
}

const GlobalStats& globalStats() {
    static const GlobalStats stats = makeGlobalStats();
    return stats;
}

std::optional<WeaponData> findWeapon(const std::string& name) {
    auto target = lower(name);
    for (const auto& weapon : weapons()) {
        if (lower(weapon.name) == target) {
            return weapon;
        }
    }
    return std::nullopt;
}

std::string toString(WeaponClass weaponClass) {
    switch (weaponClass) {
        case WeaponClass::AssaultRifle: return "Assault Rifle";
        case WeaponClass::BattleRifle: return "Battle Rifle";
        case WeaponClass::HeavyRifle: return "Heavy Rifle";
        case WeaponClass::Carbine: return "Carbine";
        case WeaponClass::SMG: return "SMG";
        case WeaponClass::LMG: return "LMG";
        case WeaponClass::Shotgun: return "Shotgun";
        case WeaponClass::Pistol: return "Pistol";
        case WeaponClass::MachinePistol: return "Machine Pistol";
        case WeaponClass::Revolver: return "Revolver";
    }
    return "Unknown";
}

std::string toString(HitRegion region) {
    switch (region) {
        case HitRegion::Head: return "Head";
        case HitRegion::Torso: return "Torso";
        case HitRegion::Limb: return "Limb";
    }
    return "Unknown";
}

void printWeaponTable() {
    std::cout << "NightFall Arsenal\n";
    std::cout << "=================\n";
    for (const auto& weapon : weapons()) {
        std::cout << weapon.name << " | " << toString(weapon.weaponClass)
                  << " | DMG " << weapon.stats.damage
                  << " | RPM " << weapon.stats.fireRate
                  << " | MAG " << weapon.stats.magazine
                  << " | RELOAD " << weapon.stats.reloadDescription
                  << " | ADS " << weapon.stats.adsMilliseconds << " ms"
                  << " | RECOIL " << weapon.stats.recoil
                  << " | RANGE " << weapon.stats.effectiveRange << " m\n";
    }
}

void printOperatorRoster(const std::vector<OperatorLoadout>& operators) {
    for (const auto& operatorEntry : operators) {
        std::cout << operatorEntry.name << " (" << operatorEntry.faction << ")\n";
        std::cout << "  Primary: ";
        for (size_t i = 0; i < operatorEntry.primaryWeapons.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << operatorEntry.primaryWeapons[i];
        }
        std::cout << "\n";

        std::cout << "  Secondary: ";
        for (size_t i = 0; i < operatorEntry.secondaryWeapons.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << operatorEntry.secondaryWeapons[i];
        }
        std::cout << "\n\n";
    }
}

}  // namespace nightfall
