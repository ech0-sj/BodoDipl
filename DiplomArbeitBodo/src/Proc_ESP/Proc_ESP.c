/*
 * Proc_ESP.c
 *
 * Created: 21.07.2015 19:47:10
 *  Author: ech0
 */ 

#include "Proc_ESP.h"
#include "string.h"
#include "ESP8266/ESP8266.h"
#include "Http/HttpSimple.h"
#include "conf_defaults.h"
#include "Persistent/PersistentItems.h"

ProcEspStruct gEspProcStruct;



void ProcESP_Init( ProcessStruct* procStruct )
{
	procStruct->pid	= PID_ESP;
	procStruct->prio = Prio_Normal;
	procStruct->state = ProcState_Running;
	procStruct->specified = NULL; // ProcESP_getProcptr();
	
	ProcESP_InitProcstruct( &procStruct->specified );
}

void ProcESP_InitProcstruct( ProcEspStruct* espStruct )
{
	espStruct->lastState		= ESPState_start;
	espStruct->procState		= ESPState_start;
	espStruct->ESPVersion.asInt = 0; 
}

ProcEspStruct* ProcESP_getProcptr( )
{
	return &gEspProcStruct;
}

void ProcEsp_UpdateState( ePRocEspState newState )
{
	ProcEspStruct* espStruct = ProcESP_getProcptr();
	
	if( espStruct->procState == newState )
		return;
	
	// max abfangen
	if( newState >= ESPState_max )
		newState = ESPState_start;
	
	espStruct->lastState = espStruct->procState; 
	espStruct->procState = newState; 
	
	DebugOut( "-- ESPproc state changed to %i \r\n", newState ); 	
}





