#include "AllHead.h"

#if 1//WCDMA
const u8 *ucTxCIMI              ="AT+CIMI";
const u8 *ucTxZPPPOPEN          ="AT+ZPPPOPEN";
const u8 *ucTxCSQ               ="AT+CSQ?";
const u8 *ucTxPlayZtts          = "AT+ZTTS=";
const u8 *ucTxPocOpenConfig     = "at+poc=000000010001";
const u8 *ucTxReset             ="at+cfun=1,1";

const u8 *ucRxZIND      ="ZIND:8";
const u8 *ucRxCIMI      ="CIMI:";
const u8 *ucRxCMEERROR  ="CME ERROR: 10";
const u8 *ucRxZPPPSTATUS="ZPPPSTATUS: OPENED";
const u8 *ucRxCSQ = "CSQ:";

u8 KeyDownUpChoose_GroupOrUser_Flag=0;
u8 HDRCSQValue=0;//HDRCSQ��ֵ
#endif



#if 0//����
u8 BootProcess_SIMST_Flag=0;
u8 BootProcess_PPPCFG_Flag=0;
u8 ApiAtCmd_TrumpetVoicePlay_Flag=0;//���ſ��Ʊ�־λ
bool ApiAtCmd_ZTTS_Flag=FALSE;
u8 CSQ_Flag=0;
u8 CSQ99Count_Flag=0;


const u8 *ucGD83Reset  = "at^reset";
const u8 *ucRxPASTATE1 = "PASTATE:1";
const u8 *ucRxPASTATE0 = "PASTATE:0";
const u8 *ucRxZTTS0 = "ZTTS:0";
const u8 *ucCheckRssi = "AT+CSQ?";
const u8 *ucHDRCSQ = "AT^HDRCSQ";
const u8 *ucRxCSQ = "CSQ:";
const u8 *ucRxHDRCSQ = "^HDRCSQ:";
const u8 *ucGpsPosition = "LATLON:";
const u8 *ucCDMATIME = "^CDMATIME:";
const u8 *ucGPSINFO = "^GPSINFO:";
const u8 *ucGPSCNO = "^GPSCNO:";
const u8 *ucSIMST1="^SIMST:1";
const u8 *ucSIMST255="^SIMST:255";
const u8 *ucCaretPPPCFG="^PPPCFG:";
const u8 *ucCheckTcp = "at^pocsockstat=";//���TCP Ip�Ƿ���������
const u8 *ucSetIp = "at^POCSETUPUDP=0";//����TCP����1 UDP:0
const u8 *ucSendTcp = "at^POCSENDUDP=0,";//at^pocsendudp=0,123.56.80.107,6969,0x
const u8 *ucRxCheckTcp = "^POCSOCKSTAT: 0";//TCP���������·�ָ��
const u8 *ucZpppOpen = "at^pocnetopen";//����PPP����
const u8 *ucCheckPPP = "AT^POCNETOPEN?";//���PPP�����Ƿ���������
const u8 *ucRxCheckPppOpen = "^POCNETOPEN:1";
const u8 *ucRxCheckPppClose = "^POCNETOPEN:0";
const u8 *ucCheckCard = "AT^GETICCID";
bool PositionInformationSendToATPORT_Flag=FALSE;
bool PositionInfoSendToATPORT_RedLed_Flag=FALSE;
bool PositionInfoSendToATPORT_SetPPP_Flag=FALSE;
bool PositionInfoSendToATPORT_InfoDisplay_Flag=FALSE;
#endif

