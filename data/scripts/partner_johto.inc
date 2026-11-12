@ The player's partner is NORMAN's child, the "new kid in town" of the original HOENN story ({PARTNER}: MAY for a male
@ player, BRENDAN for a female player). They arrive by ferry in OLIVINE CITY, the only way into JOHTO until the
@ MAGNET TRAIN is repaired, and are met there after the lighthouse AMPHAROS is healed. Later one-time scenes follow
@ in Cianwood, Mahogany, Blackthorn and Vermilion. The partner's sprite is VAR_OBJ_GFX_ID_C (the same one the
@ Littleroot home uses after the time skip).
@
@ Visibility (object flag = FLAG_HIDE_PARTNER_x): shown while the town's requirement is met and its scene is not done.
@ Every scene after OLIVINE also requires FLAG_PARTNER_MET.

PartnerJohto_SetGfx::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, PartnerJohto_SetGfxMale
	setvar VAR_OBJ_GFX_ID_C, OBJ_EVENT_GFX_BRENDAN_NORMAL
	return

PartnerJohto_SetGfxMale::
	setvar VAR_OBJ_GFX_ID_C, OBJ_EVENT_GFX_MAY_NORMAL
	return

PartnerJohto_UpdateOlivine::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_OLIVINE_DONE, PartnerJohto_HideOlivine
	goto_if_unset FLAG_AMPHAROS_HEALED, PartnerJohto_HideOlivine
	clearflag FLAG_HIDE_PARTNER_OLIVINE
	return

PartnerJohto_HideOlivine::
	setflag FLAG_HIDE_PARTNER_OLIVINE
	return

PartnerJohto_UpdateCianwood::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_CIANWOOD_DONE, PartnerJohto_HideCianwood
	goto_if_unset FLAG_PARTNER_MET, PartnerJohto_HideCianwood
	goto_if_unset FLAG_DEFEATED_CIANWOOD_GYM, PartnerJohto_HideCianwood
	clearflag FLAG_HIDE_PARTNER_CIANWOOD
	return

PartnerJohto_HideCianwood::
	setflag FLAG_HIDE_PARTNER_CIANWOOD
	return

PartnerJohto_UpdateMahogany::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_MAHOGANY_DONE, PartnerJohto_HideMahogany
	goto_if_unset FLAG_PARTNER_MET, PartnerJohto_HideMahogany
	goto_if_unset FLAG_DEFEATED_MAHOGANY_TOWN_GYM, PartnerJohto_HideMahogany
	clearflag FLAG_HIDE_PARTNER_MAHOGANY
	return

PartnerJohto_HideMahogany::
	setflag FLAG_HIDE_PARTNER_MAHOGANY
	return

PartnerJohto_UpdateBlackthorn::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_BLACKTHORN_DONE, PartnerJohto_HideBlackthorn
	goto_if_unset FLAG_PARTNER_MET, PartnerJohto_HideBlackthorn
	goto_if_unset FLAG_DEFEATED_BLACKTHORN_GYM, PartnerJohto_HideBlackthorn
	clearflag FLAG_HIDE_PARTNER_BLACKTHORN
	return

PartnerJohto_HideBlackthorn::
	setflag FLAG_HIDE_PARTNER_BLACKTHORN
	return

PartnerJohto_UpdateVermilion::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_VERMILION_DONE, PartnerJohto_HideVermilion
	goto_if_unset FLAG_PARTNER_MET, PartnerJohto_HideVermilion
	goto_if_unset FLAG_DEFEATED_VERMILION_GYM, PartnerJohto_HideVermilion
	clearflag FLAG_HIDE_PARTNER_VERMILION
	return

PartnerJohto_HideVermilion::
	setflag FLAG_HIDE_PARTNER_VERMILION
	return

@ The partner's starter matches the type of the player's JOHTO starter: Cyndaquil -> Torchic, Totodile -> Mudkip,
@ Chikorita -> Treecko. STR_VAR_1 gets that starter's final evolution (the partner's long-time partner POKeMON).
PartnerJohto_BufferStarter::
	goto_if_eq PLAYER_STARTER_SPECIES, SPECIES_CYNDAQUIL, PartnerJohto_BufferStarterFire
	goto_if_eq PLAYER_STARTER_SPECIES, SPECIES_TOTODILE, PartnerJohto_BufferStarterWater
	bufferspeciesname STR_VAR_1, SPECIES_SCEPTILE
	return

