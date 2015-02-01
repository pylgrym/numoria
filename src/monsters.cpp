/* source/monsters.c: monster definitions

   Copyright (C) 1989-2008 James E. Wilson, Robert A. Koeneke, 
                           David J. Grabiner

   This file is part of Umoria.

   Umoria is free software; you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Umoria is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of 
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with Umoria.  If not, see <http://www.gnu.org/licenses/>. */

#include "stdafx.h"
#include "config.h"
#include "constant.h"
#include "types.h"

/* Following are creature arrays and variables			*/
	/* Creatures must be defined here				*/
	/*	See types.h under creature_type for a complete list
		of all variables for creatures.	 Some of the less obvious
		are explained below.

	Hit points:	#1, #2: where #2 is the range of each roll and
			#1 is the number of added up rolls to make.
			Example: a creature with 5 eight-sided hit die
			is given {5,8}.

		Attack types:
		1	Normal attack
		2	Poison Strength
		3	Confusion attack
		4	Fear attack
		5	Fire attack
		6	Acid attack
		7	Cold attack
		8	Lightning attack
		9	Corrosion attack
		10	Blindness attack
		11	Paralysis attack
		12	Steal Money
		13	Steal Object
		14	Poison
		15	Lose dexterity
		16	Lose constitution
		17	Lose intelligence
		18	Lose wisdom
		19	Lose experience
		20	Aggravation
		21	Disenchants
		22	Eats food
		23	Eats light
		24	Eats charges
		99	Blank

		Attack descriptions:
		1	hits you.
		2	bites you.
		3	claws you.
		4	stings you.
		5	touches you.
		6	kicks you.
		7	gazes at you.
		8	breathes on you.
		9	spits on you.
		10	makes a horrible wail.
		11	embraces you.
		12	crawls on you.
		13	releases a cloud of spores.
		14	begs you for money.
		15	You've been slimed.
		16	crushes you.
		17	tramples you.
		18	drools on you.
		19	insults you.
		99	is repelled.

	Example:  For a creature which bites for 1d6, then stings for
		  2d4 and loss of dex you would use:
			{1,2,1,6},{15,4,2,4}

		CMOVE flags:
Movement.	00000001	Move only to attack
	.	00000002	Move, attack normal
	.	00000008	20% random movement
	.	00000010	40% random movement
	.	00000020	75% random movement
Special +	00010000	Invisible movement
	+	00020000	Move through door
	+	00040000	Move through wall
	+	00080000	Move through creatures
	+	00100000	Picks up objects
	+	00200000	Multiply monster
Carries =	01000000	Carries objects.
	=	02000000	Carries gold.
	=	04000000	Has 60% of time.
	=	08000000	Has 90% of time.
	=	10000000	1d2 objects/gold.
	=	20000000	2d2 objects/gold.
	=	40000000	4d2 objects/gold.
Special ~	80000000	Win-the-Game creature.

		SPELL Flags:
Frequency	000001	  1	These add up to x.  Then
(1 in x).	000002	  2	if RANDINT(X) = 1 the
	.	000004	  4	creature casts a spell.
	.	000008	  8
Spells	=	000010	Teleport short (blink)
	=	000020	Teleport long
	=	000040	Teleport player to monster
	=	000080	Cause light wound
	=	000100	Cause serious wound
	=	000200	Hold person (Paralysis)
	=	000400	Cause blindness
	=	000800	Cause confusion
	=	001000	Cause fear
	=	002000	Summon monster
	=	004000	Summon undead
	=	008000	Slow Person
	=	010000	Drain Mana
	=	020000	Not Used
	=	040000	Not Used
Breath/	+	080000	Breathe/Resist Lightning
Resist	+	100000	Breathe/Resist Gas
	+	200000	Breathe/Resist Acid
	+	400000	Breathe/Resist Frost
	+	800000	Breathe/Resist Fire

		CDEFENSE flags:
		0001	Hurt by Slay Dragon.
		0002	Hurt by Slay Animal.
		0004	Hurt by Slay Evil.
		0008	Hurt by Slay Undead.
		0010	Hurt by Frost.
		0020	Hurt by Fire.
		0040	Hurt by Poison.
		0080	Hurt by Acid.
		0100	Hurt by Light-Wand.
		0200	Hurt by Stone-to-Mud.
		0400	Not used.
		0800	Not used.
		1000	Cannot be charmed or slept.
		2000	Can be seen with infra-vision.
		4000	Max Hit points.
		8000	Not used.


	Sleep (sleep)	:	A measure in turns of how fast creature
				will notice player (on the average).
	Area of affect (aaf) :	Max range that creature is able to "notice"
				the player.
									*/
typedef const COLORREF COL;

