You better get used to this. youre gonna have everyone asking you for free dental advice once they
find out you are a dentist
also, <---- yusufs first patient, i win

its those useful carreers; lawyer, doctor, ; always asked for free advice
biochemists will just get the; how do i lose weight?; implying you need boichem to answer that
my aunts find out im finishing biochem, ask me how to lose weight, dont even need my biochem
knowledge, i just say; you need more exercise


lol you cant say counterparts and then imply they require additional kindness/gentleness. You also
imply being civil/well mannered means treating women with gentleness/kindness, lol you see how thats 
discrimination? i mean, can i get you to scold sam for using ugly language when im present pls?
your thinking; they are women = they require special behavior = discrimination
	its male chauvenism because you are in a superior position to give kindess and be gentle
	implying being civil and well mannered is to be kind/gentle to women; 
	
lol this has gotta be troll. this is the first thing you say to troll women feminists, by claiming their sex
requires kindness/gentleness. then you go on to say this is how to be civil/well mannered !!

// kijiji
- lcd tv 46", <350$	- has hdmi port, dvi port good, usb good
- lcd monitor 22" <100
- cheap andriod <40
- roadbike wheel <20$
- electric guitar <80 ibanez
- elec guit pedal <40

// sir yolo swaggins III

// to cam class
- fix forward/right vec decay, becomes move in wrong dir
- add camPos, roll/yaw/pitch speed to constructor

// Possibly useful
card holder
paper holder

// maek game
Abiogenesis - the natural process by which life arises from simple organic compounds.

//--------------------------------------------------------------
//=====================talk to me, talk to me===================
//--------------------------------------------------------------
talk to me pls? @ *hopeToTalkTo
	hopeToTalkTo
	{
		if(timeToTalk > ?)
			if((benefits - detriments))
				talk();
	}
	
	

//--------------------------------------------------------------
//=====================shape recog==============================
//--------------------------------------------------------------
videoOutMat[w][h];

struct rectDef
{
	Vert2xi leftTop, leftBot, rightTop, rightBot;
}

struct boundaryNode
{
	boundaryNode *nextNodeInBoundary;
}

struct boundaryDef
{
	boundaryNode *boundaryHead;
}

void rectGets(videoOutMat, rectDef *rect)
{
	// Scan VMat, return ptr to 
}

void boundGets(videoOutMat, boundaryDef *bound)
{
	// Scan VMat build snake of LL across bound, return head
}



//--------------------------------------------------------------
//=====================Snake====================================
//--------------------------------------------------------------

propagate()
{
	// Tail takes dir of head
	for(snakeNode *tempSNode = snakeTail; tempSNode != NULL; tempSNode = tempSNode->prevSNode)
	{
		tempSNode->forwardVec.x = tempSNode->prevSNode->forwardVec.x;
		tempSNode->forwardVec.z = tempSNode->prevSNode->forwardVec.z;
	}
	
	// Move snake
	for(snakeNode *tempSNode = snakeHead; tempSNode != NULL; tempSNode = tempSNode->nextSNode)
	{
		tempSNode->pos.x += tempSNode->forwardVec.x*;
		tempSNode->pos.z += tempSNode->forwardVec.z;
	}
}

processPickup(pickupType type)
{
	switch(type)
	{
	case shorten:
		// -- node @ tail
		break;
	case lengthen:
		//++ node to tail
		break;
	case quicken:
		// ++Snake speed
		break;
	case slow:
		break;
	}
}
