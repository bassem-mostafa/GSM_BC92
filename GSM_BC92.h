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

/**
 *  @file
 *
 *  @brief Platform GSM BC92 Driver
 */

// #############################################################################
// #### Control Include(s) #####################################################
// #############################################################################

// #############################################################################
// #### Control Macro(s) #######################################################
// #############################################################################

// #############################################################################
// #### File Guard #############################################################
// #############################################################################

/**
 *  @addtogroup Platform_GSM_Driver
 *
 *  @{
 */

/**
 *  @defgroup Platform_GSM_BC92 BC92
 *
 *  @{
 */

#ifndef GSM_BC92_H_
    #define GSM_BC92_H_

    #ifdef __cplusplus
extern "C"
{
    #endif /* __cplusplus */

    // #############################################################################
    // #### Include(s) #############################################################
    // #############################################################################

    #include <stdint.h>

    // #############################################################################
    // #### Public Macro(s) ########################################################
    // #############################################################################

    // #############################################################################
    // #### Public Type(s) #########################################################
    // #############################################################################

    /**
     *  @brief GSM BC92 Operation Status
     *
     *  @enum GSM_BC92_Status_t
     */
    typedef enum GSM_BC92_Status
    {
        GSM_BC92_Status_Success = 0,     ///< Success
        GSM_BC92_Status_ArgumentInvalid, ///< Invalid Argument
        GSM_BC92_Status_NotSupported,    ///< Not Supported
        GSM_BC92_Status_Error,           ///< Generic Error
        GSM_BC92_Status_Busy,            ///< Busy
        GSM_BC92_Status_Timeout,         ///< Timeout
    } GSM_BC92_Status_t;

    /**
     *  @brief GSM BC92 Data Length Type
     */
    typedef uint32_t GSM_BC92_DataLength_t;

    /**
     *  @brief GSM BC92 Data Type
     */
    typedef uint8_t GSM_BC92_Data_t;

    /**
     *  @brief GSM BC92 Voltage Type
     */
    typedef int32_t GSM_BC92_Voltage_t;

    /**
     *  @brief GSM BC92 RSSI Type
     */
    typedef int32_t GSM_BC92_RSSI_t;

    /**
     *  @brief GSM BC92 Timestamp Type
     */
    typedef struct GSM_BC92_Timestamp
    {
        uint32_t Year;   ///< Year
        uint32_t Month;  ///< Month
        uint32_t Day;    ///< Day
        uint32_t Hour;   ///< Hour
        uint32_t Minute; ///< Minute
        uint32_t Second; ///< Second
    } GSM_BC92_Timestamp_t;

    // TODO Add Doxygen Documentation

    typedef struct GSM_BC92_Instance GSM_BC92_Instance_t;

    typedef void ( *GSM_BC92_Callback_OnComplete_t )( GSM_BC92_Instance_t * Instance, GSM_BC92_Status_t Status );

    typedef void ( *GSM_BC92_Callback_OnTime_t )( GSM_BC92_Instance_t * Instance, GSM_BC92_Timestamp_t Timestamp );

    typedef void ( *GSM_BC92_Callback_OnRSSI_t )( GSM_BC92_Instance_t * Instance, GSM_BC92_RSSI_t RSSI );

    typedef void ( *GSM_BC92_Callback_OnVoltage_t )( GSM_BC92_Instance_t * Instance, GSM_BC92_Voltage_t Voltage );

    typedef void ( *GSM_BC92_Callback_OnSocketOpen_t )( GSM_BC92_Instance_t * Instance );

    typedef void ( *GSM_BC92_Callback_OnSocketClose_t )( GSM_BC92_Instance_t * Instance );

    typedef struct GSM_BC92_InstanceContext GSM_BC92_InstanceContext_t;

    typedef struct GSM_BC92_Instance
    {
        UART_t UARTx;

        GPIO_t PowerKey;
        GPIO_t Reset;
        GPIO_t Interrupt;
        GPIO_t Indication;

        GSM_BC92_Callback_OnComplete_t OnComplete;
        GSM_BC92_Callback_OnTime_t OnTime;
        GSM_BC92_Callback_OnVoltage_t OnVoltage;
        GSM_BC92_Callback_OnRSSI_t OnRSSI;
        GSM_BC92_Callback_OnSocketOpen_t OnSocketOpen;
        GSM_BC92_Callback_OnSocketClose_t OnSocketClose;

        // Managed Internally
        GSM_BC92_InstanceContext_t * Context;
    } GSM_BC92_Instance_t;

    typedef enum GSM_BC92_Argument_Set_Command_Echo_Mode
    {
        GSM_BC92_Argument_Set_Command_Echo_Mode_Off = 0,
        GSM_BC92_Argument_Set_Command_Echo_Mode_On
    } GSM_BC92_Argument_Set_Command_Echo_Mode_t;

    typedef enum GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile
    {
        GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile_0 = 0,
    } GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile_t;

    typedef enum GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults
    {
        GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults_0 = 0,
    } GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults_t;

    typedef enum GSM_BC92_Argument_Request_Product_Serial_Number
    {
        GSM_BC92_Argument_Request_Product_Serial_Number_UUID = 0,
        GSM_BC92_Argument_Request_Product_Serial_Number_IMEI,
    } GSM_BC92_Argument_Request_Product_Serial_Number_t;

    typedef enum GSM_BC92_Argument_Report_Mobile_Termination_Error
    {
        GSM_BC92_Argument_Report_Mobile_Termination_Error_Disable = 0,
        GSM_BC92_Argument_Report_Mobile_Termination_Error_Numeric,
        GSM_BC92_Argument_Report_Mobile_Termination_Error_Verbose,
    } GSM_BC92_Argument_Report_Mobile_Termination_Error_t;

    typedef enum GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate
    {
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_2400 = 2400,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_4800 = 4800,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_9600 = 9600,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_14400 = 14400,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_19200 = 19200,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_28800 = 28800,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_33600 = 33600,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_38400 = 38400,
        GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_57600 = 57600,
    } GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_t;

    typedef struct GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing
    {
        enum GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format
        {
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format_8_Data_0_Parity_2_Stop = 1,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format_8_Data_1_Parity_1_Stop,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format_8_Data_0_Parity_1_Stop,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format_7_Data_0_Parity_2_Stop,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format_7_Data_1_Parity_1_Stop,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Format_7_Data_0_Parity_1_Stop,
        } Format;

        enum GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Parity
        {
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Parity_Odd = 0,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Parity_Even,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Parity_Mark,
            GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_Parity_Space,
        } Parity;
    } GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_t;

    typedef struct GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control
    {
        enum GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TE
        {
            GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TE_None = 0,
            GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TE_XON_XOFF_No_Stack,
            GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TE_RTS,
            GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TE_XON_XOFF,
        } TE;

        enum GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TA
        {
            GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TA_None = 0,
            GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TA_XON_XOFF,
            GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_TA_CTS,
        } TA;
    } GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_t;

    typedef char * GSM_BC92_Argument_Enter_PIN_t;

    typedef enum GSM_BC92_Argument_GSM_Network_Registration_Status
    {
        GSM_BC92_Argument_GSM_Network_Registration_Status_Disable = 0,
        GSM_BC92_Argument_GSM_Network_Registration_Status_Enable,
        GSM_BC92_Argument_GSM_Network_Registration_Status_Enable_With_Location,
    } GSM_BC92_Argument_GSM_Network_Registration_Status_t;

    typedef enum GSM_BC92_Argument_GPRS_Network_Registration_Status
    {
        GSM_BC92_Argument_GPRS_Network_Registration_Status_Disable = 0,
        GSM_BC92_Argument_GPRS_Network_Registration_Status_Enable,
        GSM_BC92_Argument_GPRS_Network_Registration_Status_Enable_With_Location,
    } GSM_BC92_Argument_GPRS_Network_Registration_Status_t;

    typedef enum GSM_BC92_Argument_PacketSwitched
    {
        GSM_BC92_Argument_PacketSwitched_Detach = 0,
        GSM_BC92_Argument_PacketSwitched_Attach,
    } GSM_BC92_Argument_PacketSwitched_t;

    typedef enum GSM_BC92_Argument_PDP_Context
    {
        GSM_BC92_Argument_PDP_Context_1 = 1,
        GSM_BC92_Argument_PDP_Context_2,
        GSM_BC92_Argument_PDP_Context_3,
        GSM_BC92_Argument_PDP_Context_4,
        GSM_BC92_Argument_PDP_Context_5,
        GSM_BC92_Argument_PDP_Context_6,
        GSM_BC92_Argument_PDP_Context_7,
    } GSM_BC92_Argument_PDP_Context_t;

    typedef enum GSM_BC92_Argument_PDP_Context_State
    {
        GSM_BC92_Argument_PDP_Context_State_Deactivate = 0,
        GSM_BC92_Argument_PDP_Context_State_Activate,
    } GSM_BC92_Argument_PDP_Context_State_t;

    typedef struct GSM_BC92_Argument_Set_UE_Functionality
    {
        enum GSM_BC92_Argument_Set_UE_Functionality_Level
        {
            GSM_BC92_Argument_Set_UE_Functionality_Level_Minimal = 0,
            GSM_BC92_Argument_Set_UE_Functionality_Level_Full,
        } Level;

        enum GSM_BC92_Argument_Set_UE_Functionality_Reset
        {
            GSM_BC92_Argument_Set_UE_Functionality_Reset_Disable = 0,
            GSM_BC92_Argument_Set_UE_Functionality_Reset_Enable,
        } Reset;
    } GSM_BC92_Argument_Set_UE_Functionality_t;

    typedef enum GSM_BC92_Argument_Reset_The_Module
    {
        GSM_BC92_Argument_Reset_The_Module_Immediate = 1,
    } GSM_BC92_Argument_Reset_The_Module_t;

    typedef enum GSM_BC92_Argument_Power_Off_The_Module
    {
        GSM_BC92_Argument_Power_Off_The_Module_Urgent = 0,
        GSM_BC92_Argument_Power_Off_The_Module_Normal,
        GSM_BC92_Argument_Power_Off_The_Module_Reset,
    } GSM_BC92_Argument_Power_Off_The_Module_t;

    typedef enum GSM_BC92_Argument_Select_SMS_Message_Format
    {
        GSM_BC92_Argument_Select_SMS_Message_Format_PDU = 0,
        GSM_BC92_Argument_Select_SMS_Message_Format_Text,
    } GSM_BC92_Argument_Select_SMS_Message_Format_t;

    typedef struct GSM_BC92_Argument_Send_SMS_Message
    {
        char * Number;
        char * Content;
    } GSM_BC92_Argument_Send_SMS_Message_t;

    typedef enum GSM_BC92_Socket_Status
    {
        GSM_BC92_Socket_Status_Closed = 0,
        GSM_BC92_Socket_Status_Opened,
    } GSM_BC92_Socket_Status_t;

    typedef uint8_t GSM_BC92_Socket_Address_t;

    typedef uint32_t GSM_BC92_Socket_Port_t;

    // #############################################################################
    // #### Public Method(s) #######################################################
    // #############################################################################

    GSM_BC92_Status_t GSM_BC92_Initialize( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Cycle( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_DeInitialize( GSM_BC92_Instance_t * Instance );

    // Basic Operations
    GSM_BC92_Status_t GSM_BC92_Power_Off( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Power_On( GSM_BC92_Instance_t * Instance );

    // General Commands
    GSM_BC92_Status_t GSM_BC92_Ping( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Display_Product_Identification_Information( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Set_Command_Echo_Mode( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_Command_Echo_Mode_t GSM_BC92_Argument_Set_Command_Echo_Mode );
    GSM_BC92_Status_t GSM_BC92_Store_Current_Parameters_To_User_Defined_Profile( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile_t GSM_BC92_Argument_Store_Current_Parameters_To_User_Defined_Profile );
    GSM_BC92_Status_t GSM_BC92_Set_All_Parameters_To_Manufacturer_Defaults( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults_t GSM_BC92_Argument_Set_All_Parameters_To_Manufacturer_Profile_Defaults );
    GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Identification( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Request_Model_Identification( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Request_Manufacturer_Revision( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Request_Product_Serial_Number( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Request_Product_Serial_Number_t GSM_BC92_Argument_Request_Product_Serial_Number );
    GSM_BC92_Status_t GSM_BC92_Report_Mobile_Termination_Error( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Report_Mobile_Termination_Error_t GSM_BC92_Argument_Report_Mobile_Termination_Error );
    GSM_BC92_Status_t GSM_BC92_Extended_Error_Report( GSM_BC92_Instance_t * Instance );

    // Serial Interface Control Commands
    GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Fixed_Local_Rate( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate_t GSM_BC92_Argument_Set_TE_TA_Fixed_Local_Rate );
    GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Control_Character_Framing( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing_t GSM_BC92_Argument_Set_TE_TA_Control_Character_Framing );
    GSM_BC92_Status_t GSM_BC92_Set_TE_TA_Local_Data_Flow_Control( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control_t GSM_BC92_Argument_Set_TE_TA_Local_Data_Flow_Control );

    // (U)SIM Card Related Commands
    GSM_BC92_Status_t GSM_BC92_Enter_PIN( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Enter_PIN_t GSM_BC92_Argument_Enter_PIN );
    GSM_BC92_Status_t GSM_BC92_Request_International_Mobile_Subscriber_Identity( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_USIM_Card_Identification( GSM_BC92_Instance_t * Instance );

    // Network service commands
    GSM_BC92_Status_t GSM_BC92_Signal_Quality_Report( GSM_BC92_Instance_t * Instance );
    // GSM_BC92_Status_t GSM_BC92_Get_Cell_Information( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Extended_Signal_Quality( GSM_BC92_Instance_t *Instance );
    GSM_BC92_Status_t GSM_BC92_GSM_Network_Registration_Status( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GSM_Network_Registration_Status_t GSM_BC92_Argument_GSM_Network_Registration_Status );
    GSM_BC92_Status_t GSM_BC92_GPRS_Network_Registration_Status( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_GPRS_Network_Registration_Status_t GSM_BC92_Argument_GPRS_Network_Registration_Status );
    // GSM_BC92_Status_t GSM_BC92_EPS_Network_Registration_Status( GSM_BC92_Instance_t *Instance );
    GSM_BC92_Status_t GSM_BC92_PS_Attach_or_Detach( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PacketSwitched_t GSM_BC92_Argument_PacketSwitched );
    GSM_BC92_Status_t GSM_BC92_Set_Default_PSD_Connection_Settings( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Define_A_PDP_Context( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context );
    GSM_BC92_Status_t GSM_BC92_PDP_Context_Activation_or_Deactivation( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_State_t State );
    GSM_BC92_Status_t GSM_BC92_Show_PDP_Addresses( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_PDP_Context_t GSM_BC92_Argument_PDP_Context );
    // GSM_BC92_Status_t GSM_BC92_Define_A_TCP_IP_Context( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Signaling_Connection_Status( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Operator_Selection( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Power_Saving_Mode_Setting( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_eDRX_Setting( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_eDRX_Read_Dynamic_Parameters( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Get_and_Set_Mobile_Operation_Band( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Lock_NB_IoT_Frequency( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Engineering_Mode( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Enable_Disable_NB_IoT_Related_Event_Report( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Clear_Stored_NB_IoT_GSM_EARFCN_List( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Extended_Configuration_Setting( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_CloT_Optimization_Configuration( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Time_Zone_Reporting( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Query_APN_Rate_Control( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Set_Scrambling_Algorithm( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Configure_Networking_Mode( GSM_BC92_Instance_t *Instance );

    GSM_BC92_Status_t GSM_BC92_Socket_Open( GSM_BC92_Instance_t * Instance, GSM_BC92_Socket_Address_t * Address, GSM_BC92_Socket_Port_t Port );
    GSM_BC92_Status_t GSM_BC92_Socket_Close( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Socket_Query( GSM_BC92_Instance_t * Instance, GSM_BC92_Socket_Status_t * Status );
    GSM_BC92_Status_t GSM_BC92_Socket_Write( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t DataLength );
    GSM_BC92_Status_t GSM_BC92_Socket_Read( GSM_BC92_Instance_t * Instance, GSM_BC92_Data_t * Data, GSM_BC92_DataLength_t * DataLength );
    // TODO Add More Socket APIs

    // Hardware Related Commands
    GSM_BC92_Status_t GSM_BC92_Set_UE_Functionality( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Set_UE_Functionality_t GSM_BC92_Argument_Set_UE_Functionality );
    // GSM_BC92_Status_t GSM_BC92_Configure_Sleep_Mode( GSM_BC92_Instance_t *Instance );
    GSM_BC92_Status_t GSM_BC92_Reset_The_Module( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Reset_The_Module_t GSM_BC92_Argument_Reset_The_Module );
    GSM_BC92_Status_t GSM_BC92_Power_Off_The_Module( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Power_Off_The_Module_t GSM_BC92_Argument_Power_Off_The_Module );
    GSM_BC92_Status_t GSM_BC92_Return_Current_Date_and_Time( GSM_BC92_Instance_t * Instance );
    GSM_BC92_Status_t GSM_BC92_Query_Power_Supply_Voltage( GSM_BC92_Instance_t * Instance );
    // GSM_BC92_Status_t GSM_BC92_Enable_Disable_Deep_Sleep_Wakeup_Indication( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Configure_Network_Status_Indication_Light( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Read_ADC( GSM_BC92_Instance_t *Instance );

    // SMS Related Commands
    GSM_BC92_Status_t GSM_BC92_Select_SMS_Message_Format( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Select_SMS_Message_Format_t GSM_BC92_Argument_Select_SMS_Message_Format );
    // GSM_BC92_Status_t GSM_BC92_Delete_SMS_Message( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_List_SMS_Messages_From_Preferred_Store( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Read_SMS_Message( GSM_BC92_Instance_t *Instance );
    GSM_BC92_Status_t GSM_BC92_Send_SMS_Message( GSM_BC92_Instance_t * Instance, GSM_BC92_Argument_Send_SMS_Message_t GSM_BC92_Argument_Send_SMS_Message );
    // GSM_BC92_Status_t GSM_BC92_Write_SMS_Message_To_Memory( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Send_SMS_Message_From_Storage( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_Send_SMS_Command( GSM_BC92_Instance_t *Instance );
    // GSM_BC92_Status_t GSM_BC92_New_SMS_Message_Indications( GSM_BC92_Instance_t *Instance );

    // #############################################################################
    // #### Public Variable(s) #####################################################
    // #############################################################################

    // #############################################################################
    // #### File Guard #############################################################
    // #############################################################################

    #ifdef __cplusplus
} /* extern "C" */
    #endif /* __cplusplus */

#endif /* GSM_BC92_H_ */

/**
 *  @}
 *
 *  @}
 */

// #############################################################################
// #### END OF FILE ############################################################
// #############################################################################
