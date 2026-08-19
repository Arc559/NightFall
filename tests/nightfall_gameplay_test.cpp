#include "nightfall.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

int main() {
    using namespace nightfall;

    const auto& weapons = nightfall::weapons();
    assert(!weapons.empty());

    auto falcon = findWeapon("Falcon-9");
    assert(falcon.has_value());
    assert(falcon->stats.damage == 38);
    assert(falcon->stats.fireRate == 720);

    auto unknown = findWeapon("Unknown-Weapon");
    assert(!unknown.has_value());

    const auto& raiders = nightfall::raiders();
    const auto& rebellions = nightfall::rebellions();
    assert(!raiders.empty());
    assert(!rebellions.empty());

    auto ashen = std::find_if(raiders.begin(), raiders.end(), [](const OperatorLoadout& op) {
        return op.name == "Ashen";
    });
    assert(ashen != raiders.end());
    assert(ashen->primaryWeapons.size() == 2);
    assert(ashen->secondaryWeapons.size() == 2);

    auto stats = globalStats();
    assert(stats.playerHealth == 100);
    assert(stats.headshotMultiplier == 2.5);
    assert(stats.armorSystem == false);

    Weapon falconWeapon(*falcon);
    auto damage = falconWeapon.simulateDamage(HitRegion::Head, 12.0, stats.playerHealth);
    assert(damage.damage > 0);
    assert(damage.headshot == true);

    auto firstTorsoDamage = falconWeapon.simulateDamage(HitRegion::Torso, 12.0, stats.playerHealth);
    assert(firstTorsoDamage.remainingHealth == 62);
    auto followUpDamage = falconWeapon.simulateDamage(HitRegion::Torso, 12.0, firstTorsoDamage.remainingHealth);
    assert(followUpDamage.remainingHealth == 24);
    assert(followUpDamage.kill == false);

    std::cout << "NightFall gameplay tests passed." << std::endl;
    return 0;
}
