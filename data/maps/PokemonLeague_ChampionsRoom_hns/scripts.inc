PokemonLeague_ChampionsRoom_hns_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, ChampionRoom_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_ChampionsRoom_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonLeague_ChampionsRoom_OnTransition
	.byte 0

PokemonLeague_ChampionsRoom_OnResume::
	setvar VAR_0x8004, 4
	special DoPokemonLeagueLightingEffect
	end

ChampionRoom_OnFrame::
	map_script_2 VAR_TEMP_0, 0, ChampionRoom_CloseDoor
	.2byte 0

ChampionRoom_CloseDoor::
	lock
	setflag FLAG_TEMP_2
	special DrawWholeMapView
	applymovement OBJ_EVENT_ID_PLAYER, ChamionRoom_Movement_Enter
	waitmovement 0
	setvar VAR_TEMP_0, 1
	release
	end

ChamionRoom_Movement_Enter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

ChampionRoom_Trigger::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkUp1
	waitmovement 0
	release
	end

PokemonLeague_WillsRoom_EventScript_Lance::
	lockall
	call_if_eq VAR_FACING, DIR_EAST, PokemonLeague_ChampionsRoom_EventScript_AdjustPlayerFacingEast
	call_if_eq VAR_FACING, DIR_SOUTH, PokemonLeague_ChampionsRoom_EventScript_AdjustPlayerFacingSouth
	call_if_eq VAR_FACING, DIR_WEST, PokemonLeague_ChampionsRoom_EventScript_AdjustPlayerFacingWest
	faceplayer
	goto_if_set FLAG_IS_CHAMPION, PokemonLeague_WillsRoom_EventScript_LanceRematch
	msgbox PokemonLeague_ChampionsRoom_Text_BattleIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_LANCE_1_HNS, PokemonLeague_ChampionsRoom_Text_BattleWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatJohto

PokemonLeague_WillsRoom_EventScript_LanceRematch::
	@ KANTO LEAGUE rematches rotate the champion: Lance -> Steven -> the partner. The first KANTO LEAGUE run is the partner.
	call ChampionRotation_Select
	goto_if_eq VAR_RESULT, 1, ChampionRotation_Steven
	goto_if_eq VAR_RESULT, 2, ChampionRotation_Partner
	goto_if_unset FLAG_IS_KANTO_CHAMPION, PokemonLeague_WillsRoom_EventScript_LanceRematchStandard
	goto_if_ge VAR_SINJOH_STORYLINE, 13, PokemonLeague_WillsRoom_EventScript_LanceRematchPostOBC

PokemonLeague_WillsRoom_EventScript_LanceRematchStandard::
	msgbox PokemonLeague_ChampionsRoom_Text_RematchBattleIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_LANCE_2_HNS, PokemonLeague_ChampionsRoom_Text_BattleWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

PokemonLeague_WillsRoom_EventScript_LanceRematchPostOBC::
	msgbox PokemonLeague_ChampionsRoom_Text_RematchBattleIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_LANCE_POSTOBC_HNS, PokemonLeague_ChampionsRoom_Text_BattleWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