#if 1//WCDMA ׿�Ǵ�
typedef struct{
  union{
    struct{
      u16  bStartingUp     :1;
      u16  bCardIn         :1;
      u16  bNoCard         :1;
      u16  bPPPStatusOpen  :1;
      u16                  :13;
    }Bits;
    u8 Byte;
  }Msg;
  struct{
    u8 Buf[2];
    u8 Len;
    u8 Value;//HDRCSQ��ֵ
  }CSQParam;
  u8 Buf[30];
  u8 Len;
}AtCmdDrv;
#else
typedef struct{
	struct{
		union{
			struct{
				u16 bFunOff	: 1;
				u16 bFun	: 1;
				u16 bEsn	: 1;
				u16 bCard	: 1;
				u16 bRssi	: 1;
				u16 bPppStep	: 2;
				u16 bPppOk	: 1;
				u16 bTcp	: 1;
				u16 bTcpOk	: 1;
				u16 bUdp	: 1;
				u16 bUdpOk	: 1;
				u16 bFirstPlay  : 1;
				u16		: 3;
			}Bits;
			u16 Byte;
		}Msg;

                struct{
                  u8 Buf[21];//���AT�յ��ľ�γ����Ϣ
                  u8 BufLen;//���վ�γ����Ϣ�����ݳ���
                  u8 Longitude_Minute;//С����ǰ����
                  u32 Longitude_Second;//С��������
                  u8 Latitude_Minute;
                  u32 Latitude_Second;
                }Position;
                u8 SouXingConut;
                u16 SignalToNoiseMax;//����ź�ֵ
                bool EffectiveLocation;//�Ƿ��յ���γ����Ϣ
                u8 TimeBuf[20];//���AT�յ���ʱ����Ϣ
                u8 TimeBufLen;//����ʱ����Ϣ�����ݳ���
                u8 GpsInfoBuf[30];//���AT�յ����ٶ�����
                u8 GpsInfoBufLen;//�����ٶȵ����ݳ���
                u8 GpsCnoBuf[10];//���CNO�յ����ٶ�����
                u8 GpsCnoBufLen;//����Cno�����ݳ���
                u8 ucDate[3];//������
                u8 ucTime[3];//ʱ����
                u16 ucSpeed;
		u8 Buf[30];
		u8 Len;
                u8 HDRCSQBuf[2];
                u8 HDRCSQLen;
                u8 CSQBuf[2];
                u8 CSQLen;
	}NetState;
}AtCmdDrv;

#endif

static AtCmdDrv AtCmdDrvobj;
static void AtCmd_NetParamCode(void);//��ȡTCP IP��ַ
#if 1//WCDMA ׿�Ǵ�
bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  switch(id)
  {
  case ATCOMM_CIMI:
    DrvGD83_UART_TxCommand((u8*)ucTxCIMI, strlen((char const*)ucTxCIMI));
    break;
  case ATCOMM_ZPPPOPEN:
    DrvGD83_UART_TxCommand((u8*)ucTxZPPPOPEN, strlen((char const*)ucTxZPPPOPEN));
    break;
  case ATCOMM_CSQ:
    DrvGD83_UART_TxCommand((u8*)ucTxCSQ, strlen((char const*)ucTxCSQ));
    break;
  case ATCOMM_RESET:
    DrvGD83_UART_TxCommand((u8*)ucTxReset, strlen((char const*)ucTxReset));
    break;
  default:
    break;
  }
  r = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}
