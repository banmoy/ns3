/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef EPC_X2_SAP_H
#define EPC_X2_SAP_H

#include "ns3/packet.h"
#include "ns3/eps-bearer.h"
#include "ns3/ipv4-address.h"

#include <ns3/lte-rrc-sap.h>

#include <bitset>

namespace ns3 {


class Node;

/**
 * The X2 SAP defines the service between the X2 entity and the RRC entity.
 * 
 * The X2 SAP follows the specification 3GPP TS 36.423: "X2 application protocol (X2AP)"
 * 
 * The service primitives corresponds to the X2AP procedures and messages and 
 * the service parameters corresponds to the Information Elements
 * 
 * Note: Any reference in this file refers to the 3GPP TS 36.423 specification
 */

/**
 * \brief Common structures for EpcX2SapProvider and EpcX2SapUser
 */
class EpcX2Sap
{
public:
  virtual ~EpcX2Sap ();
  
  /**
   * E-RABs to be setup item as
   * it is used in the HANDOVER REQUEST message.
   * See section 9.1.1.1 for further info about the parameters
   */
  struct ErabToBeSetupItem
  {
    uint16_t    erabId;
    EpsBearer   erabLevelQosParameters;
    bool        dlForwarding;
    Ipv4Address transportLayerAddress;
    uint32_t    gtpTeid;

    ErabToBeSetupItem ();
  };

  /**
   * E-RABs admitted item as
   * it is used in the HANDOVER REQUEST ACKNOWLEDGE message.
   * See section 9.1.1.2 for further info about the parameters
   */
  struct ErabAdmittedItem
  {
    uint16_t    erabId;
    uint32_t    ulGtpTeid;
    uint32_t    dlGtpTeid;
  };

  /**
   * E-RABs not admitted item as
   * it is used in the HANDOVER REQUEST ACKNOWLEDGE message.
   * See section 9.1.1.2 for further info about the parameters
   */
  struct ErabNotAdmittedItem
  {
    uint16_t    erabId;
    uint16_t    cause;
  };

  /**
   * E-RABs subject to status transfer item as
   * it is used in the SN STATUS TRANSFER message.
   * See section 9.1.1.4 for further info about the parameters
   */
  static const uint16_t m_maxPdcpSn = 4096;
  struct ErabsSubjectToStatusTransferItem
  {
    uint16_t            erabId;
    std::bitset<m_maxPdcpSn> receiveStatusOfUlPdcpSdus;
    uint16_t            ulPdcpSn;
    uint32_t            ulHfn;
    uint16_t            dlPdcpSn;
    uint32_t            dlHfn;
  };

  /**
   * UL Interference OverloadIndication as
   * it is used in the LOAD INFORMATION message.
   * See section 9.2.17 for further info about the values
   */
  enum UlInterferenceOverloadIndicationItem
  {
    HighInterference,
    MediumInterference,
    LowInterference
  };

  /**
   * UL High Interference Information as
   * it is used in the LOAD INFORMATION message.
   * See section 9.1.2.1 for further info about the parameters
   */
  struct UlHighInterferenceInformationItem
  {
    uint16_t    targetCellId;
    std::vector <bool> ulHighInterferenceIndicationList;
  };

  /**
   * Relative Narrowband Tx Power (RNTP) as
   * it is used in the LOAD INFORMATION message.
   * See section 9.2.19 for further info about the parameters
   * 
   * Note: You can use INT16_MIN value for -infinite value
   *       in the rntpThreshold field
   */
  struct RelativeNarrowbandTxBand
  {
    std::vector <bool> rntpPerPrbList;
    int16_t     rntpThreshold;
    uint16_t    antennaPorts;
    uint16_t    pB;
    uint16_t    pdcchInterferenceImpact;
  };

  /**
   * Cell Information Item as
   * it is used in the LOAD INFORMATION message.
   * See section 9.1.2.1 for further info about the parameters
   */
  struct CellInformationItem
  {
    uint16_t    sourceCellId;
    std::vector <UlInterferenceOverloadIndicationItem> ulInterferenceOverloadIndicationList;
    std::vector <UlHighInterferenceInformationItem> ulHighInterferenceInformationList;
    RelativeNarrowbandTxBand relativeNarrowbandTxBand;
  };