void ProcESP( ProcessStruct* procStruct )
{	
	ProcEspStruct* espStruct = ProcESP_getProcptr();
	uint8_t* msg[100]; 
	memset( msg, 0, 100 ); 
	uint32_t msglen = 0; 
		
	switch( espStruct->procState )
	{
		case ESPState_start:
		{
			// Zu beginn, das Teil erstmal neu starten 
			ESP_EncodeRestart( msg, &msglen ); 
			
			USARTWifi_Write(msg, msglen ); 								
			ProcEsp_UpdateState( ESPState_start_res );
		}//no break; 		
		case ESPState_start_res:
		{
			// Warten auf boot vorgang
		}break;
		
		
		case ESPState_getVersion:
		{			
			ESP_EncodeGetVersion(msg, &msglen ); 
			
			USARTWifi_Write(msg, msglen ); 							
			ProcEsp_UpdateState( ESPState_getVersion_res );
		}//no break;		
		case ESPState_getVersion_res:
		{
			// Im der Empfangsroutine
			// wird die Antwort verarbeitet
			// hier wird darauf geartet			
		}break;
		
		
		case ESPState_setMode:
		{	
			// softAP mode
			ESP_EncodeSetMode( msg, &msglen, 3 ); 		
			
			USARTWifi_Write(msg, msglen );
			ProcEsp_UpdateState( ESPState_setMode_res );
		}//no break; 		
		case ESPState_setMode_res:
		{			
		}break;
		
		
		case ESPState_setSoftAP:
		{
			uint8_t ssid[20] = {0}; 
			uint8_t pw[20] = {0}; 	
				
			LoadSSIDPass( pw );			
			LoadSSIDName( ssid ); 
			
			ESP_EncodeSetupAP( msg, &msglen, ssid, pw ); 
			
			USARTWifi_Write(msg, msglen );
			ProcEsp_UpdateState( ESPState_setSoftAP_res );
		}//no break;		
		case ESPState_setSoftAP_res:
		{			
		}break;
		
		
		case ESPState_softAPReboot:
		{
			// Zu beginn, das Teil erstmal neu starten
			ESP_EncodeRestart( msg, &msglen );
			
			USARTWifi_Write(msg, msglen );
			ProcEsp_UpdateState( ESPState_softAPReboot_res );
		}//no break; 		
		case ESPState_softAPReboot_res:
		{			
		}break;
		
				
		case ESPState_setMultiConnection:
		{
			// multiuser erlauben
			ESP_EncodeSetMultiCon( msg, &msglen, 1 );
			
			USARTWifi_Write(msg, msglen );
			ProcEsp_UpdateState( ESPState_setMultiConnection_res );
		}//no break;		
		case ESPState_setMultiConnection_res:
		{			
		}break;
		
		
		case ESPState_ServerMode:
		{
			ESP_EncodeServerMode( msg, &msglen, 1, 80 );
			
			USARTWifi_Write(msg, msglen );
			ProcEsp_UpdateState( ESPState_ServerMode_res );
		}//no break; 		
		case ESPState_ServerMode_res:
		{			
		}break; 
		
		
		case ESPState_Server:
		{
			// waiting for connection 
			ProcEsp_UpdateState( ESPState_Server_running );
		}//no break;
		case ESPState_Server_running:
		{
		}break;
		
		
		case ESPState_SendAsyncHead:
		{
			ESPAsyncMsgBuffer* asyncBuffer = ESP_GetAsyncBuffer();
			
			ESP_EncodeSendHead( msg, &msglen, asyncBuffer ); 
			USARTWifi_Write(msg, msglen );
					
			ProcEsp_UpdateState( ESPState_SendAsyncHead_res);	
		}//no break; 		
		case ESPState_SendAsyncHead_res:
		{			
		}break;
		
		case ESPState_SendAsyncPart:
		{
			ESPAsyncMsgBuffer* asyncBuffer = ESP_GetAsyncBuffer();
			
			ESP_SendNext( asyncBuffer );
			
			ProcEsp_UpdateState( ESPState_SendAsyncPart_res);
		}//no break;
		case ESPState_SendAsyncPart_res:
		{
		}break;

		case ESPState_SendAsyncClose:
		{
			ESPAsyncMsgBuffer* asyncBuffer = ESP_GetAsyncBuffer();
	
			ESP_EncodeClose( asyncBuffer->sendToID, msg, &msglen );
			USARTWifi_Write(msg, msglen );
	
			ProcEsp_UpdateState( ESPState_SendAsyncClose_res);
		}//no break;
		case ESPState_SendAsyncClose_res:
		{
		}break;

		
		default: 
		{
			ProcEsp_UpdateState( ESPState_start ); 
		}break; 
	}
	
	
	// debug ausgabe auf der ser. console 
	if( msglen > 0 )
	{	
		DebugOut( "    -> [%s]", msg );	
	}
}


