/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v2.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"

/* USER CODE BEGIN Includes */
#include "fatfs.h"
#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
#define f_size(fp) ((fp)->obj.objsize)
FATFS USBDISKFatFs;
FIL MyFile;
char USBDISKPath[4] = {48, 58, 47, 0};
USBH_HandleTypeDef hUSB_Host;
char *arquivo;
int opened_image = 0;
int file_count = 1;
DWORD size_image = 0;
DWORD size_count = 0;
extern uint8_t read_image[2048];
extern uint32_t bytes_read;
extern uint8_t transmitido_anterior = 1;
extern uint8_t transmitir = 0;
extern uint8_t lernovo = 1;
extern volatile StateMachine_TypeDef main_state;

char * imagesss(int fc);
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */
void USB_Error_Handler(void)
{
  /* USER CODE BEGIN USB_Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
	HAL_Delay(1);
  /* USER CODE END USB_Error_Handler */
}

static void MSC_Application(void)
{
 	//montar o pendrive
	while(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 1) != FR_OK);


}

void cam_image(void)
{
	FRESULT res;
	if(Appli_state == APPLICATION_READY)
	{
		if(file_count < 354)
		{
			if(!opened_image) 	//arquivo aberto?
			{	//não, então abrir
				arquivo = imagesss(file_count);
				if(f_open(&MyFile, arquivo, FA_READ) != FR_OK)	//abrir arquivo
				{
					opened_image = 0;	//tentar reabir na proxima
				}
				else
				{
					f_size(&MyFile);	//informacao do tamanho do arquivo
					size_image = MyFile.obj.objsize;	//salvar tamanho
					size_count = 0;		//resetar contador de leitura
					opened_image = 1;	//imagem aberta com sucesso
				}

			}
			else
			{	//já aberto
				if(transmitido_anterior)	//se pacote anterior foi transmitido
				{
					if(size_count == size_image)	//se contador de leitura = tamanho total da imagem
					{
						f_close(&MyFile);			//fechar aquivo
						opened_image = 0;			//arquivo fecahdo
						file_count++;				//ir para proximo arquivo
						main_state = DEINIT_ESP_STATE;
					}
					else
					{
						if(lernovo)	//ler novo?
						{
							f_lseek(&MyFile, size_count);	//atualizar ponteiro de leitura
							res = f_read(&MyFile, read_image, sizeof(read_image), (void *)&bytes_read);	//ler e salvar no buffer em read_image
							if((bytes_read == 0) || (res != FR_OK))
							{
								transmitir = 0;	//não transmitir, tentará ler novamente na proxima
							}
							else	//lido com sucesso
							{
								size_count = size_count + bytes_read;	//atualizar contador de leitura
								transmitir = 1;							//pode transmitir
							}
						}
						else
							HAL_Delay(10);	//nop
					}
				}
				else
					HAL_Delay(10);	//nop
			}
		}
		else file_count = 1;	//voltar ao primeiro arquivo
	}
}

