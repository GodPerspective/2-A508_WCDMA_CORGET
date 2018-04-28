#include "ALLHead.h"
#if 1//WCDMA ׿�Ǵ�

#define DrvMC8332_UseId_Len	        100//define UART Tx buffer length value
#define APIPOC_GroupName_Len            32
#define APIPOC_UserName_Len             32
#define APIPOC_Group_Num                40
#define APIPOC_User_Num                 40

const u8 *ucAtPocHead           = "AT+POC=";
const u8 *ucSetIPAndID          = "010000";
const u8 *ucPocOpenConfig       ="000000010101";
u8 *ucStartPTT                  = "0B0000";
u8 *ucEndPTT                    = "0C0000";
typedef struct{
  struct{
    union{
      struct{
        u16 bUserInfo	: 3;
        u16 bUserWrite	: 1;
        u16 bPocReset	: 1;
        u16 bPocOpen	: 1;
        u16 bModeChange	: 1;
        u16 bMode	: 3;
        u16 bNetStat	: 2;
        u16 bUnline	: 1;
        u16             : 1;
        u16             : 2;
      }Bits;
      u16 Byte;
    }Msg;
    struct{
      struct{
        u8 bSet	: 1;
        u8 Len	: 7;
      }Msg;
      u8 Buf[DrvMC8332_UseId_Len];
    }LoginInfo;
  }NetState;
  
  struct{
    PocStatesType PocStatus;
    GroupStatsType GroupStats;
    u8 KeyPttState;
  }States;
  struct{
/*****����**************/
    struct{
      u8 Name[APIPOC_GroupName_Len];
      u8 NameLen;
    }AllGroupName[APIPOC_Group_Num];//����Ⱥ���Ա��
    struct{
      u8 Name[APIPOC_GroupName_Len];
      u8 NameLen;
    }NowWorkingGroupName;//��ǰ����Ⱥ���Ա��
/******����**************/
    struct{
      u8 Name[APIPOC_UserName_Len];
      u8 NameLen;
    }AllGroupUserName[APIPOC_User_Num];//Ⱥ���Ա��
    struct{
      u8 Name[APIPOC_UserName_Len];
      u8 NameLen;
    }LocalUserName;//�����û���
    struct{
      u8 Name[APIPOC_UserName_Len];
      u8 NameLen;
    }SpeakingUserName;//��ǰ˵���˵�����
/**************************/
  }NameInfo;
  u8 ReadBuffer[80];//��EEPROM��ȡ������ʹ��
}PocCmdDrv;


#else
#define DrvMC8332_GroupName_Len		16			//define UART Tx buffer length value

#define APIPOC_UserList_Len			16
#define APIPOC_UserLoad_Len			8
#define APIPOC_UserName_Len			39//39 in20180303 Ⱥ�������7λ��Ⱥ�������40��
#define APIPOC_CalledUserName_Len	        44//��Ա������

u8 POC_GetAllGroupNameStart_Flag=0;

u8 InvalidCallCount=0;
u8 ASCII_ActiveUserID[22];//Test ��EEPROM��ȡ������ʹ��
u8 Get_Unicode_ActiveUserIDBuf[45];//
u8 Get_0X_ActiveUserIDBuf[11];//
u8 UnicodeForGbk_AllUserNameBuf[25];
u8 UnicodeForGbk_AllGrounpNameBuf[25];
u8 UnicodeForGbk_MainWorkNameBuf[15];
u8 UnicodeForGbk_MainUserNameBuf[25];
u8 UnicodeForGbk_SpeakerRightnowNameBuf[25];

const u8 *ucTingEnd   = "0B0000";
const u8 *ucTingStart   = "0B0001";


u8 POC_GetAllGroupNameDone_Flag=FALSE; 
u8 POC_EnterPersonalCalling_Flag=0;
u8 POC_QuitPersonalCalling_Flag=0;
u8 POC_AtEnterPersonalCalling_Flag=0;
u8 POC_AtQuitPersonalCalling_Flag=0;
u8 POC_EnterGroupCalling_Flag=0;
u8 POC_QuitGroupCalling_Flag=0;
bool POC_ReceivedVoice_Flag=FALSE;
bool ApiPocCmd_Tone_Flag=FALSE;
bool ApiPocCmd_PlayReceivedVoice_Flag=FALSE;//��������ʹ��
bool SwitchGroupBUG=FALSE;
//bool POC_ReceivedNoVoice_Flag=FALSE;
bool EnterPersonalCalling=FALSE;
u8 POC_ReceivedVoiceStart_Flag=0;
u8 POC_ReceivedVoiceEnd_Flag=0;
u8 POC_ReceivedVoiceEndForXTSF_Flag=0;
bool POC_Receive86_Flag=FALSE;
u8 OffLineCount=0;
u8 OnlineMembership=0;

