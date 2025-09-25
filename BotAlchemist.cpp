#include "stdafx.h"
#include "BotAlchemist.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "SkillManager.h"
#include "EffectManager.h"
#include "MasterSkillTree.h"
#include "ServerInfo.h"
#include "MemScript.h"
#include "Util.h"
#include "CashShop.h"
#include "readscript.h"
#include "Message.h"
#include "Notice.h"
#include "Trade.h"
#include "DSProtocol.h"
#include "ItemManager.h"

ObjBotAlchemist BotAlchemist;

void ObjBotAlchemist::Read(char * FilePath)
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTALCHEMIST;botNum++)
		{
			if(this->bot[botNum].Enabled == true)
			{
				int bIndex = this->bot[botNum].index;
				if(gObjIsConnected(bIndex) == TRUE)
				{
					gObjDel(bIndex);
				}
			}
		}
	}
	memset(&this->bot,0,sizeof(this->bot));
	this->Enabled = FALSE;
	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("BotAlchemist data load error %s", FilePath);
		return;
	}

	while ( true )
	{
		int iType = GetToken();
		
		if ( iType == 1 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTALCHEMIST-1)
				{
					MsgBox("BotAlchemistKG error: Bot Index:%d out of range!", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].Class = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].Rate = TokenNumber;

				Token = GetToken();
				strncpy(this->bot[BotNum].Name,TokenString,sizeof(this->bot[BotNum].Name));

				Token = GetToken();
				this->bot[BotNum].Map = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].X = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].Y = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].Dir = TokenNumber;

				this->bot[BotNum].Enabled = true;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		if ( iType == 2 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTALCHEMIST-1)
				{
					MsgBox("BotAlchemistKG error: Bot Index:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotAlchemistKG error: Bot Index:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotAlchemistKG error: Min Slot 0 ; Max Slot 8");
					return;
				}

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->bot[BotNum].body[Slot].num = GET_ITEM(iType,iIndex);

				Token = GetToken();
				this->bot[BotNum].body[Slot].level = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].body[Slot].opt = TokenNumber;

				this->bot[BotNum].body[Slot].Enabled = true;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		if ( iType == 3 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTALCHEMIST-1)
				{
					MsgBox("BotAlchemistKG error: Bot Index:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotAlchemistKG error: Bot Index:%d doesnt exist", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].AllowLevel = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].AllowOpt = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].AllowLuck = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].AllowSkill = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].AllowExc = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].AllowFFFF = TokenNumber;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		if ( iType == 4 )
		{
			this->Enabled = TRUE;
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTALCHEMIST-1)
				{
					MsgBox("BotAlchemistKG error: Bot Index:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotAlchemistKG error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].OnlyVip = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].VipMoney = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].Zen = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].PCPoint = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].OnlySameType = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].OnlyLowerIndex = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].AcceptAncient = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].MaxLevel = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].MaxExc = TokenNumber;
			}
		}
		
		break;
	}
			
	fclose(SMDFile);
}


