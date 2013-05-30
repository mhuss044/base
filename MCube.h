// TODO: cubeBodyMind.knowFoodLocation() has gotta be more elegant
// TODO: backPack no give more food than it actually has
//                  backpack limit
//                  backpack....
// TODO: cubes cant escape smell sensitivity... even though they have tons of food in bag...
// TODO: cubes transfer food to nearest tribe member in dir of tribe loc, which transfers to enarest member (like a chain), resulting
//			eventually reaching tribe
// TODO: cubes get stuck at extremes of map for some reason

// Cube defining header.

// Cubes are 10x10x10
// Cubes are drawn by applying displacement to the original cube at
// 		bottom left vertice 0,0,0
// Cube colour determines health, life: healthy: green, Near death: grey, Dead: Black, Newborn?: Pink
// eventually: cubes will tell eachother, CubeGod: birth and kill cubes, CubeRegulator: regualate life time
// life: each time it moves a step, life--, not every frame, so place it in DecNextStep
// Wait at eating: when it finds food, it has to eat a little while, so have an 'eating' int, and decrease that everytime in decnextmove,
//		until 0, then will not be eating
// -------Include sense of smell, when close to food will come to it
// -------Will use memory to find food when hungry.
// Dormant stage: when cube reaches cetain food threshold(low), goes dormant, consumes low food during this phase, no other calcs happen
//      but check if food nearby(<20?) so remnove dormant flag
// Include disease/infection umong population
// Incorporate predators, cubes near pred will store in mem, then tell others nearby, have a influence on where cubes go
// Form packs: packs will have circle around them to indicate a pack, all move in unison, attack predators in unison, terretorial?
// disease circles: red circles, cube steps in, turns red, path changes to opposite of center of circle, disease state increases cube life decay
// Mind control fungus: member cube gets infected, parasite makes cube move to site of many member cubes, explode spread parasite
// multiplayer: can control cubes, makeup packs, fight off other packs, hunt, find food
//              tell allies where food is, tribal war? construct shelters..., weather -= health
// Advanced Paths: Pathbuilder(gives paths to food, enemies, etc): builds linked list of vectors to follow and mag for how long to follow to cubes
// Resources: food, water, materials
//		use resources to keep alive(food(80), water(10)), birth new into tribe(materials(80%), food(10), water(10))
// Tribes: cubes bind together in tribes in order to share resources, share birther
//			kicked from tribe: your food/resources low, tribe marks you for attack, member cubes consume life/food/resources
//
//      Tribe info report: as cube is moving through world, increases DEMANDINFOREPORT, when past certain threshold, stops going for materials and set path toward
//                               tribes birther, once there gives up location of food/materials found, goes back out into world.
// Cube behaivior classes: WARRIOR(takes life, foodLoc/MaterialLoc info from slain, messengers give loc of enemy tribe members, defends gatherers),
//							FORAGER(looks for food), BUILDER(assembles birther, structures), materialGatherer(gathers building materials)
//							MESSENGER(INFO DISSEMINATOR, to predator: give loc of enemy tribe members, to foragers; give loc of food, changes role of cubes; goes
//									out changes 1 cubes role, back to tribe get #change role, go back out)
//							FEEDER: has bigger than normal food backpack, moves thru field feeding cubes so that dont starve/wont serach for their own food/can keep foraging
// Cube upgrades: cube handsize(how much food/resources a cube can grab), cube activityFactor(makes cubes faster), cube backpackSize(how much can hold in sac)
//                  cubeStaminaFactor,
// Cube batt\les: maybe when nontribe cube enters tribe territory, set to attack in attack list, have function that returns value quantifying cubes 'fight' ability,
//				 have it so that cubes with great health/materials, gain advantage, but increased number of attackers increases probability of death
// CubeGod: random upgrade placing function, use have two rand nums check w/m0odulating factor, place some of the avail drops for cubes to pick up
// Paths:
//		lay trails to enemies/food/material
//		tribememb dies via attack; gives out radius of scent to avoid
//		path to nest
//		path to queen

// Research Nest:
//		nest upgrades: on site garden up (higher yeild crops, more crops sustained)
//		warrior ups: faster warriors, better killers, bigger packs to carry caught food
//		nest defenses: ... venom stains that cant be moved past if enemy?
//		wings; faster movement? land fairing cant attack
//		enhanced pheromone glands; longer lasting pheromones
//
//	Queen/Master lays eggs in cocoons, drones construct cocoons from material

// PLAYERCUBE; controls special cube, whereever pos, press button, tells players tribe where food is, where
// 		resources(use to build cubes are, fights predators, recruits cubes to aid in predator(drop materials, or special mats) fights


// Method to store & calculate location data.
// 		use this to tell predators where prey are,
// 		use this to tell messengers where tribe members are
// 		use this to tell foragers where others have looked for food
// 		Mayb a vector origin@tribeHQ, pointing to desired data?
//			so if messenger hits tribe member, vector add b/w vec of member&HQ and messenger's vector for tribe member pos
//				get a consensus vec pointing toward most likely pos of members>?
//			use magnitude to get a weighted val, so harder to offset?

#define NUMCUBES 500
#define CUBE_LIFE_DECAY 3                                              // Value by which cube life decreases per cycle
#define CUBE_ACTIVITY 40                                              // Value which determines how often cubes move(decToGo), lower = more activity
#define CUBE_SMELL_SENSITIVITY 150                                     // Distance cubes can smell food
#define CUBE_SIGHT_RADIUS 150                                     // Distance cubes can smell food
#define CUBE_STAMINA 30                                                // Distance cube moves along a path(of its choosing), 10 = grid
#define CUBE_BACKPACK_FOODLIMIT 500										// Max amount of food can carry
#define CUBE_BACKPACK_MATERIALLIMIT 300										// Max amount of resources can carry

// CUBETYPE: Forager
#define CUBETYPE_FORAGER_FOODPACK_THRESHOLD 200						// how many cubes messenger contacts before return to tribe

// CUBETYPE: Messenger
#define CUBETYPE_MESSENGER_SIGHTRANGE 80							// how far messenger must be before relay messages
#define CUBETYPE_MESSENGER_CONTACT_THRESHOLD 20						// how many cubes messenger contacts before return to tribe

// CUBETYPE: Feeder
#define CUBETYPE_FEEDER_PACKSIZE_THRESHOLD 100						// Amount of stuff before return to get more food
#define CUBETYPE_FEEDER_PACKSIZE CUBE_BACKPACK_FOODLIMIT*2			// Max amount of stuff feeder can carry
#define CUBETYPE_FEEDER_SIGHTRANGE 200

// CUBETYPE: Predator
#define CUBETYPE_PREDATOR_PACKSIZE CUBE_BACKPACK_FOODLIMIT*2
#define CUBETYPE_PREDATOR_SIGHTRANGE 100
#define CUBETYPE_PREDATOR_ATTACK_THRESHOLD 5

// TRIBE SOECIFIC
#define TRIBE_NUMTRIBES 5                                             // Max number of tribes
#define TRIBE_MAXMEMBERS 100										      // Max number of tribe members per tribe
#define TRIBE_TERRITORY_RADIUS 150										      // Max number of tribe members per tribe

// Define modifyible stats of cube
typedef struct _tribeToCubeInfo
{
	bool tribeHasFood;
	int tribeCallBackThreshold, lastTribeMemberContacted;											// return to tribe demand
	Vec2d tribePos;														// holds pos of tribe base

	_tribeToCubeInfo()
	{
		tribeHasFood = false;
		tribeCallBackThreshold = 0;
		lastTribeMemberContacted = 0;
		tribePos.e1 = -998;												// symbolize no tribe
		tribePos.e2 = -998;
	}
}cubesTribeInfo;

// Define modifyible stats of cube
typedef struct _cubeModifiers
{
	int modifier_cubeLife, modifier_cubeBackPackSize;
	int modifier_cubeActivity, modifier_cubeStamina;
	_cubeModifiers()
	{
		modifier_cubeLife = 0;
		modifier_cubeBackPackSize = 0;
		modifier_cubeActivity = 0;
		modifier_cubeStamina = 0;
	}
}cubeModifiers;

cubeModifiers WARRIOR_TYPE;
//WARRIOR_TYPE.modifier_cubeLife = 100;

cubeModifiers FORAGER_TYPE;
cubeModifiers MESSENGER_TYPE;
cubeModifiers FEEDER_TYPE;

enum CUBETYPES
{
	FORAGER = 0,			// with enums, set val of 1 field, the other fields become set accordingly
	PREDATOR,
	MESSENGER,
	FEEDER
};

//-------------------------------------------------------------------------------------
//---------------------CUBE CubeBackPack-----------------------------------------------
//---------------------CONTAINED WITHIN CUBE CLASS-------------------------------------
//-------------------------------------------------------------------------------------

class CCubeBackPack													    // Methods woring with the cubes backpack: addResources, addFood, giveFood,...
{
private:
	int foodStored;                                                     // Food stored in units of 1, so 1 int = 1 food unit = 1 health unit
	int materialStored;                                                // Resources stored

public:
    CCubeBackPack(void);
    int getFoodStored(void);
    int getMaterialStored(void);
	bool isFoodPackFull(void);
	bool isMaterialPackFull(void);
    bool isPrepUnloadFood(void);	// Cube comfortable returning food>? got enough for survival?
    bool isPrepUnloadMaterial(void);	// Cube comfortable returning food>? got enough for survival?
    void addFood(int howMuchFood, int backPackSizeMod);           // adds howMuchFood to foodStored
    void addMaterial(int howMuchMaterial, int backPackSizeMod);   // adds howMuchResources to resourcesStored
    int giveFood(int howMuchFood);                                    //  subtracts howMuchFood from foodStored, and returns howMuchFood
    int giveMaterial(int howMuchMaterial);                            //  subtracts howMuchResources from resourcesStored, and returns howMuchResources
};

CCubeBackPack::CCubeBackPack(void)
{
    foodStored = 0;
    materialStored = 0;
}

void CCubeBackPack::addFood(int howMuchFood, int backPackSizeMod = 0)
{
    if(foodStored < (CUBE_BACKPACK_FOODLIMIT+backPackSizeMod))
    	foodStored += howMuchFood;
    if(foodStored > (CUBE_BACKPACK_FOODLIMIT+backPackSizeMod))	// make sure dont ever have >limit
    	foodStored = (CUBE_BACKPACK_FOODLIMIT+backPackSizeMod);
}

void CCubeBackPack::addMaterial(int howMuchMaterial, int backPackSizeMod = 0)
{
    if(materialStored < (CUBE_BACKPACK_MATERIALLIMIT+backPackSizeMod))
    	materialStored += howMuchMaterial;
    if(materialStored > (CUBE_BACKPACK_MATERIALLIMIT+backPackSizeMod))	// make sure dont ever have >limit
    	materialStored = (CUBE_BACKPACK_MATERIALLIMIT+backPackSizeMod);
}

int CCubeBackPack::giveFood(int howMuchFood)
{
    if(howMuchFood > foodStored)
    {
    /*  while(1)                                        // too intensive, use % operator to tell
        {
            howMuchFood--;
            if(howMuchFood <= foodStored)
                break;
        }
    */
        howMuchFood = 0;
    }

    foodStored -= howMuchFood;
    return howMuchFood;
}

int CCubeBackPack::giveMaterial(int howMuchMaterial)
{
    if(howMuchMaterial > materialStored)
    {
    /*  while(1)                                        // too intensive, use % operator to tell
        {
            howMuchFood--;
            if(howMuchFood <= foodStored)
                break;
        }
    */
    	howMuchMaterial = 0;
    }

    materialStored -= howMuchMaterial;
    return howMuchMaterial;
}

int CCubeBackPack::getFoodStored(void)
{
	return foodStored;
}

int CCubeBackPack::getMaterialStored(void)
{
	return materialStored;
}

bool CCubeBackPack::isFoodPackFull(void)
{
	if(foodStored >= CUBE_BACKPACK_FOODLIMIT)
		return true;
	else
		return false;
}

bool CCubeBackPack::isMaterialPackFull(void)
{
	if(materialStored >= CUBE_BACKPACK_MATERIALLIMIT)
		return true;
	else
		return false;
}

bool CCubeBackPack::isPrepUnloadFood(void)
{
	if(foodStored >= 200)
		return true;
	else
		return false;
}

bool CCubeBackPack::isPrepUnloadMaterial(void)
{
	if(materialStored > 0)
		return true;
	else
		return false;
}

//-------------------------------------------------------------------------------------
//-------^^^^^^^^^^^---CUBE CubeBackPack---------^^^^^^^^^^^^^^^^^^^^^^^---------------
//---------------------CONTAINED WITHIN CUBE CLASS-------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//---------------------CUBE CubeBodyMind-----------------------------------------------
//---------------------CONTAINED WITHIN CUBE CLASS-------------------------------------
//-------------------------------------------------------------------------------------