bool PocNoOnlineMember_Flag=FALSE;
bool PocNoOnlineMember_Flag2=FALSE;
bool GettheOnlineMembersDone=FALSE;
typedef struct{
  struct{
    union{
      struct{
        u16 bUserInfo	: 3;
        u16 bUserWrite	: 1;
        u16 bPocReset	: 1;
        u16 bPocOpen	: 1;
        u16 bModeChange	: 1;
        u16	 bMode	: 3;
        u16 bNetStat	: 2;
        u16 bUnline	: 1;
        u16             : 1;
        u16             : 2;
      }Bits;
      u16 Byte;
    }Msg;
    //u8 Buf[10];
    u8 Buf2[10];
    u8 Buf3[3];
    u8 Buf4[3];
    u8 Buf5[3];
    u8 Buf6[3];
    u8 Buf7[3];
    u8 Buf8[9];
    struct{
      struct{
        u8 bSet	: 1;
        u8 Len	: 7;
      }Msg;
      u8 Buf[DrvMC8332_UseId_Len];
    }LoginInfo;
  }NetState;
  struct{
    struct{
      union{
        struct{
          u16 bInitial		        : 1;
          u16 bPttState		        : 1;			//0: ptt involide; 1 :ptt volide
          u16 bPttUser		        : 1;			//ptt operrtor 0: oneself; 1: other
          u16 bWorkGrpVolide	        : 3;
          u16 bEnterGroup		: 1;
          u16 bCallFail	                : 1;
          u16 bLogin                    : 1;
          u16 AlarmMode            	: 1;
          u16 PersonalCallingMode 	: 1;
          u16 			        : 5;
        }Bits;
        u16 Byte;
      }Msg;
                        struct{
                                u8 PresentUserId;
				u8 Name[APIPOC_CalledUserName_Len];
				u8 NameLen;
			}SpeakerRightnow;
			struct{
                          
				u8 PresentGroupId;
                                u8 GroupNum;
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}MainWorkGroup;
                        struct{
                                u8 PresentUserId;
                                u8 UserNum;
				u8 Id[8];
				u8 Name[APIPOC_CalledUserName_Len];
				u8 NameLen;
			}PttUserName;
#if 1//POC:80 added by Tom in 2017.11.21
			struct{
                                u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}Group[30];//��Ⱥ�����30ʱ�ᵼ�¸��������һ��Ⱥ���쳣
                        struct{
                                u8 Id[8];
				u8 Name[APIPOC_CalledUserName_Len];
				u8 NameLen;
			}UserName[20];
#endif
			struct{
				u8 Id[8];
				u8 Name[APIPOC_UserName_Len];
				u8 NameLen;
			}WorkGroup;
			
			struct{
				u8 Id[8];
				u8 Name[APIPOC_CalledUserName_Len];
				u8 NameLen;
			}WorkUserName;
		}UseState;
	}WorkState;
}PocCmdDrv;
#endif
static PocCmdDrv PocCmdDrvobj;

#if 0//CDMA ����
void ApiPocCmd_PowerOnInitial(void)
{
  PocCmdDrvobj.NetState.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Byte = 0x00;
  PocCmdDrvobj.WorkState.UseState.Msg.Bits.bInitial = 0x01;
  PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin=0;
  
  PocCmdDrvobj.States.PocStatus=OffLine;
}
#endif

