/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Static utility class for transfering network enums to
*                 descriptor.
*
*/


#include "GSNetworkDebugHelper.h"
#include "GSNetworkPluginModel.h"


// ---------------------------------------------------------------------------
// GSNetworkDebugHelper::NwToDes
//
// ---------------------------------------------------------------------------
void GSNetworkDebugHelper::NwToDes( TGSNetworkModeItems aMode, TDes& aDes )
    {
    switch( aMode )
        {
        case EGSNetworkModeDualmode:
            _LIT( KGSDesModeDual, "EGSNetworkModeDualmode (%d)");
            aDes.Format( KGSDesModeDual, aMode );
            break;
        case EGSNetworkModeUMTS:
            _LIT( KGSDesModeUMTS, "EGSNetworkModeUMTS (%d)");
            aDes.Format( KGSDesModeUMTS, aMode );
            break;
        case EGSNetworkModeGSM:
            _LIT( KGSDesModeGSM, "EGSNetworkModeGSM (%d)");
            aDes.Format( KGSDesModeGSM, aMode );
            break;
        default:
            _LIT( KGSDesCapsUnknown, "Unknown mode (%d)!!!");
            aDes.Format( KGSDesCapsUnknown, aMode );
            break;
        }
    }


// ---------------------------------------------------------------------------
// GSNetworkDebugHelper::NwCapsToDes
//
// ---------------------------------------------------------------------------
void GSNetworkDebugHelper::NwCapsToDes( TUint32 aMode, TDes& aDes )
    {
    switch( aMode )
        {
        case RMmCustomAPI::KCapsNetworkModeGsm:
            _LIT( KGSDesCapsModeDual, "RMmCustomAPI::KCapsNetworkModeGsm (%d)");
            aDes.Format( KGSDesCapsModeDual, aMode );
            break;
        case RMmCustomAPI::KCapsNetworkModeUmts:
            _LIT( KGSDesCapsModeUMTS, "RMmCustomAPI::KCapsNetworkModeUmts (%d)");
            aDes.Format( KGSDesCapsModeUMTS, aMode );
            break;
        case RMmCustomAPI::KCapsNetworkModeDual:
            _LIT( KGSDesCapsModeGSM, "RMmCustomAPI::KCapsNetworkModeDual (%d)");
            aDes.Format( KGSDesCapsModeGSM, aMode );
            break;
        case KGSNetworkModeCapsNotUpdated:
            _LIT( KGSDesCapsModeNotUpdated, "KGSNetworkModeCapsNotUpdated (%d)");
            aDes.Format( KGSDesCapsModeNotUpdated, aMode );
            break;
        default:
            _LIT( KGSDesCapsUnknown, "Unknown mode (%d)!!!");
            aDes.Format( KGSDesCapsUnknown, aMode );
            break;
        }
    }


// ---------------------------------------------------------------------------
// GSNetworkDebugHelper::NetworkStatusToDes
//
// ---------------------------------------------------------------------------
void GSNetworkDebugHelper::NetworkStatusToDes(
        TNWRegistrationStatus aNetworkStatus,
        TDes& aDes )
    {
    switch( aNetworkStatus )
        {
        case ENWRegistrationUnknown:
            _LIT( KGSDes1, "ENWRegistrationUnknown (%d)");
            aDes.Format( KGSDes1, aNetworkStatus );
            break;
        case ENWNotRegisteredNoService:
            _LIT( KGSDes2, "ENWNotRegisteredNoService (%d)");
            aDes.Format( KGSDes2, aNetworkStatus );
            break;
        case ENWNotRegisteredEmergencyOnly:
            _LIT( KGSDes3, "ENWNotRegisteredEmergencyOnly (%d)");
            aDes.Format( KGSDes3, aNetworkStatus );
            break;
        case ENWNotRegisteredSearching:
            _LIT( KGSDes4, "ENWNotRegisteredSearching (%d)");
            aDes.Format( KGSDes4, aNetworkStatus );
            break;
        case ENWRegisteredBusy:
            _LIT( KGSDes5, "ENWRegisteredBusy (%d)");
            aDes.Format( KGSDes5, aNetworkStatus );
            break;
        case ENWRegisteredOnHomeNetwork:
            _LIT( KGSDes6, "ENWRegisteredOnHomeNetwork (%d)");
            aDes.Format( KGSDes6, aNetworkStatus );
            break;
        case ENWRegistrationDenied:
            _LIT( KGSDes7, "ENWRegistrationDenied (%d)");
            aDes.Format( KGSDes7, aNetworkStatus );
            break;
        case ENWRegisteredRoaming:
            _LIT( KGSDes8, "ENWRegisteredRoaming (%d)");
            aDes.Format( KGSDes8, aNetworkStatus );
            break;

        default:
            _LIT( KGSDes, "Unknown status (%d)!!!");
            aDes.Format( KGSDes, aNetworkStatus );
            break;
        }
    }