eProcResult ProcESP_Receive( ProcEspStruct* espStruct, uint8_t* inBuffer, uint32_t inBuflen )
{
	eProcResult funcResult = ProcResult_WaitForMore;
	
	
	// Debug meldung ausgeben 	
	DebugOut( "    <- [%s]", inBuffer ); 
	
	// Nachricht auswerten
	switch( espStruct->procState )
	{
		case ESPState_start_res:	// Reboot nach start
		{
			// am ende sollte etwas mit ready stehen 			
			if( strstr(inBuffer, "ready") != NULL )
			{
				ProcEsp_UpdateState(ESPState_getVersion);
				funcResult = ProcResult_OK;			
			}
			// Etwas mit fehler -> nochmal versuchen 
			if( (strstr(inBuffer, "error") != NULL )
			||( strstr(inBuffer, "ERROR") != NULL ) )
			{
				ProcEsp_UpdateState(ESPState_start);						
				funcResult = ProcResult_OK;
			}	
			if( strstr(inBuffer, "busy") != NULL )
			{
				ProcEsp_UpdateState(ESPState_start);
				funcResult = ProcResult_OK;
			}		
		}break; 
		
		
		case ESPState_getVersion_res:
		{
			// Befehl vollständig?
			if( strstr( inBuffer, "OK") != NULL )
			{
				/*
				uint8_t temp[20]; 
				// nun die Versions nr finden
				// sie steht zwischen 2 \r\n 
				temp = strwrd( temp, inBuffer, 100, "\r\n" );
				temp = strwrd( temp, inBuffer, 100, "\r\n" );
				*/ 			
				ProcEsp_UpdateState(ESPState_setMode);
				funcResult = ProcResult_OK;	
			}			
		}break; 
		
				
		case ESPState_setMode_res:
		{
			// Nichts geändert? alles gut weiter machen
			// Falls erst auf diesen Mode gewechselt wird 
			// einmal den ESP neustarten
			if (strstr( inBuffer, "no change") != NULL )
			{
				// ProcEsp_UpdateState(ESPState_setMultiConnection);
				ProcEsp_UpdateState(ESPState_setSoftAP);
				
				funcResult = ProcResult_OK;	
			}
			else if(strstr( inBuffer, "OK") != NULL )
			{
				ProcEsp_UpdateState(ESPState_start);
				funcResult = ProcResult_OK;	
			}			
		}break;
			
				
		case ESPState_setSoftAP_res:
		{
			if( strstr( inBuffer, "OK") != NULL )
			{
				// alles Ok 
				ProcEsp_UpdateState(ESPState_softAPReboot);				
				funcResult = ProcResult_OK;				
			}
			else if( (strstr( inBuffer, "ERROR") != NULL )
			|| (strstr( inBuffer, "error") != NULL ) )
			{
				// Er gab einen Fehler 
				// nochmal von vorn
				ProcEsp_UpdateState(ESPState_start);
				funcResult = ProcResult_OK;	
			}
		}break; 
		
		
		case ESPState_softAPReboot_res:
		{
			// am ende sollte etwas mit ready stehen
			if( strstr(inBuffer, "ready") != NULL )
			{
				// ProcEsp_UpdateState(ESPState_ServerMode);
				ProcEsp_UpdateState(ESPState_setMultiConnection);
				
				funcResult = ProcResult_OK;
			}
			// Etwas mit fehler -> nochmal versuchen
			if( (strstr(inBuffer, "error") != NULL )
			||( strstr(inBuffer, "ERROR") != NULL ) )
			{
				ProcEsp_UpdateState(ESPState_start);
				funcResult = ProcResult_OK;
			}
			if( strstr(inBuffer, "busy") != NULL )
			{
				ProcEsp_UpdateState(ESPState_start);
				funcResult = ProcResult_OK;
			}
			
		}break; 
		
		
		case ESPState_setMultiConnection_res:
		{
			if( strstr( inBuffer, "OK") != NULL )
			{
				// alles Ok
				// ProcEsp_UpdateState(ESPState_setSoftAP);
				ProcEsp_UpdateState(ESPState_ServerMode);
				
				funcResult = ProcResult_OK;
			}
		}break;
		
		
		case ESPState_ServerMode_res:
		{
			if( strstr( inBuffer, "OK") != NULL )
			{
				// alles Ok 
				ProcEsp_UpdateState(ESPState_Server);
				funcResult = ProcResult_OK;				
			}
		}break;
				
			
		case ESPState_Server_running:
		{
			funcResult = ProcESP_DoReceive( inBuffer, inBuflen );
		}break;
					
				
		case ESPState_SendAsyncHead_res:
		{
			DebugOut( "    <- [%s]", inBuffer );
			if( strstr( inBuffer, "OK") != NULL )
			{
				// alles Ok
				ProcEsp_UpdateState(ESPState_SendAsyncPart);
				funcResult = ProcResult_OK;
			}
			/*
			if( strstr( inBuffer, "busy") != NULL )
			{
				// Das Teil ist abgeschmiert
				ProcEsp_UpdateState(ESPState_start);		
				funcResult = ProcResult_OK;
			}
			*/					
		}break;
				
				
		case ESPState_SendAsyncPart_res:
		{			
			ESPAsyncMsgBuffer* asyncBuffer = ESP_GetAsyncBuffer();
			
			DebugOut( "    <- [%s]", inBuffer );
			if( strstr( inBuffer, "OK") != NULL )
			{
				if( asyncBuffer->bytesThisRun < ESP_MAX_BLOCK_SIZE )
				{
					ProcEsp_UpdateState(ESPState_SendAsyncClose);					
				}
				else 				
				 {
					ProcEsp_UpdateState(ESPState_SendAsyncHead);
				 }
				funcResult = ProcResult_OK;
			}
		}break;

				
		case ESPState_SendAsyncClose_res:
		{
			ESPAsyncMsgBuffer* asyncBuffer = ESP_GetAsyncBuffer();
			
			DebugOut( "    <- [%s]", inBuffer ); 
			if( strstr( inBuffer, "OK") != NULL )
			{
				ESP_InitAsyncMsgBuffer( asyncBuffer);
				
				ProcEsp_UpdateState(ESPState_Server_running);
				funcResult = ProcResult_OK;
			}
		}break;
				
				
				
				
				
		case ESPState_start:
		case ESPState_getVersion:
		case ESPState_setMode:
		case ESPState_Server:
		case ESPState_setSoftAP:
		case ESPState_softAPReboot:	
		case ESPState_ServerMode:
		case ESPState_SetupIP:
		case ESPState_SendAsyncHead:
		case ESPState_SendAsyncPart:
		case ESPState_SendAsyncClose:
		{
			// nichts machen
		}break; 
		
		default: 
		{
			// ProcEsp_UpdateState(espStruct->procState += 1);
		}
	} 
	return funcResult; 
}