#if 1//WCDMA ׿�Ǵ�
void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len)
{
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8 *)ucAtPocHead,strlen((char const *)ucAtPocHead));
  switch(id)
  {
  case PocComm_OpenPOC://1
    DrvGD83_UART_TxCommand((u8*)ucPocOpenConfig, strlen((char const *)ucPocOpenConfig));
    break;
  case PocComm_SetParam://�����˺�����
    memset(PocCmdDrvobj.ReadBuffer,0,sizeof(PocCmdDrvobj.ReadBuffer));
    FILE_Read(0,80,PocCmdDrvobj.ReadBuffer);//80λ
    DrvGD83_UART_TxCommand((u8 *)ucSetIPAndID,strlen((char const *)ucSetIPAndID));
    DrvGD83_UART_TxCommand(PocCmdDrvobj.ReadBuffer, strlen((char const *)PocCmdDrvobj.ReadBuffer));
    break;
  case PocComm_SetURL:
      DrvGD83_UART_TxCommand((u8 *)"120000687474703a2f2f736925642e7265616c7074742e636f6d3a32393939392f00",strlen((char const *)"120000687474703a2f2f736925642e7265616c7074742e636f6d3a32393939392f00"));
  case PocComm_Login:
    break;
  case PocComm_Logout:
  case PocComm_Cancel:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_ModifyPwd:
    break;
  case PocComm_EnterGroup:
    break;
  case PocComm_Invite:
    break;
  case PocComm_StartPTT://3
    DrvGD83_UART_TxCommand(ucStartPTT,strlen((char const *)ucStartPTT));
    break;
  case PocComm_EndPTT://4
    DrvGD83_UART_TxCommand(ucEndPTT,strlen((char const *)ucEndPTT));
    break;
  case PocComm_GroupListInfo://5
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_UserListInfo://6
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_Key://7
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(ON);
}
#else //CDMA����
void ApiPocCmd_WritCommand(PocCommType id, u8 *buf, u16 len)
{
  u8 NetStateBuf[5]={0,0,0,0,0};
  u8 testData=0;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8 *)ucAtPocHead,strlen((char const *)ucAtPocHead));
  switch(id)
  {
  case PocComm_OpenPOC://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_SetParam://�����˺�����
    DrvGD83_UART_TxCommand((u8 *)ucSetIPAndID,strlen((char const *)ucSetIPAndID));
    //FILE_Read(0,80,ReadBuffer);//80λ

   // FILE_Read(28,22,ActiveUserID);
    //FILE_Read(0x230,250,TestReadBuffer);//0x260-0x2cc
    DrvGD83_UART_TxCommand(PocCmdDrvobj.ReadBuffer, strlen((char const *)PocCmdDrvobj.ReadBuffer));
    break;
  case PocComm_GetParam:
  case PocComm_Login:
    break;
  case PocComm_Logout:
  case PocComm_Cancel:
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_ModifyPwd:
    break;
  case PocComm_EnterGroup:
    DrvGD83_UART_TxCommand("090000000000", 12);
#if 0//���Ⱥ�鳬��10���Ļ����쳣��BUG
    PocCmdDrvobj.NetState.Buf[0] = (GroupCallingNum/10)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf[1] = (GroupCallingNum%10)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf, 2);
#else
    COML_HexToAsc(GroupCallingNum, NetStateBuf);
    if(strlen((char const*)NetStateBuf)==1)
    {
      NetStateBuf[1]=NetStateBuf[0];
      NetStateBuf[0]=0x30;
    }
    else
    {
      testData          =NetStateBuf[0];
      NetStateBuf[0]    =NetStateBuf[1];
      NetStateBuf[1]    =testData;
    }
    DrvGD83_UART_TxCommand(NetStateBuf, 2);
#endif
    break;
  case PocComm_Invite:
    DrvGD83_UART_TxCommand("0A0000000000", 12);
    PocCmdDrvobj.NetState.Buf2[0] = (((PersonalCallingNum+0x64)&0xf0)>>4)+0x30;	// 0x03+0x30
    PocCmdDrvobj.NetState.Buf2[1] = ((PersonalCallingNum+0x64)&0x0f)+0x30;
    DrvGD83_UART_TxCommand(PocCmdDrvobj.NetState.Buf2, 2);
    break;
  case PocComm_StartPTT://3
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_EndPTT://4
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_GroupListInfo://5
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_UserListInfo://6
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case PocComm_Key://7
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(ON);
}
#endif


//��EEPROM�ж�ȡ���ݴ���дƵ����
u8 ApiPocCmd_user_info_get(u8 ** pBuf)
{
  u8 Len=0;
  Len = Combine2Hex(PocCmdDrvobj.ReadBuffer, strlen((char const *)PocCmdDrvobj.ReadBuffer), PocCmdDrvobj.ReadBuffer);
  *pBuf = PocCmdDrvobj.ReadBuffer;
  return (strlen((char const *)PocCmdDrvobj.ReadBuffer))/2;
}