void ObjBotAlchemist::MakeBot()
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTALCHEMIST;botNum++)
		{
			if(this->bot[botNum].Enabled == true)
			{
				int aIndex = gObjAddMonster(bot[botNum].Map);

				if(aIndex >= 0)
				{
					LPOBJ lpObj = &gObj[aIndex];
					this->bot[botNum].index = aIndex;
					lpObj->PosNum = (WORD)-1;
					lpObj->X = this->bot[botNum].X;
					lpObj->Y = this->bot[botNum].Y;
					lpObj->OldX = this->bot[botNum].X;
					lpObj->OldY = this->bot[botNum].Y;
					lpObj->TX = this->bot[botNum].X;
					lpObj->TY = this->bot[botNum].Y;
					lpObj->MTX = this->bot[botNum].X;
					lpObj->MTY = this->bot[botNum].Y;
					lpObj->Dir = this->bot[botNum].Dir;
					lpObj->Map = this->bot[botNum].Map;
					lpObj->Live = TRUE;
					lpObj->PathCount = 0;
					lpObj->IsBot = 2;
					lpObj->Type = OBJECT_USER;
					gObjSetMonster(aIndex,this->bot[botNum].Class);

					lpObj->ChangeUp = this->bot[botNum].Class % 16;	// Set Second Type of Character
					lpObj->Class = this->bot[botNum].Class / 16;
					lpObj->Level = 400;
					lpObj->Life = 1;
					lpObj->MaxLife = 2;
					lpObj->Mana = 1;
					lpObj->MaxMana = 2;
					lpObj->Experience = 0;

					lpObj->Inventory = new CItem[INVENTORY_EXT4_SIZE];					
					lpObj->Inventory1 = new CItem[INVENTORY_EXT4_SIZE];
					lpObj->InventoryMap1 = new BYTE[INVENTORY_EXT4_SIZE];
					lpObj->InventoryMap = new BYTE[INVENTORY_EXT4_SIZE];

					for (int i=0;i<INVENTORY_EXT4_SIZE;i++)
					{
						lpObj->Inventory[i].Clear();
						lpObj->Inventory1[i].Clear();
					}

					memset(&lpObj->InventoryMap[0], (BYTE)-1, INVENTORY_EXT4_SIZE);
					memset(&lpObj->InventoryMap1[0], (BYTE)-1, INVENTORY_EXT4_SIZE);

					strncpy(lpObj->Name,this->bot[botNum].Name,sizeof(lpObj->Name));

					for(int i=0;i<9;i++)
					{
						if(this->bot[botNum].body[i].num >= 0 && this->bot[botNum].body[i].Enabled == true)
						{
							CItem lpItem;

							lpObj->Inventory[i].Clear();

							lpItem.Clear();

							lpItem.m_Index = this->bot[botNum].body[i].num;

							lpItem.m_Option1 = 0;

							lpItem.m_Option2 = 1;

							lpItem.m_Option3 = this->bot[botNum].body[i].opt;

							lpItem.m_Level = this->bot[botNum].body[i].level;

							lpItem.m_Durability = 255.0f;

							lpItem.m_Serial = 0;

							lpItem.m_NewOption = 0;

							lpItem.m_SetOption = 0;

							lpItem.m_ItemOptionEx = 0;

							lpItem.m_IsPeriodicItem = 0;

							lpItem.m_IsExpiredItem = 0;

							memset(lpItem.m_SocketOption,0xFF,MAX_SOCKET_OPTION);

							lpItem.m_JewelOfHarmonyOption = 0;

							lpItem.m_SocketOptionBonus = 0xFF;

							lpItem.Convert(lpItem.m_Index,lpItem.m_Option1,lpItem.m_Option2,lpItem.m_Option3,lpItem.m_NewOption,lpItem.m_SetOption,lpItem.m_JewelOfHarmonyOption,lpItem.m_ItemOptionEx,lpItem.m_SocketOption,lpItem.m_SocketOptionBonus);

							gItemManager.InventoryAddItem(aIndex,lpItem,i);
						}
					}

					lpObj->Inventory1 = lpObj->Inventory;
					lpObj->InventoryMap1 = lpObj->InventoryMap;
					gObjectManager.CharacterMakePreviewCharSet(aIndex);

					lpObj->AttackType = 0;
					lpObj->BotSkillAttack = 0;					

					lpObj->Attribute = 100;
					lpObj->TargetNumber = (WORD)-1;
					lpObj->ActionState.Emotion = 0;
					lpObj->ActionState.Attack = 0;
					lpObj->ActionState.EmotionCount = 0;
					lpObj->PathCount = 0;
					lpObj->BotPower = 0;
					lpObj->BotDefense = this->bot[botNum].OnlyVip;
					lpObj->BotLife = 1;
					lpObj->BotMaxLife = 1;
					
					lpObj->BotLvlUpDefense = 1;
					lpObj->BotLvlUpPower = 1;
					lpObj->BotLvlUpLife = 1;
					lpObj->BotLvlUpMana = 1;
					lpObj->BotLvlUpExp = 1;
					lpObj->BotLvlUpMaxLevel = 1;

					lpObj->MoveRange = 1;
					lpObj->BotFollowMe = 0;
					lpObj->NextExperience = gLevelExperience[lpObj->Level];
				}
			}
		}
	}
}
BOOL ObjBotAlchemist::IsInTrade(int aIndex)
{
	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);
	if(number != -1)
		return 1;
	else
		return 0;
}

int ObjBotAlchemist::GetBotIndex(int aIndex)
{
	for(int i=0;i<MAX_BOTALCHEMIST;i++)
	{
		if(this->bot[i].Enabled == true)
		{
			if(this->bot[i].index == aIndex)
				return i;
		}
	}
	return -1;
}

int ObjBotAlchemist::getNumberOfExcOptions(int checksum)
{
	int optionscount=0;
	int ExcOrgArr[6];

	ExcOrgArr[0]=1;
	ExcOrgArr[1]=2;
	ExcOrgArr[2]=4;
	ExcOrgArr[3]=8;
	ExcOrgArr[4]=16;
	ExcOrgArr[5]=32;

	if (checksum>0)
	{
		for (int i=0;i<6;i++)
		{
			int and_val=checksum & ExcOrgArr[i];
			if (and_val != 0)
				optionscount+=1;
		}
	}

	return optionscount;
}