class CCubeBodyMind				    									// Methods working with physical/mental?, memory,speed,health
{
public:
	int cubeLife;
	cubeModifiers cubeMods;
	cubesTribeInfo infoOnTribe;											// Struct contains info on tribe;
	GLfloat cubeColour[3], tribeColour[3];
	Vec2d foodMem;														// holds positions of known food.
	Vec2d materialMem;													// holds position of known materials

    CCubeBodyMind(void);
	void decreaseLife(void);
	void editMem(int h, int v);											// Edit cube memory of food
	void checkFoodMemory(void);                                         // Check food mem if valid location for food, if not delete it
	void checkMaterialMemory(void);
	bool knowFoodLocation(void);                                        // reutrn true if has correct food location in memory
	bool knowMaterialLocation(void);									// return true if has correct resource location in memory
	bool foundFood(int h, int v);										// Found some food?
	bool foundMaterial(int h, int v);									// Found building materials?
	bool isTribeMember(void);
	bool isHungry(void);												// Should cube eat?
	void eat(int howMuchToEat);			    							// Cube has eaten
};

CCubeBodyMind::CCubeBodyMind(void)
{
	cubeLife = 100;

	// Set colour to healthy(green):
	cubeColour[0] = 0;													// R
	cubeColour[1] = 1;													// G
	cubeColour[2] = 0;													// B

    // Set colour to default tribe colour:
	tribeColour[0] = 0;													// R
	tribeColour[1] = 1;													// G
	tribeColour[2] = 0;													// B

	// Set memory to empty:
	foodMem.e1 = -998;
	foodMem.e2 = -998;

	materialMem.e1 = -998;
	materialMem.e2 = -998;
}

void CCubeBodyMind::decreaseLife(void)									// Called every step.
{
	cubeLife -= CUBE_LIFE_DECAY;                                        // Decrease life

    if(cubeLife <= 0)                                                   // Dont want negative life
    {
        cubeLife = 0;
        return;
    }

	if(cubeLife < 50)													// Near death, make hungry colour
	{
		cubeColour[0] = 0.3;
		cubeColour[1] = 0.4;
	}
	if(cubeLife < 20)                                                   // Dead colour
	{
		cubeColour[0] = 0;
		cubeColour[1] = 0;
		cubeColour[2] = 0;
	}
}

void CCubeBodyMind::editMem(int x, int z)								// Called when food is found, and loc needs to be recorded.
{
	// Set mem;
	foodMem.e1 = x;
	foodMem.e2 = z;
}

bool CCubeBodyMind::foundFood(int x, int z)							    // Found food? and loc needs to be recorded.
{
	if(Distance2f(x, z, gameFoodLocX, gameFoodLocZ) < 20)               // If cube withing 1 square of food
	{
		foodMem.e1 = gameFoodLocX;
		foodMem.e2 = gameFoodLocZ;
		return true;
	}
    else
		return false;
}

bool CCubeBodyMind::foundMaterial(int x, int z)						    // Found material? and loc needs to be recorded.
{
	if(Distance2f(x, z, gameMaterialLocX, gameMaterialLocZ) < 20)               // If cube withing 1 square of material
	{
		materialMem.e1 = gameMaterialLocX;
		materialMem.e2 = gameMaterialLocZ;
		return true;
	}
    else
		return false;
}

bool CCubeBodyMind::knowFoodLocation(void)      					    // Gotta be a better way to do this....
{
	if(foodMem.e1 == -998 || foodMem.e1 == -1)
		return false;
    else
		return true;
}

bool CCubeBodyMind::knowMaterialLocation(void)      				    // Gotta be a better way to do this....
{
	if(materialMem.e1 == -998)
		return false;
    else
		return true;
}

void CCubeBodyMind::checkFoodMemory(void)	// This way cube verifies data, its not cheating, bcs how else would cube know food is there when its actually travelled there>?
{
    if(foodMem.e1 != gameFoodLocX || foodMem.e2 != gameFoodLocZ)
    {
        foodMem.e1 = -998;
        foodMem.e2 = -998;
    }
}

void CCubeBodyMind::checkMaterialMemory(void)
{
    if(materialMem.e1 != gameMaterialLocX || materialMem.e2 != gameMaterialLocZ)
    {
    	materialMem.e1 = -998;
    	materialMem.e2 = -998;
    }
}

bool CCubeBodyMind::isTribeMember(void)										// Called when food is found, and loc needs to be recorded.
{
	if(infoOnTribe.tribePos.e1 != -998)
		return true;
	else
		return false;
}

bool CCubeBodyMind::isHungry(void)										// Called when food is found, and loc needs to be recorded.
{
	if(cubeLife < 40)
		return true;
	else
		return false;
}

void CCubeBodyMind::eat(int howMuchToEat)								// Called when food is found, and loc needs to be recorded.
{
	cubeLife += howMuchToEat;

	// Set colour to healthy:
	if(cubeLife >= 50)                                                  // check if cube at max health
	{
        cubeColour[0] = 0;													// R
        cubeColour[1] = 1;													// G
        cubeColour[2] = 0;													// B
	}
}

//-------------------------------------------------------------------------------------
//-------^^^^^^^^^^^^--CUBE CubeBodyMind--------------^^^^^^^^^^^^^^^^-----------------
//---------------------CONTAINED WITHIN CUBE CLASS-------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//---------------------CUBE CLASS------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
class CCube
{
private:
    Vec2d moveDir;                                                      // Movement direction; e1: x dir, e2; ydir:: possible vals: 0, 1, -1
    int moveMag;                                                        // Magnitude of move vector
public:
	Vec2d disp;															// Displacement: e1: X(mapwidth), e2: Z(mapheight)
    Vec2d cubeSectorXZ, prevCubeSectorXZ;								// e1 = X, e2 = Z: possible vals: -1, 1
    CUBETYPES cubeType;
    CCubeBodyMind cubesBodyMind;										// Methods which makeup a cubes body/mind
    CCubeBackPack cubesBackPack;                                        // Methods working on the contents of cubesBackPack, privately holds food/resource stores

	CCube(int h, int v);
	void cubeLifeCycle(void);											// Concerned with processing of cubes individual descision making and movement
	void renderSelf(void);												// renders cube in the world
	void decNextPath(void);												// Picks next position
	bool decToGo(void);													// Chooses to go or not
	bool smellFood(int foodLocH, int foodLocV);
	bool materialInSight(int materialLocX, int materialLocZ);
	void keepInBounds(void);                                            // Cube keeps itself in bounds
	bool isMoveMagZero(void);                                           // is Magnitude of movement vector zero?
	void moveDetPath(void);                                             // Move determined path, as in, cube randomly chose path
	void carryOutMove(void);                                            // Move along path
	void pickNewPathDir(bool isRand, int posH, int posV);               // Cube decides direction for new path, random or toward posH/V
	void setMoveMag(int mag);                                           // Sets distance cube moves along a path(of its choosing)
	void setPathTowardFood(bool fromMemory);                            // Cube moves toward food
	void setPathTowardMaterial(bool fromMemory);						// Cube moves toward materials
	void moveAnyRandDir(void);                                          // Cube picks random dir, moves 1 grid
	void drawPath(int lengthToDraw);
    bool desireSectorChange(void);										// Set sector vals, use cubes position, return true if require change of sector
};

CCube::CCube(int h = GAME_MAP_SIZE_X/2, int v = GAME_MAP_SIZE_Z/2)		// Set horizontal & vertical spawn pt; spawn mid map
{
	disp.e1 = h;
	disp.e2 = v;
	moveMag = 0;				// else you will get an initial drawPath huge

	// Set sector to 0(impossible but, ... i think its ok?) so that wont change sector initially..
	cubeSectorXZ.e1 = 0;		// X
	cubeSectorXZ.e2 = 0;		// Z
	prevCubeSectorXZ.e1 = 0;	// X
	prevCubeSectorXZ.e2 = 0;	// Z

	// Set all cubes to forager initially
	cubeType = FORAGER;
}