PokemonLeague_WillsRoom_EventScript_LanceOnDefeatJohto::
	clearflag FLAG_TEMP_5
	call ChampionRotation_AfterText
	closemessage
	setmetatile 6, 1, 0x3F2, FALSE
	setmetatile 6, 2, 0x296, FALSE	
	playse SE_BANG
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 4  @ num shakes
	setvar VAR_0x8007, 4  @ shake delay
	special ShakeCamera
	waitstate
	special DrawWholeMapView
	playse SE_PIN
	applymovement LOCALID_LEAGUE_LANCE, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LEAGUE_LANCE
	turnobject OBJ_EVENT_ID_PLAYER, DIR_SOUTH
	applymovement OBJ_EVENT_ID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerMoveToCenter
	applymovement LOCALID_LEAGUE_LANCE, PokemonLeague_ChampionsRoom_Movement_LanceMoveOver
	waitmovement OBJ_EVENT_ID_PLAYER
	waitmovement LOCALID_LEAGUE_LANCE
	addobject LOCALID_LEAGUE_MARY
	addobject LOCALID_LEAGUE_OAK
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryEnter
	waitmovement 0
	msgbox PokemonLeague_ChampionsRoom_Text_MaryOhNoOak, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LEAGUE_OAK, PokemonLeague_ChampionsRoom_Movement_OakEnter
	delay 60
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryMoveOver
	waitmovement LOCALID_LEAGUE_MARY
	waitmovement LOCALID_LEAGUE_OAK
	msgbox PokemonLeague_ChampionsRoom_Text_OakCongratulations, MSGBOX_DEFAULT
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryInterview
	waitmovement 0
	msgbox PokemonLeague_ChampionsRoom_Text_MaryInterview, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LEAGUE_LANCE, PokemonLeague_ChampionsRoom_Movement_LanceAnxious
	waitmovement 0
	call ChampionRotation_NoisyText
	closemessage
	applymovement LOCALID_LEAGUE_LANCE, PokemonLeague_ChampionsRoom_Movement_LanceLeave
	delay 30
	applymovement OBJ_EVENT_ID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerLeave
	waitmovement LOCALID_LEAGUE_LANCE
	@setobjectxyperm LOCALID_LEAGUE_LANCE, 6, 14
	removeobject LOCALID_LEAGUE_LANCE
	playse SE_EXIT
	waitmovement OBJ_EVENT_ID_PLAYER
	special SpawnCameraObject
	removeobject OBJ_EVENT_ID_PLAYER
	playse SE_EXIT
	waitse
	delay 50
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryLeft
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_LEAGUE_MARY, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LEAGUE_MARY
	msgbox PokemonLeague_ChampionsRoom_Text_MaryNoInterview, MSGBOX_DEFAULT
	closemessage
	setvar VAR_LEAGUE_STATE, 6
	warpsilent MAP_POKEMON_LEAGUE_HALL_OF_FAME_HNS, 0
	waitstate
	releaseall
	end

PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto::
	clearflag FLAG_TEMP_5
	call ChampionRotation_Advance
	call ChampionRotation_AfterText
	closemessage
	setmetatile 6, 1, 0x3F2, FALSE
	setmetatile 6, 2, 0x296, FALSE	
	playse SE_BANG
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 4  @ num shakes
	setvar VAR_0x8007, 4  @ shake delay
	special ShakeCamera
	waitstate
	special DrawWholeMapView
	playse SE_PIN
	applymovement LOCALID_LEAGUE_LANCE, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LEAGUE_LANCE
	turnobject OBJ_EVENT_ID_PLAYER, DIR_SOUTH
	applymovement OBJ_EVENT_ID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerMoveToCenter
	applymovement LOCALID_LEAGUE_LANCE, PokemonLeague_ChampionsRoom_Movement_LanceMoveOver
	waitmovement OBJ_EVENT_ID_PLAYER
	waitmovement LOCALID_LEAGUE_LANCE
	addobject LOCALID_LEAGUE_MARY
	addobject LOCALID_LEAGUE_OAK
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryEnter
	waitmovement 0
	msgbox PokemonLeague_ChampionsRoom_Text_MaryOhNoOak, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LEAGUE_OAK, PokemonLeague_ChampionsRoom_Movement_OakEnter
	delay 60
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryMoveOver
	waitmovement LOCALID_LEAGUE_MARY
	waitmovement LOCALID_LEAGUE_OAK
	msgbox PokemonLeague_ChampionsRoom_Text_RematchOakCongratulations, MSGBOX_DEFAULT
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryInterview
	waitmovement 0
	msgbox PokemonLeague_ChampionsRoom_Text_RematchMaryInterview, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LEAGUE_LANCE, PokemonLeague_ChampionsRoom_Movement_LanceAnxious
	waitmovement 0
	call ChampionRotation_NoisyText
	closemessage
	applymovement LOCALID_LEAGUE_LANCE, PokemonLeague_ChampionsRoom_Movement_LanceLeave
	delay 30
	applymovement OBJ_EVENT_ID_PLAYER, PokemonLeague_ChampionsRoom_Movement_PlayerLeave
	waitmovement LOCALID_LEAGUE_LANCE
	@setobjectxyperm LOCALID_LEAGUE_LANCE, 6, 14
	removeobject LOCALID_LEAGUE_LANCE
	playse SE_EXIT
	waitmovement OBJ_EVENT_ID_PLAYER
	special SpawnCameraObject
	removeobject OBJ_EVENT_ID_PLAYER
	playse SE_EXIT
	waitse
	delay 50
	applymovement LOCALID_LEAGUE_MARY, PokemonLeague_ChampionsRoom_Movement_MaryLeft
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_LEAGUE_MARY, Common_Movement_ExclamationMark
	delay 55
	waitmovement LOCALID_LEAGUE_MARY
	msgbox PokemonLeague_ChampionsRoom_Text_RematchMaryNoInterview, MSGBOX_DEFAULT
	closemessage
	setvar VAR_LEAGUE_STATE, 6
	warpsilent MAP_POKEMON_LEAGUE_HALL_OF_FAME_HNS, 0
	waitstate
	releaseall
	end

