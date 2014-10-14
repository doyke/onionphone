/*____________________________________________________________________________
	Copyright (C) 1996-1999 Network Associates, Inc.
	All rights reserved.

	$Id: HashWordList.cpp,v 1.5 1999/03/10 02:42:54 heller Exp $
____________________________________________________________________________*/
/*
   These two lists of words are for conveying data bytes via a voice
   channel.  They are analogous in purpose to the military alphabet
   used by pilots, and were carefully selected to be as phonetically
   distinct as possible.  This list was designed by Patrick Juola and
   Philip Zimmermann, using genetic algorithms to select lists of
   words that had optimum separations in phoneme space.

   Each list contains 256 phonetically distinct words, each word
   representing a different byte value between 0 and 255.  We use two
   lists, because reading aloud long random sequences of human words
   usually risks three kinds of errors: 1) transposition of two
   consecutive words, 2) duplicate words, or 3) omitted words. To
   detect all three kinds of errors, the two lists are used alternately
   for the even-offset bytes and the odd-offset bytes in the byte
   sequence. Each byte value is actually represented by two different
   words, depending on whether that byte appears at an even or an odd
   offset from the beginning of the byte sequence.  Using a two-list
   scheme was suggested by Zhahai Stewart.

   One suggested application for these word lists is to use a voice 
   channel to verify PGP public key fingerprints.

   Last revised 29 Sep 1998.

*/

#include "wordlist.h"



/* These 3-syllable words are no longer than 11 characters. */
const char hashWordListOdd[256][12] = {
	"adroitness",
	"adviser",
	"aftermath",
	"aggregate",
	"alkali",
	"almighty",
	"amulet",
	"amusement",
	"antenna",
	"applicant",
	"Apollo",
	"armistice",
	"article",
	"asteroid",
	"Atlantic",
	"atmosphere",
	"autopsy",
	"Babylon",
	"backwater",
	"barbecue",
	"belowground",
	"bifocals",
	"bodyguard",
	"bookseller",
	"borderline",
	"bottomless",
	"Bradbury",
	"bravado",
	"Brazilian",
	"breakaway",
	"Burlington",
	"businessman",
	"butterfat",
	"Camelot",
	"candidate",
	"cannonball",
	"Capricorn",
	"caravan",
	"caretaker",
	"celebrate",
	"cellulose",
	"certify",
	"chambermaid",
	"Cherokee",
	"Chicago",
	"clergyman",
	"coherence",
	"combustion",
	"commando",
	"company",
	"component",
	"concurrent",
	"confidence",
	"conformist",
	"congregate",
	"consensus",
	"consulting",
	"corporate",
	"corrosion",
	"councilman",
	"crossover",
	"crucifix",
	"cumbersome",
	"customer",
	"Dakota",
	"decadence",
	"December",
	"decimal",
	"designing",
	"detector",
	"detergent",
	"determine",
	"dictator",
	"dinosaur",
	"direction",
	"disable",
	"disbelief",
	"disruptive",
	"distortion",
	"document",
	"embezzle",
	"enchanting",
	"enrollment",
	"enterprise",
	"equation",
	"equipment",
	"escapade",
	"Eskimo",
	"everyday",
	"examine",
	"existence",
	"exodus",
	"fascinate",
	"filament",
	"finicky",
	"forever",
	"fortitude",
	"frequency",
	"gadgetry",
	"Galveston",
	"getaway",
	"glossary",
	"gossamer",
	"graduate",
	"gravity",
	"guitarist",
	"hamburger",
	"Hamilton",
	"handiwork",
	"hazardous",
	"headwaters",
	"hemisphere",
	"hesitate",
	"hideaway",
	"holiness",
	"hurricane",
	"hydraulic",
	"impartial",
	"impetus",
	"inception",
	"indigo",
	"inertia",
	"infancy",
	"inferno",
	"informant",
	"insincere",
	"insurgent",
	"integrate",
	"intention",
	"inventive",
	"Istanbul",
	"Jamaica",
	"Jupiter",
	"leprosy",
	"letterhead",
	"liberty",
	"maritime",
	"matchmaker",
	"maverick",
	"Medusa",
	"megaton",
	"microscope",
	"microwave",
	"midsummer",
	"millionaire",
	"miracle",
	"misnomer",
	"molasses",
	"molecule",
	"Montana",
	"monument",
	"mosquito",
	"narrative",
	"nebula",
	"newsletter",
	"Norwegian",
	"October",
	"Ohio",
	"onlooker",
	"opulent",
	"Orlando",
	"outfielder",
	"Pacific",
	"pandemic",
	"Pandora",
	"paperweight",
	"paragon",
	"paragraph",
	"paramount",
	"passenger",
	"pedigree",
	"Pegasus",
	"penetrate",
	"perceptive",
	"performance",
	"pharmacy",
	"phonetic",
	"photograph",
	"pioneer",
	"pocketful",
	"politeness",
	"positive",
	"potato",
	"processor",
	"provincial",
	"proximate",
	"puberty",
	"publisher",
	"pyramid",
	"quantity",
	"racketeer",
	"rebellion",
	"recipe",
	"recover",
	"repellent",
	"replica",
	"reproduce",
	"resistor",
	"responsive",
	"retraction",
	"retrieval",
	"retrospect",
	"revenue",
	"revival",
	"revolver",
	"sandalwood",
	"sardonic",
	"Saturday",
	"savagery",
	"scavenger",
	"sensation",
	"sociable",
	"souvenir",
	"specialist",
	"speculate",
	"stethoscope",
	"stupendous",
	"supportive",
	"surrender",
	"suspicious",
	"sympathy",
	"tambourine",
	"telephone",
	"therapist",
	"tobacco",
	"tolerance",
	"tomorrow",
	"torpedo",
	"tradition",
	"travesty",
	"trombonist",
	"truncated",
	"typewriter",
	"ultimate",
	"undaunted",
	"underfoot",
	"unicorn",
	"unify",
	"universe",
	"unravel",
	"upcoming",
	"vacancy",
	"vagabond",
	"vertigo",
	"Virginia",
	"visitor",
	"vocalist",
	"voyager",
	"warranty",
	"Waterloo",
	"whimsical",
	"Wichita",
	"Wilmington",
	"Wyoming",
	"yesteryear",
	"Yucatan"
	};