bool ObjBotAlchemist::AllowExc(BYTE BotNum, BYTE ExcOpt)
{				
	int dwExOpCount = this->getNumberOfExcOptions(ExcOpt);

	if(dwExOpCount < this->bot[BotNum].MaxExc)
	{
		return true;
	}
	return false;
}

BYTE ObjBotAlchemist::Alchemy(int aIndex,int BotNum)
{
	char sbuf[512]={0};
	int fitem=-1;
	int sitem=-1;
	int count=0;

	if(gObjIsConnected(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if((lpObj->TradeMoney - this->bot[BotNum].Zen) < 0)
	{
		wsprintf(sbuf,gMessage.GetMessage(874),this->bot[BotNum].Zen );
		ChatSend(&gObj[this->bot[BotNum].index],sbuf);
		goto Cancel;
	}
	
	if(this->bot[BotNum].PCPoint > 0)
	{
		if(lpObj->Coin1 < this->bot[BotNum].PCPoint)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(875),this->bot[BotNum].PCPoint);
			goto Cancel;
		}
	}

	if(this->bot[BotNum].VipMoney > 0)
	{
		if(lpObj->Coin1 < this->bot[BotNum].VipMoney)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(876),this->bot[BotNum].VipMoney);
			goto Cancel;
		}
	}
	
	for(int n = 0; n < TRADE_SIZE; n++)
	{
		if(lpObj->Trade[n].IsItem() != 0)
		{
			if((n >= 0 && n<=3) || (n >= 8 && n<=11) || (n >= 16 && n<=19) || (n >= 24 && n<=27))
			{
				fitem = n;
			}else
			{
				sitem = n;
			}

			count++;
		}
	}
	if(count != 2)
	{
		ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(877));
		goto Cancel;
	}
	if((lpObj->Trade[fitem].m_Index >= 6144) || (lpObj->Trade[sitem].m_Index >= 6144))
	{
		ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(878));
		goto Cancel;
	}
	
	if(fitem == -1 || sitem == -1)
	{
		ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(879));
		goto Cancel;
	}

	if(this->bot[BotNum].OnlySameType == 1)
	{
		int fType = lpObj->Trade[fitem].m_Index/512;
		int sType = lpObj->Trade[sitem].m_Index/512;
		if(fType != sType)
		{
			ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(880));
			goto Cancel;
		}
	}

	if(this->bot[BotNum].OnlyLowerIndex == 1)
	{
		int fLevel = gItemManager.m_ItemInfo[lpObj->Trade[fitem].m_Index].Level;
		int sLevel = gItemManager.m_ItemInfo[lpObj->Trade[sitem].m_Index].Level;

		if(fLevel > sLevel)
		{
			if(fLevel > (sLevel + 10))
			{
				ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(881));
				goto Cancel;
			}
		}
	}

	if(this->bot[BotNum].AcceptAncient == 0 && lpObj->Trade[fitem].m_SetOption > 0)
	{
		ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(882));
		goto Cancel;
	}
	srand(static_cast<int>(time(NULL)));
	int random = rand()%100;
	bool failed = false;

	if(random > this->bot[BotNum].Rate )
	{
		failed = true;
	}

	if(this->bot[BotNum].AllowLuck == 1)
	{
		if(lpObj->Trade[fitem].m_Option2 == 0 && lpObj->Trade[sitem].m_Option2 == 1)
		{
			if(!failed)
				lpObj->Trade[fitem].m_Option2 = 1;
			lpObj->Trade[sitem].m_Option2 = 0;
		}
	}
	if(this->bot[BotNum].AllowLevel == 1)
	{
		int fLevel = lpObj->Trade[fitem].m_Level;
		if(fLevel < this->bot[BotNum].MaxLevel)
		{
			int sLevel = lpObj->Trade[sitem].m_Level;
			if ((fLevel + sLevel) > this->bot[BotNum].MaxLevel)
			{
				sLevel -= (this->bot[BotNum].MaxLevel - fLevel);
				if(!failed)
					fLevel = this->bot[BotNum].MaxLevel;
			}else
			{
				if(!failed)
					fLevel += sLevel;
				sLevel = 0;
			}
			lpObj->Trade[fitem].m_Level = fLevel;
			lpObj->Trade[sitem].m_Level = sLevel;
		}
	}
	if(this->bot[BotNum].AllowSkill == 1)
	{
		if(lpObj->Trade[fitem].m_Index < 3584)
		{
			if(lpObj->Trade[fitem].m_Option1 == 0 && lpObj->Trade[sitem].m_Option1 == 1)
			{
				if(!failed)
					lpObj->Trade[fitem].m_Option1 = 1;
				lpObj->Trade[sitem].m_Option1 = 0;
			}
		}
	}
	if(this->bot[BotNum].AllowOpt == 1)
	{
		int fOpt = lpObj->Trade[fitem].m_Option3;
		if(fOpt < 7)
		{
			int sOpt = lpObj->Trade[sitem].m_Option3;
			if ((fOpt + sOpt) > 7)
			{
				sOpt -= (7 - fOpt);
				if(!failed)
					fOpt = 7;
			}else
			{
				if(!failed)
					fOpt += sOpt;
				sOpt = 0;
			}
			lpObj->Trade[fitem].m_Option3 = fOpt;
			lpObj->Trade[sitem].m_Option3 = sOpt;
		}
	}
	if(this->bot[BotNum].AllowExc == 1)
	{
		if(lpObj->Trade[fitem].m_NewOption != 63)
		{
			if(lpObj->Trade[sitem].m_NewOption != 0)
			{		
				if(this->AllowExc(BotNum,lpObj->Trade[fitem].m_NewOption) == true)
				{
					BYTE fLife		= lpObj->Trade[fitem].IsExtLifeAdd();
					BYTE fMana		= lpObj->Trade[fitem].IsExtManaAdd();
					BYTE fDmg		= lpObj->Trade[fitem].IsExtDamageMinus();
					BYTE fReflect	= lpObj->Trade[fitem].IsExtDamageReflect();
					BYTE fDef		= lpObj->Trade[fitem].IsExtDefenseSuccessfull();
					BYTE fMoney		= lpObj->Trade[fitem].IsExtMonsterMoney();
					
					BYTE sLife		= lpObj->Trade[sitem].IsExtLifeAdd();
					BYTE sMana		= lpObj->Trade[sitem].IsExtManaAdd();
					BYTE sDmg		= lpObj->Trade[sitem].IsExtDamageMinus();
					BYTE sReflect	= lpObj->Trade[sitem].IsExtDamageReflect();
					BYTE sDef		= lpObj->Trade[sitem].IsExtDefenseSuccessfull();
					BYTE sMoney		= lpObj->Trade[sitem].IsExtMonsterMoney();

					
					if(fLife == 0 && sLife != 0)
					{
						if(!failed)
							lpObj->Trade[fitem].m_NewOption |= 0x20;
						lpObj->Trade[sitem].m_NewOption &= ~0x20;
					}
					if(this->AllowExc(BotNum,lpObj->Trade[fitem].m_NewOption) == true)
					{
						if(fMana == 0 && sMana != 0)
						{
							if(!failed)
								lpObj->Trade[fitem].m_NewOption |= 0x10;
							lpObj->Trade[sitem].m_NewOption &= ~0x10;
						}
					}
		
					if(this->AllowExc(BotNum,lpObj->Trade[fitem].m_NewOption) == true)
					{
						if(fDmg == 0 && sDmg != 0)
						{
							if(!failed)
								lpObj->Trade[fitem].m_NewOption |= 8;
							lpObj->Trade[sitem].m_NewOption &= ~8;
						}
					}
					
					if(this->AllowExc(BotNum,lpObj->Trade[fitem].m_NewOption) == true)
					{
						if(fReflect == 0 && sReflect != 0)
						{
							if(!failed)
								lpObj->Trade[fitem].m_NewOption |= 4;
							lpObj->Trade[sitem].m_NewOption &= ~4;
						}
					}
					
					if(this->AllowExc(BotNum,lpObj->Trade[fitem].m_NewOption) == true)
					{
						if(fDef == 0 && sDef != 0)
						{
							if(!failed)
								lpObj->Trade[fitem].m_NewOption |= 2;
							lpObj->Trade[sitem].m_NewOption &= ~2;
						}
					}
					
					if(this->AllowExc(BotNum,lpObj->Trade[fitem].m_NewOption) == true)
					{
						if(fMoney == 0 && sMoney != 0)
						{
							if(!failed)
								lpObj->Trade[fitem].m_NewOption |= 1;
							lpObj->Trade[sitem].m_NewOption &= ~1;
						}
					}
				}
			}
		}
	}

	int a = gItemManager.InventoryInsertItem(aIndex,lpObj->Trade[fitem]);
	int b = gItemManager.InventoryInsertItem(aIndex,lpObj->Trade[sitem]);
	if(a == 255 || b == 255)
	{
		ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(883));
		goto Cancel;
	}

	if(failed)
	{
		ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(884));
	}
	else
	{
		ChatSend(&gObj[this->bot[BotNum].index],gMessage.GetMessage(885));
	}

	lpObj->Money -= this->bot[BotNum].Zen;
	
	if(this->bot[BotNum].PCPoint > 0)
	{
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,0,this->bot[BotNum].PCPoint,0); //
	}
	
	else if(this->bot[BotNum].VipMoney > 0)
	{
		gCashShop.GDCashShopSubPointSaveSend(lpObj->Index,0,this->bot[BotNum].VipMoney,0,0); //
	}
	
	gObjInventoryCommit(aIndex);
	gObjectManager.CharacterMakePreviewCharSet(aIndex);
	GDCharacterInfoSaveSend(aIndex);
	GDPetItemInfoSend(aIndex,0);
	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	lpObj->TargetNumber = -1;
	lpObj->TradeOk = 0;
	lpObj->TradeOkTime = 0;
	lpObj->TradeMoney = 0;
	GCMoneySend(aIndex,lpObj->Money);
	gTrade.GCTradeResultSend(aIndex,1);
	gItemManager.GCItemListSend(aIndex);
	gTrade.ClearTrade(lpObj);
	return 1;