// Rename to: Think? or cubeLifeIteration/cubeLifeCycle/decideNextPath
void CCube::decNextPath(void)											// Picks next position, or edits position
{
// DEBUG DEBUG DEBUG DEBUGDEBUG DEBUGVDEBUG DEBUGDEBUG DEBUG
/*
    cout << INS << cubesBodyMind.cubeLife;
    cout << INS << disp.e1 << " , " << disp.e2;
    cout << INS << gameFoodLocZ << " , " << gameFoodLocX;
    cout << INS << Distance2f(disp.e1, disp.e2, gameFoodLocZ, gameFoodLocX);
*/

	// Keep in bounds:
	if(disp.e1 > GAME_MAP_SIZE_X || disp.e1 < 0)						// Out of bounds?
		disp.e1 > GAME_MAP_SIZE_X ? disp.e1 -= 30 : disp.e1 += 30;		//	if true -> disp.e1 -= 30, false then disp.e1 < 0 -> disp.e1 += 30

	if(disp.e2 > GAME_MAP_SIZE_Z || disp.e2 < 0)						// Out of bounds?
		disp.e2 > GAME_MAP_SIZE_Z ? disp.e2 -= 30 : disp.e2 += 30;		//	if true -> disp.e2 -= 30, false then disp.e2 < 0 -> disp.e2 += 30

	// Add as much as can carry:
	if(cubesBodyMind.foundFood(disp.e1, disp.e2))                       // Found food?
		cubesBackPack.addFood(CUBE_BACKPACK_FOODLIMIT+cubesBodyMind.cubeMods.modifier_cubeBackPackSize-1, cubesBodyMind.cubeMods.modifier_cubeBackPackSize);
	// Add Material from cubes location to napsack:
	if(cubesBodyMind.foundMaterial(disp.e1, disp.e2))             		// Found Material?
        cubesBackPack.addMaterial(CUBE_BACKPACK_MATERIALLIMIT+cubesBodyMind.cubeMods.modifier_cubeBackPackSize-1, cubesBodyMind.cubeMods.modifier_cubeBackPackSize);

    switch(cubeType)
    {
    	case FORAGER:
    	    if(isMoveMagZero())                                                 // Cube is at end of last path, pick new path for
    	    {
    	        if(cubesBodyMind.isHungry())                                        // Is cube hungry? then choose what to do about it
    	        {
    	            // Check smell/memory, then set path to food:
    	            if(smellFood(gameFoodLocX, gameFoodLocZ))                       // if cube smells food, and is hungry = goes to food
    	                setPathTowardFood(false);
    	            else if(cubesBodyMind.knowFoodLocation())                       // if cube has food lcoation in memory, and is hungry, = goes to food
    	            {
    	                setPathTowardFood(true);                                    // Set path toward food from memory
    	                cubesBodyMind.checkFoodMemory();                            // Check if memory is still good, ie if its not, delete it
    	            }
    	            else if(cubesBodyMind.isTribeMember() && cubesBodyMind.infoOnTribe.tribeHasFood)// Have not found any food, and dont know wher any is, = get info from tribe
    	            {// gtfo and look for food you moochers
    	            //    pickNewPathDir(false, cubesBodyMind.infoOnTribe.tribePos.e1, cubesBodyMind.infoOnTribe.tribePos.e2);
    	            }
    	            else														// Not part of tribe, go random dir
    	            {
    	            	pickNewPathDir(true, 0, 0);
    	            }
    	        }
    	        else if(cubesBodyMind.isTribeMember() && (cubesBackPack.isPrepUnloadFood() || cubesBackPack.isMaterialPackFull()))
    	        {
    	            pickNewPathDir(false, cubesBodyMind.infoOnTribe.tribePos.e1, cubesBodyMind.infoOnTribe.tribePos.e2);
    	        }
    	        else                                                                // Cube isnt hungry
    	        {
    	            // Cube looks for resources, set path toward resources:
    	            // Check sight/memory, then set path to material:
    	            if(materialInSight(gameFoodLocX, gameFoodLocZ))                 // if cube has material in sight= goes to material
    	            	setPathTowardMaterial(false);
    	            else if(cubesBodyMind.knowMaterialLocation())                   // if cube has material location in memory = goes to material
    	            {
    	            	setPathTowardMaterial(true);                                // Set path toward material from memory
    	                cubesBodyMind.checkMaterialMemory();                        // Check if memory is still good, ie if its not, delete it
    	            }
    	            else															// Have not found any food, and dont know wher any is, = get info from tribe
    	            {
    				   pickNewPathDir(true, 0, 0);// Not part of tribe, go random dir
    	            }
    	        }

    	        // Have a new path from the above, so set mag to move along this new path:
    	        setMoveMag(CUBE_STAMINA+cubesBodyMind.cubeMods.modifier_cubeStamina);                                           // Set dist to move along path
    	    }
    	    else    // Move mag not zero, but if pass by food, change path, or if suddenly learned of food, change path
    	    {
    	        if(cubesBodyMind.isHungry())                                        // Is cube hungry? then choose what to do about it
    	        {
    	            // Check smell/memory, then set path to food:
    	            if(smellFood(gameFoodLocX, gameFoodLocZ))                       // if cube smells food, and is hungry = goes to food
    	                setPathTowardFood(false);
    	            else if(cubesBodyMind.knowFoodLocation())                       // if cube has food lcoation in memory, and is hungry, = goes to food
    	            {
    	                setPathTowardFood(true);                                    // Set path toward food from memory
    	                cubesBodyMind.checkFoodMemory();                            // Check if memory is still good, ie if its not, delete it
    	            }
    	        }
    	        else
    	        {
    	            if(materialInSight(gameFoodLocX, gameFoodLocZ))                 // if cube has material in sight= goes to material
    	            	setPathTowardMaterial(false);
    	            else if(cubesBodyMind.knowMaterialLocation())                   // if cube has material location in memory = goes to material
    	            {
    	            	setPathTowardMaterial(true);                                // Set path toward material from memory
    	                cubesBodyMind.checkMaterialMemory();                        // Check if memory is still good, ie if its not, delete it
    	            }
    	        }
    	    }
    	break;
    	case FEEDER:
			if(isMoveMagZero())                                                 // Cube is at end of last path, pick new path for
			{
				if(cubesBodyMind.isHungry() || cubesBackPack.getFoodStored() <= CUBETYPE_FEEDER_PACKSIZE_THRESHOLD)  // Is feeder hungry/low food? choose from options
				{
					// Check smell/memory, then set path to food:
					if(smellFood(gameFoodLocX, gameFoodLocZ))                       // if cube smells food, and is hungry = goes to food
						setPathTowardFood(false);
					else if(cubesBodyMind.knowFoodLocation())                       // if cube has food lcoation in memory, and is hungry, = goes to food
					{
						setPathTowardFood(true);                                    // Set path toward food from memory
						cubesBodyMind.checkFoodMemory();                            // Check if memory is still good, ie if its not, delete it
					}
					else if(cubesBodyMind.isTribeMember() && cubesBodyMind.infoOnTribe.tribeHasFood)// Have not found any food, and dont know wher any is, = get info from tribe
					{
						pickNewPathDir(false, cubesBodyMind.infoOnTribe.tribePos.e1, cubesBodyMind.infoOnTribe.tribePos.e2);
					}
					else														// Not part of tribe or tribe has no food, go random dir
					{
						pickNewPathDir(true, 0, 0);
					}
				}
    	        else if(cubesBodyMind.isTribeMember() && (cubesBackPack.isPrepUnloadFood() || cubesBackPack.isMaterialPackFull()))
    	        {
    	            pickNewPathDir(false, cubesBodyMind.infoOnTribe.tribePos.e1, cubesBodyMind.infoOnTribe.tribePos.e2);
    	        }
				else                                                            // Feeder isnt hungry/low on food
				{
					// Feeder moves rand, feeding
					pickNewPathDir(true, 0, 0);// Not part of tribe, go random dir
				}

				// Have a new path from the above, so set mag to move along this new path:
				setMoveMag(CUBE_STAMINA+cubesBodyMind.cubeMods.modifier_cubeStamina);                                           // Set dist to move along path
			}
			else    // Move mag not zero, but if pass by food, change path, or if suddenly learned of food, change path
			{
				if(cubesBodyMind.isHungry() || cubesBackPack.getFoodStored() <= CUBETYPE_FEEDER_PACKSIZE_THRESHOLD)                                        // Is cube hungry? then choose what to do about it
				{
					// Check smell/memory, then set path to food:
					if(smellFood(gameFoodLocX, gameFoodLocZ))                       // if cube smells food, and is hungry = goes to food
						setPathTowardFood(false);
					else if(cubesBodyMind.knowFoodLocation())                       // if cube has food lcoation in memory, and is hungry, = goes to food
					{
						setPathTowardFood(true);                                    // Set path toward food from memory
						cubesBodyMind.checkFoodMemory();                            // Check if memory is still good, ie if its not, delete it
					}
				}
			}
		break;
    	case MESSENGER:
    	    if(isMoveMagZero())                                                 // Cube is at end of last path, pick new path for
    	    {/*
    	        if(cubesBodyMind.isHungry())                          	            // Is cube hungry? then choose what to do about it
    	        {
					if(cubesBodyMind.isTribeMember() && cubesBodyMind.infoOnTribe.tribeHasFood)// Is cube a memeber of a tribe
						pickNewPathDir(false, cubesBodyMind.infoOnTribe.tribePos.e1, cubesBodyMind.infoOnTribe.tribePos.e2);
    	            else
    	            	pickNewPathDir(true, 0, 0);// tribe has no food, dont smell any, dont know where any food is, go random dir
    	        }
    	        else */
    	    	if(cubesBodyMind.isTribeMember() && cubesBodyMind.infoOnTribe.tribeCallBackThreshold >= CUBETYPE_MESSENGER_CONTACT_THRESHOLD)
    	        {
    	            pickNewPathDir(false, cubesBodyMind.infoOnTribe.tribePos.e1, cubesBodyMind.infoOnTribe.tribePos.e2);
    	        }
    	        else                                                                // Cube isnt hungry
    	        {
    	        	pickNewPathDir(true, 0, 0);// not hungry, less than contact thres, go random dir
    	        }

    	        // Have a new path from the above, so set mag to move along this new path:
    	        setMoveMag(CUBE_STAMINA+cubesBodyMind.cubeMods.modifier_cubeStamina);                                           // Set dist to move along path
    	    }
    	    else    // Move mag not zero, but if pass by food, change path, or if suddenly learned of food, change path
    	    {/*
    	    // messenger has a job to do, so this isnt appropriate?
    	        if(cubesBodyMind.isHungry())                                        // Is cube hungry? then choose what to do about it
    	        {
    	            // Check smell/memory, then set path to food:
    	        	if(smellFood(gameFoodLocX, gameFoodLocZ))                       // if cube smells food, and is hungry = goes to food
    	                setPathTowardFood(false);
    	            else if(cubesBodyMind.knowFoodLocation())                       // if cube has food lcoation in memory, and is hungry, = goes to food
    	            {
    	                setPathTowardFood(true);                                    // Set path toward food from memory
    	                cubesBodyMind.checkFoodMemory();                            // Check if memory is still good, ie if its not, delete it
    	            }
    	        }*/
     	    }
    	break;
    	case PREDATOR:
      	    if(isMoveMagZero())                                                 // Cube is at end of last path, pick new path for
        	    {
        	        if(cubesBodyMind.isHungry())                          	            // Is cube hungry? then choose what to do about it
        	        {
    					// use potential field to find dir where most likely find prey
        	        	pickNewPathDir(true, 0, 0);// not hungry, less than contact thres, go random dir
        	        }
        	        else if(cubesBodyMind.isTribeMember() && cubesBodyMind.infoOnTribe.tribeCallBackThreshold >= CUBETYPE_PREDATOR_ATTACK_THRESHOLD)
        	        {
        	            pickNewPathDir(false, cubesBodyMind.infoOnTribe.tribePos.e1, cubesBodyMind.infoOnTribe.tribePos.e2);
        	        }
        	        else                                                                // Cube isnt hungry
        	        {
        	        	pickNewPathDir(true, 0, 0);// not hungry, less than contact thres, go random dir
        	        }

        	        // Have a new path from the above, so set mag to move along this new path:
        	        setMoveMag(CUBE_STAMINA+cubesBodyMind.cubeMods.modifier_cubeStamina);                                           // Set dist to move along path
        	    }
        	    else    // Move mag not zero, but if pass by food, change path, or if suddenly learned of food, change path
        	    {/*
        	    // messenger has a job to do, so this isnt appropriate?
        	        if(cubesBodyMind.isHungry())                                        // Is cube hungry? then choose what to do about it
        	        {
        	            // Check smell/memory, then set path to food:
        	        	if(smellFood(gameFoodLocX, gameFoodLocZ))                       // if cube smells food, and is hungry = goes to food
        	                setPathTowardFood(false);
        	            else if(cubesBodyMind.knowFoodLocation())                       // if cube has food lcoation in memory, and is hungry, = goes to food
        	            {
        	                setPathTowardFood(true);                                    // Set path toward food from memory
        	                cubesBodyMind.checkFoodMemory();                            // Check if memory is still good, ie if its not, delete it
        	            }
        	        }*/
         	    }
    	break;
    }
}

bool CCube::decToGo(void)												// Move or not?
{
	// If two random numbers are the same, cube goes
	if(rand()%(CUBE_ACTIVITY+cubesBodyMind.cubeMods.modifier_cubeActivity) + 1 == rand()%(CUBE_ACTIVITY+cubesBodyMind.cubeMods.modifier_cubeActivity) + 1)
	{	                                                                // Increase the range to slow movement
        // Decrease life, then go:
        cubesBodyMind.decreaseLife();
		return true;
	}
	else
		return false;
}

// Rename to: Animate?
void CCube::cubeLifeCycle(void)											// Stuff that happens to cube every cycle
{
    if(cubesBodyMind.isHungry())                                        // Is cube hungry?
        cubesBodyMind.eat(cubesBackPack.giveFood(50));				// Eat some food from backpack

	// Move:
	if(decToGo())														// Regulate when cube can move, depends on cube activity factor
		decNextPath();

    // Move along determined path:
    moveDetPath();
    // Draw cube's path
    drawPath(20);                                                       // distance to draw
}

void CCube::renderSelf(void)
{
	// Draw cube:
	glPushMatrix();
		glTranslated(disp.e1, 0, disp.e2);								// Move to position
		glColor3d(0, 0, 0);// Set to white colour
		glColor3fv(cubesBodyMind.cubeColour);							// Set to cube colour
		DrawCubeNoBottom();												// Draw bottomless cube

        // Draw cube's tribe identifier:
		glColor3d(0, 0, 0);                                             // Set to white colour
		glColor3fv(cubesBodyMind.tribeColour);							// Set to cube's tribe colour
		glTranslated(0, 10, 0);                                         // Move 10 in ydir above cube
		glPushMatrix();
            glTranslated(5, 0, -5);					            		// Move unit triangle to middle of cube, since cube pos is bottom left of cube model
            glScaled(10, 10, 10);                                       // Transform unit triangle
            DrawTriangle();                                             // Draw tribe identifier
        glPopMatrix();

        switch(cubeType)
        {
        	case FORAGER:
        	break;
        	case MESSENGER:
        		glPushMatrix();
        			glColor3d(0, 0, 0);									// Set to white colour
        			if(cubesBodyMind.knowFoodLocation())
        				glColor3d(0, 0, 1);									// set to blue MESSENGER colour
        			else
        				glColor3d(1, 0, 0);									// set to red MESSENGER colour
        			glRotatef(90, 1, 0, 0);						// rotate into xz plane
					glScalef(1.0, 1.0, 0.2);                    // make the profile in z axis less
					glutSolidTorus(1,CUBETYPE_MESSENGER_SIGHTRANGE+1,10,10);
				glPopMatrix();
        	break;
        	case FEEDER:
        		glPushMatrix();
        			glColor3d(0, 0, 0);									// Set to white colour
        			if(cubesBodyMind.isHungry())
        				glColor3d(0, 1, 1);									// set to purple(?) FEEDER colour
        			else
        				glColor3d(0, 1, 0);									// set to green FEEDER colour
        			glRotatef(90, 1, 0, 0);						// rotate into xz plane
					glScalef(1.0, 1.0, 0.2);                    // make the profile in z axis less
					glutSolidTorus(1,CUBETYPE_FEEDER_SIGHTRANGE+1,10,10);
				glPopMatrix();
        	break;
        	case PREDATOR:
        		glPushMatrix();
        			glColor3d(0, 0, 0);									// Set to white colour
        			glColor3d(1, 0, 0);									// set to red PREDATOR colour
        			glRotatef(90, 1, 0, 0);						// rotate into xz plane
					glScalef(1.0, 1.0, 0.2);                    // make the profile in z axis less
					glutSolidTorus(1,CUBETYPE_PREDATOR_SIGHTRANGE+1,10,10);
				glPopMatrix();
        	break;
        }

		// Draw current food stores, represented as cubes above primary cube
		for(int x = 0; x < cubesBackPack.getFoodStored(); x+= 100)		// 100 food/cube
		{
			glTranslated(0, 10, 0);										// already trans atop cube pos, translated stacks so, dont need trans++, +x+10 to y, +10 so that first resource cube is placed ontop cube
			glColor3d(0, 0, 0);											// Set to white colour
			glColor3d(0, 0.3, 0.5);										// Use Food colour
			DrawCubeNoBottom();											// Draw bottomless cube
		}
		// Draw current resource stores, represented as cubes above primary cube
		for(int x = 0; x < cubesBackPack.getMaterialStored(); x+= 100)
		{
			glTranslated(0, 10, 0);										// already trans atop cube pos, +x+10 to y, +10 so that first resource cube is placed ontop cube
			glColor3d(0, 0, 0);											// Set to white colour
			glColor3d(0.5, 0.5, 0.5);									// Use resource colour
			DrawCubeNoBottom();											// Draw bottomless cube
		}
	glPopMatrix();
}