  /**
   * Load Indicator as
   * it is used in the RESOURCE STATUS UPDATE message.
   * See section 9.2.36 for further info about the value
   */
  enum LoadIndicator
  {
    LowLoad,
    MediumLoad,
    HighLoad,
    Overload
  };

  /**
   * Composite Available Capacity as
   * it is used in the RESOURCE STATUS UPDATE message.
   * See section 9.2.45 for further info about the parameters
   */
  struct CompositeAvailCapacity
  {
    uint16_t    cellCapacityClassValue;
    uint16_t    capacityValue;
  };

  /**
   * Cell Measurement Result Item as
   * it is used in the RESOURCE STATUS UPDATE message.
   * See section 9.1.2.14 for further info about the parameters
   */
  struct CellMeasurementResultItem
  {
    uint16_t        sourceCellId;

    LoadIndicator   dlHardwareLoadIndicator;
    LoadIndicator   ulHardwareLoadIndicator;

    LoadIndicator   dlS1TnlLoadIndicator;
    LoadIndicator   ulS1TnlLoadIndicator;

    uint16_t        dlGbrPrbUsage;
    uint16_t        ulGbrPrbUsage;
    uint16_t        dlNonGbrPrbUsage;
    uint16_t        ulNonGbrPrbUsage;
    uint16_t        dlTotalPrbUsage;
    uint16_t        ulTotalPrbUsage;

    CompositeAvailCapacity  dlCompositeAvailableCapacity;
    CompositeAvailCapacity  ulCompositeAvailableCapacity;
  };


  enum IdCause
  {
    HandoverDesirableForRadioReason,
    TimeCriticalHandover
  };

  struct ConnectionRequestParams
  {
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    uint16_t            rnti;
    uint64_t            imsi;
  };

  struct SmallConnCompletedParams
  {
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    uint16_t            rnti;
  };

  struct RrConfigParams
  {
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    uint16_t            rnti;
    LteRrcSap::RadioResourceConfigDedicated rrcd;
  };

  /**
   * \brief Parameters of the HANDOVER REQUEST message.
   *
   * See section 9.1.1.1 for further info about the parameters
   */
  struct HandoverRequestParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            cause;
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    uint32_t            mmeUeS1apId;
    uint64_t            ueAggregateMaxBitRateDownlink;
    uint64_t            ueAggregateMaxBitRateUplink;
    std::vector <ErabToBeSetupItem> bearers;
    Ptr<Packet>         rrcContext;
  };

  /**
   * \brief Parameters of the HANDOVER REQUEST ACKNOWLEDGE message.
   *
   * See section 9.1.1.2 for further info about the parameters
   */
  struct HandoverRequestAckParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            newEnbUeX2apId;
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    std::vector <ErabAdmittedItem> admittedBearers;
    std::vector <ErabNotAdmittedItem> notAdmittedBearers;
    Ptr<Packet>         rrcContext;
  };

  /**
   * \brief Parameters of the HANDOVER PREPARATION FAILURE message.
   *
   * See section 9.1.1.3 for further info about the parameters
   */
  struct HandoverPreparationFailureParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    uint16_t            cause;
    uint16_t            criticalityDiagnostics;
  };

  /**
   * \brief Parameters of the SN STATUS TRANSFER message.
   *
   * See section 9.1.1.4 for further info about the parameters
   */
  struct SnStatusTransferParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            newEnbUeX2apId;
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    std::vector <ErabsSubjectToStatusTransferItem> erabsSubjectToStatusTransferList;
  };

  /**
   * \brief Parameters of the UE CONTEXT RELEASE message.
   *
   * See section 9.1.1.5 for further info about the parameters
   */
  struct UeContextReleaseParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            newEnbUeX2apId;
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
  };

  /**
   * \brief Parameters of the LOAD INFORMATION message.
   *
   * See section 9.1.2.1 for further info about the parameters
   */
  struct LoadInformationParams
  {
    uint16_t            targetCellId;
    std::vector <CellInformationItem> cellInformationList;
  };

  /**
   * \brief Parameters of the RESOURCE STATUS UPDATE message.
   *
   * See section 9.1.2.14 for further info about the parameters
   */
  struct ResourceStatusUpdateParams
  {
    uint16_t            targetCellId;
    uint16_t            enb1MeasurementId;
    uint16_t            enb2MeasurementId;
    std::vector <CellMeasurementResultItem> cellMeasurementResultList;
  };

  /**
   * \brief Parameters of the UE DATA primitive
   *
   * Forward UE data during the handover procedure from source eNB (sourceCellId)
   * to target eNB (targetCellId) using a GTP-U tunnel (gtpTeid)
   */
  struct UeDataParams
  {
    uint16_t    sourceCellId;
    uint16_t    targetCellId;
    uint32_t    gtpTeid;
    Ptr<Packet> ueData;
  };

  struct OnOffRequestParams
  {
    uint16_t    sourceCellId;
    uint16_t    targetCellId;
    bool        op;
  };

  struct DlCqiParams
  {
    uint16_t sourceCellId;
    uint16_t targetCellId;
    std::vector<uint16_t> rntis;
    std::vector<uint8_t> cqis;
  };

  struct HandoverTriggerParams
  {
    uint16_t sourceCellId;
    uint16_t targetCellId;

    uint16_t hoTargetCellId;
    uint16_t rnti;
  };

  struct SwitchStateParams
  {
    uint16_t sourceCellId;
    uint16_t targetCellId;

    uint16_t rnti;
    uint8_t state;
  };

  struct HandoverReconfigurationAckParams
  {
    uint16_t sourceCellId;
    uint16_t targetCellId;
    uint16_t rnti;
  };

};