PokemonLeague_ChampionsRoom_EventScript_AdjustPlayerFacingEast::
	applymovement OBJ_EVENT_ID_PLAYER, PokemonLeague_ChampionsRoom_Movement_AdjustPlayerFacingEast
	waitmovement 0
	return
PokemonLeague_ChampionsRoom_EventScript_AdjustPlayerFacingSouth::
	applymovement OBJ_EVENT_ID_PLAYER, PokemonLeague_ChampionsRoom_Movement_AdjustPlayerFacingSouth
	waitmovement 0
	return
PokemonLeague_ChampionsRoom_EventScript_AdjustPlayerFacingWest::
	applymovement OBJ_EVENT_ID_PLAYER, PokemonLeague_ChampionsRoom_Movement_AdjustPlayerFacingWest
	waitmovement 0
	return

PokemonLeague_ChampionsRoom_Movement_AdjustPlayerFacingEast:
	walk_down
	walk_right
	face_up
	step_end
PokemonLeague_ChampionsRoom_Movement_AdjustPlayerFacingSouth:
	walk_left
	walk_down
	walk_down
	walk_right
	face_up
	step_end
PokemonLeague_ChampionsRoom_Movement_AdjustPlayerFacingWest:
	walk_down
	walk_left
	face_up
	step_end

PokemonLeague_ChampionsRoom_Movement_PlayerMoveToCenter:
	lock_facing_direction
	walk_up
	unlock_facing_direction
	step_end

PokemonLeague_ChampionsRoom_Movement_LanceMoveOver:
	lock_facing_direction
	walk_right
	unlock_facing_direction
	step_end

PokemonLeague_ChampionsRoom_Movement_MaryEnter:
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	delay_2
	walk_in_place_fast_down
	step_end

PokemonLeague_ChampionsRoom_Movement_MaryMoveOver:
	walk_left
	face_right
	step_end

PokemonLeague_ChampionsRoom_Movement_OakEnter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PokemonLeague_ChampionsRoom_Movement_MaryInterview:
	walk_fast_up
	walk_in_place_fast_right
	step_end

PokemonLeague_ChampionsRoom_Movement_LanceAnxious:
	face_up
	delay_16
	delay_16
	face_down
	delay_16
	delay_16
	face_left
	step_end

PokemonLeague_ChampionsRoom_Movement_LanceLeave:
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PokemonLeague_ChampionsRoom_Movement_PlayerLeave:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PokemonLeague_ChampionsRoom_Movement_MaryLeft:
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

PokemonLeague_ChampionsRoom_Text_RematchBattleIntro:
    .string "LANCE: Welcome back!\p"
    .string "You've proven yourself in JOHTO,\p"
    .string "acquiring all 8 badges and even\n"
    .string "besting me once.\p"
    .string "But now, you're in the big\n"
    .string "leagues.\p"
    .string "The KANTO LEAGUE!\p"
    .string "A region with its own trials and\n"
    .string "champions.\p"
    .string "Here, I am the final challenge,\n"
    .string "with a team stronger than ever\l"
    .string "before.\p"
    .string "This is the true test,\p"
    .string "a battle to determine the\n"
    .string "CHAMPION of KANTO.\p"
    .string "Are you ready to rise to this new\n"
    .string "challenge?\p"
    .string "To prove yourself once more?\p"
    .string "Show me the strength that\n"
    .string "conquered JOHTO, CHAMPION.$"

PokemonLeague_ChampionsRoom_Text_BattleIntro:
    .string "LANCE: I've been waiting for you.\p"
    .string "{PLAYER}!\p"
    .string "I knew that you, with your skills,\n"
    .string "would eventually reach me here.\p"
    .string "There's no need for words now.\p"
    .string "We will battle to determine who is\n"
    .string "the stronger of the two of us.\p"
    .string "As the most powerful trainer and as\n"
    .string "the POKéMON LEAGUE CHAMPION…\p"
    .string "I, LANCE the dragon master, accept\n"
    .string "your challenge!$"