Cancel:
	this->TradeCancel(aIndex);
	return 0;
}


void ObjBotAlchemist::TradeOk(int aIndex)
{
	int MixNum=-1;
	int MixItem=-1;

	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);

	if(number == -1)
	{			
		gObj[aIndex].Interface.state = 0;
		gObj[aIndex].Interface.type = 0;
		gObj[aIndex].TargetShopNumber = -1;
		gObj[aIndex].Transaction = 0;

		return;
	}
	this->Alchemy(aIndex,number);
}

BOOL ObjBotAlchemist::TradeOpen(int index, int nindex)
{
	if(gObjIsConnected(index) == 0)
	{
		return 0;
	}
	if(gObjIsConnected(nindex) == 0)
	{
		return 0;
	}

	int number = this->GetBotIndex(nindex);
	if(number == -1)
		return 0;
	char sbuf[512]={0};
	LPOBJ lpObj = &gObj[index];
	LPOBJ lpBot = &gObj[nindex];

	if(this->bot[number].OnlyVip == 1 && gObj[index].AccountLevel == 0)
	{
		ChatSend(&gObj[this->bot[number].index],gMessage.GetMessage(886));
		return 0;
	}
	
	if((lpObj->Coin1 - this->bot[number].VipMoney) < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(876),this->bot[number].VipMoney);
		return 0;
	}
	
	if((gObj[index].Coin1 - this->bot[number].PCPoint) < 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(876),this->bot[number].PCPoint);
		return 0;
	}
	if(this->Enabled == TRUE)
	{
		if ( lpObj->Interface.use > 0 )
		{
			return 0;
		}else
		{	
			for(int n = 0; n < TRADE_SIZE; n++)
			{
				lpObj->Trade[n].Clear();
			}
			memset(lpObj->TradeMap, (BYTE)-1, TRADE_SIZE );

			gObjInventoryTransaction(lpObj->Index);
			gTrade.GCTradeResponseSend(lpObj->Index,1,lpBot->Name, 400, 0);
			gTrade.GCTradeOkButtonSend(lpObj->Index, 1);
			lpObj->Interface.state = 1;
			lpObj->Interface.use = 1;
			lpObj->Interface.type = 1;
			lpObj->TradeMoney = 0;
			lpObj->TargetNumber =lpBot->Index;
			lpObj->Transaction = 1;

			char wbuf[1024]={0};
			wsprintf(wbuf,"[BotAlchemistKG] (%s)(%s) OPEN",gObj[index].Account,gObj[index].Name);
			LogAdd(LOG_RED,wbuf);
			ChatSend(&gObj[this->bot[number].index],gMessage.GetMessage(887));
			
			if(this->bot[number].PCPoint > 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(876),this->bot[number].PCPoint);;
			}
			
			if(this->bot[number].VipMoney > 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(876),this->bot[number].VipMoney);
			}
			
			if(this->bot[number].Zen > 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(874),this->bot[number].Zen);
			}				
		}
	}
	return 1;
}

void ObjBotAlchemist::TradeCancel(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
	{
		return;
	}

	gTrade.ResetTrade(aIndex);
	gTrade.GCTradeResultSend(aIndex,0);
}