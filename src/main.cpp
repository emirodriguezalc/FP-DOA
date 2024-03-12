#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct ResourceBonus
{
    int wood = 0;
    int stone = 0;
    int metal = 0;
    int food = 0;
    int gold = 0;
};

struct TargetData
{
    int defendersPower;
    ResourceBonus resourceBonus;
};

std::unordered_map<std::string, std::vector<TargetData>> targetDatabase;
struct Troop
{
    std::string name;
    int attack; // Melee Attack
    int defense;
    int life;
    int speed;
    int load; // Carrying capacity
    int upkeep;
    int power;
    int quantity; // Quantity of troops
};

std::vector<Troop> playerTroops = {
    {"Porter", 1, 10, 45, 100, 200, 2, 1, 0},
    {"Conscript", 10, 10, 75, 200, 20, 3, 1, 0},
    {"Spy", 5, 5, 10, 3000, 0, 5, 2, 0},
    {"Halberdsman", 40, 40, 150, 300, 40, 6, 2, 0},
    {"Minotaur", 70, 45, 225, 275, 30, 7, 3, 0},
    {"Longbowman", 5, 30, 75, 250, 25, 9, 4, 0},
    {"Swift Strike Dragon", 150, 60, 300, 1000, 100, 18, 5, 0},
    {"Battle Dragon", 300, 300, 1500, 750, 80, 35, 7, 0},
    {"Armored Transport", 5, 200, 750, 150, 5000, 10, 6, 0},
    {"Giant", 1000, 400, 4000, 120, 45, 100, 9, 0},
    {"Fire Mirror", 20, 30, 1500, 50, 75, 250, 10, 0},
    {"Pack Dragon", 150, 400, 850, 1000, 6000, 10, 6, 0}};