#else //CDMA ����
bool ApiAtCmd_WritCommand(AtCommType id, u8 *buf, u16 len)
{
  bool r = TRUE;
  u8 Buf1[4];
  DrvMC8332_TxPort_SetValidable(ON);
  switch(id)
  {
  case ATCOMM3_GD83StartupReset://1
    DrvGD83_UART_TxCommand((u8*)ucGD83Reset,strlen((char const *)ucGD83Reset));
    //KaiJi_Flag=FALSE;
    break;
  case ATCOMM3_GD83Reset://1
    //DrvGD83_UART_TxCommand((u8*)ucGD83Reset,strlen((char const *)ucGD83Reset));
    api_lcd_pwr_on_hint("                ");//
    main_init();
    ApiAtCmd_SetLoginState();
    BootProcess_SIMST_Flag=0;
    CSQ_Flag=0;
    TaskStartDeleteDelay1=0;
    TaskStartDeleteDelay2=0;
    TaskStartDeleteDelay3=0;
    TaskStartDeleteDelay4=1;
    TaskStartDeleteDelay5=0;
    TaskStartDeleteDelay6=0;
    TaskStartDeleteDelay1Count=0;
    TaskStartDeleteDelay3Count=0;
    TaskStartDeleteDelay4Count=0;
    TaskStartDeleteDelay6Count=0;
    break;
  case ATCOMM0_OSSYSHWID://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM1_PPPCFG://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM2_ZTTS_Abell://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM4_GD83Mode://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM5_CODECCTL://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM6_CSQ://1
    DrvGD83_UART_TxCommand((u8*)ucCheckRssi, strlen((char const*)ucCheckRssi));
    break;
  case ATCOMM15_HDRCSQ:
    DrvGD83_UART_TxCommand((u8*)ucHDRCSQ, strlen((char const*)ucHDRCSQ));
    break;
  case ATCOMM7_VGR://1
    DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM8_CheckTcp://2
    DrvGD83_UART_TxCommand((u8*)ucCheckTcp, strlen((char const*)ucCheckTcp));
    DrvGD83_UART_TxCommand(buf, len);
    break; 
  case ATCOMM9_SetIp	://1
    DrvGD83_UART_TxCommand((u8*)ucSetIp, strlen((char const *)ucSetIp));
    //DrvGD83_UART_TxCommand(buf, len);
    break;
  case ATCOMM10_SendTcp://1
    if(len <= 1024 && len != 0x00)
    {
      DrvGD83_UART_TxCommand((u8*)ucSendTcp, strlen((char const *)ucSendTcp));
      AtCmd_NetParamCode();//��ȡIP��ַ
      DrvGD83_UART_TxCommand(AtCmdDrvobj.NetState.Buf, AtCmdDrvobj.NetState.Len);
      Buf1[0]=',';
      Buf1[1]='0';
      Buf1[2]='x';
      DrvGD83_UART_TxCommand(Buf1, 3);
      DrvGD83_UART_TxCommand(buf, len);
    }
    break; 
  case ATCOMM11_ZpppOpen ://1
    DrvGD83_UART_TxCommand((u8*)ucZpppOpen, strlen((char const *)ucZpppOpen));
    break;
  case ATCOMM12_CheckPPP ://2
    DrvGD83_UART_TxCommand((u8*)ucCheckPPP, strlen((char const *)ucCheckPPP));
    break;
  case ATCOMM13_CheckRssi:
    DrvGD83_UART_TxCommand((u8*)ucCheckRssi, strlen((char const *)ucCheckRssi));
    break;
  case ATCOMM14_CheckCard:
    DrvGD83_UART_TxCommand((u8*)ucCheckCard, strlen((char const *)ucCheckCard));
    break;
  default:
    break;
  }
  r = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}
#endif


void ApiAtCmd_100msRenew(void)
{
  if(GetTaskId()==Task_NormalOperation)//��¼�ɹ�����Ⱥ��״̬
    {
    }
}
void ApiCaretCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len,i;
  while((Len = DrvMC8332_CaretNotify_Queue_front(&pBuf)) != 0)
  {
  }
}

void ApiAtCmd_10msRenew(void)
{
  u8 * pBuf, ucRet, Len, i;
  while((Len = DrvMC8332_AtNotify_Queue_front(&pBuf)) != 0)
  {
/*****����ZIND:8*********/
    ucRet = memcmp(pBuf, ucRxZIND, 6);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bStartingUp=1;
    }
/****�Ѳ忨����ȡ����CIMI*************/
    ucRet = memcmp(pBuf, ucRxCIMI, 5);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bCardIn=1;
      AtCmdDrvobj.Msg.Bits.bNoCard=0;//��ȡ������������޿���־λ
    }
/****δ�忨CMEERROR**********************/
    ucRet = memcmp(pBuf, ucRxCMEERROR, 13);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bNoCard=1;
    }
/****����ҵ�񲦺�ZPPPSTATUS*************/
    ucRet = memcmp(pBuf, ucRxZPPPSTATUS, 18);
    if(ucRet == 0x00)
    {
      AtCmdDrvobj.Msg.Bits.bPPPStatusOpen=1;
    }
