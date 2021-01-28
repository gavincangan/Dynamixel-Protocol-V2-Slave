#include "NetworkController.h"
#include "Register.h"

void NetworkController::resetMachineState()
 {
	DEBUG_SERIAL.println("ResetMachineState called!");
	state=0;
	//instruction=0x00;
	messageIndex=0x00;
	crc=0;
	messageLength=0;
	index_tabParameter=0;
 }


unsigned short NetworkController::compute_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)  //http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/crc.htm
 {
    unsigned short i, j;
    unsigned short crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
        0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
        0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
        0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
        0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
        0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
        0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
        0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
        0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
        0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
        0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
        0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
        0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
        0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
        0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
        0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
        0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
    };

    for(j = 0; j < data_blk_size; j++)
    {
        i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }

    return crc_accum;
 }

void NetworkController::machineState(unsigned char data)
 {

	switch(state)
	 {
		case 0:
		 DEBUG_SERIAL.println("case 0");
		 if(data==HEADER_1)
		 {
			state=1;
			DEBUG_SERIAL.print("N1: ");
			DEBUG_SERIAL.println(data);
		 }
		 break;

		case 1:
		 DEBUG_SERIAL.println("case 1");
 		 if(data==HEADER_2)
		 {
			state=2;
			DEBUG_SERIAL.print("N2: ");
			DEBUG_SERIAL.println(data);
		 }
		 else
		 {
			resetMachineState();
		 }
		 break;


		case 2:
		 DEBUG_SERIAL.println("case 2");
		 if(data==HEADER_3)
		 {
			state=3;
			DEBUG_SERIAL.print("N3: ");
			DEBUG_SERIAL.println(data);
		 }
		 else
		 {
			resetMachineState();
		 }
		 break;


		case 3:
		 DEBUG_SERIAL.println("case 3");
		 if(data==RESERVED)
		 {
			state=4;
			DEBUG_SERIAL.print("N4: ");
			DEBUG_SERIAL.println(data);
		 }
		 else
		 {
			resetMachineState();
		 }
		 break;


		case 4:
		 DEBUG_SERIAL.println("case 4");
		 if(data==ID || data==BROADCAST_ID )
		 {
			state=5;
			DEBUG_SERIAL.print("N5: ");
			DEBUG_SERIAL.println(data);
		 }
		 else
		 {
			DEBUG_SERIAL.print("xN5: ");
			DEBUG_SERIAL.println(data);
			resetMachineState();
		 }
		 break;


		case 5:
		 DEBUG_SERIAL.println("case 5");                   //Lenght_Low
		 messageLength=data;
		 messageData[5]=data;
		 state=6;
		 break;

		case 6:
		 DEBUG_SERIAL.println("case 6");                   //Lenght_High
		 messageLength=messageLength+(data<<8);     //messageLenght|=data*256;
		 messageData[6]=data;
		 state=7;
		 DEBUG_SERIAL.print("MessageLength (N7): ");
		 DEBUG_SERIAL.println(messageLength);
		 DEBUG_SERIAL.print("number of Parameter: ");
		 DEBUG_SERIAL.println(messageLength-3);
		 break;

		case 7:
		 DEBUG_SERIAL.println("case 7");
		 instruction=data;
		 if (instruction==INST_PING)
		 {
			state=9;
		 }
		 else
		 {
			state=8;
		 }
		 break;

		case 8:
		 DEBUG_SERIAL.println("case 8");
		 messageData[messageIndex+8]=data;
		 messageIndex++;
		 if(messageIndex>=messageLength-3)
		 {
			DEBUG_SERIAL.print("Param: ");
			DEBUG_SERIAL.println(data);
			tabParameter[index_tabParameter]=data;
			index_tabParameter=0;
			state=9;
		 }
		 else
		 {
			DEBUG_SERIAL.print("Param: ");
			DEBUG_SERIAL.println(data);
			tabParameter[index_tabParameter]=data;
			index_tabParameter++;
		 }
		 break;

		case 9:
		 DEBUG_SERIAL.println("case 9");
		 crc=data;
		 state=10;
		 break;

		case 10:
		 DEBUG_SERIAL.println("case 10");
		 crc=crc+(data<<8);
		 messageData[0]=HEADER_1;
		 messageData[1]=HEADER_2; 
		 messageData[2]=HEADER_3;
		 messageData[3]=RESERVED;
		 messageData[4]=ID;
		 messageData[7]=instruction;
		 DEBUG_SERIAL.println("parametre tab: ");
		 DEBUG_SERIAL.println(tabParameter[0]);
		 DEBUG_SERIAL.println(tabParameter[1]);
		 DEBUG_SERIAL.println(tabParameter[2]);
		 DEBUG_SERIAL.println(tabParameter[3]);
		 DEBUG_SERIAL.print("crc received: ");
		 DEBUG_SERIAL.println(crc);
		 crc_compute = compute_crc(0, messageData, messageLength+5);
		 DEBUG_SERIAL.print("crc compute: ");
		 DEBUG_SERIAL.println(crc_compute);
		 if(crc_compute==crc)
		 {
			 DEBUG_SERIAL.println("CRC -> OK");
			 DEBUG_SERIAL.print("instruction");
			 DEBUG_SERIAL.println(instruction); 
			 returnStatus();
		 }
		 else
		 {
			 DEBUG_SERIAL.println("CRC is not good: Packet has been damaged during communication");
		 }
		 resetMachineState();
		 break;

		default:
		 resetMachineState();
		 break;
	 }
 }	 