std::string selectedTarget = "AnthropusCamp";
int selectedLevel = 1;
std::vector<std::string> targets = {"AnthropusCamp", "WildernessForest", "WildernessMountain", "WildernessHill", "WildernessLake"};
std::string battleOutcomeMessage = ""; // To display the outcome
// Total Combat Strength=attack×life×quantity
void initializeTargetDatabase()
{
    // Level 1 - 60 LBM + 147 Porters/33 ATs
    // Level 2 - 320 LBM + 600 Porters/50 ATs
    // Level 3 - 600 LBM + 1815 Porters/72 ATs
    // Level 4 - 2k LBM + 2420 Porters/100 ATs
    // Level 5 - 5k LBM + 100 ATs
    // Level 6 - 7k LBM + 200 ATs
    // Level 7 - 25k LBM + 225 ATs
    // Level 8 - 45k LBM + 1k ATs
    // Level 9 - 70k LBM + 2k ATs
    // Level 10 - 100k LBM + 1k ATs
    targetDatabase["AnthropusCamp"] = {
        {22500, {0, 0, 0, 112500, 2500}},      // Level 1
        {120000, {0, 0, 0, 225000, 5000}},     // Level 2
        {225000, {0, 0, 0, 337500, 7500}},     // Level 3
        {750000, {0, 0, 0, 450000, 10000}},    // Level 4
        {1875000, {0, 0, 0, 562500, 12500}},   // Level 5
        {2625000, {0, 0, 0, 675000, 15000}},   // Level 6
        {9375000, {0, 0, 0, 787500, 17500}},   // Level 7
        {16875000, {0, 0, 0, 900000, 20000}},  // Level 8
        {26250000, {0, 0, 0, 1012500, 22500}}, // Level 9
        {37500000, {0, 0, 0, 1125000, 25000}}  // Level 10
    };
    // Level 1: 20 LGB
    // Level 2: 25 LGB
    // Level 3: 50 LGB
    // Level 4: 200 LGB
    // Level 5: 700 LGB
    // Level 6: 850 LGB
    // Level 7: 3100 LGB
    // Level 8: 4500 LGB
    // Level 9: 7k LGB
    // Level 10: 20k LGB
    // Level 10: 10k + GD LGB
    targetDatabase["WildernessForest"] = {
        {7500, {10000, 500, 500, 1125, 25}},         // Level 1
        {9375, {20000, 1000, 1000, 2250, 50}},       // Level 2
        {18750, {30000, 1500, 1500, 3375, 75}},      // Level 3
        {75000, {40000, 2000, 2000, 4500, 100}},     // Level 4
        {262500, {50000, 2500, 2500, 5625, 125}},    // Level 5
        {318750, {60000, 3000, 3000, 6750, 150}},    // Level 6
        {1162500, {70000, 3500, 3500, 7875, 175}},   // Level 7
        {1687500, {80000, 4000, 4000, 9000, 200}},   // Level 8
        {2625000, {90000, 4500, 4500, 10125, 225}},  // Level 9
        {7500000, {100000, 5000, 5000, 11250, 250}}, // Level 10
    };

    // Mountains - Higher Metal
    targetDatabase["WildernessMountain"] = {
        {7500, {500, 500, 10000, 1125, 25}},         // Level 1
        {9375, {1000, 1000, 20000, 2250, 50}},       // Level 2
        {18750, {1500, 1500, 30000, 3375, 75}},      // Level 3
        {75000, {2000, 2000, 40000, 4500, 100}},     // Level 4
        {262500, {2500, 2500, 50000, 5625, 125}},    // Level 5
        {318750, {3000, 3000, 60000, 6750, 150}},    // Level 6
        {1162500, {3500, 3500, 70000, 7875, 175}},   // Level 7
        {1687500, {4000, 4000, 80000, 9000, 200}},   // Level 8
        {2625000, {4500, 4500, 90000, 10125, 225}},  // Level 9
        {7500000, {5000, 5000, 100000, 11250, 250}}, // Level 10
    };

    // Hills - Higher Stone
    targetDatabase["WildernessHill"] = {
        {7500, {500, 10000, 500, 1125, 25}},         // Level 1
        {9375, {1000, 20000, 1000, 2250, 50}},       // Level 2g
        {18750, {1500, 30000, 1500, 3375, 75}},      // Level 3
        {75000, {2000, 40000, 2000, 4500, 100}},     // Level 4
        {262500, {2500, 50000, 2500, 5625, 125}},    // Level 5
        {318750, {3000, 60000, 3000, 6750, 150}},    // Level 6
        {1162500, {3500, 70000, 3500, 7875, 175}},   // Level 7
        {1687500, {4000, 80000, 4000, 9000, 200}},   // Level 8
        {2625000, {4500, 90000, 4500, 10125, 225}},  // Level 9
        {7500000, {5000, 100000, 5000, 11250, 250}}, // Level 10
    };
    targetDatabase["WildernessLake"] = {
        {7500, {1125, 25, 500, 10000, 500}},         // Level 1
        {9375, {2250, 50, 1000, 20000, 1000}},       // Level 2
        {18750, {3375, 75, 1500, 30000, 1500}},      // Level 3
        {75000, {4500, 100, 2000, 40000, 2000}},     // Level 4
        {262500, {5625, 125, 2500, 50000, 2500}},    // Level 5
        {318750, {6750, 150, 3000, 60000, 3000}},    // Level 6
        {1162500, {7875, 175, 3500, 70000, 3500}},   // Level 7
        {1687500, {9000, 200, 4000, 80000, 4000}},   // Level 8
        {2625000, {10125, 225, 4500, 90000, 4500}},  // Level 9
        {7500000, {11250, 250, 5000, 100000, 5000}}, // Level 10
    };

    // Savannas - Higher Food
    targetDatabase["WildernessSavanna"] = {
        {7500, {15000, 25, 500, 500, 500}},         // Level 1
        {9375, {30000, 50, 1000, 1000, 1000}},      // Level 2
        {18750, {45000, 75, 1500, 1500, 1500}},     // Level 3
        {75000, {60000, 100, 2000, 2000, 2000}},    // Level 4
        {262500, {75000, 125, 2500, 2500, 2500}},   // Level 5
        {318750, {90000, 150, 3000, 3000, 3000}},   // Level 6
        {1162500, {105000, 175, 3500, 3500, 3500}}, // Level 7
        {1687500, {120000, 200, 4000, 4000, 4000}}, // Level 8
        {2625000, {135000, 225, 4500, 4500, 4500}}, // Level 9
        {7500000, {150000, 250, 5000, 5000, 5000}}, // Level 10
    };
    targetDatabase["WildernessPlains"] = {
        {7500, {15000, 2500, 500, 500, 500}},         // Level 1
        {9375, {30000, 5000, 1000, 1000, 1000}},      // Level 2
        {18750, {45000, 7500, 1500, 1500, 1500}},     // Level 3
        {75000, {60000, 10000, 2000, 2000, 2000}},    // Level 4
        {262500, {75000, 12500, 2500, 2500, 2500}},   // Level 5
        {318750, {90000, 15000, 3000, 3000, 3000}},   // Level 6
        {1162500, {105000, 17500, 3500, 3500, 3500}}, // Level 7
        {1687500, {120000, 20000, 4000, 4000, 4000}}, // Level 8
        {2625000, {135000, 22500, 4500, 4500, 4500}}, // Level 9
        {7500000, {150000, 25000, 5000, 5000, 5000}}, // Level 10
    };
}

