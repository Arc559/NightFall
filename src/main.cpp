#include "nightfall.hpp"

#include <iostream>
#include <limits>

namespace {

int readIntInRange(int minValue, int maxValue, const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (value >= minValue && value <= maxValue) {
            return value;
        }

        std::cout << "Selection out of range. Try again.\n";
    }
}

}  // namespace

int main() {
    using namespace nightfall;

    NightfallGame game;
    const auto& stats = globalStats();

    std::cout << "========================================\n";
    std::cout << "            NIGHTFALL                  \n";
    std::cout << "========================================\n";
    std::cout << "Player Health: " << stats.playerHealth << " HP\n";
    std::cout << "Headshot Multiplier: " << stats.headshotMultiplier << "x\n";
    std::cout << "Armor System: " << (stats.armorSystem ? "Enabled" : "Disabled") << "\n\n";

    while (true) {
        std::cout << "========================================\n";
        std::cout << "Select faction:\n";
        std::cout << "1) Raiders\n";
        std::cout << "2) Rebellions\n";
        std::cout << "3) Exit\n";
        std::cout << "========================================\n";

        int factionChoice = readIntInRange(1, 3, "> ");
        if (factionChoice == 3) {
            std::cout << "\nNightFall session ended.\n";
            return 0;
        }

        const auto& roster = factionChoice == 1 ? game.raidRoster() : game.rebellionRoster();
        std::cout << "\nOperators:\n";
        for (size_t i = 0; i < roster.size(); ++i) {
            std::cout << i + 1 << ") " << roster[i].name << "\n";
        }

        int operatorChoice = readIntInRange(1, static_cast<int>(roster.size()), "Select operator number\n> ");

        const auto& selectedOperator = roster[static_cast<size_t>(operatorChoice - 1)];
        std::vector<std::string> loadout;
        loadout.reserve(selectedOperator.primaryWeapons.size() + selectedOperator.secondaryWeapons.size());
        for (const auto& weaponName : selectedOperator.primaryWeapons) {
            loadout.push_back(weaponName);
        }
        for (const auto& weaponName : selectedOperator.secondaryWeapons) {
            loadout.push_back(weaponName);
        }

        std::cout << "\n" << selectedOperator.name << " loadout:\n";
        for (size_t i = 0; i < selectedOperator.primaryWeapons.size(); ++i) {
            std::cout << "  P" << i + 1 << ") " << selectedOperator.primaryWeapons[i] << "\n";
        }
        for (size_t i = 0; i < selectedOperator.secondaryWeapons.size(); ++i) {
            std::cout << "  S" << i + 1 << ") " << selectedOperator.secondaryWeapons[i] << "\n";
        }

        int weaponChoice = readIntInRange(1, static_cast<int>(loadout.size()), "Select weapon number\n> ");

        std::string weaponName = loadout[static_cast<size_t>(weaponChoice - 1)];
        auto weaponData = findWeapon(weaponName);
        if (!weaponData.has_value()) {
            std::cout << "Weapon not found in arsenal.\n\n";
            continue;
        }

        Weapon weapon(*weaponData);

        std::cout << "\n========================================\n";
        std::cout << "COMBAT STARTED\n";
        std::cout << "Operator: " << selectedOperator.name << "\n";
        std::cout << "Weapon: " << weapon.name() << "\n";
        std::cout << "========================================\n\n";

        int enemyHealth = stats.playerHealth;
        int round = 1;
        int shotsHit = 0;
        int totalDamage = 0;

        while (enemyHealth > 0) {
            std::cout << "--- ROUND " << round << " ---\n";
            std::cout << "Enemy HP: " << enemyHealth << "\n\n";

            int adsChoice = readIntInRange(1, 2, "ADS aim?\n1) Yes\n2) No\n> ");
            weapon.setAds(adsChoice == 1);

            int regionChoice = readIntInRange(1, 3, "\nChoose hit region:\n1) Head\n2) Torso\n3) Limb\n> ");
            HitRegion region = HitRegion::Torso;
            if (regionChoice == 1) {
                region = HitRegion::Head;
            } else if (regionChoice == 3) {
                region = HitRegion::Limb;
            }

            double distance = 0.0;
            while (true) {
                std::cout << "Enter distance in meters\n> ";
                std::cin >> distance;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid distance. Enter a number.\n";
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (distance >= 0.0) break;
                std::cout << "Distance cannot be negative.\n";
            }

            auto result = weapon.simulateDamage(region, distance, enemyHealth);
            enemyHealth = result.remainingHealth;
            shotsHit++;
            totalDamage += result.damage;

            std::cout << "\n[SHOT]\n";
            std::cout << "Region: " << toString(region) << "\n";
            std::cout << "Distance: " << distance << "m\n";
            std::cout << "Damage: " << result.damage << "\n";
            if (result.headshot) {
                std::cout << "HEADSHOT! [2.5x multiplier]\n";
            } else if (result.limbHit) {
                std::cout << "Limb hit. [0.9x damage]\n";
            }
            std::cout << "Enemy Health: " << enemyHealth << "\n";

            if (result.kill) {
                std::cout << "\n========================================\n";
                std::cout << "ENEMY ELIMINATED!\n";
                std::cout << "========================================\n";
                std::cout << "Combat Summary:\n";
                std::cout << "Rounds: " << round << "\n";
                std::cout << "Shots: " << shotsHit << "\n";
                std::cout << "Total Damage: " << totalDamage << "\n";
                std::cout << "Average Damage per Shot: " << (totalDamage / shotsHit) << "\n\n";
                break;
            }

            std::cout << "\n";
            round++;
        }

        int again = readIntInRange(1, 2, "Play another match?\n1) Yes\n2) No\n> ");

        if (again != 1) {
            std::cout << "\nNightFall session ended.\n";
            return 0;
        }

        std::cout << "\n";
    }

    return 0;
}
