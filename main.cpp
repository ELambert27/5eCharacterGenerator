//5eCharacterGenerator
//Makes a random D&D 5e character, either at the given level or at a random level if no level is given

#include <iostream>
#include <string>
#include <time.h>
#include <random>
#include <stdexcept>

#define STR_COMPARE_TRUE 0

//races and subraces
#pragma region
static int numberOfRaces = 31;
static std::string races[] = {
	"revenant", "aarakocra", "aasimar", "bugbear", "changeling", "dragonborn", "dwarf", "elf", "firbolg", "genasi", "gnome",
	"goblin", "goliath", "half-elf", "halfling", "half-orc", "hobgoblin", "human", "kenku", "kobold", "lizardfolk", "minotaur",
	"orc", "shifter", "tabaxi", "tiefling", "tortle", "triton", "warforged", "yuan-ti pureblood", "gith"
};
static int numEmptySubs = 0;
static std::string emptySubrace[] = { "" };
static std::string emptySubAbil = "";
static std::string emptySubraceAbilities[] = { emptySubAbil };

static int subRevenantStats[] = { 0, 0, 1, 0, 0, 0 }; // // REVENANT IS SUPPOSED TO ALSO REDUCE CERTAIN STATS IF YOU'RE A DRAGONBORN, HUMAN, OR TIEFLING // //
static int* revenantSubraceStats[] = { subRevenantStats };
static std::string revenantAbilities = "Relentless Nature";
	//revenant has no subrace, and therefore no subrace abilities

static int subAarakocraStats[] = { 0, 2, 0, 0, 1, 0 };
static int* aarakocraSubraceStats[] = { subAarakocraStats };
static std::string aarakocraAbilities = "50 feet fly speed (not in med/heavy armor), Talons (1d4)";
	//aarakocra has no subrace, and therefore no subrace abilities

static int numAasimarSubs = 3;
static std::string aasimarSubrace[] = { "protector", "scourge", "fallen" };
static int protectorAasimarStats[] = { 0, 0, 0, 0, 1, 2 };
static int scourgeAasimarStats[] = { 0, 0, 1, 0, 0, 2 };
static int fallenAasimarStats[] = { 1, 0, 0, 0, 0, 2 };
static int* aasimarSubraceStats[] = { protectorAasimarStats, scourgeAasimarStats, fallenAasimarStats };
static std::string aasimarAbilities = "60 foot darkvision, healing hands, light cantrip, necrotic damage resistance, radiant damage resistance";
static std::string protectorAasimarAbilities = "radiant soul";
static std::string scourgeAasimarAbilities = "radiant consumption";
static std::string fallenAasimarAbilities = "necrotic shroud";
static std::string aasimarSubraceAbilities[] = { protectorAasimarAbilities, scourgeAasimarAbilities, fallenAasimarAbilities };

static int subBugbearStats[] = { 2, 1, 0, 0, 0, 0 };
static int* bugbearSubraceStats[] = { subBugbearStats };
static std::string bugbearAbilities = "60 foot darkvision, +5 foot melee reach, powerful build, surprise attack, proficiency in stealth";
	//bugbear also gains proficiency in the stealth skill
	//bugbear has no subraces, so no subrace abilities

static int subChangelingStats[] = { 0, 1, 0, 0, 0, 1 };
static int* changelingSubraceStats[] = { subChangelingStats };
static std::string changelingAbilities = "shapechanger, proficiency in deception";
	//changeling also gains proficiency in the deception skill
	//changeling has no subraces, so no subrace abilities

static int numDragonbornSubs = 10;
static std::string dragonbornSubrace[] = { "black", "blue", "brass", "bronze", "copper", "gold", "green", "red", "silver", "white" };
static int subDragonbornStats[] = { 2, 0, 0, 0, 0, 1 };
static int* dragonbornSubraceStats[] = { subDragonbornStats };
static std::string dragonbornAbilities = "";
static std::string blackDragonbornAbilities = "acid breath weapon (5 x 30 foot line w/ dex save), resistance to acid damage";
static std::string blueDragonbornAbilities = "lightning breath weapon (5 x 30 foot line w/ dex save), resistance to lightning damage";
static std::string brassDragonbornAbilities = "fire breath weapon (5 x 30 foot line w/ dex save), resistance to fire damage";
static std::string bronzeDragonbornAbilities = "lightning breath weapon (5 x 30 foot line w/ dex save), resistance to lightning damage";
static std::string copperDragonbornAbilities = "acid breath weapon (5 x 30 foot line w/ dex save), resistance to acid damage";
static std::string goldDragonbornAbilities = "fire breath weapon (15 foot cone w/ dex save), resistance to fire damage";
static std::string greenDragonbornAbilities = "poison breath weapon (15 foot cone w/ con save), resistance to poison damage";
static std::string redDragonbornAbilities = "fire breath weapon (15 foot cone w/ dex save), resistance to fire damage";
static std::string silverDragonbornAbilties = "cold breath weapon (15 foot cone w/ con save), resistance to cold damage";
static std::string whiteDragonbornAbilities = "cold breath weapon (15 foot cone w/ con save), resistance to cold damage";
static std::string dragonbornSubraceAbilities[] = { blackDragonbornAbilities, blueDragonbornAbilities, brassDragonbornAbilities, bronzeDragonbornAbilities, copperDragonbornAbilities, goldDragonbornAbilities, greenDragonbornAbilities, redDragonbornAbilities, silverDragonbornAbilties, whiteDragonbornAbilities };

static int numDwarfSubs = 3;
static std::string dwarfSubrace[] = { "mountain", "hill", "duergar" };
static int mountainDwarfStats[] = { 2, 0, 2, 0, 0, 0 };
static int hillDwarfStats[] = { 0, 0, 2, 0, 1, 0 };
static int duergarDwarfStats[] = { 1, 0, 2, 0, 0, 0 };
static int* dwarfSubraceStats[] = { mountainDwarfStats, hillDwarfStats, duergarDwarfStats };
static std::string dwarfAbilities = "dwarven combat training, stone-cunning, advantage on saves vs poison, resistance to poison damage, proficiency with (one of) smith/brewer/mason's tools, speed is not reduced by heavy armor";
static std::string mountainDwarfAbilities = "60 foot darkvision, proficiency with light and medium armor";
static std::string hillDwarfAbilities = "60 foot darkvision"; //+1 hp per level, accounted for later on
static std::string duergarDwarfAbilities = "120 foot darkvision, sunlight sensativity, duergar magic, advantage on saves vs illusions/being charmed/being paralyzed";
static std::string dwarfSubraceAbilities[] = { mountainDwarfAbilities, hillDwarfAbilities, duergarDwarfAbilities };

static int numElfSubs = 8;
static std::string elfSubrace[] = { "high", "wood", "drow", "avariel", "grugach", "sea elf", "shadar-kai", "eladrin" };
static int highElfStats[] = { 0, 2, 0, 1, 0, 0 };
static int woodElfStats[] = { 0, 2, 0, 0, 1, 0 };
static int drowElfStats[] = { 0, 2, 0, 0, 0, 1 };
static int avarielElfStats[] = { 0, 2, 0, 0, 0, 0 };
static int grugachElfStats[] = { 1, 2, 0, 0, 0, 0 };
static int seaElfStats[] = { 0, 2, 1, 0, 0, 0 };
static int shadarKaiElfStats[] = { 0, 2, 0, 0, 0, 1 };
static int eladrinElfStats[] = { 0, 2, 0, 1, 0, 0 };
static int* elfSubraceStats[] = { highElfStats, woodElfStats, drowElfStats, avarielElfStats, grugachElfStats, seaElfStats, shadarKaiElfStats, eladrinElfStats };
static std::string elfAbilities = "fey ancestry, trance, proficiency in perception";
	//elves have proficiency in perception
static std::string highElfAbilities = "60 foot darkvision, elf weapon training, gain a wizard cantrip";
	//high elves gain a bonus wizard cantrip which needs to be accounted for later
static std::string woodElfAbilities = "60 foot darkvision, elf weapon training, mask of the wild";
static std::string drowElfAbilities = "120 foot darkvision, sunlight sensativity, drow weapon training, drow magic";
static std::string avarielElfAbilities = "60 foot darkvision, 30 foot fly speed (not in med/heavy armor)";
static std::string grugachElfAbilities = "60 foot darkvision, grugach weapon training, gain a druid cantrip";
	//grugach elves gain a bonus druid cantrip which needs to be accounted for later
