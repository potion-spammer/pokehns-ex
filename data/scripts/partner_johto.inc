@ The player's partner is the HOENN rival: MAY for a male player, BRENDAN for a female player ({PARTNER} text).
@ They are met on the road in JOHTO / KANTO, one short scene per town. Each scene is order-independent: the first one
@ the player finds also plays the first-meeting intro and registers the partner in the POKeGEAR.
@
@ Visibility (object flag = FLAG_HIDE_PARTNER_x): shown while the town's requirement is met and its scene is not done.

@ Called from each town's OnTransition: sets the rival sprite, then shows/hides the partner object.
PartnerJohto_SetGfx::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, PartnerJohto_SetGfxMale
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL
	return

PartnerJohto_SetGfxMale::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL
	return

PartnerJohto_UpdateViolet::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_VIOLET_DONE, PartnerJohto_HideViolet
	goto_if_unset FLAG_DEFEATED_VIOLET_GYM, PartnerJohto_HideViolet
	clearflag FLAG_HIDE_PARTNER_VIOLET
	return

PartnerJohto_HideViolet::
	setflag FLAG_HIDE_PARTNER_VIOLET
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

PartnerJohto_UpdateEcruteak::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_ECRUTEAK_DONE, PartnerJohto_HideEcruteak
	goto_if_unset FLAG_DEFEATED_ECRUTEAK_CITY_GYM, PartnerJohto_HideEcruteak
	clearflag FLAG_HIDE_PARTNER_ECRUTEAK
	return

PartnerJohto_HideEcruteak::
	setflag FLAG_HIDE_PARTNER_ECRUTEAK
	return

PartnerJohto_UpdateGoldenrod::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_GOLDENROD_DONE, PartnerJohto_HideGoldenrod
	goto_if_unset FLAG_DEFEATED_GOLDENROD_CITY_GYM, PartnerJohto_HideGoldenrod
	clearflag FLAG_HIDE_PARTNER_GOLDENROD
	return

PartnerJohto_HideGoldenrod::
	setflag FLAG_HIDE_PARTNER_GOLDENROD
	return

PartnerJohto_UpdateVermilion::
	call PartnerJohto_SetGfx
	goto_if_set FLAG_PARTNER_SCENE_VERMILION_DONE, PartnerJohto_HideVermilion
	goto_if_unset FLAG_DEFEATED_VERMILION_GYM, PartnerJohto_HideVermilion
	clearflag FLAG_HIDE_PARTNER_VERMILION
	return

PartnerJohto_HideVermilion::
	setflag FLAG_HIDE_PARTNER_VERMILION
	return

@ First meeting (or reunion, if the player has already been to Littleroot). Registers the POKeGEAR contact.
PartnerJohto_FirstMeetIfNeeded::
	goto_if_set FLAG_PARTNER_MET, PartnerJohto_ReturnOnly
	goto_if_set FLAG_VISITED_LITTLEROOT_TOWN, PartnerJohto_Reunion
	msgbox PartnerJohto_Text_FirstMeet, MSGBOX_DEFAULT
	goto PartnerJohto_Register

PartnerJohto_Reunion::
	msgbox PartnerJohto_Text_Reunion, MSGBOX_DEFAULT
	goto PartnerJohto_Register

PartnerJohto_Register::
	closemessage
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox PartnerJohto_Text_Registered, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	setflag FLAG_PARTNER_MET
	setflag FLAG_ENABLE_MOM_MATCH_CALL
	return

PartnerJohto_ReturnOnly::
	return

@ ---- VIOLET CITY (after the Violet GYM) ----
PartnerJohto_EventScript_Violet::
	lock
	faceplayer
	call PartnerJohto_FirstMeetIfNeeded
	msgbox PartnerJohto_Text_Violet, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_VIOLET_DONE
	setflag FLAG_HIDE_PARTNER_VIOLET
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_VIOLET
	fadescreen FADE_FROM_BLACK
	release
	end

@ ---- OLIVINE CITY (after the lighthouse AMPHAROS is healed) ----
PartnerJohto_EventScript_Olivine::
	lock
	faceplayer
	call PartnerJohto_FirstMeetIfNeeded
	msgbox PartnerJohto_Text_Olivine, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_OLIVINE_DONE
	setflag FLAG_HIDE_PARTNER_OLIVINE
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_OLIVINE
	fadescreen FADE_FROM_BLACK
	release
	end

@ ---- ECRUTEAK CITY (after the Ecruteak GYM): a rest together heals the party ----
PartnerJohto_EventScript_Ecruteak::
	lock
	faceplayer
	call PartnerJohto_FirstMeetIfNeeded
	msgbox PartnerJohto_Text_Ecruteak, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PartnerJohto_EcruteakDecline
	msgbox PartnerJohto_Text_EcruteakYes, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox PartnerJohto_Text_EcruteakAfter, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_ECRUTEAK_DONE
	setflag FLAG_HIDE_PARTNER_ECRUTEAK
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_ECRUTEAK
	fadescreen FADE_FROM_BLACK
	release
	end

PartnerJohto_EcruteakDecline::
	msgbox PartnerJohto_Text_EcruteakNo, MSGBOX_DEFAULT
	release
	end