bool CCube::smellFood(int foodLocX, int foodLocZ)						// Cube smells food>?
{
	if(Distance2f(disp.e1, disp.e2, foodLocX, foodLocZ) < CUBE_SMELL_SENSITIVITY)// if dist b.w food and cube, true
        return true;
    else
        return false;
}

bool CCube::materialInSight(int materialLocX, int materialLocZ) 		// Cube sees material?
{
	if(Distance2f(disp.e1, disp.e2, materialLocX, materialLocZ) < CUBE_SIGHT_RADIUS)// if dist b.w material and cube, true
        return true;
    else
        return false;
}

bool CCube::isMoveMagZero(void)
{
    if(moveMag <= 0)
        return true;
    else
        return false;
}

void CCube::moveDetPath(void)                                        // Move determined path, as in, cube randomly chose path
{
    if(isMoveMagZero())
        return;                                                     // if mag is zero, dont move along path any further
    else
        carryOutMove();                                              // Move along path
}

void CCube::pickNewPathDir(bool isRand, int x, int z)            	// Cube randomly decides direction for new path, or picks path toward pos
{
    if(isRand)                                                         // If isRand is true, ppicks random path, else picks path towards posH/V
    {
        // Random Path:
        // h
        switch(rand()%3 + 1)                                             // Pic dir of h component: Direction can be 0, 1, -1:
        {
            case 1:                             // dont move in x dir
                moveDir.e1 = 0;
            break;
            case 2:
                moveDir.e1 = -1;                // move in -x dir
            break;
            case 3:
                moveDir.e1 = 1;                 // move in +x dir
            break;
        }
        // v
        switch(rand()%3 + 1)                                             // Pic dir of v component: Direction can be 0, 1, -1:
        {
            case 1:                             // dont move in y dir
                moveDir.e2 = 0;
            break;
            case 2:
                moveDir.e2 = -1;                // move in -z dir
            break;
            case 3:
                moveDir.e2 = 1;                 // move in +z dir
            break;
        }
    }
    else
    {
        if(disp.e1 < x)                      // Set x dir
            moveDir.e1 = 1;                     // less than, so incremen towards
        else
            moveDir.e1 = -1;

        if(disp.e2 < z)                      // Set z dir
            moveDir.e2 = 1;                     // less than, so incremen towards
        else
            moveDir.e2 = -1;
    }
}

void CCube::carryOutMove(void)                                      // Cube moves along path; increments position vector
{
    disp.e1 += 10*(moveDir.e1);                                 	// move along x, 10 = 1grid length, moveDir = 0, 1, -1
    disp.e2 += 10*(moveDir.e2);                                     // move along y, ""
    moveMag -= 10;                                                  // Decrease movement magnitude, bcs just moved up the path
}

void CCube::setPathTowardFood(bool fromMemory = false)              // Cube sets path toward food
{
    if(fromMemory)
        pickNewPathDir(false, cubesBodyMind.foodMem.e1, cubesBodyMind.foodMem.e2);		       	// set path toward location of food in memory
    else
        pickNewPathDir(false, gameFoodLocX, gameFoodLocZ);			// dont use memory pos, not random, just set to global food pos val
}

void CCube::setPathTowardMaterial(bool fromMemory = false)          // Cube sets path toward food
{
    if(fromMemory)
        pickNewPathDir(false, cubesBodyMind.materialMem.e1, cubesBodyMind.materialMem.e2);		       	// set path toward location of food in memory
    else
        pickNewPathDir(false, gameMaterialLocX, gameMaterialLocZ);	// dont use memory pos, not random, just set to global food pos val
}

void CCube::setMoveMag(int mag)										// Set magnitude to move along path
{
    moveMag = mag;
}

void CCube::moveAnyRandDir(void)                                     // UNUSED,instead: pickNewPathDir TO GET RAND PATH Cube picks random dir, moves 1 grid
{
    switch(rand()%4 + 1)
    {
        case 1:                             // move in +x dir
            disp.e1 += 10;
        break;
        case 2:
            disp.e1 -= 10;                  // move in -x dir
        break;
        case 3:
            disp.e2 += 10;                  // move in +y dir
        break;
        case 4:
            disp.e2 -= 10;                  // move in -y dir
        break;
    }
}

void CCube::drawPath(int lengthToDraw)                               // Draws cubes chosen path up to lengthToDraw units, 10, 20, 30
{
    glColor3f(1.0, 0.0, 0.0);                                        // Make red colour

    glBegin(GL_LINES);
        glVertex3i(disp.e1, 0, disp.e2);                             // Cube's position
        glVertex3i(disp.e1 + moveDir.e1*lengthToDraw, 0, disp.e2 + moveDir.e2*lengthToDraw); // 2nd vert is moveDir + length
    glEnd();
}

bool CCube::desireSectorChange(void)								// Sets sector vals, so reduce compute times for certain ops
{
	// Record last sector, to compare against, to decide if need to change sector
	prevCubeSectorXZ.e1 = cubeSectorXZ.e1;		// X
	prevCubeSectorXZ.e2 = cubeSectorXZ.e2;		// Z

	// If X position greater than half of map size width/X, then set sector X coord to be +1
	disp.e1 > (GAME_MAP_SIZE_X/2) ? cubeSectorXZ.e1 = 1 : cubeSectorXZ.e1 = -1;

	// If Z position greater than half of map size height/Z, then set sector Z coord to be +1
	disp.e2 > (GAME_MAP_SIZE_Z/2) ? cubeSectorXZ.e2 = 1 : cubeSectorXZ.e2 = -1;

	// Compare old sector to new sector vals,
	// 		if changed, require reg into new sec list, and dereg from old sec list
	if((prevCubeSectorXZ.e1 != cubeSectorXZ.e1) || (prevCubeSectorXZ.e2 != cubeSectorXZ.e2))
		return true;
	else
		return false;

/*
// DEBUG DEBUGDEBUG DEBUGDEBUG DEBUGDEBUG DEBUGDEBUG DEBUG
// Successful sort into sectors
	switch(cubeSectorXZ.e1)
	{
		case 1:
			switch(cubeSectorXZ.e2)
				{
					case 1:
						cubesBodyMind.cubeColour[0] = 0;
						cubesBodyMind.cubeColour[1] = 0;
						cubesBodyMind.cubeColour[2] = 1;
					break;

					case -1:
						cubesBodyMind.cubeColour[0] = 1;
						cubesBodyMind.cubeColour[1] = 1;
						cubesBodyMind.cubeColour[2] = 0;
					break;
				}
		break;

		case -1:
			switch(cubeSectorXZ.e2)
				{
					case 1:
						cubesBodyMind.cubeColour[0] = 1;
						cubesBodyMind.cubeColour[1] = 0;
						cubesBodyMind.cubeColour[2] = 0;
					break;

					case -1:
						cubesBodyMind.cubeColour[0] = 0;
						cubesBodyMind.cubeColour[1] = 1;
						cubesBodyMind.cubeColour[2] = 0;
					break;
				}
		break;
	}
*/
}

//-------------------------------------------------------------------------------------
//------------------^CUBE CLASS^-------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

// Cube node.
class cubeNode
{
public:
	CCube cube;															// Tribe cube is affiliated with
	int affiliatedTribeID, tribeMemberNum, sectorID;					// Current tribe, tribe member number, current sector
	cubeNode *nextCubeInMasterList;                                     // master linkList contains all cubes
	cubeNode *nextCubeInSectorList, *prevCubeInSectorList;              // sectorlist contains all cubes in sector
	cubeNode *nextCubeInTribe, *prevCubeInTribe;                        // tribe list contains all cube in tribe

	cubeNode(void);
};

cubeNode::cubeNode(void)												// pt to nothing so can be sure when compare if pts to obj
{
	sectorID = 0;														// ??? this wont end badly right? dec compiler warning
	affiliatedTribeID = -1;												// Set tribe to master tribe
	tribeMemberNum = -1;
	nextCubeInMasterList = NULL;
	nextCubeInSectorList = NULL;
	prevCubeInSectorList = NULL;
	nextCubeInTribe = NULL;
	prevCubeInTribe = NULL;
}

class CTribeMessengerNode
{
public:
	cubeNode *tribeMessenger;											// Pt to cubeNode of messenger
    CTribeMessengerNode *nextTribeMessenger, *prevTribeMessenger;		// Next messenger in list

    CTribeMessengerNode(void);
};

CTribeMessengerNode::CTribeMessengerNode(void)
{
    tribeMessenger = NULL;									// Pt to cubeNode of messenger
    nextTribeMessenger = NULL;
    prevTribeMessenger = NULL;						// Next messenger in list
}

class CTribeFeederNode
{
public:
	cubeNode *tribeFeeder;										// Pt to cubeNode of Feeder
	CTribeFeederNode *nextTribeFeeder, *prevTribeFeeder;		// Next Feeder in list

    CTribeFeederNode(void);
};

CTribeFeederNode::CTribeFeederNode(void)
{
	tribeFeeder = NULL;									// Pt to cubeNode of Feeder
    nextTribeFeeder = NULL;
    prevTribeFeeder = NULL;								// Next Feeder in list
}

class CTribePredatorNode
{
public:
	cubeNode *tribePredator;									// Pt to cubeNode of Predator
	CTribePredatorNode *nextTribePredator, *prevTribePredator;	// Next Predator in list

	CTribePredatorNode(void);
};

CTribePredatorNode::CTribePredatorNode(void)
{
	tribePredator = NULL;									// Pt to cubeNode of Predator
	nextTribePredator = NULL;
	prevTribePredator = NULL;								// Next Predator in list
}

class cubeNodeBSTNode
{
	cubeNodeBSTNode *parentPos, *childNeg, *rightPos, *leftNeg;
	cubeNode *containedCubeNode;
	cubeNodeBSTNode(void);
};

cubeNodeBSTNode::cubeNodeBSTNode(void)
{
	parentPos = NULL;
	childNeg = NULL;
	rightPos = NULL;
	leftNeg = NULL;
	containedCubeNode = NULL;
}

//-------------------------------------------------------------------------------------
//------------------CUBE TRIBE CLASS---------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
class CCubeTribe
{
public:
    cubeNode *tribeHead;                                                // Head of tribe list
    CTribeMessengerNode *tribeMessengerHead;							// Head of messenger list
    CTribeFeederNode *tribeFeederHead;									// Head of feeder list
    CTribePredatorNode *tribePredatorHead;								// Head of predator list

    //    cubeNode *killQueue[5];											// The last # cubes which are not productive (returned the least resoureces), marked for consumption
                                                                        //      gives tribe a way to remove cubes which are reducing success for tribe, these cubes are far from food
    int collectiveFood, collectiveMaterial;                            // Resources gathered from all member cubes
    int tribeVacancy, tribeID, tribeINFOExhangeDemand;
    GLfloat tribeColour[3];
    Vec2d tribeFoodPosConsensus, tribeMaterialPosConsensus, tribeBirtherPos, tribeSectorXZ;

    CCubeTribe(void);
    ~CCubeTribe(void);
    bool withinTerritory(int posX, int posZ);          				    // Check if given coords is within territory
    bool knowFoodLocation(void);										// Does tribe know food loc? checks if != -1
    bool knowMaterialLocation(void);									// Does tribe know material loc? checks if != -1
    void consensusFoodPosUpdate(int foodPosX, int foodPosZ);			// Average passed vals with stored val, get closer to actual val
    void consensusMaterialPosUpdate(int materialPosX, int materialPosZ);// "
    int foodDue(cubeNode *cubeNodeToEvaluate, int foodRemaining );		// Return cube total food - remaining
    void addToCollectiveFood(int howMuchFood);							// Adds HowMuch to collectiveFood
    void addToCollectiveMaterial(int howMuchMaterial);
    int giveFood(int howMuchFood);										// Removes food from collective food, returns given food
    bool desireRecruits(void);                                          // Does tribe want recruits
    void recruitCube(cubeNode *cubeNodeToRecruit);						// Adds cubeNode to tribe
    void registerTribeMessenger(cubeNode *tribeMessengerToReg);			// Registers a tribe messenger
    void deregisterMessenger(cubeNode *tribeMessengerToDeReg);
    void registerTribeFeeder(cubeNode *tribeFeederToReg);				// Reg a tribe feeder
    void deregisterTribeFeeder(cubeNode *tribeFeederToDeReg);
    void registerTribePredator(cubeNode *tribePredatorToReg);
    void deregisterTribePredator(cubeNode *tribePredatorToDeReg);
    //   void collectFoodDues(cubeNode *cubeToCollectFrom);                  // Collects food contribution from cube
};