//дƵд�����ݴ���EEPROM
bool ApiPocCmd_user_info_set(u8 *pBuf, u8 len)//cTxBufΪ���ip�˺��������Ϣ
{
	bool r;
	u8 i, uRet = 0;
	//ADRLEN_INF	adr;

	for(i = 0; i < len; i++)
	{
		if(pBuf[i] == ';')
		{
			uRet++;
		}
	}
	if(uRet >= 2)
	{
		Dec2Hex(pBuf, len, PocCmdDrvobj.NetState.LoginInfo.Buf);//���յ�����ת��Ϊ�ַ���//LoginInfo.BufΪ���
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len << 0x01;//ΪʲôҪlen*2��
		PocCmdDrvobj.NetState.LoginInfo.Msg.bSet = ON;
		//adr = CFG_GetCurAdr(ADR_IDLocalUserInfo);
		//FILE_Write(adr.Adr,adr.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                //FILE_Write(0,PocCmdDrvobj.NetState.LoginInfo.Msg.Len,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
                FILE_Write(0,90,(u8*)(&PocCmdDrvobj.NetState.LoginInfo));
		for(i = 0; i < len; i++)
		{
			PocCmdDrvobj.NetState.LoginInfo.Buf[i] = pBuf[i];
		}
		PocCmdDrvobj.NetState.LoginInfo.Msg.Len = len;
		
		//SYS_BufReset();
#if 0//WCDMA ׿�Ǵ�
		PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0;
#endif
		PocCmdDrvobj.NetState.Msg.Bits.bUserInfo = 3;
		PocCmdDrvobj.NetState.Msg.Bits.bUserWrite = ON;
		r = TRUE;
	}
	else
	{
		r = FALSE;
	}
	return r;
}

#if 1 //WCDMA ׿�Ǵ�
void ApiPocCmd_10msRenew(void)
{
  u8 ucId, Len;
  u8 * pBuf;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x02://��ȡPOC����
      break;
    case 0x09://����һ��Ⱥ��
      break;
    case 0x0A://����ĳ�û�
      break;
    case 0x0B://����PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        PocCmdDrvobj.States.KeyPttState=1;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
      }
      break;
    case 0x0C://�ж��ɿ�PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        PocCmdDrvobj.States.KeyPttState=3;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
      }
      break;
    case 0x0D://��ȡȺ����Ϣ
      break;
    case 0x0e://��ȡ���Ա��Ϣ
      break;
    case 0x11://�ϱ���γ��
      break;
    case 0x12://����URL����
      break;
    case 0x13://��ȡ���Ա����
      break;
    case 0x16://���ú���ʱ������
      break;
    case 0x28://��ȡ����ʱ��
      break;
    case 0x80://Ⱥ���б�
      break;
    case 0x81://���Ա�б�
      break;
    case 0x82://��¼״̬�������û���
      ucId = COML_AscToHex(pBuf+2, 0x02);
      switch(ucId)
      {
      case 0x00://����
        PocCmdDrvobj.States.PocStatus=OffLine;
        break;
      case 0x01://��½��
        PocCmdDrvobj.States.PocStatus=Landing;
        break;
      case 0x02://��¼�ɹ�
        PocCmdDrvobj.States.PocStatus=LandSuccess;
        //PocCmdDrvobj.LocalUserName.Name
        break;
      case 0x03://ע����
        PocCmdDrvobj.States.PocStatus=Logout;
        break;
      default:
        break;
      }
      break;
    case 0x83://���ؽ����û���Ϣ
      break;
    case 0x84://������ʾ��Ϣ
      break;
    case 0x85://֪ͨ��������Ա��Ϣ
      break;
    case 0x86://֪ͨ�û�����Ⱥ����Ϣ
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x00)
      {
        PocCmdDrvobj.States.GroupStats=EnterGroup;
      }
      else if(ucId==0xff)
      {
        PocCmdDrvobj.States.GroupStats=LeaveGroup;
      }
      else
      {}
        
      break;
    case 0x87://֪ͨ�û�������Ϣ
      break;
    case 0x88://֪ͨ����Ⱥ����Ϣ
      break;
    case 0x8A://֪ͨ���յ���Ϣ
      break;
    case 0x8B://֪ͨ��Ƶ����״̬
      break;
    case 0x8C://֪ͨ���������ն˷�������Ϣ
      break;
    default:
      break;
    }
  }
}

#else //CDMA ����

void ApiPocCmd_10msRenew(void)
{
  u8 ucId,i, Len;
  u8 * pBuf, ucRet;
  while((Len = DrvMC8332_PocNotify_Queue_front(&pBuf)) != 0x00)
  {
    ucId = COML_AscToHex(pBuf, 0x02);
    switch(ucId)
    {
    case 0x0A://�жϽ���״̬
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        if(TASK_Ptt_StartPersonCalling_Flag==TRUE)//�������PTT������ĳ�û�
        {
          EnterPersonalCalling=TRUE;
        }
      }
      break;
    case 0x0B://�жϰ���PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        KeyPttState=1;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
      }
      break;
    case 0x0C://�ж��ɿ�PTT
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId==0x00)
      {
        KeyPttState=3;//KeyPttState 0��δ��PTT 1:����ptt˲��  2����סPTT״̬ 3���ɿ�PTT˲��
      }
      break;  
    case 0x0e://�����û�����
      ucId = COML_AscToHex(pBuf+8, 0x04);
      //0e0000000007
      PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;
      if(Len==12)
      {
        if(PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum==0)
        {
          PocNoOnlineMember_Flag=TRUE;
        }
        else
        { 
          PocCmdDrvobj.WorkState.UseState.PttUserName.UserNum = ucId;
        }
      }
      break;
    case 0x0d://Ⱥ�����
      ucId = COML_AscToHex(pBuf+10, 0x02);
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum = ucId;
      break;
    case 0x80://��ȡ�������б�
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(Len >= 24)//���Ⱥ��id���滹��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 24;
      }
      else//��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+24];//�����ȡ��Ⱥ����
        PocCmdDrvobj.WorkState.UseState.Group[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.Group[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
      if(ucId==PocCmdDrvobj.WorkState.UseState.MainWorkGroup.GroupNum)
      {
      }
      POC_GetAllGroupNameStart_Flag=TRUE; 
      break;
    case 0x81://��ȡ���ڳ�Ա�б�
      ucId = COML_AscToHex(pBuf+10, 0x02);//
      if(Len >= 20)//����û�ID���滹���û���
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = Len - 20;
      }
      else//���û���
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i] = pBuf[i+20];//�����ȡ��Ⱥ����
        PocCmdDrvobj.WorkState.UseState.UserName[ucId].Name[i] = 
          PocCmdDrvobj.WorkState.UseState.WorkUserName.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.UserName[ucId].NameLen = PocCmdDrvobj.WorkState.UseState.WorkUserName.NameLen;
#if 1
          GettheOnlineMembersDone=TRUE;
#endif
      break;
    case 0x82://�ж��Ƿ��¼�ɹ�
      ucId = COML_AscToHex(pBuf+3, 0x01);
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x01;
        OffLineCount=0;
      }
      else if(ucId == 0x00)
      {
        OffLineCount++;
        if(OffLineCount>=5)
        {
          //ApiAtCmd_WritCommand(ATCOMM3_GD83Reset,(void*)0, 0);
          OffLineCount=0;
        }
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x00;
      }
      else
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.bLogin = 0x00;
      }
      break;