// ---------------------------------------------------------------------------
// GSNetworkDebugHelper::NetworkMessageToDes
//
// ---------------------------------------------------------------------------
void GSNetworkDebugHelper::NetworkMessageToDes(
        MNWMessageObserver::TNWMessages aNetworkMessage,
        TDes& aDes )
    {
    switch( aNetworkMessage )
        {
        case MNWMessageObserver::ENWMessageNetworkInfoChange:
            _LIT( KGSDes1, "ENWMessageNetworkInfoChange (%d)");
            aDes.Format( KGSDes1, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageNetworkModeChange:
            _LIT( KGSDes2, "ENWMessageNetworkModeChange (%d)");
            aDes.Format( KGSDes2, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange:
            _LIT( KGSDes3, "ENWMessageNetworkRegistrationStatusChange (%d)");
            aDes.Format( KGSDes3, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageProgrammableOperatorInfoChange:
            _LIT( KGSDes4, "ENWMessageProgrammableOperatorInfoChange (%d)");
            aDes.Format( KGSDes4, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageNetworkProviderNameChange:
            _LIT( KGSDes5, "ENWMessageNetworkProviderNameChange (%d)");
            aDes.Format( KGSDes5, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageServiceProviderNameChange:
            _LIT( KGSDes6, "ENWMessageServiceProviderNameChange (%d)");
            aDes.Format( KGSDes6, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageProgrammableOperatorInfoUpdating:
            _LIT( KGSDes7, "ENWMessageProgrammableOperatorInfoUpdating (%d)");
            aDes.Format( KGSDes7, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageNetworkProviderNameUpdating:
            _LIT( KGSDes8, "ENWMessageNetworkProviderNameUpdating (%d)");
            aDes.Format( KGSDes8, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageServiceProviderNameUpdating:
            _LIT( KGSDes9, "ENWMessageServiceProviderNameUpdating (%d)");
            aDes.Format( KGSDes9, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageAllowRefresh:
            _LIT( KGSDes10, "ENWMessageAllowRefresh (%d)");
            aDes.Format( KGSDes10, aNetworkMessage );
            break;
#ifdef RD_PHONE_NG
        case MNWMessageObserver::ENWMessageNetworkSelectionSettingChange:
            _LIT( KGSDes11, "ENWMessageNetworkSelectionSettingChange (%d)");
            aDes.Format( KGSDes11, aNetworkMessage );
            break;
#endif // RD_PHONE_NG
        case MNWMessageObserver::ENWMessageNetworkCellReselection:
            _LIT( KGSDes12, "ENWMessageNetworkCellReselection (%d)");
            aDes.Format( KGSDes12, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageNetworkConnectionFailure:
            _LIT( KGSDes13, "ENWMessageNetworkConnectionFailure (%d)");
            aDes.Format( KGSDes13, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageCurrentCellInfoMessage:
            _LIT( KGSDes14, "ENWMessageCurrentCellInfoMessage (%d)");
            aDes.Format( KGSDes14, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageCurrentHomeZoneMessage:
            _LIT( KGSDes15, "ENWMessageCurrentHomeZoneMessage (%d)");
            aDes.Format( KGSDes15, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageNetworkIndicatorChange:
            _LIT( KGSDes16, "ENWMessageNetworkIndicatorChange (%d)");
            aDes.Format( KGSDes16, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageNetworkEmergencyMode:
            _LIT( KGSDes17, "ENWMessageNetworkEmergencyMode (%d)");
            aDes.Format( KGSDes17, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageStartSystemSearchRequestCompleteOk:
            _LIT( KGSDes18, "ENWMessageStartSystemSearchRequestCompleteOk (%d)");
            aDes.Format( KGSDes18, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageStartSystemSearchRequestCompleteFail:
            _LIT( KGSDes19, "ENWMessageStartSystemSearchRequestCompleteFail (%d)");
            aDes.Format( KGSDes19, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageStopProtocolStackRequestCompleteOk:
            _LIT( KGSDes20, "ENWMessageStopProtocolStackRequestCompleteOk (%d)");
            aDes.Format( KGSDes20, aNetworkMessage );
            break;
        case MNWMessageObserver::ENWMessageStopProtocolStackRequestCompleteFail:
            _LIT( KGSDes21, "ENWMessageStopProtocolStackRequestCompleteFail (%d)");
            aDes.Format( KGSDes21, aNetworkMessage );
            break;
        default:
            _LIT( KGSDes, "Unknown message (%d)!!!");
            aDes.Format( KGSDes, aNetworkMessage );
            break;
        }
    }


// ---------------------------------------------------------------------------
// GSNetworkDebugHelper::NetworkOperationToDes
//
// ---------------------------------------------------------------------------
void GSNetworkDebugHelper::NetworkOperationToDes(
        MNWMessageObserver::TNWOperation aNetworkOperation,
        TDes& aDes )
    {
    switch( aNetworkOperation )
        {
        case MNWMessageObserver::ENWNone:
            _LIT( KGSDes1, "ENWNone (%d)");
            aDes.Format( KGSDes1, aNetworkOperation );
            break;

        case MNWMessageObserver::ENWGetServiceProviderName:
            _LIT( KGSDes2, "ENWGetServiceProviderName (%d)");
            aDes.Format( KGSDes2, aNetworkOperation );
            break;
        case MNWMessageObserver::ENWGetNetworkProviderName:
            _LIT( KGSDes3, "ENWGetNetworkProviderName (%d)");
            aDes.Format( KGSDes3, aNetworkOperation );
            break;
        case MNWMessageObserver::ENWGetProgrammableOperatorName:
            _LIT( KGSDes4, "ENWGetProgrammableOperatorName (%d)");
            aDes.Format( KGSDes4, aNetworkOperation );
            break;
        default:
            _LIT( KGSDes, "Unknown operation (%d)!!!");
            aDes.Format( KGSDes, aNetworkOperation );
            break;
        }
    }


// ---------------------------------------------------------------------------
// GSNetworkDebugHelper::NetworkModeRequestToDes
//
// ---------------------------------------------------------------------------
void GSNetworkDebugHelper::NetworkModeRequestToDes(
        MPsetNetworkModeObserver::TServiceRequest aRequest,
        TDes& aDes )
    {
    switch( aRequest )
            {
            case MPsetNetworkModeObserver::EServiceRequestNone:
                _LIT( KGSDes1, "EServiceRequestNone (%d)");
                aDes.Format( KGSDes1, aRequest );
                break;

            case MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode:
                _LIT( KGSDes2, "EServiceRequestSetSelectedNetworkMode (%d)");
                aDes.Format( KGSDes2, aRequest );
                break;
            case MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode:
                _LIT( KGSDes3, "EServiceRequestGetCurrentNetworkMode (%d)");
                aDes.Format( KGSDes3, aRequest );
                break;
            default:
                _LIT( KGSDes, "Unknown request (%d)!!!");
                aDes.Format( KGSDes, aRequest );
                break;
            }
    }


// ---------------------------------------------------------------------------
// GSNetworkDebugHelper::NetworkInfoRequestToDes
//
// ---------------------------------------------------------------------------
void GSNetworkDebugHelper::NetworkInfoRequestToDes(
        MPsetNetworkInfoObserver::TServiceRequest aRequest,
        TDes& aDes )
    {
    switch( aRequest )
            {
            case MPsetNetworkInfoObserver::EServiceRequestNone:
                _LIT( KGSDes1, "EServiceRequestNone (%d)");
                aDes.Format( KGSDes1, aRequest );
                break;
            case MPsetNetworkInfoObserver::EServiceRequestEnumerateNetworks:
                _LIT( KGSDes2, "EServiceRequestEnumerateNetworks (%d)");
                aDes.Format( KGSDes2, aRequest );
                break;
            case MPsetNetworkInfoObserver::EServiceRequestGetNetworkInfo:
                _LIT( KGSDes3, "EServiceRequestGetNetworkInfo (%d)");
                aDes.Format( KGSDes3, aRequest );
                break;
            case MPsetNetworkInfoObserver::EServiceRequestGetCurrentNetworkInfo:
                _LIT( KGSDes4, "EServiceRequestGetCurrentNetworkInfo (%d)");
                aDes.Format( KGSDes4, aRequest );
                break;
            case MPsetNetworkInfoObserver::EServiceRequestGetNetworkSearchMode:
                _LIT( KGSDes5, "EServiceRequestGetNetworkSearchMode (%d)");
                aDes.Format( KGSDes5, aRequest );
                break;
            case MPsetNetworkInfoObserver::EServiceRequestSetNetwork:
                _LIT( KGSDes6, "EServiceRequestSetNetwork (%d)");
                aDes.Format( KGSDes6, aRequest );
                break;
            case MPsetNetworkInfoObserver::EServiceRequestSetNetworkMode:
                _LIT( KGSDes7, "EServiceRequestSetNetworkMode (%d)");
                aDes.Format( KGSDes7, aRequest );
                break;
            case MPsetNetworkInfoObserver::EServiceRequestSetNetworkAutomatic:
                _LIT( KGSDes8, "EServiceRequestSetNetworkAutomatic (%d)");
                aDes.Format( KGSDes8, aRequest );
                break;
            default:
                _LIT( KGSDes, "Unknown request (%d)!!!");
                aDes.Format( KGSDes, aRequest );
                break;
            }
    }



//End of File