TargetData getTargetData(const std::string &targetType, int level)
{
    return targetDatabase[targetType][level - 1];
}

int calculateTotalCombatStrength(const std::vector<Troop> &troops)
{
    int totalCombatStrength = 0;
    for (const auto &troop : troops)
    {
        if (troop.quantity > 0)
        {
            totalCombatStrength += troop.attack * troop.life * troop.quantity;
        }
    }
    return totalCombatStrength;
}

void displayTroopInputForm()
{
    ImGui::Begin("Troop Deployment Form");

    for (auto &troop : playerTroops)
    {
        ImGui::InputInt(troop.name.c_str(), &troop.quantity);
    }

    if (ImGui::BeginCombo("Target", selectedTarget.c_str()))
    {
        for (const auto &target : targets)
        {
            bool isSelected = (selectedTarget == target);
            if (ImGui::Selectable(target.c_str(), isSelected))
            {
                selectedTarget = target;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::InputInt("Level", &selectedLevel);
    selectedLevel = std::max(1, std::min(selectedLevel, 10));

    if (ImGui::Button("Deploy"))
    {
        int playerCombatStrength = calculateTotalCombatStrength(playerTroops);
        TargetData target = getTargetData(selectedTarget, selectedLevel);
        bool victory = playerCombatStrength > target.defendersPower;
        battleOutcomeMessage = victory ? "Victory! " : "Defeat! ";
        battleOutcomeMessage += "Your power: " + std::to_string(playerCombatStrength) + " vs Enemy power: " + std::to_string(target.defendersPower) + ". ";

        if (victory)
        {
            battleOutcomeMessage += "Resources earned: Wood " + std::to_string(target.resourceBonus.wood) +
                                    ", Stone " + std::to_string(target.resourceBonus.stone) +
                                    ", Metal " + std::to_string(target.resourceBonus.metal) +
                                    ", Food " + std::to_string(target.resourceBonus.food) +
                                    ", Gold " + std::to_string(target.resourceBonus.gold) + ".";
        }
    }

    if (!battleOutcomeMessage.empty())
    {
        ImGui::Text("%s", battleOutcomeMessage.c_str());
    }

    ImGui::End();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Battle Simulator");
    ImGui::SFML::Init(window);

    initializeTargetDatabase();

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        displayTroopInputForm();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