/********�ж��Ƿ��Ǳ���״̬******************************/
    case 0x83:
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        //830000000001 07592875268df7533100f7530000
        //830000000001 310039003800300030003300300037003400370035000000
        //830000000001 310039003800300030003300300037003400370035000000
        if(Len >= 12)
        {
          PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = Len - 12;
#if 1//���Խ��Ⱥ���ڱ�����ʾֻ��8λ������
          if(InvalidCallCount==1)
          {
            InvalidCallCount=0;
          }
          else
          {
            if(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen > APIPOC_CalledUserName_Len)
          {
            PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = APIPOC_CalledUserName_Len;
            //����л�Ⱥ����ֻ�Ȩ�·�ָ����½��� 
            //PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = 0;
          }
          POC_ReceivedVoice_Flag=TRUE;
          POC_ReceivedVoiceStart_Flag=2;//0:���� 1���յ����� 2���տ�ʼ����
          POC_ReceivedVoiceEnd_Flag=1;//0:���� 1���յ����� 2���ս�������
          POC_ReceivedVoiceEndForXTSF_Flag=1;
          }

#else
          if(PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen > APIPOC_CalledUserName_Len)
          {
            //PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = APIPOC_CalledUserName_Len;
            //����л�Ⱥ����ֻ�Ȩ�·�ָ����½��� 
            PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen = 0;
          }
          else
          {
            POC_ReceivedVoice_Flag=TRUE;
            POC_ReceivedVoiceStart_Flag=2;//0:���� 1���յ����� 2���տ�ʼ����
            POC_ReceivedVoiceEnd_Flag=1;//0:���� 1���յ����� 2���ս�������
            POC_ReceivedVoiceEndForXTSF_Flag=1;
          }
#endif
        }
        for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.NameLen; i++)
        {
          PocCmdDrvobj.WorkState.UseState.SpeakerRightnow.Name[i] = pBuf[i+12];//���뵱ǰ˵���˵�����
        }
      }
      if(ucId == 0xff)
      {
        //Set_GreenLed(LED_ON);
        if(SwitchGroupBUG==TRUE)
        {
#if 0//����������ģʽ����סPTT�����ͣ���������һ�����̵��ܽ��յ��������ɿ�PTT�̵Ƴ���������
            POC_ReceivedVoice_Flag=FALSE;
            POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
            POC_ReceivedVoiceEndForXTSF_Flag=2;
            POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����
            api_disp_icoid_output( eICO_IDTALKAR, TRUE, TRUE);//�޷����޽����ź�ͼ��
            api_disp_all_screen_refresh();// ȫ��ͳһˢ��
            SwitchGroupBUG=FALSE;
#endif
        }
        else
        {
          POC_ReceivedVoice_Flag=FALSE;
          POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
          POC_ReceivedVoiceEndForXTSF_Flag=2;
          //POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����//���Խ����������
        }
        //POC_ReceivedNoVoice_Flag=FALSE;
      }
      break;
/**************************************/
    case 0x8B:
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x00)//����ͨ������
      {
        ApiPocCmd_PlayReceivedVoice_Flag=FALSE;
      }
      if(ucId==0x01)//����ͨ����ʼ
      {
        ApiPocCmd_PlayReceivedVoice_Flag=TRUE;
      }
      if(ucId==0x03)//tone��
      {
        ApiPocCmd_Tone_Flag=TRUE;
#if 1//���յ�Tone������ZTTS��0
        ApiAtCmd_ZTTS_Flag=FALSE;
#endif
      }

      break;
    case 0x86:
      //InvalidCallCount=1;
      POC_Receive86_Flag=TRUE;
/****************�жϽ��뵥��**************************************************************/
      ucId = COML_AscToHex(pBuf+4, 0x02);
      if(ucId==0x0a)//���뵥��
      { 
        POC_EnterPersonalCalling_Flag=2;//�����жϽ��뵥��,0:���� 2�����ڽ��뵥�� 1������״̬
        POC_QuitPersonalCalling_Flag=1;//�����ж��˳�����
      }
      else
      {
#if 0//test���Ե������鲻������BUG
        if(ucId==0x00)//�յ����˳��������˳�����������״̬��
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
        }
#else
        if(POC_QuitPersonalCalling_Flag==1)//�յ����˳��������˳�����������״̬��
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
        }
#endif
      }