CCubeTribe::CCubeTribe(void)
{
    tribeHead = NULL;                                                   // Head of tribe not assigned on start
    tribeMessengerHead = NULL;
    tribeFeederHead = NULL;
    tribePredatorHead = NULL;

    collectiveFood = 0;
    collectiveMaterial = 0;
    tribeVacancy = TRIBE_MAXMEMBERS;
    tribeID = 0;
    tribeINFOExhangeDemand = CUBE_STAMINA*5;
    tribeFoodPosConsensus.e1 = -1;										// -1 instead of -998 because -1 closer to actual val
    tribeFoodPosConsensus.e2 = -1;
    tribeMaterialPosConsensus.e1 = -1;
    tribeMaterialPosConsensus.e2 = -1;
    tribeBirtherPos.e1 = 0;
    tribeBirtherPos.e2 = 0;
}

CCubeTribe::~CCubeTribe(void)	// CHECK, UNFIN , ADD FEEDERS, CHECK FEEDERS ELSEWHERE
{
	CTribeMessengerNode *tempMessengerNode = NULL;
	CTribeFeederNode	*tempFeederNode = NULL;
	CTribePredatorNode	*tempPredatorNode = NULL;

	// Delete tribe cube node classes; this is safe bcs the classes only contain ptrs to cube class, etc
	// Delete Messengers:
	for(tempMessengerNode = tribeMessengerHead; tempMessengerNode != NULL; tempMessengerNode = tribeMessengerHead)
	{
		tribeMessengerHead = tribeMessengerHead->nextTribeMessenger;
		if(tempMessengerNode != NULL)
			delete tempMessengerNode;
	}

	// Delete Feeders:
	for(tempFeederNode = tribeFeederHead; tempFeederNode != NULL; tempFeederNode = tribeFeederHead)
	{
		tribeFeederHead = tribeFeederHead->nextTribeFeeder;
		if(tempFeederNode != NULL)
			delete tempFeederNode;
	}

	// Delete Predators:
	for(tempPredatorNode = tribePredatorHead; tempPredatorNode != NULL; tempPredatorNode = tribePredatorHead)
	{
		tribePredatorHead = tribePredatorHead->nextTribePredator;
		if(tempPredatorNode != NULL)
			delete tempPredatorNode;
	}
}

bool CCubeTribe::withinTerritory(int posX, int posZ)
{
    if(Distance2f(posX, posZ, tribeBirtherPos.e1, tribeBirtherPos.e2) < TRIBE_TERRITORY_RADIUS)
        return true;
    else
        return false;
}

bool CCubeTribe::knowFoodLocation(void)      					    // Gotta be a better way to do this....
{
	if(tribeFoodPosConsensus.e1 == -1)
		return false;
    else
		return true;
}

bool CCubeTribe::knowMaterialLocation(void)      					    // Gotta be a better way to do this....
{
	if(tribeMaterialPosConsensus.e1 == -1)
		return false;
    else
		return true;
}

void CCubeTribe::consensusFoodPosUpdate(int foodPosX, int foodPosZ)  // Move foodPosConsensus closer to actual value
{
	// Yea thats bogus:
    if(foodPosX < 0 || foodPosZ < 0)
    	return;

	tribeFoodPosConsensus.e1 += foodPosX;
    tribeFoodPosConsensus.e2 += foodPosZ;

    tribeFoodPosConsensus.e1 /= 2;
    tribeFoodPosConsensus.e2 /= 2;

    // Cannot possibly have neg vals:
    if(tribeFoodPosConsensus.e1 < 0 || tribeFoodPosConsensus.e2 < 0)
    {
    	tribeFoodPosConsensus.e1 = 0;
    	tribeFoodPosConsensus.e2 = 0;
    }
}

void CCubeTribe::consensusMaterialPosUpdate(int materialPosX, int materialPosZ)  // Move tribeMaterialPosConsensus closer to actual value
{
	// Yea thats bogus:
    if(materialPosX < 0 || materialPosZ < 0)
    	return;

	tribeMaterialPosConsensus.e1 += materialPosX;
	tribeMaterialPosConsensus.e2 += materialPosZ;

	tribeMaterialPosConsensus.e1 /= 2;
	tribeMaterialPosConsensus.e2 /= 2;

    // Cannot possibly have neg vals:
    if(tribeMaterialPosConsensus.e1 < 0 || tribeMaterialPosConsensus.e2 < 0)
    {
    	tribeMaterialPosConsensus.e1 = 0;
    	tribeMaterialPosConsensus.e2 = 0;
    }
}

int CCubeTribe::foodDue(cubeNode *cubeNodeToEvaluate, int foodRemaining)  // Return food to take from cube while leaving enough for cube's survival
{
	if((cubeNodeToEvaluate->cube.cubesBackPack.getFoodStored()-foodRemaining) > 0)		// dont want negative food
		return cubeNodeToEvaluate->cube.cubesBackPack.getFoodStored()-foodRemaining;
	else
		return 0;
}

void CCubeTribe::addToCollectiveFood(int howMuchFood)
{
	// impose food limit for tribes?
	collectiveFood += howMuchFood;
}

void CCubeTribe::addToCollectiveMaterial(int howMuchMaterial)
{
	// impose food limit for tribes?
	collectiveMaterial += howMuchMaterial;
}

int CCubeTribe::giveFood(int howMuchFood)
{
    if(howMuchFood > collectiveFood)
    {
    /*  while(1)                                        // too intensive, use % operator to tell
        {
            howMuchFood--;
            if(howMuchFood <= foodStored)
                break;
        }
    */
        howMuchFood = 0;
    }

    collectiveFood -= howMuchFood;
    return howMuchFood;
}

bool CCubeTribe::desireRecruits(void)
{
	if(collectiveFood < 10000 || tribeVacancy != 0)	// Tribe in comfortable food situation?
		return true;
	else
		return false;
}

void CCubeTribe::recruitCube(cubeNode *cubeNodeToRecruit)
{
	// Check if node is NULL(safe from crash), have room for members, cube not part of another tribe ie must be master tribe
	if(cubeNodeToRecruit != NULL)
	{
        if(tribeVacancy > 0 && cubeNodeToRecruit->affiliatedTribeID == -1)
        {
            if(tribeHead == NULL)								// First cube
            	tribeHead = cubeNodeToRecruit;
            else
            {
            	tribeHead->prevCubeInTribe = cubeNodeToRecruit; // link head node prev to nodeToreg
                cubeNodeToRecruit->nextCubeInTribe = tribeHead; // Link nodeToreg nex to head node
                cubeNodeToRecruit->prevCubeInTribe = NULL; 		// Redundant, should already be NULL
                tribeHead = cubeNodeToRecruit; 					// set head to just linked nodeToreg
            }

            // Give tribe ID to cube
            cubeNodeToRecruit->affiliatedTribeID = tribeID;
            cubeNodeToRecruit->tribeMemberNum = tribeVacancy;
            // Give cube tribe's colour:
            cubeNodeToRecruit->cube.cubesBodyMind.tribeColour[0] = tribeColour[0];
            cubeNodeToRecruit->cube.cubesBodyMind.tribeColour[1] = tribeColour[1];
            cubeNodeToRecruit->cube.cubesBodyMind.tribeColour[2] = tribeColour[2];
            // Give cube tribes pos:
            cubeNodeToRecruit->cube.cubesBodyMind.infoOnTribe.tribePos.e1 = tribeBirtherPos.e1;
            cubeNodeToRecruit->cube.cubesBodyMind.infoOnTribe.tribePos.e2 = tribeBirtherPos.e2;


            if((tribeVacancy%8) == 0)	//re spec messenger:
            {
            	// Set cube type:
            	cubeNodeToRecruit->cube.cubeType = MESSENGER;
            	// Adjust cube modifiers to match type:
            	// Increase length of steps cube takes on a path(travels further):
            	cubeNodeToRecruit->cube.cubesBodyMind.cubeMods.modifier_cubeStamina += 10;
            	// Increase cube activity (how often cubes move/cycle) lower, more active:
            	cubeNodeToRecruit->cube.cubesBodyMind.cubeMods.modifier_cubeActivity = -20;
            	// Give messenger tribes current foodLoc consensus:
            	cubeNodeToRecruit->cube.cubesBodyMind.foodMem.e1 = tribeFoodPosConsensus.e1;
            	cubeNodeToRecruit->cube.cubesBodyMind.foodMem.e1 = tribeFoodPosConsensus.e2;

            	// OTHER MESSENGER MODS

            	// Register messenger in tribes messenger list:
            	registerTribeMessenger(cubeNodeToRecruit);
            }
            else if((tribeVacancy%7) == 0)	//re spec feeder
            {
            	// Set cube type:
            	cubeNodeToRecruit->cube.cubeType = FEEDER;
            	// Adjust cube modifiers to match type:
            	// Increase length of steps cube takes on a path(travels further):
            	cubeNodeToRecruit->cube.cubesBodyMind.cubeMods.modifier_cubeStamina += 10;
            	// Increase cube activity (how often cubes move/cycle) lower, more active:
            	cubeNodeToRecruit->cube.cubesBodyMind.cubeMods.modifier_cubeActivity = -20;
            	// Increase size of feeder's pack
            	cubeNodeToRecruit->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize = CUBETYPE_FEEDER_PACKSIZE;
            	// Give feeder tribe's current foodLoc consensus:
            	cubeNodeToRecruit->cube.cubesBodyMind.foodMem.e1 = tribeFoodPosConsensus.e1;
            	cubeNodeToRecruit->cube.cubesBodyMind.foodMem.e1 = tribeFoodPosConsensus.e2;

            	// OTHER FEEDER MODS

            	// Register messenger in tribes feeder list:
            	registerTribeFeeder(cubeNodeToRecruit);
            }
            else if((tribeVacancy%12) == 0)	//re spec predator
            {
            	// Set cube type:
            	cubeNodeToRecruit->cube.cubeType = PREDATOR;
            	// Adjust cube modifiers to match type:
            	// Increase length of steps cube takes on a path(travels further):
            	cubeNodeToRecruit->cube.cubesBodyMind.cubeMods.modifier_cubeStamina += 10;
            	// Increase size of predator's pack
            	cubeNodeToRecruit->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize = CUBETYPE_PREDATOR_PACKSIZE;
            	// Give feeder tribe's current foodLoc consensus:
            	cubeNodeToRecruit->cube.cubesBodyMind.foodMem.e1 = tribeFoodPosConsensus.e1;
            	cubeNodeToRecruit->cube.cubesBodyMind.foodMem.e1 = tribeFoodPosConsensus.e2;

            	// implement life mod

            	// do it this way:
            	//cubeNodeToRecruit->cube.cubesBodyMind.cubeMods = FORAGER_TYPE;

            	// OTHER PREDATOR MODS

            	// Register messenger in tribes predator list:
            	registerTribePredator(cubeNodeToRecruit);
            }
            // other methods to tell when need to respec a recruited cube

            // Reduce tribe vacancy:
            tribeVacancy--;
// DEBUG DEBUG
//           cout << INS << "Tribe: " << tribeID << " Has recruited a cube.";
        }
	}
	else
	{
		cout << EXC << "Reg Func of recruitCube: just passed a NULL tempNode? what the heck..";
		cin.get();
		return;
	}
}

void CCubeTribe::registerTribeMessenger(cubeNode *tribeMessengerToReg)
{
	CTribeMessengerNode *messengerNodeToReg = new CTribeMessengerNode;	// New messenger node
	messengerNodeToReg->tribeMessenger = tribeMessengerToReg;			// Add cube node to messenger node

	// Add messenger to tribe's messenger BST: less searching
	// func search thru list, find x & z: x up/down, z left/right

	// Add messenger to tribe's messenger list:
    if(tribeMessengerHead == NULL)								// First cube set to head
    	tribeMessengerHead = messengerNodeToReg;
    else
    {
    	tribeMessengerHead->prevTribeMessenger = messengerNodeToReg; // link head node prev to nodeToreg
    	messengerNodeToReg->nextTribeMessenger = tribeMessengerHead; // Link nodeToreg nex to head node
    	messengerNodeToReg->prevTribeMessenger = NULL; 				// Redundant, should already be NULL
    	tribeMessengerHead = messengerNodeToReg; 					// set head to just linked nodeToreg
    }
}

void CCubeTribe::registerTribeFeeder(cubeNode *tribeFeederToReg)
{
	CTribeFeederNode *feederNodeToReg = new CTribeFeederNode;	// New feeder node
	feederNodeToReg->tribeFeeder = tribeFeederToReg;			// Add cube node to feeder node

	// Add feeder to tribe's feeder BST: less searching
	// func search thru list, find x & z: x up/down, z left/right

	// Add messenger to tribe's feeder list:
    if(tribeFeederHead == NULL)								// First cube set to head
    	tribeFeederHead = feederNodeToReg;
    else
    {
    	tribeFeederHead->prevTribeFeeder = feederNodeToReg; // link head node prev to nodeToreg
    	feederNodeToReg->nextTribeFeeder = tribeFeederHead; // Link nodeToreg nex to head node
    	feederNodeToReg->prevTribeFeeder = NULL; 			// Redundant, should already be NULL
    	tribeFeederHead = feederNodeToReg; 					// set head to just linked nodeToreg
    }
}