PokemonLeague_ChampionsRoom_Text_BattleWin:
    .string "…It's over.\p"
    .string "But it's an odd feeling.\p"
    .string "I'm not angry that I lost. In fact,\n"
    .string "I feel happy.\p"
    .string "Happy that I witnessed the rise\n"
    .string "of a great new CHAMPION!$"

PokemonLeague_ChampionsRoom_Text_BattleAfter:
    .string "…Whew.\p"
    .string "You have become truly powerful,\n"
    .string "{PLAYER}.\p"
    .string "Your POKéMON have responded to your\n"
    .string "strong and upstanding nature.\p"
    .string "As a trainer, you will continue to\n"
    .string "grow strong with your POKéMON.$"

PokemonLeague_ChampionsRoom_Text_MaryOhNoOak:
    .string "MARY: Oh, no! It's all over!\p"
    .string "PROF.OAK, if you weren't so slow…$"

PokemonLeague_ChampionsRoom_Text_OakCongratulations:
    .string "PROF.OAK: Ah, {PLAYER}!\p"
    .string "It's been a long while.\p"
    .string "You certainly look more impressive.\p"
    .string "Your conquest of the JOHTO LEAGUE\n"
    .string "is just fantastic!\p"
    .string "Your dedication, trust and love for\n"
    .string "your POKéMON made this happen.\p"
    .string "Your POKéMON were outstanding too.\p"
    .string "Because they believed in you as a\n"
    .string "trainer, they persevered.\p"
    .string "Congratulations, {PLAYER}!$"

PokemonLeague_ChampionsRoom_Text_RematchOakCongratulations:
    .string "PROF.OAK: Ah, {PLAYER}!\p"
    .string "I've been following your progress\n"
    .string "since PROF.ELM sent you on your\l"
    .string "way.\p"
    .string "You've certainly become more\n"
    .string "impressive.\p"
    .string "Conquering both the JOHTO and now\n"
    .string "the KANTO LEAGUE is no small feat.\p"
    .string "You've truly outdone yourself!\p"
    .string "It's clear that your POKéMON trust\n"
    .string "you immensely,\p"
    .string "and that trust has been the\n"
    .string "foundation of your success.\p"
    .string "But remember, a trainer's journey\n"
    .string "is never truly over.\p"
    .string "Congratulations on your victory,\n"
    .string "{PLAYER}!$"

PokemonLeague_ChampionsRoom_Text_MaryInterview:
    .string "MARY: Let's interview the brand new\n"
    .string "JOHTO CHAMPION!$"

PokemonLeague_ChampionsRoom_Text_RematchMaryInterview:
    .string "MARY: Let's interview the brand new\n"
    .string "KANTO CHAMPION!$"

PokemonLeague_ChampionsRoom_Text_Noisy:
    .string "LANCE: This is getting to be a bit\n"
    .string "too noisy…\p"
    .string "{PLAYER}, could you come with me?$"

PokemonLeague_ChampionsRoom_Text_MaryNoInterview:
    .string "MARY: Oh, wait! We haven't done\n"
    .string "the interview!$"

PokemonLeague_ChampionsRoom_Text_RematchMaryNoInterview:
    .string "MARY: Come on! You can't blow off\n"
    .string "the interview AGAIN!$"

@ ---- Champion rotation (HnS Extended Legacy) ----
@ VAR_RESULT after ChampionRotation_Select: 0 = Lance, 1 = Steven, 2 = the partner.
@ VAR_TEMP_1 remembers the champion of this visit (0/1/2) for the after-battle text.
PokemonLeague_ChampionsRoom_OnTransition::
	call ChampionRotation_Select
	goto_if_eq VAR_RESULT, 1, ChampionRotation_GfxSteven
	goto_if_eq VAR_RESULT, 2, ChampionRotation_GfxPartner
	setvar VAR_OBJ_GFX_ID_E, OBJ_EVENT_GFX_LANCE_HNS
	end

ChampionRotation_GfxSteven::
	setvar VAR_OBJ_GFX_ID_E, OBJ_EVENT_GFX_STEVEN_HNS
	end

ChampionRotation_GfxPartner::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, ChampionRotation_GfxMay
	setvar VAR_OBJ_GFX_ID_E, OBJ_EVENT_GFX_BRENDAN_NORMAL
	end

ChampionRotation_GfxMay::
	setvar VAR_OBJ_GFX_ID_E, OBJ_EVENT_GFX_MAY_NORMAL
	end