static std::string seaElfAbilities = "60 foot darkvision, 30 foot swim speed, can breathe air or water, sea elf weapon training, friend of the sea";
static std::string shadarKaiElfAbilities = "60 foot darkvision, blessing of the raven queen, gain (one of) chill touch/spare the dying/thaumaturgy";
	//shadark kai elves gain a bonus cantrip which needs to be accounted for later
static std::string eladrinElfAbilities = "60 foot darkvision, fey step, shifting seasons";
static std::string elfSubraceAbilities[] = { highElfAbilities, woodElfAbilities, drowElfAbilities, avarielElfAbilities, grugachElfAbilities, seaElfAbilities, shadarKaiElfAbilities, eladrinElfAbilities };

static int subFirbolgStats[] = { 1, 0, 0, 0, 2, 0 };
static int* firbolgSubraceStats[] = { subFirbolgStats };
static std::string firbolgAbilities = "firbolg magic, hidden step, powerful build, speech of beast and leaf";
	//firbolg has no subrace, so therefore no subrace abilities

static int numGenasiSubs = 4;
static std::string genasiSubrace[] = { "air", "earth", "fire", "water" };
static int airGenasiStats[] = { 0, 1, 2, 0, 0, 0 };
static int earthGenasiStats[] = { 1, 0, 2, 0, 0, 0 };
static int fireGenasiStats[] = { 0, 0, 2, 1, 0, 0 };
static int waterGenasiStats[] = { 0, 0, 2, 0, 1, 0 };
static int* genasiSubraceStats[] = { airGenasiStats, earthGenasiStats, fireGenasiStats, waterGenasiStats };
static std::string genasiAbilities = "";
static std::string airGenasiAbilities = "unending breath, mingle with the wind";
static std::string earthGenasiAbilities = "earth walk, merge with stone";
static std::string fireGenasiAbilities = "60 foot darkvision, reach to the blaze, resistance to fire damage";
static std::string waterGenasiAbilities = "30 foot swim speed, amphibious, call to the wave, resistance to acid damage";
static std::string genasiSubraceAbilities[] = { airGenasiAbilities, earthGenasiAbilities, fireGenasiAbilities, waterGenasiAbilities };

static int numGnomeSubs = 3;
static std::string gnomeSubrace[] = { "deep", "forest", "rock" };
static int deepGnomeStats[] = { 0, 1, 0, 2, 0, 0 };
static int forestGnomeStats[] = { 0, 1, 0, 2, 0, 0 };
static int rockGnomeStats[] = { 0, 0, 1, 2, 0, 0 };
static int* gnomeSubraceStats[] = { deepGnomeStats, forestGnomeStats, rockGnomeStats };
static std::string gnomeAbilities = "advantage on int/wis/cha saves against magic";
static std::string deepGnomeAbilities = "120 foot darkvision, advantage on stealth to hide in rocky terrain";
static std::string forestGnomeAbilities = "60 foot darkvision, speak with small beasts, gain the minor illusion cantrip";
static std::string rockGnomeAbilities = "60 foot darkvision, artificer's lore, tinker";
static std::string gnomeSubraceAbilities[] = { deepGnomeAbilities, forestGnomeAbilities, rockGnomeAbilities };

static int subGoblinStats[] = { 0, 2, 1, 0, 0, 0 };
static int* goblinSubraceStats[] = { subGoblinStats };
static std::string goblinAbilities = "60 foot darkvision, fury of the small, bonus action disengage or hide";
	//goblins have no subraces, therefore no subrace abilities

static int subGoliathStats[] = { 2, 0, 1, 0, 0, 0 };
static int* goliathSubraceStats[] = { subGoliathStats };
static std::string goliathAbilities = "stone's endurance, powerful build, mountain born, proficiency in athletics";
	//goliaths have no subraces, therefore no subrace abilities

static int subHalfElfStats[] = { 0, 0, 0, 0, 0, 2 };
static int* halfElfSubraceStats[] = { subHalfElfStats };
static std::string halfElfAbilities = "60 foot darkvision, fey ancestry, skill versatility (+2 skill proficiencies)";
	//half elves gain 2 bonus skill proficiencies which will need to be accounted for later
	//half elves have no (implemented) subraces, therefore no subrace abilities

static int numHalflingSubs = 3;
static std::string halflingSubrace[] = { "ghostwise", "lightfoot", "stout" };
static int ghostwiseHalflingStats[] = { 0, 2, 0, 0, 1, 0 };
static int lightfootHalflingStats[] = { 0, 2, 0, 0, 0, 1 };
static int stoutHalflingStats[] = { 0, 2, 1, 0, 0, 0 };
static int* halflingSubraceStats[] = { ghostwiseHalflingStats, lightfootHalflingStats, stoutHalflingStats };
static std::string halflingAbilities = "lucky, brave, nimbleness";
static std::string ghostwiseHalflingAbilities = "silent speech";
static std::string lightfootHalflingAbillities = "naturally stealthy";
static std::string stoutHalflingAbilities = "advantage on saves vs poison, resistance to poison damage";
static std::string halflingSubraceAbilities[] = { ghostwiseHalflingAbilities, lightfootHalflingAbillities, stoutHalflingAbilities };

static int subHalfOrcStats[] = { 2, 0, 1, 0, 0, 0 };
static int* halfOrcSubraceStats[] = { subHalfOrcStats };
static std::string halfOrcAbilities = "60 foot darkvision, relentless endurance, savage attacks, proficiency in intimidation";
	//half orcs have no subraces, therefore no subrace abilities

static int subHobgoblinStats[] = { 0, 0, 2, 1, 0, 0 };
static int* hobgoblinSubraceStats[] = { subHobgoblinStats };
static std::string hobgoblinAbilities = "60 foot darkvision, saving face, proficiency in light armor, proficiency in 2 martial weapons";
	//hobgoblins have proficiency in light armor and 2 martial weapons, which needs to be accounted for later
	//hobgoblins have no subraces, therefore no subrace abilities

static int numHumanSubs = 2;
static std::string humanSubrace[] = { "standard", "variant" };
static int standardHumanStats[] = { 1, 1, 1, 1, 1, 1 };
static int variantHumanStats[] = { 0, 0, 0, 0, 0, 0 };
static int* humanSubraceStats[] = { standardHumanStats, variantHumanStats };
static std::string humanAbilities = "";
static std::string standardHumanAbilities = "";
static std::string variantHumanAbilities = "+1 skill proficiency, +1 feat";
	//variant humans gain a bonus skill which needs to be accounted for later -- the bonus feat is already taken into account
static std::string humanSubraceAbilities[] = { standardHumanAbilities, variantHumanAbilities };

static int subKenkuStats[] = { 0, 2, 0, 0, 1, 0 };
static int* kenkuSubraceStats[] = { subKenkuStats };
static std::string kenkuAbilities = "expert forgery, mimicry, proficiency in 2 of acrobatics/deception/stealth/sleight of hand";
	//kenku gains 2 bonus skills which need to be accounted for later
	//kenku has no subraces, therefore no subrace abilities

static int subKoboldStats[] = { -2, 2, 0, 0, 0, 0 };
static int* koboldSubraceStats[] = { subKoboldStats };
static std::string koboldAbilities = "60 foot darkvision, sunlight sensativity, grover cower and beg, pack tactics";
	//kobolds have no subraces, therefore no subrace abilities

static int subLizardfolkStats[] = { 0, 0, 2, 0, 1, 0 };
static int* lizardfolkSubraceStats[] = { subLizardfolkStats };
static std::string lizardfolkAbilities = "30 foot swim speed, hold breath 15 mins, bite, hungry jaws, natural armor, cunning artisan, proficiency in 2 of animal handling/nature/perception/stealth/survival";
	//lizaradfolk have no subraces, therefore no subrace abilities

static int subMinotaurStats[] = { 1, 0, 0, 0, 0, 0 };
static int* minotaurSubraceStats[] = { subMinotaurStats };
static std::string minotaurAbilities = "hammering horns, goring rush, labyrinthine recall, proficiency with navigator's tools and water vehicles";
	//minotaurs gain 2 tool proficiencies which need to be accounted for later
	//minotaurs have no subraces, therefore no subrace abilities

static int subOrcStats[] = { 2, 0, 1, -2, 0, 0 };
static int* orcSubraceStats[] = { subOrcStats };
static std::string orcAbilities = "60 foot darkvision, aggressive, powerful build, proficiency with intimidation";
	//orcs have intimidation proficiency which needs to be accounted for later
	//orcs have no subraces, therefore no subrace abilities

