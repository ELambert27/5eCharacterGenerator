//5eCharacterGenerator
//Makes a random D&D 5e character, either at the given level or at a random level if no level is given

//TODO: BUGFIX: Rune scribe class abilities get printed off with part of another class's abilities present, due to 
	//nature of rune scribe having zero subclasses. Need to write into the printing a check to ensure we don't print when
	//subclass == -1 

//TODO: can cut down on the amount of memory used by class abilities & subclass abilities by adjusting the system of
	//choosing abilities to just list abilities gained at certain levels and the levels those abilities are gained
	//Didn't do this before because not all classes & subclasses gain abilities at the same levels, but we can make
	//it work with a little bit of extra work
//TODO: multiclass currently limits to a maximum of 2 classes. could upgrade to doing any number of classes by switching to
	//a variable-size data structure like std::vector
//TODO: Add some checking on rune scribes to make sure that a character who rolls rune scribe meets the required prereqs

#include <iostream>
#include <string>
#include <time.h>
#include <random>
#include <stdexcept>

#define STR_COMPARE_TRUE 0

//races and subraces
#pragma region
static int numberOfRaces = 32;
static std::string races[] = {
	"revenant", "aarakocra", "aasimar", "bugbear", "changeling", "dragonborn", "dwarf", "elf", "firbolg", "genasi", "gnome",
	"goblin", "goliath", "half-elf", "halfling", "half-orc", "hobgoblin", "human", "kenku", "kobold", "lizardfolk", "minotaur",
	"orc", "shifter", "tabaxi", "tiefling", "tortle", "triton", "warforged", "yuan-ti pureblood", "gith", "centaur"
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
static std::string duergarDwarfAbilities = "120 foot darkvision, sunlight sensitivity, duergar magic, advantage on saves vs illusions/being charmed/being paralyzed";
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
static std::string drowElfAbilities = "120 foot darkvision, sunlight sensitivity, drow weapon training, drow magic";
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
static std::string koboldAbilities = "60 foot darkvision, sunlight sensitivity, grovel cower and beg, pack tactics";
	//kobolds have no subraces, therefore no subrace abilities

static int subLizardfolkStats[] = { 0, 0, 2, 0, 1, 0 };
static int* lizardfolkSubraceStats[] = { subLizardfolkStats };
static std::string lizardfolkAbilities = "30 foot swim speed, hold breath 15 mins, bite, hungry jaws, natural armor, cunning artisan, proficiency in 2 of animal handling/nature/perception/stealth/survival";
	//lizaradfolk have no subraces, therefore no subrace abilities

static int subMinotaurStats[] = { 2, 0, 1, 0, 0, 0 };
static int* minotaurSubraceStats[] = { subMinotaurStats };
static std::string minotaurAbilities = "horns, goring rush, hammering horns, proficiency with intimidation, hybrid nature";
	//minotaurs gain proficiency with intimidation, which needs to be accounted for later
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

static int subCentaurStats[] = { 2, 0, 0, 0, 1, 0 };
static int* centaurSubraceStats[] = { subCentaurStats };
static std::string centaurAbilities = "charge, hooves, equine build, proficiency with survival, hybrid nature";
	//centaurs have increased base land speed, which will need to be accounted for later
	//centaurs gain a bonus skill proficiency which needs to be accounted for later
	//centaurs have no subraces, therefore no subrace abilities


static int subraceNums[] = {
	numEmptySubs, numEmptySubs, numAasimarSubs, numEmptySubs, numEmptySubs, numDragonbornSubs, numDwarfSubs, numElfSubs, numEmptySubs, numGenasiSubs, numGnomeSubs,
	numEmptySubs, numEmptySubs, numEmptySubs, numHalflingSubs, numEmptySubs, numEmptySubs, numHumanSubs, numEmptySubs, numEmptySubs, numEmptySubs, numEmptySubs,
	numEmptySubs, numShifterSubs, numEmptySubs, numTieflingSubs, numEmptySubs, numEmptySubs, numEmptySubs, numEmptySubs, numGithSubs, numEmptySubs
};
static std::string* subraces[] = {
	emptySubrace, emptySubrace, aasimarSubrace, emptySubrace, emptySubrace, dragonbornSubrace, dwarfSubrace, elfSubrace, emptySubrace, genasiSubrace, gnomeSubrace,
	emptySubrace, emptySubrace, emptySubrace, halflingSubrace, emptySubrace, emptySubrace, humanSubrace, emptySubrace, emptySubrace, emptySubrace, emptySubrace,
	emptySubrace, shifterSubrace, emptySubrace, tieflingSubrace, emptySubrace, emptySubrace, emptySubrace, emptySubrace, githSubrace, emptySubrace
};
static int** subraceStats[] = {
	revenantSubraceStats, aarakocraSubraceStats, aasimarSubraceStats, bugbearSubraceStats, changelingSubraceStats, dragonbornSubraceStats, dwarfSubraceStats, elfSubraceStats, firbolgSubraceStats, genasiSubraceStats, gnomeSubraceStats,
	goblinSubraceStats, goliathSubraceStats, halfElfSubraceStats, halflingSubraceStats, halfOrcSubraceStats, hobgoblinSubraceStats, humanSubraceStats, kenkuSubraceStats, koboldSubraceStats, lizardfolkSubraceStats, minotaurSubraceStats,
	orcSubraceStats, shifterSubraceStats, tabaxiSubraceStats, tieflingSubraceStats, tortleSubraceStats, tritonSubraceStats, warforgedSubraceStats, yuanTiSubraceStats, githSubraceStats, centaurSubraceStats
};
static std::string raceAbilities[] = {
	revenantAbilities, aarakocraAbilities, aasimarAbilities, bugbearAbilities, changelingAbilities, dragonbornAbilities, dwarfAbilities, elfAbilities, firbolgAbilities, genasiAbilities, gnomeAbilities,
	goblinAbilities, goliathAbilities, halfElfAbilities, halflingAbilities, halfOrcAbilities, hobgoblinAbilities, humanAbilities, kenkuAbilities, koboldAbilities, lizardfolkAbilities, minotaurAbilities,
	orcAbilities, shifterAbilities, tabaxiAbilities, tieflingAbilities, tortleAbilities, tritonAbilities, warforgedAbilities, yuanTiAbilities, githAbilities, centaurAbilities
};
static std::string* subraceAbilities[] = {
	emptySubraceAbilities, emptySubraceAbilities, aasimarSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, dragonbornSubraceAbilities, dwarfSubraceAbilities, elfSubraceAbilities, emptySubraceAbilities, genasiSubraceAbilities, gnomeSubraceAbilities,
	emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, halflingSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, humanSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities,
	emptySubraceAbilities, shifterSubraceAbilities, emptySubraceAbilities, tieflingSubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, emptySubraceAbilities, githSubraceAbilities, emptySubraceAbilities
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
static std::string rangerSubclass[] = { "beast master", "gloom stalker", "horizon walker", "hunter", "monster slayer", "primeval guardian" };
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
static std::string alchemistArtificerAbilities[] = {
	"alchemist's satchel, alchemical formula (fire, acid, and one other)",
	"alchemist's satchel, alchemical formula (fire, acid, and one other)",
	"alchemist's satchel, alchemical formula (fire, acid, and two other)",
	"alchemist's satchel, alchemical formula (fire, acid, and two other)",
	"alchemist's satchel, alchemical formula (fire, acid, and two other)",
	"alchemist's satchel, alchemical formula (fire, acid, and two other)",
	"alchemist's satchel, alchemical formula (fire, acid, and two other)",
	"alchemist's satchel, alchemical formula (fire, acid, and two other)",
	"alchemist's satchel, alchemical formula (fire, acid, and three other)",
	"alchemist's satchel, alchemical formula (fire, acid, and three other)",
	"alchemist's satchel, alchemical formula (fire, acid, and three other)",
	"alchemist's satchel, alchemical formula (fire, acid, and three other)",
	"alchemist's satchel, alchemical formula (fire, acid, and three other)",
	"alchemist's satchel, alchemical formula (fire, acid, and four other)",
	"alchemist's satchel, alchemical formula (fire, acid, and four other)",
	"alchemist's satchel, alchemical formula (fire, acid, and four other)",
	"alchemist's satchel, alchemical formula (fire, acid, and five other)",
	"alchemist's satchel, alchemical formula (fire, acid, and five other)",
	"alchemist's satchel, alchemical formula (fire, acid, and five other)",
	"alchemist's satchel, alchemical formula (fire, acid, and five other)"
};
	//alchemist has random alchemical formulas that need to be accounted for later
static std::string gunslingerArtificerAbilites[] = {
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave, piercing round",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave, piercing round",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave, piercing round",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave, piercing round, explosive round",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave, piercing round, explosive round",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave, piercing round, explosive round",
	"proficiency with smith's tools, gain the mending cantrip, thunder cannon, arcane magazine, thunder monger, blast wave, piercing round, explosive round"
};
	//gunslinger gains the mending cantrip, which needs to be accounted for later
static std::string* artificerSubclassAbilities[] = {
	alchemistArtificerAbilities, gunslingerArtificerAbilites
};
#pragma endregion artificer abilities
#pragma region
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
static std::string ancestralGuardianBarbarianAbilities[] = { 
	"",
	"",
	"ancestral protectors",
	"ancestral protectors",
	"ancestral protectors",
	"ancestral protectors, spirit shield",
	"ancestral protectors, spirit shield",
	"ancestral protectors, spirit shield",
	"ancestral protectors, spirit shield",
	"ancestral protectors, spirit shield, consult the spirits",
	"ancestral protectors, spirit shield, consult the spirits",
	"ancestral protectors, spirit shield, consult the spirits",
	"ancestral protectors, spirit shield, consult the spirits",
	"ancestral protectors, spirit shield, consult the spirits, vengeful ancestors",
	"ancestral protectors, spirit shield, consult the spirits, vengeful ancestors",
	"ancestral protectors, spirit shield, consult the spirits, vengeful ancestors",
	"ancestral protectors, spirit shield, consult the spirits, vengeful ancestors",
	"ancestral protectors, spirit shield, consult the spirits, vengeful ancestors",
	"ancestral protectors, spirit shield, consult the spirits, vengeful ancestors",
	"ancestral protectors, spirit shield, consult the spirits, vengeful ancestors"
};
static std::string battleragerBarbarianAbilities[] = { 
	"",
	"",
	"battlerager armor",
	"battlerager armor",
	"battlerager armor",
	"battlerager armor, reckless abandon",
	"battlerager armor, reckless abandon",
	"battlerager armor, reckless abandon",
	"battlerager armor, reckless abandon",
	"battlerager armor, reckless abandon, battlerager charge",
	"battlerager armor, reckless abandon, battlerager charge",
	"battlerager armor, reckless abandon, battlerager charge",
	"battlerager armor, reckless abandon, battlerager charge",
	"battlerager armor, reckless abandon, battlerager charge, spiked retribution",
	"battlerager armor, reckless abandon, battlerager charge, spiked retribution",
	"battlerager armor, reckless abandon, battlerager charge, spiked retribution",
	"battlerager armor, reckless abandon, battlerager charge, spiked retribution",
	"battlerager armor, reckless abandon, battlerager charge, spiked retribution",
	"battlerager armor, reckless abandon, battlerager charge, spiked retribution",
	"battlerager armor, reckless abandon, battlerager charge, spiked retribution"
};
static std::string berserkerBarbarianAbilities[] = { 
	"", 
	"",
	"frenzy",
	"frenzy",
	"frenzy",
	"frenzy, mindless rage",
	"frenzy, mindless rage",
	"frenzy, mindless rage",
	"frenzy, mindless rage",
	"frenzy, mindless rage, intimidating presence",
	"frenzy, mindless rage, intimidating presence",
	"frenzy, mindless rage, intimidating presence",
	"frenzy, mindless rage, intimidating presence",
	"frenzy, mindless rage, intimidating presence, retaliation",
	"frenzy, mindless rage, intimidating presence, retaliation",
	"frenzy, mindless rage, intimidating presence, retaliation",
	"frenzy, mindless rage, intimidating presence, retaliation",
	"frenzy, mindless rage, intimidating presence, retaliation",
	"frenzy, mindless rage, intimidating presence, retaliation",
	"frenzy, mindless rage, intimidating presence, retaliation"
};
static std::string stormHeraldBarbarianAbilities[] = { 
	"",
	"",
	"storm aura",
	"storm aura",
	"storm aura",
	"storm aura, storm soul",
	"storm aura, storm soul",
	"storm aura, storm soul",
	"storm aura, storm soul",
	"storm aura, storm soul, shielding storm",
	"storm aura, storm soul, shielding storm",
	"storm aura, storm soul, shielding storm",
	"storm aura, storm soul, shielding storm",
	"storm aura, storm soul, shielding storm, raging storm",
	"storm aura, storm soul, shielding storm, raging storm",
	"storm aura, storm soul, shielding storm, raging storm",
	"storm aura, storm soul, shielding storm, raging storm",
	"storm aura, storm soul, shielding storm, raging storm",
	"storm aura, storm soul, shielding storm, raging storm",
	"storm aura, storm soul, shielding storm, raging storm"
};
	//storm herald has a random environment that affects their rage that needs to be accounted for later
static std::string totemBarbarianAbilities[] = { 
	"",
	"",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker, totemic attunement",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker, totemic attunement",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker, totemic attunement",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker, totemic attunement",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker, totemic attunement",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker, totemic attunement",
	"spirit seeker (beast sense and speak with animals as rituals), totem spirit, aspect of the beast, spirit walker, totemic attunement"
};
	//totem has a random totem that needs to be accounted for later
	//totem also gains some spells that need to be accounted for later, though they can only cast them as rituals
static std::string zealotBarbarianAbilities[] = { 
	"",
	"",
	"divine fury, warrior of the gods",
	"divine fury, warrior of the gods",
	"divine fury, warrior of the gods",
	"divine fury, warrior of the gods, fanatical focus",
	"divine fury, warrior of the gods, fanatical focus",
	"divine fury, warrior of the gods, fanatical focus",
	"divine fury, warrior of the gods, fanatical focus",
	"divine fury, warrior of the gods, fanatical focus, zealous presence",
	"divine fury, warrior of the gods, fanatical focus, zealous presence",
	"divine fury, warrior of the gods, fanatical focus, zealous presence",
	"divine fury, warrior of the gods, fanatical focus, zealous presence",
	"divine fury, warrior of the gods, fanatical focus, zealous presence, rage beyond death",
	"divine fury, warrior of the gods, fanatical focus, zealous presence, rage beyond death",
	"divine fury, warrior of the gods, fanatical focus, zealous presence, rage beyond death",
	"divine fury, warrior of the gods, fanatical focus, zealous presence, rage beyond death",
	"divine fury, warrior of the gods, fanatical focus, zealous presence, rage beyond death",
	"divine fury, warrior of the gods, fanatical focus, zealous presence, rage beyond death",
	"divine fury, warrior of the gods, fanatical focus, zealous presence, rage beyond death"
};
static std::string* barbarianSubclassAbilities[] = {
	ancestralGuardianBarbarianAbilities, battleragerBarbarianAbilities, berserkerBarbarianAbilities, stormHeraldBarbarianAbilities, totemBarbarianAbilities, zealotBarbarianAbilities
};
#pragma endregion barbarian abilities
#pragma region
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
static std::string glamourBardAbilities[] = {
	"",
	"",
	"mantle of inspiration, enthralling performance",
	"mantle of inspiration, enthralling performance",
	"mantle of inspiration, enthralling performance",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty, unbreakable majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty, unbreakable majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty, unbreakable majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty, unbreakable majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty, unbreakable majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty, unbreakable majesty",
	"mantle of inspiration, enthralling performance, mantle of majesty, unbreakable majesty"
};
static std::string loreBardAbilities[] = {
	"",
	"",
	"+3 skill proficiencies, cutting words",
	"+3 skill proficiencies, cutting words",
	"+3 skill proficiencies, cutting words",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets",
	"+3 skill proficiencies, cutting words, additional magical secrets, peerless skill",
	"+3 skill proficiencies, cutting words, additional magical secrets, peerless skill",
	"+3 skill proficiencies, cutting words, additional magical secrets, peerless skill",
	"+3 skill proficiencies, cutting words, additional magical secrets, peerless skill",
	"+3 skill proficiencies, cutting words, additional magical secrets, peerless skill",
	"+3 skill proficiencies, cutting words, additional magical secrets, peerless skill",
	"+3 skill proficiencies, cutting words, additional magical secrets, peerless skill"
};
	//lore bard has 3 bonus skill proficiencies that need to be accounted for later
	//lore bard also has additional magical secrets that need to be accounted for later
static std::string satireBardAbilities[] = {
	"",
	"",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight, fool's luck",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight, fool's luck",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight, fool's luck",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight, fool's luck",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight, fool's luck",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight, fool's luck",
	"proficiency in thieves tools/sleight of hand/one other skill, tumbling fool, fool's insight, fool's luck",
};
	//satire bard has additional skill & tool proficiencies that need to be accounted for later
static std::string swordBardAbilities[] = {
	"",
	"",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack, master's flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack, master's flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack, master's flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack, master's flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack, master's flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack, master's flourish",
	"proficiency with scimitars/medium armor, can use proficient weapons as spellcasting foci, fighting style, blade flourish, extra attack, master's flourish"
};
	//sword bard gains additional weapon and armor proficiencies that need to be accounted for later
	//sword bard gains a random fighting style that needs to be accounted for later
static std::string valorBardAbilities[] = {
	"",
	"",
	"proficiency with medium armor/shields/martial weapons, combat inspiration",
	"proficiency with medium armor/shields/martial weapons, combat inspiration",
	"proficiency with medium armor/shields/martial weapons, combat inspiration",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack, battle magic",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack, battle magic",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack, battle magic",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack, battle magic",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack, battle magic",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack, battle magic",
	"proficiency with medium armor/shields/martial weapons, combat inspiration, extra attack, battle magic"
};
static std::string whisperBardAbilities[] = {
	"",
	"",
	"psychic blades, words of terror",
	"psychic blades, words of terror",
	"psychic blades, words of terror",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers",
	"psychic blades, words of terror, mantle of whispers, shadow lore",
	"psychic blades, words of terror, mantle of whispers, shadow lore",
	"psychic blades, words of terror, mantle of whispers, shadow lore",
	"psychic blades, words of terror, mantle of whispers, shadow lore",
	"psychic blades, words of terror, mantle of whispers, shadow lore",
	"psychic blades, words of terror, mantle of whispers, shadow lore",
	"psychic blades, words of terror, mantle of whispers, shadow lore"
};
static std::string* bardSubclassAbilities[] = {
	glamourBardAbilities, loreBardAbilities, satireBardAbilities, swordBardAbilities, valorBardAbilities, whisperBardAbilities
};
#pragma endregion bard abilities
#pragma region
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
static std::string arcanaClericAbilities[] = {
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting, arcane mastery (gain bonus wizard spells)",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting, arcane mastery (gain bonus wizard spells)",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting, arcane mastery (gain bonus wizard spells)",
	"bonus domain spells, proficiency in arcana, gain 2 wizard cantrips, channel divinity: arcane abjuration, spell breaker, potent spellcasting, arcane mastery (gain bonus wizard spells)"
};
	//arcana clerics gain proficiency in arcana which needs to be accounted for later
	//arcana clerics gain 2 bonus wizard cantrips which needs to be accounted for later
static std::string cityClericAbilities[] = {
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike, express transit",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike, express transit",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike, express transit",
	"bonus domain spells, gain on/off cantrip, proficiency with sidearms/vehicles (land), heart of the city, channel divinity: spirits of the city, block watch, divine strike, express transit"
};
	//city clerics gain a bonus cantrip that needs to be accounted for later
	//city clerics gain bonus tool & weapon proficiencies that need to be accounted for later
static std::string deathClericAbilities[] = {
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike, improved reaper",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike, improved reaper",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike, improved reaper",
	"bonus domain spells, proficiency with martial weapons, bonus necromancy cantrip, reaper, channel divinity: touch of death, inescapable destruction, divine strike, improved reaper"
};
	//death clerics gain a bonus necromancy cantrip that needs to be accounted for later
static std::string forgeClericAbilities[] = {
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike, saint of forge and fire",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike, saint of forge and fire",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike, saint of forge and fire",
	"bonus domain spells, proficiency with heavy armor/smith's tools, blessing of the forge, channel divinity: artisan's blessing, soul of the forge, divine strike, saint of forge and fire"
};
	//forge clerics gain bonus proficiencies that need to be accounted for later
static std::string graveClericAbilities[] = {
	"bonus domain spells, circle of mortality, eyes of the grave",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting, keeper of souls",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting, keeper of souls",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting, keeper of souls",
	"bonus domain spells, circle of mortality, eyes of the grave, channel divinity: path to the grave, sentinel at death's door, potent spellcasting, keeper of souls"
};
static std::string knowledgeClericAbilities[] = {
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting, visions of the past",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting, visions of the past",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting, visions of the past",
	"bonus domain spells, proficiency in 2 languages, proficiency and expertise in (2 of) arcana/history/nature/religion, channel divinity: knowledge of the ages, channel divinity: read thoughts, potent spellcasting, visions of the past"
};
	//knowledge clerics gain language and skill proficiencies that need to be accounted for later
static std::string lifeClericAbilities[] = {
	"bonus domain spells, proficiency with heavy armor, disciple of life",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike, supreme healing",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike, supreme healing",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike, supreme healing",
	"bonus domain spells, proficiency with heavy armor, disciple of life, channel divinity: preserve life, blessed healer, divine strike, supreme healing"
};
	//life clerics gain bonus armor proficiency that needs to be accounted for later
static std::string lightClericAbilities[] = {
	"bonus domain spells, learn light cantrip, warding flare",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting, corona of light",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting, corona of light",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting, corona of light",
	"bonus domain spells, learn light cantrip, warding flare, channel divinity: radiance of the dawn, improved flare, potent spellcasting, corona of light"
};
	//light clerics gain the light cantrip, which needs to be accounted for later
static std::string natureClericAbilities[] = {
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike, master of nature",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike, master of nature",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike, master of nature",
	"bonus domain spells, learn one druid cantrip, proficiency in (1 of) animal handling/nature/survival, proficiency with heavy armor, channel divinity: charm animals and plants, dampen elements, divine strike, master of nature"
};
	//nature clerics have bonus spells & skills & armor, which need to be accounted for later
static std::string protectionClericAbilities[] = {
	"bonus domain spells, proficiency with heavy armor, shield of the faithful",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike, indomitable defense",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike, indomitable defense",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike, indomitable defense",
	"bonus domain spells, proficiency with heavy armor, shield of the faithful, channel divinity: radiant defense, blessed healer, divine strike, indomitable defense"
};
	//protection clerics have heavy armor prof, which needs to be accounted for later
static std::string tempestClericAbilities[] = {
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike, stormborn",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike, stormborn",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike, stormborn",
	"bonus domain spells, proficiency with martial weapons/heavy armor, wrath of the storm, channel divinity: destructive wrath, thunderbolt strike, divine strike, stormborn"
};
	//tempest clerics gain martial weapon & heavy armor proficiencies, which need to be accounted for later
static std::string trickeryClericAbilities[] = {
	"bonus domain spells, blessing of the trickster",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike, improved duplicity",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike, improved duplicity",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike, improved duplicity",
	"bonus domain spells, blessing of the trickster, channel divinity: invoke duplicity, cloak of shadows, divine strike, improved duplicity"
};
static std::string warClericAbilities[] = {
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike, avatar of battle",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike, avatar of battle",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike, avatar of battle",
	"bonus domain spells, proficiency with martial weapons/heavy armor, war priest, channel divinity: guided strike, channel divinity: war god's blessing, divine strike, avatar of battle"
};
	//war clerics gain weapon & armor proficiencies that need to be accounted for later
static std::string orderClericAbilities[] = {
	"bonus domain spells, proficiency with heavy armor, voice of authority",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike, order's wrath",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike, order's wrath",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike, order's wrath",
	"bonus domain spells, proficiency with heavy armor, voice of authority, channel divinity: order's demand, order's dominion, divine strike, order's wrath"
};
	//order clerics gain armor proficiency which needs to be acccounted for later
static std::string* clericSubclassAbilities[] = {
	arcanaClericAbilities, cityClericAbilities, deathClericAbilities, forgeClericAbilities, graveClericAbilities, knowledgeClericAbilities, lifeClericAbilities, lightClericAbilities, natureClericAbilities, protectionClericAbilities, tempestClericAbilities, trickeryClericAbilities, warClericAbilities, orderClericAbilities
};
	//all cleric subclasses gain access to bonus spells that need to be accounted for later
#pragma endregion cleric abilities
#pragma region
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
static std::string dreamsDruidAbilities[] = {
	"",
	"balm of the summer court",
	"balm of the summer court",
	"balm of the summer court",
	"balm of the summer court",
	"balm of the summer court, hearth of moonlight and shadow",
	"balm of the summer court, hearth of moonlight and shadow",
	"balm of the summer court, hearth of moonlight and shadow",
	"balm of the summer court, hearth of moonlight and shadow",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths, walker in dreams",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths, walker in dreams",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths, walker in dreams",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths, walker in dreams",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths, walker in dreams",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths, walker in dreams",
	"balm of the summer court, hearth of moonlight and shadow, hidden paths, walker in dreams"
};
static std::string landDruidAbilities[] = {
	"",
	"bonus druid cantrip, natural recovery",
	"bonus druid cantrip, natural recovery, bonus environmental spells",
	"bonus druid cantrip, natural recovery, bonus environmental spells",
	"bonus druid cantrip, natural recovery, bonus environmental spells",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward, nature's sanctuary",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward, nature's sanctuary",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward, nature's sanctuary",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward, nature's sanctuary",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward, nature's sanctuary",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward, nature's sanctuary",
	"bonus druid cantrip, natural recovery, bonus environmental spells, land's stride, nature's ward, nature's sanctuary"
};
	//land druid gains bonus cantrips/spells which need to be accounted for later
static std::string moonDruidAbilities[] = {
	"",
	"combat wild shape, circle forms",
	"combat wild shape, circle forms",
	"combat wild shape, circle forms",
	"combat wild shape, circle forms",
	"combat wild shape, circle forms, primal strike",
	"combat wild shape, circle forms, primal strike",
	"combat wild shape, circle forms, primal strike",
	"combat wild shape, circle forms, primal strike",
	"combat wild shape, circle forms, primal strike, elemental wild shape",
	"combat wild shape, circle forms, primal strike, elemental wild shape",
	"combat wild shape, circle forms, primal strike, elemental wild shape",
	"combat wild shape, circle forms, primal strike, elemental wild shape",
	"combat wild shape, circle forms, primal strike, elemental wild shape, thousand forms",
	"combat wild shape, circle forms, primal strike, elemental wild shape, thousand forms",
	"combat wild shape, circle forms, primal strike, elemental wild shape, thousand forms",
	"combat wild shape, circle forms, primal strike, elemental wild shape, thousand forms",
	"combat wild shape, circle forms, primal strike, elemental wild shape, thousand forms",
	"combat wild shape, circle forms, primal strike, elemental wild shape, thousand forms",
	"combat wild shape, circle forms, primal strike, elemental wild shape, thousand forms"
};
static std::string shepherdDruidAbilities[] = {
	"",
	"speech of the woods, spirit totem",
	"speech of the woods, spirit totem",
	"speech of the woods, spirit totem",
	"speech of the woods, spirit totem",
	"speech of the woods, spirit totem, mighty summoner",
	"speech of the woods, spirit totem, mighty summoner",
	"speech of the woods, spirit totem, mighty summoner",
	"speech of the woods, spirit totem, mighty summoner",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit, faithful summons",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit, faithful summons",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit, faithful summons",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit, faithful summons",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit, faithful summons",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit, faithful summons",
	"speech of the woods, spirit totem, mighty summoner, guardian spirit, faithful summons"
};
static std::string sporesDruidAbilities[] = {
	"",
	"gain chill touch cantrip, halo of spores, symbiotic entity",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores, fungul body",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores, fungul body",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores, fungul body",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores, fungul body",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores, fungul body",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores, fungul body",
	"gain chill touch cantrip, halo of spores, symbiotic entity, bonus circle spells, fungul infestation, spreading spores, fungul body"
};
	//spores druid gains bonus cantrip/spells which need to be accounted for later
static std::string twilightDruidAbilities[] = {
	"",
	"harvest's scythe",
	"harvest's scythe",
	"harvest's scythe",
	"harvest's scythe",
	"harvest's scythe, speech beyond the grave",
	"harvest's scythe, speech beyond the grave",
	"harvest's scythe, speech beyond the grave",
	"harvest's scythe, speech beyond the grave",
	"harvest's scythe, speech beyond the grave, watcher at the threshold",
	"harvest's scythe, speech beyond the grave, watcher at the threshold",
	"harvest's scythe, speech beyond the grave, watcher at the threshold",
	"harvest's scythe, speech beyond the grave, watcher at the threshold",
	"harvest's scythe, speech beyond the grave, watcher at the threshold, paths of the dead",
	"harvest's scythe, speech beyond the grave, watcher at the threshold, paths of the dead",
	"harvest's scythe, speech beyond the grave, watcher at the threshold, paths of the dead",
	"harvest's scythe, speech beyond the grave, watcher at the threshold, paths of the dead",
	"harvest's scythe, speech beyond the grave, watcher at the threshold, paths of the dead",
	"harvest's scythe, speech beyond the grave, watcher at the threshold, paths of the dead",
	"harvest's scythe, speech beyond the grave, watcher at the threshold, paths of the dead"
};
static std::string* druidSubclassAbilities[] = {
	dreamsDruidAbilities, landDruidAbilities, moonDruidAbilities, shepherdDruidAbilities, sporesDruidAbilities, twilightDruidAbilities
};
#pragma endregion druid abilities
#pragma region 
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
	//fighter gains things like fighting style that need to be accounted for later
static std::string arcaneArcherFighterAbilities[] = {
	"",
	"",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (2 options)",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (2 options)",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (2 options)",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (2 options)",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (3 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (3 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (3 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (4 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (4 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (4 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (4 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (4 options), magic arrow, curving shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (5 options), magic arrow, curving shot, ever-ready shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (5 options), magic arrow, curving shot, ever-ready shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (5 options), magic arrow, curving shot, ever-ready shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (5 options), magic arrow, curving shot, ever-ready shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (5 options), magic arrow, curving shot, ever-ready shot",
	"proficiency in (1 of) arcana/nature, gain (1 of) prestidigitation/druidcraft cantrip, arcane shot (5 options), magic arrow, curving shot, ever-ready shot"
};
	//arcane archer gains skill, cantrip, and arcane shot options which need to be accounted for later
static std::string purpleDragonKnightFighterAbilities[] = {
	"",
	"",
	"rallying cry",
	"rallying cry",
	"rallying cry",
	"rallying cry",
	"rallying cry, royal envoy (bonus skill proficiency)",
	"rallying cry, royal envoy (bonus skill proficiency)",
	"rallying cry, royal envoy (bonus skill proficiency)",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge, bulwark",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge, bulwark",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge, bulwark",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge, bulwark",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge, bulwark",
	"rallying cry, royal envoy (bonus skill proficiency), inspiring surge, bulwark"
};
	//purple dragon knights gain bonus skills that need to be accounted for later
static std::string battleMasterFighterAbilities[] = {
	"",
	"",
	"combat superiority (3 maneuvers), proficiency with 1 type of artisan's tools",
	"combat superiority (3 maneuvers), proficiency with 1 type of artisan's tools",
	"combat superiority (3 maneuvers), proficiency with 1 type of artisan's tools",
	"combat superiority (3 maneuvers), proficiency with 1 type of artisan's tools",
	"combat superiority (5 maneuvers), proficiency with 1 type of artisan's tools, know your enemy",
	"combat superiority (5 maneuvers), proficiency with 1 type of artisan's tools, know your enemy",
	"combat superiority (5 maneuvers), proficiency with 1 type of artisan's tools, know your enemy",
	"combat superiority (7 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority",
	"combat superiority (7 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority",
	"combat superiority (7 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority",
	"combat superiority (7 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority",
	"combat superiority (7 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority",
	"combat superiority (9 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority, relentless",
	"combat superiority (9 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority, relentless",
	"combat superiority (9 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority, relentless",
	"combat superiority (9 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority, relentless",
	"combat superiority (9 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority, relentless",
	"combat superiority (9 maneuvers), proficiency with 1 type of artisan's tools, know your enemy, improved combat superiority, relentless"
};
	//battle master gains tool proficiencies which need to be accounted for later
static std::string bruteFighterAbilities[] = {
	"",
	"",
	"brute force (1d4)",
	"brute force (1d4)",
	"brute force (1d4)",
	"brute force (1d4)",
	"brute force (1d4), brutish durability",
	"brute force (1d4), brutish durability",
	"brute force (1d4), brutish durability",
	"brute force (1d6), brutish durability, additional fighting style",
	"brute force (1d6), brutish durability, additional fighting style",
	"brute force (1d6), brutish durability, additional fighting style",
	"brute force (1d6), brutish durability, additional fighting style",
	"brute force (1d6), brutish durability, additional fighting style",
	"brute force (1d6), brutish durability, additional fighting style, devastating critical",
	"brute force (1d8), brutish durability, additional fighting style, devastating critical",
	"brute force (1d8), brutish durability, additional fighting style, devastating critical",
	"brute force (1d8), brutish durability, additional fighting style, devastating critical, survivor",
	"brute force (1d8), brutish durability, additional fighting style, devastating critical, survivor",
	"brute force (1d10), brutish durability, additional fighting style, devastating critical, survivor"
};
	//brute fighters gain bonus fighting style that needs to be accounted for
static std::string cavalierFighterAbiltiies[] = {
	"",
	"",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line, ferocious charger",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line, ferocious charger",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line, ferocious charger",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line, ferocious charger, vigilant defender",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line, ferocious charger, vigilant defender",
	"proficiency in (1 of) animal handling/history/insight/performance/persuasion/one language, born to the saddle, unwavering mark, warding maneuver, hold the line, ferocious charger, vigilant defender"
};
	//cavalier gains bonus skill or language proficiency which needs to be accounted for later
static std::string championFighterAbilities[] = {
	"",
	"",
	"improved critical",
	"improved critical",
	"improved critical",
	"improved critical",
	"improved critical, remarkable athlete",
	"improved critical, remarkable athlete",
	"improved critical, remarkable athlete",
	"improved critical, remarkable athlete, additional fighting style",
	"improved critical, remarkable athlete, additional fighting style",
	"improved critical, remarkable athlete, additional fighting style",
	"improved critical, remarkable athlete, additional fighting style",
	"improved critical, remarkable athlete, additional fighting style",
	"superior critical, remarkable athlete, additional fighting style",
	"superior critical, remarkable athlete, additional fighting style",
	"superior critical, remarkable athlete, additional fighting style",
	"superior critical, remarkable athlete, additional fighting style, survivor",
	"superior critical, remarkable athlete, additional fighting style, survivor",
	"superior critical, remarkable athlete, additional fighting style, survivor"
};
	//champion gains an additional fighting style that needs to be accounted for later
static std::string eldritchKnightFighterAbilities[] = {
	"",
	"",
	"spellcasting, weapon bond",
	"spellcasting, weapon bond",
	"spellcasting, weapon bond",
	"spellcasting, weapon bond",
	"spellcasting, weapon bond, war magic",
	"spellcasting, weapon bond, war magic",
	"spellcasting, weapon bond, war magic",
	"spellcasting, weapon bond, war magic, eldritch strike",
	"spellcasting, weapon bond, war magic, eldritch strike",
	"spellcasting, weapon bond, war magic, eldritch strike",
	"spellcasting, weapon bond, war magic, eldritch strike",
	"spellcasting, weapon bond, war magic, eldritch strike",
	"spellcasting, weapon bond, war magic, eldritch strike, arcane charge",
	"spellcasting, weapon bond, war magic, eldritch strike, arcane charge",
	"spellcasting, weapon bond, war magic, eldritch strike, arcane charge",
	"spellcasting, weapon bond, improved war magic, eldritch strike, arcane charge",
	"spellcasting, weapon bond, improved war magic, eldritch strike, arcane charge",
	"spellcasting, weapon bond, improved war magic, eldritch strike, arcane charge"
};
	//eldritch knight gains spells, which needs to be accounted for later
static std::string knightFighterAbilities[] = {
	"",
	"",
	"born to the saddle, implacable mark",
	"born to the saddle, implacable mark",
	"born to the saddle, implacable mark",
	"born to the saddle, implacable mark",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language)",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language)",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language)",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line, rapid strike",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line, rapid strike",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line, rapid strike",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line, rapid strike, defender's blade",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line, rapid strike, defender's blade",
	"born to the saddle, implacable mark, noble cavalry (bonus skills or language), hold the line, rapid strike, defender's blade"
};
	//knight fighter has bonus skills or language that needs to be accounted for later
static std::string samuraiFighterAbilities[] = {
	"",
	"",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit, rapid strike",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit, rapid strike",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit, rapid strike",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit, rapid strike, strength before death",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit, rapid strike, strength before death",
	"proficiency in (1 of) history/insight/performance/persuasion/one language, fighting spirit, elegant courtier, tireless spirit, rapid strike, strength before death"
};
	//samurai gains bonus skill or language that needs to be accounted for later
static std::string sharpshooterFighterAbilities[] = {
	"",
	"",
	"steady aim",
	"steady aim",
	"steady aim",
	"steady aim",
	"steady aim, careful eyes",
	"steady aim, careful eyes",
	"steady aim, careful eyes",
	"steady aim, careful eyes, close-quarters shooting",
	"steady aim, careful eyes, close-quarters shooting",
	"steady aim, careful eyes, close-quarters shooting",
	"steady aim, careful eyes, close-quarters shooting",
	"steady aim, careful eyes, close-quarters shooting",
	"steady aim, careful eyes, close-quarters shooting, rapid strike",
	"steady aim, careful eyes, close-quarters shooting, rapid strike",
	"steady aim, careful eyes, close-quarters shooting, rapid strike",
	"steady aim, careful eyes, close-quarters shooting, rapid strike, snap shot",
	"steady aim, careful eyes, close-quarters shooting, rapid strike, snap shot",
	"steady aim, careful eyes, close-quarters shooting, rapid strike, snap shot"
};
static std::string monsterHunterFighterAbilities[] = {
	"",
	"",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority, relentless",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority, relentless",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority, relentless",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority, relentless",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority, relentless",
	"proficiency in (2 of) arcana/history/insight/investigation/nature/perception/one tool, combat superiority, hunter's mysticism, monster slayer, improved combat superiority, relentless"
};
static std::string* fighterSubclassAbilities[] = {
	arcaneArcherFighterAbilities, purpleDragonKnightFighterAbilities, battleMasterFighterAbilities, bruteFighterAbilities, cavalierFighterAbiltiies, championFighterAbilities, eldritchKnightFighterAbilities, knightFighterAbilities, samuraiFighterAbilities, sharpshooterFighterAbilities, monsterHunterFighterAbilities
};
#pragma endregion fighter abilities
#pragma region
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
static std::string drunkenMasterMonkAbilities[] = {
	"",
	"",
	"proficiency with performance/brewer's tools, drunken technique",
	"proficiency with performance/brewer's tools, drunken technique",
	"proficiency with performance/brewer's tools, drunken technique",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck, intoxicated frenzy",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck, intoxicated frenzy",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck, intoxicated frenzy",
	"proficiency with performance/brewer's tools, drunken technique, tipsy sway, drunkard's luck, intoxicated frenzy"
};
	//drunken master gains bonus skill & tool proficiencies that need to be accounted for later
static std::string fourElementsMonkAbilities[] = {
	"",
	"",
	"disciple of the elements (elemental attunement and 1 other)",
	"disciple of the elements (elemental attunement and 1 other)",
	"disciple of the elements (elemental attunement and 1 other)",
	"disciple of the elements (elemental attunement and 2 others)",
	"disciple of the elements (elemental attunement and 2 others)",
	"disciple of the elements (elemental attunement and 2 others)",
	"disciple of the elements (elemental attunement and 2 others)",
	"disciple of the elements (elemental attunement and 2 others)",
	"disciple of the elements (elemental attunement and 3 others)",
	"disciple of the elements (elemental attunement and 3 others)",
	"disciple of the elements (elemental attunement and 3 others)",
	"disciple of the elements (elemental attunement and 3 others)",
	"disciple of the elements (elemental attunement and 3 others)",
	"disciple of the elements (elemental attunement and 3 others)",
	"disciple of the elements (elemental attunement and 4 others)",
	"disciple of the elements (elemental attunement and 4 others)",
	"disciple of the elements (elemental attunement and 4 others)",
	"disciple of the elements (elemental attunement and 4 others)"
};
	//four elements gains elemental abilities/spells which need to be accounted for later
static std::string kensaiMonkAbilities[] = {
	"",
	"",
	"path of the kensai",
	"path of the kensai",
	"path of the kensai",
	"path of the kensai, one with the blade",
	"path of the kensai, one with the blade",
	"path of the kensai, one with the blade",
	"path of the kensai, one with the blade",
	"path of the kensai, one with the blade",
	"path of the kensai, one with the blade, sharpen the blade",
	"path of the kensai, one with the blade, sharpen the blade",
	"path of the kensai, one with the blade, sharpen the blade",
	"path of the kensai, one with the blade, sharpen the blade",
	"path of the kensai, one with the blade, sharpen the blade",
	"path of the kensai, one with the blade, sharpen the blade",
	"path of the kensai, one with the blade, sharpen the blade, unerring accuracy",
	"path of the kensai, one with the blade, sharpen the blade, unerring accuracy",
	"path of the kensai, one with the blade, sharpen the blade, unerring accuracy",
	"path of the kensai, one with the blade, sharpen the blade, unerring accuracy"
};
static std::string longDeathMonkAbilities[] = {
	"",
	"",
	"touch of death",
	"touch of death",
	"touch of death",
	"touch of death, hour of reaping",
	"touch of death, hour of reaping",
	"touch of death, hour of reaping",
	"touch of death, hour of reaping",
	"touch of death, hour of reaping",
	"touch of death, hour of reaping, mastery of death",
	"touch of death, hour of reaping, mastery of death",
	"touch of death, hour of reaping, mastery of death",
	"touch of death, hour of reaping, mastery of death",
	"touch of death, hour of reaping, mastery of death",
	"touch of death, hour of reaping, mastery of death",
	"touch of death, hour of reaping, mastery of death, touch of the long death",
	"touch of death, hour of reaping, mastery of death, touch of the long death",
	"touch of death, hour of reaping, mastery of death, touch of the long death",
	"touch of death, hour of reaping, mastery of death, touch of the long death"
};
static std::string openHandMonkAbilities[] = {
	"",
	"",
	"open hand technique",
	"open hand technique",
	"open hand technique",
	"open hand technique, wholeness of body",
	"open hand technique, wholeness of body",
	"open hand technique, wholeness of body",
	"open hand technique, wholeness of body",
	"open hand technique, wholeness of body",
	"open hand technique, wholeness of body, tranquility",
	"open hand technique, wholeness of body, tranquility",
	"open hand technique, wholeness of body, tranquility",
	"open hand technique, wholeness of body, tranquility",
	"open hand technique, wholeness of body, tranquility",
	"open hand technique, wholeness of body, tranquility",
	"open hand technique, wholeness of body, tranquility, quivering palm",
	"open hand technique, wholeness of body, tranquility, quivering palm",
	"open hand technique, wholeness of body, tranquility, quivering palm",
	"open hand technique, wholeness of body, tranquility, quivering palm"
};
static std::string shadowMonkAbilities[] = {
	"",
	"",
	"shadow arts",
	"shadow arts",
	"shadow arts",
	"shadow arts, shadow step",
	"shadow arts, shadow step",
	"shadow arts, shadow step",
	"shadow arts, shadow step",
	"shadow arts, shadow step",
	"shadow arts, shadow step, cloak of shadows",
	"shadow arts, shadow step, cloak of shadows",
	"shadow arts, shadow step, cloak of shadows",
	"shadow arts, shadow step, cloak of shadows",
	"shadow arts, shadow step, cloak of shadows",
	"shadow arts, shadow step, cloak of shadows",
	"shadow arts, shadow step, cloak of shadows, opportunist",
	"shadow arts, shadow step, cloak of shadows, opportunist",
	"shadow arts, shadow step, cloak of shadows, opportunist",
	"shadow arts, shadow step, cloak of shadows, opportunist"
};
static std::string sunSoulMonkAbilities[] = {
	"",
	"",
	"radiant sun bolt",
	"radiant sun bolt",
	"radiant sun bolt",
	"radiant sun bolt, searing arc strike",
	"radiant sun bolt, searing arc strike",
	"radiant sun bolt, searing arc strike",
	"radiant sun bolt, searing arc strike",
	"radiant sun bolt, searing arc strike",
	"radiant sun bolt, searing arc strike, searing sunburst",
	"radiant sun bolt, searing arc strike, searing sunburst",
	"radiant sun bolt, searing arc strike, searing sunburst",
	"radiant sun bolt, searing arc strike, searing sunburst",
	"radiant sun bolt, searing arc strike, searing sunburst",
	"radiant sun bolt, searing arc strike, searing sunburst",
	"radiant sun bolt, searing arc strike, searing sunburst, sun shield",
	"radiant sun bolt, searing arc strike, searing sunburst, sun shield",
	"radiant sun bolt, searing arc strike, searing sunburst, sun shield",
	"radiant sun bolt, searing arc strike, searing sunburst, sun shield"
};
static std::string tranquilityMonkAbilities[] = {
	"",
	"",
	"path of tranquility, healing hands",
	"path of tranquility, healing hands",
	"path of tranquility, healing hands",
	"path of tranquility, healing hands, emissary of peace",
	"path of tranquility, healing hands, emissary of peace",
	"path of tranquility, healing hands, emissary of peace",
	"path of tranquility, healing hands, emissary of peace",
	"path of tranquility, healing hands, emissary of peace",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war, anger of a gentle soul",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war, anger of a gentle soul",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war, anger of a gentle soul",
	"path of tranquility, healing hands, emissary of peace, douse the flames of war, anger of a gentle soul"
};
static std::string* monkSubclassAbilities[] = {
	drunkenMasterMonkAbilities, fourElementsMonkAbilities, kensaiMonkAbilities, longDeathMonkAbilities, openHandMonkAbilities, shadowMonkAbilities, sunSoulMonkAbilities, tranquilityMonkAbilities
};
/*
static std::string monkSubclass[] = { "drunken master", "four elements", "kensai", "long death", "open hand", "shadow", "sun soul", "tranquility" };
*/
#pragma endregion monk abilities
#pragma region
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
static std::string avatarMysticAbilities[] = {
	"bonus avatar disciplines, proficiency with medium armor/shields",
	"bonus avatar disciplines, proficiency with medium armor/shields",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing",	
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing, avatar of speed",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing, avatar of speed",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing, avatar of speed",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing, avatar of speed",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing, avatar of speed",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing, avatar of speed",
	"bonus avatar disciplines, proficiency with medium armor/shields, avatar of battle, avatar of healing, avatar of speed"
};
	//avatar mystics gain bonus armor proficiencies that need to be accounted for later
static std::string awakenedMysticAbilities[] = {
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge, spectral form",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge, spectral form",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge, spectral form",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge, spectral form",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge, spectral form",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge, spectral form",
	"bonus awakened disciplines, proficiency with (2 of) animal handling/deception/insight/intimidation/investigation/perception/persuasion, psionic investigation, psionic surge, spectral form"
};
	//awakened mystics gain bonus skill proficiencies that need to be accounted for later
static std::string immortalMysticAbilities[] = {
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total)",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total)",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health, immortal will",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health, immortal will",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health, immortal will",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health, immortal will",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health, immortal will",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health, immortal will",
	"bonus immortal disciplines, immortal durability (bonus hp already included in hp total), psionic resilience, surge of health, immortal will"
};
	//immortal mystics gain bonus hit points, which are already taken into account 
static std::string nomadMysticAbilities[] = {
	"bonus nomad disciplines, breadth of knowledge",
	"bonus nomad disciplines, breadth of knowledge",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation, effortless journey",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation, effortless journey",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation, effortless journey",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation, effortless journey",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation, effortless journey",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation, effortless journey",
	"bonus nomad disciplines, breadth of knowledge, memory of one thousand steps, superior teleportation, effortless journey"
};
static std::string soulKnifeMysticAbilities[] = {
	"proficiency in medium armor/martial weapons, soul knife",
	"proficiency in medium armor/martial weapons, soul knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife, phantom knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife, phantom knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife, phantom knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife, phantom knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife, phantom knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife, phantom knife",
	"proficiency in medium armor/martial weapons, soul knife, hone the blade, consumptive knife, phantom knife"
};
	//soul knife gains bonus weapon & armor proficiencies that need to be accounted for later
static std::string wuJenMysticAbilities[] = {
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler, elemental mastery",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler, elemental mastery",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler, elemental mastery",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler, elemental mastery",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler, elemental mastery",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler, elemental mastery",
	"bonus wu jen disciplines, proficiency with (2 of) animal handling/arcana/history/insight/medicine/nature/perception/religion/survival, elemental attunement, arcane dabbler, elemental mastery"
};
	//wu jen gains bonus proficiencies that need to be accounted for later
	//wu jen also gains some spells which need to be accounted for later
static std::string* mysticSubclassAbilities[] = {
	avatarMysticAbilities, awakenedMysticAbilities, immortalMysticAbilities, nomadMysticAbilities, soulKnifeMysticAbilities, wuJenMysticAbilities
};
	//most mystic subclasses gain bonus psionic disciplines that need to be accounted for later
#pragma endregion mystic abilities
#pragma region
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
static std::string ancientsPaladinAbilities[] = {
	"",
	"",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding, undying sentinel",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding, undying sentinel",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding, undying sentinel",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding, undying sentinel",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding, undying sentinel",
	"oath spells, channel divinity: nature's wrath, channel divinity: turn the faithless, aura of warding, undying sentinel, elder champion"
};
static std::string conquestPaladinAbilities[] = {
	"",
	"",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest, scornful rebuke",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest, scornful rebuke",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest, scornful rebuke",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest, scornful rebuke",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest, scornful rebuke",
	"oath spells, channel divinity: conquering presence, channel divinity: guided strike, aura of conquest, scornful rebuke, invincible conquerer"
};
static std::string crownPaladinAbilities[] = {
	"",
	"",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance, unyielding spirit",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance, unyielding spirit",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance, unyielding spirit",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance, unyielding spirit",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance, unyielding spirit",
	"oath spells, channel divinity: champion challenge, channel divinity: turn the tide, divine allegiance, unyielding spirit, exalted champion"
};
static std::string devotionPaladinAbilities[] = {
	"",
	"",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion, purity of spirit",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion, purity of spirit",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion, purity of spirit",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion, purity of spirit",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion, purity of spirit",
	"oath spells, channel divinity: sacred weapon, channel divinity: turn the unholy, aura of devotion, purity of spirit, holy nimbus"
};
static std::string redemptionPaladinAbilities[] = {
	"",
	"",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian, protective spirit",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian, protective spirit",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian, protective spirit",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian, protective spirit",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian, protective spirit",
	"oath spells, channel divinity: emissary of peace, channel divinity: rebuke the violent, aura of the guardian, protective spirit, emissary of redemption"
};
static std::string treacheryPaladinAbilities[] = {
	"",
	"",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery, blackguard's escape",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery, blackguard's escape",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery, blackguard's escape",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery, blackguard's escape",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery, blackguard's escape",
	"oath spells, channel divinity: conjure duplicate, channel divinity: poison strike, aura of treachery, blackguard's escape, icon of deceit"
};
static std::string vengeancePaladinAbilities[] = {
	"",
	"",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger, soul of vengeance",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger, soul of vengeance",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger, soul of vengeance",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger, soul of vengeance",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger, soul of vengeance",
	"oath spells, channel divinity: abjure enemy, channel divinity: vow of enmity, relentless avenger, soul of vengeance, avenging angel"
};
static std::string oathbreakerPaladinAbilities[] = {
	"",
	"",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate, supernatural resistance",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate, supernatural resistance",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate, supernatural resistance",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate, supernatural resistance",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate, supernatural resistance",
	"oathbreaker spells, channel divinity: control undead, channel divinity: dreadful aspect, aura of hate, supernatural resistance, dread lord"
};
static std::string* paladinSubclassAbilities[] = {
	ancientsPaladinAbilities, conquestPaladinAbilities, crownPaladinAbilities, devotionPaladinAbilities, redemptionPaladinAbilities, treacheryPaladinAbilities, vengeancePaladinAbilities, oathbreakerPaladinAbilities
};
	//most paladin subclasses gain bonus spells which need to be accounted for later
#pragma endregion paladin abilities
#pragma region
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
static std::string beastMasterRangerAbilities[] = {
	"",
	"",
	"ranger's companion",
	"ranger's companion",
	"ranger's companion",
	"ranger's companion",
	"ranger's companion, exceptional training",
	"ranger's companion, exceptional training",
	"ranger's companion, exceptional training",
	"ranger's companion, exceptional training",
	"ranger's companion, exceptional training, bestial fury",
	"ranger's companion, exceptional training, bestial fury",
	"ranger's companion, exceptional training, bestial fury",
	"ranger's companion, exceptional training, bestial fury",
	"ranger's companion, exceptional training, bestial fury, share spells",
	"ranger's companion, exceptional training, bestial fury, share spells",
	"ranger's companion, exceptional training, bestial fury, share spells",
	"ranger's companion, exceptional training, bestial fury, share spells",
	"ranger's companion, exceptional training, bestial fury, share spells",
	"ranger's companion, exceptional training, bestial fury, share spells"
};
	//beast master gains an animal companion which will have to be accounted for later
static std::string gloomStalkerRangerAbilities[] = {
	"",
	"",
	"gloom stalker magic, dread ambusher, umbral sight",
	"gloom stalker magic, dread ambusher, umbral sight",
	"gloom stalker magic, dread ambusher, umbral sight",
	"gloom stalker magic, dread ambusher, umbral sight",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry, shadowy dodge",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry, shadowy dodge",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry, shadowy dodge",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry, shadowy dodge",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry, shadowy dodge",
	"gloom stalker magic, dread ambusher, umbral sight, iron mind, stalker's flurry, shadowy dodge"
};
	//gloom stalkers gain bonus magic that needs to be accounted for later
static std::string horizonWalkerRangerAbilities[] = {
	"",
	"",
	"horizon walker magic, detect portal, planar warrior",
	"horizon walker magic, detect portal, planar warrior",
	"horizon walker magic, detect portal, planar warrior",
	"horizon walker magic, detect portal, planar warrior",
	"horizon walker magic, detect portal, planar warrior, ethereal step",
	"horizon walker magic, detect portal, planar warrior, ethereal step",
	"horizon walker magic, detect portal, planar warrior, ethereal step",
	"horizon walker magic, detect portal, planar warrior, ethereal step",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike, spectral defense",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike, spectral defense",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike, spectral defense",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike, spectral defense",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike, spectral defense",
	"horizon walker magic, detect portal, planar warrior, ethereal step, distant strike, spectral defense"
};
	//horizon walkers gain bonus magic that needs to be accounted for later
static std::string hunterRangerAbilities[] = {
	"",
	"",
	"hunter's prey",
	"hunter's prey",
	"hunter's prey",
	"hunter's prey",
	"hunter's prey, defensive tactics",
	"hunter's prey, defensive tactics",
	"hunter's prey, defensive tactics",
	"hunter's prey, defensive tactics",
	"hunter's prey, defensive tactics, multiattack",
	"hunter's prey, defensive tactics, multiattack",
	"hunter's prey, defensive tactics, multiattack",
	"hunter's prey, defensive tactics, multiattack",
	"hunter's prey, defensive tactics, multiattack, superior hunter's defense",
	"hunter's prey, defensive tactics, multiattack, superior hunter's defense",
	"hunter's prey, defensive tactics, multiattack, superior hunter's defense",
	"hunter's prey, defensive tactics, multiattack, superior hunter's defense",
	"hunter's prey, defensive tactics, multiattack, superior hunter's defense",
	"hunter's prey, defensive tactics, multiattack, superior hunter's defense"
};
	//hunter has a bunch of unique features that need to be accounted for later
static std::string monsterSlayerRangerAbilities[] = {
	"",
	"",
	"monster slayer magic, hunter's sense, slayer's prey",
	"monster slayer magic, hunter's sense, slayer's prey",
	"monster slayer magic, hunter's sense, slayer's prey",
	"monster slayer magic, hunter's sense, slayer's prey",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis, slayer's counter",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis, slayer's counter",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis, slayer's counter",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis, slayer's counter",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis, slayer's counter",
	"monster slayer magic, hunter's sense, slayer's prey, supernatural defense, magic-user's nemesis, slayer's counter"
};
	//monster slayer gains bonus magic that needs to be accounted for later
static std::string primevalGuardianRangerAbilities[] = {
	"",
	"",
	"guardian magic, guardian soul, piercing thorns",
	"guardian magic, guardian soul, piercing thorns",
	"guardian magic, guardian soul, piercing thorns",
	"guardian magic, guardian soul, piercing thorns",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense, guardian aura",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense, guardian aura",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense, guardian aura",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense, guardian aura",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense, guardian aura",
	"guardian magic, guardian soul, piercing thorns, ancient fortitude, rooted defense, guardian aura"
};
static std::string* rangerSubclassAbilities[] = {
	beastMasterRangerAbilities, gloomStalkerRangerAbilities, horizonWalkerRangerAbilities, hunterRangerAbilities, monsterSlayerRangerAbilities, primevalGuardianRangerAbilities
};
#pragma endregion ranger abilities
#pragma region
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
static std::string arcaneTrickerRogueAbilities[] = {
	"",
	"",
	"spellcasting, mage hand legerdemain",
	"spellcasting, mage hand legerdemain",
	"spellcasting, mage hand legerdemain",
	"spellcasting, mage hand legerdemain",
	"spellcasting, mage hand legerdemain",
	"spellcasting, mage hand legerdemain",
	"spellcasting, mage hand legerdemain, magical ambush",
	"spellcasting, mage hand legerdemain, magical ambush",
	"spellcasting, mage hand legerdemain, magical ambush",
	"spellcasting, mage hand legerdemain, magical ambush",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster, spell thief",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster, spell thief",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster, spell thief",
	"spellcasting, mage hand legerdemain, magical ambush, versatile trickster, spell thief"
};
	//arcane trickster gains spellcasting, which needs to be accounted for later
static std::string assassinRogueAbilities[] = {
	"",
	"",
	"proficiency with disguise kit/poisoner's kit, assassinate",
	"proficiency with disguise kit/poisoner's kit, assassinate",
	"proficiency with disguise kit/poisoner's kit, assassinate",
	"proficiency with disguise kit/poisoner's kit, assassinate",
	"proficiency with disguise kit/poisoner's kit, assassinate",
	"proficiency with disguise kit/poisoner's kit, assassinate",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter, death strike",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter, death strike",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter, death strike",
	"proficiency with disguise kit/poisoner's kit, assassinate, infiltration expertise, imposter, death strike"
};
	//assassin gains bonus proficiencies that need to be accounted for later
static std::string inquisitiveRogueAbilities[] = {
	"",
	"",
	"ear for deceit, eye for detail, insightful fighting",
	"ear for deceit, eye for detail, insightful fighting",
	"ear for deceit, eye for detail, insightful fighting",
	"ear for deceit, eye for detail, insightful fighting",
	"ear for deceit, eye for detail, insightful fighting",
	"ear for deceit, eye for detail, insightful fighting",
	"ear for deceit, eye for detail, insightful fighting, steady eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye, eye for weakness",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye, eye for weakness",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye, eye for weakness",
	"ear for deceit, eye for detail, insightful fighting, steady eye, unerring eye, eye for weakness"
};
static std::string mastermindRogueAbilities[] = {
	"",
	"",
	"master of intrigue, master of tactics",
	"master of intrigue, master of tactics",
	"master of intrigue, master of tactics",
	"master of intrigue, master of tactics",
	"master of intrigue, master of tactics",
	"master of intrigue, master of tactics",
	"master of intrigue, master of tactics, insightful manipulator",
	"master of intrigue, master of tactics, insightful manipulator",
	"master of intrigue, master of tactics, insightful manipulator",
	"master of intrigue, master of tactics, insightful manipulator",
	"master of intrigue, master of tactics, insightful manipulator, misdirection",
	"master of intrigue, master of tactics, insightful manipulator, misdirection",
	"master of intrigue, master of tactics, insightful manipulator, misdirection",
	"master of intrigue, master of tactics, insightful manipulator, misdirection",
	"master of intrigue, master of tactics, insightful manipulator, misdirection, soul of deceit",
	"master of intrigue, master of tactics, insightful manipulator, misdirection, soul of deceit",
	"master of intrigue, master of tactics, insightful manipulator, misdirection, soul of deceit",
	"master of intrigue, master of tactics, insightful manipulator, misdirection, soul of deceit"
};
static std::string scoutRogueAbilities[] = {
	"",
	"",
	"skirmisher, proficiency+expertise in nature/surivival",
	"skirmisher, proficiency+expertise in nature/surivival",
	"skirmisher, proficiency+expertise in nature/surivival",
	"skirmisher, proficiency+expertise in nature/surivival",
	"skirmisher, proficiency+expertise in nature/surivival",
	"skirmisher, proficiency+expertise in nature/surivival",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master, sudden strike",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master, sudden strike",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master, sudden strike",
	"skirmisher, proficiency+expertise in nature/surivival, superior mobility, ambush master, sudden strike"
};
	//scouts gain bonus proficiencies that need to be accounted for later
	//scouts get bonus speed that needs to be accounted for later
static std::string swashbucklerRogueAbilities[] = {
	"",
	"",
	"fancy footwork, rakish audacity",
	"fancy footwork, rakish audacity",
	"fancy footwork, rakish audacity",
	"fancy footwork, rakish audacity",
	"fancy footwork, rakish audacity",
	"fancy footwork, rakish audacity",
	"fancy footwork, rakish audacity, panache",
	"fancy footwork, rakish audacity, panache",
	"fancy footwork, rakish audacity, panache",
	"fancy footwork, rakish audacity, panache",
	"fancy footwork, rakish audacity, panache, elegant maneuver",
	"fancy footwork, rakish audacity, panache, elegant maneuver",
	"fancy footwork, rakish audacity, panache, elegant maneuver",
	"fancy footwork, rakish audacity, panache, elegant maneuver",
	"fancy footwork, rakish audacity, panache, elegant maneuver, master duelist",
	"fancy footwork, rakish audacity, panache, elegant maneuver, master duelist",
	"fancy footwork, rakish audacity, panache, elegant maneuver, master duelist",
	"fancy footwork, rakish audacity, panache, elegant maneuver, master duelist"
};
static std::string thiefRogueAbilities[] = {
	"",
	"",
	"fast hands, second story work",
	"fast hands, second story work",
	"fast hands, second story work",
	"fast hands, second story work",
	"fast hands, second story work",
	"fast hands, second story work",
	"fast hands, second story work, supreme sneak",
	"fast hands, second story work, supreme sneak",
	"fast hands, second story work, supreme sneak",
	"fast hands, second story work, supreme sneak",
	"fast hands, second story work, supreme sneak, use magic device",
	"fast hands, second story work, supreme sneak, use magic device",
	"fast hands, second story work, supreme sneak, use magic device",
	"fast hands, second story work, supreme sneak, use magic device",
	"fast hands, second story work, supreme sneak, use magic device, thief's reflexes",
	"fast hands, second story work, supreme sneak, use magic device, thief's reflexes",
	"fast hands, second story work, supreme sneak, use magic device, thief's reflexes",
	"fast hands, second story work, supreme sneak, use magic device, thief's reflexes"
};
static std::string* rogueSubclassAbilities[] = {
	arcaneTrickerRogueAbilities, assassinRogueAbilities, inquisitiveRogueAbilities, mastermindRogueAbilities, scoutRogueAbilities, swashbucklerRogueAbilities, thiefRogueAbilities
};
#pragma endregion rogue abilities
#pragma region 
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
static std::string divineSoulSorcererAbilities[] = {
	"divine magic, favored by the gods",
	"divine magic, favored by the gods",
	"divine magic, favored by the gods",
	"divine magic, favored by the gods",
	"divine magic, favored by the gods",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing",
	"divine magic, favored by the gods, empowered healing, otherworldly wings",
	"divine magic, favored by the gods, empowered healing, otherworldly wings",
	"divine magic, favored by the gods, empowered healing, otherworldly wings",
	"divine magic, favored by the gods, empowered healing, otherworldly wings",
	"divine magic, favored by the gods, empowered healing, otherworldly wings, unearthly recovery",
	"divine magic, favored by the gods, empowered healing, otherworldly wings, unearthly recovery",
	"divine magic, favored by the gods, empowered healing, otherworldly wings, unearthly recovery"
};
	//divine souls gain access to cleric spells which is like a whole big thing that will need to be accounted for later
	//divine souls also gain a specific spell based on their alignment that needs to be accounted for later
static std::string draconicSorcererAbilities[] = {
	"dragon ancestor, unarmored AC equals 13 + Dex mod",
	"dragon ancestor, unarmored AC equals 13 + Dex mod",
	"dragon ancestor, unarmored AC equals 13 + Dex mod",
	"dragon ancestor, unarmored AC equals 13 + Dex mod",
	"dragon ancestor, unarmored AC equals 13 + Dex mod",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity, dragon wings",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity, dragon wings",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity, dragon wings",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity, dragon wings",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity, dragon wings, draconic presence",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity, dragon wings, draconic presence",
	"dragon ancestor, unarmored AC equals 13 + Dex mod, elemental affinity, dragon wings, draconic presence"
};
	//draconic sorcerers gain abilities based on their dragon color, which will need to be accounted for later
	//draconic sorcerers gain bonus hp which has already been taken into account
static std::string phoenixSorcererAbilities[] = {
	"ignite, mantle of flames",
	"ignite, mantle of flames",
	"ignite, mantle of flames",
	"ignite, mantle of flames",
	"ignite, mantle of flames",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark",
	"ignite, mantle of flames, phoenix spark, nourishing fire",
	"ignite, mantle of flames, phoenix spark, nourishing fire",
	"ignite, mantle of flames, phoenix spark, nourishing fire",
	"ignite, mantle of flames, phoenix spark, nourishing fire",
	"ignite, mantle of flames, phoenix spark, nourishing fire, form of the phoenix",
	"ignite, mantle of flames, phoenix spark, nourishing fire, form of the phoenix",
	"ignite, mantle of flames, phoenix spark, nourishing fire, form of the phoenix"
};
static std::string seaSorcererAbilities[] = {
	"soul of the sea, curse of the sea",
	"soul of the sea, curse of the sea",
	"soul of the sea, curse of the sea",
	"soul of the sea, curse of the sea",
	"soul of the sea, curse of the sea",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense",
	"soul of the sea, curse of the sea, watery defense, shifting form",
	"soul of the sea, curse of the sea, watery defense, shifting form",
	"soul of the sea, curse of the sea, watery defense, shifting form",
	"soul of the sea, curse of the sea, watery defense, shifting form",
	"soul of the sea, curse of the sea, watery defense, shifting form, water soul",
	"soul of the sea, curse of the sea, watery defense, shifting form, water soul",
	"soul of the sea, curse of the sea, watery defense, shifting form, water soul"
};
static std::string shadowSorcererAbilities[] = {
	"eyes of the dark, strength of the grave",
	"eyes of the dark, strength of the grave",
	"eyes of the dark, strength of the grave",
	"eyes of the dark, strength of the grave",
	"eyes of the dark, strength of the grave",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen",
	"eyes of the dark, strength of the grave, hound of ill omen, shadow walk",
	"eyes of the dark, strength of the grave, hound of ill omen, shadow walk",
	"eyes of the dark, strength of the grave, hound of ill omen, shadow walk",
	"eyes of the dark, strength of the grave, hound of ill omen, shadow walk",
	"eyes of the dark, strength of the grave, hound of ill omen, shadow walk, umbral form",
	"eyes of the dark, strength of the grave, hound of ill omen, shadow walk, umbral form",
	"eyes of the dark, strength of the grave, hound of ill omen, shadow walk, umbral form"
};
static std::string stoneSorcererAbilities[] = {
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp)",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp)",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp)",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp)",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp)",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis, stone's edge",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis, stone's edge",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis, stone's edge",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis, stone's edge",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis, stone's edge, earth master's aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis, stone's edge, earth master's aegis",
	"proficiency with shields/simple weapons/martial weapons, metal magic, stone's durability (hp boost already counted into hp), stone aegis, stone's edge, earth master's aegis"
};
	//stone sorcerers gain bonus armor and weapon proficiencies that need to be accounted for later
static std::string stormSorcererAbilities[] = {
	"wind speaker, tempestuous magic",
	"wind speaker, tempestuous magic",
	"wind speaker, tempestuous magic",
	"wind speaker, tempestuous magic",
	"wind speaker, tempestuous magic",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide",
	"wind speaker, tempestuous magic, heart of the storm, storm guide, storm's fury",
	"wind speaker, tempestuous magic, heart of the storm, storm guide, storm's fury",
	"wind speaker, tempestuous magic, heart of the storm, storm guide, storm's fury",
	"wind speaker, tempestuous magic, heart of the storm, storm guide, storm's fury",
	"wind speaker, tempestuous magic, heart of the storm, storm guide, storm's fury, wind soul",
	"wind speaker, tempestuous magic, heart of the storm, storm guide, storm's fury, wind soul",
	"wind speaker, tempestuous magic, heart of the storm, storm guide, storm's fury, wind soul"
};
static std::string wildSorcererAbilities[] = {
	"wild magic surge, tides of chaos",
	"wild magic surge, tides of chaos",
	"wild magic surge, tides of chaos",
	"wild magic surge, tides of chaos",
	"wild magic surge, tides of chaos",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck",
	"wild magic surge, tides of chaos, bend luck, controlled chaos",
	"wild magic surge, tides of chaos, bend luck, controlled chaos",
	"wild magic surge, tides of chaos, bend luck, controlled chaos",
	"wild magic surge, tides of chaos, bend luck, controlled chaos",
	"wild magic surge, tides of chaos, bend luck, controlled chaos, spell bombardment",
	"wild magic surge, tides of chaos, bend luck, controlled chaos, spell bombardment",
	"wild magic surge, tides of chaos, bend luck, controlled chaos, spell bombardment"
};
static std::string* sorcererSubclassAbilities[] = {
	divineSoulSorcererAbilities, draconicSorcererAbilities, phoenixSorcererAbilities, seaSorcererAbilities, shadowSorcererAbilities, stoneSorcererAbilities, stormSorcererAbilities, wildSorcererAbilities
};
#pragma endregion sorcerer abilities
#pragma region
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
static std::string archfeyWarlockAbilities[] = {
	"expanded spell list, fey presence",
	"expanded spell list, fey presence",
	"expanded spell list, fey presence",
	"expanded spell list, fey presence",
	"expanded spell list, fey presence",
	"expanded spell list, fey presence, misty escape",
	"expanded spell list, fey presence, misty escape",
	"expanded spell list, fey presence, misty escape",
	"expanded spell list, fey presence, misty escape",
	"expanded spell list, fey presence, misty escape, beguiling defenses",
	"expanded spell list, fey presence, misty escape, beguiling defenses",
	"expanded spell list, fey presence, misty escape, beguiling defenses",
	"expanded spell list, fey presence, misty escape, beguiling defenses",
	"expanded spell list, fey presence, misty escape, beguiling defenses, dark delerium",
	"expanded spell list, fey presence, misty escape, beguiling defenses, dark delerium",
	"expanded spell list, fey presence, misty escape, beguiling defenses, dark delerium",
	"expanded spell list, fey presence, misty escape, beguiling defenses, dark delerium",
	"expanded spell list, fey presence, misty escape, beguiling defenses, dark delerium",
	"expanded spell list, fey presence, misty escape, beguiling defenses, dark delerium",
	"expanded spell list, fey presence, misty escape, beguiling defenses, dark delerium"
};
static std::string celestialWarlockAbilities[] =	{
	"expanded spell list, gain light/sacred flame cantrips, healing light",
	"expanded spell list, gain light/sacred flame cantrips, healing light",
	"expanded spell list, gain light/sacred flame cantrips, healing light",
	"expanded spell list, gain light/sacred flame cantrips, healing light",
	"expanded spell list, gain light/sacred flame cantrips, healing light",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience, searing vengeance",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience, searing vengeance",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience, searing vengeance",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience, searing vengeance",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience, searing vengeance",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience, searing vengeance",
	"expanded spell list, gain light/sacred flame cantrips, healing light, radiant soul, celestial resilience, searing vengeance"
};
	//celestial warlocks gain bonus cantrips that will need to be accounted for later
static std::string fiendWarlockAbilities[] = {
	"expanded spell list, dark one's blessing",
	"expanded spell list, dark one's blessing",
	"expanded spell list, dark one's blessing",
	"expanded spell list, dark one's blessing",
	"expanded spell list, dark one's blessing",
	"expanded spell list, dark one's blessing, dark one's own luck",
	"expanded spell list, dark one's blessing, dark one's own luck",
	"expanded spell list, dark one's blessing, dark one's own luck",
	"expanded spell list, dark one's blessing, dark one's own luck",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience, hurl through hell",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience, hurl through hell",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience, hurl through hell",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience, hurl through hell",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience, hurl through hell",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience, hurl through hell",
	"expanded spell list, dark one's blessing, dark one's own luck, fiendish resilience, hurl through hell"
};
static std::string ghostInTheMachineWarlockAbilities[] = {
	"expanded spell list, proficiency with hacking tools, information surge",
	"expanded spell list, proficiency with hacking tools, information surge",
	"expanded spell list, proficiency with hacking tools, information surge",
	"expanded spell list, proficiency with hacking tools, information surge",
	"expanded spell list, proficiency with hacking tools, information surge",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption, technovirus",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption, technovirus",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption, technovirus",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption, technovirus",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption, technovirus",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption, technovirus",
	"expanded spell list, proficiency with hacking tools, information surge, wire walk, personal encryption, technovirus"
};
	//ghost in the machine warlocks gain bonus proficiencies that will need to be accounted for later
static std::string greatOldOneWarlockAbilities[] = {
	"expanded spell list, awakened mind",
	"expanded spell list, awakened mind",
	"expanded spell list, awakened mind",
	"expanded spell list, awakened mind",
	"expanded spell list, awakened mind",
	"expanded spell list, awakened mind, entropic ward",
	"expanded spell list, awakened mind, entropic ward",
	"expanded spell list, awakened mind, entropic ward",
	"expanded spell list, awakened mind, entropic ward",
	"expanded spell list, awakened mind, entropic ward, thought shield",
	"expanded spell list, awakened mind, entropic ward, thought shield",
	"expanded spell list, awakened mind, entropic ward, thought shield",
	"expanded spell list, awakened mind, entropic ward, thought shield",
	"expanded spell list, awakened mind, entropic ward, thought shield, create thrall",
	"expanded spell list, awakened mind, entropic ward, thought shield, create thrall",
	"expanded spell list, awakened mind, entropic ward, thought shield, create thrall",
	"expanded spell list, awakened mind, entropic ward, thought shield, create thrall",
	"expanded spell list, awakened mind, entropic ward, thought shield, create thrall",
	"expanded spell list, awakened mind, entropic ward, thought shield, create thrall",
	"expanded spell list, awakened mind, entropic ward, thought shield, create thrall"
};
static std::string hexbladeWarlockAbilities[] = {
	"expanded spell list, hexblade's curse, hex warrior",
	"expanded spell list, hexblade's curse, hex warrior",
	"expanded spell list, hexblade's curse, hex warrior",
	"expanded spell list, hexblade's curse, hex warrior",
	"expanded spell list, hexblade's curse, hex warrior",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes, master of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes, master of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes, master of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes, master of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes, master of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes, master of hexes",
	"expanded spell list, hexblade's curse, hex warrior, accursed specter, armor of hexes, master of hexes"
};
	//hexblade gains bonus proficiencies that will need to be accounted for later (in the hex warrior ability)
static std::string ravenQueenWarlockAbilities[] = {
	"expanded spell list, sentinel raven",
	"expanded spell list, sentinel raven",
	"expanded spell list, sentinel raven",
	"expanded spell list, sentinel raven",
	"expanded spell list, sentinel raven",
	"expanded spell list, sentinel raven, soul of the raven",
	"expanded spell list, sentinel raven, soul of the raven",
	"expanded spell list, sentinel raven, soul of the raven",
	"expanded spell list, sentinel raven, soul of the raven",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield, queen's right hand",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield, queen's right hand",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield, queen's right hand",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield, queen's right hand",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield, queen's right hand",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield, queen's right hand",
	"expanded spell list, sentinel raven, soul of the raven, raven's shield, queen's right hand"
};
static std::string seekerWarlockAbilities[] = {
	"expanded spell list, shielding aurora",
	"expanded spell list, shielding aurora",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer, astral sequestration",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer, astral sequestration",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer, astral sequestration",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer, astral sequestration",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer, astral sequestration",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer, astral sequestration",
	"expanded spell list, shielding aurora, pact boon option: pact of the star chain, astral refuge, far wanderer, astral sequestration"
};
static std::string undyingWarlockAbilities[] = {
	"expanded spell list, among the dead",
	"expanded spell list, among the dead",
	"expanded spell list, among the dead",
	"expanded spell list, among the dead",
	"expanded spell list, among the dead",
	"expanded spell list, among the dead, defy death",
	"expanded spell list, among the dead, defy death",
	"expanded spell list, among the dead, defy death",
	"expanded spell list, among the dead, defy death",
	"expanded spell list, among the dead, defy death, undying nature",
	"expanded spell list, among the dead, defy death, undying nature",
	"expanded spell list, among the dead, defy death, undying nature",
	"expanded spell list, among the dead, defy death, undying nature",
	"expanded spell list, among the dead, defy death, undying nature, indestructible life",
	"expanded spell list, among the dead, defy death, undying nature, indestructible life",
	"expanded spell list, among the dead, defy death, undying nature, indestructible life",
	"expanded spell list, among the dead, defy death, undying nature, indestructible life",
	"expanded spell list, among the dead, defy death, undying nature, indestructible life",
	"expanded spell list, among the dead, defy death, undying nature, indestructible life",
	"expanded spell list, among the dead, defy death, undying nature, indestructible life"
};
static std::string* warlockSubclassAbilities[] = {
	archfeyWarlockAbilities, celestialWarlockAbilities, fiendWarlockAbilities, ghostInTheMachineWarlockAbilities, greatOldOneWarlockAbilities, hexbladeWarlockAbilities, ravenQueenWarlockAbilities, seekerWarlockAbilities, undyingWarlockAbilities
};
	//most warlock subclasses gain extra spells which will need to be accounted for later
#pragma endregion warlock abilities
#pragma region
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
static std::string artificerWizardAbilities[] = {
	"",
	"infuse potions, infuse scrolls",
	"infuse potions, infuse scrolls",
	"infuse potions, infuse scrolls",
	"infuse potions, infuse scrolls",
	"infuse potions, infuse scrolls, infuse weapons and armor",
	"infuse potions, infuse scrolls, infuse weapons and armor",
	"infuse potions, infuse scrolls, infuse weapons and armor",
	"infuse potions, infuse scrolls, infuse weapons and armor",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer, master artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer, master artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer, master artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer, master artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer, master artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer, master artificer",
	"infuse potions, infuse scrolls, infuse weapons and armor, superior artificer, master artificer"
};
static std::string bladesingerWizardAbilities[] = {
	"",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense, song of victory",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense, song of victory",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense, song of victory",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense, song of victory",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense, song of victory",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense, song of victory",
	"proficiency with light armor/one one-handed melee weapon/performance, bladesong, extra attack, song of defense, song of victory"
};
	//bladesinger gains bonus proficiencies that will need to be accounted for later
static std::string loreMasterWizardAbilities[] = {
	"",
	"lore master, spell secrets",
	"lore master, spell secrets",
	"lore master, spell secrets",
	"lore master, spell secrets",
	"lore master, spell secrets, alchemical casting",
	"lore master, spell secrets, alchemical casting",
	"lore master, spell secrets, alchemical casting",
	"lore master, spell secrets, alchemical casting",
	"lore master, spell secrets, alchemical casting, prodigious memory",
	"lore master, spell secrets, alchemical casting, prodigious memory",
	"lore master, spell secrets, alchemical casting, prodigious memory",
	"lore master, spell secrets, alchemical casting, prodigious memory",
	"lore master, spell secrets, alchemical casting, prodigious memory, master of magic",
	"lore master, spell secrets, alchemical casting, prodigious memory, master of magic",
	"lore master, spell secrets, alchemical casting, prodigious memory, master of magic",
	"lore master, spell secrets, alchemical casting, prodigious memory, master of magic",
	"lore master, spell secrets, alchemical casting, prodigious memory, master of magic",
	"lore master, spell secrets, alchemical casting, prodigious memory, master of magic",
	"lore master, spell secrets, alchemical casting, prodigious memory, master of magic"
};
	//lore masters gain expertise (from lore master ability), which will need to be accounted for later
static std::string abjurationWizardAbilities[] = {
	"",
	"abjuration savant, arcane ward",
	"abjuration savant, arcane ward",
	"abjuration savant, arcane ward",
	"abjuration savant, arcane ward",
	"abjuration savant, arcane ward, projected ward",
	"abjuration savant, arcane ward, projected ward",
	"abjuration savant, arcane ward, projected ward",
	"abjuration savant, arcane ward, projected ward",
	"abjuration savant, arcane ward, projected ward, improved abjuration",
	"abjuration savant, arcane ward, projected ward, improved abjuration",
	"abjuration savant, arcane ward, projected ward, improved abjuration",
	"abjuration savant, arcane ward, projected ward, improved abjuration",
	"abjuration savant, arcane ward, projected ward, improved abjuration, spell resistance",
	"abjuration savant, arcane ward, projected ward, improved abjuration, spell resistance",
	"abjuration savant, arcane ward, projected ward, improved abjuration, spell resistance",
	"abjuration savant, arcane ward, projected ward, improved abjuration, spell resistance",
	"abjuration savant, arcane ward, projected ward, improved abjuration, spell resistance",
	"abjuration savant, arcane ward, projected ward, improved abjuration, spell resistance",
	"abjuration savant, arcane ward, projected ward, improved abjuration, spell resistance"
};
static std::string conjurationWizardAbilities[] = {
	"",
	"conjuration savant, minor conjuration",
	"conjuration savant, minor conjuration",
	"conjuration savant, minor conjuration",
	"conjuration savant, minor conjuration",
	"conjuration savant, minor conjuration, benign transposition",
	"conjuration savant, minor conjuration, benign transposition",
	"conjuration savant, minor conjuration, benign transposition",
	"conjuration savant, minor conjuration, benign transposition",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration, durable summons",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration, durable summons",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration, durable summons",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration, durable summons",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration, durable summons",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration, durable summons",
	"conjuration savant, minor conjuration, benign transposition, focused conjuration, durable summons"
};
static std::string divinationWizardAbilities[] = {
	"",
	"divination savant, portent",
	"divination savant, portent",
	"divination savant, portent",
	"divination savant, portent",
	"divination savant, portent, expert divination",
	"divination savant, portent, expert divination",
	"divination savant, portent, expert divination",
	"divination savant, portent, expert divination",
	"divination savant, portent, expert divination, the third eye",
	"divination savant, portent, expert divination, the third eye",
	"divination savant, portent, expert divination, the third eye",
	"divination savant, portent, expert divination, the third eye",
	"divination savant, portent, expert divination, the third eye, greater portent",
	"divination savant, portent, expert divination, the third eye, greater portent",
	"divination savant, portent, expert divination, the third eye, greater portent",
	"divination savant, portent, expert divination, the third eye, greater portent",
	"divination savant, portent, expert divination, the third eye, greater portent",
	"divination savant, portent, expert divination, the third eye, greater portent",
	"divination savant, portent, expert divination, the third eye, greater portent"
};
static std::string enchantmentWizardAbilities[] = {
	"",
	"enchantment savant, hypnotic gaze",
	"enchantment savant, hypnotic gaze",
	"enchantment savant, hypnotic gaze",
	"enchantment savant, hypnotic gaze",
	"enchantment savant, hypnotic gaze, instinctive charm",
	"enchantment savant, hypnotic gaze, instinctive charm",
	"enchantment savant, hypnotic gaze, instinctive charm",
	"enchantment savant, hypnotic gaze, instinctive charm",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment, alter memories",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment, alter memories",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment, alter memories",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment, alter memories",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment, alter memories",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment, alter memories",
	"enchantment savant, hypnotic gaze, instinctive charm, split enchantment, alter memories"
};
static std::string evocationWizardAbilities[] = {
	"",
	"evocation savant, sculpt spells",
	"evocation savant, sculpt spells",
	"evocation savant, sculpt spells",
	"evocation savant, sculpt spells",
	"evocation savant, sculpt spells, potent cantrip",
	"evocation savant, sculpt spells, potent cantrip",
	"evocation savant, sculpt spells, potent cantrip",
	"evocation savant, sculpt spells, potent cantrip",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation, overchannel",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation, overchannel",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation, overchannel",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation, overchannel",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation, overchannel",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation, overchannel",
	"evocation savant, sculpt spells, potent cantrip, empowered evocation, overchannel"
};
static std::string illusionWizardAbilities[] = {
	"",
	"illusion savant, improved minor illusion",
	"illusion savant, improved minor illusion",
	"illusion savant, improved minor illusion",
	"illusion savant, improved minor illusion",
	"illusion savant, improved minor illusion, malleable illusions",
	"illusion savant, improved minor illusion, malleable illusions",
	"illusion savant, improved minor illusion, malleable illusions",
	"illusion savant, improved minor illusion, malleable illusions",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self, illusory reality",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self, illusory reality",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self, illusory reality",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self, illusory reality",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self, illusory reality",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self, illusory reality",
	"illusion savant, improved minor illusion, malleable illusions,	illusory self, illusory reality"
};
static std::string inventionWizardAbilities[] = {
	"",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration, controlled chaos",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration, controlled chaos",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration, controlled chaos",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration, controlled chaos",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration, controlled chaos",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration, controlled chaos",
	"proficiency with 2 tools, arcanomechanical armor, reckless casting, alchemical casting, prodigious inspiration, controlled chaos"
};
	//invention wizards gain bonus tools that need to be accounted for later
static std::string necromancyWizardAbilities[] = {
	"",
	"necromancy savant, grim harvest",
	"necromancy savant, grim harvest",
	"necromancy savant, grim harvest",
	"necromancy savant, grim harvest",
	"necromancy savant, grim harvest, undead thralls",
	"necromancy savant, grim harvest, undead thralls",
	"necromancy savant, grim harvest, undead thralls",
	"necromancy savant, grim harvest, undead thralls",
	"necromancy savant, grim harvest, undead thralls, inured to undeath",
	"necromancy savant, grim harvest, undead thralls, inured to undeath",
	"necromancy savant, grim harvest, undead thralls, inured to undeath",
	"necromancy savant, grim harvest, undead thralls, inured to undeath",
	"necromancy savant, grim harvest, undead thralls, inured to undeath, command undead",
	"necromancy savant, grim harvest, undead thralls, inured to undeath, command undead",
	"necromancy savant, grim harvest, undead thralls, inured to undeath, command undead",
	"necromancy savant, grim harvest, undead thralls, inured to undeath, command undead",
	"necromancy savant, grim harvest, undead thralls, inured to undeath, command undead",
	"necromancy savant, grim harvest, undead thralls, inured to undeath, command undead",
	"necromancy savant, grim harvest, undead thralls, inured to undeath, command undead"
};
static std::string transmutationWizardAbilities[] = {
	"",
	"transmutation savant, minor alchemy",
	"transmutation savant, minor alchemy",
	"transmutation savant, minor alchemy",
	"transmutation savant, minor alchemy",
	"transmutation savant, minor alchemy, transmuter's stone",
	"transmutation savant, minor alchemy, transmuter's stone",
	"transmutation savant, minor alchemy, transmuter's stone",
	"transmutation savant, minor alchemy, transmuter's stone",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger, master transmuter",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger, master transmuter",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger, master transmuter",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger, master transmuter",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger, master transmuter",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger, master transmuter",
	"transmutation savant, minor alchemy, transmuter's stone, shapechanger, master transmuter"
};
static std::string technomancyWizardAbilities[] = {
	"",
	"proficiency with sidearms/hacking tools, technological savant",
	"proficiency with sidearms/hacking tools, technological savant",
	"proficiency with sidearms/hacking tools, technological savant",
	"proficiency with sidearms/hacking tools, technological savant",
	"proficiency with sidearms/hacking tools, technological savant, program spell",
	"proficiency with sidearms/hacking tools, technological savant, program spell",
	"proficiency with sidearms/hacking tools, technological savant, program spell",
	"proficiency with sidearms/hacking tools, technological savant, program spell",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting, chained device",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting, chained device",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting, chained device",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting, chained device",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting, chained device",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting, chained device",
	"proficiency with sidearms/hacking tools, technological savant, program spell, online casting, chained device"
};
	//technomancy wizards gain bonus proficiencies that need to be accounted for later
static std::string theurgyWizardAbilities[] = {
	"",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest, arcane high priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest, arcane high priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest, arcane high priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest, arcane high priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest, arcane high priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest, arcane high priest",
	"divine inspiration, arcane initiate, channel arcana, channel arcana: divine arcana, arcane acolyte, arcane priest, arcane high priest"
};
static std::string warMagicWizardAbilities[] = {
	"",
	"arcane deflection, tactical wit",
	"arcane deflection, tactical wit",
	"arcane deflection, tactical wit",
	"arcane deflection, tactical wit",
	"arcane deflection, tactical wit, power surge",
	"arcane deflection, tactical wit, power surge",
	"arcane deflection, tactical wit, power surge",
	"arcane deflection, tactical wit, power surge",
	"arcane deflection, tactical wit, power surge, durable magic",
	"arcane deflection, tactical wit, power surge, durable magic",
	"arcane deflection, tactical wit, power surge, durable magic",
	"arcane deflection, tactical wit, power surge, durable magic",
	"arcane deflection, tactical wit, power surge, durable magic, deflecting shroud",
	"arcane deflection, tactical wit, power surge, durable magic, deflecting shroud",
	"arcane deflection, tactical wit, power surge, durable magic, deflecting shroud",
	"arcane deflection, tactical wit, power surge, durable magic, deflecting shroud",
	"arcane deflection, tactical wit, power surge, durable magic, deflecting shroud",
	"arcane deflection, tactical wit, power surge, durable magic, deflecting shroud",
	"arcane deflection, tactical wit, power surge, durable magic, deflecting shroud"
};
	//war magic wizards gain a bonus to initiative that will need to be accounted for later (from their tactical wit ability)
static std::string* wizardSubclassAbilities[] = {
	artificerWizardAbilities, bladesingerWizardAbilities, loreMasterWizardAbilities, abjurationWizardAbilities, conjurationWizardAbilities, divinationWizardAbilities, enchantmentWizardAbilities, evocationWizardAbilities, illusionWizardAbilities, inventionWizardAbilities, necromancyWizardAbilities, transmutationWizardAbilities, technomancyWizardAbilities, theurgyWizardAbilities, warMagicWizardAbilities
};
#pragma endregion wizard abilities
#pragma region
static std::string runeScribeAbilities[] = {
	"rune lore, runic magic",
	"rune lore, runic magic, runic discovery",
	"rune lore, runic magic, runic discovery (2)",
	"rune lore, runic magic, runic discovery (2), living rune",
	"rune lore, runic magic, runic discovery (3), living rune, rune mastery"
};
//no subclasses to rune scribe
static std::string emptyAbilities[] = {
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};
//no subclasses to empty abilities (multiclass I guess)
static std::string subEmptyAbils[] = {
	""
};
static std::string* emptySubclassAbilities[] = {
	subEmptyAbils
};
#pragma endregion prestige & multiclass abilities
static std::string* classAbilities[] = {
	artificerAbilities, barbarianAbilities, bardAbilities, clericAbilities, druidAbilities, fighterAbilities, monkAbilities,
	mysticAbilities, paladinAbilities, rangerAbilities, rogueAbilities, sorcererAbilities, warlockAbilities,
	wizardAbilities, runeScribeAbilities, emptyAbilities
};
static std::string** subclassAbilities[] = {
	artificerSubclassAbilities, barbarianSubclassAbilities, bardSubclassAbilities, clericSubclassAbilities, druidSubclassAbilities, fighterSubclassAbilities, monkSubclassAbilities,
	mysticSubclassAbilities, paladinSubclassAbilities, rangerSubclassAbilities, rogueSubclassAbilities, sorcererSubclassAbilities, warlockSubclassAbilities,
	wizardSubclassAbilities, emptySubclassAbilities, emptySubclassAbilities
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
int secondaryRace; //represents secondary race if primary race is revanant (revenant is technically a subrace all races can take, but this is how we're representing it)
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
		if (classes[class_].compare("mystic") == STR_COMPARE_TRUE && subclasses[class_][subclass].compare("immortal") == STR_COMPARE_TRUE) {
			hitPoints += levelMainClass;
		}
		else if (classes[secondaryClass].compare("mystic") == STR_COMPARE_TRUE && subclasses[secondaryClass][secondarySubclass].compare("immortal") == STR_COMPARE_TRUE) {
			hitPoints += levelSecondaryClass;
		}
		if (classes[class_].compare("sorcerer") == STR_COMPARE_TRUE && (subclasses[class_][subclass].compare("draconic") == STR_COMPARE_TRUE || subclasses[class_][subclass].compare("stone") == STR_COMPARE_TRUE)) {
			hitPoints += levelMainClass;
		}
		else if (classes[secondaryClass].compare("sorcerer") == STR_COMPARE_TRUE && (subclasses[secondaryClass][secondarySubclass].compare("draconic") == STR_COMPARE_TRUE || subclasses[class_][subclass].compare("stone") == STR_COMPARE_TRUE)) {
			hitPoints += levelSecondaryClass;
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
			if (subraceNums[race] != 0 && subraceAbilities[race][subrace].compare("") != STR_COMPARE_TRUE) {
				//case: race abilities is not empty, subrace exists, subrace abilities is not empty
				std::cout << ", ";
			}
		}
		if (subraceNums[race] != 0) {	//only races with some number of subraces should have their subrace abilities printed
			std::cout << subraceAbilities[race][subrace];
		}
		if (secondaryRace != -1) {
			//if main race has secondary (i.e. if main is something like revenant), we need to also remember to print off the second race abilities
			if ((raceAbilities[race].compare("") != STR_COMPARE_TRUE || subraceAbilities[race][subrace].compare("") != STR_COMPARE_TRUE) && raceAbilities[secondaryRace].compare("") != STR_COMPARE_TRUE) {
				//case: race or subrace is not empty, and secondary race is not empty
				std::cout << ", ";
			}
			std::cout << raceAbilities[secondaryRace];
		}
		std::cout << std::endl;

			//class abilities
		std::cout << std::endl << "Class abilities: " << classAbilities[class_][levelMainClass - 1];
		if (classAbilities[class_][levelMainClass - 1].compare("") != STR_COMPARE_TRUE && subclassAbilities[class_][subclass][levelMainClass - 1].compare("") != STR_COMPARE_TRUE) {
			std::cout << ", ";
		}
		std::cout << subclassAbilities[class_][subclass][levelMainClass - 1];
		if (secondaryClass != -1) {
			std::cout << std::endl << "Second class abilities: " << classAbilities[secondaryClass][levelSecondaryClass - 1] << std::endl;
			if (classAbilities[secondaryClass][levelSecondaryClass - 1].compare("") != STR_COMPARE_TRUE && subclassAbilities[secondaryClass][secondarySubclass][levelSecondaryClass - 1].compare("") != STR_COMPARE_TRUE) {
				std::cout << ", ";
			}
			std::cout << subclassAbilities[secondaryClass][secondarySubclass][levelSecondaryClass - 1];
		}

		std::cout << std::endl << "-----------------------------------------" << std::endl << std::endl;
	} while (true);
}