void CCubeTribe::registerTribePredator(cubeNode *tribePredatorToReg)
{
	CTribePredatorNode *predatorNodeToReg = new CTribePredatorNode;	// New Predator node
	predatorNodeToReg->tribePredator = tribePredatorToReg;				// Add cube node to Predator node

	// Add Predator to tribe's feeder BST: less searching
	// func search thru list, find x & z: x up/down, z left/right

	// Add messenger to tribe's feeder list:
    if(tribePredatorHead == NULL)							// First cube set to head
    	tribePredatorHead = predatorNodeToReg;
    else
    {
    	tribePredatorHead->prevTribePredator = predatorNodeToReg; // link head node prev to nodeToreg
    	predatorNodeToReg->nextTribePredator = tribePredatorHead; // Link nodeToreg nex to head node
    	predatorNodeToReg->prevTribePredator = NULL; 			// Redundant, should already be NULL
    	tribePredatorHead = predatorNodeToReg; 					// set head to just linked nodeToreg
    }
}


//-------------------------------------------------------------------------------------
//------------------CUBE TRIBE CLASS^^^^^^^^^^^^^--------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//------------------CUBE GOD CLASS-----------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

class CCubeGod															// Creates and destroys cubes, births/kills, sends predators, env effects?
{
public:
	int numCubes;
	cubeNode *headofMasterList, *tempCubeNode;							// Points to Head of Master Cube list, temp traverses this list
	CCubeTribe globalTribes[TRIBE_NUMTRIBES];                            // Array of tribes
	cubeNode *sectorHead[4];											// Only four sectors for now, ofcourse smaller sectors better(?)
																		// 		Point to the heads of each sector

	CCubeGod(void);
	~CCubeGod(void);
	void createCubes(int);
	void deleteCubes(void);
	void initCubes(void);                                               // Fills out sector lists, if dont run this once, then empty sector lists
	void cubeLifeCycleandRender(void);
	cubeNode *returnSectorHead(int X, int Z);							// Returns address of sector head from sector coords
	void deregisterCube(cubeNode *cubeNodeToDeregister);
	void registerCube(cubeNode *cubeNodeToRegister);
	void tribeInit(void);
	void tribeRenderArtifacts(void);
	void tribeLifeCycle(void);
};

CCubeGod::CCubeGod(void)
{
	headofMasterList = NULL;
	tempCubeNode = NULL;

	sectorHead[0] = NULL;
	sectorHead[1] = NULL;
	sectorHead[2] = NULL;
	sectorHead[3] = NULL;

	numCubes = 0;														// ??? declare =0 should be ok...
}

CCubeGod::~CCubeGod(void)
{
	cubeNode *node = NULL;

	if(headofMasterList != NULL)
	{
		// Delete nodes:
		for(node = headofMasterList; node != NULL; node = headofMasterList)
		{
			headofMasterList = node->nextCubeInMasterList;
			if(node != NULL)
				delete node;
		}
	}
}

void CCubeGod::createCubes(int numCubes)	// Can probably optimize this..., like have to find tail of list evry time? rly?
{
//	int tribeCounter = 0;
	cubeNode *node = NULL;

	// Assign head:
	if(headofMasterList == NULL)
	{
		node = new cubeNode;

		// Next is nothing
		node->nextCubeInMasterList = NULL;
		headofMasterList = node;
	}
	node = NULL;

	// Add more cubes
	for(int x = 1; x < numCubes; x++)	// x = 1 bcause already assigned head to 1,
	{
		node = new cubeNode;

		// Head exists, must find last cube:
		for(tempCubeNode = headofMasterList; tempCubeNode->nextCubeInMasterList != NULL; tempCubeNode = tempCubeNode->nextCubeInMasterList);		// Search every time, inefficient

		tempCubeNode->nextCubeInMasterList = node;
		node->nextCubeInMasterList = NULL;
	}


	// lol ok so this thing was assignning tribe head to a cube right, but then when recruit cubes to tribe, this one eventually gets recruited right?
	//		this will be his second time being recruited, so what ends up happenning is you recruit a cube, next it to first cube in tribe
	// 		then recruit first cube in tribe again, then next cube of first cube is 2nd cube of tribe, COMPLETE CIRCLE!!
	// so then ask, why is this here in the first place..., bcs recruiting ccubes np based on territory, remove this, head gets setup on first
	// 		cube recruit
	// Run through master cube list and assign tribe heads as run along.
   /*
	for(tempCubeNode = headofMasterList; tempCubeNode->nextCubeInMasterList != NULL; tempCubeNode = tempCubeNode->nextCubeInMasterList)
    {
    	globalTribes[tribeCounter].tribeHead = tempCubeNode;
    	tribeCounter++;
    	if(tribeCounter >= TRIBE_NUMTRIBES)
    		break;
    }
*/
}

void CCubeGod::initCubes(void)   // MasterList created, now assign cubes to a sector, fill out sector lists
{
    for(tempCubeNode = headofMasterList; tempCubeNode != NULL; tempCubeNode = tempCubeNode->nextCubeInMasterList)
	{
        if(tempCubeNode->cube.desireSectorChange())
		{
 			deregisterCube(tempCubeNode);
			registerCube(tempCubeNode);
		}
	}
}

void CCubeGod::deleteCubes(void)
{
	cubeNode *node;

	// Delete nodes:
	for(node = headofMasterList; node != NULL; node = headofMasterList)
	{
		headofMasterList = node->nextCubeInMasterList;
		if(node != NULL)
			delete node;
	}
}

cubeNode *CCubeGod::returnSectorHead(int X, int Z)			// Could be dangerous? because vals x,z maybe 0,0 or NAN, or unzero.. unpredictable..?
{
	cubeNode *returnedSecHead = NULL;
	switch(X)	// Which sector head is this cube?:
	{
		case 1:
			switch(Z)
				{
					case 1:// First sector head
						returnedSecHead = sectorHead[0];
					break;
					case -1:
						returnedSecHead = sectorHead[1];
					break;
				}
		break;
		case -1:
			switch(Z)
				{
					case 1:
						returnedSecHead = sectorHead[2];
					break;
					case -1:
						returnedSecHead = sectorHead[3];
					break;
				}
		break;
	}

	return returnedSecHead;
}

void CCubeGod::deregisterCube(cubeNode *cubeNodeToDeregister)
{

	/* Handle:
				 * 	cube not registered in any sector list
				 * 	cube registered last in sector list
				 * 	cube registered first in sector list
				 */
	// The loop containing dereg should never pass a NULL tempNode,
	//		so... maybe deleted and didnt remove from master list?, null node should never happen
	if(cubeNodeToDeregister != NULL)
	{
		if(cubeNodeToDeregister->nextCubeInSectorList != NULL)	// Stay safe
		{
			// Assign next in Sec List to previous in SecList
			cubeNodeToDeregister->nextCubeInSectorList->prevCubeInSectorList = cubeNodeToDeregister->prevCubeInSectorList;

			//  IF DEREG HERE, CANT ASSIN HEAD(IFCURR CUBE IS HEAD) TO NEXT CUBE IN LIST
			// Dereg
		//	cubeNodeToDeregister->nextCubeInSectorList = NULL;
		}

		if(cubeNodeToDeregister->prevCubeInSectorList != NULL)	// Stay safe
		{
			// Assign prev in Sec List to next in SecList
			cubeNodeToDeregister->prevCubeInSectorList->nextCubeInSectorList = cubeNodeToDeregister->nextCubeInSectorList;
			// Dereg
			cubeNodeToDeregister->prevCubeInSectorList = NULL;
		}
		else // Found sector head, dereg it, assign it to next in Sec list(if exists):
		{
			switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e1)	// Which sector head is this cube?:
			{
				case 1:
					switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e2)
						{
							case 1:// First sector head
								if(cubeNodeToDeregister == sectorHead[0])
								{
									sectorHead[0] = cubeNodeToDeregister->nextCubeInSectorList; // Assign head to next in list, perfectly OK if that
																								//		is NULL(then there was only that node in this sector)
								}
									break;
							case -1:
								if(cubeNodeToDeregister == sectorHead[1])
									sectorHead[1] = cubeNodeToDeregister->nextCubeInSectorList;
							break;
						}
				break;
				case -1:
					switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e2)
						{
							case 1:
								if(cubeNodeToDeregister == sectorHead[2])
									sectorHead[2] = cubeNodeToDeregister->nextCubeInSectorList;
							break;
							case -1:
								if(cubeNodeToDeregister == sectorHead[3])
									sectorHead[3] = cubeNodeToDeregister->nextCubeInSectorList;
							break;
						}
				break;
			}
		}
	}
	else
	{
		cout << EXC << "Dereg Func of CubeGod: just passed a NULL tempNode? what the heck..";
		cin.get();
		return;
	}

	// To be sure cube is totally decoupled:
	cubeNodeToDeregister->nextCubeInSectorList = NULL;
	cubeNodeToDeregister->prevCubeInSectorList = NULL;
	/* So didnt have this, what ended up happening was since
	 * if Headsector == NULL, then Head = cubeNodetoAdd; So if next never reset to NULL at DEREG,
	 * then the last cubeNode in sector list will point next to old link = Infinite loop.
	 */
}

void CCubeGod::registerCube(cubeNode *cubeNodeToRegister)
{
	/* Handle:
				 * 	cube not registered in any sector list
				 * 	cube registered last in sector list
				 * 	cube registered first in sector list
				 */
	if(cubeNodeToRegister != NULL)
		{
		switch(cubeNodeToRegister->cube.cubeSectorXZ.e1)	// Which sector head is this cube?:
			{
				case 1:
					switch(cubeNodeToRegister->cube.cubeSectorXZ.e2)
						{
							case 1:// First sector head
								if(sectorHead[0] == NULL)
									sectorHead[0] = cubeNodeToRegister;// Assign head to cubeNodeToDeregister
								else
								{
									sectorHead[0]->prevCubeInSectorList = cubeNodeToRegister; // link head node prev to nodeToreg
									cubeNodeToRegister->nextCubeInSectorList = sectorHead[0]; // Link nodeToreg nex to head node
									cubeNodeToRegister->prevCubeInSectorList = NULL; // Redundant, should already be set NULL after dereg
									sectorHead[0] = cubeNodeToRegister; // set head to just linked nodeToreg
/*
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[0] = 0;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[1] = 0;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[2] = 1;
									*/
								}
							break;
							case -1:
								if(sectorHead[1] == NULL)
									sectorHead[1] = cubeNodeToRegister;
								else
								{
									sectorHead[1]->prevCubeInSectorList = cubeNodeToRegister; // link head node prev to nodeToreg
									cubeNodeToRegister->nextCubeInSectorList = sectorHead[1]; // Link nodeToreg nex to head node
									cubeNodeToRegister->prevCubeInSectorList = NULL; // Redundant, should already be set NULL after dereg
									sectorHead[1] = cubeNodeToRegister; // set head to just linked nodeToreg
/*
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[0] = 1;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[1] = 1;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[2] = 0;
*/
								}
							break;
						}
				break;
				case -1:
					switch(cubeNodeToRegister->cube.cubeSectorXZ.e2)
						{
							case 1:
								if(sectorHead[2] == NULL)
									sectorHead[2] = cubeNodeToRegister;
								else
								{
									sectorHead[2]->prevCubeInSectorList = cubeNodeToRegister; // link head node prev to nodeToreg
									cubeNodeToRegister->nextCubeInSectorList = sectorHead[2]; // Link nodeToreg nex to head node
									cubeNodeToRegister->prevCubeInSectorList = NULL; // Redundant, should already be set NULL after dereg
									sectorHead[2] = cubeNodeToRegister; // set head to just linked nodeToreg
/*
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[0] = 1;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[1] = 0;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[2] = 0;
									*/
								}
							break;
							case -1:
								if(sectorHead[3] == NULL)
									sectorHead[3] = cubeNodeToRegister;
								else
								{
									sectorHead[3]->prevCubeInSectorList = cubeNodeToRegister; // link head node prev to nodeToreg
									cubeNodeToRegister->nextCubeInSectorList = sectorHead[3]; // Link nodeToreg nex to head node
									cubeNodeToRegister->prevCubeInSectorList = NULL; // Redundant, should already be set NULL after dereg
									sectorHead[3] = cubeNodeToRegister; // set head to just linked nodeToreg
/*
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[0] = 0;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[1] = 1;
									cubeNodeToRegister->cube.cubesBodyMind.cubeColour[2] = 0;
									*/
								}
							break;
						}
				break;
			}
		}
		else
		{
			cout << EXC << "Reg Func of CubeGod: just passed a NULL tempNode? what the heck..";
			cin.get();
			return;
		}
}