static int numShifterSubs = 6;
static std::string shifterSubrace[] = { "beasthide", "cliffwalk", "longstride", "longtooth", "razorclaw", "wildhunt" };
static int beasthideShifterStats[] = { 0, 1, 1, 0, 0, 0 };
static int cliffwalkShifterStats[] = { 0, 2, 0, 0, 0, 0 };
static int longstrideShifterStats[] = { 0, 2, 0, 0, 0, 0 };
static int longtoothShifterStats[] = { 1, 1, 0, 0, 0, 0 };
static int razorclawShifterStats[] = { 0, 2, 0, 0, 0, 0 };
static int wildhuntShifterStats[] = { 0, 1, 0, 0, 1, 0 };
static int* shifterSubraceStats[] = { beasthideShifterStats, cliffwalkShifterStats, longstrideShifterStats, longtoothShifterStats, razorclawShifterStats, wildhuntShifterStats };
static std::string shifterAbilities = "60 foot darkvision, shifting";
static std::string beasthideShifterAbilities = "+1 AC while shifting";
static std::string cliffwalkShifterAbilities = "30 foot climb speed while shifting";
static std::string longstrideShifterAbilities = "dash as bonus action while shifting";
static std::string longtoothShifterAbilities = "gain a bite attack while shifting";
static std::string razorclawShifterAbilities = "gain an unarmed strike attack while shifting";
static std::string wildhuntShifterAbilities = "advantage on wis checks and saves while shifting";
static std::string shifterSubraceAbilities[] = { beasthideShifterAbilities, cliffwalkShifterAbilities, longstrideShifterAbilities, longtoothShifterAbilities, razorclawShifterAbilities, wildhuntShifterAbilities };

static int subTabaxiStats[] = { 0, 2, 0, 0, 0, 1 };
static int* tabaxiSubraceStats[] = { subTabaxiStats };
static std::string tabaxiAbilities = "20 foot climb speed, 60 foot darkvision, feline agility, cat's claws, proficiency in perception and stealth";
	//tabaxi gains proficiency in 2 skills, which needs to be accounted for later
	//tabaxi has no subraces, therefore no subrace abilities

static int numTieflingSubs = 10;
static std::string tieflingSubrace[] = { "infernal/asmodeus", "abyssal", "baalzebul", "dispater", "fierna", "glasya", "levistus", "mammon", "mephistopheles", "zariel" };
static int infernalAsmodeusTieflingStats[] = { 0, 0, 0, 1, 0, 2 };
static int abyssalTieflingStats[] = { 0, 0, 1, -1, 0, 0 };
static int baalzebulTieflingStats[] = { 0, 0, 0, 1, 0, 2 };
static int dispaterTieflingStats[] = { 0, 1, 0, 0, 0, 2 };
static int fiernaTieflingStats[] = { 0, 0, 0, 0, 1, 2 };
static int glasyaTieflingStats[] = { 0, 1, 0, 0, 0, 2 };
static int levistusTieflingStats[] = { 0, 0, 1, 0, 0, 2 };
static int mammonTieflingStats[] = { 0, 0, 0, 1, 0, 2 };
static int mephistophelesTieflingStats[] = { 0, 0, 0, 1, 0, 2 };
static int zarielTieflingStats[] = { 1, 0, 0, 0, 0, 2 };
static int* tieflingSubraceStats[] = { infernalAsmodeusTieflingStats, abyssalTieflingStats, baalzebulTieflingStats, dispaterTieflingStats, fiernaTieflingStats, glasyaTieflingStats, levistusTieflingStats, mammonTieflingStats, mephistophelesTieflingStats, zarielTieflingStats };
static std::string tieflingAbilities = "60 foot darkvision";
static std::string infernalAsmodeusTieflingAbilities = "hellish resistance, infernal legacy";
static std::string abyssalTieflingAbilities = "abyssal arcana";
	//abyssal tieflings also gain 1/2 their level in bonus hp, which is accounted for later
static std::string baalzebulTieflingAbilities = "legacy of maladomini (bonus spells)";
static std::string dispaterTieflingAbilities = "legacy of dis (bonus spells)";
static std::string fiernaTieflingAbilities = "legacy of phlegethos (bonus spells)";
static std::string glasyaTieflingAbilities = "legacy of malbolge (bonus spells)";
static std::string levistusTieflingAbilities = "legacy of stygia (bonus spells)";
static std::string mammonTieflingAbilities = "legacy of minauros (bonus spells)";
static std::string mephistophelesTieflingAbilities = "legacy of cania (bonus spells)";
static std::string zarielTieflingAbilities = "legacy of avernus (bonus spells)";
	//a bunch of tiefling variants gain bonus spells which need to be accounted for later
static std::string tieflingSubraceAbilities[] = { infernalAsmodeusTieflingAbilities, abyssalTieflingAbilities, baalzebulTieflingAbilities, dispaterTieflingAbilities, fiernaTieflingAbilities, glasyaTieflingAbilities, levistusTieflingAbilities, mammonTieflingAbilities, mephistophelesTieflingAbilities, zarielTieflingAbilities };

static int subTortleStats[] = { 2, 0, 0, 0, 1, 0 };
static int* tortleSubraceStats[] = { subTortleStats };
static std::string tortleAbilities = "natural armor, shell defense, claws, hold breath, proficiency in survival";
	//tortles gain proficiency in survival, which needs to be accounted for later
	//tortles have no subraces, therefore no subrace abilities

static int subTritonStats[] = { 1, 0, 1, 0, 0, 1 };
static int* tritonSubraceStats[] = { subTritonStats };
static std::string tritonAbilities = "30 foot swim speed, amphibious, control air and water (bonus spells), emissary of the sea, guardians of the depths";
	//tritons have bonus spells which need to be accounted for later
	//tritons have no subraces, therefore no subrace abilities

static int subWarforgedStats[] = { 1, 0, 1, 0, 0, 0 };
static int* warforgedSubraceStats[] = { subWarforgedStats };
static std::string warforgedAbilities = "composite plating (+1 AC), living construct";
	//warforged have no subraces, therefore no subrace abilities

static int subYuanTiStats[] = { 0, 0, 0, 1, 0, 2 };
static int* yuanTiSubraceStats[] = { subYuanTiStats };
static std::string yuanTiAbilities = "60 foot darkvision, innate spellcasting, advantage on saves vs magic, immune to poison damage and the poisoned condition";
	//yuan ti have bonus spells that need to be accounted for later
	//yuan ti have no subraces, therefore no subrace abilities

static int numGithSubs = 2;
static std::string githSubrace[] = { "githyanki", "githzerai" };
static int githyankiGithStats[] = { 2, 0, 0, 1, 0, 0 };
static int githzeraiGithStats[] = { 0, 0, 0, 1, 2, 0 };
static int* githSubraceStats[] = { githyankiGithStats, githzeraiGithStats };
static std::string githAbilities = "";
static std::string githyankiGithAbilities = "githyanki psionics (bonus spells), proficiency in one language, proficiency in one skill or language, proficiency in light and medium armor";
	//githyanki have a bunch of bonus proficiencies that need to be accounted for later
static std::string githzeraiGithAbilities = "monastic training, githzerai psionics (bonus spells)";
	//githyanki have bonus spells and stuff that needs to be accounted for later
static std::string githSubraceAbilities[] = { githyankiGithAbilities, githzeraiGithAbilities };