const COLORREF redDark = RGB(128, 0, 0);
COL grey = RGB(128, 128, 128);
COL greyDark = RGB(64, 64, 64);
COL greyLight = RGB(200, 200, 200);
COL greyLightBlue = RGB(200, 200, 220);
COL greyBlue = RGB(128, 128, 160);
COL greyPink = RGB(128, 128, 160);
COL greyBrown = RGB(220, 200, 180);
COL greyCyan = RGB(100, 128, 128);
COL greyGreen = RGB(132, 124, 110); // 100, 128, 100);
COL greenGrey = RGB(93, 137, 97); //  100, 192, 100);
COL yellow = RGB(255, 255, 0);
COL white = RGB(255,255,255);
COL silver = RGB(204, 198, 206); //  255, 255, 255);
COL whiteBlue = RGB(240,240,255);
COL whiteGreen = RGB(240,255,240);
COL whiteCyan = RGB(240,255,255);
COL whiteMummy = RGB(191, 160, 113); 
COL brown = RGB(106, 47, 29); // 150, 120, 90);
COL tanColor = RGB(192, 156, 94); // 0, 200, 0);
COL green = RGB(0,200,0);
COL greenZombie = RGB(50,255,90);
COL lime = RGB(90,255,0);
COL yellowGreen = RGB(184, 228, 17); //  90, 255, 0);
COL pink = RGB(255,128,200);
COL pinkSkin = RGB(255,178,200);
COL cyan = RGB(0,255,255);
COL cyanGhost = RGB(83, 189, 203); 
COL black = RGB(40,40,40);
COL blue = RGB(0,0,255);
COL blueDark = RGB(0,0,166);
COL blueLight = RGB(128,128,255);
COL brownLight = RGB(133, 84, 57); // 255, 0, 0);
COL orange = RGB(255,128,0);
COL orangeGlow = RGB(255,108,5);
COL crimson = RGB(255,0,62);
COL red = RGB(255,0,0);
COL redGlow = RGB(240,0,0); // for vampires..
COL copperRed = RGB(255,70,70);
COL rotRed = RGB(255,40,150);
COL purple = RGB(255,0,255);
COL purpleBlue = RGB(134,17,252);
COL purpleBlack  = RGB(130,0,140);
COL gold = RGB(255,255,136);
COL fireColor = RGB(250,118,9);
COL violet = RGB(218,125,255);
COL pinkFlesh = RGB(227,172,152);
COL purpleRed = RGB(204, 0, 97); 
/*
COL x = RGB();
COL x = RGB();
COL x = RGB();
COL x = RGB();
COL x = RGB();
COL x = RGB();
COL x = RGB();
COL x = RGB();
COL x = RGB();
COL x = RGB();
*/
creature_type c_list[MAX_CREATURES] = {
{"Filthy Street Urchin"	    ,0x0012000AL,0x00000000L,0x2034,    0,  40,   4,	1,  11, 'p', { 1, 4}   , {72,148,0,0}		,    0, redDark},
{"Blubbering Idiot"	        ,0x0012000AL,0x00000000L,0x2030,    0,   0,   6,	1,  11, 'p', { 1, 2}   , {79,0,0,0}		  ,    0, redDark},
{"Pitiful-Looking Beggar"   ,0x0012000AL,0x00000000L,0x2030,    0,  40,  10,	1,  11, 'p', { 1, 4}   , {72,0,0,0}		  ,    0, redDark},
{"Mangy-Looking Leper"	    ,0x0012000AL,0x00000000L,0x2030,    0,  50,  10,	1,  11, 'p', { 1, 1}   , {72,0,0,0}		  ,    0, redDark},
{"Squint-Eyed Rogue"	      ,0x07120002L,0x00000000L,0x2034,    0,  99,  10,	8,  11, 'p', { 2, 8}   , {5,149,0,0}		,    0, redDark},
{"Singing, Happy Drunk"	    ,0x06120038L,0x00000000L,0x2030,    0,   0,  10,	1,  11, 'p', { 2, 3}   , {72,0,0,0}		  ,    0, redDark},
{"Mean-Looking Mercenary"   ,0x0B12000AL,0x00000000L,0x2034,    0, 250,  10,  20, 11, 'p', { 5, 8}   , {9,0,0,0}			,    0, redDark},
{"Battle-Scarred Veteran"   ,0x0B12000AL,0x00000000L,0x2030,    0, 250,  10,  30, 11, 'p', { 7, 8}   , {15,0,0,0}		  ,    0, redDark},
{"Grey Mushroom patch"	    ,0x00000001L,0x00000000L,0x10A0,    1,   0,   2,	1,  11, ',', { 1, 2}   , {91,0,0,0}		  ,    1, grey},
{"Giant Yellow Centipede"   ,0x00000002L,0x00000000L,0x0002,    2,  30,   8,  12, 11, 'c', { 2, 6}   , {26,60,0,0}		,    1, yellow},
{"Giant White Centipede"    ,0x0000000AL,0x00000000L,0x0002,    2,  40,   7,  10, 11, 'c', { 3, 5}   , {25,59,0,0}		,    1, white},
{"White Icky-Thing"	        ,0x00000012L,0x00000000L,0x0020,    2,  10,  12,	7,  11, 'i', { 3, 5}   , {63,0,0,0}		  ,    1, white},
{"Clear Icky-Thing"	        ,0x00010012L,0x00000000L,0x0020,    1,  10,  12,	6,  11, 'i', { 2, 5}   , {63,0,0,0}		  ,    1, grey},
{"Giant White Mouse"	      ,0x0020000AL,0x00000000L,0x2072,    1,  20,   8,	4,  11, 'r', { 1, 3}   , {25,0,0,0}	  	,    1, white},
{"Large Brown Snake"	      ,0x0000000AL,0x00000000L,0x00B2,    3,  99,   4,  35, 10, 'R', { 4, 6}   , {26,73,0,0}		,    1, brown},
{"Large White Snake"	      ,0x00000012L,0x00000000L,0x00B2,    2,  99,   4,  30, 11, 'R', { 3, 6}   , {24,0,0,0}		  ,    1, white},
{"Kobold"		                ,0x07820002L,0x00000000L,0x2030,    5,  10,  20,  16, 11, 'k', { 3, 7}   , {5,0,0,0}			,    1, green},
{"White Worm mass"	        ,0x00200022L,0x00000000L,0x01B2,    2,  10,   7,	1,  10, 'w', { 4, 4}   , {173,0,0,0}		,    1, white},
{"Floating Eye"		          ,0x00000001L,0x0001000DL,0x2100,    1,  10,   2,	6,  11, 'e', { 3, 6}   , {146,0,0,0}		,    1, white},
{"Shrieker Mushroom patch"  ,0x00000001L,0x00000000L,0x10A0,    1,   0,   2,	1,  11, ',', { 1, 1}   , {203,0,0,0}		,    2, pink},
{"Blubbering Icky-Thing"    ,0x0B980012L,0x00000000L,0x0020,    8,  10,  14,	4,  11, 'i', { 5, 8}   , {174,210,0,0}	,    2, cyan},
{"Metallic Green Centipede" ,0x00000012L,0x00000000L,0x0002,    3,  10,   5,	4,  12, 'c', { 4, 4}   , {68,0,0,0}		  ,    2, green},
{"Novice Warrior"	          ,0x07020002L,0x00000000L,0x2030,    6,   5,  20,  16, 11, 'p', { 9, 4}   , {6,0,0,0}			,    2, redDark},
{"Novice Rogue"		          ,0x07120002L,0x00000000L,0x2034,    6,   5,  20,  12, 11, 'p', { 8, 4}   , {5,148,0,0}		,    2, redDark},
{"Novice Priest"	          ,0x07020002L,0x0000108CL,0x2030,    7,   5,  20,  10, 11, 'p', { 7, 4}   , {4,0,0,0}			,    2, redDark},
{"Novice Mage"              ,0x07020002L,0x0000089CL,0x2030,    7,   5,  20,  6,  11, 'p', { 6, 4}   , {3, 0, 0, 0 }  ,    2, redDark},
{"Yellow Mushroom patch"    ,0x00000001L,0x00000000L,0x10A0,    2,   0,   2,	1,  11, ',', { 1, 1}   , {100,0,0,0}		,    2, yellow},
{"White Jelly"		          ,0x00000001L,0x00000000L,0x11A0,   10,  99,   2,	1,  12, 'J', { 8, 8}   , {168,0,0,0}		,    2, white},
{"Giant Green Frog"	        ,0x0000000AL,0x00000000L,0x00A2,    6,  30,  12,	8,  11, 'f', { 2, 8}   , {26,0,0,0}		  ,    2, green},
{"Giant Black Ant"	        ,0x0000000AL,0x00000000L,0x0002,    8,  80,   8,  20, 11, 'a', { 3, 6}   , {27,0,0,0}		  ,    2, black},
{"White Harpy"		          ,0x00000012L,0x00000000L,0x2034,    5,  10,  16,  17, 11, 'h', { 2, 5}   , {49,49,25,0}		,    2, white},
{"Blue Yeek"		            ,0x07020002L,0x00000000L,0x2030,    4,  10,  18,  14, 11, 'y', { 2, 6}   , {4,0,0,0}			,    2, blue},
{"Green Worm mass"	        ,0x00200022L,0x00000000L,0x0132,    3,  10,   7,	3,  10, 'w', { 6, 4}   , {140,0,0,0}		,    2, green},
{"Large Black Snake"	      ,0x0000000AL,0x00000000L,0x00B2,    9,  75,   5,  38, 10, 'R', { 4, 8}   , {27,74,0,0}		,    2, black},
{"Poltergeist"		          ,0x0F95003AL,0x0000001FL,0x110C,    6,  10,   8,  15, 13, 'G', { 2, 5}   , {93,0,0,0}		  ,    3, cyan},
{"Metallic Blue Centipede"  ,0x00000012L,0x00000000L,0x0002,    7,  15,   6,	6,  12, 'c', { 4, 5}   , {69,0,0,0}		  ,    3, blue},
{"Giant White Louse"	      ,0x00200022L,0x00000000L,0x01F2,    1,  10,   6,	5,  12, 'l', { 1, 1}   , {24,0,0,0}		  ,    3, white},
{"Black Naga"		            ,0x0710000AL,0x00000000L,0x20E4,   20, 120,  16,  40, 11, 'n', { 6, 8}   , {75,0,0,0}		  ,    3, black},
{"Spotted Mushroom patch"   ,0x00000001L,0x00000000L,0x10A0,    3,   0,   2,	1,  11, ',', { 1, 1}   , {175,0,0,0}		,    3, red},
{"Yellow Jelly"		          ,0x00000001L,0x0001000FL,0x11A0,   12,  99,   2,	1,  12, 'J', {10, 8}   , {169,0,0,0}		,    3, yellow},
{"Scruffy-Looking Hobbit"   ,0x07920002L,0x00000000L,0x2034,    4,  10,  16,	8,  11, 'p', { 3, 5}   , {3,148,0,0}		,    3, redDark},
{"Huge Brown Bat"	          ,0x00000022L,0x00000000L,0x2162,    4,  40,   8,  12, 13, 'b', { 2, 6}   , {25,0,0,0}		  ,    3, brown},
{"Giant White Ant"	        ,0x00000002L,0x00000000L,0x0002,    7,  80,   8,  16, 11, 'a', { 3, 6}   , {27,0,0,0}		  ,    3, white},
{"Yellow Mold"		          ,0x00000001L,0x00000000L,0x10A0,    9,  99,   2,  10, 11, 'm', { 8, 8}   , {3,0,0,0}			,    3, yellow},
{"Metallic Red Centipede"   ,0x0000000AL,0x00000000L,0x0002,   12,  20,   8,	9,  12, 'c', { 4, 8}   , {69,0,0,0}		  ,    3, red},
{"Yellow Worm mass"	        ,0x00200022L,0x00000000L,0x01B2,    4,  10,   7,	4,  10, 'w', { 4, 8}   , {182,0,0,0}		,    3, yellow},
{"Large Green Snake"	      ,0x0000000AL,0x00000000L,0x00B2,   10,  70,   5,  40, 10, 'R', { 6, 8}   , {27,74,0,0}		,    3, green},
{"Radiation Eye"	          ,0x00000001L,0x0001000BL,0x2100,    6,  10,   2,	6,  11, 'e', { 3, 6}   , {88,0,0,0}		  ,    3, blueLight},
{"Drooling Harpy"	          ,0x00000012L,0x00000000L,0x2034,    7,  10,  16,  22, 11, 'h', { 2, 8}   , {49,49,25,79}	,    3, purple},
{"Silver Mouse"		          ,0x0020000AL,0x00000000L,0x0072,    1,  10,   8,	5,  11, 'r', { 1, 1}   , {212,0,0,0}		,    4, greyLightBlue},
{"Black Mushroom patch"	    ,0x00000001L,0x00000000L,0x10A0,    8,   0,   2,	1,  11, ',', { 8, 8}   , {71,0,0,0}		  ,    4, black},
{"Blue Jelly"		            ,0x00000001L,0x00400000L,0x11A0,   14,  99,   2,	1,  11, 'J', {12, 8}   , {125,0,0,0}		,    4, blue},
{"Creeping Copper Coins"    ,0x12000002L,0x00000000L,0x1000,    9,  10,   3,  24, 10, '$', { 7, 8}   , {3,170,0,0}		,    4, copperRed},
{"Giant White Rat"	        ,0x0020000AL,0x00000000L,0x2072,    1,  30,   8,	7,  11, 'r', { 2, 2}   , {153,0,0,0}		,    4, white},
{"Giant Black Centipede"    ,0x0000000AL,0x00000000L,0x0002,   11,  30,   8,  20, 11, 'c', { 5, 8}   , {25,59,0,0}		,    4, black},
{"Giant Blue Centipede"	    ,0x00000002L,0x00000000L,0x0002,   10,  50,   8,  20, 11, 'c', { 4, 8}   , {26,61,0,0}		,    4, blue},
{"Blue Worm mass"	          ,0x00200022L,0x00400000L,0x01A2,    5,  10,   7,  12, 10, 'w', { 5, 8}   , {129,0,0,0}		,    4, blue},
{"Large Grey Snake"	        ,0x0000000AL,0x00000000L,0x00B2,   14,  50,   6,  41, 10, 'R', { 6, 8}   , {28,75,0,0}		,    4, grey},
{"Jackal"		                ,0x00000012L,0x00000000L,0x2032,    8,  30,  12,  16, 11, 'j', { 3, 8}   , {29,0,0,0}		  ,    4, brownLight},
{"Green Naga"		            ,0x0710000AL,0x00200000L,0x2064,   30, 120,  18,  40, 11, 'n', { 9, 8}   , {75,118,0,0}		,    5, green},
{"Green Glutton Ghost"	    ,0x0F950032L,0x0000003FL,0x110C,   15,  10,  10,  20, 13, 'G', { 3, 6}   , {211,0,0,0}		,    5, green},
{"White Mushroom patch"	    ,0x00000001L,0x00000000L,0x10A0,    5,   0,   2,	1,  11, ',', { 1, 1}   , {147,0,0,0}		,    5, white},
{"Green Jelly"		          ,0x00000001L,0x00200000L,0x1120,   18,  99,   2,	1,  12, 'J', {22, 8}   , {136,0,0,0}		,    5, green},
{"Skeleton Kobold"	        ,0x00020002L,0x00000000L,0x100C,   12,  40,  20,  26, 11, 's', { 5, 8}   , {5,0,0,0}			,    5, white},
{"Silver Jelly"		          ,0x00000001L,0x00000000L,0x10A0,   15,  40,   2,  25, 11, 'J', {20, 8}   , {213,0,0,0}		,    5, greyLightBlue},
{"Giant Black Frog"	        ,0x0000000AL,0x00000000L,0x00A2,   12,  40,  12,  18, 11, 'f', { 4, 8}   , {29,0,0,0}		  ,    5, black},
{"Grey Icky-Thing"	        ,0x00000012L,0x00000000L,0x0020,   10,  15,  14,  12, 11, 'i', { 4, 8}   , {66,0,0,0}		  ,    5, grey},
{"Disenchanter Eye"	        ,0x00000001L,0x00010009L,0x2100,   20,  10,   2,  10, 10, 'e', { 7, 8}   , {207,0,0,0}		,    5, purple},
{"Black Yeek"		            ,0x07020002L,0x00000000L,0x2030,    8,  10,  18,  16, 11, 'y', { 2, 8}   , {4,0,0,0}			,    5, black},
{"Red Worm mass"	          ,0x00200022L,0x00800000L,0x2192,    6,  10,   7,  12, 10, 'w', { 5, 8}   , {111,0,0,0}		,    5, red},
{"Giant House Fly"	        ,0x00000022L,0x00000000L,0x0062,   10,  20,  12,  16, 13, 'F', { 3, 8}   , {25,0,0,0}	  	,    5, grey},
{"Copperhead Snake"	        ,0x00000012L,0x00000000L,0x00B2,   15,   1,   6,  20, 11, 'R', { 4, 6}   , {158,0,0,0}		,    5, copperRed},
{"Rot Jelly"		            ,0x00000001L,0x00000000L,0x10A0,   15,  99,   2,  30, 11, 'J', {20, 8}   , {209,0,0,0}		,    5, rotRed},
{"Purple Mushroom patch"    ,0x00000001L,0x00000000L,0x10A0,   12,   0,   2,	1,  12, ',', { 1, 1}   , {183,0,0,0}		,    6, purple},
{"Brown Mold"		            ,0x00000001L,0x00000000L,0x10A0,   20,  99,   2,  12, 11, 'm', {15, 8}   , {89,0,0,0}		  ,    6, brown},
{"Giant Brown Bat"	        ,0x0000001AL,0x00000000L,0x2162,   10,  30,  10,  15, 13, 'b', { 3, 8}   , {26,0,0,0}		  ,    6, brown},
{"Creeping Silver Coins"    ,0x16000002L,0x00000000L,0x1000,   18,  10,   4,  30, 10, '$', {12, 8}   , {5,171,0,0}		,    6, greyLightBlue},
{"Orc"			                ,0x0B020002L,0x00000000L,0x2034,   16,  30,  20,  32, 11, 'o', { 9, 8}   , {7,0,0,0}			,    6, green},
{"Grey Harpy"		            ,0x00000012L,0x00000000L,0x2034,   14,  10,  16,  20, 12, 'h', { 3, 8}   , {50,50,25,0}		,    6, grey},
{"Blue Icky-Thing"	        ,0x00000012L,0x00400000L,0x0020,   12,  20,  14,  14, 11, 'i', { 4, 8}   , {126,0,0,0}		,    6, blue},
{"Rattlesnake"		          ,0x00000012L,0x00000000L,0x00B2,   20,   1,   6,  24, 11, 'R', { 6, 7}   , {159,0,0,0}		,    6, brownLight},
{"Bloodshot Eye"	          ,0x00000001L,0x00010007L,0x2100,   15,  10,   2,	6,  11, 'e', { 4, 8}   , {143,0,0,0}		,    7, red},
{"Red Naga"		              ,0x0710000AL,0x00000000L,0x20E4,   40, 120,  20,  40, 11, 'n', {11, 8}   , {76,82,0,0}		,    7, red},
{"Red Jelly"		            ,0x00000001L,0x00000000L,0x11A0,   26,  99,   2,	1,  11, 'J', {26, 8}   , {87,0,0,0}		  ,    7, red},
{"Giant Red Frog"	          ,0x0000000AL,0x00000000L,0x00A2,   16,  50,  12,  16, 11, 'f', { 5, 8}   , {83,0,0,0}		  ,    7, red},
{"Green Icky-Thing"	        ,0x00000012L,0x00000000L,0x0020,   18,  20,  14,  12, 11, 'i', { 5, 8}   , {137,0,0,0}		,    7, green},
{"Zombie Kobold"	          ,0x00020002L,0x00000000L,0x102C,   14,  30,  20,  14, 11, 'z', { 6, 8}   , {1,1,0,0}			,    7, greenZombie},
{"Lost Soul"		            ,0x0F95001AL,0x0001002FL,0x110C,   18,  10,  12,  10, 11, 'G', { 2, 8}   , {11,185,0,0}		,    7, purpleBlue},
{"Greedy Little Gnome"	    ,0x0B920002L,0x00000000L,0x2034,   13,  10,  18,  14, 11, 'p', { 3, 8}   , {6,149,0,0}		,    7, blue},
{"Giant Green Fly"	        ,0x00000022L,0x00000000L,0x0062,   15,  20,  12,  14, 12, 'F', { 3, 8}   , {27,0,0,0}		  ,    7, green},
{"Brown Yeek"		            ,0x07020002L,0x00000000L,0x2030,   11,  10,  18,  18, 11, 'y', { 3, 8}   , {5,0,0,0}			,    8, brown},
{"Green Mold"		            ,0x00000001L,0x00000000L,0x10A0,   28,  75,   2,  14, 11, 'm', {21, 8}   , {94,0,0,0}		  ,    8, green},
{"Skeleton Orc"		          ,0x00020002L,0x00000000L,0x100C,   26,  40,  20,  36, 11, 's', {10, 8}   , {14,0,0,0}	  	,    8, white},
{"Seedy Looking Human"	    ,0x13020002L,0x00000000L,0x2034,   22,  20,  20,  26, 11, 'p', { 8, 8}   , {17,0,0,0}	  	,    8, redDark},
{"Red Icky-Thing"	          ,0x00000012L,0x00200000L,0x0020,   22,  20,  14,  18, 12, 'i', { 4, 8}   , {64,117,0,0}		,    8, red},
{"Bandit"		                ,0x13120002L,0x00000000L,0x2034,   26,  10,  20,  24, 11, 'p', { 8, 8}   , {13,148,0,0}		,    8, redDark},
{"Yeti"			                ,0x00020002L,0x00400000L,0x2024,   30,  10,  20,  24, 11, 'Y', {11, 8}   , {51,51,27,0}		,    9, white},
{"Bloodshot Icky-Thing"	    ,0x00000012L,0x0001000BL,0x0020,   24,  20,  14,  18, 11, 'i', { 7, 8}   , {65,139,0,0}		,    9, red},
{"Giant Grey Rat"	          ,0x0020000AL,0x00000000L,0x2072,    2,  20,   8,  12, 11, 'r', { 2, 3}   , {154,0,0,0}		,    9, grey},
{"Black Harpy"		          ,0x0000000AL,0x00000000L,0x2034,   19,  10,  16,  22, 12, 'h', { 3, 8}   , {50,50,26,0}		,    9, black},
{"Giant Black Bat"	        ,0x00000012L,0x00000000L,0x2162,   16,  25,  12,  18, 13, 'b', { 2, 8}   , {29,0,0,0}		  ,    9, black},
{"Clear Yeek"		            ,0x07030002L,0x00000000L,0x0030,   14,  10,  18,  24, 11, 'y', { 3, 6}   , {4,0,0,0}			,    9, greyLight},
{"Orc Shaman"		            ,0x0B020002L,0x00008085L,0x2034,   30,  20,  20,  15, 11, 'o', { 7, 8}   , {5,0,0,0}			,    9, green},
{"Giant Red Ant"	          ,0x00000002L,0x00000000L,0x0002,   22,  60,  12,  34, 11, 'a', { 4, 8}   , {27,85,0,0}		,    9, red},
{"King Cobra"		            ,0x00000012L,0x00000000L,0x00B2,   28,   1,   8,  30, 11, 'R', { 8, 8}   , {144,161,0,0}	,    9, yellow},
{"Clear Mushroom patch"	    ,0x00210001L,0x00000000L,0x10A0,    1,   0,   4,	1,  12, ',', { 1, 1}   , {70,0,0,0}		  ,   10, greyLight},
{"Giant White Tick"	        ,0x0000000AL,0x00000000L,0x0022,   27,  20,  12,  40, 10, 't', {15, 8}   , {160,0,0,0}		,   10, white},
{"Hairy Mold"		            ,0x00000001L,0x00000000L,0x10A0,   32,  70,   2,  15, 11, 'm', {15, 8}   , {151,0,0,0}		,   10, cyan},
{"Disenchanter Mold"	      ,0x00000001L,0x0001000BL,0x10A0,   40,  70,   2,  20, 11, 'm', {16, 8}   , {206,0,0,0}		,   10, purple},
{"Giant Red Centipede"	    ,0x00000002L,0x00000000L,0x0002,   24,  50,  12,  26, 12, 'c', { 3, 8}   , {25,164,0,0}		,   10, red},
{"Creeping Gold Coins"	    ,0x1A000002L,0x00000000L,0x1000,   32,  10,   5,  36, 10, '$', {18, 8}   , {14,172,0,0}		,   10, gold},
{"Giant Fruit Fly"	        ,0x00200022L,0x00000000L,0x0062,    4,  10,   8,  14, 12, 'F', { 2, 2}   , {25,0,0,0}		  ,   10, grey},
{"Brigand"		              ,0x13120002L,0x00000000L,0x2034,   35,  10,  20,  32, 11, 'p', { 9, 8}   , {13,149,0,0}		,   10, redDark},
{"Orc Zombie"		            ,0x00020002L,0x00000000L,0x102C,   30,  25,  20,  24, 11, 'z', {11, 8}   , {3,3,0,0}			,   11, greenZombie},
{"Orc Warrior"		          ,0x0B020002L,0x00000000L,0x2034,   34,  25,  20,  36, 11, 'o', {11, 8}   , {15,0,0,0}		  ,   11, green},
{"Vorpal Bunny"		          ,0x0020000AL,0x00000000L,0x2072,    2,  30,   8,  10, 12, 'r', { 2, 3}   , {28,0,0,0}		  ,   11, white},
{"Nasty Little Gnome"	      ,0x13820002L,0x000020B5L,0x2034,   32,  10,  18,  10, 11, 'p', { 4, 8}   , {4,0,0,0}			,   11, blue},
{"Hobgoblin"		            ,0x0F020002L,0x00000000L,0x2034,   38,  30,  20,  38, 11, 'H', {12, 8}   , {9,0,0,0}			,   11, green},
{"Black Mamba"		          ,0x00000012L,0x00000000L,0x00B2,   40,   1,  10,  32, 12, 'R', {10, 8}   , {163,0,0,0}		,   12, black},
{"Grape Jelly"		          ,0x00000001L,0x0001000BL,0x11A0,   60,  99,   2,	1,  11, 'J', {52, 8}   , {186,0,0,0}		,   12, lime},
{"Master Yeek"		          ,0x07020002L,0x00008018L,0x2030,   28,  10,  18,  24, 11, 'y', { 5, 8}   , {7,0,0,0}			,   12, pink},
{"Priest"		                ,0x13020002L,0x00000285L,0x2030,   36,  40,  20,  22, 11, 'p', { 7, 8}   , {12,0,0,0}		  ,   12, redDark},
{"Giant Clear Ant"	        ,0x00010002L,0x00000000L,0x0002,   24,  60,  12,  18, 11, 'a', { 3, 7}   , {27,0,0,0}		  ,   12, greyLight},
{"Air Spirit"		            ,0x00030022L,0x00000000L,0x1000,   40,  20,  12,  20, 13, 'E', { 5, 8}   , {2,0,0,0}			,   12, blueLight},
{"Skeleton Human"	          ,0x00020002L,0x00000000L,0x100C,   38,  30,  20,  30, 11, 's', {12, 8}   , {7,0,0,0}			,   12, white},
{"Human Zombie"		          ,0x00020002L,0x00000000L,0x102C,   34,  20,  20,  24, 11, 'z', {11, 8}   , {3,3,0,0}			,   12, greenZombie},
{"Moaning Spirit"	          ,0x0F15000AL,0x0001002FL,0x110C,   44,  10,  14,  20, 11, 'G', { 4, 8}   , {99,178,0,0}		,   12, cyanGhost},
{"Swordsman"		            ,0x13020002L,0x00000000L,0x2030,   40,  20,  20,  34, 11, 'p', {11, 8}   , {18,0,0,0}		  ,   12, redDark},
{"Killer Brown Beetle"	    ,0x0000000AL,0x00000000L,0x0002,   38,  30,  10,  40, 11, 'K', {13, 8}   , {41,0,0,0}		  ,   13, brown},
{"Ogre"			                ,0x07020002L,0x00000000L,0x2034,   42,  30,  20,  32, 11, 'o', {13, 8}   , {16,0,0,0}		  ,   13, pinkSkin},
{"Giant Red Speckled Frog"  ,0x0000000AL,0x00000000L,0x00A2,   32,  30,  12,  20, 11, 'f', { 6, 8}   , {41,0,0,0}		  ,   13, red},
{"Magic User"		            ,0x13020002L,0x00002413L,0x2030,   35,  10,  20,  10, 11, 'p', { 7, 8}   , {11,0,0,0}		  ,   13, redDark},
{"Black Orc"		            ,0x0B020002L,0x00000000L,0x2034,   40,  20,  20,  36, 11, 'o', {12, 8}   , {17,0,0,0}		  ,   13, black},
{"Giant Long-Eared Bat"	    ,0x00000012L,0x00000000L,0x2162,   20,  20,  12,  20, 13, 'b', { 5, 8}   , {27,50,50,0}		,   13, greyDark},
{"Giant Gnat"		            ,0x00200022L,0x00000000L,0x0062,    1,  10,   8,	4,  13, 'F', { 1, 2}   , {24,0,0,0}		  ,   13, grey},
{"Killer Green Beetle"	    ,0x0000000AL,0x00000000L,0x0002,   46,  30,  12,  45, 11, 'K', {16, 8}   , {43,0,0,0}		  ,   14, green},
{"Giant Flea"		            ,0x00200022L,0x00000000L,0x0062,    1,  10,   8,  25, 12, 'F', { 2, 2}   , {25,0,0,0}		  ,   14, grey},
{"Giant White Dragon Fly"   ,0x00000012L,0x0040000AL,0x0062,   54,  50,  20,  20, 11, 'F', { 5, 8}   , {122,0,0,0}		,   14, white},
{"Hill Giant"		            ,0x07020002L,0x00000000L,0x2034,   52,  50,  20,  36, 11, 'P', {16, 8}   , {19,0,0,0}		  ,   14, redDark},
{"Skeleton Hobgoblin"	      ,0x00020002L,0x00000000L,0x100C,   46,  30,  20,  34, 11, 's', {13, 8}   , {14,0,0,0}		  ,   14, white},
{"Flesh Golem"		          ,0x00000002L,0x00000000L,0x1030,   48,  10,  12,  10, 11, 'g', {12, 8}   , {5,5,0,0}			,   14, pinkSkin},
{"White Dragon Bat"	        ,0x00000012L,0x00400004L,0x0162,   40,  50,  12,  20, 13, 'b', { 2, 6}   , {121,0,0,0}		,   14, white},
{"Giant Black Louse"	      ,0x00200012L,0x00000000L,0x01F2,    1,  10,   6,	7,  12, 'l', { 1, 1}   , {25,0,0,0}		  ,   14, black},
{"Guardian Naga"	          ,0x1710000AL,0x00000000L,0x20E4,   60, 120,  20,  50, 11, 'n', {24, 8}   , {77,31,0,0}		,   15, gold},
{"Giant Grey Bat"	          ,0x00000012L,0x00000000L,0x2162,   22,  15,  12,  22, 13, 'b', { 4, 8}   , {29,50,50,0}		,   15, grey},
{"Giant Clear Centipede"    ,0x00010002L,0x00000000L,0x0002,   30,  30,  12,  30, 11, 'c', { 5, 8}   , {34,62,0,0}		,   15, greyLight},
{"Giant Yellow Tick"	      ,0x0000000AL,0x00000000L,0x0022,   48,  20,  12,  48, 10, 't', {20, 8}   , {162,0,0,0}		,   15, yellow},
{"Giant Ebony Ant"	        ,0x00200002L,0x00000000L,0x0002,    3,  60,  12,  24, 11, 'a', { 3, 4}   , {33,0,0,0}		  ,   15, black},
{"Frost Giant"		          ,0x07020002L,0x00400000L,0x0024,   54,  50,  20,  38, 11, 'P', {17, 8}   , {120,0,0,0}		,   15, whiteBlue},
{"Clay Golem"		            ,0x00000002L,0x00000000L,0x1200,   50,  10,  12,  20, 11, 'g', {14, 8}   , {7,7,0,0}			,   15, greyBrown},
{"Huge White Bat"	          ,0x00200012L,0x00000000L,0x2162,    3,  40,   7,  12, 12, 'b', { 3, 8}   , {29,0,0,0}		  ,   15, white},
{"Giant Tan Bat"	          ,0x00000012L,0x00000000L,0x2162,   18,  40,  12,  18, 12, 'b', { 3, 8}   , {95,49,49,0}		,   15, tanColor},
{"Violet Mold"		          ,0x00000001L,0x00010009L,0x10A0,   50,  70,   2,  15, 11, 'm', {17, 8}   , {145,0,0,0}		,   15, violet},
{"Umber Hulk"		            ,0x00020002L,0x00000000L,0x2124,   75,  10,  20,  20, 11, 'U', {20, 8}   , {92,5,5,36}		,   16, brownLight},
{"Gelatinous Cube"	        ,0x2F98000AL,0x00200000L,0x1020,   36,   1,  12,  18, 10, 'C', {45, 8}   , {115,0,0,0}		,   16, cyan,},
{"Giant Black Rat"	        ,0x0020000AL,0x00000000L,0x2072,    3,  20,   8,  16, 11, 'r', { 3, 4}   , {155,0,0,0}		,   16, black},
{"Giant Green Dragon Fly"   ,0x00000012L,0x0010000AL,0x0032,   58,  50,  20,  20, 11, 'F', { 5, 8}   , {156,0,0,0}		,   16, green, },
{"Fire Giant"		            ,0x07020002L,0x00800000L,0x2014,   62,  50,  20,  40, 11, 'P', {20, 8}   , {102,0,0,0}		,   16, fireColor},
{"Green Dragon Bat"	        ,0x00000012L,0x00100004L,0x2112,   44,  50,  12,  22, 13, 'b', { 2, 7}   , {153,0,0,0}		,   16, green},
{"Quasit"		                ,0x1183000AL,0x000010FAL,0x1004,   48,  20,  20,  30, 11, 'q', { 5, 8}   , {176,51,51,0}	,   16, green},
{"Troll"		                ,0x0F020002L,0x00000000L,0x2024,   64,  40,  20,  40, 11, 'T', {17, 8}   , {3,3,29,0}		  ,   17, green},
{"Water Spirit"		          ,0x0000000AL,0x00000000L,0x1020,   58,  40,  12,  28, 12, 'E', { 8, 8}   , {13,0,0,0}		  ,   17, blueLight},
{"Giant Brown Scorpion"	    ,0x0000000AL,0x00000000L,0x0002,   62,  20,  12,  44, 11, 'S', {11, 8}   , {34,86,0,0}		,   17, brown},
{"Earth Spirit"		          ,0x0016000AL,0x00000000L,0x1200,   64,  50,  10,  40, 11, 'E', {13, 8}   , {7,7,0,0}			,   17, brown},
{"Fire Spirit"		          ,0x0000000AL,0x00800000L,0x3010,   66,  20,  16,  30, 12, 'E', {10, 8}   , {101,0,0,0}		,   18, fireColor},
{"Uruk-Hai Orc"		          ,0x0B020002L,0x00000000L,0x2034,   68,  20,  20,  42, 11, 'o', {14, 8}   , {18,0,0,0}		  ,   18, white},
{"Stone Giant"		          ,0x07020002L,0x00000000L,0x2204,   80,  50,  20,  40, 11, 'P', {22, 8}   , {20,0,0,0}		  ,   18, greyPink},
{"Stone Golem"		          ,0x00000002L,0x00000000L,0x1200,  100,  10,  12,  75, 10, 'g', {28, 8}   , {9,9,0,0}			,   19, grey},
{"Grey Ooze"		            ,0x07980022L,0x00400000L,0x10A0,   40,   1,  15,  10, 11, 'O', { 6, 8}   , {127,0,0,0}		,   19, grey},
{"Disenchanter Ooze"	      ,0x07980022L,0x00000000L,0x10B0,   50,   1,  15,  15, 11, 'O', { 6, 8}   , {205,0,0,0}		,   19, purple},
{"Giant Spotted Rat"	      ,0x0020000AL,0x00000000L,0x2072,    3,  20,   8,  20, 11, 'r', { 4, 3}   , {155,0,0,0}		,   19, grey},
{"Mummified Kobold"	        ,0x0B820002L,0x00000000L,0x102C,   46,  75,  20,  24, 11, 'M', {13, 8}   , {5,5,0,0}			,   19, whiteMummy},
{"Killer Black Beetle"	    ,0x0000000AL,0x00000000L,0x0002,   75,  30,  12,  46, 11, 'K', {18, 8}   , {44,0,0,0}		  ,   19, black},
{"Red Mold"		              ,0x00000001L,0x00800000L,0x3090,   64,  70,   2,  16, 11, 'm', {17, 8}   , {108,0,0,0}		,   19, red},
{"Quylthulg"		            ,0x00010004L,0x00002017L,0x5000,  200,   0,  10,	1,  11, 'Q', { 4, 8}   , {0,0,0,0}			,   20, pinkSkin},
{"Giant Red Bat"	          ,0x00000012L,0x00000000L,0x2162,   40,  20,  12,  24, 12, 'b', { 5, 8}   , {30,51,51,0}		,   20, red},
{"Giant Black Dragon Fly"   ,0x00000012L,0x00200009L,0x0072,   58,  50,  20,  22, 11, 'F', { 4, 8}   , {141,0,0,0}		,   20, black},
{"Cloud Giant"		          ,0x07020002L,0x00080000L,0x2034,  125,  50,  20,  44, 11, 'P', {24, 8}   , {130,0,0,0}		,   20, whiteBlue},
{"Black Dragon Bat"	        ,0x00000012L,0x00200004L,0x2152,   50,  50,  12,  24, 13, 'b', { 2, 8}   , {112,0,0,0}		,   21, black},
{"Blue Dragon Bat"	        ,0x00000012L,0x00080004L,0x2052,   54,  50,  12,  26, 13, 'b', { 3, 6}   , {131,0,0,0}		,   21, blue},
{"Mummified Orc"	          ,0x0B020002L,0x00000000L,0x102C,   56,  75,  20,  28, 11, 'M', {14, 8}   , {13,13,0,0}		,   21, whiteMummy},
{"Killer Boring Beetle"	    ,0x0000000AL,0x00000000L,0x0002,   70,  30,  12,  48, 11, 'K', {18, 8}   , {44,0,0,0}		  ,   21, yellow},
{"Killer Stag Beetle"	      ,0x0000000AL,0x00000000L,0x0002,   80,  30,  12,  50, 11, 'K', {20, 8}   , {41,10,0,0}		,   22, red},
{"Black Mold"		            ,0x00000081L,0x00000000L,0x10A0,   68,  50,   2,  18, 11, 'm', {15, 8}   , {21,0,0,0}		  ,   22, black},
{"Iron Golem"		            ,0x00000002L,0x00000000L,0x1080,  160,  10,  12,  99,  9, 'g', {80, 8}   , {10,10,0,0}		,   22, greyBlue},
{"Giant Yellow Scorpion"    ,0x0000000AL,0x00000000L,0x0002,   60,  20,  12,  38, 11, 'S', {12, 8}   , {31,167,0,0}		,   22, yellow},
{"Green Ooze"		            ,0x07BA0012L,0x00200000L,0x1030,    6,   1,  15,	5,  10, 'O', { 4, 8}   , {116,0,0,0}		,   22, green},
{"Black Ooze"		            ,0x07BA0012L,0x0001000BL,0x1030,    7,   1,  10,	6,   9, 'O', { 6, 8}   , {138,0,0,0}		,   23, black},
{"Warrior"		              ,0x13020002L,0x00000000L,0x2030,   60,  40,  20,  40, 11, 'p', {15, 8}   , {18,0,0,0}		  ,   23, redDark},
{"Red Dragon Bat"	          ,0x00000012L,0x00800004L,0x2152,   60,  50,  12,  28, 13, 'b', { 3, 8}   , {105,0,0,0}		,   23, red},
{"Killer Blue Beetle"	      ,0x0000000AL,0x00000000L,0x0002,   85,  30,  14,  50, 11, 'K', {20, 8}   , {44,0,0,0}		  ,   23, blue},
{"Giant Silver Ant"	        ,0x0000000AL,0x00200000L,0x0002,   45,  60,  10,  38, 11, 'a', { 6, 8}   , {114,0,0,0}		,   23, silver},
{"Crimson Mold"		          ,0x00000001L,0x00000000L,0x10A0,   65,  50,   2,  18, 11, 'm', {16, 8}   , {2,97,0,0}		  ,   23, crimson},
{"Forest Wight"		          ,0x0F02000AL,0x0000100FL,0x112C,  140,  30,  20,  30, 11, 'W', {12, 8}   , {5,5,187,0}		,   24, whiteGreen},
{"Berzerker"		            ,0x13020002L,0x00000000L,0x2030,   65,  10,  20,  20, 11, 'p', {15, 8}   , {7,7,0,0}			,   24, red},
{"Mummified Human"	        ,0x0B020002L,0x00000000L,0x102C,   70,  60,  20,  34, 11, 'M', {17, 8}   , {13,13,0,0}		,   24, whiteMummy},
{"Banshee"		              ,0x0F15001AL,0x0001002FL,0x110C,   60,  10,  20,  24, 12, 'G', { 6, 8}   , {99,188,0,0}		,   24, whiteBlue},
{"Giant Troll"		          ,0x0F020002L,0x00000000L,0x2024,   85,  50,  20,  40, 11, 'T', {19, 8}   , {5,5,41,0}		  ,   25, green},
{"Giant Brown Tick"	        ,0x0000000AL,0x00000000L,0x0022,   70,  20,  12,  50, 10, 't', {18, 8}   , {157,142,0,0}	,   25, brown},
{"Killer Red Beetle"	      ,0x0000000AL,0x00000000L,0x0002,   85,  30,  14,  50, 11, 'K', {20, 8}   , {84,0,0,0}		  ,   25, red},
{"Wooden Mold"		          ,0x00000001L,0x00000000L,0x10A0,  100,  50,   2,  50, 11, 'm', {25, 8}   , {171,0,0,0}		,   25, brown},
{"Giant Blue Dragon Fly"    ,0x00000012L,0x00080009L,0x0072,   75,  50,  20,  24, 11, 'F', { 6, 8}   , {29,0,0,0}		  ,   25, blue},
{"Giant Grey Ant Lion"	    ,0x0008000AL,0x00000000L,0x0032,   90,  40,  10,  40, 11, 'A', {19, 8}   , {39,0,0,0}		  ,   26, grey},
{"Disenchanter Bat"	        ,0x00000012L,0x00000000L,0x2162,   75,   1,  14,  24, 13, 'b', { 4, 8}   , {204,0,0,0}		,   26, purple},
{"Giant Fire Tick"	        ,0x0000000AL,0x00800000L,0x2012,   90,  20,  14,  54, 11, 't', {16, 8}   , {109,0,0,0}		,   26, fireColor},
{"White Wraith"		          ,0x0F02000AL,0x0000100CL,0x112C,  165,  10,  20,  40, 11, 'W', {15, 8}   , {5,5,189,0}		,   26, white},
{"Giant Black Scorpion"	    ,0x0000000AL,0x00000000L,0x0002,   85,  20,  12,  50, 11, 'S', {13, 8}   , {32,167,0,0}		,   26, black},
{"Clear Ooze"		            ,0x0799000AL,0x00000000L,0x10B0,   12,   1,  10,  14, 11, 'O', { 4, 8}   , {90,0,0,0}		  ,   26, greyLight},
{"Killer Fire Beetle"	      ,0x0000000AL,0x00800000L,0x2012,   95,  30,  14,  45, 11, 'K', {13, 8}   , {41,110,0,0}		,   27, fireColor},
{"Vampire"		              ,0x17020002L,0x00001209L,0x112C,  175,  10,  20,  45, 11, 'V', {20, 8}   , {5,5,190,0}		,   27, redGlow},
{"Giant Red Dragon Fly"	    ,0x00000012L,0x00800008L,0x2052,   75,  50,  20,  24, 11, 'F', { 7, 8}   , {96,0,0,0}		  ,   27, red},
{"Shimmering Mold"	        ,0x00000081L,0x00080000L,0x10A0,  180,  50,   2,  24, 11, 'm', {32, 8}   , {135,0,0,0}		,   27, blueLight},
{"Black Knight"		          ,0x13020002L,0x0000010FL,0x2034,  140,  10,  20,  60, 11, 'p', {25, 8}   , {23,0,0,0}		  ,   28, black},
{"Mage"			                ,0x13020002L,0x00002C73L,0x2030,  150,  10,  20,  30, 11, 'p', {10, 8}   , {14,0,0,0}		  ,   28, redDark},
{"Ice Troll"		            ,0x0F020002L,0x00400000L,0x0024,  160,  50,  20,  46, 11, 'T', {22, 8}   , {4,4,123,0}		,   28, whiteBlue},
{"Giant Purple Worm"	      ,0x0000000AL,0x00200000L,0x2032,  400,  30,  14,  65, 11, 'w', {65, 8}   , {7,113,166,0}	,   29, purple},
{"Young Blue Dragon"	      ,0x1F00000AL,0x0008100BL,0x2005,  300,  70,  20,  50, 11, 'd', {33, 8}   , {52,52,29,0}		,   29, blue},
{"Young White Dragon"	      ,0x1F00000AL,0x0040100BL,0x0025,  275,  70,  20,  50, 11, 'd', {32, 8}   , {52,52,29,0}		,   29, white},
{"Young Green Dragon"	      ,0x1F00000AL,0x0010100BL,0x2005,  290,  70,  20,  50, 11, 'd', {32, 8}   , {52,52,29,0}		,   29, green},
{"Giant Fire Bat"	          ,0x00000012L,0x00800000L,0x2152,   85,  10,  14,  30, 12, 'b', {5,  8}   , {106,52,52,0}	,   29, fireColor},
{"Giant Glowing Rat"	      ,0x0020000AL,0x00080000L,0x2072,    4,  20,   8,  24, 11, 'r', {3,  3}   , {132,0,0,0}		,   29, orangeGlow},
	/* Now things are going to get tough.			 */
	/* Some of the creatures have Max hit points, denoted in */
	/* their CDEFENSE flags as the '4000' bit set		 */
{"Skeleton Troll"	          ,0x00020002L,0x00000000L,0x500C,  225,  20,  20,  55, 11, 's', {14, 8}  , {5,5,41,0}		  ,   30, white},
{"Giant Lightning Bat"	    ,0x00000012L,0x00080000L,0x2042,   80,  10,  15,  34, 12, 'b', {8,  8}  , {133,53,53,0}	  ,   30, blueLight},
{"Giant Static Ant"	        ,0x0000000AL,0x00080000L,0x0002,   80,  60,  10,  40, 11, 'a', {8,  8}  , {134,0,0,0}		  ,   30, blueLight},
{"Grave Wight"		          ,0x0F02000AL,0x0000110AL,0x512C,  325,  30,  20,  35, 11, 'W', {12, 8}  , {6,6,191,0}		  ,   30, whiteGreen},
{"Killer Slicer Beetle"	    ,0x0000000AL,0x00000000L,0x0002,  200,  30,  14,  55, 11, 'K', {22, 8}  , {48,0,0,0}		  ,   30, orange},
{"Giant White Ant Lion"	    ,0x0008000AL,0x00400000L,0x0022,  175,  40,  12,  45, 11, 'A', {20, 8}  , {124,0,0,0}		  ,   30, white},
{"Ghost"		                ,0x1715000AL,0x0001002FL,0x510C,  350,  10,  20,  30, 12, 'G', {13, 8}  , {99,192,184,0}  ,   31, white},
{"Giant Black Ant Lion"	    ,0x0008000AL,0x00200000L,0x0032,  170,  40,  14,  45, 11, 'A', {23, 8}  , {39,119,0,0}	  ,   31, black},
{"Death Watch Beetle"	      ,0x0000000AL,0x00000000L,0x0002,  190,  30,  16,  60, 11, 'K', {25, 8}  , {47,67,0,0}		  ,   31, brown},
{"Ogre Mage"		            ,0x0B020002L,0x0000A355L,0x6034,  250,  30,  20,  42, 11, 'o', {14, 8}  , {19,0,0,0}		  ,   31, pinkSkin},
{"Two-Headed Troll"	        ,0x0F020002L,0x00000000L,0x6024,  275,  50,  20,  48, 11, 'T', {14, 8}  , {7,7,29,29}		  ,   32, green},
{"Invisible Stalker"	      ,0x00030022L,0x00000000L,0x1000,  200,  20,  20,  46, 13, 'E', {19, 8}  , {5,0,0,0}			  ,   32, greyGreen},
{"Giant Hunter Ant"	        ,0x00000002L,0x00000000L,0x0002,  150,   1,  16,  40, 11, 'a', {12, 8}  , {46,0,0,0}		  ,   32, yellow},
{"Ninja"		                ,0x13020002L,0x00000000L,0x6034,  300,  10,  20,  65, 11, 'p', {15, 8}  , {152,80,0,0}	  ,   32, black},
{"Barrow Wight"		          ,0x0F02000AL,0x00001308L,0x512C,  375,  10,  20,  40, 11, 'W', {13, 8}  , {7,7,193,0}		  ,   33, whiteCyan},
{"Skeleton 2-Headed Troll"  ,0x00020002L,0x00000000L,0x500C,  325,  20,  20,  48, 11, 's', {20, 8}  , {8,8,28,28}		  ,   33, white},
{"Water Elemental"	        ,0x0008000AL,0x00000000L,0x1020,  325,  50,  12,  36, 11, 'E', {25, 8}  , {9,9,0,0}			  ,   33, blueLight},
{"Fire Elemental"	          ,0x0008000AL,0x00800000L,0x3010,  350,  70,  16,  40, 10, 'E', {25, 8}  , {103,0,0,0}		  ,   33, fireColor},
{"Lich"			                ,0x1F020002L,0x00019F75L,0x510C,  750,  60,  20,  50, 11, 'L', {25, 8}  , {179,194,214,0} ,   34, greyCyan},
{"Master Vampire"	          ,0x17020002L,0x00001307L,0x512C,  700,  10,  20,  55, 11, 'V', {23, 8}  , {5,5,195,0}		  ,   34, redGlow},
{"Spirit Troll"		          ,0x17150002L,0x00000000L,0x510C,  425,  10,  20,  40, 11, 'G', {15, 8}  , {53,53,29,185}	,   34, blueDark},
{"Giant Red Scorpion"	      ,0x0000000AL,0x00000000L,0x0002,  275,  40,  12,  50, 12, 'S', {18, 8}  , {29,165,0,0}		,   34, red},
{"Earth Elemental"	        ,0x001E000AL,0x00000000L,0x1200,  375,  90,  10,  60, 10, 'E', {30, 8}  , {22,22,0,0}		  ,   34, brown},
{"Young Black Dragon"	      ,0x1F00000AL,0x0020100BL,0x6005,  600,  50,  20,  55, 11, 'd', {32, 8}  , {53,53,29,0}		,   35, black},
{"Young Red Dragon"	        ,0x1F00000AL,0x0080100AL,0x6015,  650,  50,  20,  60, 11, 'd', {36, 8}  , {54,54,37,0}		,   35, red},
{"Necromancer"		          ,0x13020002L,0x00005763L,0x6030,  600,  10,  20,  40, 11, 'p', {17, 8}  , {15,0,0,0}		  ,   35, purple},
{"Mummified Troll"	        ,0x0F020002L,0x00000000L,0x502C,  400,  50,  20,  38, 11, 'M', {18, 8}  , {15,15,0,0}		  ,   35, whiteMummy},
{"Giant Red Ant Lion"	      ,0x0008000AL,0x00800000L,0x2012,  350,  40,  14,  48, 11, 'A', {23, 8}  , {107,0,0,0}		  ,   35, red},
{"Mature White Dragon"	    ,0x2F00000AL,0x0040100AL,0x4025, 1000,  70,  20,  65, 11, 'd', {48, 8}  , {54,54,37,0}		,   35, white},
{"Xorn"			                ,0x00160002L,0x00000000L,0x4200,  650,  10,  20,  80, 11, 'X', {20, 8}  , {5,5,5,0}			  ,   36, greenGrey},
{"Giant Mottled Ant Lion"   ,0x0008000AL,0x00000000L,0x0032,  350,  40,  14,  50, 12, 'A', {24, 8}  , {38,0,0,0}		  ,   36, yellowGreen},
{"Grey Wraith"		          ,0x0F02000AL,0x00001308L,0x512C,  700,  10,  20,  50, 11, 'W', {23, 8}  , {9,9,196,0}		  ,   36, grey},
{"Young Multi-Hued Dragon"  ,0x4F00000AL,0x00F81005L,0x6005, 1250,  50,  20,  55, 11, 'd', {40, 8}  , {55,55,38,0}		,   36, pink},
{"Mature Blue Dragon"	      ,0x2F00000AL,0x00081009L,0x6005, 1200,  70,  20,  75, 11, 'd', {48, 8}  , {54,54,38,0}		,   36, blue},
{"Mature Green Dragon"	    ,0x2F00000AL,0x0010100AL,0x6005, 1100,  70,  20,  70, 11, 'd', {48, 8}  , {52,52,29,0}		,   36, green},
{"Iridescent Beetle"	      ,0x0000000AL,0x00000000L,0x0002,  850,  30,  16,  60, 11, 'K', {32, 8}  , {45,10,146,0}		,   37, pink},
{"King Vampire"		          ,0x17020002L,0x00001307L,0x512C, 1000,  10,  20,  65, 11, 'V', {38, 8}  , {5,5,198,0}		  ,   37, redGlow},
{"King Lich"		            ,0x1F020002L,0x00019F73L,0x510C, 1400,  50,  20,  65, 11, 'L', {52, 8}  , {180,197,214,0}	,   37, greyCyan},
{"Mature Red Dragon"	      ,0x2F00000AL,0x00801808L,0x6015, 1400,  30,  20,  80, 11, 'd', {60, 8}  , {56,56,39,0}		,   37, red},
{"Mature Black Dragon"	    ,0x2F00000AL,0x00201009L,0x6005, 1350,  30,  20,  55, 11, 'd', {58, 8}  , {54,54,38,0}		,   37, black},
{"Mature Multi-Hued Dragon" ,0x6F00000AL,0x00F81A05L,0x6005, 1650,  50,  20,  65, 11, 'd', {80, 8}  , {56,56,39,0}		,   38, pink},
{"Ancient White Dragon"	    ,0x4F000002L,0x00401A09L,0x4025, 1500,  80,  20,  80, 12, 'D', {88, 8}  , {54,54,37,0}		,   38, white},
{"Emperor Wight"	          ,0x1B02000AL,0x00001306L,0x512C, 1600,  10,  20,  40, 12, 'W', {48, 8}  , {10,10,199,0}		,   38, greyGreen},
{"Black Wraith"		          ,0x1F02000AL,0x00001307L,0x512C, 1700,  10,  20,  55, 11, 'W', {50, 8}  , {10,10,200,0}		,   38, black},
{"Nether Wraith"	          ,0x1F07000AL,0x00005316L,0x512C, 2100,  10,  20,  55, 11, 'W', {58, 8}  , {10,10,202,0}		,   39, purpleBlack},
{"Sorcerer"		              ,0x1F020002L,0x0000FF73L,0x6030, 2150,  10,  20,  50, 12, 'p', {30, 8}  , {16,0,0,0}		  ,   39, purpleRed},
{"Ancient Blue Dragon"	    ,0x4F000002L,0x00081A08L,0x6005, 2500,  80,  20,  90, 12, 'D', {87, 8}  , {55,55,39,0}		,   39, blue},
{"Ancient Green Dragon"	    ,0x4F000002L,0x00101A09L,0x6005, 2400,  80,  20,  85, 12, 'D', {90, 8}  , {54,54,38,0}		,   39, green},
{"Ancient Black Dragon"	    ,0x4F000002L,0x00201A07L,0x6005, 2500,  70,  20,  90, 12, 'D', {90, 8}  , {55,55,38,0}		,   39, black},
{"Crystal Ooze"		          ,0x07BB000AL,0x00400000L,0x10A0,    8,   1,  10,  30,  9, 'O', {12, 8}  , {128,0,0,0}		  ,   40, blueLight},
{"Disenchanter Worm"	      ,0x00200022L,0x00000000L,0x01B2,   30,  10,   7,	5,  10, 'w', {10, 8}  , {208,0,0,0}		  ,   40, purple},
{"Rotting Quylthulg"	      ,0x00010004L,0x00004014L,0x5000, 1000,   0,  20,	1,  12, 'Q', {12, 8}  , {0,0,0,0}			  ,   40, pinkFlesh},
{"Ancient Red Dragon"	      ,0x6F000002L,0x00801E06L,0x6015, 2750,  70,  20, 100, 12, 'D', {105,8}  , {56,56,40,0}		,   40, red},
{"Death Quasit"		          ,0x1103000AL,0x000010FAL,0x1004, 1000,   0,  20,  80, 13, 'q', {55, 8}  , {177,58,58,0}		,   40, pinkFlesh},
{"Emperor Lich"		          ,0x2F020002L,0x00019F72L,0x510C,10000,  50,  20,  75, 12, 'L', {38,40}  , {181,201,214,0}	,   40, greyCyan},
{"Ancient Multi-Hued Dragon",0x7F000002L,0x00F89E05L,0x6005,12000,  70,  20, 100, 12, 'D', {52,40}  , {57,57,42,0}		,   40, pink},
	/* Winning creatures should follow here.			 */
	/* Winning creatures are denoted by the 32 bit in CMOVE		 */
	/* Iggy is not a win creature, just a royal pain in the ass.	 */
{"Evil Iggy"		            ,0x7F130002L,0x0001D713L,0x5004,18000,   0,  30,  80, 12, 'p', {60,40} , {81,150,0,0}		  ,   50, blue},
	/* Here is the only actual win creature.			 */
{"Balrog"		                ,0xFF1F0002L,0x0081C743L,0x5004,55000L,  0,  40, 125, 13, 'B', {75,40} , {104,78,214,0}		,  100, red}
};