@ ---- GOLDENROD CITY (after the Goldenrod GYM): a small present ----
PartnerJohto_EventScript_Goldenrod::
	lock
	faceplayer
	call PartnerJohto_FirstMeetIfNeeded
	msgbox PartnerJohto_Text_Goldenrod, MSGBOX_DEFAULT
	giveitem ITEM_SWEET_HEART
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox PartnerJohto_Text_GoldenrodAfter, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_PARTNER_SCENE_GOLDENROD_DONE
	setflag FLAG_HIDE_PARTNER_GOLDENROD
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_PARTNER_GOLDENROD
	fadescreen FADE_FROM_BLACK
	release
	end

@ ---- VERMILION CITY (after the Vermilion GYM): the invitation to HOENN ----
PartnerJohto_EventScript_Vermilion::
	lock
	faceplayer
	call PartnerJohto_FirstMeetIfNeeded
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
	.string "{PARTNER}: Oh! You're {PLAYER}, aren't\n"
	.string "you? I saw your battle at the GYM.\p"
	.string "That was really something.\p"
	.string "I'm {PARTNER}. My dad is PROF. BIRCH,\n"
	.string "back in HOENN.\p"
	.string "I came to JOHTO to study POKéMON\n"
	.string "on my own for once.\p"
	.string "…Mind if I run into you every now\n"
	.string "and then? Let's swap POKéGEAR\l"
	.string "numbers.$"

PartnerJohto_Text_Reunion:
	.string "{PARTNER}: {PLAYER}! I knew it was you.\p"
	.string "HOENN felt a lot smaller after you\n"
	.string "left. Then I heard about a trainer\l"
	.string "beating GYM LEADERS out here.\p"
	.string "It's just a coincidence that we\n"
	.string "keep running into each other.\n"
	.string "Honest.\p"
	.string "Let's swap POKéGEAR numbers, so\n"
	.string "I'm not just guessing next time.$"

PartnerJohto_Text_Registered:
	.string "Registered {PARTNER} in the\n"
	.string "POKéGEAR.$"

PartnerJohto_Text_Violet:
	.string "{PARTNER}: SPROUT TOWER is right\n"
	.string "there. Have you been to the top?\p"
	.string "The sages say the swaying pillar\n"
	.string "teaches you to stay steady in a\l"
	.string "storm.\p"
	.string "I think I need a lot of practice.\n"
	.string "But with someone like you around,\l"
	.string "it feels easier.\p"
	.string "Anyway! See you down the road,\n"
	.string "{PLAYER}.$"

PartnerJohto_Text_Olivine:
	.string "{PARTNER}: {PLAYER}! I was hoping to see\n"
	.string "you here.\p"
	.string "The whole city is talking about the\n"
	.string "lighthouse. JASMINE says the\l"
	.string "AMPHAROS is shining again.\p"
	.string "She was so happy she nearly cried.\n"
	.string "So was I, a little.\p"
	.string "Come on. Look at the sea with me\n"
	.string "for a minute.\p"
	.string "…Okay, that's enough of that.\n"
	.string "I need to look less pleased. See you!$"

PartnerJohto_Text_Ecruteak:
	.string "{PARTNER}: The dancers here are\n"
	.string "wonderful. I watched a show\l"
	.string "yesterday and couldn't stop smiling.\p"
	.string "The old towers, the quiet streets…\n"
	.string "HOENN has the ocean, but this\l"
	.string "place has history.\p"
	.string "You look tired. Want to rest with\n"
	.string "me for a bit?$"

PartnerJohto_Text_EcruteakYes:
	.string "{PARTNER}: Come on, there's a bench\n"
	.string "by the water.$"

PartnerJohto_Text_EcruteakAfter:
	.string "{PARTNER}: Better? Good.\p"
	.string "Don't push yourself too hard out\n"
	.string "there. I'd worry.\p"
	.string "See you soon, {PLAYER}!$"

PartnerJohto_Text_EcruteakNo:
	.string "{PARTNER}: Okay. Maybe next time.\p"
	.string "Be careful out there, {PLAYER}.$"

PartnerJohto_Text_Goldenrod:
	.string "{PARTNER}: GOLDENROD is huge! I tried\n"
	.string "to shop at the DEPT. STORE and got\l"
	.string "lost on the escalators.\p"
	.string "I did find something, though. It\n"
	.string "isn't much, but it made me think\l"
	.string "of you.$"

PartnerJohto_Text_GoldenrodAfter:
	.string "{PARTNER}: It's a little embarrassing\n"
	.string "to hand over, so just take it, okay?\p"
	.string "Good luck at the next GYM, {PLAYER}!$"

PartnerJohto_Text_Vermilion:
	.string "{PARTNER}: The harbor smells like salt\n"
	.string "and engine oil. It reminds me of\l"
	.string "HOENN.\p"
	.string "I'll be taking the boat home soon.\n"
	.string "Dad's lab won't run itself.\p"
	.string "{PLAYER}… when you come to HOENN, look\n"
	.string "me up. LITTLEROOT is the little town\l"
	.string "with a lot of grass.\p"
	.string "I'd really like to show you around.\n"
	.string "Will you come? Promise?$"

PartnerJohto_Text_VermilionYes:
	.string "{PARTNER}: …Thank you. I'll be\n"
	.string "waiting.\p"
	.string "Good luck out there, {PLAYER}.\n"
	.string "Call me if you get homesick for a\l"
	.string "place that isn't yours yet.$"

PartnerJohto_Text_VermilionNo:
	.string "{PARTNER}: Oh. That's okay.\p"
	.string "Still call me, alright? I'll be\n"
	.string "hoping.$"