void CCubeGod::cubeLifeCycleandRender(void)										// Draw, and run life cycle for all cubes
{
/*
    // Must run first, allows sectorHeads to be assigned, maybe can
    // Run through all cubes, next node assigned at bottom of forloop, so a NULL tempNode should never be passed:
	for(tempCubeNode = headofMasterList; tempCubeNode != NULL; tempCubeNode = tempCubeNode->nextCubeInMasterList)
	{
		// Run a cycle of a cubes life:
		tempCubeNode->cube.cubeLifeCycle();										// member cube, of object pointed to by temp
		// Draw the cube:
		tempCubeNode->cube.renderSelf();

		// Check if changed sector:
		if(tempCubeNode->cube.desireSectorChange())
		{
 			deregisterCube(tempCubeNode);
			registerCube(tempCubeNode);
		}
	}
*/

	// Run through sectors:
	for(int x = 0; x <= 3; x++)
	{
		for(tempCubeNode = sectorHead[x]; tempCubeNode != NULL; tempCubeNode = tempCubeNode->nextCubeInSectorList)
		{
			// Run a cycle of a cubes life:
			tempCubeNode->cube.cubeLifeCycle();									// member cube, of object pointed to by temp
			// Draw the cube: get flickering, so draw every cube at once
			tempCubeNode->cube.renderSelf();

            // Algorithm for tribes to recruit members
			if(tempCubeNode->affiliatedTribeID == -1)							// Cube not a member of a tribe already
				for(int x = 0; x < TRIBE_NUMTRIBES; x++)
					if(globalTribes[x].tribeVacancy > 0)						// Tribe gas room for members
						if(globalTribes[x].tribeSectorXZ.e1 == tempCubeNode->cube.cubeSectorXZ.e1 && globalTribes[x].tribeSectorXZ.e2 == tempCubeNode->cube.cubeSectorXZ.e2)
							if(globalTribes[x].desireRecruits())                        // Does tribe want members
								if(globalTribes[x].withinTerritory(tempCubeNode->cube.disp.e1, tempCubeNode->cube.disp.e2))// Is cube within territory
									globalTribes[x].recruitCube(tempCubeNode);          // Recruit cube to tribe


            // Check if changed sector: results in tempCubeNode assigned to null, so..
			if(tempCubeNode->cube.desireSectorChange())
			{
	 			deregisterCube(tempCubeNode);
				registerCube(tempCubeNode);
			}
/*
			switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e1)	// Which sector head is this cube?:
			{
				case 1:
					switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e2)
						{
							case 1:// First sector head
								if(cubeNodeToDeregister == sectorHead[0])
								{
									sectorHead[0] = cubeNodeToDeregister->nextCubeInSectorList; // Assign head to next in list, perfectly OK if that
																								//		is NULL(then there was only that node in this sector)
								}
									break;
							case -1:
								if(cubeNodeToDeregister == sectorHead[1])
									sectorHead[1] = cubeNodeToDeregister->nextCubeInSectorList;
							break;
						}
				break;
				case -1:
					switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e2)
						{
							case 1:
								if(cubeNodeToDeregister == sectorHead[2])
									sectorHead[2] = cubeNodeToDeregister->nextCubeInSectorList;
							break;
							case -1:
								if(cubeNodeToDeregister == sectorHead[3])
									sectorHead[3] = cubeNodeToDeregister->nextCubeInSectorList;
							break;
						}
				break;
			}*/
		}
	}
}

void CCubeGod::tribeInit(void)
{
	for(int x = 0; x < TRIBE_NUMTRIBES; x++)
	{
		// Set birther pos:
		globalTribes[x].tribeBirtherPos.e1 = rand()%GAME_MAP_SIZE_X + 1;
		globalTribes[x].tribeBirtherPos.e2 = rand()%GAME_MAP_SIZE_Z + 1;

		// Set tribe colour:
		globalTribes[x].tribeColour[0] = (rand()%10)/10.0;		// must be 10.0 or else calculation returns an int, which means 0(white)
		globalTribes[x].tribeColour[1] = (rand()%10)/10.0;
		globalTribes[x].tribeColour[2] = (rand()%10)/10.0;

		// Set tribe sector from birther pos:
		// If X position greater than half of map size width/X, then set sector X coord to be +1
		globalTribes[x].tribeBirtherPos.e1 > (GAME_MAP_SIZE_X/2) ? globalTribes[x].tribeSectorXZ.e1 = 1 : globalTribes[x].tribeSectorXZ.e1 = -1;
		// If Z position greater than half of map size height/Z, then set sector Z coord to be +1
		globalTribes[x].tribeBirtherPos.e2 > (GAME_MAP_SIZE_Z/2) ? globalTribes[x].tribeSectorXZ.e2 = 1 : globalTribes[x].tribeSectorXZ.e2 = -1;

		// Assign an ID to tribe, for identification purposes.
		globalTribes[x].tribeID = x;

/*      // Set cubes sector index,so can when sector search tribe sector index corresponds, then distance test to see if recruit range
        switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e1)	// Which sector head is this cube?:
        {
            case 1:
                switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e2)
                    {
                        case 1:// First sector head
                            if(cubeNodeToDeregister == sectorHead[0])
                            {
                                sectorHead[0] = cubeNodeToDeregister->nextCubeInSectorList; // Assign head to next in list, perfectly OK if that
                                                                                            //		is NULL(then there was only that node in this sector)
                            }
                                break;
                        case -1:
                            if(cubeNodeToDeregister == sectorHead[1])
                                sectorHead[1] = cubeNodeToDeregister->nextCubeInSectorList;
                        break;
                    }
            break;
            case -1:
                switch(cubeNodeToDeregister->cube.prevCubeSectorXZ.e2)
                    {
                        case 1:
                            if(cubeNodeToDeregister == sectorHead[2])
                                sectorHead[2] = cubeNodeToDeregister->nextCubeInSectorList;
                        break;
                        case -1:
                            if(cubeNodeToDeregister == sectorHead[3])
                                sectorHead[3] = cubeNodeToDeregister->nextCubeInSectorList;
                        break;
                    }
            break;
        }*/
	}
}

void CCubeGod::tribeRenderArtifacts(void)
{
	for(int x = 0; x < TRIBE_NUMTRIBES; x++)
	{
		// Draw birther: change this from torus to maybe a cross shape, but has 4cube head, rotates along y?
	    glColor3f(0, 0, 0);
	    glColor3fv(globalTribes[x].tribeColour);
	    glPushMatrix();
	        glTranslatef(globalTribes[x].tribeBirtherPos.e1, 60.0, globalTribes[x].tribeBirtherPos.e2);
	        glScalef(50,50,50);
			glRotatef(90, 1, 0, 0);						// rotate into xz plane
			glScalef(1.0, 1.0, 0.2);                    // make the profile in z axis less
	        glutSolidTorus(0.1,1.2,10,10);
		glPopMatrix();

		// Draw territory radius:
	    glColor3f(0, 0, 0);
	    glColor3fv(globalTribes[x].tribeColour);
	    glPushMatrix();
	        glTranslatef(globalTribes[x].tribeBirtherPos.e1, 60.0, globalTribes[x].tribeBirtherPos.e2);
	        glRotatef(90, 1, 0, 0);						// rotate into xz plane
	        glutSolidTorus(1,TRIBE_TERRITORY_RADIUS+1,10,10);
	        glRotatef(-90, 1, 0, 0);						// rotate back into xy plane

	    	// Draw current food stores, represented as cubes
			for(int z = 0; z < globalTribes[x].collectiveFood; z+= 100)		// 100 food/cube
			{
				glTranslated(0, 10, 0);										// already trans to birther pos, translated stacks so, dont need trans++, +x+10 to y, +10 so that first resource cube is placed ontop cube
				glColor3d(0, 0, 0);											// Set to white colour
				glColor3d(0, 0.3, 0.5);										// Use Food colour
				DrawCubeNoBottom();											// Draw bottomless cube
			}

			// Move materials stack +10x:
			glTranslated(10, 0, 0);
			// Draw current resource stores, represented as cubes
			for(int z = 0; z < globalTribes[x].collectiveMaterial; z+= 100)
			{
				glTranslated(0, 10, 0);										// already trans to birther pos, +x+10 to y, +10 so that first resource cube is placed ontop cube
				glColor3d(0, 0, 0);											// Set to white colour
				glColor3d(0.5, 0.5, 0.5);									// Use resource colour
				DrawCubeNoBottom();											// Draw bottomless cube
			}
		glPopMatrix();

        // Draw food consensus pos:
	    glColor3f(0, 0, 0);
	    glColor3fv(globalTribes[x].tribeColour);
	    glPushMatrix();
	        glTranslatef(globalTribes[x].tribeFoodPosConsensus.e1, 60.0, globalTribes[x].tribeFoodPosConsensus.e2);
            glScaled(50,50,50);
            glutWireSphere(1,10,10);
		glPopMatrix();
	}
}