ChampionRotation_Select::
	goto_if_unset FLAG_IS_CHAMPION, ChampionRotation_SelectLance
	goto_if_unset FLAG_IS_KANTO_CHAMPION, ChampionRotation_SelectPartner
	goto_if_set FLAG_CHAMPION_ROTATION_BIT1, ChampionRotation_SelectPartner
	goto_if_set FLAG_CHAMPION_ROTATION_BIT0, ChampionRotation_SelectSteven
ChampionRotation_SelectLance::
	setvar VAR_RESULT, 0
	return

ChampionRotation_SelectSteven::
	setvar VAR_RESULT, 1
	return

ChampionRotation_SelectPartner::
	setvar VAR_RESULT, 2
	return

@ Rotation state 0 (Lance) -> 1 (Steven) -> 2 (partner) -> 0. Only advances once the KANTO LEAGUE has been won.
ChampionRotation_Advance::
	goto_if_unset FLAG_IS_KANTO_CHAMPION, ChampionRotation_AdvanceDone
	goto_if_set FLAG_CHAMPION_ROTATION_BIT1, ChampionRotation_AdvanceFrom2
	goto_if_set FLAG_CHAMPION_ROTATION_BIT0, ChampionRotation_AdvanceFrom1
	setflag FLAG_CHAMPION_ROTATION_BIT0
	return

ChampionRotation_AdvanceFrom1::
	clearflag FLAG_CHAMPION_ROTATION_BIT0
	setflag FLAG_CHAMPION_ROTATION_BIT1
	return

ChampionRotation_AdvanceFrom2::
	clearflag FLAG_CHAMPION_ROTATION_BIT1
ChampionRotation_AdvanceDone::
	return

ChampionRotation_Steven::
	setvar VAR_TEMP_1, 1
	msgbox ChampionRotation_Text_StevenIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	trainerbattle_no_intro TRAINER_STEVEN_HNS, ChampionRotation_Text_StevenWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

ChampionRotation_Partner::
	setvar VAR_TEMP_1, 2
	call PartnerJohto_BufferStarter
	goto_if_unset FLAG_IS_KANTO_CHAMPION, ChampionRotation_PartnerFirstIntro
	msgbox ChampionRotation_Text_PartnerRematchIntro, MSGBOX_DEFAULT
	goto ChampionRotation_PartnerBattle

ChampionRotation_PartnerFirstIntro::
	msgbox ChampionRotation_Text_PartnerFirstIntro, MSGBOX_DEFAULT
	goto ChampionRotation_PartnerBattle

@ The partner's starter matches the player's JOHTO starter's type: Cyndaquil -> Torchic, Totodile -> Mudkip,
@ Chikorita -> Treecko. Their champion ace is that starter's final evolution.
ChampionRotation_PartnerBattle::
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, ChampionRotation_PartnerMay
	goto_if_eq PLAYER_STARTER_SPECIES, SPECIES_CYNDAQUIL, ChampionRotation_PartnerBrendanFire
	goto_if_eq PLAYER_STARTER_SPECIES, SPECIES_TOTODILE, ChampionRotation_PartnerBrendanWater
	trainerbattle_no_intro TRAINER_PARTNER_CHAMPION_BRENDAN_GRASS, ChampionRotation_Text_PartnerWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

ChampionRotation_PartnerBrendanFire::
	trainerbattle_no_intro TRAINER_PARTNER_CHAMPION_BRENDAN_FIRE, ChampionRotation_Text_PartnerWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

ChampionRotation_PartnerBrendanWater::
	trainerbattle_no_intro TRAINER_PARTNER_CHAMPION_BRENDAN_WATER, ChampionRotation_Text_PartnerWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

ChampionRotation_PartnerMay::
	goto_if_eq PLAYER_STARTER_SPECIES, SPECIES_CYNDAQUIL, ChampionRotation_PartnerMayFire
	goto_if_eq PLAYER_STARTER_SPECIES, SPECIES_TOTODILE, ChampionRotation_PartnerMayWater
	trainerbattle_no_intro TRAINER_PARTNER_CHAMPION_MAY_GRASS, ChampionRotation_Text_PartnerWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

ChampionRotation_PartnerMayFire::
	trainerbattle_no_intro TRAINER_PARTNER_CHAMPION_MAY_FIRE, ChampionRotation_Text_PartnerWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

ChampionRotation_PartnerMayWater::
	trainerbattle_no_intro TRAINER_PARTNER_CHAMPION_MAY_WATER, ChampionRotation_Text_PartnerWin
	goto PokemonLeague_WillsRoom_EventScript_LanceOnDefeatKanto