/**
 * These service primitives of this part of the X2 SAP
 * are provided by the X2 entity and issued by RRC entity
 */
class EpcX2SapProvider : public EpcX2Sap
{
public:
  virtual ~EpcX2SapProvider ();

  /**
   * Service primitives
   */

  virtual void SendHandoverRequest (HandoverRequestParams params) = 0;

  virtual void SendHandoverRequestAck (HandoverRequestAckParams params) = 0;

  virtual void SendHandoverPreparationFailure (HandoverPreparationFailureParams params) = 0;

  virtual void SendSnStatusTransfer (SnStatusTransferParams params) = 0;

  virtual void SendUeContextRelease (UeContextReleaseParams params) = 0;

  virtual void SendLoadInformation (LoadInformationParams params) = 0;

  virtual void SendResourceStatusUpdate (ResourceStatusUpdateParams params) = 0;

  virtual void SendSmallConnectionCompleted (SmallConnCompletedParams params) = 0;

  virtual void SendConnectionRequest (ConnectionRequestParams params) = 0;

  virtual void SendRrConfig (RrConfigParams params) = 0;

  virtual void SendUeData (UeDataParams params) = 0;

  virtual void SendOnOffRequest (OnOffRequestParams params) = 0;

  virtual void SendDlCqi (DlCqiParams params) = 0;

  virtual void SendHandoverTrigger (HandoverTriggerParams params) = 0;

  virtual void SendSwitchState (SwitchStateParams params) = 0;

  virtual void SendHandoverReconfigurationAck (HandoverReconfigurationAckParams params) = 0;

  virtual void SendHandoverTriggerAck (HandoverTriggerParams params) = 0;
};


/**
 * These service primitives of this part of the X2 SAP
 * are provided by the RRC entity and issued by the X2 entity
 */
class EpcX2SapUser : public EpcX2Sap
{
public:
  virtual ~EpcX2SapUser ();

  /**
   * Service primitives
   */

  virtual void RecvHandoverRequest (HandoverRequestParams params) = 0;

  virtual void RecvHandoverRequestAck (HandoverRequestAckParams params) = 0;

  virtual void RecvHandoverPreparationFailure (HandoverPreparationFailureParams params) = 0;

  virtual void RecvSnStatusTransfer (SnStatusTransferParams params) = 0;

  virtual void RecvUeContextRelease (UeContextReleaseParams params) = 0;

  virtual void RecvLoadInformation (LoadInformationParams params) = 0;
  
  virtual void RecvResourceStatusUpdate (ResourceStatusUpdateParams params) = 0;

  virtual void RecvSmallConnectionCompleted (SmallConnCompletedParams params) = 0;