static int subraceNums[] = {
	numEmptySubs, numEmptySubs, numAasimarSubs, numEmptySubs, numEmptySubs, numDragonbornSubs, numDwarfSubs, numElfSubs, numEmptySubs, numGenasiSubs, numGnomeSubs,
	numEmptySubs, numEmptySubs, numEmptySubs, numHalflingSubs, numEmptySubs, numEmptySubs, numHumanSubs, numEmptySubs, numEmptySubs, numEmptySubs, numEmptySubs,
	numEmptySubs, numShifterSubs, numEmptySubs, numTieflingSubs, numEmptySubs, numEmptySubs, numEmptySubs, numEmptySubs, numGithSubs
};
static std::string* subraces[] = {
	emptySubrace, emptySubrace, aasimarSubrace, emptySubrace, emptySubrace, dragonbornSubrace, dwarfSubrace, elfSubrace, emptySubrace, genasiSubrace, gnomeSubrace,
	emptySubrace, emptySubrace, emptySubrace, halflingSubrace, emptySubrace, emptySubrace, humanSubrace, emptySubrace, emptySubrace, emptySubrace, emptySubrace,
	emptySubrace, shifterSubrace, emptySubrace, tieflingSubrace, emptySubrace, emptySubrace, emptySubrace, emptySubrace, githSubrace
};
static int** subraceStats[] = {
	revenantSubraceStats, aarakocraSubraceStats, aasimarSubraceStats, bugbearSubraceStats, changelingSubraceStats, dragonbornSubraceStats, dwarfSubraceStats, elfSubraceStats, firbolgSubraceStats, genasiSubraceStats, gnomeSubraceStats,
	goblinSubraceStats, goliathSubraceStats, halfElfSubraceStats, halflingSubraceStats, halfOrcSubraceStats, hobgoblinSubraceStats, humanSubraceStats, kenkuSubraceStats, koboldSubraceStats, lizardfolkSubraceStats, minotaurSubraceStats,
	orcSubraceStats, shifterSubraceStats, tabaxiSubraceStats, tieflingSubraceStats, tortleSubraceStats, tritonSubraceStats, warforgedSubraceStats, yuanTiSubraceStats, githSubraceStats
};
static std::string raceAbilities[] = {
	revenantAbilities, aarakocraAbilities, aasimarAbilities, bugbearAbilities, changelingAbilities, dragonbornAbilities, dwarfAbilities, elfAbilities, firbolgAbilities, genasiAbilities, gnomeAbilities,
	goblinAbilities, goliathAbilities, halfElfAbilities, halflingAbilities, halfOrcAbilities, hobgoblinAbilities, humanAbilities, kenkuAbilities, koboldAbilities, lizardfolkAbilities, minotaurAbilities,
	orcAbilities, shifterAbilities, tabaxiAbilities, tieflingAbilities, tortleAbilities, tritonAbilities, warforgedAbilities, yuanTiAbilities, githAbilities
};
static std::string* subraceAbilities[] = {
	emptySubraceAbilities, emptySubraceAbilities, aasimarSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, dragonbornSubraceAbilities, dwarfSubraceAbilities, elfSubraceAbilities, emptySubraceAbilities, genasiSubraceAbilities, gnomeSubraceAbilities,
	emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, halflingSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, humanSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities,
	emptySubraceAbilities, shifterSubraceAbilities, emptySubraceAbilities, tieflingSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, githSubraceAbilities
};
#pragma endregion races and subraces

//classes and subclasses
#pragma region 
static int numberOfClasses = 16;
static std::string classes[] = {
	"artificer", "barbarian", "bard", "cleric", "druid", "fighter", "monk", "mystic",
	"paladin", "ranger", "rogue", "sorcerer", "warlock", "wizard", "rune scribe", "multiclass"
};
static int classHitDice[] = {
	8, 12, 8, 8, 8, 10, 8, 8,
	10, 10, 8, 6, 8, 6, 8, 0
};
static int numArtificerSubs = 2;
static std::string artificerSubclass[] = { "alchemist", "gunslinger" };
static int numBarbarianSubs = 6;
static std::string barbarianSubclass[] = { "ancestral guardian", "battlerager", "berserker", "storm herald", "totem", "zealot" };
static int numBardSubs = 6;
static std::string bardSubclass[] = { "glamour", "lore", "satire", "swords", "valor", "whisper" };
static int numClericSubs = 14;
static std::string clericSubclass[] = { "arcana", "city", "death", "forge", "grave", "knowledge", "life", "light", "nature", "protection", "tempest", "trickery", "war", "order" };
static int numDruidSubs = 6;
static std::string druidSubclass[] = { "dreams", "land", "moon", "shepherd", "spores", "twilight" };
static int numFighterSubs = 11;
static std::string fighterSubclass[] = { "arcane archer", "purple dragon knight", "battle master", "brute", "cavalier", "champion", "eldritch knight", "knight", "samurai", "sharpshooter", "monster hunter" };
static int numMonkSubs = 8;
static std::string monkSubclass[] = { "drunken master", "four elements", "kensai", "long death", "open hand", "shadow", "sun soul", "tranquility" };
static int numMysticSubs = 6;
static std::string mysticSubclass[] = { "avatar", "awakened", "immortal", "nomad", "soul knife", "wu jen" };
static int numPaladinSubs = 8;
static std::string paladinSubclass[] = { "ancients", "conquest", "crown", "devotion", "redemption", "treachery", "vengeance", "oathbreaker" };
static int numRangerSubs = 6;
static std::string rangerSubclass[] = { "beast master", "dark stalker", "horizon walker", "hunter", "monster slayer", "primeval guardian" };
static int numRogueSubs = 7;
static std::string rogueSubclass[] = { "arcane trickster", "assassin", "inquisitive", "mastermind", "scout", "swashbuckler", "thief" };
static int numSorcererSubs = 8;
static std::string sorcererSubclass[] = { "divine soul", "draconic", "phoenix", "sea", "shadow", "stone", "storm", "wild" };
static int numWarlockSubs = 9;
static std::string warlockSubclass[] = { "archfey", "celestial", "fiend", "ghost in the machine", "great old one", "hexblade", "raven queen", "seeker", "undying" };
static int numWizardSubs = 15;
static std::string wizardSubclass[] = { "artificer", "bladesinger", "lore master", "abjuration", "conjuration", "divination", "enchantment", "evocation", "illusion", "invention", "necromancy", "transmutation", "technomancy", "theurgy", "war magic" };
static int numEmptySubcs = 0;
static std::string emptySubclass[] = { "" };
static int subclassNums[] = {
	numArtificerSubs, numBarbarianSubs, numBardSubs, numClericSubs, numDruidSubs, numFighterSubs, numMonkSubs, numMysticSubs,
	numPaladinSubs, numRangerSubs, numRogueSubs, numSorcererSubs, numWarlockSubs, numWizardSubs, numEmptySubs, numEmptySubs
};
static std::string* subclasses[] = {
	artificerSubclass, barbarianSubclass, bardSubclass, clericSubclass, druidSubclass, fighterSubclass, monkSubclass, mysticSubclass,
	paladinSubclass, rangerSubclass, rogueSubclass, sorcererSubclass, warlockSubclass, wizardSubclass, emptySubclass, emptySubclass
};
#pragma endregion classes and subclasses