/****�źŻ�ȡ���ж�CSQ*********************/
    ucRet = memcmp(pBuf, ucRxCSQ, 4);
    if(ucRet == 0x00)
    {
      if(Len > 5)//ȥ�^
      {
        Len -= 5;
      }
       for(i = 0x00; i < Len; i++)
       {
         AtCmdDrvobj.CSQParam.Buf[i] = pBuf[i + 5];//
       }
      AtCmdDrvobj.CSQParam.Len = i;
      AtCmdDrvobj.CSQParam.Value=CHAR_TO_Digital(AtCmdDrvobj.CSQParam.Buf,2);
    } 
  }
}

bool ApiAtCmd_PlayVoice(AtVoiceType id, u8 *buf, u8 len)
{
  bool r = TRUE;
  DrvMC8332_TxPort_SetValidable(ON);
  DrvGD83_UART_TxCommand((u8*)ucTxPlayZtts, strlen((char const *)ucTxPlayZtts));
  AtCmdDrvobj.Buf[0] = 0x31;	// 1
  AtCmdDrvobj.Buf[1] = 0x2C;	// ,
  AtCmdDrvobj.Buf[2] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.Buf, 3);
  switch(id)
  {
  case ATVOICE_FreePlay :
    DrvGD83_UART_TxCommand(buf, len);
    break;
  default:
    break;
  }
  AtCmdDrvobj.Buf[0] = 0x22;	// "
  DrvGD83_UART_TxCommand(AtCmdDrvobj.Buf, 1);
  r  = DrvMc8332_UART_TxTail();
  DrvMC8332_TxPort_SetValidable(OFF);
  return r;
}

u32  CHAR_TO_Digital(u8 * pBuf, u8 Len)
{
	u8 i;
	u32 buf = 0;
	for(i = 0; i < Len; i++)
	{
		buf *= 10;
		buf += (pBuf[i] - 0x30);
	}
	return buf;
}

void HDRCSQSignalIcons(void)
{
  if(MenuMode_Flag==0)
  {
    if(HDRCSQValue>=31)//5��
    {
      api_disp_icoid_output( eICO_IDSPEAKER, TRUE, TRUE);//5���ź�
    }
    else if(HDRCSQValue>=25&&HDRCSQValue<31)
    {
      api_disp_icoid_output( eICO_IDSCANPA, TRUE, TRUE);//4���ź�
    }
    else if(HDRCSQValue>=20&&HDRCSQValue<25)
    {
      api_disp_icoid_output( eICO_IDSCAN, TRUE, TRUE);//3���ź�
    }
    else if(HDRCSQValue>=15&&HDRCSQValue<20)
    {
      api_disp_icoid_output( eICO_IDRXNULL, TRUE, TRUE);//2���ź�
    }
    else if(HDRCSQValue>=10&&HDRCSQValue<15)
    {
      api_disp_icoid_output( eICO_IDRXFULL, TRUE, TRUE);//1���ź�
    }
    else
    {
      api_disp_icoid_output( eICO_IDMESSAGE, TRUE, TRUE);//0���ź�
    }
    api_disp_all_screen_refresh();// ȫ��ͳһˢ��
  }

}

u16 ApiAtCmd_bStartingUp(void)
{
  return AtCmdDrvobj.Msg.Bits.bStartingUp;
}
u16 ApiAtCmd_bCardIn(void)
{
  return AtCmdDrvobj.Msg.Bits.bCardIn;
}
u16 ApiAtCmd_bNoCard(void)
{
  return AtCmdDrvobj.Msg.Bits.bNoCard;
}
u16 ApiAtCmd_bPPPStatusOpen(void)
{
  return AtCmdDrvobj.Msg.Bits.bPPPStatusOpen;
}
u8 ApiAtCmd_CSQValue(void)
{
  return AtCmdDrvobj.CSQParam.Value;
}