  virtual void RecvConnectionRequest (ConnectionRequestParams params) = 0;

  virtual void RecvRrcd (RrConfigParams params) = 0;

  virtual void RecvUeData (UeDataParams params) = 0;

  virtual void RecvOnOffRequest (OnOffRequestParams params) = 0;

  virtual void RecvDlCqi (DlCqiParams params) = 0;

  virtual void RecvHandoverTrigger (HandoverTriggerParams params) = 0;

  virtual void RecvSwitchState (SwitchStateParams params) = 0;

  virtual void RecvHandoverReconfigurationAck (HandoverReconfigurationAckParams params) = 0;

  virtual void RecvHandoverTriggerAck (HandoverTriggerParams params) = 0;
};

///////////////////////////////////////

template <class C>
class EpcX2SpecificEpcX2SapProvider : public EpcX2SapProvider
{
public:
  EpcX2SpecificEpcX2SapProvider (C* x2);

  //
  // Interface implemented from EpcX2SapProvider
  //

  virtual void SendHandoverRequest (HandoverRequestParams params);

  virtual void SendHandoverRequestAck (HandoverRequestAckParams params);

  virtual void SendHandoverPreparationFailure (HandoverPreparationFailureParams params);

  virtual void SendSnStatusTransfer (SnStatusTransferParams params);

  virtual void SendUeContextRelease (UeContextReleaseParams params);

  virtual void SendLoadInformation (LoadInformationParams params);

  virtual void SendResourceStatusUpdate (ResourceStatusUpdateParams params);

  virtual void SendSmallConnectionCompleted (SmallConnCompletedParams params);

  virtual void SendConnectionRequest (ConnectionRequestParams params);

  virtual void SendRrConfig (RrConfigParams params);

  virtual void SendUeData (UeDataParams params);

  virtual void SendOnOffRequest (OnOffRequestParams params);

  virtual void SendDlCqi (DlCqiParams params);

  virtual void SendHandoverTrigger (HandoverTriggerParams params);

  virtual void SendSwitchState (SwitchStateParams params);

  virtual void SendHandoverReconfigurationAck (HandoverReconfigurationAckParams params);

  virtual void SendHandoverTriggerAck (HandoverTriggerParams params);

private:
  EpcX2SpecificEpcX2SapProvider ();
  C* m_x2;
};

template <class C>
EpcX2SpecificEpcX2SapProvider<C>::EpcX2SpecificEpcX2SapProvider (C* x2)
  : m_x2 (x2)
{
}