//class abilities
#pragma region 
static std::string artificerAbilities[] = {
	"magic item analysis",
	"magic item analysis, tool expertise, wondrous invention",
	"magic item analysis, tool expertise, wondrous invention",
	"magic item analysis, tool expertise, wondrous invention, infuse magic",
	"magic item analysis, tool expertise, wondrous invention (2 items), infuse magic, superior attunement",
	"magic item analysis, tool expertise, wondrous invention (2 items), infuse magic, superior attunement, mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (2 items), infuse magic, superior attunement, mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (2 items), infuse magic, superior attunement, mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (2 items), infuse magic, superior attunement, mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (3 items), infuse magic, superior attunement, mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (3 items), infuse magic, superior attunement, mechanical servant", 
	"magic item analysis, tool expertise, wondrous invention (3 items), infuse magic, superior attunement, mechanical servant", 
	"magic item analysis, tool expertise, wondrous invention (3 items), infuse magic, superior attunement, mechanical servant", 
	"magic item analysis, tool expertise, wondrous invention (3 items), infuse magic, superior attunement, mechanical servant", 
	"magic item analysis, tool expertise, wondrous invention (4 items), infuse magic, superior attunement (additional item), mechanical servant", 
	"magic item analysis, tool expertise, wondrous invention (4 items), infuse magic, superior attunement (additional item), mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (4 items), infuse magic, superior attunement (additional item), mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (4 items), infuse magic, superior attunement (additional item), mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (4 items), infuse magic, superior attunement (additional item), mechanical servant",
	"magic item analysis, tool expertise, wondrous invention (5 items), infuse magic, superior attunement (additional item), mechanical servant, soul of artifice"
};
static std::string barbarianAbilities[] = {
	"rage (2 uses, +2 damage), unarmored defense",
	"rage (2 uses, +2 damage), unarmored defense, reckless attack, danger sense",
	"rage (3 uses, +2 damage), unarmored defense, reckless attack, danger sense",
	"rage (3 uses, +2 damage), unarmored defense, reckless attack, danger sense",
	"rage (3 uses, +2 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement",
	"rage (4 uses, +2 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement",
	"rage (4 uses, +2 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct",
	"rage (4 uses, +2 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct",
	"rage (4 uses, +3 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (1 die)",
	"rage (4 uses, +3 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (1 die)",
	"rage (4 uses, +3 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (1 die), relentless rage",
	"rage (5 uses, +3 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (1 die), relentless rage",
	"rage (5 uses, +3 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (2 dice), relentless rage",
	"rage (5 uses, +3 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (2 dice), relentless rage",
	"rage (5 uses, +3 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (2 dice), relentless rage, persistent rage",
	"rage (5 uses, +4 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (2 dice), relentless rage, persistent rage",
	"rage (6 uses, +4 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (3 dice), relentless rage, persistent rage",
	"rage (6 uses, +4 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (3 dice), relentless rage, persistent rage, indomitable might",
	"rage (6 uses, +4 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (3 dice), relentless rage, persistent rage, indomitable might",
	"rage (unlimited uses, +4 damage), unarmored defense, reckless attack, danger sense, extra attack, fast movement, feral instinct, brutal critical (3 dice), relentless rage, persistent rage, indomitable might, primal champion"
};
static std::string bardAbilities[] = {
	"bardic inspiration (d6)",
	"bardic inspiration (d6), jack of all trades, song of rest (d6)",
	"bardic inspiration (d6), jack of all trades, song of rest (d6), expertise",
	"bardic inspiration (d6), jack of all trades, song of rest (d6), expertise",
	"bardic inspiration (d8), jack of all trades, song of rest (d6), expertise, font of inspiration",
	"bardic inspiration (d8), jack of all trades, song of rest (d6), expertise, font of inspiration, countercharm",
	"bardic inspiration (d8), jack of all trades, song of rest (d6), expertise, font of inspiration, countercharm",
	"bardic inspiration (d8), jack of all trades, song of rest (d6), expertise, font of inspiration, countercharm",
	"bardic inspiration (d8), jack of all trades, song of rest (d8), expertise, font of inspiration, countercharm",
	"bardic inspiration (d10), jack of all trades, song of rest (d8), expertise (x2), font of inspiration, countercharm, magical secrets",
	"bardic inspiration (d10), jack of all trades, song of rest (d8), expertise (x2), font of inspiration, countercharm, magical secrets",
	"bardic inspiration (d10), jack of all trades, song of rest (d8), expertise (x2), font of inspiration, countercharm, magical secrets",
	"bardic inspiration (d10), jack of all trades, song of rest (d10), expertise (x2), font of inspiration, countercharm, magical secrets",
	"bardic inspiration (d10), jack of all trades, song of rest (d10), expertise (x2), font of inspiration, countercharm, magical secrets (x2)",
	"bardic inspiration (d12), jack of all trades, song of rest (d10), expertise (x2), font of inspiration, countercharm, magical secrets (x2)",
	"bardic inspiration (d12), jack of all trades, song of rest (d10), expertise (x2), font of inspiration, countercharm, magical secrets (x2)",
	"bardic inspiration (d12), jack of all trades, song of rest (d12), expertise (x2), font of inspiration, countercharm, magical secrets (x2)",
	"bardic inspiration (d12), jack of all trades, song of rest (d12), expertise (x2), font of inspiration, countercharm, magical secrets (x3)",
	"bardic inspiration (d12), jack of all trades, song of rest (d12), expertise (x2), font of inspiration, countercharm, magical secrets (x3)",
	"bardic inspiration (d12), jack of all trades, song of rest (d12), expertise (x2), font of inspiration, countercharm, magical secrets (x3), superior inspiration"
};
static std::string clericAbilities[] = {
	"", 
	"channel divinity (1/rest)",
	"channel divinity (1/rest)",
	"channel divinity (1/rest)",
	"channel divinity (1/rest), destroy undead (cr 1/2)",
	"channel divinity (2/rest), destroy undead (cr 1/2)",
	"channel divinity (2/rest), destroy undead (cr 1/2)",
	"channel divinity (2/rest), destroy undead (cr 1)",
	"channel divinity (2/rest), destroy undead (cr 1)",
	"channel divinity (2/rest), destroy undead (cr 1), divine intervention",
	"channel divinity (2/rest), destroy undead (cr 2), divine intervention",
	"channel divinity (2/rest), destroy undead (cr 2), divine intervention",
	"channel divinity (2/rest), destroy undead (cr 2), divine intervention",
	"channel divinity (2/rest), destroy undead (cr 3), divine intervention",
	"channel divinity (2/rest), destroy undead (cr 3), divine intervention",
	"channel divinity (2/rest), destroy undead (cr 3), divine intervention",
	"channel divinity (2/rest), destroy undead (cr 4), divine intervention",
	"channel divinity (3/rest), destroy undead (cr 4), divine intervention",
	"channel divinity (3/rest), destroy undead (cr 4), divine intervention",
	"channel divinity (3/rest), destroy undead (cr 4), divine intervention improved"
};
static std::string druidAbilities[] = {
	"druidic language",
	"druidic language, wild shape (max cr 1/4, no flying or swimming)",
	"druidic language, wild shape (max cr 1/4, no flying or swimming)",
	"druidic language, wild shape (max cr 1/2, no flying)",
	"druidic language, wild shape (max cr 1/2, no flying)",
	"druidic language, wild shape (max cr 1/2, no flying)",
	"druidic language, wild shape (max cr 1/2, no flying)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1)",
	"druidic language, wild shape (max cr 1), timeless body, beast spells",
	"druidic language, wild shape (max cr 1), timeless body, beast spells",
	"druidic language, wild shape (max cr 1), timeless body, beast spells, archdruid"
};
static std::string fighterAbilities[] = {
	"second wind",
	"second wind, action surge (one use)",
	"second wind, action surge (one use)",
	"second wind, action surge (one use)",
	"second wind, action surge (one use), extra attack",
	"second wind, action surge (one use), extra attack",
	"second wind, action surge (one use), extra attack",
	"second wind, action surge (one use), extra attack",
	"second wind, action surge (one use), extra attack, indomitable (one use)",
	"second wind, action surge (one use), extra attack, indomitable (one use)",
	"second wind, action surge (one use), extra attack (2), indomitable (one use)",
	"second wind, action surge (one use), extra attack (2), indomitable (one use)",
	"second wind, action surge (one use), extra attack (2), indomitable (two uses)",
	"second wind, action surge (one use), extra attack (2), indomitable (two uses)",
	"second wind, action surge (one use), extra attack (2), indomitable (two uses)",
	"second wind, action surge (one use), extra attack (2), indomitable (two uses)",
	"second wind, action surge (one use), extra attack (2), indomitable (two uses)",
	"second wind, action surge (two uses), extra attack (2), indomitable (three uses)",
	"second wind, action surge (two uses), extra attack (2), indomitable (three uses)",
	"second wind, action surge (two uses), extra attack (2), indomitable (three uses)",
	"second wind, action surge (two uses), extra attack (3), indomitable (three uses)"
};
static std::string monkAbilities[] = {
	"unarmored defense, martial arts (1d4)",
	"unarmored defense, martial arts (1d4), ki (2 points), unarmored movement (+10 ft)",
	"unarmored defense, martial arts (1d4), ki (3 points), unarmored movement (+10 ft), deflect missiles",
	"unarmored defense, martial arts (1d4), ki (4 points), unarmored movement (+10 ft), deflect missiles, slow fall",
	"unarmored defense, martial arts (1d6), ki (5 points), unarmored movement (+10 ft), deflect missiles, slow fall, extra attack, stunning strike",
	"unarmored defense, martial arts (1d6), ki (6 points), unarmored movement (+15 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes",
	"unarmored defense, martial arts (1d6), ki (7 points), unarmored movement (+15 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind",
	"unarmored defense, martial arts (1d6), ki (8 points), unarmored movement (+15 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind",
	"unarmored defense, martial arts (1d6), ki (9 points), unarmored movement improved (+15 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind",
	"unarmored defense, martial arts (1d6), ki (10 points), unarmored movement improved (+20 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body",
	"unarmored defense, martial arts (1d8), ki (11 points), unarmored movement improved (+20 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body",
	"unarmored defense, martial arts (1d8), ki (12 points), unarmored movement improved (+20 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body",
	"unarmored defense, martial arts (1d8), ki (13 points), unarmored movement improved (+20 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon",
	"unarmored defense, martial arts (1d8), ki (14 points), unarmored movement improved (+25 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon, diamond soul",
	"unarmored defense, martial arts (1d8), ki (15 points), unarmored movement improved (+25 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon, diamond soul, timeless body",
	"unarmored defense, martial arts (1d8), ki (16 points), unarmored movement improved (+25 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon, diamond soul, timeless body",
	"unarmored defense, martial arts (1d10), ki (17 points), unarmored movement improved (+25 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon, diamond soul, timeless body",
	"unarmored defense, martial arts (1d10), ki (18 points), unarmored movement improved (+30 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon, diamond soul, timeless body, empty body",
	"unarmored defense, martial arts (1d10), ki (19 points), unarmored movement improved (+30 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon, diamond soul, timeless body, empty body",
	"unarmored defense, martial arts (1d10), ki (20 points), unarmored movement improved (+30 ft), deflect missiles, slow fall, extra attack, stunning strike, ki-empowered strikes, evasion, stillness of mind, purity of body, tongue of the sun and moon, diamond soul, timeless body, empty body, perfect self"
};
static std::string mysticAbilities[] = {
	"psionics (1 talent, 1 discipline, 4 psi points, 2 psi limit)",
	"psionics (1 talent, 1 discipline, 6 psi points, 2 psi limit), mystical recovery, telepathy",
	"psionics (2 talent, 2 discipline, 14 psi points, 3 psi limit), mystical recovery, telepathy",
	"psionics (2 talent, 2 discipline, 17 psi points, 3 psi limit), mystical recovery, telepathy, strength of mind",
	"psionics (2 talent, 3 discipline, 27 psi points, 5 psi limit), mystical recovery, telepathy, strength of mind",
	"psionics (2 talent, 3 discipline, 32 psi points, 5 psi limit), mystical recovery, telepathy, strength of mind",
	"psionics (2 talent, 4 discipline, 38 psi points, 6 psi limit), mystical recovery, telepathy, strength of mind",
	"psionics (2 talent, 4 discipline, 44 psi points, 6 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (1d8)",
	"psionics (2 talent, 5 discipline, 57 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (1d8)",	
	"psionics (3 talent, 5 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (1d8), consumptive power",
	"psionics (3 talent, 5 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (1d8), consumptive power, psionic mastery (1/day)",
	"psionics (3 talent, 6 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (1d8), consumptive power, psionic mastery (1/day)",
	"psionics (3 talent, 6 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (1d8), consumptive power, psionic mastery (2/day)",
	"psionics (3 talent, 6 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (2/day)",
	"psionics (3 talent, 7 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (3/day)",
	"psionics (3 talent, 7 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (3/day)",
	"psionics (3 talent, 7 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (3/day)",
	"psionics (4 talent, 7 discipline, 64 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (4/day)",
	"psionics (4 talent, 8 discipline, 71 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (4/day)",
	"psionics (4 talent, 8 discipline, 71 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (4/day)",
	"psionics (4 talent, 8 discipline, 71 psi points, 7 psi limit), mystical recovery, telepathy, strength of mind, potent psionics (2d8), consumptive power, psionic mastery (4/day), psionic body"
};
static std::string paladinAbilities[] = {
	"divine sense, lay on hands",
	"divine sense, lay on hands, divine smite",
	"divine sense, lay on hands, divine smite, divine health",
	"divine sense, lay on hands, divine smite, divine health",
	"divine sense, lay on hands, divine smite, divine health, extra attack",
	"divine sense, lay on hands, divine smite, divine health, extra attack, aura of protection",
	"divine sense, lay on hands, divine smite, divine health, extra attack, aura of protection",
	"divine sense, lay on hands, divine smite, divine health, extra attack, aura of protection",
	"divine sense, lay on hands, divine smite, divine health, extra attack, aura of protection",
	"divine sense, lay on hands, divine smite, divine health, extra attack, aura of protection, aura of courage",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, aura of protection, aura of courage",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, aura of protection, aura of courage",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, aura of protection, aura of courage",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, aura of protection, aura of courage, cleansing touch",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, aura of protection, aura of courage, cleansing touch",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, aura of protection, aura of courage, cleansing touch",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, aura of protection, aura of courage, cleansing touch",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, improved aura of protection, improved aura of courage, cleansing touch",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, improved aura of protection, improved aura of courage, cleansing touch",
	"divine sense, lay on hands, improved divine smite, divine health, extra attack, improved aura of protection, improved aura of courage, cleansing touch"
};
static std::string rangerAbilities[] = {
	"favored enemy, natural explorer",
	"favored enemy, natural explorer",
	"favored enemy, natural explorer, primeval awareness",
	"favored enemy, natural explorer, primeval awareness",
	"favored enemy, natural explorer, primeval awareness, extra attack",
	"favored enemy, natural explorer, primeval awareness, extra attack",
	"favored enemy, natural explorer, primeval awareness, extra attack",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight, vanish",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight, vanish",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight, vanish",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight, vanish",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight, vanish, feral senses",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight, vanish, feral senses",
	"favored enemy, natural explorer, primeval awareness, extra attack, land's stride, hide in plain sight, vanish, feral senses, foe slayer"
};
static std::string rogueAbilities[] = {
	"sneak attack (1d6), expertise, thieves' cant",
	"sneak attack (1d6), expertise, thieves' cant, cunning action",
	"sneak attack (2d6), expertise, thieves' cant, cunning action",
	"sneak attack (2d6), expertise, thieves' cant, cunning action",
	"sneak attack (3d6), expertise, thieves' cant, cunning action, uncanny dodge",
	"sneak attack (3d6), expertise (x2), thieves' cant, cunning action, uncanny dodge",
	"sneak attack (4d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion",
	"sneak attack (4d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion",
	"sneak attack (5d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion",
	"sneak attack (5d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion",
	"sneak attack (6d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent",
	"sneak attack (6d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent",
	"sneak attack (7d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent",
	"sneak attack (7d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent, blindsense",
	"sneak attack (8d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent, blindsense, slippery mind",
	"sneak attack (8d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent, blindsense, slippery mind",
	"sneak attack (9d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent, blindsense, slippery mind",
	"sneak attack (9d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent, blindsense, slippery mind, elusive",
	"sneak attack (10d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent, blindsense, slippery mind, elusive",
	"sneak attack (10d6), expertise (x2), thieves' cant, cunning action, uncanny dodge, evasion, reliable talent, blindsense, slippery mind, elusive, stroke of luck"
};
static std::string sorcererAbilities[] = {
	"",
	"font of magic (2 sorcerery points)",
	"font of magic (3 sorcerery points), metamagic (2)",
	"font of magic (4 sorcerery points), metamagic (2)",
	"font of magic (5 sorcerery points), metamagic (2)",
	"font of magic (6 sorcerery points), metamagic (2)",
	"font of magic (7 sorcerery points), metamagic (2)",
	"font of magic (8 sorcerery points), metamagic (2)",
	"font of magic (9 sorcerery points), metamagic (2)",
	"font of magic (10 sorcerery points), metamagic (3)",
	"font of magic (11 sorcerery points), metamagic (3)",
	"font of magic (12 sorcerery points), metamagic (3)",
	"font of magic (13 sorcerery points), metamagic (3)",
	"font of magic (14 sorcerery points), metamagic (3)",
	"font of magic (15 sorcerery points), metamagic (3)",
	"font of magic (16 sorcerery points), metamagic (3)",
	"font of magic (17 sorcerery points), metamagic (4)",
	"font of magic (18 sorcerery points), metamagic (4)",
	"font of magic (19 sorcerery points), metamagic (4)",
	"font of magic (20 sorcerery points), metamagic (4), sorcerous restoration"
};
static std::string warlockAbilities[] = {
	"",
	"eldritch invocations (2)",
	"eldritch invocations (2), pact boon",
	"eldritch invocations (2), pact boon",
	"eldritch invocations (3), pact boon",
	"eldritch invocations (3), pact boon",
	"eldritch invocations (4), pact boon",
	"eldritch invocations (4), pact boon",
	"eldritch invocations (5), pact boon",
	"eldritch invocations (5), pact boon",
	"eldritch invocations (5), pact boon, mystic arcanum (6th)",
	"eldritch invocations (6), pact boon, mystic arcanum (6th)",
	"eldritch invocations (6), pact boon, mystic arcanum (7th)",
	"eldritch invocations (6), pact boon, mystic arcanum (7th)",
	"eldritch invocations (7), pact boon, mystic arcanum (8th)",
	"eldritch invocations (7), pact boon, mystic arcanum (8th)",
	"eldritch invocations (7), pact boon, mystic arcanum (9th)",
	"eldritch invocations (8), pact boon, mystic arcanum (9th)",
	"eldritch invocations (8), pact boon, mystic arcanum (9th)",
	"eldritch invocations (8), pact boon, mystic arcanum (9th), eldritch master"
};
static std::string wizardAbilities[] = {
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery",
	"arcane recovery, spell mastery",
	"arcane recovery, spell mastery",
	"arcane recovery, spell mastery, signature spell"
};
static std::string runeScribeAbilities[] = {
	"rune lore, runic magic",
	"rune lore, runic magic, runic discovery",
	"rune lore, runic magic, runic discovery (2)",
	"rune lore, runic magic, runic discovery (2), living rune",
	"rune lore, runic magic, runic discovery (3), living rune, rune mastery"
};
static std::string emptyAbilities[] = {
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};
static std::string* classAbilities[] = {
	artificerAbilities, barbarianAbilities, bardAbilities, clericAbilities, druidAbilities, fighterAbilities, monkAbilities,
	mysticAbilities, paladinAbilities, rangerAbilities, rogueAbilities, sorcererAbilities, warlockAbilities,
	wizardAbilities, runeScribeAbilities, emptyAbilities
};
#pragma endregion class abilities
/* 
 * Some class abilities need more random decisions to be made. 
 * I'm gonna list those here (some of 'em, at least)
 * Artificer: Wondrous invention (for each of the 5 wondrous inventions you get)
 * Warlock: invocations, pact boon
 */

