// #############################################################################
// #### Copyright ##############################################################
// #############################################################################

/*
 * Copyright 2024 BaSSeM
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

// #############################################################################
// #### Description ############################################################
// #############################################################################

// #############################################################################
// #### Control Include(s) #####################################################
// #############################################################################

#include "Platform.h"

// #############################################################################
// #### Control Macro(s) #######################################################
// #############################################################################

#ifndef DEBUG
    #define DEBUG
#endif

#ifdef DEBUG
    #undef DEBUG
#endif

// #############################################################################
// #### File Guard #############################################################
// #############################################################################

// #############################################################################
// #### Include(s) #############################################################
// #############################################################################

#include "../../GSM_Internal.h"
#include "GSM_BC92.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// #############################################################################
// #### Private Macro(s) #######################################################
// #############################################################################

#define GSM_BC92_BUFFER_SIZE_TRANSMIT ( 1024 )
#define GSM_BC92_BUFFER_SIZE_RECEIVE  ( 256 )

// #############################################################################
// #### Private Type(s) ########################################################
// #############################################################################

typedef enum GSM_BC92_CommandType
{
    GSM_BC92_CommandType_None = 0,

    // Custom Command
    GSM_BC92_CommandType_Custom,

    // General Command(s)
    GSM_BC92_CommandType_Ping,
    GSM_BC92_CommandType_Display_Product_Identification_Information,
    GSM_BC92_CommandType_Set_Command_Echo_Mode,
    GSM_BC92_CommandType_Store_Current_Parameters_To_User_Defined_Profile,
    GSM_BC92_CommandType_Set_All_Parameters_To_Manufacturer_Defaults,
    GSM_BC92_CommandType_Request_Manufacturer_Identification,
    GSM_BC92_CommandType_Request_Model_Identification,
    GSM_BC92_CommandType_Request_Manufacturer_Revision,
    GSM_BC92_CommandType_Request_Product_Serial_Number,
    GSM_BC92_CommandType_Report_Mobile_Termination_Error,
    GSM_BC92_CommandType_Extended_Error_Report,

    // Serial Interface Related Command(s)
    GSM_BC92_CommandType_Set_TE_TA_Fixed_Local_Rate,
    GSM_BC92_CommandType_Set_TE_TA_Control_Character_Framing,
    GSM_BC92_CommandType_Set_TE_TA_Local_Data_Flow_Control,

    // (U)SIM Card Related Command(s)
    GSM_BC92_CommandType_Enter_Pin,
    GSM_BC92_CommandType_Request_International_Mobile_Subscriber_Identity,
    GSM_BC92_CommandType_USIM_Card_Identification,

    // Network Service Command(s)
    GSM_BC92_CommandType_Signal_Quality_Report,
    GSM_BC92_CommandType_Get_Cell_Information,
    GSM_BC92_CommandType_Extended_Signal_Quality,
    GSM_BC92_CommandType_GSM_Network_Registration_Status,
    GSM_BC92_CommandType_GPRS_Network_Registration_Status,
    GSM_BC92_CommandType_EPS_Network_Registration_Status,
    GSM_BC92_CommandType_PS_Attach_Or_Detach,
    GSM_BC92_CommandType_Set_Default_PSD_Connection_Settings,
    GSM_BC92_CommandType_Define_A_PDP_Context,
    GSM_BC92_CommandType_PDP_Context_Activation_Or_Deactivation,
    GSM_BC92_CommandType_Show_PDP_Addresses,
    GSM_BC92_CommandType_Define_A_Tcp_Ip_Context,
    GSM_BC92_CommandType_Signaling_Connection_Status,
    GSM_BC92_CommandType_Operator_Selection,
    GSM_BC92_CommandType_Power_Saving_Mode_Setting,
    GSM_BC92_CommandType_Edrx_Setting,
    GSM_BC92_CommandType_Edrx_Read_Dynamic_Parameters,
    GSM_BC92_CommandType_Get_And_Set_Mobile_Operation_Band,
    GSM_BC92_CommandType_Lock_Nb_Iot_Frequency,
    GSM_BC92_CommandType_Engineering_Mode,
    GSM_BC92_CommandType_Enable_Disable_Nb_Iot_Related_Event_Report,
    GSM_BC92_CommandType_Clear_Stored_Nb_Iot_GSM_Earfcn_List,
    GSM_BC92_CommandType_Extended_Configuration_Setting,
    GSM_BC92_CommandType_Clot_Optimization_Configuration,
    GSM_BC92_CommandType_Time_Zone_Reporting,
    GSM_BC92_CommandType_Query_Apn_Rate_Control,
    GSM_BC92_CommandType_Set_Scrambling_Algorithm,
    GSM_BC92_CommandType_Configure_Networking_Mode,

    // Hardware Related Command(s)
    GSM_BC92_CommandType_Set_UE_Functionality,
    GSM_BC92_CommandType_Configure_Sleep_Mode,
    GSM_BC92_CommandType_Reset_The_Module,
    GSM_BC92_CommandType_Power_Off_The_Module,
    GSM_BC92_CommandType_Return_Current_Date_And_Time,
    GSM_BC92_CommandType_Query_Power_Supply_Voltage,
    GSM_BC92_CommandType_Enable_Disable_Deep_Sleep_Wakeup_Indication,
    GSM_BC92_CommandType_Configure_Network_Status_Indication_Light,
    GSM_BC92_CommandType_Read_Adc,

    // TCP/IP Related Command(s)
    GSM_BC92_CommandType_Open_A_Socket,
    GSM_BC92_CommandType_Close_A_Socket,
    GSM_BC92_CommandType_Querying_Socket_Status,
    GSM_BC92_CommandType_Send_Text_String_Data,
    GSM_BC92_CommandType_Retrieve_The_Received_Tcp_Ip_Data,
    GSM_BC92_CommandType_Send_Hex_String_Data,
    GSM_BC92_CommandType_Switch_Data_Access_Modes,
    GSM_BC92_CommandType_Ping_A_Remote_Server,
    GSM_BC92_CommandType_Synchronize_Local_Time_With_Ntp_Server,
    GSM_BC92_CommandType_Get_Ip_Address_By_Domain_Name,
    GSM_BC92_CommandType_Configure_Optional_Parameters,
    GSM_BC92_CommandType_Query_The_Last_Error_Code,
    GSM_BC92_CommandType_Configure_Tcp_Socket_Keep_Alive,
    GSM_BC92_CommandType_Configure_Dns_Server_Function,

    // DFOTA Related Command(s)
    GSM_BC92_CommandType_Dfota_Via_Http_Server,

    // MQTT Related Command(s)
    GSM_BC92_CommandType_Configure_Parameters_Of_Mqtt,
    GSM_BC92_CommandType_Open_A_Network_For_Mqtt_Client,
    GSM_BC92_CommandType_Close_A_Network_For_Mqtt_Client,
    GSM_BC92_CommandType_Connect_A_Client_To_Mqtt_Server,
    GSM_BC92_CommandType_Disconnect_A_Client_From_Mqtt_Server,
    GSM_BC92_CommandType_Subscribe_To_Topics,
    GSM_BC92_CommandType_Unsubscribe_From_Topics,
    GSM_BC92_CommandType_Publish_Messages,

    // FS Related Command(s)
    GSM_BC92_CommandType_List_Files,
    GSM_BC92_CommandType_Get_Data_Storage_Size,
    GSM_BC92_CommandType_Upload_File_To_Storage,
    GSM_BC92_CommandType_Download_File_From_Storage,
    GSM_BC92_CommandType_Delete_File_From_Storage,
    GSM_BC92_CommandType_Move_The_File_From_Ram_To_Ufs,
    GSM_BC92_CommandType_Open_The_File,
    GSM_BC92_CommandType_Read_The_Content_From_File,
    GSM_BC92_CommandType_Write_The_Content_To_File,
    GSM_BC92_CommandType_The_Current_Offset_Of_File,
    GSM_BC92_CommandType_Close_The_File,
    GSM_BC92_CommandType_Get_Offset_Of_The_File_Pointer,
    GSM_BC92_CommandType_Force_To_Write_Data_Remaining_In_The_File_Buffer,
    GSM_BC92_CommandType_Truncate_The_Specified_File_From_The_File_Pointer,
    GSM_BC92_CommandType_Rename_The_File,

    // SMS Related Command(s)
    GSM_BC92_CommandType_Select_SMS_Message_Format,
    GSM_BC92_CommandType_Delete_SMS_Message,
    GSM_BC92_CommandType_List_SMS_Messages_From_Preferred_Store,
    GSM_BC92_CommandType_Read_SMS_Message,
    GSM_BC92_CommandType_Send_SMS_Message,
    GSM_BC92_CommandType_Write_SMS_Message_To_Memory,
    GSM_BC92_CommandType_Send_SMS_Message_From_Storage,
    GSM_BC92_CommandType_Send_SMS_Command,
    GSM_BC92_CommandType_New_SMS_Message_Indications,
} GSM_BC92_CommandType_t;

typedef uint8_t GSM_BC92_CommandValue_t[ 0 ];

typedef struct GSM_BC92_Command
{
    GSM_BC92_CommandType_t Type;
    GSM_BC92_CommandValue_t Value;
} GSM_BC92_Command_t;

typedef enum GSM_BC92_ResponseType
{
    GSM_BC92_ResponseType_None = 0,

    // Any Response
    GSM_BC92_ResponseType_Unknown,

    // Powering On Response(s)
    GSM_BC92_ResponseType_Ready,

    // General Response(s)
    GSM_BC92_ResponseType_Ok,
    GSM_BC92_ResponseType_Timeout,
    GSM_BC92_ResponseType_Error,
    GSM_BC92_ResponseType_Echo,
    GSM_BC92_ResponseType_Report_Error,

    // Device Related Response(s)
    GSM_BC92_ResponseType_Device_Functionality,
    GSM_BC92_ResponseType_Device_Provider,
    GSM_BC92_ResponseType_Device_Type,
    GSM_BC92_ResponseType_Device_Manufacturer,
    GSM_BC92_ResponseType_Device_Software,
    GSM_BC92_ResponseType_Device_Serial,

    // Serial Related Response(s)
    GSM_BC92_ResponseType_Serial_Rate,
    GSM_BC92_ResponseType_Serial_Framing,
    GSM_BC92_ResponseType_Serial_Control,

    // SIM Related Response(s)
    GSM_BC92_ResponseType_SIM_Status,
    GSM_BC92_ResponseType_SIM_International_Mobile_Subscriber_Identity,
    GSM_BC92_ResponseType_SIM_Integrated_Circuit_Card_Identification,

    // Network Service Response(s)
    GSM_BC92_ResponseType_Signal_Quality_Report,
    GSM_BC92_ResponseType_GSM_Network_Registration_Status,
    GSM_BC92_ResponseType_GPRS_Network_Registration_Status,
    GSM_BC92_ResponseType_PS_Attach_or_Detach,
    GSM_BC92_ResponseType_Set_Default_PSD_Connection_Settings,
    GSM_BC92_ResponseType_Define_A_PDP_Context,
    GSM_BC92_ResponseType_PDP_Context_Activation_Or_Deactivation,
    GSM_BC92_ResponseType_Show_PDP_Addresses,

    // Socket Response(s)
    GSM_BC92_ResponseType_Open_Socket,
    GSM_BC92_ResponseType_Send_Text_String_Data,
    GSM_BC92_ResponseType_Retrieve_The_Received_Tcp_Ip_Data,
    GSM_BC92_ResponseType_Socket_UnsolicitedResultCode,

    // SMS Related Response(s)
    GSM_BC92_ResponseType_SMS_Received,
    GSM_BC92_ResponseType_SMS_Start,
    GSM_BC92_ResponseType_SMS_Sent,

    // Hardware Related Response(s)
    GSM_BC92_ResponseType_Normal_Power_Down,
    GSM_BC92_ResponseType_Current_Date_And_Time,
    GSM_BC92_ResponseType_Power_Supply_Voltage,
} GSM_BC92_ResponseType_t;

typedef uint8_t GSM_BC92_ResponseValue_t[ 0 ];

typedef GSM_BC92_Status_t ( *GSM_BC92_ResponseHandler_t )( GSM_BC92_Instance_t * Instance );

typedef struct GSM_BC92_Response
{
    GSM_BC92_ResponseType_t Type;
    GSM_BC92_ResponseValue_t Value;
} GSM_BC92_Response_t;

typedef enum GSM_BC92_OperationType
{
    GSM_BC92_OperationType_None = 0,

    // Process Pre-Operation
    GSM_BC92_OperationType_Pending,

    // Basic Operation(s)
    GSM_BC92_OperationType_Power_On,
    GSM_BC92_OperationType_Power_Off,

    // General Operation(s)
    GSM_BC92_OperationType_Ping,
    GSM_BC92_OperationType_Display_Product_Identification_Information,
    GSM_BC92_OperationType_Set_Command_Echo_Mode,
    GSM_BC92_OperationType_Store_Current_Parameters_To_User_Defined_Profile,
    GSM_BC92_OperationType_Set_All_Parameters_To_Manufacturer_Defaults,
    GSM_BC92_OperationType_Request_Manufacturer_Identification,
    GSM_BC92_OperationType_Request_Model_Identification,
    GSM_BC92_OperationType_Request_Manufacturer_Revision,
    GSM_BC92_OperationType_Request_Product_Serial_Number,
    GSM_BC92_OperationType_Report_Mobile_Termination_Error,
    GSM_BC92_OperationType_Extended_Error_Report,

    // Serial Interface Related Operation(s)
    GSM_BC92_OperationType_Set_TE_TA_Fixed_Local_Rate,
    GSM_BC92_OperationType_Set_TE_TA_Control_Character_Framing,
    GSM_BC92_OperationType_Set_TE_TA_Local_Data_Flow_Control,

    // SIM Related Operation(s)
    GSM_BC92_OperationType_Enter_Pin,
    GSM_BC92_OperationType_Request_International_Mobile_Subscriber_Identity,
    GSM_BC92_OperationType_USIM_Card_Identification,

    // Network Service Operation(s)
    GSM_BC92_OperationType_Signal_Quality_Report,
    GSM_BC92_OperationType_GSM_Network_Registration_Status,
    GSM_BC92_OperationType_GPRS_Network_Registration_Status,
    GSM_BC92_OperationType_PS_Attach_Or_Detach,
    GSM_BC92_OperationType_Set_Default_PSD_Connection_Settings,
    GSM_BC92_OperationType_Define_A_PDP_Context,
    GSM_BC92_OperationType_PDP_Context_Activation_Or_Deactivation,
    GSM_BC92_OperationType_Show_PDP_Addresses,

    // Socket Operation(s)
    GSM_BC92_OperationType_Open_A_Socket,
    GSM_BC92_OperationType_Send_Text_String_Data,
    GSM_BC92_OperationType_Retrieve_The_Received_Tcp_Ip_Data,

    // Hardware Related Operation(s)
    GSM_BC92_OperationType_Set_UE_Functionality,
    GSM_BC92_OperationType_Reset_The_Module,
    GSM_BC92_OperationType_Power_Off_The_Module,
    GSM_BC92_OperationType_Return_Current_Date_And_Time,
    GSM_BC92_OperationType_Query_Power_Supply_Voltage,

    // SMS Related Operation(s)
    GSM_BC92_OperationType_Select_SMS_Message_Format,
    GSM_BC92_OperationType_Send_SMS_Message,
} GSM_BC92_OperationType_t;

typedef GSM_BC92_Status_t ( *GSM_BC92_OperationHandler_t )( GSM_BC92_Instance_t * Instance );

typedef struct GSM_BC92_Operation
{
    GSM_BC92_OperationType_t Type;
    GSM_BC92_OperationHandler_t Handler;
    GSM_BC92_Status_t Status;
    TIM_Timestamp_t Timestamp;
    // TODO Add Context
} GSM_BC92_Operation_t;

typedef enum GSM_BC92_ProcessType
{
    GSM_BC92_ProcessType_None = 0,
    GSM_BC92_ProcessType_Command,
    GSM_BC92_ProcessType_Initialize,
    GSM_BC92_ProcessType_Socket_Open,
} GSM_BC92_ProcessType_t;

typedef GSM_BC92_Status_t ( *GSM_BC92_ProcessHandler_t )( GSM_BC92_Instance_t * Instance );

typedef struct GSM_BC92_Process
{
    GSM_BC92_ProcessType_t Type;
    GSM_BC92_ProcessHandler_t Handler;
    GSM_BC92_Status_t Status;
    // TODO Add Context
} GSM_BC92_Process_t;

typedef struct GSM_BC92_BufferTransmit
{
    uint32_t Length;
    // Note: The following MUST be kept in order
    GSM_BC92_Command_t Command;
    uint8_t Content[ GSM_BC92_BUFFER_SIZE_TRANSMIT ];
} GSM_BC92_BufferTransmit_t;

typedef struct GSM_BC92_BufferReceive
{
    uint32_t Length;
    // Note: The following MUST be kept in order
    GSM_BC92_Response_t Response;
    uint8_t Content[ GSM_BC92_BUFFER_SIZE_RECEIVE ];
} GSM_BC92_BufferReceive_t;

typedef struct GSM_BC92_Instance_Context
{
    GSM_BC92_BufferTransmit_t Transmit;
    GSM_BC92_BufferReceive_t Receive;
    GSM_BC92_Operation_t Operation;
    GSM_BC92_Process_t Process;

    // FIXME User Arguments
    struct
    {
        GSM_Address_t Address;
        GSM_Port_t Port;
    } Argument;

} GSM_BC92_Instance_Context_t;

typedef struct GSM_BC92_Context
{
    TIM_Timestamp_t Timestamp;
    GSM_BC92_Instance_Context_t Context[ GSM_BC92_Count ];
} GSM_BC92_Context_t;

// #############################################################################
// #### Private Method(s) Prototype ############################################
// #############################################################################

static GSM_BC92_Status_t GSM_BC92_Process_Socket_Open_Handler( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Process_Initialize_Handler( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Process_Command_Handler( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Process_Set( GSM_BC92_Instance_t * Instance, GSM_BC92_ProcessType_t GSM_BC92_ProcessType );

static GSM_BC92_Status_t GSM_BC92_Power_Off_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Power_Off_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Power_On_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Power_On_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Ping_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Ping_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Display_Product_Identification_Information_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Display_Product_Identification_Information_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Set_Command_Echo_Mode_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Set_Command_Echo_Mode_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_Command_Echo_Mode_t GSM_BC92_Argument_Set_Command_Echo_Mode );

static GSM_BC92_Status_t GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile_t GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile );

static GSM_BC92_Status_t GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults_t GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults );

static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Identification_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Identification_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Request_Model_Identification_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Request_Model_Identification_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Revision_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Revision_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Request_Product_Serial_Number_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Request_Product_Serial_Number_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Request_Product_Serial_Number_t GSM_BC92_Argument_Request_Product_Serial_Number );

static GSM_BC92_Status_t GSM_BC92_Report_Mobile_Termination_Error_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Report_Mobile_Termination_Error_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error_t GSM_BC92_Argument_Report_Mobile_Termination_Error );

static GSM_BC92_Status_t GSM_BC92_Extended_Error_Report_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Extended_Error_Report_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Fixed_Local_Rate_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Fixed_Local_Rate_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_t GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate );

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Control_Character_Framing_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Control_Character_Framing_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_t GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing );

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Local_Data_Flow_Control_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Local_Data_Flow_Control_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_t GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control );

static GSM_BC92_Status_t GSM_BC92_Enter_PIN_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Enter_PIN_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Enter_PIN_t GSM_BC92_Argument_Enter_PIN );

static GSM_BC92_Status_t GSM_BC92_Request_International_Mobile_Subscriber_Identity_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Request_International_Mobile_Subscriber_Identity_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_USIM_Card_Identification_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_USIM_Card_Identification_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Signal_Quality_Report_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Signal_Quality_Report_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_GSM_Network_Registration_Status_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_GSM_Network_Registration_Status_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GSM_Network_Registration_Status_t GSM_BC92_Argument_GSM_Network_Registration_Status );

static GSM_BC92_Status_t GSM_BC92_GPRS_Network_Registration_Status_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_GPRS_Network_Registration_Status_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status_t GSM_BC92_Argument_GPRS_Network_Registration_Status );

static GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PacketSwitched_t GSM_BC92_Argument_PacketSwitched );

static GSM_BC92_Status_t GSM_BC92_Set_Default_PSD_Connection_Settings_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Set_Default_PSD_Connection_Settings_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Define_A_PDP_Context_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Define_A_PDP_Context_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context );

static GSM_BC92_Status_t GSM_BC92_PDP_Context_Activation_or_Deactivation_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_PDP_Context_Activation_or_Deactivation_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_State_t State );

static GSM_BC92_Status_t GSM_BC92_Show_PDP_Addresses_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Show_PDP_Addresses_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context );

static GSM_BC92_Status_t GSM_BC92_Socket_Open_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Socket_Open_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Socket_Address_t * Address, GSM_BC92_Socket_Port_t Port );

static GSM_BC92_Status_t GSM_BC92_Socket_Write_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Socket_Write_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t DataLength );

static GSM_BC92_Status_t GSM_BC92_Socket_Read_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Socket_Read_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t * DataLength );

static GSM_BC92_Status_t GSM_BC92_Set_UE_Functionality_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Set_UE_Functionality_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_UE_Functionality_t GSM_BC92_Argument_Set_UE_Functionality );

static GSM_BC92_Status_t GSM_BC92_Reset_The_Module_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Reset_The_Module_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Reset_The_Module_t GSM_BC92_Argument_Reset_The_Module );

static GSM_BC92_Status_t GSM_BC92_Power_Off_The_Module_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Power_Off_The_Module_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Power_Off_The_Module_t GSM_BC92_Argument_Power_Off_The_Module );

static GSM_BC92_Status_t GSM_BC92_Return_Current_Date_and_Time_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Return_Current_Date_and_Time_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Query_Power_Supply_Voltage_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Query_Power_Supply_Voltage_Execute( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Select_SMS_Message_Format_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Select_SMS_Message_Format_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Select_SMS_Message_Format_t GSM_BC92_Argument_Select_SMS_Message_Format );

static GSM_BC92_Status_t GSM_BC92_Send_SMS_Message_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Send_SMS_Message_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Send_SMS_Message_t GSM_BC92_Argument_Send_SMS_Message );

static GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach_Status_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_GSM_Registration_Status_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_GPRS_Registration_Status_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Result_Code_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_SMS_Received_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_SIM_Status_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Device_Functionality_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Device_Reset_Handler( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_UnSolicited_Handler( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_IsIdle( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_IsTimeout( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Delay( GSM_BC92_Instance_t * Instance, uint32_t time_sec );

static GSM_BC92_Status_t GSM_BC92_IsReady( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Parse( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Read( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Execute( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Write( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Instance_Context_IsValid( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Instance_Context_Initialize( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Instance_Context_DeInitialize( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Instance_IsValid( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Instance_Initialize( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Instance_DeInitialize( GSM_BC92_Instance_t * Instance );
static GSM_BC92_Status_t GSM_BC92_Instance_Cycle( GSM_BC92_Instance_t * Instance );

static GSM_BC92_Status_t GSM_BC92_Context_Initialize( void );
static GSM_BC92_Status_t GSM_BC92_Context_DeInitialize( void );
static GSM_BC92_Status_t GSM_BC92_Context_Cycle( void );

// #############################################################################
// #### Private Variable(s) ####################################################
// #############################################################################

static GSM_BC92_Context_t GSM_BC92_Context;

// #############################################################################
// #### Private Method(s) ######################################################
// #############################################################################

static GSM_BC92_Status_t GSM_BC92_Process_Socket_Open_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Process.Type != GSM_BC92_ProcessType_Socket_Open )
        {
            GSM_Error( "Invalid Process Handler, Expected %d Found %d", GSM_BC92_ProcessType_Socket_Open, Instance->Context->Process.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;

        if ( GSM_BC92_IsIdle( Instance ) != GSM_BC92_Status_Success )
        {
            // Operation In-progress
            break;
        }
        if ( Instance->Context->Operation.Status != GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Type = GSM_BC92_OperationType_None;
        }
        switch ( Instance->Context->Operation.Type )
        {
            case GSM_BC92_OperationType_Pending:
                Instance->Context->Operation.Status = GSM_BC92_PDP_Context_Activation_or_Deactivation_Execute( Instance, GSM_BC92_Argument_PDP_Context_State_Activate );
                break;
            case GSM_BC92_OperationType_PDP_Context_Activation_Or_Deactivation:
                Instance->Context->Operation.Status = GSM_BC92_Socket_Open_Execute( Instance, Instance->Context->Argument.Address, Instance->Context->Argument.Port );
                break;
            case GSM_BC92_OperationType_Open_A_Socket:
                Instance->Context->Operation.Type = GSM_BC92_OperationType_None;
                break;
            case GSM_BC92_OperationType_None:
            default:
                Instance->Context->Process.Status = Instance->Context->Operation.Status;
                if ( Instance->OnComplete != NULL )
                {
                    Instance->OnComplete( Instance, Instance->Context->Process.Status );
                }
                GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
                break;
        }
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Process_Initialize_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Process.Type != GSM_BC92_ProcessType_Initialize )
        {
            GSM_Error( "Invalid Process Handler, Expected %d Found %d", GSM_BC92_ProcessType_Initialize, Instance->Context->Process.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;

        if ( GSM_BC92_IsIdle( Instance ) != GSM_BC92_Status_Success )
        {
            // Operation In-progress
            break;
        }
        if ( Instance->Context->Operation.Status != GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Type = GSM_BC92_OperationType_None;
        }
        switch ( Instance->Context->Operation.Type )
        {
            case GSM_BC92_OperationType_Pending:
                Instance->Context->Operation.Status = GSM_BC92_Power_On_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Power_On:
                //        Instance->Context->Operation.Status = GSM_BC92_Ping_Execute( Instance );
                //        break;
                //      case GSM_BC92_OperationType_Ping:
                switch ( Instance->Context->Receive.Response.Type )
                {
                    case GSM_BC92_ResponseType_Ready:
                        // If First Power On, Skip Reset
                        Instance->Context->Operation.Type = GSM_BC92_OperationType_Reset_The_Module;
                        break;
                    default:
                        // Otherwise, Reset
                        Instance->Context->Operation.Status = GSM_BC92_Reset_The_Module_Execute( Instance, GSM_BC92_Argument_Reset_The_Module_Immediate );
                        break;
                }
                break;
            case GSM_BC92_OperationType_Reset_The_Module:
                Instance->Context->Operation.Status = GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults_Execute( Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults_0 );
                break;
            case GSM_BC92_OperationType_Set_All_Parameters_To_Manufacturer_Defaults:
                Instance->Context->Operation.Status = GSM_BC92_Set_Command_Echo_Mode_Execute( Instance, GSM_BC92_Argument_Set_Command_Echo_Mode_Off );
                break;
            case GSM_BC92_OperationType_Set_Command_Echo_Mode:
                Instance->Context->Operation.Status = GSM_BC92_GSM_Network_Registration_Status_Execute( Instance, GSM_BC92_Argument_GSM_Network_Registration_Status_Enable_With_Location );
                break;
            case GSM_BC92_OperationType_GSM_Network_Registration_Status:
                Instance->Context->Operation.Status = GSM_BC92_GPRS_Network_Registration_Status_Execute( Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status_Enable_With_Location );
                break;
            case GSM_BC92_OperationType_GPRS_Network_Registration_Status:
                //        Instance->Context->Operation.Status = GSM_BC92_Set_TE_TA_Control_Character_Framing_Execute( Instance, ( GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_t ){ GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format_8_Data_0_Parity_1_Stop, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Parity_Space } );
                //        break;
                //      case GSM_BC92_OperationType_Set_TE_TA_Control_Character_Framing:
                Instance->Context->Operation.Status = GSM_BC92_Set_TE_TA_Local_Data_Flow_Control_Execute( Instance, ( GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_t ) { GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TE_None, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TA_None } );
                break;
            case GSM_BC92_OperationType_Set_TE_TA_Local_Data_Flow_Control:
                //        Instance->Context->Operation.Status = GSM_BC92_Set_TE_TA_Fixed_Local_Rate_Execute( Instance, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_57600 );
                //        break;
                //      case GSM_BC92_OperationType_Set_TE_TA_Fixed_Local_Rate:
                //        Instance->Context->Operation.Status = GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile_Execute( Instance, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile_0 );
                //        break;
                //      case GSM_BC92_OperationType_Store_Current_Parameters_To_User_Defined_Profile:
                Instance->Context->Operation.Status = GSM_BC92_Display_Product_Identification_Information_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Display_Product_Identification_Information:
                Instance->Context->Operation.Status = GSM_BC92_Request_Manufacturer_Identification_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Request_Manufacturer_Identification:
                Instance->Context->Operation.Status = GSM_BC92_Request_Model_Identification_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Request_Model_Identification:
                Instance->Context->Operation.Status = GSM_BC92_Request_Manufacturer_Revision_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Request_Manufacturer_Revision:
                Instance->Context->Operation.Status = GSM_BC92_Request_Product_Serial_Number_Execute( Instance, GSM_BC92_Argument_Request_Product_Serial_Number_IMEI );
                break;
            case GSM_BC92_OperationType_Request_Product_Serial_Number:
                Instance->Context->Operation.Status = GSM_BC92_Request_International_Mobile_Subscriber_Identity_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Request_International_Mobile_Subscriber_Identity:
                Instance->Context->Operation.Status = GSM_BC92_USIM_Card_Identification_Execute( Instance );
                break;
            case GSM_BC92_OperationType_USIM_Card_Identification:
                Instance->Context->Operation.Status = GSM_BC92_Report_Mobile_Termination_Error_Execute( Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error_Disable );
                break;
            case GSM_BC92_OperationType_Report_Mobile_Termination_Error:
                Instance->Context->Operation.Status = GSM_BC92_Extended_Error_Report_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Extended_Error_Report:
                Instance->Context->Operation.Status = GSM_BC92_Set_UE_Functionality_Execute( Instance, ( GSM_BC92_Argument_Set_UE_Functionality_t ) { GSM_BC92_Argument_Set_UE_Functionality_Level_Full, GSM_BC92_Argument_Set_UE_Functionality_Reset_Disable } );
                break;
            case GSM_BC92_OperationType_Set_UE_Functionality:
                Instance->Context->Operation.Status = GSM_BC92_Signal_Quality_Report_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Signal_Quality_Report:
                Instance->Context->Operation.Status = GSM_BC92_Return_Current_Date_and_Time_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Return_Current_Date_And_Time:
                Instance->Context->Operation.Status = GSM_BC92_Query_Power_Supply_Voltage_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Query_Power_Supply_Voltage:
                Instance->Context->Operation.Status = GSM_BC92_Select_SMS_Message_Format_Execute( Instance, GSM_BC92_Argument_Select_SMS_Message_Format_Text );
                break;
            case GSM_BC92_OperationType_Select_SMS_Message_Format:
                Instance->Context->Operation.Status = GSM_BC92_Set_Default_PSD_Connection_Settings_Execute( Instance );
                break;
            case GSM_BC92_OperationType_Set_Default_PSD_Connection_Settings:
                Instance->Context->Operation.Status = GSM_BC92_Define_A_PDP_Context_Execute( Instance, GSM_BC92_Argument_PDP_Context_1 );
                break;
            case GSM_BC92_OperationType_Define_A_PDP_Context:
                //        Instance->Context->Operation.Status = GSM_BC92_Show_PDP_Addresses_Execute( Instance, GSM_BC92_Argument_PDP_Context_1 );
                //        break;
                //      case GSM_BC92_OperationType_Show_PDP_Addresses:
                //        Instance->Context->Operation.Status = GSM_BC92_PS_Attach_or_Detach_Execute( Instance, GSM_BC92_Argument_PacketSwitched_Attach );
                //        break;
                //      case GSM_BC92_OperationType_PS_Attach_Or_Detach:
                Instance->Context->Operation.Type = GSM_BC92_OperationType_None;
                break;
            case GSM_BC92_OperationType_None:
            default:
                Instance->Context->Process.Status = Instance->Context->Operation.Status;
                if ( Instance->OnComplete != NULL )
                {
                    Instance->OnComplete( Instance, Instance->Context->Process.Status );
                }
                GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
                break;
        }
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Process_Command_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Process.Type != GSM_BC92_ProcessType_Command )
        {
            GSM_Error( "Invalid Process Handler, Expected %d Found %d", GSM_BC92_ProcessType_Command, Instance->Context->Process.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;

        if ( GSM_BC92_IsIdle( Instance ) != GSM_BC92_Status_Success )
        {
            // Operation In-progress
            break;
        }
        if ( Instance->Context->Operation.Status != GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Type = GSM_BC92_OperationType_None;
        }
        switch ( Instance->Context->Operation.Type )
        {
            case GSM_BC92_OperationType_Pending:
            case GSM_BC92_OperationType_None:
            default:
                Instance->Context->Process.Status = Instance->Context->Operation.Status;
                if ( Instance->OnComplete != NULL )
                {
                    Instance->OnComplete( Instance, Instance->Context->Process.Status );
                }
                GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
                break;
        }
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Process_Set( GSM_BC92_Instance_t * Instance, GSM_BC92_ProcessType_t GSM_BC92_ProcessType )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Process_Type=%d )", __FUNCTION__, Instance, GSM_BC92_ProcessType );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Process.Type = GSM_BC92_ProcessType;
        Instance->Context->Process.Status = GSM_BC92_Status_Success;
        switch ( Instance->Context->Process.Type )
        {
            case GSM_BC92_ProcessType_Command:
                Instance->Context->Process.Handler = GSM_BC92_Process_Command_Handler;
                break;
            case GSM_BC92_ProcessType_Initialize:
                Instance->Context->Process.Handler = GSM_BC92_Process_Initialize_Handler;
                break;
            case GSM_BC92_ProcessType_Socket_Open:
                Instance->Context->Process.Handler = GSM_BC92_Process_Socket_Open_Handler;
                break;
            case GSM_BC92_ProcessType_None:
            default:
                Instance->Context->Process.Handler = NULL;
                if ( Instance->Context->Process.Type != GSM_BC92_ProcessType_None )
                {
                    Instance->Context->Process.Type = GSM_BC92_ProcessType_None;
                    GSM_Warning( "Un-Handled Process Setting" );
                }
                break;
        }
        switch ( Instance->Context->Process.Type )
        {
            case GSM_BC92_ProcessType_Command:
                // Externally Handled Operation
                break;
            case GSM_BC92_ProcessType_None:
                Instance->Context->Operation.Type = GSM_BC92_OperationType_None;
                break;
            default:
                Instance->Context->Operation.Type = GSM_BC92_OperationType_Pending;
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Power_Off_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Power_Off )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Power_Off, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Success; // Considered as `GSM_BC92_Status_Success` not as `GSM_BC92_Status_Timeout` intentionally as this operation is GPIO operation
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Power_Off_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        GSM_Debug( "Power Off" );
        // TODO GPIO Operation(s)
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Power_Off;
        Instance->Context->Operation.Handler = GSM_BC92_Power_Off_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Power_On_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Power_On )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Power_On, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Success; // Considered as `GSM_BC92_Status_Success` not as `GSM_BC92_Status_Timeout` intentionally as this operation is GPIO operation
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "RDY", UTIL_SizeOfWithoutNull( "RDY" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ready;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Power_On_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }

        if ( GPIO_Write( Instance->Reset, GPIO_Value_Low ) != GPIO_Status_Success )
        {
            Status = GSM_BC92_Status_Error;
            break;
        }

        if ( GPIO_Write( Instance->PowerKey, GPIO_Value_High ) != GPIO_Status_Success )
        {
            Status = GSM_BC92_Status_Error;
            break;
        }

        Instance->Context->Operation.Type = GSM_BC92_OperationType_Power_On;
        Instance->Context->Operation.Handler = GSM_BC92_Power_On_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 5 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Ping_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Ping )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Ping, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT", UTIL_SizeOfWithoutNull( "AT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Ping_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Ping;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Ping;
        Instance->Context->Operation.Handler = GSM_BC92_Ping_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Display_Product_Identification_Information_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Display_Product_Identification_Information )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Display_Product_Identification_Information, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Quectel_Ltd", UTIL_SizeOfWithoutNull( "Quectel_Ltd" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Provider;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Quectel_BC92", UTIL_SizeOfWithoutNull( "Quectel_BC92" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Type;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Revision", UTIL_SizeOfWithoutNull( "Revision" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Software;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ATI", UTIL_SizeOfWithoutNull( "ATI" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Display_Product_Identification_Information_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "I";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Display_Product_Identification_Information;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Display_Product_Identification_Information;
        Instance->Context->Operation.Handler = GSM_BC92_Display_Product_Identification_Information_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 5 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_Command_Echo_Mode_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Set_Command_Echo_Mode )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Set_Command_Echo_Mode, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ATE", UTIL_SizeOfWithoutNull( "ATE" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_Command_Echo_Mode_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_Command_Echo_Mode_t GSM_BC92_Argument_Set_Command_Echo_Mode )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "E";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s%d", command, GSM_BC92_Argument_Set_Command_Echo_Mode );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Set_Command_Echo_Mode;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Set_Command_Echo_Mode;
        Instance->Context->Operation.Handler = GSM_BC92_Set_Command_Echo_Mode_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Store_Current_Parameters_To_User_Defined_Profile )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Store_Current_Parameters_To_User_Defined_Profile, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT&W", UTIL_SizeOfWithoutNull( "AT&W" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile_t GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, User_Defined_Profile=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "&W";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s%d", command, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Store_Current_Parameters_To_User_Defined_Profile;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Store_Current_Parameters_To_User_Defined_Profile;
        Instance->Context->Operation.Handler = GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Set_All_Parameters_To_Manufacturer_Defaults )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Set_All_Parameters_To_Manufacturer_Defaults, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT&F", UTIL_SizeOfWithoutNull( "AT&F" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults_t GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Manufacturer_Profile=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "&F";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s%d", command, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Set_All_Parameters_To_Manufacturer_Defaults;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Set_All_Parameters_To_Manufacturer_Defaults;
        Instance->Context->Operation.Handler = GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Identification_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Request_Manufacturer_Identification )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Request_Manufacturer_Identification, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Quectel_Ltd", UTIL_SizeOfWithoutNull( "Quectel_Ltd" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Provider;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Quectel_BC92", UTIL_SizeOfWithoutNull( "Quectel_BC92" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Type;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Revision", UTIL_SizeOfWithoutNull( "Revision" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Manufacturer;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGMI", UTIL_SizeOfWithoutNull( "AT+CGMI" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Identification_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGMI";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Request_Manufacturer_Identification;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Request_Manufacturer_Identification;
        Instance->Context->Operation.Handler = GSM_BC92_Request_Manufacturer_Identification_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 5 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Model_Identification_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Request_Model_Identification )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Request_Model_Identification, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Quectel_BC92", UTIL_SizeOfWithoutNull( "Quectel_BC92" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Type;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGMM", UTIL_SizeOfWithoutNull( "AT+CGMM" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Model_Identification_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGMM";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Request_Model_Identification;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Request_Model_Identification;
        Instance->Context->Operation.Handler = GSM_BC92_Request_Model_Identification_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Revision_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Request_Manufacturer_Revision )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Request_Manufacturer_Revision, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "Revision", UTIL_SizeOfWithoutNull( "Revision" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Software;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGMR", UTIL_SizeOfWithoutNull( "AT+CGMR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Revision_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGMR";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Request_Manufacturer_Revision;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Request_Manufacturer_Revision;
        Instance->Context->Operation.Handler = GSM_BC92_Request_Manufacturer_Revision_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Product_Serial_Number_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Request_Product_Serial_Number )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Request_Product_Serial_Number, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CGSN", UTIL_SizeOfWithoutNull( "+CGSN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Serial;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGSN", UTIL_SizeOfWithoutNull( "AT+CGSN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_Product_Serial_Number_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Request_Product_Serial_Number_t GSM_BC92_Argument_Request_Product_Serial_Number )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Product_Serial_Number=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Request_Product_Serial_Number );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGSN";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_Request_Product_Serial_Number );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Request_Product_Serial_Number;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Request_Product_Serial_Number;
        Instance->Context->Operation.Handler = GSM_BC92_Request_Product_Serial_Number_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Report_Mobile_Termination_Error_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Report_Mobile_Termination_Error )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Report_Mobile_Termination_Error, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CMEE", UTIL_SizeOfWithoutNull( "AT+CMEE" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Report_Mobile_Termination_Error_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error_t GSM_BC92_Argument_Report_Mobile_Termination_Error )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Report_Mobile_Termination_Error=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CMEE";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_Report_Mobile_Termination_Error );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Report_Mobile_Termination_Error;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Report_Mobile_Termination_Error;
        Instance->Context->Operation.Handler = GSM_BC92_Report_Mobile_Termination_Error_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Extended_Error_Report_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Extended_Error_Report )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Extended_Error_Report, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CEER", UTIL_SizeOfWithoutNull( "AT+CEER" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CEER", UTIL_SizeOfWithoutNull( "+CEER" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Report_Error;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Extended_Error_Report_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CEER";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Extended_Error_Report;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Extended_Error_Report;
        Instance->Context->Operation.Handler = GSM_BC92_Extended_Error_Report_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 3 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Fixed_Local_Rate_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Set_TE_TA_Fixed_Local_Rate )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Set_TE_TA_Fixed_Local_Rate, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+IPR", UTIL_SizeOfWithoutNull( "AT+IPR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+IPR", UTIL_SizeOfWithoutNull( "+IPR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Serial_Rate;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Fixed_Local_Rate_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_t GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+IPR";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Set_TE_TA_Fixed_Local_Rate;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Set_TE_TA_Fixed_Local_Rate;
        Instance->Context->Operation.Handler = GSM_BC92_Set_TE_TA_Fixed_Local_Rate_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Control_Character_Framing_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Set_TE_TA_Control_Character_Framing )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Set_TE_TA_Control_Character_Framing, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+ICF", UTIL_SizeOfWithoutNull( "AT+ICF" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+ICF", UTIL_SizeOfWithoutNull( "+ICF" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Serial_Framing;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Control_Character_Framing_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_t GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Format=%d, Parity=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing.Format, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing.Parity );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+ICF";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d,%d", command, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing.Format, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing.Parity );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Set_TE_TA_Control_Character_Framing;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Set_TE_TA_Control_Character_Framing;
        Instance->Context->Operation.Handler = GSM_BC92_Set_TE_TA_Control_Character_Framing_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Local_Data_Flow_Control_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Set_TE_TA_Local_Data_Flow_Control )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Set_TE_TA_Local_Data_Flow_Control, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+IFC", UTIL_SizeOfWithoutNull( "AT+IFC" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+IFC", UTIL_SizeOfWithoutNull( "+IFC" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Serial_Control;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Local_Data_Flow_Control_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_t GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Flow_Control_TE=%d, Flow_Control_TA=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control.TE, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control.TA );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+IFC";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d,%d", command, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control.TE, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control.TA );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Set_TE_TA_Local_Data_Flow_Control;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Set_TE_TA_Local_Data_Flow_Control;
        Instance->Context->Operation.Handler = GSM_BC92_Set_TE_TA_Local_Data_Flow_Control_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Enter_PIN_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Enter_Pin )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Enter_Pin, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CPIN", UTIL_SizeOfWithoutNull( "AT+CPIN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CPIN", UTIL_SizeOfWithoutNull( "+CPIN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_SIM_Status;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        GSM_BC92_SIM_Status_Handler( Instance );
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Enter_PIN_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Enter_PIN_t GSM_BC92_Argument_Enter_PIN )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PIN=%s )", __FUNCTION__, Instance, GSM_BC92_Argument_Enter_PIN );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CPIN";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%s", command, GSM_BC92_Argument_Enter_PIN );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Enter_Pin;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Enter_Pin;
        Instance->Context->Operation.Handler = GSM_BC92_Enter_PIN_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_International_Mobile_Subscriber_Identity_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Request_International_Mobile_Subscriber_Identity )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Request_International_Mobile_Subscriber_Identity, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        break;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                        break;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CIMI", UTIL_SizeOfWithoutNull( "AT+CIMI" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        break;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "", UTIL_SizeOfWithoutNull( "" ) ) == 0 )
                    {
                        // FIXME Matches Any String
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_SIM_International_Mobile_Subscriber_Identity;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        // TODO Could it be useful to report Mobile Country Codes (MCC), Mobile Network Codes (MNC), and others ?
                        if ( memcmp( Instance->Context->Receive.Response.Value, "60201", UTIL_SizeOfWithoutNull( "60201" ) ) == 0 )
                        {
                            GSM_Debug( "SIM Operator: Orange" );
                        }
                        else if ( memcmp( Instance->Context->Receive.Response.Value, "60202", UTIL_SizeOfWithoutNull( "60202" ) ) == 0 )
                        {
                            GSM_Debug( "SIM Operator: Vodafone" );
                        }
                        else if ( memcmp( Instance->Context->Receive.Response.Value, "60203", UTIL_SizeOfWithoutNull( "60203" ) ) == 0 )
                        {
                            GSM_Debug( "SIM Operator: E&" );
                        }
                        else if ( memcmp( Instance->Context->Receive.Response.Value, "60204", UTIL_SizeOfWithoutNull( "60204" ) ) == 0 )
                        {
                            GSM_Debug( "SIM Operator: WE" );
                        }
                        else
                        {
                            GSM_Debug( "SIM Operator: Unknown" );
                        }
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Request_International_Mobile_Subscriber_Identity_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CIMI";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Request_International_Mobile_Subscriber_Identity;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Request_International_Mobile_Subscriber_Identity;
        Instance->Context->Operation.Handler = GSM_BC92_Request_International_Mobile_Subscriber_Identity_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_USIM_Card_Identification_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_USIM_Card_Identification )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_USIM_Card_Identification, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+QCCID", UTIL_SizeOfWithoutNull( "AT+QCCID" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+QCCID", UTIL_SizeOfWithoutNull( "+QCCID" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_SIM_Integrated_Circuit_Card_Identification;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_USIM_Card_Identification_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+QCCID";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_USIM_Card_Identification;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_USIM_Card_Identification;
        Instance->Context->Operation.Handler = GSM_BC92_USIM_Card_Identification_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Signal_Quality_Report_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Signal_Quality_Report )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Signal_Quality_Report, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CSQ", UTIL_SizeOfWithoutNull( "AT+CSQ" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CSQ", UTIL_SizeOfWithoutNull( "+CSQ" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Signal_Quality_Report;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Signal_Quality_Report_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CSQ";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Signal_Quality_Report;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Signal_Quality_Report;
        Instance->Context->Operation.Handler = GSM_BC92_Signal_Quality_Report_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_GSM_Network_Registration_Status_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_GSM_Network_Registration_Status )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_GSM_Network_Registration_Status, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CREG", UTIL_SizeOfWithoutNull( "AT+CREG" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CREG", UTIL_SizeOfWithoutNull( "+CREG" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_GSM_Network_Registration_Status;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        GSM_BC92_GSM_Registration_Status_Handler( Instance );
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_GSM_Network_Registration_Status_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GSM_Network_Registration_Status_t GSM_BC92_Argument_GSM_Network_Registration_Status )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Registration_Status=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_GSM_Network_Registration_Status );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CREG";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_GSM_Network_Registration_Status );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_GSM_Network_Registration_Status;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_GSM_Network_Registration_Status;
        Instance->Context->Operation.Handler = GSM_BC92_GSM_Network_Registration_Status_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_GPRS_Network_Registration_Status_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_GPRS_Network_Registration_Status )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_GPRS_Network_Registration_Status, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGREG", UTIL_SizeOfWithoutNull( "AT+CGREG" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CGREG", UTIL_SizeOfWithoutNull( "+GCREG" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_GPRS_Network_Registration_Status;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        GSM_BC92_GPRS_Registration_Status_Handler( Instance );
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_GPRS_Network_Registration_Status_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status_t GSM_BC92_Argument_GPRS_Network_Registration_Status )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Registration_Status=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGREG";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_GPRS_Network_Registration_Status );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_GPRS_Network_Registration_Status;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_GPRS_Network_Registration_Status;
        Instance->Context->Operation.Handler = GSM_BC92_GPRS_Network_Registration_Status_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_PS_Attach_Or_Detach )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_PS_Attach_Or_Detach, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGATT", UTIL_SizeOfWithoutNull( "AT+CGATT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CGATT", UTIL_SizeOfWithoutNull( "+CGATT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_PS_Attach_or_Detach;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        GSM_BC92_PS_Attach_or_Detach_Status_Handler( Instance );
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PacketSwitched_t GSM_BC92_Argument_PacketSwitched )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PacketSwitched=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_PacketSwitched );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGATT";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_PacketSwitched );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_PS_Attach_Or_Detach;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_PS_Attach_Or_Detach;
        Instance->Context->Operation.Handler = GSM_BC92_PS_Attach_or_Detach_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 120 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_Default_PSD_Connection_Settings_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Set_Default_PSD_Connection_Settings )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Set_Default_PSD_Connection_Settings, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+QCGDEFCONT", UTIL_SizeOfWithoutNull( "AT+QCGDEFCONT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+QCGDEFCONT", UTIL_SizeOfWithoutNull( "+QCGDEFCONT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Set_Default_PSD_Connection_Settings;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_Default_PSD_Connection_Settings_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+QCGDEFCONT";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=\"%s\"", command, "IPV4V6" );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Set_Default_PSD_Connection_Settings;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Set_Default_PSD_Connection_Settings;
        Instance->Context->Operation.Handler = GSM_BC92_Set_Default_PSD_Connection_Settings_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Define_A_PDP_Context_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Define_A_PDP_Context )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Define_A_PDP_Context, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGDCONT", UTIL_SizeOfWithoutNull( "AT+CGDCONT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CGDCONT", UTIL_SizeOfWithoutNull( "+CGDCONT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Define_A_PDP_Context;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Define_A_PDP_Context_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PDP_Context=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_PDP_Context );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGDCONT";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d,\"IP\"", command, GSM_BC92_Argument_PDP_Context );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Define_A_PDP_Context;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Define_A_PDP_Context;
        Instance->Context->Operation.Handler = GSM_BC92_Define_A_PDP_Context_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_PDP_Context_Activation_or_Deactivation_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_PDP_Context_Activation_Or_Deactivation )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_PDP_Context_Activation_Or_Deactivation, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGACT", UTIL_SizeOfWithoutNull( "AT+CGACT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CGACT", UTIL_SizeOfWithoutNull( "+CGACT" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_PDP_Context_Activation_Or_Deactivation;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_PDP_Context_Activation_or_Deactivation_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_State_t State )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, State=%d )", __FUNCTION__, Instance, State );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGACT";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, State );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_PDP_Context_Activation_Or_Deactivation;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_PDP_Context_Activation_Or_Deactivation;
        Instance->Context->Operation.Handler = GSM_BC92_PDP_Context_Activation_or_Deactivation_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 30 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Show_PDP_Addresses_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Show_PDP_Addresses )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Show_PDP_Addresses, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CGPADDR", UTIL_SizeOfWithoutNull( "AT+CGPADDR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CGPADDR", UTIL_SizeOfWithoutNull( "+CGPADDR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Show_PDP_Addresses;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Show_PDP_Addresses_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PDP_Address=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_PDP_Context );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CGPADDR";
        // FIXME Use the following
        //    Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_PDP_Context );
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Show_PDP_Addresses;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Show_PDP_Addresses;
        Instance->Context->Operation.Handler = GSM_BC92_Show_PDP_Addresses_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Socket_Open_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Open_A_Socket )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Open_A_Socket, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+QIOPEN", UTIL_SizeOfWithoutNull( "AT+QIOPEN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+QIOPEN", UTIL_SizeOfWithoutNull( "+QIOPEN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Open_Socket;
                        Instance->Context->Operation.Handler = NULL;

                        int32_t socket_id = 0;
                        int32_t result = 0;
                        sscanf( Instance->Context->Receive.Response.Value, "+QIOPEN: %ld,%ld", &socket_id, &result );

                        switch ( result )
                        {
                            case 0:
                            case 563:
                                Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                                break;
                            default:
                                Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                                break;
                        }

                        // FIXME Determine when to report socket open or not, and how to indicate errors
                        if ( Instance->OnSocketOpen != NULL )
                        {
                            Instance->OnSocketOpen( Instance );
                        }
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Socket_Open_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Socket_Address_t * Address, GSM_BC92_Socket_Port_t Port )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Address=%s, Port=%d )", __FUNCTION__, Instance, Address, Port );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+QIOPEN";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=1,0,\"TCP\",\"%s\",%d", command, Address, Port );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Open_A_Socket;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Open_A_Socket;
        Instance->Context->Operation.Handler = GSM_BC92_Socket_Open_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Socket_Write_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Send_Text_String_Data )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Send_Text_String_Data, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+QISEND", UTIL_SizeOfWithoutNull( "AT+QISEND" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "SEND OK", UTIL_SizeOfWithoutNull( "SEND OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Send_Text_String_Data;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        // TODO Data Send Success Callback
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "SEND FAIL", UTIL_SizeOfWithoutNull( "SEND FAIL" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Send_Text_String_Data;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                        // TODO Data Send Fail Callback
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Socket_Write_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t DataLength )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Data[%d]=%s )", __FUNCTION__, Instance, DataLength, Data );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+QISEND";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=0,%d,\"%s\"", command, DataLength, Data );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Send_Text_String_Data;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Send_Text_String_Data;
        Instance->Context->Operation.Handler = GSM_BC92_Socket_Write_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Socket_Read_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Retrieve_The_Received_Tcp_Ip_Data )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Retrieve_The_Received_Tcp_Ip_Data, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+QIRD", UTIL_SizeOfWithoutNull( "AT+QIRD" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+QIRD", UTIL_SizeOfWithoutNull( "+QIRD" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Retrieve_The_Received_Tcp_Ip_Data;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;

                        int32_t read_length = 0;
                        sscanf( Instance->Context->Receive.Response.Value, "+QIRD: %ld", &read_length );
                        for ( int32_t i = 0; i < read_length; ++i )
                        {
                            GSM_BC92_Read( Instance );
                        }

                        GSM_Debug( "Received Data[%d]=%s", read_length, Instance->Context->Receive.Content + Instance->Context->Receive.Length - read_length );
                        // TODO Data Read Callback
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Socket_Read_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t * DataLength )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Data[%d]=%p )", __FUNCTION__, Instance, *DataLength, Data );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+QIRD";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=0,%d", command, *DataLength );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Retrieve_The_Received_Tcp_Ip_Data;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Retrieve_The_Received_Tcp_Ip_Data;
        Instance->Context->Operation.Handler = GSM_BC92_Socket_Read_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_UE_Functionality_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Set_UE_Functionality )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Set_UE_Functionality, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CFUN", UTIL_SizeOfWithoutNull( "AT+CSQ" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CFUN", UTIL_SizeOfWithoutNull( "+CFUN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Functionality;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        GSM_BC92_Device_Functionality_Handler( Instance );
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Set_UE_Functionality_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_UE_Functionality_t GSM_BC92_Argument_Set_UE_Functionality )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, UE_Functionality_Level=%d, UE_Functionality_Reset=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_UE_Functionality.Level, GSM_BC92_Argument_Set_UE_Functionality.Reset );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CFUN";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d,%d", command, GSM_BC92_Argument_Set_UE_Functionality.Level, GSM_BC92_Argument_Set_UE_Functionality.Reset );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Set_UE_Functionality;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Set_UE_Functionality;
        Instance->Context->Operation.Handler = GSM_BC92_Set_UE_Functionality_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Reset_The_Module_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Reset_The_Module )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Reset_The_Module, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "RDY", UTIL_SizeOfWithoutNull( "RDY" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ready;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+QRST", UTIL_SizeOfWithoutNull( "AT+QRST" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Reset_The_Module_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Reset_The_Module_t GSM_BC92_Argument_Reset_The_Module )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Reset=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Reset_The_Module );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+QRST";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_Reset_The_Module );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Reset_The_Module;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Reset_The_Module;
        Instance->Context->Operation.Handler = GSM_BC92_Reset_The_Module_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Power_Off_The_Module_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Power_Off_The_Module )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Power_Off_The_Module, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "NORMAL POWER DOWN", UTIL_SizeOfWithoutNull( "NORMAL POWER DOWN" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Normal_Power_Down;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+QPOWD", UTIL_SizeOfWithoutNull( "AT+QPOWD" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Power_Off_The_Module_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Power_Off_The_Module_t GSM_BC92_Argument_Power_Off_The_Module )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Mode=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Power_Off_The_Module );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+QPOWD";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_Power_Off_The_Module );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Power_Off_The_Module;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Power_Off_The_Module;
        Instance->Context->Operation.Handler = GSM_BC92_Power_Off_The_Module_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Return_Current_Date_and_Time_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Return_Current_Date_And_Time )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Return_Current_Date_And_Time, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CCLK", UTIL_SizeOfWithoutNull( "AT+CCLK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CCLK", UTIL_SizeOfWithoutNull( "+CCLK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Current_Date_And_Time;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;

                        GSM_BC92_Timestamp_t Timestamp;
                        sscanf( Instance->Context->Receive.Response.Value, "+CCLK: \"%d/%d/%d,%d:%d:%d\"", &Timestamp.Year, &Timestamp.Month, &Timestamp.Day, &Timestamp.Hour, &Timestamp.Minute, &Timestamp.Second );
                        Timestamp.Year += 2000;

                        if ( Instance->OnTime != NULL )
                        {
                            Instance->OnTime( Instance, Timestamp );
                        }
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Return_Current_Date_and_Time_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CCLK";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s?", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Return_Current_Date_And_Time;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Return_Current_Date_And_Time;
        Instance->Context->Operation.Handler = GSM_BC92_Return_Current_Date_and_Time_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Query_Power_Supply_Voltage_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Query_Power_Supply_Voltage )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Query_Power_Supply_Voltage, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CBC", UTIL_SizeOfWithoutNull( "AT+CBC" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CBC", UTIL_SizeOfWithoutNull( "+CBC" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Power_Supply_Voltage;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                        int32_t voltage = 0;
                        sscanf( Instance->Context->Receive.Response.Value, "+CBC: %*d,%*d,%ld", &voltage );
                        if ( Instance->OnVoltage != NULL )
                        {
                            Instance->OnVoltage( Instance, voltage );
                        }
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Query_Power_Supply_Voltage_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CBC";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s", command );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Query_Power_Supply_Voltage;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Query_Power_Supply_Voltage;
        Instance->Context->Operation.Handler = GSM_BC92_Query_Power_Supply_Voltage_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Select_SMS_Message_Format_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Select_SMS_Message_Format )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Select_SMS_Message_Format, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CMGF", UTIL_SizeOfWithoutNull( "AT+CMGF" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Select_SMS_Message_Format_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Select_SMS_Message_Format_t GSM_BC92_Argument_Select_SMS_Message_Format )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Format=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Select_SMS_Message_Format );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CMGF";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%d", command, GSM_BC92_Argument_Select_SMS_Message_Format );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Select_SMS_Message_Format;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Select_SMS_Message_Format;
        Instance->Context->Operation.Handler = GSM_BC92_Select_SMS_Message_Format_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Send_SMS_Message_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_Send_SMS_Message )
        {
            GSM_Error( "Invalid Operation Handler, Expected %d Found %d", GSM_BC92_OperationType_Send_SMS_Message, Instance->Context->Operation.Type );
            GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_None );
            Status = GSM_BC92_Status_Error;
            break;
        }
        if ( ( Status = GSM_BC92_IsTimeout( Instance ) ) == GSM_BC92_Status_Success )
        {
            Instance->Context->Operation.Handler = NULL;
            Instance->Context->Operation.Status = GSM_BC92_Status_Timeout;
        }
        else
        {
            switch ( Instance->Context->Receive.Response.Type )
            {
                case GSM_BC92_ResponseType_Unknown:
                    // Solicited Response(s)
                    if ( memcmp( Instance->Context->Receive.Response.Value, "OK", UTIL_SizeOfWithoutNull( "OK" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ok;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, ">", UTIL_SizeOfWithoutNull( ">" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_SMS_Start;
                        Instance->Context->Operation.Status = GSM_BC92_Write( Instance );
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "ERROR", UTIL_SizeOfWithoutNull( "ERROR" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Error;
                        Instance->Context->Operation.Handler = NULL;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Error;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "AT+CMGS", UTIL_SizeOfWithoutNull( "AT+CMGS" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Echo;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    if ( memcmp( Instance->Context->Receive.Response.Value, "+CMGS", UTIL_SizeOfWithoutNull( "+CMGS" ) ) == 0 )
                    {
                        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_SMS_Sent;
                        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
                    }
                    // Unsolicited Response(s)
                    break;
                case GSM_BC92_ResponseType_None:
                default:
                    // No or Unsolicited Response(s)
                    break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Send_SMS_Message_Execute( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Send_SMS_Message_t GSM_BC92_Argument_Send_SMS_Message )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Number=%s, Content=%s )", __FUNCTION__, Instance, GSM_BC92_Argument_Send_SMS_Message.Number, GSM_BC92_Argument_Send_SMS_Message.Content );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char command[] = "+CMGS";
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s=%s", command, GSM_BC92_Argument_Send_SMS_Message.Number );
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_Send_SMS_Message;
        if ( ( Status = GSM_BC92_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Instance->Context->Transmit.Length = UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Command.Value, 0, UTIL_SizeOf( Instance->Context->Transmit.Content ), "%s\x1A", GSM_BC92_Argument_Send_SMS_Message.Content );
        Instance->Context->Operation.Type = GSM_BC92_OperationType_Send_SMS_Message;
        Instance->Context->Operation.Handler = GSM_BC92_Send_SMS_Message_Handler;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;
        GSM_BC92_Delay( Instance, 10 );
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach_Status_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( ( char * ) Instance->Context->Receive.Response.Value, "+CGATT", UTIL_SizeOfWithoutNull( "+CGATT" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_PS_Attach_or_Detach;
                // no break
            case GSM_BC92_ResponseType_PS_Attach_or_Detach:
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGATT: 1", UTIL_SizeOfWithoutNull( "+CGATT: 1" ) ) == 0 )
                {
                    // TODO Call PS Network Callback
                    GSM_Debug( "Packet Switched Attached" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGATT: 0", UTIL_SizeOfWithoutNull( "+CGATT: 0" ) ) == 0 )
                {
                    // TODO Call PS Network Callback
                    GSM_Debug( "Packet Switched Detached" );
                }
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_SMS_Received_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( ( char * ) Instance->Context->Receive.Response.Value, "+CMTI", UTIL_SizeOfWithoutNull( "+CMTI" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_SMS_Received;
                // no break
            case GSM_BC92_ResponseType_SMS_Received:
                GSM_Debug( "SMS Received" );
                // TODO Call SMS Received Callback
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_GPRS_Registration_Status_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( ( char * ) Instance->Context->Receive.Response.Value, "+CGREG", UTIL_SizeOfWithoutNull( "+CGREG" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_GPRS_Network_Registration_Status;
                // no break
            case GSM_BC92_ResponseType_GPRS_Network_Registration_Status:
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGREG: 5", UTIL_SizeOfWithoutNull( "+CGGREG: 5" ) ) == 0 )
                {
                    // TODO Call GPRS Network Callback
                    GSM_Debug( "GPRS Network Registered (Roaming)" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGREG: 4", UTIL_SizeOfWithoutNull( "+CGGREG: 4" ) ) == 0 )
                {
                    // TODO Call GPRS Network Callback
                    GSM_Debug( "GPRS Network Unknown" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGREG: 3", UTIL_SizeOfWithoutNull( "+CGREG: 3" ) ) == 0 )
                {
                    // TODO Call GPRS Network Callback
                    GSM_Debug( "GPRS Network Registration Denied" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGREG: 2", UTIL_SizeOfWithoutNull( "+CGREG: 2" ) ) == 0 )
                {
                    // TODO Call GPRS Network Callback
                    GSM_Debug( "GPRS Network Search" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGREG: 1", UTIL_SizeOfWithoutNull( "+CGREG: 1" ) ) == 0 )
                {
                    // TODO Call GPRS Network Callback
                    GSM_Debug( "GPRS Network Registered" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CGREG: 0", UTIL_SizeOfWithoutNull( "+CGREG: 0" ) ) == 0 )
                {
                    // TODO Call GPRS Network Callback
                    GSM_Debug( "GPRS Network Not Registered" );
                }
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

/**
 * @brief Unsolicited Result Code (URC) Handler
 */