template <class C>
EpcX2SpecificEpcX2SapProvider<C>::EpcX2SpecificEpcX2SapProvider ()
{
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverRequest (HandoverRequestParams params)
{
  m_x2->DoSendHandoverRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverRequestAck (HandoverRequestAckParams params)
{
  m_x2->DoSendHandoverRequestAck (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverPreparationFailure (HandoverPreparationFailureParams params)
{
  m_x2->DoSendHandoverPreparationFailure (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendSnStatusTransfer (SnStatusTransferParams params)
{
  m_x2->DoSendSnStatusTransfer (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendUeContextRelease (UeContextReleaseParams params)
{
  m_x2->DoSendUeContextRelease (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendLoadInformation (LoadInformationParams params)
{
  m_x2->DoSendLoadInformation (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendResourceStatusUpdate (ResourceStatusUpdateParams params)
{
  m_x2->DoSendResourceStatusUpdate (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendSmallConnectionCompleted (SmallConnCompletedParams params)
{
  m_x2->DoSendSmallConnCompleted (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendConnectionRequest (ConnectionRequestParams params)
{
  m_x2->DoSendConnectionRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendRrConfig (RrConfigParams params)
{
  m_x2->DoSendRrConfig (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendUeData (UeDataParams params)
{
  m_x2->DoSendUeData (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendOnOffRequest (OnOffRequestParams params)
{
  m_x2->DoSendOnOffRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendDlCqi (DlCqiParams params)
{
  m_x2->DoSendDlCqi (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverTrigger (HandoverTriggerParams params)
{
  m_x2->DoSendHandoverTrigger (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendSwitchState (SwitchStateParams params)
{
  m_x2->DoSendSwitchState (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverReconfigurationAck (HandoverReconfigurationAckParams params)
{
  m_x2->DoSendHandoverReconfigurationAck (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverTriggerAck (HandoverTriggerParams params)
{
  m_x2->DoSendHandoverTriggerAck (params);
}

///////////////////////////////////////

template <class C>
class EpcX2SpecificEpcX2SapUser : public EpcX2SapUser
{
public:
  EpcX2SpecificEpcX2SapUser (C* rrc);

  //
  // Interface implemented from EpcX2SapUser
  //

  virtual void RecvHandoverRequest (HandoverRequestParams params);

  virtual void RecvHandoverRequestAck (HandoverRequestAckParams params);

  virtual void RecvHandoverPreparationFailure (HandoverPreparationFailureParams params);

  virtual void RecvSnStatusTransfer (SnStatusTransferParams params);

  virtual void RecvUeContextRelease (UeContextReleaseParams params);

  virtual void RecvLoadInformation (LoadInformationParams params);

  virtual void RecvResourceStatusUpdate (ResourceStatusUpdateParams params);

  virtual void RecvSmallConnectionCompleted (SmallConnCompletedParams params);

  virtual void RecvConnectionRequest (ConnectionRequestParams params);

  virtual void RecvRrcd (RrConfigParams params);

  virtual void RecvUeData (UeDataParams params);

  virtual void RecvOnOffRequest (OnOffRequestParams params);

  virtual void RecvDlCqi (DlCqiParams params);

  virtual void RecvHandoverTrigger (HandoverTriggerParams params);

  virtual void RecvSwitchState (SwitchStateParams params);

  virtual void RecvHandoverReconfigurationAck (HandoverReconfigurationAckParams params);

  virtual void RecvHandoverTriggerAck (HandoverTriggerParams params);

private:
  EpcX2SpecificEpcX2SapUser ();
  C* m_rrc;
};

template <class C>
EpcX2SpecificEpcX2SapUser<C>::EpcX2SpecificEpcX2SapUser (C* rrc)
  : m_rrc (rrc)
{
}

template <class C>
EpcX2SpecificEpcX2SapUser<C>::EpcX2SpecificEpcX2SapUser ()
{
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverRequest (HandoverRequestParams params)
{
  m_rrc->DoRecvHandoverRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverRequestAck (HandoverRequestAckParams params)
{
  m_rrc->DoRecvHandoverRequestAck (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverPreparationFailure (HandoverPreparationFailureParams params)
{
  m_rrc->DoRecvHandoverPreparationFailure (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvSnStatusTransfer (SnStatusTransferParams params)
{
  m_rrc->DoRecvSnStatusTransfer (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvUeContextRelease (UeContextReleaseParams params)
{
  m_rrc->DoRecvUeContextRelease (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvLoadInformation (LoadInformationParams params)
{
  m_rrc->DoRecvLoadInformation (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvResourceStatusUpdate (ResourceStatusUpdateParams params)
{
  m_rrc->DoRecvResourceStatusUpdate (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvSmallConnectionCompleted (SmallConnCompletedParams params)
{
  m_rrc->DoRecvSmallConnCompleted (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvConnectionRequest (ConnectionRequestParams params)
{
  m_rrc->DoRecvMacroConnectionRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvRrcd (RrConfigParams params)
{
  m_rrc->DoRecvRrcd (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvUeData (UeDataParams params)
{
  m_rrc->DoRecvUeData (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvOnOffRequest (OnOffRequestParams params)
{
  m_rrc->DoRecvOnOffRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvDlCqi (DlCqiParams params)
{
  m_rrc->DoRecvDlCqi (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverTrigger (HandoverTriggerParams params)
{
  m_rrc->DoRecvHandoverTrigger (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvSwitchState (SwitchStateParams params)
{
  m_rrc->DoRecvSwitchState (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverReconfigurationAck (HandoverReconfigurationAckParams params)
{
  m_rrc->DoRecvHandoverReconfigurationAck (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverTriggerAck (HandoverTriggerParams params)
{
  m_rrc->DoRecvHandoverTriggerAck (params);
}

} // namespace ns3

#endif // EPC_X2_SAP_H