void CCubeGod::tribeLifeCycle(void) // Rename to ProcessCycle/ThinkinkCycle/DecisionCycle?
{
	// Run through tribes:
	for(int x = 0; x < TRIBE_NUMTRIBES; x++)
	{
		// Run through tribe members:
        for(cubeNode *tempCubeNode = globalTribes[x].tribeHead; tempCubeNode != NULL; tempCubeNode = tempCubeNode->nextCubeInTribe)
		{
            // Is cube within tribe territory
            if(globalTribes[x].withinTerritory(tempCubeNode->cube.disp.e1, tempCubeNode->cube.disp.e2))
            {
            	// Exchange info:
            	// Collect food/material location information:
                if(tempCubeNode->cube.cubesBodyMind.knowFoodLocation())             // Does cube actually know the loc of food?
                    globalTribes[x].consensusFoodPosUpdate(tempCubeNode->cube.cubesBodyMind.foodMem.e1, tempCubeNode->cube.cubesBodyMind.foodMem.e2);
                else if(globalTribes[x].knowFoodLocation())							// If tribe knows food loc, cube doesnt, give to cube
                {																	// Maybe incorporate foodLoConfidence val into if, so that dont give bogus locs
                	tempCubeNode->cube.cubesBodyMind.foodMem.e1 = globalTribes[x].tribeFoodPosConsensus.e1;
        			tempCubeNode->cube.cubesBodyMind.foodMem.e2 = globalTribes[x].tribeFoodPosConsensus.e2;
                }

                if(tempCubeNode->cube.cubesBodyMind.knowMaterialLocation())         // Does cube actually know the loc of materials?
                    globalTribes[x].consensusMaterialPosUpdate(tempCubeNode->cube.cubesBodyMind.materialMem.e1, tempCubeNode->cube.cubesBodyMind.materialMem.e2);
                else if(globalTribes[x].knowMaterialLocation())						// If tribe knows material loc, cube doesnt, give to cube
    			{																	// Maybe incorporate materialLoConfidence val into if, so that dont give bogus locs
    				tempCubeNode->cube.cubesBodyMind.materialMem.e1 = globalTribes[x].tribeMaterialPosConsensus.e1;
    				tempCubeNode->cube.cubesBodyMind.materialMem.e2 = globalTribes[x].tribeMaterialPosConsensus.e2;
    			}

    			// give location of predators

    			// Check if tribe has food:
    			if(globalTribes[x].collectiveFood>=200)
    				tempCubeNode->cube.cubesBodyMind.infoOnTribe.tribeHasFood = true;
    			else
    				tempCubeNode->cube.cubesBodyMind.infoOnTribe.tribeHasFood = false;

            	switch(tempCubeNode->cube.cubeType)
            	{
            		case FORAGER:
            			// Take/Give resources:
                    	if(tempCubeNode->cube.cubesBodyMind.isHungry())					// Feed cube if its hungry
                    		tempCubeNode->cube.cubesBackPack.addFood(globalTribes[x].giveFood(190), tempCubeNode->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);// 200 is 2full healths
                    	else															// Take excess food if not hungry
                     		globalTribes[x].addToCollectiveFood(tempCubeNode->cube.cubesBackPack.giveFood(globalTribes[x].foodDue(tempCubeNode, 190)));

                    	if(tempCubeNode->cube.cubesBackPack.isPrepUnloadMaterial())		// does cube have any materials
                    		globalTribes[x].addToCollectiveMaterial(tempCubeNode->cube.cubesBackPack.giveMaterial(CUBE_BACKPACK_MATERIALLIMIT));
            		break;
            		case MESSENGER:
            			// Take/Give resources:
                    	if(tempCubeNode->cube.cubesBodyMind.isHungry())					// Feed cube if its hungry
                    		tempCubeNode->cube.cubesBackPack.addFood(globalTribes[x].giveFood(200), tempCubeNode->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);// 200 is 2full healths
                    	else															// Take excess food if not hungry
                     		globalTribes[x].addToCollectiveFood(tempCubeNode->cube.cubesBackPack.giveFood(globalTribes[x].foodDue(tempCubeNode, 200)));

                    	if(tempCubeNode->cube.cubesBackPack.isPrepUnloadMaterial())		// does cube have any materials
                    		globalTribes[x].addToCollectiveMaterial(tempCubeNode->cube.cubesBackPack.giveMaterial(CUBE_BACKPACK_MATERIALLIMIT));

                    	// Messenger returns to tribe to update info, so reset its callback, so goes back out again
                    	tempCubeNode->cube.cubesBodyMind.infoOnTribe.tribeCallBackThreshold = 0;
                    break;
            		case FEEDER:
            			// Take/Give resources: Restock feeder
                    	if(tempCubeNode->cube.cubesBodyMind.isHungry() || tempCubeNode->cube.cubesBackPack.getFoodStored() <= CUBETYPE_FEEDER_PACKSIZE_THRESHOLD)
                    		tempCubeNode->cube.cubesBackPack.addFood(globalTribes[x].giveFood(400), tempCubeNode->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);
                    	else
                    		globalTribes[x].addToCollectiveFood(tempCubeNode->cube.cubesBackPack.giveFood(globalTribes[x].foodDue(tempCubeNode, CUBETYPE_FEEDER_PACKSIZE_THRESHOLD*2)));

                    	if(tempCubeNode->cube.cubesBackPack.isPrepUnloadMaterial())// does cube have any materials
                    		globalTribes[x].addToCollectiveMaterial(tempCubeNode->cube.cubesBackPack.giveMaterial(CUBE_BACKPACK_MATERIALLIMIT));
            		break;
            		case PREDATOR:
            			// Predator returns to tribe to update info, so reset its callback, so goes back out again
            			tempCubeNode->cube.cubesBodyMind.infoOnTribe.tribeCallBackThreshold = 0;
            		break;
            	}

    			/*	BST MESSENGER LIST
    			 * if(tribe member is forager)
    			 * 		search through BST of messengers to find closest one within messenger sight range
    			 * 		exchange info b/w messenger and forager
    			 *
    			 */
    			// check if within CUBETYPE_MESSENGER_SIGHTRANGE of cube in same tribe, then transfer info
				// run through forager/warriors/etc of tribe
				// probably should happen in tribecycle, dont have access to forager list from here

    			/*
    			 * messenger type;
    			 * 		mayb have a relayCounter which increments whenever a messenger has exchanged info with a cube
    			 * 			when counter reaches threshold, messenger reports back to tribe, updates info
    			 * 		messengers caught by food radius...
    			 * 			messengers only fed by tribe?
    			 */
            }

            // Make these guys more efficient; need some way of preventing the compounding of bogus vals
            // Perform messenger operations on this cube:
            // Run through messengers
            // NOT FULLY TESTED; ACTUALLY TRAVERSING ENTIRE LIST? CUBES SEEM TO BE CONTACTED..
            if(tempCubeNode->cube.cubeType != MESSENGER)// Messengers dont talk to messengers, they have a task to do
            {
				for(CTribeMessengerNode *tempMessengerNode = globalTribes[x].tribeMessengerHead; tempMessengerNode != NULL; tempMessengerNode = tempMessengerNode->nextTribeMessenger)
				{
					// Is current tribe member within messenger's sight range:
					if(Distance2f(tempCubeNode->cube.disp.e1, tempCubeNode->cube.disp.e2, tempMessengerNode->tribeMessenger->cube.disp.e1, tempMessengerNode->tribeMessenger->cube.disp.e2) < CUBETYPE_MESSENGER_SIGHTRANGE)
					{
						// Check if just communicated with cube
						if(tempMessengerNode->tribeMessenger->cube.cubesBodyMind.infoOnTribe.lastTribeMemberContacted != tempCubeNode->tribeMemberNum)
						{
							tempCubeNode->cube.cubesBodyMind.cubeColour[0] = 1;

							// Increase amount of cubes communicated with:
							tempMessengerNode->tribeMessenger->cube.cubesBodyMind.infoOnTribe.tribeCallBackThreshold++;
							// Register cube's tribe number in messenger memory:
							tempMessengerNode->tribeMessenger->cube.cubesBodyMind.infoOnTribe.lastTribeMemberContacted = tempCubeNode->tribeMemberNum;

							// Does messenger know tribe has food? Yes: give cube messenger's info:
							//if(tempMessengerNode->tribeMessenger->cube.cubesBodyMind.infoOnTribe.tribeHasFood)
							//	tempCubeNode->cube.cubesBodyMind.infoOnTribe.tribeHasFood = true;

							// Does messenger know food location? Yes: give cube messenger's info if cube doesnt know:
							if(tempMessengerNode->tribeMessenger->cube.cubesBodyMind.knowFoodLocation() && !tempCubeNode->cube.cubesBodyMind.knowFoodLocation())
							{
								tempCubeNode->cube.cubesBodyMind.foodMem.e1 = tempMessengerNode->tribeMessenger->cube.cubesBodyMind.foodMem.e1;
								tempCubeNode->cube.cubesBodyMind.foodMem.e2 = tempMessengerNode->tribeMessenger->cube.cubesBodyMind.foodMem.e2;
							}
							else if(tempCubeNode->cube.cubesBodyMind.knowFoodLocation() && !tempMessengerNode->tribeMessenger->cube.cubesBodyMind.knowFoodLocation())// If cube knows food loc and messenger doesnt, take cubes info:
							{
								tempMessengerNode->tribeMessenger->cube.cubesBodyMind.foodMem.e1 = tempCubeNode->cube.cubesBodyMind.foodMem.e1;
								tempMessengerNode->tribeMessenger->cube.cubesBodyMind.foodMem.e2 = tempCubeNode->cube.cubesBodyMind.foodMem.e2;
							}

							// Does messenger know material location? Yes: give cube messenger's info if cube doesnt know:
							if(tempMessengerNode->tribeMessenger->cube.cubesBodyMind.knowMaterialLocation() && !tempCubeNode->cube.cubesBodyMind.knowMaterialLocation())
							{
								tempCubeNode->cube.cubesBodyMind.materialMem.e1 = tempMessengerNode->tribeMessenger->cube.cubesBodyMind.materialMem.e1;
								tempCubeNode->cube.cubesBodyMind.materialMem.e2 = tempMessengerNode->tribeMessenger->cube.cubesBodyMind.materialMem.e2;
							}
							else if(tempCubeNode->cube.cubesBodyMind.knowMaterialLocation() && !tempMessengerNode->tribeMessenger->cube.cubesBodyMind.knowMaterialLocation())// If cube knows material loc, take info:
							{
								tempMessengerNode->tribeMessenger->cube.cubesBodyMind.materialMem.e1 = tempCubeNode->cube.cubesBodyMind.materialMem.e1;
								tempMessengerNode->tribeMessenger->cube.cubesBodyMind.materialMem.e2 = tempCubeNode->cube.cubesBodyMind.materialMem.e2;
							}
						}
					}
				}
            }

            // Run Feeder routines
            // Run through Feeders:
            for(CTribeFeederNode *tempFeederNode = globalTribes[x].tribeFeederHead; tempFeederNode != NULL; tempFeederNode = tempFeederNode->nextTribeFeeder)
            {
               	// Is current tribe member within Feeder's sight range:
				if(Distance2f(tempCubeNode->cube.disp.e1, tempCubeNode->cube.disp.e2, tempFeederNode->tribeFeeder->cube.disp.e1, tempFeederNode->tribeFeeder->cube.disp.e2) < CUBETYPE_FEEDER_SIGHTRANGE)
				{
					if(tempCubeNode->cube.cubeType == FORAGER)	// Keep giving forager food; bcs its job is to return food to tribe
					{
						// Make sure dont give more food than possible
						if((tempCubeNode->cube.cubesBackPack.getFoodStored()+100) < (CUBE_BACKPACK_FOODLIMIT+tempCubeNode->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize))
							tempCubeNode->cube.cubesBackPack.addFood(tempFeederNode->tribeFeeder->cube.cubesBackPack.giveFood(100), tempCubeNode->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);
					}
					else// WHY DOES THIS DEPLETE THE FOOD STORES OF FEEDER SO MUCH>?
					{
						if(tempCubeNode->cube.cubesBodyMind.isHungry())
						{
							tempCubeNode->cube.cubesBackPack.addFood(tempFeederNode->tribeFeeder->cube.cubesBackPack.giveFood(100), tempCubeNode->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);
						}
						else
							tempFeederNode->tribeFeeder->cube.cubesBackPack.addFood(tempCubeNode->cube.cubesBackPack.giveFood(globalTribes[x].foodDue(tempCubeNode, 200)), tempFeederNode->tribeFeeder->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);
					}
				}
            }
		}

        // Run Predator routines
        // Run through predators
        for(CTribePredatorNode *tempPredatorNode = globalTribes[x].tribePredatorHead; tempPredatorNode != NULL; tempPredatorNode = tempPredatorNode->nextTribePredator)
        {
        	// Sector Head of predator's sector:
        	cubeNode *predatorSectorHead = NULL;
        	// Get head of sector
        	predatorSectorHead = returnSectorHead(tempPredatorNode->tribePredator->cube.cubeSectorXZ.e1, tempPredatorNode->tribePredator->cube.cubeSectorXZ.e2);

        	// Make sure predator is actually in a sector
        	if(predatorSectorHead != NULL)
				for(cubeNode *tempCubeInSector = predatorSectorHead; tempCubeInSector != NULL; tempCubeInSector = tempCubeInSector->nextCubeInSectorList)
				{
					// Is current cube within predator's sight range:
					if(Distance2f(tempCubeInSector->cube.disp.e1, tempCubeInSector->cube.disp.e2, tempPredatorNode->tribePredator->cube.disp.e1, tempPredatorNode->tribePredator->cube.disp.e2) < CUBETYPE_PREDATOR_SIGHTRANGE)
					{
						// Increment attack counter:
						tempPredatorNode->tribePredator->cube.cubesBodyMind.infoOnTribe.tribeCallBackThreshold++;

						// Make sure dont attack tribe members
						if(tempCubeInSector->affiliatedTribeID != tempPredatorNode->tribePredator->affiliatedTribeID)
						{
							// Prey knows food loc, predator doesnt, take prey's info:
							if(tempCubeInSector->cube.cubesBodyMind.knowFoodLocation() && !tempPredatorNode->tribePredator->cube.cubesBodyMind.knowFoodLocation())
							{
								tempPredatorNode->tribePredator->cube.cubesBodyMind.foodMem.e1 = tempCubeInSector->cube.cubesBodyMind.foodMem.e1;
								tempPredatorNode->tribePredator->cube.cubesBodyMind.foodMem.e2 = tempCubeInSector->cube.cubesBodyMind.foodMem.e2;
							}

							/* ummmmmm: ???
							// Fill predators pack from prey's pack
							if(tempCubeInSector->cube.cubesBackPack.getFoodStored() < (CUBETYPE_PREDATOR_PACKSIZE - tempPredatorNode->tribePredator->cube.cubesBackPack.getFoodStored()))
							{
								tempPredatorNode->tribePredator->cube.cubesBackPack.addFood(tempCubeInSector->cube.cubesBackPack.giveFood(tempCubeInSector->cube.cubesBackPack.getFoodStored()), tempPredatorNode->tribePredator->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);
							}
							*/

							// Take from prey's pack: add 50/cycle
							if(50 < (CUBETYPE_PREDATOR_PACKSIZE - tempPredatorNode->tribePredator->cube.cubesBackPack.getFoodStored()))
							{
								tempPredatorNode->tribePredator->cube.cubesBackPack.addFood(tempCubeInSector->cube.cubesBackPack.giveFood(50), tempPredatorNode->tribePredator->cube.cubesBodyMind.cubeMods.modifier_cubeBackPackSize);
							}
						}
						else// this cube is a tribe member
						{
							// if tribe member doesnt have a loc for food, give it:
							if(!tempCubeInSector->cube.cubesBodyMind.knowFoodLocation() && tempPredatorNode->tribePredator->cube.cubesBodyMind.knowFoodLocation())
							{
								tempCubeInSector->cube.cubesBodyMind.foodMem.e1 = tempPredatorNode->tribePredator->cube.cubesBodyMind.foodMem.e1;
								tempCubeInSector->cube.cubesBodyMind.foodMem.e2 = tempPredatorNode->tribePredator->cube.cubesBodyMind.foodMem.e2;
							}
						}
					}
				}
        }
	}
}

//-------------------------------------------------------------------------------------
//----^^^^^^^^^^^^^^CUBE GOD CLASS^^^^^^^^^^^^^^^--------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