char * imagesss(int fc)
{
	char *retorno;
	switch(fc)
	{
		case 1:
			retorno = "i001.jpg";
			break;
		case 2:
			retorno = "i002.jpg";
			break;
		case 3:
			retorno = "i003.jpg";
			break;
		case 4:
			retorno = "i004.jpg";
			break;
		case 5:
			retorno = "i005.jpg";
			break;
		case 6:
			retorno = "i006.jpg";
			break;
		case 7:
			retorno = "i007.jpg";
			break;
		case 8:
			retorno = "i008.jpg";
			break;
		case 9:
			retorno = "i009.jpg";
			break;
		case 10:
			retorno = "i010.jpg";
			break;
		case 11:
			retorno = "i011.jpg";
			break;
		case 12:
			retorno = "i012.jpg";
			break;
		case 13:
			retorno = "i013.jpg";
			break;
		case 14:
			retorno = "i014.jpg";
			break;
		case 15:
			retorno = "i015.jpg";
			break;
		case 16:
			retorno = "i016.jpg";
			break;
		case 17:
			retorno = "i017.jpg";
			break;
		case 18:
			retorno = "i018.jpg";
			break;
		case 19:
			retorno = "i019.jpg";
			break;
		case 20:
			retorno = "i020.jpg";
			break;
		case 21:
			retorno = "i021.jpg";
			break;
		case 22:
			retorno = "i022.jpg";
			break;
		case 23:
			retorno = "i023.jpg";
			break;
		case 24:
			retorno = "i024.jpg";
			break;
		case 25:
			retorno = "i025.jpg";
			break;
		case 26:
			retorno = "i026.jpg";
			break;
		case 27:
			retorno = "i027.jpg";
			break;
		case 28:
			retorno = "i028.jpg";
			break;
		case 29:
			retorno = "i029.jpg";
			break;
		case 30:
			retorno = "i030.jpg";
			break;
		case 31:
			retorno = "i031.jpg";
			break;
		case 32:
			retorno = "i032.jpg";
			break;
		case 33:
			retorno = "i033.jpg";
			break;
		case 34:
			retorno = "i034.jpg";
			break;
		case 35:
			retorno = "i035.jpg";
			break;
		case 36:
			retorno = "i036.jpg";
			break;
		case 37:
			retorno = "i037.jpg";
			break;
		case 38:
			retorno = "i038.jpg";
			break;
		case 39:
			retorno = "i039.jpg";
			break;
		case 40:
			retorno = "i040.jpg";
			break;
		case 41:
			retorno = "i041.jpg";
			break;
		case 42:
			retorno = "i042.jpg";
			break;
		case 43:
			retorno = "i043.jpg";
			break;
		case 44:
			retorno = "i044.jpg";
			break;
		case 45:
			retorno = "i045.jpg";
			break;
		case 46:
			retorno = "i046.jpg";
			break;
		case 47:
			retorno = "i047.jpg";
			break;
		case 48:
			retorno = "i048.jpg";
			break;
		case 49:
			retorno = "i049.jpg";
			break;
		case 50:
			retorno = "i050.jpg";
			break;
		case 51:
			retorno = "i051.jpg";
			break;
		case 52:
			retorno = "i052.jpg";
			break;
		case 53:
			retorno = "i053.jpg";
			break;
		case 54:
			retorno = "i054.jpg";
			break;
		case 55:
			retorno = "i055.jpg";
			break;
		case 56:
			retorno = "i056.jpg";
			break;
		case 57:
			retorno = "i057.jpg";
			break;
		case 58:
			retorno = "i058.jpg";
			break;
		case 59:
			retorno = "i059.jpg";
			break;
		case 60:
			retorno = "i060.jpg";
			break;
		case 61:
			retorno = "i061.jpg";
			break;
		case 62:
			retorno = "i062.jpg";
			break;
		case 63:
			retorno = "i063.jpg";
			break;
		case 64:
			retorno = "i064.jpg";
			break;
		case 65:
			retorno = "i065.jpg";
			break;
		case 66:
			retorno = "i066.jpg";
			break;
		case 67:
			retorno = "i067.jpg";
			break;
		case 68:
			retorno = "i068.jpg";
			break;
		case 69:
			retorno = "i069.jpg";
			break;
		case 70:
			retorno = "i070.jpg";
			break;
		case 71:
			retorno = "i071.jpg";
			break;
		case 72:
			retorno = "i072.jpg";
			break;
		case 73:
			retorno = "i073.jpg";
			break;
		case 74:
			retorno = "i074.jpg";
			break;
		case 75:
			retorno = "i075.jpg";
			break;
		case 76:
			retorno = "i076.jpg";
			break;
		case 77:
			retorno = "i077.jpg";
			break;
		case 78:
			retorno = "i078.jpg";
			break;
		case 79:
			retorno = "i079.jpg";
			break;
		case 80:
			retorno = "i080.jpg";
			break;
		case 81:
			retorno = "i081.jpg";
			break;
		case 82:
			retorno = "i082.jpg";
			break;
		case 83:
			retorno = "i083.jpg";
			break;
		case 84:
			retorno = "i084.jpg";
			break;
		case 85:
			retorno = "i085.jpg";
			break;
		case 86:
			retorno = "i086.jpg";
			break;
		case 87:
			retorno = "i087.jpg";
			break;
		case 88:
			retorno = "i088.jpg";
			break;
		case 89:
			retorno = "i089.jpg";
			break;
		case 90:
			retorno = "i090.jpg";
			break;
		case 91:
			retorno = "i091.jpg";
			break;
		case 92:
			retorno = "i092.jpg";
			break;
		case 93:
			retorno = "i093.jpg";
			break;
		case 94:
			retorno = "i094.jpg";
			break;
		case 95:
			retorno = "i095.jpg";
			break;
		case 96:
			retorno = "i096.jpg";
			break;
		case 97:
			retorno = "i097.jpg";
			break;
		case 98:
			retorno = "i098.jpg";
			break;
		case 99:
			retorno = "i099.jpg";
			break;
		case 100:
			retorno = "i100.jpg";
			break;
		case 101:
			retorno = "i101.jpg";
			break;
		case 102:
			retorno = "i102.jpg";
			break;
		case 103:
			retorno = "i103.jpg";
			break;
		case 104:
			retorno = "i104.jpg";
			break;
		case 105:
			retorno = "i105.jpg";
			break;
		case 106:
			retorno = "i106.jpg";
			break;
		case 107:
			retorno = "i107.jpg";
			break;
		case 108:
			retorno = "i108.jpg";
			break;
		case 109:
			retorno = "i109.jpg";
			break;
		case 110:
			retorno = "i110.jpg";
			break;
		case 111:
			retorno = "i111.jpg";
			break;
		case 112:
			retorno = "i112.jpg";
			break;
		case 113:
			retorno = "i113.jpg";
			break;
		case 114:
			retorno = "i114.jpg";
			break;
		case 115:
			retorno = "i115.jpg";
			break;
		case 116:
			retorno = "i116.jpg";
			break;
		case 117:
			retorno = "i117.jpg";
			break;
		case 118:
			retorno = "i118.jpg";
			break;
		case 119:
			retorno = "i119.jpg";
			break;
		case 120:
			retorno = "i120.jpg";
			break;
		case 121:
			retorno = "i121.jpg";
			break;
		case 122:
			retorno = "i122.jpg";
			break;
		case 123:
			retorno = "i123.jpg";
			break;
		case 124:
			retorno = "i124.jpg";
			break;
		case 125:
			retorno = "i125.jpg";
			break;
		case 126:
			retorno = "i126.jpg";
			break;
		case 127:
			retorno = "i127.jpg";
			break;
		case 128:
			retorno = "i128.jpg";
			break;
		case 129:
			retorno = "i129.jpg";
			break;
		case 130:
			retorno = "i130.jpg";
			break;
		case 131:
			retorno = "i131.jpg";
			break;
		case 132:
			retorno = "i132.jpg";
			break;
		case 133:
			retorno = "i133.jpg";
			break;
		case 134:
			retorno = "i134.jpg";
			break;
		case 135:
			retorno = "i135.jpg";
			break;
		case 136:
			retorno = "i136.jpg";
			break;
		case 137:
			retorno = "i137.jpg";
			break;
		case 138:
			retorno = "i138.jpg";
			break;
		case 139:
			retorno = "i139.jpg";
			break;
		case 140:
			retorno = "i140.jpg";
			break;
		case 141:
			retorno = "i141.jpg";
			break;
		case 142:
			retorno = "i142.jpg";
			break;
		case 143:
			retorno = "i143.jpg";
			break;
		case 144:
			retorno = "i144.jpg";
			break;
		case 145:
			retorno = "i145.jpg";
			break;
		case 146:
			retorno = "i146.jpg";
			break;
		case 147:
			retorno = "i147.jpg";
			break;
		case 148:
			retorno = "i148.jpg";
			break;
		case 149:
			retorno = "i149.jpg";
			break;
		case 150:
			retorno = "i150.jpg";
			break;
		case 151:
			retorno = "i151.jpg";
			break;
		case 152:
			retorno = "i152.jpg";
			break;
		case 153:
			retorno = "i153.jpg";
			break;
		case 154:
			retorno = "i154.jpg";
			break;
		case 155:
			retorno = "i155.jpg";
			break;
		case 156:
			retorno = "i156.jpg";
			break;
		case 157:
			retorno = "i157.jpg";
			break;
		case 158:
			retorno = "i158.jpg";
			break;
		case 159:
			retorno = "i159.jpg";
			break;
		case 160:
			retorno = "i160.jpg";
			break;
		case 161:
			retorno = "i161.jpg";
			break;
		case 162:
			retorno = "i162.jpg";
			break;
		case 163:
			retorno = "i163.jpg";
			break;
		case 164:
			retorno = "i164.jpg";
			break;
		case 165:
			retorno = "i165.jpg";
			break;
		case 166:
			retorno = "i166.jpg";
			break;
		case 167:
			retorno = "i167.jpg";
			break;
		case 168:
			retorno = "i168.jpg";
			break;
		case 169:
			retorno = "i169.jpg";
			break;
		case 170:
			retorno = "i170.jpg";
			break;
		case 171:
			retorno = "i171.jpg";
			break;
		case 172:
			retorno = "i172.jpg";
			break;
		case 173:
			retorno = "i173.jpg";
			break;
		case 174:
			retorno = "i174.jpg";
			break;
		case 175:
			retorno = "i175.jpg";
			break;
		case 176:
			retorno = "i176.jpg";
			break;
		case 177:
			retorno = "i177.jpg";
			break;
		case 178:
			retorno = "i178.jpg";
			break;
		case 179:
			retorno = "i179.jpg";
			break;
		case 180:
			retorno = "i180.jpg";
			break;
		case 181:
			retorno = "i181.jpg";
			break;
		case 182:
			retorno = "i182.jpg";
			break;
		case 183:
			retorno = "i183.jpg";
			break;
		case 184:
			retorno = "i184.jpg";
			break;
		case 185:
			retorno = "i185.jpg";
			break;
		case 186:
			retorno = "i186.jpg";
			break;
		case 187:
			retorno = "i187.jpg";
			break;
		case 188:
			retorno = "i188.jpg";
			break;
		case 189:
			retorno = "i189.jpg";
			break;
		case 190:
			retorno = "i190.jpg";
			break;
		case 191:
			retorno = "i191.jpg";
			break;
		case 192:
			retorno = "i192.jpg";
			break;
		case 193:
			retorno = "i193.jpg";
			break;
		case 194:
			retorno = "i194.jpg";
			break;
		case 195:
			retorno = "i195.jpg";
			break;
		case 196:
			retorno = "i196.jpg";
			break;
		case 197:
			retorno = "i197.jpg";
			break;
		case 198:
			retorno = "i198.jpg";
			break;
		case 199:
			retorno = "i199.jpg";
			break;
		case 200:
			retorno = "i200.jpg";
			break;
		case 201:
			retorno = "i201.jpg";
			break;
		case 202:
			retorno = "i202.jpg";
			break;
		case 203:
			retorno = "i203.jpg";
			break;
		case 204:
			retorno = "i204.jpg";
			break;
		case 205:
			retorno = "i205.jpg";
			break;
		case 206:
			retorno = "i206.jpg";
			break;
		case 207:
			retorno = "i207.jpg";
			break;
		case 208:
			retorno = "i208.jpg";
			break;
		case 209:
			retorno = "i209.jpg";
			break;
		case 210:
			retorno = "i210.jpg";
			break;
		case 211:
			retorno = "i211.jpg";
			break;
		case 212:
			retorno = "i212.jpg";
			break;
		case 213:
			retorno = "i213.jpg";
			break;
		case 214:
			retorno = "i214.jpg";
			break;
		case 215:
			retorno = "i215.jpg";
			break;
		case 216:
			retorno = "i216.jpg";
			break;
		case 217:
			retorno = "i217.jpg";
			break;
		case 218:
			retorno = "i218.jpg";
			break;
		case 219:
			retorno = "i219.jpg";
			break;
		case 220:
			retorno = "i220.jpg";
			break;
		case 221:
			retorno = "i221.jpg";
			break;
		case 222:
			retorno = "i222.jpg";
			break;
		case 223:
			retorno = "i223.jpg";
			break;
		case 224:
			retorno = "i224.jpg";
			break;
		case 225:
			retorno = "i225.jpg";
			break;
		case 226:
			retorno = "i226.jpg";
			break;
		case 227:
			retorno = "i227.jpg";
			break;
		case 228:
			retorno = "i228.jpg";
			break;
		case 229:
			retorno = "i229.jpg";
			break;
		case 230:
			retorno = "i230.jpg";
			break;
		case 231:
			retorno = "i231.jpg";
			break;
		case 232:
			retorno = "i232.jpg";
			break;
		case 233:
			retorno = "i233.jpg";
			break;
		case 234:
			retorno = "i234.jpg";
			break;
		case 235:
			retorno = "i235.jpg";
			break;
		case 236:
			retorno = "i236.jpg";
			break;
		case 237:
			retorno = "i237.jpg";
			break;
		case 238:
			retorno = "i238.jpg";
			break;
		case 239:
			retorno = "i239.jpg";
			break;
		case 240:
			retorno = "i240.jpg";
			break;
		case 241:
			retorno = "i241.jpg";
			break;
		case 242:
			retorno = "i242.jpg";
			break;
		case 243:
			retorno = "i243.jpg";
			break;
		case 244:
			retorno = "i244.jpg";
			break;
		case 245:
			retorno = "i245.jpg";
			break;
		case 246:
			retorno = "i246.jpg";
			break;
		case 247:
			retorno = "i247.jpg";
			break;
		case 248:
			retorno = "i248.jpg";
			break;
		case 249:
			retorno = "i249.jpg";
			break;
		case 250:
			retorno = "i250.jpg";
			break;
		case 251:
			retorno = "i251.jpg";
			break;
		case 252:
			retorno = "i252.jpg";
			break;
		case 253:
			retorno = "i253.jpg";
			break;
		case 254:
			retorno = "i254.jpg";
			break;
		case 255:
			retorno = "i255.jpg";
			break;
		case 256:
			retorno = "i256.jpg";
			break;
		case 257:
			retorno = "i257.jpg";
			break;
		case 258:
			retorno = "i258.jpg";
			break;
		case 259:
			retorno = "i259.jpg";
			break;
		case 260:
			retorno = "i260.jpg";
			break;
		case 261:
			retorno = "i261.jpg";
			break;
		case 262:
			retorno = "i262.jpg";
			break;
		case 263:
			retorno = "i263.jpg";
			break;
		case 264:
			retorno = "i264.jpg";
			break;
		case 265:
			retorno = "i265.jpg";
			break;
		case 266:
			retorno = "i266.jpg";
			break;
		case 267:
			retorno = "i267.jpg";
			break;
		case 268:
			retorno = "i268.jpg";
			break;
		case 269:
			retorno = "i269.jpg";
			break;
		case 270:
			retorno = "i270.jpg";
			break;
		case 271:
			retorno = "i271.jpg";
			break;
		case 272:
			retorno = "i272.jpg";
			break;
		case 273:
			retorno = "i273.jpg";
			break;
		case 274:
			retorno = "i274.jpg";
			break;
		case 275:
			retorno = "i275.jpg";
			break;
		case 276:
			retorno = "i276.jpg";
			break;
		case 277:
			retorno = "i277.jpg";
			break;
		case 278:
			retorno = "i278.jpg";
			break;
		case 279:
			retorno = "i279.jpg";
			break;
		case 280:
			retorno = "i280.jpg";
			break;
		case 281:
			retorno = "i281.jpg";
			break;
		case 282:
			retorno = "i282.jpg";
			break;
		case 283:
			retorno = "i283.jpg";
			break;
		case 284:
			retorno = "i284.jpg";
			break;
		case 285:
			retorno = "i285.jpg";
			break;
		case 286:
			retorno = "i286.jpg";
			break;
		case 287:
			retorno = "i287.jpg";
			break;
		case 288:
			retorno = "i288.jpg";
			break;
		case 289:
			retorno = "i289.jpg";
			break;
		case 290:
			retorno = "i290.jpg";
			break;
		case 291:
			retorno = "i291.jpg";
			break;
		case 292:
			retorno = "i292.jpg";
			break;
		case 293:
			retorno = "i293.jpg";
			break;
		case 294:
			retorno = "i294.jpg";
			break;
		case 295:
			retorno = "i295.jpg";
			break;
		case 296:
			retorno = "i296.jpg";
			break;
		case 297:
			retorno = "i297.jpg";
			break;
		case 298:
			retorno = "i298.jpg";
			break;
		case 299:
			retorno = "i299.jpg";
			break;
		case 300:
			retorno = "i300.jpg";
			break;
		case 301:
			retorno = "i301.jpg";
			break;
		case 302:
			retorno = "i302.jpg";
			break;
		case 303:
			retorno = "i303.jpg";
			break;
		case 304:
			retorno = "i304.jpg";
			break;
		case 305:
			retorno = "i305.jpg";
			break;
		case 306:
			retorno = "i306.jpg";
			break;
		case 307:
			retorno = "i307.jpg";
			break;
		case 308:
			retorno = "i308.jpg";
			break;
		case 309:
			retorno = "i309.jpg";
			break;
		case 310:
			retorno = "i310.jpg";
			break;
		case 311:
			retorno = "i311.jpg";
			break;
		case 312:
			retorno = "i312.jpg";
			break;
		case 313:
			retorno = "i313.jpg";
			break;
		case 314:
			retorno = "i314.jpg";
			break;
		case 315:
			retorno = "i315.jpg";
			break;
		case 316:
			retorno = "i316.jpg";
			break;
		case 317:
			retorno = "i317.jpg";
			break;
		case 318:
			retorno = "i318.jpg";
			break;
		case 319:
			retorno = "i319.jpg";
			break;
		case 320:
			retorno = "i320.jpg";
			break;
		case 321:
			retorno = "i321.jpg";
			break;
		case 322:
			retorno = "i322.jpg";
			break;
		case 323:
			retorno = "i323.jpg";
			break;
		case 324:
			retorno = "i324.jpg";
			break;
		case 325:
			retorno = "i325.jpg";
			break;
		case 326:
			retorno = "i326.jpg";
			break;
		case 327:
			retorno = "i327.jpg";
			break;
		case 328:
			retorno = "i328.jpg";
			break;
		case 329:
			retorno = "i329.jpg";
			break;
		case 330:
			retorno = "i330.jpg";
			break;
		case 331:
			retorno = "i331.jpg";
			break;
		case 332:
			retorno = "i332.jpg";
			break;
		case 333:
			retorno = "i333.jpg";
			break;
		case 334:
			retorno = "i334.jpg";
			break;
		case 335:
			retorno = "i335.jpg";
			break;
		case 336:
			retorno = "i336.jpg";
			break;
		case 337:
			retorno = "i337.jpg";
			break;
		case 338:
			retorno = "i338.jpg";
			break;
		case 339:
			retorno = "i339.jpg";
			break;
		case 340:
			retorno = "i340.jpg";
			break;
		case 341:
			retorno = "i341.jpg";
			break;
		case 342:
			retorno = "i342.jpg";
			break;
		case 343:
			retorno = "i343.jpg";
			break;
		case 344:
			retorno = "i344.jpg";
			break;
		case 345:
			retorno = "i345.jpg";
			break;
		case 346:
			retorno = "i346.jpg";
			break;
		case 347:
			retorno = "i347.jpg";
			break;
		case 348:
			retorno = "i348.jpg";
			break;
		case 349:
			retorno = "i349.jpg";
			break;
		case 350:
			retorno = "i350.jpg";
			break;
		case 351:
			retorno = "i351.jpg";
			break;
		case 352:
			retorno = "i352.jpg";
			break;
		case 353:
			retorno = "i353.jpg";
			break;
	}
return retorno;

}
/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  
  /* USER CODE END USB_HOST_Init_PreTreatment */
  
  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostFS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  MSC_Application();
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_1 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