/* These 2-syllable words are no longer than 9 characters. */
const char hashWordListEven[256][10] = {
	"aardvark",
	"absurd",
	"accrue",
	"acme",
	"adrift",
	"adult",
	"afflict",
	"ahead",
	"aimless",
	"Algol",
	"allow",
	"alone",
	"ammo",
	"ancient",
	"apple",
	"artist",
	"assume",
	"Athens",
	"atlas",
	"Aztec",
	"baboon",
	"backfield",
	"backward",
	"banjo",
	"beaming",
	"bedlamp",
	"beehive",
	"beeswax",
	"befriend",
	"Belfast",
	"berserk",
	"billiard",
	"bison",
	"blackjack",
	"blockade",
	"blowtorch",
	"bluebird",
	"bombast",
	"bookshelf",
	"brackish",
	"breadline",
	"breakup",
	"brickyard",
	"briefcase",
	"Burbank",
	"button",
	"buzzard",
	"cement",
	"chairlift",
	"chatter",
	"checkup",
	"chisel",
	"choking",
	"chopper",
	"Christmas",
	"clamshell",
	"classic",
	"classroom",
	"cleanup",
	"clockwork",
	"cobra",
	"commence",
	"concert",
	"cowbell",
	"crackdown",
	"cranky",
	"crowfoot",
	"crucial",
	"crumpled",
	"crusade",
	"cubic",
	"dashboard",
	"deadbolt",
	"deckhand",
	"dogsled",
	"dragnet",
	"drainage",
	"dreadful",
	"drifter",
	"dropper",
	"drumbeat",
	"drunken",
	"Dupont",
	"dwelling",
	"eating",
	"edict",
	"egghead",
	"eightball",
	"endorse",
	"endow",
	"enlist",
	"erase",
	"escape",
	"exceed",
	"eyeglass",
	"eyetooth",
	"facial",
	"fallout",
	"flagpole",
	"flatfoot",
	"flytrap",
	"fracture",
	"framework",
	"freedom",
	"frighten",
	"gazelle",
	"Geiger",
	"glitter",
	"glucose",
	"goggles",
	"goldfish",
	"gremlin",
	"guidance",
	"hamlet",
	"highchair",
	"hockey",
	"indoors",
	"indulge",
	"inverse",
	"involve",
	"island",
	"jawbone",
	"keyboard",
	"kickoff",
	"kiwi",
	"klaxon",
	"locale",
	"lockup",
	"merit",
	"minnow",
	"miser",
	"Mohawk",
	"mural",
	"music",
	"necklace",
	"Neptune",
	"newborn",
	"nightbird",
	"Oakland",
	"obtuse",
	"offload",
	"optic",
	"orca",
	"payday",
	"peachy",
	"pheasant",
	"physique",
	"playhouse",
	"Pluto",
	"preclude",
	"prefer",
	"preshrunk",
	"printer",
	"prowler",
	"pupil",
	"puppy",
	"python",
	"quadrant",
	"quiver",
	"quota",
	"ragtime",
	"ratchet",
	"rebirth",
	"reform",
	"regain",
	"reindeer",
	"rematch",
	"repay",
	"retouch",
	"revenge",
	"reward",
	"rhythm",
	"ribcage",
	"ringbolt",
	"robust",
	"rocker",
	"ruffled",
	"sailboat",
	"sawdust",
	"scallion",
	"scenic",
	"scorecard",
	"Scotland",
	"seabird",
	"select",
	"sentence",
	"shadow",
	"shamrock",
	"showgirl",
	"skullcap",
	"skydive",
	"slingshot",
	"slowdown",
	"snapline",
	"snapshot",
	"snowcap",
	"snowslide",
	"solo",
	"southward",
	"soybean",
	"spaniel",
	"spearhead",
	"spellbind",
	"spheroid",
	"spigot",
	"spindle",
	"spyglass",
	"stagehand",
	"stagnate",
	"stairway",
	"standard",
	"stapler",
	"steamship",
	"sterling",
	"stockman",
	"stopwatch",
	"stormy",
	"sugar",
	"surmount",
	"suspense",
	"sweatband",
	"swelter",
	"tactics",
	"talon",
	"tapeworm",
	"tempest",
	"tiger",
	"tissue",
	"tonic",
	"topmost",
	"tracker",
	"transit",
	"trauma",
	"treadmill",
	"Trojan",
	"trouble",
	"tumor",
	"tunnel",
	"tycoon",
	"uncut",
	"unearth",
	"unwind",
	"uproot",
	"upset",
	"upshot",
	"vapor",
	"village",
	"virus",
	"Vulcan",
	"waffle",
	"wallet",
	"watchword",
	"wayside",
	"willow",
	"woodlark",
	"Zulu"
	};


    const char* getword(short b)
    {
     if(b>>8) return hashWordListEven[b&0xFF];
     else return hashWordListOdd[b&0xFF];  
    }
