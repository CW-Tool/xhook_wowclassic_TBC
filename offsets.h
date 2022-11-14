#pragma once
#include "stdafx.h"
#include "Constants.h"
namespace Offsets
{
	////////////////////////
	//     3.4.0.46368T   //
	////////////////////////

	// base address
	static inline uintptr_t Base = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));


	//Lua
	inline static uintptr_t lua_pushnumber = Base + 0x511E00;
	inline static uintptr_t lua_pushboolean = Base + 0x511E00;

	// framescript
	// //FrameScript_ExecuteBuffer(1.13.6.37497)	0x3A2DA0 
	//FrameScriptGetText = 0x39D160;
	//FrameScriptRegister = 0x3A4CE0;(=FrameScript_RegisterFunctionNamespaceWithCount ��λ��+B0)
	//FrameScript_RegisterFunctionNamespaceWithCount = 0x3A4C30; 
	/*
	typedef UINT64(__fastcall *ptrFrameScriptExecute) (const char* , const char*, UINT64);
	ptrFrameScriptExecute pFrameScriptExecute = (ptrFrameScriptExecute)(baseaddress +0x3A2DA0 );
	try
	{
	hResult = pFrameScriptExecute(��DoEmote("dance")��, ��Script��, 0);
	}
	catch (...)
	{
	}
	*/
	// 2.5.2.41446
	//static inline uintptr_t FrameScriptRegister = 0x08C7A70;//(=FrameScript_RegisterFunctionNamespaceWithCount ��λ��+B0) __int64 __fastcall sub_8C7A70(__int64 a1)
	//static inline uintptr_t FrameScript_RegisterFunctionNamespaceWithCount = 0x8C79C0;//__int64 __fastcall sub_8C79C0(__int64 a1)

	
	static inline uintptr_t FrameScriptExecute = 0x00;//

	//3.4.0.46368T 40 55 41 54 41 55 41 56 41 57 48 83 EC 20 48 8D 6C 24 ? 48 89 5D 38 4C 8D 35 ? ? ? ? 4C 8B E1 48 89 75 40 8B 0D ? ? ? ? 41 8B F0 48 89 7D 48 45 0F B6 E9 4C 89 35 ? ? ? ? E8 ? ? ? ? 33 C9 89 4D 30 8D 50 FF 83 FA 08 77 13 B3 5F C6 45 31 50 04 30 88 5D 30 88 45 32 88 4D 33 EB 04 
	static inline uintptr_t FrameScriptGetText = 0x596CF0;//ida string window search _FEMALE, click the sub after DATA XREF:, go grahpic view to see the first sub

	//?? __int64 0x593650 __fastcall sub_593650(__int64 a1, __int64 a2)
	//?? 0x593600 __int64 __fastcall sub_593600(__int64 a1)
	//?? 0x593550 __int64 __fastcall sub_593550(__int64 a1)
	static inline uintptr_t FrameScriptRegister = 0x593600;//(=FrameScript_RegisterFunctionNamespaceWithCount ��λ��+B0)
	static inline uintptr_t FrameScript_RegisterFunctionNamespaceWithCount = 0x593550;

	// Pointers
	static inline uintptr_t IsPlayerInWorld = Base + 0x308F174; //3.4.0.46368T movzx   edx, cs:byte (after  0F B6 15)//48 83 EC 28 0F B6 15 ?? ?? ?? ?? C1 EA 03 83 E2 01 E8 ?? ?? ?? ?? B8 ?? ?? ?? ?? 48 83 C4 28 C3
	static inline uintptr_t LoadingScreen = Base + 0x00; // good //Reversed From Script_CanLogIn to CGlueMgr__CanLogIn
	static inline uintptr_t InWorld = Base + 0x00;
	static inline uintptr_t CGGameUI_s_inWorld = Base + 0x00; //   NotInitialized = 0, LoadingScreen1 = 3, LoadingScreen2 = 2, InGame = 4

	// object manager
	static inline uintptr_t ClntObjMgrEnumVisibleObjectsPtr = Base + 0x1378560;//3.4.0.46368T  NOT TESTED // 48 89 74 24 ? 57 48 83 EC 20 48 8B 05 ? ? ? ? 48 8B F1 33 C9 48 8B FA A8 01 75 08 44 8B C1 48 85 C0 75 06
	static inline uintptr_t ClntObjMgrGetMapId = Base + 0x2048120;//3.4.0.46368T  //function start address 48 8B 05 ?? ?? ?? ?? 48 85 C0 74 07 8B 80 ?? ?? ?? ?? C3 C3  !!Second QWORD
	static inline uintptr_t ClntObjMgrIsValid = Base + 0x15BE1B0; //3.4.0.46368T // function start address 48 83 EC 28 44 0F B6 C1 48 8B 0D ?? ?? ?? ?? 48 85 C9 74 15 BA ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 07 B0 01 48 83 C4 28 C3 32 C0 48 83 C4 28 C3 


	//CTM    --Must   //Mhmmh??  v11 = sub_1B75050(*(_QWORD *)(PlayerPtr + 240), (__int64)&Position, (unsigned int *)&v13);  //ClickToMove(v11, PlayerPtr, PlayerPtr, a3, v12); 
	static inline uintptr_t ClickToMove = Base + 0x143F140;  //3.4.0.46368T 48 83 EC 48 48 83 B9 ? ? ? ? ? 7E 62 4C 8B 81 ? ? ? ? 49 8B 00 48 39 05 ? ? ? ? 75 4F 49 8B 40 08 48 39 05 ? ? ? ? 

	//3.4.0.46368T sub_143EEB0 48 83 EC 58 48 83 B9 ?? ?? ?? ?? ?? 7E 72 48 8B 91 ?? ?? ?? ?? 48 8B 02 48 39 05 ?? ?? ?? ?? 75 5F 48 8B 42 08 48 39 05 ?? ?? ?? ?? 
	static inline uintptr_t FaceTo = Base + 0x143EEB0; //3.4.0.46368T
	// void __usercall FaceTo(__int64 PlayerPtr@<rcx>, int a2@<ebx>, int a3@<esi>)
	
	// pointers 40 55 48 83 EC 60 48 8B 15 ?? ?? ?? ?? 48 8B E9 48 8B 05 ?? ?? ?? ?? 48 85 D2 74 09 48 85 C0 0F 85 ?? ?? ?? ?? 48 89 5C 24 ?? 48 8D 05 ?? ?? ?? ?? 
	static inline uintptr_t InvalidPtrCheckMin = Base + 0x2F72BF0;//3.4.0 46368T   mov     rdx, cs:qword_2F72BF0
	static inline uintptr_t InvalidPtrCheckMax = Base + 0x2F72BF8;//3.4.0 46368T    mov     rax, cs:qword_2F72BF8
	static inline uintptr_t HardwareEventPtr = Base + 0x2D6A388;//3.4.0 46368T  after mov ebx, cs: (8B 1D) //0F 85 ?? ?? ?? ?? 48 8B 07 48 8B CF 8B 1D ?? ?? ?? ?? FF 90 ?? ?? ?? ?? 85 C0 74 11 48 8B 07 45 33 C0 33 D2 48 8B CF FF 90 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 85 C0 74 0A 33

	static inline uintptr_t CGItem_UseItemParm = Base + 0x3029070; //3.4.0 46368T // Close to second lea under loc 75 34 E8 ?? ?? ?? ?? 8B D0 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 0F 84 ?? ?? ?? ?? 48 8B CB E8 ?? ?? ?? ?? 8B C8 48 8B D7 E8 ?? ?? ?? ?? 48 8B CB 84 C0 75 27 48 89 75 B8 48 89 75 C0 E8 ?? ?? ?? ?? 4C 8D 4D B8 45 33 C0 48 8D 15 ?? ?? ?? ?? 
	static inline uintptr_t CGItem_UseItem = Base + 0x147C140;//3.4.0 46368T
	//3.4.0 46368T 4C 89 4C 24 ?? 44 89 44 24 ?? 48 89 54 24 ?? 55 53 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 68 0F B6 41 10 48 8B F1 48 8D 0D ?? ?? ?? ?? 44 8B 14 81 41 C1 EA 02 41 F6 C2 01 0F 85 ?? ?? ?? ?? 48 8B 86 ?? ?? ?? ?? 48 8D 8E ?? ?? ?? ?? FF 50 08
	//2.5.3 42083 48 89 5C 24 ?? 4C 89 4C 24 ?? 44 89 44 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ?? ?? ?? ?? 0F B6 41 20 48 8B F9 48 8D 0D ?? ?? ?? ?? 45 8B F0 4C 8B EA 44 8B 14 81 41 C1 EA 02
	//OLD, not sure where to use 3B 91 ?? ?? ?? ?? 73 11 8B C2 48 83 C0 28 48 69 C0 ?? ?? ?? ?? 48 03 C1 C3 33 C0 C3

	// Unit struct
	static inline uint8_t Type = 0x20;
	static inline uint16_t Guid = 0x58;
	static inline uint16_t AnimationStatus = 0xA0; //// Script_GetInventoryItemTexture test dword ptr [rax+0A0h
	inline static uint16_t	GatherStatus = 0x1A8; //Maybe 0x448  |    Older Builds 36.xxx, 37.xxx: 0x6B0;
	static inline uint16_t DisplayID = 0x003C;
	static inline uint16_t Owner = 0x534;

	//cast  
	//static inline uintptr_t GetBagAndSlot = Base + 0x17C2630; // 48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 48 89 7C 24 ?? 41 56 48 83 EC 30 4C 8B 74 24 ?? 49 8B D9 49 C7 00 ?? ?? ?? ?? 49 8B F0 41 C7 01 ?? ?? ?? ?? 8B EA 48 63 F9 41 C7 06 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 85 C0 74 3D 89 2B 85 FF 79 7D 48 05 ?? ?? ?? ?? 48 89 06 83 FF FB 74 63
	//static inline uintptr_t GetItemPointer = Base + 0x139B360; // script_C_NewItems_IsNewItem 40 57 48 83 EC 50 BA ?? ?? ?? ?? 48 8B F9 E8 ?? ?? ?? ?? 85 C0 0F 84 ?? ?? ?? ?? BA ?? ?? ?? ?? 48 8B CF E8 ?? ?? ?? ?? 85 C0 0F 84 ?? ?? ?? ?? BA ?? ?? ?? ?? 48 89 5C 24 ?? 48 8B CF E8 ?? ?? ?? ?? BA ?? ?? ?? ?? 48 8B CF 8B D8 E8 ?? ?? ?? ?? 48 8D 4C 24 ?? 48 89 4C 24 ?? 4C 8D 4C 24 ?? 4C 8D 44 24 ?? 8D 48 FF 8D 53 FF E8 ?? ?? ?? ?? 48 8B 5C 24 ?? 84 C0 
	static inline uintptr_t Spell_C_GetMinMaxRange = Base + 0xE43A60;//3.4.0 46360t // 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 49 8B D9 49 8B F8 8B F2 48 8B E9 E8 ? ? ? ? 
	static inline uintptr_t Spell_C_IsCurrentSpell = Base + 0x136BA00; //3.4.0 46360t // 48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 81 EC ? ? ? ? 8B D9 E8 ? ? ? ? 48 8B 10 48 3B D0 74 0D
	static inline uintptr_t Spell_C_GetSpellCoolDown = Base + 0x13636F0;//3.4.0 46360t  // 48 83 EC 58 44 8B D1 C6 44 24 ?? ?? 41 F7 D8 48 8D 05 ?? ?? ?? ?? 44 8B C2 41 8B D2 48 1B C9 81 E1 ?? ?? ?? ?? 48 03 C8 0F B6 84 24 ?? ?? ?? ?? 88 44 24 40 48 8B 84 24 ?? ?? ?? ?? 48 89 44 24 ?? 48 8B 84 24 ?? ?? ?? ?? 
	static inline uintptr_t Spell_C_CastSpell = Base + 0x1662D90;//3.4.0 46360t // 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 50 41 0F B6 F1 49 8B E8 44 0F B6 F2 8B D9 E8 ? ? ? ? 
	static inline uintptr_t Spell_C_HaveSpellPower = Base + 0x1367960; //3.4.0 46360t// 4C 89 4C 24 ?? 4C 89 44 24 ?? 89 54 24 10 53 55 56 57
	static inline uintptr_t isSpellKnown = Base + 0x166BB30; //3.4.0 46360t // 48 89 5C 24 ?? 57 48 83 EC 20 0F B6 FA 8B D9 E8 ?? ?? ?? ?? 85 C0 79 43 40 84 FF 74 18 8B CB E8 ?? ?? ?? ?? 48 85 C0 0F 95 C0 48 8B 5C 24 ?? 48 83 C4 20 5F C3 81 FB ?? ?? ?? ??
	
	static inline uintptr_t findSlotBySpellId = Base + 0x00; //3.4.0 46360t //old sig, not working for 3.4.0 anymore 44 8B C1 85 C9 0F 84 ?? ?? ?? ?? 84 D2 74 51 44 8B 0D ?? ?? ?? ?? 33 D2 45 85 C9 74 74 4C 8B 15 ?? ?? ?? ?? 49 8B 0C D2 
	
	static inline uintptr_t CancelChannelOffset = Base + 0x1358C80; // 3.4.0 46360t //48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC 50 8B F2 8B D9 E8 ?? ?? ?? ?? BA ?? ?? ?? ?? 41 B8 ?? ?? (...) FF
	static inline uintptr_t UnitReaction = Base + 0x14299A0; // 3.4.0 46360t //48 89 5C 24 ?? 57 48 83 EC 20 48 8B DA 48 8B F9 E8 ?? ?? ?? ?? 83 F8 04 7D 37 48 8B D7 48 8B CB E8 ?? ?? ?? ?? 83 F8 04 7D 27 8B 87 ?? ?? ?? ?? C1 E8 14 A8 01 74 0D 8B 83 ?? ?? ?? ?? C1 E8 03 A8 01 75 0D 32 C0 48 8B 5C 24 ?? 48 83 C4 20 5F C3 48 8B 5C 24 ?? B0 01 48 83 C4 20 5F C3
	static inline uintptr_t Cooldown = Base + 0x3029090;   // 3.4.0 46360t  dword after lea rax: unk(F6 D8 48 8D 05 ?? ?? ?? ??)  //40 53 48 83 EC 20 80 79 24 00 0F B6 59 25 8B 51 20 41 0F 95 C0 0F B6 C3 F6 D8 48 8D 05 ? ? ? ? 48 1B C9 45 33 C9 81 E1 ? ? ? ? 48 03 C8 E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 84 DB 74 0A 48 83 C4 20 5B

	static inline uintptr_t GuidToString = Base + 0x11B1480; // 3.4.0 46360t // 48 89 ?? ?? ?? 48 89 ?? ?? ?? 56 48 ?? ?? ?? 41 ?? ?? 48 ?? ?? 48 ?? ?? 45

	//Release ghost
	static inline uintptr_t CGPlayer_C_HandleRepopRequest = Base + 0x13AFFB0; //3.4.0 46360t //function address 40 53 48 83 EC 50 48 8B 01 0F B6 DA FF 90 ?? ?? ?? ?? 84 C0 74 18 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 48 8D 4C 24 ?? 88 5C 24 40 E8 ?? ?? ?? ?? 48 83 C4 50 5B 
	static inline uintptr_t RetrieveCorpse = Base + 0x00;//3.4.0 46360t  //below are the old sig, not working for 3.4.0 anymore
	//89 54 24 10 48 89 4C 24 ?? 48 81 EC ?? ?? ?? ?? 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 48 8B C8 E8 ?? ?? ?? ?? 0F B6 C0 85 C0 75 02 EB 73 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 0F 10 00 

	//40 53 48 83 EC 30 83 3D ?? ?? ?? ?? ?? 48 8B D9 74 0C BA ?? ?? ?? ?? Second Call LeftClick, Third Call RightClick
	static inline uintptr_t SpriteLeftClick = Base + 0x15FD630; //3.4.0 46360t 
	static inline uintptr_t SpriteRightClick = Base + 0x15FDA60; //3.4.0 46360t 

	static inline uintptr_t CGUnit_C_OnAttackIconPressed = Base + 0x142FF60; //3.4.0 46360t //Script::Attack 48 89 74 24 ?? 41 54 41 56 41 57 48 83 EC 70 0F 10 02 48 8B F1 45 8B F9 48 8D 0D ?? ?? ?? ?? 45 8B E0 0F 11 44 24 ?? E8 ?? ?? ?? ?? C6 44 24 ?? ?? 48 8D 54 24 ?? 45 33 F6 C6 44 24 ?? ?? 45 8B C4 C6 44 24 ?? ?? 0F 10 00 
	static inline uintptr_t CGUnit_C_IsInMelee = Base + 0x1429B10; //3.4.0 46360t // 48 8B 81 ?? ?? ?? ?? 48 C1 E8 3A 84 C0 0F 95 C0 C3 

	//static inline uintptr_t spellDB = 0x00;//Script_GetProfessionInfo, First CS:Offset under WowClientDB2_Base__GetRecordDataUnsafe
	//static inline uintptr_t spellDBTotalRowsAddr = 0x00 ; , Second CS:Offset

	//Globals
	static inline uintptr_t isLootWindowOpen = Base + 0x30C3230;//3.4.0 46360t  // last xmmword after 0F 10 05,   xmmword 4C 8D 05 ?? ?? ?? ?? BA ?? ?? ?? ?? 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 48 85 C0 0F 84 ?? ?? ?? ?? 4C 8B 00 48 8B C8 41 FF 90 ?? ?? ?? ?? 4C 8B F0 4D 85 F6 0F 84 ?? ?? ?? ?? 85 DB 0F 88 ?? ?? ?? ?? 41 3B 1E 0F 8D ?? ?? ?? ?? 8B D3 49 8B CE EB 24 49 8D 9F ?? ?? ?? ?? E8 ?? ?? ?? ?? 8D 50 FF 48 85 DB 0F 84 ?? ?? ?? ?? 83 FA 12 0F 87 ?? ?? ?? ?? 48 8B CB E8 ?? ?? ?? ?? 48 8B C8 48 85 C0 0F 84 ?? ?? ?? ?? 0F 10 05 ?? ?? ?? ?
	static inline uintptr_t CorpseMapID = Base + 0x2C24550;  //3.4.0 46360t // First Dword 40 53 48 83 EC 20 48 8B D9 48 8B D1 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? F3 0F 10 43 ?? F3 0F 10 13 F3 0F 10 4B ?? F3 0F 59 D2 F3 0F 59 C0 F3 0F 59 C9 F3 0F 58 D0 F3 0F 58 D1 0F 54 15 ?? ?? ?? ?? 0F 2F 15 ?? ?? ?? ?? 0F 93 C0 48 83 C4 20 5B C3 
	static inline uintptr_t Corpsex = CorpseMapID + 0x40; //Bruteforced always ~close to the CorpseMapID
	static inline uintptr_t Bool_MustEnterInstanceToRecoverBodymsg = CorpseMapID + 0x04; 

	//Camera WorldFrame::GetActiveCamera
	static inline  uintptr_t CameraMgr = Base + 0x2FBA1B8;//3.4.0 46360t 
	//first qword after 48 8B 0D
	//3.4.0 46360t  0F 29 74 24 ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 8B 89 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ??
	// 48 89 74 24 ?? 48 89 7C 24 ?? 0F 29 74 24 ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 8B 89 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?
	
	
	static inline  uintptr_t CameraPtr = 0x38E0; // Shifted 8 bytes since 38631

	//ESP
	// 
	// method 1, get by memeory
	//                                                                                          4byte after E8 ?? ?? ?? ?? 48 8B 0D XX XX XX XX 
	//48 83 EC 28 80 3D ?? ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 75 39 E8 ?? ?? ?? ?? 45 33 C0 B1 FE 41 8D 50 01 E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 4C 8D 44 24 ?? 33 D2 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 
	
	// method2 get by the call, Script_QuitGame,  search String logs sub Above that! First Call, First DWORD.  (48 83 EC 28 45 33 C0 48 8D 0D ?? ?? ?? ?? 33 D2 E8 ?? ?? ?? ?? 4C 8D 05 ?? ?? ?? ?? 33 D2 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 85 C9 74 10 48 8D 15 ?? ?? ?? ?? 48 83 C4 28 E9 ?? ?? ?? ?? 48 83 C4 28 C3 )
	static inline uintptr_t GetWorld = Base + 0x1F1D40; //3.4.0 46360t read memory method  Read QWORD (Base + 0x2D01290)
	
	
	static inline uintptr_t WorldFrameIntersect = Base + 0x1CFAC60; //3.4.0 46360t F12, Search: Preload Immediate End -> DATA XREF: sub -> Sub below that, First Call and select second sub.
	//48 83 EC 58 8B 42 08 F2 0F 10 02 48 8D 54 24 ?? 89 44 24 28 41 8B 40 08 89 44 24 34 48 8B 84 24 ?? ?? ?? ?? 48 89 44 24 ?? 48 8B 84 24 ?? ?? ?? ?? F2 0F 11 44 24 ?? F2 41 0F 10 00 48 89 44 24 ?? F2 0F 11 44 24 ?? 4C 89 4C 24 ?? E8 ?? ?? ?? ?? 48 83 C4 58 C3 
};