PartnerJohto_BufferStarterFire::
	bufferspeciesname STR_VAR_1, SPECIES_BLAZIKEN
	return

PartnerJohto_BufferStarterWater::
	bufferspeciesname STR_VAR_1, SPECIES_SWAMPERT
	return

@ ---- OLIVINE CITY: the first meeting. Registers the POKeGEAR contact. ----
PartnerJohto_EventScript_Olivine::
	lock
	faceplayer
	msgbox PartnerJohto_Text_FirstMeet, MSGBOX_DEFAULT
	closemessage
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox PartnerJohto_Text_Registered, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	setflag FLAG_PARTNER_MET
	setflag FLAG_ENABLE_MOM_MATCH_CALL
	msgbox PartnerJohto_Text_Olivine, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_OLIVINE_DONE
	setflag FLAG_HIDE_PARTNER_OLIVINE
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_OLIVINE
	fadescreen FADE_FROM_BLACK
	release
	end

@ ---- CIANWOOD CITY (after the Cianwood GYM): a rest by the sea heals the party ----
PartnerJohto_EventScript_Cianwood::
	lock
	faceplayer
	call PartnerJohto_BufferStarter
	msgbox PartnerJohto_Text_Cianwood, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PartnerJohto_CianwoodDecline
	msgbox PartnerJohto_Text_CianwoodYes, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox PartnerJohto_Text_CianwoodAfter, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_CIANWOOD_DONE
	setflag FLAG_HIDE_PARTNER_CIANWOOD
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_CIANWOOD
	fadescreen FADE_FROM_BLACK
	release
	end

PartnerJohto_CianwoodDecline::
	msgbox PartnerJohto_Text_CianwoodNo, MSGBOX_DEFAULT
	release
	end

@ ---- MAHOGANY TOWN (after the Mahogany GYM): TEAM ROCKET and SILVER ----
PartnerJohto_EventScript_Mahogany::
	lock
	faceplayer
	msgbox PartnerJohto_Text_Mahogany, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_MAHOGANY_DONE
	setflag FLAG_HIDE_PARTNER_MAHOGANY
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_MAHOGANY
	fadescreen FADE_FROM_BLACK
	release
	end

@ ---- BLACKTHORN CITY (after the Blackthorn GYM): a small present ----
PartnerJohto_EventScript_Blackthorn::
	lock
	faceplayer
	msgbox PartnerJohto_Text_Blackthorn, MSGBOX_DEFAULT
	giveitem ITEM_SWEET_HEART
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox PartnerJohto_Text_BlackthornAfter, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_BLACKTHORN_DONE
	setflag FLAG_HIDE_PARTNER_BLACKTHORN
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_BLACKTHORN
	fadescreen FADE_FROM_BLACK
	release
	end

@ ---- VERMILION CITY (after the Vermilion GYM): the invitation to HOENN ----
PartnerJohto_EventScript_Vermilion::
	lock
	faceplayer
	msgbox PartnerJohto_Text_Vermilion, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PartnerJohto_VermilionNo
	msgbox PartnerJohto_Text_VermilionYes, MSGBOX_DEFAULT
	goto PartnerJohto_VermilionEnd

PartnerJohto_VermilionNo::
	msgbox PartnerJohto_Text_VermilionNo, MSGBOX_DEFAULT
	goto PartnerJohto_VermilionEnd

PartnerJohto_VermilionEnd::
	closemessage
	setflag FLAG_PARTNER_SCENE_VERMILION_DONE
	setflag FLAG_HIDE_PARTNER_VERMILION
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_VERMILION
	fadescreen FADE_FROM_BLACK
	release
	end

PartnerJohto_Text_FirstMeet:
	.string "{PARTNER}: Excuse me! Were you the one\n"
	.string "who helped that AMPHAROS in the\l"
	.string "lighthouse?\p"
	.string "I watched from the street. You\n"
	.string "went up there without a second\l"
	.string "thought.\p"
	.string "Sorry, I should introduce myself.\n"
	.string "I'm {PARTNER}, from PETALBURG CITY\l"
	.string "in HOENN.\p"
	.string "My dad is NORMAN, the GYM LEADER\n"
	.string "there. We came to visit old friends.\l"
	.string "OLIVINE's port is the only way in\l"
	.string "until the MAGNET TRAIN is repaired.\p"
	.string "I'll be around JOHTO for a while.\n"
	.string "Let's swap POKéGEAR numbers.$"

