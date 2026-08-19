#ifndef NIGHTFALL_HPP
#define NIGHTFALL_HPP

#include <optional>
#include <string>
#include <vector>

namespace nightfall {

enum class WeaponClass {
    AssaultRifle,
    BattleRifle,
    HeavyRifle,
    Carbine,
    SMG,
    LMG,
    Shotgun,
    Pistol,
    MachinePistol,
    Revolver
};

enum class HitRegion {
    Head,
    Torso,
    Limb
};

struct WeaponStats {
    int damage = 0;
    int fireRate = 0;
    int magazine = 0;
    float reloadSeconds = 0.0f;
    float adsMilliseconds = 0.0f;
    std::string recoil = "Unknown";
    int effectiveRange = 0;
    int pellets = 0;
    int damagePerPellet = 0;
    std::string reloadDescription = "N/A";
};

struct WeaponData {
    std::string name;
    WeaponClass weaponClass;
    WeaponStats stats;
};

struct OperatorLoadout {
    std::string name;
    std::string faction;
    std::vector<std::string> primaryWeapons;
    std::vector<std::string> secondaryWeapons;
};

struct GlobalStats {
    int playerHealth = 100;
    double headshotMultiplier = 2.5;
    double limbMultiplier = 0.9;
    double walkSpeed = 5.5;
    double sprintSpeed = 7.2;
    double crouchSpeed = 2.8;
    double proneSpeed = 1.6;
    bool armorSystem = false;
    bool oneShotHeadshots = true;
};

struct DamageResult {
    int damage = 0;
    bool headshot = false;
    bool limbHit = false;
    bool kill = false;
    int remainingHealth = 100;
};

class Weapon {
public:
    explicit Weapon(const WeaponData& weaponData);

    const std::string& name() const;
    WeaponClass weaponClass() const;
    int ammoInMagazine() const;
    int reserveAmmo() const;
    bool isAds() const;
    void setAds(bool ads);
    bool fire();
    void reload();
    DamageResult simulateDamage(HitRegion region, double distanceMeters, int currentHealth) const;
    const WeaponData& data() const;

private:
    WeaponData data_;
    int ammoInMagazine_ = 0;
    int reserveAmmo_ = 0;
    bool ads_ = false;
};

class NightfallGame {
public:
    NightfallGame();

    std::vector<OperatorLoadout> raidRoster() const;
    std::vector<OperatorLoadout> rebellionRoster() const;
    std::optional<Weapon> makeWeapon(const std::string& name) const;
    std::optional<OperatorLoadout> getOperator(const std::string& name, const std::string& faction) const;
    std::string regionName(HitRegion region) const;

private:
    const GlobalStats& stats_;
};

const std::vector<WeaponData>& weapons();
const std::vector<OperatorLoadout>& raiders();
const std::vector<OperatorLoadout>& rebellions();
const GlobalStats& globalStats();
std::optional<WeaponData> findWeapon(const std::string& name);
std::string toString(WeaponClass weaponClass);
std::string toString(HitRegion region);
void printWeaponTable();
void printOperatorRoster(const std::vector<OperatorLoadout>& operators);

}  // namespace nightfall

#endif  // NIGHTFALL_HPP