//spellcasting

//backgrounds
#pragma region
static int numBackgrounds = 31;
static std::string backgrounds[] = {
	"acolyte", "anthropologist", "archaeologist", "charlatan", "city watch", "clan crafter", "cloistered scholar", "courtier", 
	"criminal", "entertainer", "faction agent", "far traveler", "folk hero", "gladiator", "guild artisan/merchant", "haunted one",
	"hermit", "inheritor", "knight", "knight of the order", "mercenary veteren", "noble", "outlander", "pirate", "sage", 
	"sailor", "soldier", "urban bounty hunter", "urchin", "uthgardt tribe member", "waterdhavian noble"
};
#pragma endregion backgrounds


#pragma region 
int level;
int levelMainClass, levelSecondaryClass; //levels for main and secondary class
int race; //represents position in races[]
int secondaryRace; //represents secondary race if primary race is revanant
int subrace; //represents position in subraces[race][]
int class_; //represents position in classes[] -- unfortunately have to use _ because C++ already has classes
int secondaryClass; //represents secondary class if primary class is rune scribe or multiclass
int subclass; //represents position in subclasses[class_][]
int secondarySubclass; //represents position in subclasses[secondaryClass][]
int background; //represents position in backgrounds[]
enum statNames { STR, DEX, CON, INT, WIS, CHA };
int stats[6]; //0=strength, 1=dexterity, ... 5=charisma
int statBonuses[6]; //0=strength, 1=dexterity, ... 5=charisma
int hitPoints;
#pragma endregion variables


