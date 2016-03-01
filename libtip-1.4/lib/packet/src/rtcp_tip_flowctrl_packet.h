/*
 *   Copyright 2010 IMTC, Inc.  All rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RTCP_TIP_FLOWCTRL_PACKET_H
#define RTCP_TIP_FLOWCTRL_PACKET_H

#include "rtcp_packet.h"

namespace LibTip {

    class CRtcpAppFlowCtrlPacket : public CRtcpTipPacket {
    public:
        // passed in flowCtrlType should be either TXFLOWCTRL or RXFLOWCTRL
        CRtcpAppFlowCtrlPacket(TipPacketType flowCtrlType);
        virtual ~CRtcpAppFlowCtrlPacket();

        enum {
            OPCODE_START = 0,
            OPCODE_STOP,
            OPCODE_H264_CONTROL
        };
        
        // get/set the opcode for this packet
        uint32_t GetOpcode() const { return mFlowCtrl.mOpcode; }
        void SetOpcode(uint32_t opcode) { mFlowCtrl.mOpcode = opcode; }

        // get/set the target SSRC/CSRC
        uint32_t GetTarget() const { return mFlowCtrl.mTarget; }
        void SetTarget(uint32_t target) { mFlowCtrl.mTarget = target; }

        virtual void ToStream(std::ostream& o, MediaType mType = MT_MAX) const;
        
    protected:
        // pack the component data into the packet buffer.
        virtual uint32_t PackData(CPacketBuffer& buffer) const;

        // unpack from a network packet buffer
        virtual int UnpackData(CPacketBuffer& buffer);

        // convert an opcode into a string
        virtual const char* OpcodeToString(uint16_t opcode) const;
        
        struct RtcpAppFlowCtrl {
            uint32_t mOpcode;
            uint32_t mTarget;
        };
        RtcpAppFlowCtrl mFlowCtrl;
    };

    class CRtcpAppTXFlowCtrlPacket : public CRtcpAppFlowCtrlPacket {
    public:
        CRtcpAppTXFlowCtrlPacket();
        virtual ~CRtcpAppTXFlowCtrlPacket();
    };

    class CRtcpAppRXFlowCtrlPacket : public CRtcpAppFlowCtrlPacket {
    public:
        CRtcpAppRXFlowCtrlPacket();
        virtual ~CRtcpAppRXFlowCtrlPacket();
    };

    class CRtcpAppTXFlowCtrlPacketV8 : public CRtcpAppTXFlowCtrlPacket {
    public:
        CRtcpAppTXFlowCtrlPacketV8();
        virtual ~CRtcpAppTXFlowCtrlPacketV8();

        // get/set the bitrate
        uint32_t GetBitrate() const { return mControl.mBitrate; }
        void SetBitrate(uint32_t b) { mControl.mBitrate = b; }

        // get/set the H.264 level integer
        uint16_t GetH264LevelInteger() const { return mControl.mH264LevelInteger; }
        void SetH264LevelInteger(uint16_t l) { mControl.mH264LevelInteger = l; }

        // get/set the H.264 level decimal
        uint16_t GetH264LevelDecimal() const { return mControl.mH264LevelDecimal; }
        void SetH264LevelDecimal(uint16_t l) { mControl.mH264LevelDecimal = l; }

        // get/set the H.264 max-mbps
        uint32_t GetH264MaxMbps() const { return mControl.mH264MaxMbps; }
        void SetH264MaxMbps(uint32_t m) { mControl.mH264MaxMbps = m; }

        // get/set the H.264 max-fs
        uint32_t GetH264MaxFs() const { return mControl.mH264MaxFs; }
        void SetH264MaxFs(uint32_t m) { mControl.mH264MaxFs = m; }

        // get/set the H.264 max-fps
        uint32_t GetH264MaxFps() const { return mControl.mH264MaxFps; }
        void SetH264MaxFps(uint32_t m) { mControl.mH264MaxFps = m; }

        virtual void ToStream(std::ostream& o, MediaType mType = MT_MAX) const;

    protected:
        // pack the component data into the packet buffer.
        virtual uint32_t PackData(CPacketBuffer& buffer) const;

        // unpack from a network packet buffer
        virtual int UnpackData(CPacketBuffer& buffer);

        struct RtcpAppFlowCtrlH264Control {
            uint32_t mBitrate;
            uint16_t mH264LevelInteger;
            uint16_t mH264LevelDecimal;
            uint32_t mH264MaxMbps;
            uint32_t mH264MaxFs;
            uint32_t mH264MaxFps;
        };
        RtcpAppFlowCtrlH264Control mControl;
    };
};

#endif