ChampionRotation_AfterText::
	goto_if_eq VAR_TEMP_1, 1, ChampionRotation_AfterSteven
	goto_if_eq VAR_TEMP_1, 2, ChampionRotation_AfterPartner
	msgbox PokemonLeague_ChampionsRoom_Text_BattleAfter, MSGBOX_DEFAULT
	return

ChampionRotation_AfterSteven::
	msgbox ChampionRotation_Text_StevenAfter, MSGBOX_DEFAULT
	return

ChampionRotation_AfterPartner::
	goto_if_unset FLAG_IS_KANTO_CHAMPION, ChampionRotation_AfterPartnerFirst
	msgbox ChampionRotation_Text_PartnerAfterRematch, MSGBOX_DEFAULT
	return

ChampionRotation_AfterPartnerFirst::
	msgbox ChampionRotation_Text_PartnerAfterFirst, MSGBOX_DEFAULT
	return

ChampionRotation_NoisyText::
	goto_if_eq VAR_TEMP_1, 1, ChampionRotation_NoisySteven
	goto_if_eq VAR_TEMP_1, 2, ChampionRotation_NoisyPartner
	msgbox PokemonLeague_ChampionsRoom_Text_Noisy, MSGBOX_DEFAULT
	return

ChampionRotation_NoisySteven::
	msgbox ChampionRotation_Text_StevenNoisy, MSGBOX_DEFAULT
	return

ChampionRotation_NoisyPartner::
	msgbox ChampionRotation_Text_PartnerNoisy, MSGBOX_DEFAULT
	return

ChampionRotation_Text_StevenIntro:
	.string "STEVEN: Welcome to the champion's\n"
	.string "chair, {PLAYER}.\p"
	.string "They say a CHAMPION is never done\n"
	.string "training. I traveled all the way from\l"
	.string "HOENN to see if that is true of you.\p"
	.string "Show me the bond you share with your\n"
	.string "POKéMON. I will not hold back!$"

ChampionRotation_Text_StevenWin:
	.string "STEVEN: Magnificent.\p"
	.string "You and your POKéMON move as one.\n"
	.string "I am glad I came.$"

ChampionRotation_Text_StevenAfter:
	.string "STEVEN: …A fine battle, {PLAYER}.\p"
	.string "A bond like yours is rarer than any\n"
	.string "stone I have ever found.\p"
	.string "Hold on to it. The HALL OF FAME\n"
	.string "awaits.$"

ChampionRotation_Text_StevenNoisy:
	.string "STEVEN: It is rather loud in here.\p"
	.string "{PLAYER}, would you walk with me?$"

ChampionRotation_Text_PartnerFirstIntro:
	.string "{PARTNER}: Hi, {PLAYER}. Surprised?\p"
	.string "I said I'd be competing too, didn't\n"
	.string "I? While you were climbing the JOHTO\l"
	.string "LEAGUE, I was climbing the KANTO one.\p"
	.string "I'm the CHAMPION now. My {STR_VAR_1} and\n"
	.string "I have waited a long time to stand\l"
	.string "across from you.\p"
	.string "You mean far too much to me for me\n"
	.string "to hold back. Ready, {PLAYER}?$"

ChampionRotation_Text_PartnerWin:
	.string "{PARTNER}: …Ha. Of course.\p"
	.string "You really are the best I know.$"

ChampionRotation_Text_PartnerAfterFirst:
	.string "{PARTNER}: I'm not sad. I'm proud.\p"
	.string "The title should belong to the person\n"
	.string "who earned it by being exactly who\l"
	.string "they are.\p"
	.string "That's you. Go. The HALL OF FAME is\n"
	.string "waiting. I'll cheer from the front\l"
	.string "row.$"

ChampionRotation_Text_PartnerRematchIntro:
	.string "{PARTNER}: CHAMPION to CHAMPION this\n"
	.string "time, {PLAYER}.\p"
	.string "My {STR_VAR_1} has been training for\n"
	.string "this. No holding back!$"

ChampionRotation_Text_PartnerAfterRematch:
	.string "{PARTNER}: Every rematch, you get\n"
	.string "stronger. I love watching it.\p"
	.string "Go on. I'll see you at home.$"

ChampionRotation_Text_PartnerNoisy:
	.string "{PARTNER}: It's getting loud in here.\p"
	.string "Come with me?$"