PartnerJohto_Text_Registered:
	.string "Registered {PARTNER} in the\n"
	.string "POKéGEAR.$"

PartnerJohto_Text_Olivine:
	.string "{PARTNER}: JASMINE says the AMPHAROS\n"
	.string "is shining again. She was so happy\l"
	.string "she nearly cried.\p"
	.string "So was I, a little.\p"
	.string "Come on. Look at the sea with me\n"
	.string "for a minute.\p"
	.string "…Okay, that's enough of that.\n"
	.string "I need to look less pleased. See you!$"

PartnerJohto_Text_Cianwood:
	.string "{PARTNER}: CIANWOOD smells like salt\n"
	.string "and medicine. I love it here.\p"
	.string "My {STR_VAR_1} has been with me since\n"
	.string "my very first POKéMON. Today we\l"
	.string "tried to surf the shallows and got\l"
	.string "knocked flat by a wave. We're both\l"
	.string "a little soggy.\p"
	.string "You look tired, though. Want to sit\n"
	.string "by the water with me for a bit?$"

PartnerJohto_Text_CianwoodYes:
	.string "{PARTNER}: Come on, there's a quiet\n"
	.string "spot by the rocks.$"

PartnerJohto_Text_CianwoodAfter:
	.string "{PARTNER}: Better? Good.\p"
	.string "Don't push yourself too hard out\n"
	.string "there. I'd worry.\p"
	.string "See you soon, {PLAYER}!$"

PartnerJohto_Text_CianwoodNo:
	.string "{PARTNER}: Okay. Maybe next time.\p"
	.string "Be careful out there, {PLAYER}.$"

PartnerJohto_Text_Mahogany:
	.string "{PARTNER}: {PLAYER}! I heard about the\n"
	.string "RADIO TOWER and the hideout. TEAM\l"
	.string "ROCKET is back, and you went\l"
	.string "straight in.\p"
	.string "I was scared for you. And a little\n"
	.string "in awe.\p"
	.string "I ran into that red-haired boy on\n"
	.string "the road. SILVER. He said he's the\l"
	.string "strongest, then walked off\l"
	.string "muttering your name.\p"
	.string "I told him to say it to your face.\n"
	.string "…Was that okay?\p"
	.string "Be careful, {PLAYER}. I mean it.$"

PartnerJohto_Text_Blackthorn:
	.string "{PARTNER}: Dragons! Dad would love\n"
	.string "it up here. He'd pretend he wasn't\l"
	.string "excited.\p"
	.string "I found something for you in a\n"
	.string "shop. It isn't much, but it made\l"
	.string "me think of you.$"

PartnerJohto_Text_BlackthornAfter:
	.string "{PARTNER}: It's a little embarrassing\n"
	.string "to hand over, so just take it, okay?\p"
	.string "Good luck at the LEAGUE, {PLAYER}!$"

PartnerJohto_Text_Vermilion:
	.string "{PARTNER}: The harbor smells like salt\n"
	.string "and engine oil. It reminds me of\l"
	.string "HOENN.\p"
	.string "You beat the JOHTO LEAGUE, and now\n"
	.string "you're taking on KANTO. I'm so\l"
	.string "proud of you.\p"
	.string "I'll be taking the boat home soon.\n"
	.string "{PLAYER}… when you come to HOENN, look\l"
	.string "me up in PETALBURG.\p"
	.string "I'd really like to show you around.\n"
	.string "Will you come? Promise?$"

PartnerJohto_Text_VermilionYes:
	.string "{PARTNER}: …Thank you. I'll be\n"
	.string "waiting.\p"
	.string "Dad will want to meet you, too.\n"
	.string "Try not to be nervous.$"

PartnerJohto_Text_VermilionNo:
	.string "{PARTNER}: Oh. That's okay.\p"
	.string "Still call me, alright? I'll be\n"
	.string "hoping.$"