void NetworkController::returnStatus()
 {
	switch(instruction)
	{
		case INST_PING:
		 
		 //Header and Reserved
		 returnPacket[0]=HEADER_1;					//Default: 0xFF
		 returnPacket[1]=HEADER_2;					//Default=0xFF
		 returnPacket[2]=HEADER_3;					//Default=0xFD
		 returnPacket[3]=RESERVED;					//Default=0x00
		 //ID
		 returnPacket[4]=ID;			
		 //Lenght_High
		 returnPacket[5]=0x07;
		 //Lenght_Low
		 returnPacket[6]=0x00;
		 //Instruction
		 returnPacket[7]=0x55;		//Default=0x55
		 //Error
		 returnPacket[8]=0x00;   					//error : if no error, bit 7=0     cf -> http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm
		 //Param1 & Param2: Model Number
		 //Parameter 1
		 returnPacket[9]=MODEL_NUMBER & 0xFF;
		 //Parameter 2
		 returnPacket[10]=MODEL_NUMBER >> 8;
		 //Param3: Firmware Version
		 returnPacket[11]=FIRMWARE;
		 //CRC
		 crc_compute = compute_crc(0, returnPacket, 12);
		 returnPacket[12]=crc_compute & 0xFF;
		 returnPacket[13]=crc_compute >> 8;
		 //
		 DEBUG_SERIAL.println("retour");		
		 DEBUG_SERIAL.println(returnPacket[0]);
		 DEBUG_SERIAL.println(returnPacket[1]);
		 DEBUG_SERIAL.println(returnPacket[2]);
		 DEBUG_SERIAL.println(returnPacket[3]);
		 DEBUG_SERIAL.println(returnPacket[4]);
		 DEBUG_SERIAL.println(returnPacket[5]);
		 DEBUG_SERIAL.println(returnPacket[6]);
		 DEBUG_SERIAL.println(returnPacket[7]);
		 DEBUG_SERIAL.println(returnPacket[8]);
		 DEBUG_SERIAL.println(returnPacket[9]);
		 DEBUG_SERIAL.println(returnPacket[10]);
		 DEBUG_SERIAL.println(returnPacket[11]);
		 DEBUG_SERIAL.print("crc:");	
		 DEBUG_SERIAL.println(crc_compute);	
		 DEBUG_SERIAL.println(returnPacket[12]);
		 DEBUG_SERIAL.println(returnPacket[13]);
		 
		 tab_lenght=14;
		 //DEBUG_SERIAL.write(returnPacket, tab_lenght);
		 
		 digitalWrite(2, LOW);
		 DXL_SERIAL.write(returnPacket, tab_lenght);
		 resetMachineState();
		 DXL_SERIAL.flush();
		 digitalWrite(2, HIGH);
		 break;
	
	 
		 case INST_READ:
		 //Header and Reserved
		 returnPacket[0]=HEADER_1;					//Default: 0xFF
		 returnPacket[1]=HEADER_2;					//Default=0xFF
		 returnPacket[2]=HEADER_3;					//Default=0xFD
		 returnPacket[3]=RESERVED;					//Default=0x00
		 //ID
		 returnPacket[4]=ID;			
		
		 //Instruction
		 returnPacket[7]=INST_STATUS_RETURN;		//Default=0x55
		 //Error
		 returnPacket[8]=0x00;   					//error : if no error, bit 7=0     cf -> http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm
		

		//Parameter 
		 addressParameter=0;
		 addressParameter=tabParameter[0];
		 addressParameter=addressParameter+ (tabParameter[1] << 8);
		 DEBUG_SERIAL.println("addressParameter: ");
		 DEBUG_SERIAL.println(addressParameter);
		 
		 numberParameter=0;
		 numberParameter=tabParameter[2];
		 numberParameter=numberParameter + (tabParameter[3] << 8);
		 DEBUG_SERIAL.println("numberParameter: ");
		 DEBUG_SERIAL.println(numberParameter);
		 
		 
		 //Lenght_High
		 returnPacket[5]=4+numberParameter;
		 //Lenght_Low
		 returnPacket[6]=0x00;
		 
		 //    
		 tabRegister[13]=0x44;    // A faire dans un autre .H 
		 tabRegister[14]=0xF1;
		 //
		 
		 index_tabParameter=0;
		 for(int j=0; j<numberParameter; j++) 
		 {
		 returnPacket[9+index_tabParameter]= tabRegister[addressParameter+index_tabParameter]; 
		 index_tabParameter++;
		 }
		 index_tabParameter--;
		//CRC
		 crc_compute = compute_crc(0, returnPacket, 10+index_tabParameter);    
		 returnPacket[10+index_tabParameter]=crc_compute & 0xFF;
		 returnPacket[11+index_tabParameter]=crc_compute >> 8;
		 
		 //
		 DEBUG_SERIAL.println(" ");
		 DEBUG_SERIAL.println("Retour");		
		 DEBUG_SERIAL.println(returnPacket[0]);	//Header
		 DEBUG_SERIAL.println(returnPacket[1]);
		 DEBUG_SERIAL.println(returnPacket[2]);
		 DEBUG_SERIAL.println(returnPacket[3]);	//Reserved
		 DEBUG_SERIAL.println(returnPacket[4]);	//Id
		 DEBUG_SERIAL.println(returnPacket[5]);	//Lenght
		 DEBUG_SERIAL.println(returnPacket[6]);
		 DEBUG_SERIAL.println(returnPacket[7]);	//Instruct
		 DEBUG_SERIAL.println(returnPacket[8]);	//Error
		 DEBUG_SERIAL.println(returnPacket[9]);	//Param
		 DEBUG_SERIAL.println(returnPacket[10]);	
		
		 
		 DEBUG_SERIAL.print("crc:");	
		 DEBUG_SERIAL.println(crc_compute);	
		 DEBUG_SERIAL.println(returnPacket[11]);	//CRC
		 DEBUG_SERIAL.println(returnPacket[12]);
		 tab_lenght=12+index_tabParameter;
		 DEBUG_SERIAL.write(returnPacket, tab_lenght);
		 
		 digitalWrite(2, LOW);
		 DXL_SERIAL.write(returnPacket, tab_lenght);
		 resetMachineState();
		 DXL_SERIAL.flush();
		 digitalWrite(2, HIGH);
		 break;
	
		case INST_WRITE:
		 
		 //Writing
		 
		 addressParameter=0;
		 addressParameter=tabParameter[0];
		 addressParameter=addressParameter+ (tabParameter[1] << 8);
		 DEBUG_SERIAL.println("addressParameter: ");
		 DEBUG_SERIAL.println(addressParameter);
		 
		 numberParameter = (messageLength - 5); // 5 = Instruction (1) + Address (2) + CRC (2)   
		 
		 index_tabParameter=0;
		 for(int k=0; k<numberParameter; k++) 
		 {
		 tabRegister[addressParameter+index_tabParameter]=tabParameter[2+index_tabParameter];    // "2" Because Byte 1 & 2 are address Bytes 
		 index_tabParameter++;
		 }
		 
		 
		 //Header and Reserved
		 returnPacket[0]=HEADER_1;					//Default: 0xFF
		 returnPacket[1]=HEADER_2;					//Default=0xFF
		 returnPacket[2]=HEADER_3;					//Default=0xFD
		 returnPacket[3]=RESERVED;					//Default=0x00
		 //ID
		 returnPacket[4]=ID;			
		 //Lenght_High
		 returnPacket[5]=0x04;			//Constant Value
		 //Lenght_Low
		 returnPacket[6]=0x00;
		 //Instruction
		 returnPacket[7]=INST_STATUS_RETURN;		//Default=0x55
		 //Error
		 returnPacket[8]=0x00;   					//error : if no error, bit 7=0     cf -> http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm
		 
		 
		 //CRC
		 crc_compute = compute_crc(0, returnPacket, 9);    // 9= Header (3) + reserved (1) + id (1) + Lenght (2) + Instruction (1) + error (1)
		 returnPacket[9]=crc_compute & 0xFF;
		 returnPacket[10]=crc_compute >> 8;
		 
		 //
		 DEBUG_SERIAL.println(" ");
		 DEBUG_SERIAL.println("Retour");		
		 DEBUG_SERIAL.println(returnPacket[0]);	//Header
		 DEBUG_SERIAL.println(returnPacket[1]);
		 DEBUG_SERIAL.println(returnPacket[2]);
		 DEBUG_SERIAL.println(returnPacket[3]);	//Reserved
		 DEBUG_SERIAL.println(returnPacket[4]);	//Id
		 DEBUG_SERIAL.println(returnPacket[5]);	//Lenght
		 DEBUG_SERIAL.println(returnPacket[6]);
		 DEBUG_SERIAL.println(returnPacket[7]);	//Instruct
		 DEBUG_SERIAL.println(returnPacket[8]);	//Error
	
		 DEBUG_SERIAL.print("crc:");	
		 DEBUG_SERIAL.println(crc_compute);	
		 DEBUG_SERIAL.println(returnPacket[9]);	//CRC
		 DEBUG_SERIAL.println(returnPacket[10]);
		 tab_lenght=11;     		// 11= Header (3) + reserved (1) + id (1) + Lenght (2) + Instruction (1) + error (1) + CRC (2)
		 DEBUG_SERIAL.write(returnPacket, tab_lenght);
		 
		 digitalWrite(2, LOW);
		 DXL_SERIAL.write(returnPacket, tab_lenght);
		 resetMachineState();
		 DXL_SERIAL.flush();
		 digitalWrite(2, HIGH);
		 break;
		 
		 case INST_FACTORY_RESET:
		 
		 //Option: Param=0xFF "Reset all values" | Param=0x01  " Reset all values except ID " | Param=0x02  "Reset all values except ID and Baudrate"
		 
		 switch(tabParameter[0])
		  {
			 case 0xFF:
			//  #define MODEL_NUMBER 350
			//  ID=1;
			//  #define BAUDRATE 1000000
			 
			 break;
			
			 case 0x01:
			//  #define MODEL_NUMBER 350
			//  #define BAUDRATE 1000000
			 break;
			 
			 case 0x02:
			//  #define MODEL_NUMBER 350
			 break;
			 
			 default:
			 break;
		  }
		 
		 
		 //Header and Reserved
		 returnPacket[0]=HEADER_1;					//Default: 0xFF
		 returnPacket[1]=HEADER_2;					//Default=0xFF
		 returnPacket[2]=HEADER_3;					//Default=0xFD
		 returnPacket[3]=RESERVED;					//Default=0x00
		 //ID
		 returnPacket[4]=ID;			
		 //Lenght_High
		 returnPacket[5]=0x04;			//Constant Value
		 //Lenght_Low
		 returnPacket[6]=0x00;
		 //Instruction
		 returnPacket[7]=INST_STATUS_RETURN;		//Default=0x55
		 //Error
		 returnPacket[8]=0x00;   					//error : if no error, bit 7=0     cf -> http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm
		 
		 
		 //CRC
		 crc_compute = compute_crc(0, returnPacket, 9);    // 9= Header (3) + reserved (1) + id (1) + Lenght (2) + Instruction (1) + error (1)
		 returnPacket[9]=crc_compute & 0xFF;
		 returnPacket[10]=crc_compute >> 8;
		 //
		 DEBUG_SERIAL.println(" ");
		 DEBUG_SERIAL.println("Retour");		
		 DEBUG_SERIAL.println(returnPacket[0]);	//Header
		 DEBUG_SERIAL.println(returnPacket[1]);
		 DEBUG_SERIAL.println(returnPacket[2]);
		 DEBUG_SERIAL.println(returnPacket[3]);	//Reserved
		 DEBUG_SERIAL.println(returnPacket[4]);	//Id
		 DEBUG_SERIAL.println(returnPacket[5]);	//Lenght
		 DEBUG_SERIAL.println(returnPacket[6]);
		 DEBUG_SERIAL.println(returnPacket[7]);	//Instruct
		 DEBUG_SERIAL.println(returnPacket[8]);	//Error
	
		 DEBUG_SERIAL.print("crc:");	
		 DEBUG_SERIAL.println(crc_compute);	
		 DEBUG_SERIAL.println(returnPacket[9]);	//CRC
		 DEBUG_SERIAL.println(returnPacket[10]);
		 tab_lenght=11;     // 11= Header (3) + reserved (1) + id (1) + Lenght (2) + Instruction (1) + error (1) + CRC (2)
		 DEBUG_SERIAL.write(returnPacket, tab_lenght);
		 
		 digitalWrite(2, LOW);
		 DXL_SERIAL.write(returnPacket, tab_lenght);
		 resetMachineState();
		 DXL_SERIAL.flush();
		 digitalWrite(2, HIGH);
		 break;
	}
 }