/****************Ⱥ��״̬�жϼ�Ⱥ����Ϣ��ȡ**************************************************************/
#if 1//��Ⱥ�����뵥�����ֿ�
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        if(POC_EnterPersonalCalling_Flag==1)
        {
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=2;
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=0;
        }
        if(POC_AtEnterPersonalCalling_Flag==1)
        {
          POC_AtEnterPersonalCalling_Flag=0;
          POC_AtQuitPersonalCalling_Flag=2;
          POC_EnterPersonalCalling_Flag=0;
          POC_QuitPersonalCalling_Flag=0;
        }
        POC_EnterGroupCalling_Flag=0;//0Ĭ�� 1��Ⱥ���� 2���ڽ���
        POC_QuitGroupCalling_Flag=2;//0Ĭ�� 1��Ⱥ���� 2�����˳�
      }
      else//���ڽ���Ⱥ��򵥺�
      {
          if(EnterPersonalCalling==TRUE)
          {
            POC_AtEnterPersonalCalling_Flag=2;
            POC_AtQuitPersonalCalling_Flag=1;
            EnterPersonalCalling=FALSE;
            TASK_Ptt_StartPersonCalling_Flag=FALSE;
          }
#if 1//�������״̬�»����PTT��ʾ�������̵���
          if(POC_AtEnterPersonalCalling_Flag==0)//�������ģʽ����ʾ������ǰ����ʾһ��Ⱥ������BUG
          {
            POC_ReceivedVoice_Flag=FALSE;
            POC_ReceivedVoiceEnd_Flag=2;//0:���� 1���յ����� 2���ս�������
            POC_ReceivedVoiceEndForXTSF_Flag=2;
            POC_ReceivedVoiceStart_Flag=0;//0:���� 1���յ����� 2���տ�ʼ����
          }

#endif
        if(PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode == 0x01)//����ǽ��뵥��ģʽ��86���뵥����
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;
            if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen > APIPOC_CalledUserName_Len)
              {
                PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = APIPOC_CalledUserName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen = 0x00;
            }
            for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.PttUserName.NameLen; i++)
            {
              PocCmdDrvobj.WorkState.UseState.PttUserName.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
            }
        }
        else//���������
        {
          POC_EnterGroupCalling_Flag=2;
          POC_QuitGroupCalling_Flag=1;
          PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;
          ucId = 0x00;
          for(i = 0x00; i < 0x08; i++)
          {
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
            if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=fΪ�뿪Ⱥ��
              ucId++;
          }
          if(ucId==0x00)//���Ϊָ������뿪Ⱥ��
          {}
          else//r���Ϊ��Ⱥ����
          {
            if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 12;
              if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > APIPOC_UserName_Len)
              {
                PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = APIPOC_UserName_Len;
              }
            }
            else//��Ⱥ����
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
            }
            for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
            {
              PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
              PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
            }
            PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
          }
        }
      }

#else
      ucId = COML_AscToHex(pBuf+10, 0x02);
      if(ucId==0xff)
      {
        POC_EnterGroupCalling_Flag=0;//0Ĭ�� 1��Ⱥ���� 2���ڽ���
        POC_QuitGroupCalling_Flag=2;//0Ĭ�� 1��Ⱥ���� 2�����˳�
      }
      else//���ڽ���Ⱥ��
      {
        POC_EnterGroupCalling_Flag=2;
        POC_QuitGroupCalling_Flag=1;
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.PresentGroupId = ucId;

      ucId = 0x00;
      for(i = 0x00; i < 0x08; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] = pBuf[i+4];
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Id[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i];
      if(PocCmdDrvobj.WorkState.UseState.WorkGroup.Id[i] != 'f') //=fΪ�뿪Ⱥ��
        ucId++;
      }
      if(ucId==0x00)//���Ϊָ������뿪Ⱥ��
      {
      }
      else//r���Ϊ��Ⱥ����
      {
        if(Len >= 12)//���Ⱥ��id���滹��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = Len - 12;
        if(PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen > APIPOC_UserName_Len)
        {
          PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = APIPOC_UserName_Len;
        }
      }
      else//��Ⱥ����
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen = 0x00;
      }
      for(i = 0x00; i < PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen; i++)
      {
        PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i] = pBuf[i+12];//�����ȡ��Ⱥ����
        PocCmdDrvobj.WorkState.UseState.MainWorkGroup.Name[i] = 
            PocCmdDrvobj.WorkState.UseState.WorkGroup.Name[i];
      }
      PocCmdDrvobj.WorkState.UseState.MainWorkGroup.NameLen = PocCmdDrvobj.WorkState.UseState.WorkGroup.NameLen;
      }
      }
#endif
      break;
    case 0x91://֪ͨ����ĳ��ģʽ�������˳�һ���澯������ģʽ��
      ucId = COML_AscToHex(pBuf+2, 0x02);
      if(ucId == 0x00)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x00;//�˳�һ���澯
      }
      if(ucId == 0x01)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.AlarmMode = 0x01;//����һ���澯
      }
      if(ucId == 0x02)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x01;//���뵥��
      }
      if(ucId == 0x03)
      {
        PocCmdDrvobj.WorkState.UseState.Msg.Bits.PersonalCallingMode = 0x00;//�˳����� 
        //AUDIO_IOAFPOW(ON);//�򿪹��ţ����'���ģʽ'��������

      }
      break;
    default:
      break;
    }
  }
}
#endif


GroupStatsType ApiPocCmd_GroupStats(void)
{
  return PocCmdDrvobj.States.GroupStats;
}
u8 ApiPocCmd_KeyPttState(void)
{
  return PocCmdDrvobj.States.KeyPttState;
}
void ApiPocCmd_SetKeyPttState(u8 i)
{
  PocCmdDrvobj.States.KeyPttState=i;
}