static GSM_BC92_Status_t GSM_BC92_Result_Code_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( ( char * ) Instance->Context->Receive.Response.Value, "+QIURC", UTIL_SizeOfWithoutNull( "+QIURC" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Socket_UnsolicitedResultCode;
                // no break
            case GSM_BC92_ResponseType_Socket_UnsolicitedResultCode:
                if ( memcmp( Instance->Context->Receive.Response.Value, "+QIURC: \"closed\"", UTIL_SizeOfWithoutNull( "+QIURC: \"closed\"" ) ) == 0 )
                {
                    int32_t socket_id = 0;
                    sscanf( Instance->Context->Receive.Response.Value, "+QIURC: \"closed\",%ld", &socket_id );
                    if ( Instance->OnSocketClose != NULL )
                    {
                        Instance->OnSocketClose( Instance );
                    }
                    break;
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+QIURC: \"recv\"", UTIL_SizeOfWithoutNull( "+QIURC: \"recv\"" ) ) == 0 )
                {
                    bool full_buffer = false;
                    int32_t socket_id = 0;
                    sscanf( Instance->Context->Receive.Response.Value, "+QIURC: \"recv\",%ld", &socket_id );
                    full_buffer = sscanf( Instance->Context->Receive.Response.Value, "+QIURC: \"recv\",%*ld,\"buff full\"" ) == 0 ? true : false;
                    GSM_Debug( "Received Data Indication %d%s", socket_id, ( char *[] ) { "", ": FULL" }[ full_buffer ] );
                    // TODO Call Socket Data Callback
                    break;
                }
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_GSM_Registration_Status_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( ( char * ) Instance->Context->Receive.Response.Value, "+CREG", UTIL_SizeOfWithoutNull( "+CREG" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_GSM_Network_Registration_Status;
                // no break
            case GSM_BC92_ResponseType_GSM_Network_Registration_Status:
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CREG: 4", UTIL_SizeOfWithoutNull( "+CREG: 4" ) ) == 0 )
                {
                    // TODO Call GSM Network Callback
                    GSM_Debug( "GSM Network Unknown" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CREG: 3", UTIL_SizeOfWithoutNull( "+CREG: 3" ) ) == 0 )
                {
                    // TODO Call GSM Network Callback
                    GSM_Debug( "GSM Network Registration Denied" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CREG: 2", UTIL_SizeOfWithoutNull( "+CREG: 2" ) ) == 0 )
                {
                    // TODO Call GSM Network Callback
                    GSM_Debug( "GSM Network Search" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CREG: 1", UTIL_SizeOfWithoutNull( "+CREG: 1" ) ) == 0 )
                {
                    // TODO Call GSM Network Callback
                    GSM_Debug( "GSM Network Registered" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CREG: 0", UTIL_SizeOfWithoutNull( "+CREG: 0" ) ) == 0 )
                {
                    // TODO Call GSM Network Callback
                    GSM_Debug( "GSM Network Not Registered" );
                }
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_SIM_Status_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( ( char * ) Instance->Context->Receive.Response.Value, "+CPIN", UTIL_SizeOfWithoutNull( "+CPIN" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_SIM_Status;
                // no break
            case GSM_BC92_ResponseType_SIM_Status:
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CPIN: READY", UTIL_SizeOfWithoutNull( "+CPIN: READY" ) ) == 0 )
                {
                    // TODO Call SIM Ready Status Callback
                    GSM_Debug( "SIM Status Ready" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CPIN: SIM PIN", UTIL_SizeOfWithoutNull( "+CPIN: SIM PIN" ) ) == 0 )
                {
                    // TODO Call SIM Requires PIN Status Callback
                    GSM_Debug( "SIM Status Requires PIN" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CPIN: SIM PUK", UTIL_SizeOfWithoutNull( "+CPIN: SIM PUK" ) ) == 0 )
                {
                    // TODO Call SIM Requires PUK Status Callback
                    GSM_Debug( "SIM Status Requires PUK" );
                }
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Device_Functionality_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( ( char * ) Instance->Context->Receive.Response.Value, "+CFUN", UTIL_SizeOfWithoutNull( "+CFUN" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Device_Functionality;
                // no break
            case GSM_BC92_ResponseType_Device_Functionality:
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CFUN: 0", UTIL_SizeOfWithoutNull( "+CFUN: 0" ) ) == 0 )
                {
                    // TODO Call Device Functionality Minimal Callback
                    GSM_Debug( "Device Functionality Minimal" );
                }
                if ( memcmp( Instance->Context->Receive.Response.Value, "+CFUN: 1", UTIL_SizeOfWithoutNull( "+CFUN: 1" ) ) == 0 )
                {
                    // TODO Call Device Functionality Full Callback
                    GSM_Debug( "Device Functionality Full" );
                }
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Device_Reset_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                if ( memcmp( Instance->Context->Receive.Response.Value, "RDY", UTIL_SizeOfWithoutNull( "RDY" ) ) != 0 )
                {
                    // Not the right handler, skip
                    break;
                }
                Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Ready;
                // no break
            case GSM_BC92_ResponseType_Ready:
                GSM_Debug( "Device Powered On / Reset" );
                // TODO Call Power On / Reset Callback
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_UnSolicited_Handler( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        switch ( Instance->Context->Receive.Response.Type )
        {
            case GSM_BC92_ResponseType_Unknown:
                // Unsolicited Response(s)
                GSM_BC92_Device_Reset_Handler( Instance );
                GSM_BC92_Device_Functionality_Handler( Instance );
                GSM_BC92_SIM_Status_Handler( Instance );
                GSM_BC92_SMS_Received_Handler( Instance );
                GSM_BC92_GSM_Registration_Status_Handler( Instance );
                GSM_BC92_GPRS_Registration_Status_Handler( Instance );
                GSM_BC92_Result_Code_Handler( Instance );
                break;
            case GSM_BC92_ResponseType_None:
            default:
                // No or Not Related Response(s)
                break;
        }
        if ( Instance->Context->Receive.Response.Type == GSM_BC92_ResponseType_Unknown )
        {
            GSM_Warning( "No Handler" );
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_IsIdle( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Handler != NULL )
        {
            Status = GSM_BC92_Status_Busy;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_IsTimeout( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        TIM_Status_t TIM_Status = TIM_Status_Error;
        if ( ( TIM_Status = TIM_IsExpiredTimestamp( GSM_TIM, &Instance->Context->Operation.Timestamp ) ) != TIM_Status_Success )
        {
            Status = GSM_BC92_Status_Busy;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Delay( GSM_BC92_Instance_t * Instance, uint32_t time_sec )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, time_sec=%d )", __FUNCTION__, Instance, time_sec );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        TIM_Status_t TIM_Status = TIM_Status_Error;
        if ( ( TIM_Status = TIM_Timestamp_AddSecond( &Instance->Context->Operation.Timestamp, time_sec ) ) != TIM_Status_Success )
        {
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_IsReady( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Type != GSM_BC92_OperationType_None
             || Instance->Context->Process.Type != GSM_BC92_ProcessType_None )
        {
            Status = GSM_BC92_Status_Busy;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Parse( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Read( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        const char delimiter[] = "\r\n";
        if ( strstr( ( char * ) Instance->Context->Receive.Content, delimiter ) == ( char * ) Instance->Context->Receive.Content )
        {
            // start delimiter remove
            memmove( Instance->Context->Receive.Content, Instance->Context->Receive.Content + UTIL_SizeOfWithoutNull( delimiter ), Instance->Context->Receive.Length - UTIL_SizeOfWithoutNull( delimiter ) );
            Instance->Context->Receive.Length -= UTIL_SizeOfWithoutNull( delimiter );
            Instance->Context->Receive.Content[ Instance->Context->Receive.Length ] = 0;
            Status = GSM_BC92_Status_Success;
            break;
        }
        if ( strstr( ( char * ) Instance->Context->Receive.Content, delimiter ) == NULL )
        {
            // end delimiter not yet received
            Status = GSM_BC92_Status_Success;
            if ( Instance->Context->Transmit.Command.Type == GSM_BC92_CommandType_Send_SMS_Message
                 && memcmp( Instance->Context->Receive.Content, "> ", UTIL_SizeOfWithoutNull( "> " ) ) == 0 )
            {
                // FIXME SMS Content Handling
                // Note: Consider end delimiter received and continue
                Instance->Context->Receive.Length += UTIL_SizeOfWithoutNull( delimiter );
            }
            else
            {
                break;
            }
        }
        // end delimiter remove
        Instance->Context->Receive.Length -= UTIL_SizeOfWithoutNull( delimiter );
        Instance->Context->Receive.Content[ Instance->Context->Receive.Length ] = 0;
        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_Unknown;

        GSM_Debug( "Received [%d]: %s", Instance->Context->Receive.Length, Instance->Context->Receive.Content );
        if ( Instance->Context->Operation.Handler != NULL )
        {
            if ( ( Status = Instance->Context->Operation.Handler( Instance ) ) != GSM_BC92_Status_Success )
            {
                // FIXME Operation Failure handling
                GSM_Warning( "Solicited Response [%d]: %s, Status: %d", Instance->Context->Receive.Length, Instance->Context->Receive.Content, Status );
            }
        }
        if ( Instance->Context->Receive.Response.Type == GSM_BC92_ResponseType_Unknown )
        {
            if ( ( Status = GSM_BC92_UnSolicited_Handler( Instance ) ) != GSM_BC92_Status_Success )
            {
                // FIXME Unknown response handling
                GSM_Warning( "UnSolicited Response [%d]: %s, Status: %d", Instance->Context->Receive.Length, Instance->Context->Receive.Content, Status );
            }
        }
        if ( Instance->Context->Process.Handler != NULL )
        {
            if ( ( Status = Instance->Context->Process.Handler( Instance ) ) != GSM_BC92_Status_Success )
            {
                // FIXME Process Failure handling
            }
        }
        // Remove Response
        Instance->Context->Receive.Length = 0;
        Instance->Context->Receive.Content[ Instance->Context->Receive.Length ] = 0;
        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_None;
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Read( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        UART_Status_t UART_Status = UART_Status_Error;
        if ( ( UART_Status = UART_Read( Instance->UARTx, Instance->Context->Receive.Content + Instance->Context->Receive.Length, 1 ) ) == UART_Status_Success )
        {
            Instance->Context->Receive.Length += 1;
            if ( Instance->Context->Receive.Length >= UTIL_SizeOf( Instance->Context->Receive.Content ) )
            {
                GSM_Error( "Reached MAX supported size" );
                Instance->Context->Receive.Length = 0;
            }
            Instance->Context->Receive.Content[ Instance->Context->Receive.Length ] = 0;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Execute( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Transmit.Length + UTIL_SizeOfWithoutNull( "AT" ) + UTIL_SizeOfWithoutNull( "\r" ) >= UTIL_SizeOf( Instance->Context->Transmit.Content ) )
        {
            GSM_Error( "Reached MAX Supported Size!" );
            Status = GSM_BC92_Status_Error;
        }
        else
        {
            UTIL_MemoryCopy( Instance->Context->Transmit.Content + UTIL_SizeOfWithoutNull( "AT" ), Instance->Context->Transmit.Content, Instance->Context->Transmit.Length );
            UTIL_MemoryCopy( Instance->Context->Transmit.Content, "AT", UTIL_SizeOfWithoutNull( "AT" ) );
            Instance->Context->Transmit.Length += UTIL_SizeOfWithoutNull( "AT" );
            GSM_Debug( "Execute [%d]: %.*s", Instance->Context->Transmit.Length, Instance->Context->Transmit.Length, Instance->Context->Transmit.Content );
            Instance->Context->Transmit.Length += UTIL_StringFormatBufferAppend( Instance->Context->Transmit.Content, Instance->Context->Transmit.Length, UTIL_SizeOf( Instance->Context->Transmit.Content ), "\r" );
            Status = GSM_BC92_Write( Instance );
        }
        Instance->Context->Transmit.Length = 0;
        Instance->Context->Transmit.Content[ Instance->Context->Transmit.Length ] = 0;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Write( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        UART_Status_t UART_Status = UART_Status_Error;
        if ( ( UART_Status = UART_Write( Instance->UARTx, ( uint8_t * ) Instance->Context->Transmit.Content, Instance->Context->Transmit.Length ) ) != UART_Status_Success )
        {
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Instance_Context_IsValid( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context == NULL )
        {
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Instance_Context_Initialize( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            RAM_Status_t RAM_Status = RAM_Status_Success;
            if ( ( RAM_Status = RAM_Allocate( RAM_1, ( RAM_Reference_t * ) &Instance->Context, UTIL_SizeOf( GSM_BC92_Instance_Context_t ) ) ) != RAM_Status_Success )
            {
                Status = GSM_BC92_Status_Error;
                break;
            }
            // Double check context validity
            if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
            {
                break;
            }
        }
        Instance->Context->Transmit.Length = 0;
        Instance->Context->Transmit.Command.Type = GSM_BC92_CommandType_None;
        Instance->Context->Transmit.Content[ Instance->Context->Transmit.Length ] = 0;

        Instance->Context->Receive.Length = 0;
        Instance->Context->Receive.Response.Type = GSM_BC92_ResponseType_None;
        Instance->Context->Receive.Content[ Instance->Context->Receive.Length ] = 0;

        Instance->Context->Operation.Type = GSM_BC92_OperationType_None;
        Instance->Context->Operation.Handler = NULL;
        Instance->Context->Operation.Status = GSM_BC92_Status_Success;
        Instance->Context->Operation.Timestamp = GSM_BC92_Context.Timestamp;

        Instance->Context->Process.Type = GSM_BC92_ProcessType_None;
        Instance->Context->Process.Handler = NULL;
        Instance->Context->Process.Status = GSM_BC92_Status_Success;

        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Instance_Context_DeInitialize( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) == GSM_BC92_Status_Success )
        {
            RAM_Status_t RAM_Status = RAM_Status_Success;
            if ( ( RAM_Status = RAM_DeAllocate( RAM_1, ( RAM_Reference_t * ) &Instance->Context ) ) != RAM_Status_Success )
            {
                Status = GSM_BC92_Status_Error;
                break;
            }
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Instance_IsValid( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( Instance == NULL )
        {
            GSM_Error( "Invalid Argument" );
            Status = GSM_BC92_Status_ArgumentInvalid;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Instance_Initialize( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        Status = GSM_BC92_Instance_Context_Initialize( Instance );
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Instance_DeInitialize( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        Status = GSM_BC92_Instance_Context_DeInitialize( Instance );
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Instance_Cycle( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Context_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Parse( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( Instance->Context->Operation.Handler != NULL )
        {
            if ( ( Status = Instance->Context->Operation.Handler( Instance ) ) != GSM_BC92_Status_Success )
            {
                // FIXME Operation Failure Handling
            }
        }
        if ( Instance->Context->Process.Handler != NULL )
        {
            if ( ( Status = Instance->Context->Process.Handler( Instance ) ) != GSM_BC92_Status_Success )
            {
                // FIXME Process Failure Handling
            }
        }
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Context_Initialize( void )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( void )", __FUNCTION__ );
        TIM_Status_t TIM_Status = TIM_Status_Error;
        if ( ( TIM_Status = TIM_GetTimestamp( GSM_TIM, &GSM_BC92_Context.Timestamp ) ) != TIM_Status_Success )
        {
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Context_DeInitialize( void )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( void )", __FUNCTION__ );
        // Nothing to be done
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

static GSM_BC92_Status_t GSM_BC92_Context_Cycle( void )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( void )", __FUNCTION__ );
        TIM_Status_t TIM_Status = TIM_Status_Error;
        if ( ( TIM_Status = TIM_GetTimestamp( GSM_TIM, &GSM_BC92_Context.Timestamp ) ) != TIM_Status_Success )
        {
            Status = GSM_BC92_Status_Error;
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

// #############################################################################
// #### Public Method(s) #######################################################
// #############################################################################

GSM_BC92_Status_t GSM_BC92_Initialize( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Context_Initialize( ) ) != GSM_BC92_Status_Success )
        {
            // FIXME Context Initialization Failure Handling
        }
        if ( ( Status = GSM_BC92_Instance_IsValid( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Instance_Initialize( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Initialize ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Cycle( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_Context_Cycle( ) ) != GSM_BC92_Status_Success )
        {
            GSM_Warning( "Context Cycle Failed: %d", Status );
        }
        Status = GSM_BC92_Instance_Cycle( Instance );
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_DeInitialize( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Success;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Power_Off( GSM_BC92_Instance_t * Instance )
{
}

GSM_BC92_Status_t GSM_BC92_Power_On( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Power_On_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Ping( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Ping_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Display_Product_Identification_Information( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Display_Product_Identification_Information_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Set_Command_Echo_Mode( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_Command_Echo_Mode_t GSM_BC92_Argument_Set_Command_Echo_Mode )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Set_Command_Echo_Mode_Execute( Instance, GSM_BC92_Argument_Set_Command_Echo_Mode ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile_t GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile_Execute( Instance, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults_t GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Manufacturer_Profile=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults_Execute( Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Identification( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Request_Manufacturer_Identification_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Request_Model_Identification( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Request_Model_Identification_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Revision( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Request_Manufacturer_Revision_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Request_Product_Serial_Number( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Request_Product_Serial_Number_t GSM_BC92_Argument_Request_Product_Serial_Number )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Product_Serial_Number=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Request_Product_Serial_Number );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Request_Product_Serial_Number_Execute( Instance, GSM_BC92_Argument_Request_Product_Serial_Number ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Report_Mobile_Termination_Error( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error_t GSM_BC92_Argument_Report_Mobile_Termination_Error )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Report_Mobile_Termination_Error=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Report_Mobile_Termination_Error_Execute( Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Extended_Error_Report( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Extended_Error_Report_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Fixed_Local_Rate( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_t GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Local_Rate=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Set_TE_TA_Fixed_Local_Rate_Execute( Instance, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Control_Character_Framing( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_t GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Format=%d, Parity=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing.Format, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing.Parity );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Set_TE_TA_Control_Character_Framing_Execute( Instance, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Local_Data_Flow_Control( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_t GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Flow_Control_TE=%d, Flow_Control_TA=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control.TE, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control.TA );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Set_TE_TA_Local_Data_Flow_Control_Execute( Instance, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Enter_PIN( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Enter_PIN_t GSM_BC92_Argument_Enter_PIN )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PIN=%s )", __FUNCTION__, Instance, GSM_BC92_Argument_Enter_PIN );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Enter_PIN_Execute( Instance, GSM_BC92_Argument_Enter_PIN ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Request_International_Mobile_Subscriber_Identity( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Request_International_Mobile_Subscriber_Identity_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_USIM_Card_Identification( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_USIM_Card_Identification_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Signal_Quality_Report( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Signal_Quality_Report_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_GSM_Network_Registration_Status( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GSM_Network_Registration_Status_t GSM_BC92_Argument_GSM_Network_Registration_Status )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Registration_Status=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_GSM_Network_Registration_Status );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_GSM_Network_Registration_Status_Execute( Instance, GSM_BC92_Argument_GSM_Network_Registration_Status ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_GPRS_Network_Registration_Status( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status_t GSM_BC92_Argument_GPRS_Network_Registration_Status )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Registration_Status=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_GPRS_Network_Registration_Status_Execute( Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PacketSwitched_t GSM_BC92_Argument_PacketSwitched )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PacketSwitched=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_PacketSwitched );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_PS_Attach_or_Detach_Execute( Instance, GSM_BC92_Argument_PacketSwitched ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Set_Default_PSD_Connection_Settings( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Set_Default_PSD_Connection_Settings_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Define_A_PDP_Context( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PDP_Context=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_PDP_Context );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Define_A_PDP_Context_Execute( Instance, GSM_BC92_Argument_PDP_Context ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_PDP_Context_Activation_or_Deactivation( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_State_t State )
{
}

GSM_BC92_Status_t GSM_BC92_Show_PDP_Addresses( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, PDP_Address=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_PDP_Context );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Show_PDP_Addresses_Execute( Instance, GSM_BC92_Argument_PDP_Context ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Socket_Open( GSM_BC92_Instance_t * Instance, GSM_BC92_Socket_Address_t * Address, GSM_BC92_Socket_Port_t Port )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Address=%s, Port=%d )", __FUNCTION__, Instance, Address, Port );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        // TODO Provide user input to the process
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Socket_Open ) ) != GSM_BC92_Status_Success )
        {
            break;
        }

        Instance->Context->Argument.Address = Address;
        Instance->Context->Argument.Port = Port;

        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Socket_Close( GSM_BC92_Instance_t * Instance )
{
}

GSM_BC92_Status_t GSM_BC92_Socket_Query( GSM_BC92_Instance_t * Instance, GSM_BC92_Socket_Status_t * GSM_BC92_Socket_Status )
{
}

GSM_BC92_Status_t GSM_BC92_Socket_Write( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t DataLength )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Data[%d]=%s )", __FUNCTION__, Instance, DataLength, Data );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Socket_Write_Execute( Instance, Data, DataLength ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Socket_Read( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t * DataLength )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Data[%d]=%p )", __FUNCTION__, Instance, *DataLength, Data );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Socket_Read_Execute( Instance, Data, DataLength ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Set_UE_Functionality( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_UE_Functionality_t GSM_BC92_Argument_Set_UE_Functionality )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, UE_Functionality_Level=%d, UE_Functionality_Reset=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Set_UE_Functionality.Level, GSM_BC92_Argument_Set_UE_Functionality.Reset );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Set_UE_Functionality_Execute( Instance, GSM_BC92_Argument_Set_UE_Functionality ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Reset_The_Module( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Reset_The_Module_t GSM_BC92_Argument_Reset_The_Module )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Reset=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Reset_The_Module );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Reset_The_Module_Execute( Instance, GSM_BC92_Argument_Reset_The_Module ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Power_Off_The_Module( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Power_Off_The_Module_t GSM_BC92_Argument_Power_Off_The_Module )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Mode=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Power_Off_The_Module );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Power_Off_The_Module_Execute( Instance, GSM_BC92_Argument_Power_Off_The_Module ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Return_Current_Date_and_Time( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Return_Current_Date_and_Time_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Query_Power_Supply_Voltage( GSM_BC92_Instance_t * Instance )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p )", __FUNCTION__, Instance );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Query_Power_Supply_Voltage_Execute( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Select_SMS_Message_Format( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Select_SMS_Message_Format_t GSM_BC92_Argument_Select_SMS_Message_Format )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Format=%d )", __FUNCTION__, Instance, GSM_BC92_Argument_Select_SMS_Message_Format );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Select_SMS_Message_Format_Execute( Instance, GSM_BC92_Argument_Select_SMS_Message_Format ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

GSM_BC92_Status_t GSM_BC92_Send_SMS_Message( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Send_SMS_Message_t GSM_BC92_Argument_Send_SMS_Message )
{
    GSM_BC92_Status_t Status = GSM_BC92_Status_Error;
    do
    {
        GSM_Trace( "%s( Instance=%p, Number=%s, Content=%s )", __FUNCTION__, Instance, GSM_BC92_Argument_Send_SMS_Message.Number, GSM_BC92_Argument_Send_SMS_Message.Content );
        if ( ( Status = GSM_BC92_IsReady( Instance ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Process_Set( Instance, GSM_BC92_ProcessType_Command ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        if ( ( Status = GSM_BC92_Send_SMS_Message_Execute( Instance, GSM_BC92_Argument_Send_SMS_Message ) ) != GSM_BC92_Status_Success )
        {
            break;
        }
        Status = GSM_BC92_Status_Success;
    }
    while ( 0 );
    return Status;
}

// #############################################################################
// #### Public Variable(s) #####################################################
// #############################################################################

// #############################################################################
// #### File Guard #############################################################
// #############################################################################

// #############################################################################
// #### END OF FILE ############################################################
// #############################################################################