int main()
{
	//region stuff not being worked on -- move the endregion to right before whatever part you're working on if you're gonna be moving back and forth between static stuff and dynamic stuff frequently
#pragma region
	//seed rng
	srand(time(NULL));

	do {
		//generate character level -- random is between 1 and 20
		std::cout << "Input level of character (or leave blank for a random level) : ";
		std::string input;
		std::getline(std::cin, input);
		if (input.compare("") == STR_COMPARE_TRUE) {
			level = 0; 
		}
		else {
			try {
				level = std::stoi(input);
			}
			catch (std::invalid_argument exc) {
				std::cout << "Failure to read in input, getting a random level instead" << std::endl;
				level = 0;
			}
			catch (std::out_of_range exc) {
				std::cout << "Failure to read in input, getting a random level instead" << std::endl;
				level = 0;
			}
		}
		if (level < 1 || level > 20) {
			level = rand() % 20 + 1;
		}

		//choose race & subrace
		race = rand() % numberOfRaces;
		if (races[race].compare("revenant") == STR_COMPARE_TRUE) {
			secondaryRace = rand() % numberOfRaces;
			while (races[secondaryRace].compare("revenant") == STR_COMPARE_TRUE) {
				secondaryRace = rand() % numberOfRaces;
			}
		}
		else {
			secondaryRace = -1;
		}
		if (subraceNums[race] > 0) {
			subrace = rand() % subraceNums[race];
		}
		else {
			subrace = -1;
		}

		//choose class & subclass
		class_ = rand() % numberOfClasses;
		if (classes[class_].compare("multiclass") == STR_COMPARE_TRUE) {
			if (level > 1) {
				while (classes[class_].compare("multiclass") == STR_COMPARE_TRUE || classes[class_].compare("rune scribe") == STR_COMPARE_TRUE) {
					class_ = rand() % numberOfClasses; //keep rerolling until you get a base class
				}
				secondaryClass = rand() % numberOfClasses;
				while (classes[secondaryClass].compare("multiclass") == STR_COMPARE_TRUE || classes[secondaryClass].compare("rune scribe") == STR_COMPARE_TRUE || secondaryClass == class_) {
					secondaryClass = rand() % numberOfClasses; //keep rerolling until secondary is a base class that is not main class
				}
			}
			else { //level <= 1
				while (classes[class_].compare("multiclass") == STR_COMPARE_TRUE || classes[class_].compare("rune scribe") == STR_COMPARE_TRUE) {
					class_ = rand() % numberOfClasses;
				}
			}
		}
		else if (classes[class_].compare("rune scribe") == STR_COMPARE_TRUE) {
			if (level > 1) {
				secondaryClass = class_; //set secondary class to be rune scribe, then we'll set main class as base class
				class_ = rand() % numberOfClasses;
				while (classes[class_].compare("multiclass") == STR_COMPARE_TRUE || classes[class_].compare("rune scribe") == STR_COMPARE_TRUE) {
					class_ = rand() % numberOfClasses; //keep rerolling until secondary is a base class
				}
			}
			else { //level <= 1
				while (classes[class_].compare("multiclass") == STR_COMPARE_TRUE || classes[class_].compare("rune scribe") == STR_COMPARE_TRUE) {
					class_ = rand() % numberOfClasses;
				}
			}
		}
		else {
			secondaryClass = -1;
		}
		if (subclassNums[class_] > 0) {
			subclass = rand() % subclassNums[class_];
		}
		else {
			subclass = -1;
		}
		if (secondaryClass >= 0) {
			if (subclassNums[secondaryClass] > 0) {
				secondarySubclass = rand() % subclassNums[secondaryClass];
			}
			else {
				secondarySubclass = -1;
			}
		}
		else {
			secondarySubclass = -1;
		}

		//choose background
		background = rand() % numBackgrounds;

		//set stats
		//following the colville system - reroll if you don't have at least 2 stats that are fifteen or above
		int counter;
		do
		{
			for (int i = 0; i < 6; i++) {
				int roll1 = rand() % 6;
				int roll2 = rand() % 6;
				int roll3 = rand() % 6;
				int roll4 = rand() % 6;
				if (roll1 <= roll2 && roll1 <= roll3 && roll1 <= roll4) {
					stats[i] = roll2 + roll3 + roll4;
				}
				else if (roll2 <= roll1 && roll2 <= roll3 && roll2 <= roll4) {
					stats[i] = roll1 + roll3 + roll4;
				}
				else if (roll3 <= roll1 && roll3 <= roll2 && roll3 <= roll4) {
					stats[i] = roll1 + roll2 + roll4;
				}
				else {
					stats[i] = roll1 + roll2 + roll3;
				}
			}
			counter = 0;
			for (int i = 0; i < 6; i++) {
				if (stats[i] > 14) {
					counter++;
					if (counter > 1) {
						break;
					}
				}
			}
		} while (counter < 2);
#pragma endregion stuff not being worked on

		//racial stat upgrades
		for (int i = 0; i < 6; i++)
		{
			if (subrace == -1 || races[race].compare("dragonborn") == STR_COMPARE_TRUE) {
				stats[i] += subraceStats[race][0][i];
				//since the standard i'm using is to assign a -1 to the subrace of races w/out subs, this gets us to go into the array at the right point
				//additionally, since certain races (like dragonborn) have the same stats regardless of subrace, they get dropped into this category too
			}
			else {
				stats[i] += subraceStats[race][subrace][i];
			}
		}
		int bonusRandomAssignedPoints = 0;
		if (races[race].compare("half-elf") == STR_COMPARE_TRUE || (races[race].compare("human") == STR_COMPARE_TRUE && subraces[race][subrace].compare("variant") == STR_COMPARE_TRUE)) {
			bonusRandomAssignedPoints = 2;
		}
		for (int i = 0; i < bonusRandomAssignedPoints; i++)	{
			stats[rand() % 6]++;
		}
		if (races[race].compare("minotaur") == STR_COMPARE_TRUE) {
			int randomStatToBoost = rand() % 3; //minotaur gets +1 to STR, INT, or WIS 
			if (randomStatToBoost == 0) {
				stats[0]++;
			}
			else if (randomStatToBoost == 1) {
				stats[3]++;
			}
			else {
				stats[4]++;
			}
		}

		//feats or stat upgrades
		int numFeats = levelMainClass / 4 + levelSecondaryClass / 4;
		if (levelMainClass == 19 || levelSecondaryClass == 19) {
			numFeats++;
		}
			//fighter bonus feats
		if ((classes[class_].compare("fighter") == STR_COMPARE_TRUE && levelMainClass >= 6) || (classes[secondaryClass].compare("fighter") == STR_COMPARE_TRUE && levelSecondaryClass >= 6)) {
			numFeats++;
		}
		if ((classes[class_].compare("fighter") == STR_COMPARE_TRUE && levelMainClass >= 14) || (classes[secondaryClass].compare("fighter") == STR_COMPARE_TRUE && levelSecondaryClass >= 14)) {
			numFeats++;
		}
			//variant human bonus feat
		if ((races[race].compare("human") == STR_COMPARE_TRUE) && subraces[race][subrace].compare("variant") == STR_COMPARE_TRUE) {
			numFeats++;
		}
		for (int i = 0; i < numFeats; i++) {
			//randomly assign +1 to two stats -- eventually I'll include feats too
			int statToUpgrade = rand() % 6;
			stats[statToUpgrade]++;
			statToUpgrade = rand() % 6;
			stats[statToUpgrade]++;
		}
		for (int i = 0; i < 6; i++) {
			statBonuses[i] = (stats[i] - 10) / 2;
			if (stats[i] < 10 && stats[i] % 2 == 1) {
				statBonuses[i]--;
				//need to reduce bonus by 1 if stat is < 10 and is odd, because of the way computers divide negative numbers
			}
		}

		//choose levels for each class and hit point totals
		if (secondaryClass != -1) {
			levelMainClass = rand() % level + 1; //minimum 1 level in each class
			if (levelMainClass == level) {
				levelMainClass -= 1;
			}
			levelSecondaryClass = level - levelMainClass;
			if (classes[secondaryClass].compare("rune scribe") == STR_COMPARE_TRUE && levelSecondaryClass > 5) {
				//can't have a rune scribe with more than 5 levels
				int levelDiff = levelSecondaryClass - 5;
				levelSecondaryClass = 5;
				levelMainClass += levelDiff;
			}
		}
		else { //secondaryClass == -1, meaning the character has no secondary class
			levelMainClass = level;
			levelSecondaryClass = 0;
		}
		hitPoints = classHitDice[class_] + statBonuses[CON]; //max hP for first level
		for (int i = 1; i < levelMainClass; i++) {
			int pointsToAdd = rand() % classHitDice[class_] + 1 + statBonuses[CON];
			if (pointsToAdd < 1) {
				pointsToAdd = 1;
			}
			hitPoints += pointsToAdd;
		}
		for (int i = 0; i < levelSecondaryClass; i++) {
			int pointsToAdd = rand() % classHitDice[secondaryClass] + 1 + statBonuses[CON];
			if (pointsToAdd < 1) {
				pointsToAdd = 1;
			}
			hitPoints += pointsToAdd;
		}
		if (races[race].compare("dwarf") == STR_COMPARE_TRUE && subraces[race][subrace].compare("hill") == STR_COMPARE_TRUE) {
			hitPoints += level;
			//hill dwarves get +1 hp per level
		}
		if (races[race].compare("tiefling") == STR_COMPARE_TRUE && subraces[race][subrace].compare("abyssal") == STR_COMPARE_TRUE) {
			int bonusHP = level / 2;
			if (bonusHP == 0) {
				bonusHP++;
			}
			hitPoints += bonusHP;
		}

		//print the results to std::out
			//level
		std::string toPrint = "You are a level " + std::to_string(level) + " ";
		if (levelMainClass != level) {
			//indicate the levels of their 2 classes
			toPrint += "(" + std::to_string(levelMainClass) + "/" + std::to_string(levelSecondaryClass) + ") ";
		}
			//race & subrace
		if (secondaryRace == -1) { //i.e. don't have a secondaryRace
			if (subrace == -1) {
				toPrint += races[race];
			}
			else {
				toPrint += subraces[race][subrace] + " " + races[race];
			}
		}
		else { //do have a secondaryRace
			if (subrace == -1) {
				toPrint += races[secondaryRace] + " " + races[race];
			}
			else {
				toPrint += races[secondaryRace] + " " + subraces[race][subrace] + " " + races[race];
			}
		}
			//class & subclass
		toPrint += " ";
		if (secondaryClass == -1) { //i.e. don't have a secondaryClass
			if (subclass == -1) {
				toPrint += classes[class_];
			}
			else {
				toPrint += subclasses[class_][subclass] + " " + classes[class_];
			}
		}
		else { //do have a secondaryClass
			if (subclass == -1) {
				if (secondarySubclass == -1) {
					toPrint += classes[class_] + " / " + classes[secondaryClass];
				}
				else {
					toPrint += classes[class_] + " / " + subclasses[secondaryClass][secondarySubclass] + " " + classes[secondaryClass];
				}
			}
			else {
				if (secondarySubclass == -1) {
					toPrint += subclasses[class_][subclass] + " " + classes[class_] + " / " + classes[secondaryClass];
				}
				else {
					toPrint += subclasses[class_][subclass] + " " + classes[class_] + " / " + subclasses[secondaryClass][secondarySubclass] + " " + classes[secondaryClass] ;
				}
			}
		}
			//background
		if (backgrounds[background][0] == 'a' || backgrounds[background][0] == 'e' || backgrounds[background][0] == 'i' ||
			backgrounds[background][0] == 'o' || backgrounds[background][0] == 'u') {
			toPrint += " from an " + backgrounds[background] + " background";
		}
		else {
			toPrint += " from a " + backgrounds[background] + " background";
		}
			//stats
		std::cout << toPrint << std::endl;

		toPrint = "STR: " + std::to_string(stats[STR]) + " (" + std::to_string(statBonuses[STR]);
		toPrint += ") DEX: " + std::to_string(stats[DEX]) + " (" + std::to_string(statBonuses[DEX]);
		toPrint += ") CON: " + std::to_string(stats[CON]) + " (" + std::to_string(statBonuses[CON]);
		toPrint += ") INT: " + std::to_string(stats[INT]) + " (" + std::to_string(statBonuses[INT]);
		toPrint += ") WIS: " + std::to_string(stats[WIS]) + " (" + std::to_string(statBonuses[WIS]);
		toPrint += ") CHA: " + std::to_string(stats[CHA]) + " (" + std::to_string(statBonuses[CHA]) + ")";
		std::cout << toPrint << std::endl;

		toPrint = "HP: " + std::to_string(hitPoints);
		std::cout << toPrint << std::endl;

			//race abilities
		std::cout << std::endl << "Racial abilities: " << raceAbilities[race];
		if (raceAbilities[race].compare("") != STR_COMPARE_TRUE) {
			std::cout << ", ";
		}
		std::cout << subraceAbilities[race][subrace] << std::endl;

			//class abilities
		std::cout << std::endl << "Class abilities: " << classAbilities[class_][levelMainClass - 1] << std::endl;
		if (secondaryClass != -1) {
			std::cout << std::endl << "Second class abilities: " << classAbilities[secondaryClass][levelSecondaryClass - 1] << std::endl;
		}

		std::getchar();
		std::cout << std::endl << "-----------------------------------------" << std::endl << std::endl;
	} while (true);
}