/* ERROR: attack #35 is no longer used */
struct m_attack_type monster_attacks[N_MONS_ATTS] = {
/* 0 */	{0, 0, 0, 0},	{1, 1, 1, 2},	{1, 1, 1, 3},	{1, 1, 1, 4},
	{1, 1, 1, 5},	{1, 1, 1, 6},	{1, 1, 1, 7},	{1, 1, 1, 8},
	{1, 1, 1, 9},	{1, 1, 1, 10},	{1, 1, 1, 12},	{1, 1, 2, 2},
	{1, 1, 2, 3},	{1, 1, 2, 4},	{1, 1, 2, 5},	{1, 1, 2, 6},
	{1, 1, 2, 8},	{1, 1, 3, 4},	{1, 1, 3, 5},	{1, 1, 3, 6},
/* 20 */{1, 1, 3, 8},	{1, 1, 4, 3},	{1, 1, 4, 6},	{1, 1, 5, 5},
	{1, 2, 1, 1},	{1, 2, 1, 2},	{1, 2, 1, 3},	{1, 2, 1, 4},
	{1, 2, 1, 5},	{1, 2, 1, 6},	{1, 2, 1, 7},	{1, 2, 1, 8},
	{1, 2, 1, 10},	{1, 2, 2, 3},	{1, 2, 2, 4},	{1, 2, 2, 5},
	{1, 2, 2, 6},	{1, 2, 2, 8},	{1, 2, 2, 10},	{1, 2, 2, 12},
/* 40 */{1, 2, 2, 14},	{1, 2, 3, 4},	{1, 2, 3, 12},	{1, 2, 4, 4},
	{1, 2, 4, 5},	{1, 2, 4, 6},	{1, 2, 4, 8},	{1, 2, 5, 4},
	{1, 2, 5, 8},	{1, 3, 1, 1},	{1, 3, 1, 2},	{1, 3, 1, 3},
	{1, 3, 1, 4},	{1, 3, 1, 5},	{1, 3, 1, 8},	{1, 3, 1, 9},
	{1, 3, 1, 10},	{1, 3, 1, 12},	{1, 3, 3, 3},	{1, 4, 1, 2},
/* 60 */{1, 4, 1, 3},	{1, 4, 1, 4},	{1, 4, 2, 4},	{1, 5, 1, 2},
	{1, 5, 1, 3},	{1, 5, 1, 4},	{1, 5, 1, 5},	{1, 10, 5, 6},
	{1, 12, 1, 1},	{1, 12, 1, 2},	{1, 13, 1, 1},	{1, 13, 1, 3},
	{1, 14, 0, 0},	{1, 16, 1, 4},	{1, 16, 1, 6},	{1, 16, 1, 8},
	{1, 16, 1, 10},	{1, 16, 2, 8},	{1, 17, 8, 12},	{1, 18, 0, 0},
/* 80 */{2, 1, 3, 4},	{2, 1, 4, 6},	{2, 2, 1, 4},	{2, 2, 2, 4},
	{2, 2, 4, 4},	{2, 4, 1, 4},	{2, 4, 1, 7},	{2, 5, 1, 5},
	{2, 7, 1, 6},	{3, 1, 1, 4},	{3, 5, 1, 8},	{3, 13, 1, 4},
	{3, 7, 0, 0},	{4, 1, 1, 1},	{4, 1, 1, 4},	{4, 2, 1, 2},
	{4, 2, 1, 6},	{4, 5, 0, 0},	{4, 7, 0, 0},	{4, 10, 0, 0},
/*100 */{4, 13, 1, 6},	{5, 1, 2, 6},	{5, 1, 3, 7},	{5, 1, 4, 6},
	{5, 1, 10, 12},	{5, 2, 1, 3},	{5, 2, 3, 6},	{5, 2, 3, 12},
	{5, 5, 4, 4},	{5, 9, 3, 7},	{5, 9, 4, 5},	{5, 12, 1, 6},
	{6, 2, 1, 3},	{6, 2, 2, 8},	{6, 2, 4, 4},	{6, 5, 1, 10},
	{6, 5, 2, 3},	{6, 8, 1, 5},	{6, 9, 2, 6},	{6, 9, 3, 6},
/*120 */{7, 1, 3, 6},	{7, 2, 1, 3},	{7, 2, 1, 6},	{7, 2, 3, 6},
	{7, 2, 3, 10},	{7, 5, 1, 6},	{7, 5, 2, 3},	{7, 5, 2, 6},
	{7, 5, 4, 4},	{7, 12, 1, 4},	{8, 1, 3, 8},	{8, 2, 1, 3},
	{8, 2, 2, 6},	{8, 2, 3, 8},	{8, 2, 5, 5},	{8, 5, 5, 4},
	{9, 5, 1, 2},	{9, 5, 2, 5},	{9, 5, 2, 6},	{9, 8, 2, 4},
/*140 */{9, 12, 1, 3},	{10, 2, 1, 6},	{10, 4, 1, 1},	{10, 7, 2, 6},
	{10, 9, 1, 2},	{11, 1, 1, 2},	{11, 7, 0, 0},	{11, 13, 2, 4},
	{12, 5, 0, 0},	{13, 5, 0, 0},	{13, 19, 0, 0},	{14, 1, 1, 3},
	{14, 1, 3, 4},	{14, 2, 1, 3},	{14, 2, 1, 4},	{14, 2, 1, 5},
	{14, 2, 1, 6},	{14, 2, 1, 10},	{14, 2, 2, 4},	{14, 2, 2, 5},
/*160 */{14, 2, 2, 6},	{14, 2, 3, 4},	{14, 2, 3, 9},	{14, 2, 4, 4},
	{14, 4, 1, 2},	{14, 4, 1, 4},	{14, 4, 1, 8},	{14, 4, 2, 5},
	{14, 5, 1, 2},	{14, 5, 1, 3},	{14, 5, 2, 4},	{14, 5, 2, 6},
	{14, 5, 3, 5},	{14, 12, 1, 2},	{14, 12, 1, 4},	{14, 13, 2, 4},
	{15, 2, 1, 6},	{15, 2, 3, 6},	{15, 5, 1, 8},	{15, 5, 2, 8},
/*180 */{15, 5, 2, 10},	{15, 5, 2, 12},	{15, 12, 1, 3},	{16, 13, 1, 2},
	{17, 3, 1, 10},	{18, 5, 0, 0},	{19, 5, 5, 8},	{19, 5, 12, 8},
	{19, 5, 14, 8},	{19, 5, 15, 8},	{19, 5, 18, 8},	{19, 5, 20, 8},
	{19, 5, 22, 8},	{19, 5, 26, 8},	{19, 5, 30, 8},	{19, 5, 32, 8},
	{19, 5, 34, 8},	{19, 5, 36, 8},	{19, 5, 38, 8},	{19, 5, 42, 8},
/*200 */{19, 5, 44, 8},	{19, 5, 46, 8},	{19, 5, 52, 8},	{20, 10, 0, 0},
	{21, 1, 0, 0},	{21, 5, 0, 0},	{21, 5, 1, 6},	{21, 7, 0, 0},
	{21, 12, 1, 4},	{22, 5, 2, 3},	{22, 12, 0, 0},	{22, 15, 1, 1},
/*212 */{23, 1, 1, 1},	{23, 5, 1, 3},	{24, 5, 0, 0}
};


monster_type m_list[MAX_MALLOC];
int16 m_level[MAX_MONS_LEVEL+1];

/* Blank monster values	*/
monster_type blank_monster = {0,0,0,0,0,0,0,FALSE,0,FALSE};
int16 mfptr;			/* Cur free monster ptr	*/
int16 mon_tot_mult;		/* # of repro's of creature	*/