eProcResult ProcESP_DoReceive( uint8_t* msg, uint32_t len )
{
	eProcResult funcresult;
	uint8_t* ptr = NULL;
	uint8_t id = 0;
	ESPAsyncMsgBuffer* asyncBuffer = ESP_GetAsyncBuffer();
	
	
	// FORM:
	// link\r\n\r\n+IPD,id,len:GET ...
	//
	// 1. nach dem IPD suchen
	// nach dem komma -> ID ( id + 8 => socket |intern )
	// den : suchen, danach ist HTTP anfrage 
	
	
	// auf die gesamte Nachricht warten
	if( strstr( msg, "\r\nOK\r\n") == NULL )
		return ProcResult_WaitForMore;
	
	// Aus der Nachricht muss die senderID herausgeholt werden
	// aus ihr wird die SOCKETNr erzeugt ( beginnt bei 8 ) 		
	ptr = strstr( msg, "IPD"); 
	if( !ptr )
		return -1; 		
	ptr+=4; 	
	id = *ptr; 
	id -= 48;		// auf int runterrechnen 
	id += 8; 
	
	// Nun den Anfang der Nachricht finden
	// diese kommt nach dem ersten : 
	while( (*ptr!=':') && (*ptr!=0))
	{
		ptr++; 
	}
	ptr++; 
	
	// Nun die Nachricht in die Auswertung geben 
	HTTP_DoSimpleHTTP_ByMessage( id, ptr, strlen(ptr)); 
	
	// Das übertragen von Seiten ist teilw. kompliziert
	// zum einen ist der ESP recht lahm 
	// zum anderen dürfen die Blöcke nicht länger als 2KB sein
	// Es muss asynchron gesendet werden. 
	// Es muss aus dem Treiber heraus das asynchrone Sende angestoßen 
	// werden. der ESP Prozess soll die Verabeitung machen.
	//
	// Das Senden besteht aus 3 Teilen: 
	// 1. Mitteilen an wen, und wie lanf
	// 2. Daten 
	// 3. beenden der Verbindung 
	
	if( asyncBuffer->bufferLen > 0 )
	{		
		ProcEsp_UpdateState( ESPState_SendAsyncHead ); 		
	}
	return ProcResult_OK; 
}