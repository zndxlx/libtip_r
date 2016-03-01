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

#include <iostream>
using namespace std;

#include "rtcp_tip_flowctrl_packet.h"
#include "test_packet_data.h"
#include "tip_debug_tools.h"
using namespace LibTip;

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class CRtcpAppFlowCtrlPacketTest : public CppUnit::TestFixture {
private:
    CRtcpAppFlowCtrlPacket* packet;

public:
    void setUp() {
        packet = new CRtcpAppFlowCtrlPacket(TXFLOWCTRL);
    }

    void tearDown() {
        delete packet;
    }

    void testDefaults() {
        CPPUNIT_ASSERT_EQUAL( packet->GetOpcode(), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetTarget(), (uint32_t) 0 );
    }

    void testOpcode() {
        packet->SetOpcode(0xA5A5A5A5);
        CPPUNIT_ASSERT_EQUAL( packet->GetOpcode(), (uint32_t) 0xA5A5A5A5 );
    }

    void testTarget() {
        packet->SetTarget(0xA5A5A5A5);
        CPPUNIT_ASSERT_EQUAL( packet->GetTarget(), (uint32_t) 0xA5A5A5A5 );
    }

#define RTCP_APP_FLOWCTRL_BYTES(opcode, target)      \
        ((opcode) >> 24), (((opcode) >> 16) & 0xFF), \
        (((opcode) >> 8) & 0xFF), ((opcode) & 0xFF), \
        ((target) >> 24), (((target) >> 16) & 0xFF), \
        (((target) >> 8) & 0xFF), ((target) & 0xFF)

    void testPack1() {
        CPacketBufferData buffer;

        uint8_t def[] = { RTCP_PACKET_BYTES(TXFLOWCTRL,
                                            CRtcpPacket::APP,
                                            packet->GetLength()),
                          RTCP_APP_PACKET_BYTES_TP1,
                          RTCP_TIP_PACKET_BYTES,
                          RTCP_APP_FLOWCTRL_BYTES(0, 0)
        };

        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), (uint32_t) sizeof(def) );
        CPPUNIT_ASSERT_EQUAL( (packet->GetPackSize() % 4), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), packet->Pack(buffer) );
        if (memcmp(buffer.GetBuffer(), def, packet->GetPackSize()) != 0) {
            ostringstream oss;
            
            oss << "\nExpected:  " << HexDump(def, packet->GetPackSize())
                << "\nPacked:    " << HexDump(buffer.GetBuffer(), packet->GetPackSize());
            CPPUNIT_FAIL(oss.str());
        }
    }
    
    void testPack2() {
        CPacketBufferData buffer;

        uint8_t def[] = { RTCP_PACKET_BYTES(TXFLOWCTRL,
                                            CRtcpPacket::APP,
                                            packet->GetLength()),
                          RTCP_APP_PACKET_BYTES_TP1,
                          RTCP_TIP_PACKET_BYTES,
                          RTCP_APP_FLOWCTRL_BYTES(0xA5A5A5A5, 0x12345678)
        };

        packet->SetOpcode(0xA5A5A5A5);
        packet->SetTarget(0x12345678);
        
        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), (uint32_t) sizeof(def) );
        CPPUNIT_ASSERT_EQUAL( (packet->GetPackSize() % 4), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), packet->Pack(buffer) );
        if (memcmp(buffer.GetBuffer(), def, packet->GetPackSize()) != 0) {
            ostringstream oss;
            
            oss << "\nExpected:  " << HexDump(def, packet->GetPackSize())
                << "\nPacked:    " << HexDump(buffer.GetBuffer(), packet->GetPackSize());
            CPPUNIT_FAIL(oss.str());
        }
    }

    void testUnpack1() {
        CRtcpAppFlowCtrlPacket packet2(packet->GetTipPacketType());
        CPacketBufferData buffer;
        CPacketBufferData buffer2;

        packet->Pack(buffer);
        CPPUNIT_ASSERT_EQUAL( packet2.Unpack(buffer), 0 );
        packet2.Pack(buffer2);
        if (memcmp(buffer.GetBuffer(), buffer2.GetBuffer(), buffer.GetBufferSize()) != 0) {
            ostringstream oss;
            
            oss << "\nExpected:  " << HexDump(buffer.GetBuffer(), buffer.GetBufferSize())
                << "\nPacked:    " << HexDump(buffer2.GetBuffer(), buffer.GetBufferSize());

            CPPUNIT_FAIL(oss.str());
        }
    }
    
    void testUnpack2() {
        CRtcpAppFlowCtrlPacket packet2(TXFLOWCTRL);
        CPacketBufferData buffer;

        packet->SetOpcode(0xA5A5A5A5);
        packet->SetTarget(0x12345678);
        
        packet->Pack(buffer);
        CPPUNIT_ASSERT_EQUAL( packet2.Unpack(buffer), 0 );

        CPPUNIT_ASSERT_EQUAL( packet->GetOpcode(), packet2.GetOpcode() );
        CPPUNIT_ASSERT_EQUAL( packet->GetTarget(), packet2.GetTarget() );
    }

    void testUnpackFail() {
        CRtcpAppFlowCtrlPacket packet2(TXFLOWCTRL);
        CPacketBufferData buffer;

        packet2.SetType(CRtcpPacket::RR);
        packet2.Pack(buffer);
        
        CPPUNIT_ASSERT_EQUAL( packet->Unpack(buffer), -1 );
    }
        
    void testTXFlowCtrl() {
        CRtcpAppTXFlowCtrlPacket packet2;
        CPPUNIT_ASSERT_EQUAL( packet2.GetTipPacketType(), TXFLOWCTRL );
    }
    
    void testRXFlowCtrl() {
        CRtcpAppRXFlowCtrlPacket packet2;
        CPPUNIT_ASSERT_EQUAL( packet2.GetTipPacketType(), RXFLOWCTRL );
    }
    
    void testToStream() {
        ostringstream oss;
        packet->ToStream(oss);

        CRtcpAppRXFlowCtrlPacket packet2;
        packet2.ToStream(oss);

        CRtcpAppTXFlowCtrlPacket packet3;
        packet3.ToStream(oss);
    }
    
    CPPUNIT_TEST_SUITE( CRtcpAppFlowCtrlPacketTest );
    CPPUNIT_TEST( testDefaults );
    CPPUNIT_TEST( testOpcode );
    CPPUNIT_TEST( testTarget );
    CPPUNIT_TEST( testPack1 );
    CPPUNIT_TEST( testPack2 );
    CPPUNIT_TEST( testUnpack1 );
    CPPUNIT_TEST( testUnpack2 );
    CPPUNIT_TEST( testUnpackFail );
    CPPUNIT_TEST( testTXFlowCtrl );
    CPPUNIT_TEST( testRXFlowCtrl );
    CPPUNIT_TEST( testToStream );
    CPPUNIT_TEST_SUITE_END();
};

class CRtcpAppTXFlowCtrlPacketV8Test : public CppUnit::TestFixture {
private:
    CRtcpAppTXFlowCtrlPacketV8* packet;

public:
    void setUp() {
        packet = new CRtcpAppTXFlowCtrlPacketV8();
    }

    void tearDown() {
        delete packet;
    }

    void testDefaults() {
        CPPUNIT_ASSERT_EQUAL( packet->GetBitrate(), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetH264LevelInteger(), (uint16_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetH264LevelDecimal(), (uint16_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetH264MaxMbps(), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetH264MaxFs(), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetH264MaxFps(), (uint32_t) 0 );
    }

    void testBitrate() {
        packet->SetBitrate(0x12345678);
        CPPUNIT_ASSERT_EQUAL( packet->GetBitrate(), (uint32_t) 0x12345678 );
    }

    void testH264LevelInteger() {
        packet->SetH264LevelInteger(0x1234);
        CPPUNIT_ASSERT_EQUAL( packet->GetH264LevelInteger(), (uint16_t) 0x1234 );
    }

    void testH264LevelDecimal() {
        packet->SetH264LevelDecimal(0x5678);
        CPPUNIT_ASSERT_EQUAL( packet->GetH264LevelDecimal(), (uint16_t) 0x5678 );
    }

    void testH264MaxMbps() {
        packet->SetH264MaxMbps(0x12345678);
        CPPUNIT_ASSERT_EQUAL( packet->GetH264MaxMbps(), (uint32_t) 0x12345678 );
    }

    void testH264MaxFs() {
        packet->SetH264MaxFs(0x12345678);
        CPPUNIT_ASSERT_EQUAL( packet->GetH264MaxFs(), (uint32_t) 0x12345678 );
    }

    void testH264MaxFps() {
        packet->SetH264MaxFps(0x12345678);
        CPPUNIT_ASSERT_EQUAL( packet->GetH264MaxFps(), (uint32_t) 0x12345678 );
    }

#define RTCP_APP_TX_FLOWCTRL_V8_BYTES(bitrate, li, ld, mbps, fs, fps) \
        ((bitrate) >> 24), (((bitrate) >> 16) & 0xFF),                \
        (((bitrate) >> 8) & 0xFF), ((bitrate) & 0xFF),                \
        (((li) >> 8) & 0xFF), ((li) & 0xFF),                          \
        (((ld) >> 8) & 0xFF), ((ld) & 0xFF),                          \
        ((mbps) >> 24), (((mbps) >> 16) & 0xFF),                      \
        (((mbps) >> 8) & 0xFF), ((mbps) & 0xFF),                      \
        ((fs) >> 24), (((fs) >> 16) & 0xFF),                          \
        (((fs) >> 8) & 0xFF), ((fs) & 0xFF),                          \
        ((fps) >> 24), (((fps) >> 16) & 0xFF),                        \
        (((fps) >> 8) & 0xFF), ((fps) & 0xFF)

    void testPack1() {
        CPacketBufferData buffer;

        uint8_t def[] = { RTCP_PACKET_BYTES(TXFLOWCTRL,
                                            CRtcpPacket::APP,
                                            packet->GetLength()),
                          RTCP_APP_PACKET_BYTES_TP1,
                          RTCP_TIP_PACKET_BYTES,
                          RTCP_APP_FLOWCTRL_BYTES(0, 0),
                          RTCP_APP_TX_FLOWCTRL_V8_BYTES(0, 0, 0, 0, 0, 0)
        };

        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), (uint32_t) sizeof(def) );
        CPPUNIT_ASSERT_EQUAL( (packet->GetPackSize() % 4), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), packet->Pack(buffer) );
        if (memcmp(buffer.GetBuffer(), def, packet->GetPackSize()) != 0) {
            ostringstream oss;
            
            oss << "\nExpected:  " << HexDump(def, packet->GetPackSize())
                << "\nPacked:    " << HexDump(buffer.GetBuffer(), packet->GetPackSize());
            CPPUNIT_FAIL(oss.str());
        }
    }
    
    void testPack2() {
        CPacketBufferData buffer;

        uint8_t def[] = { RTCP_PACKET_BYTES(TXFLOWCTRL,
                                            CRtcpPacket::APP,
                                            packet->GetLength()),
                          RTCP_APP_PACKET_BYTES_TP1,
                          RTCP_TIP_PACKET_BYTES,
                          RTCP_APP_FLOWCTRL_BYTES(0, 0),
                          RTCP_APP_TX_FLOWCTRL_V8_BYTES(0x12345678, 0x90AB, 0xCDEF,
                                                        0xDEADBEEF, 0xFACEFACE, 0xA5A5A5A5)
        };

        packet->SetBitrate(0x12345678);
        packet->SetH264LevelInteger(0x90AB);
        packet->SetH264LevelDecimal(0xCDEF);
        packet->SetH264MaxMbps(0xDEADBEEF);
        packet->SetH264MaxFs(0xFACEFACE);
        packet->SetH264MaxFps(0xA5A5A5A5);
        
        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), (uint32_t) sizeof(def) );
        CPPUNIT_ASSERT_EQUAL( (packet->GetPackSize() % 4), (uint32_t) 0 );
        CPPUNIT_ASSERT_EQUAL( packet->GetPackSize(), packet->Pack(buffer) );
        if (memcmp(buffer.GetBuffer(), def, packet->GetPackSize()) != 0) {
            ostringstream oss;
            
            oss << "\nExpected:  " << HexDump(def, packet->GetPackSize())
                << "\nPacked:    " << HexDump(buffer.GetBuffer(), packet->GetPackSize());
            CPPUNIT_FAIL(oss.str());
        }
    }
    
    void testUnpack1() {
        CRtcpAppTXFlowCtrlPacketV8 packet2;
        CPacketBufferData buffer;
        CPacketBufferData buffer2;

        packet->Pack(buffer);
        CPPUNIT_ASSERT_EQUAL( packet2.Unpack(buffer), 0 );
        packet2.Pack(buffer2);
        if (memcmp(buffer.GetBuffer(), buffer2.GetBuffer(), buffer.GetBufferSize()) != 0) {
            ostringstream oss;
            
            oss << "\nExpected:  " << HexDump(buffer.GetBuffer(), buffer.GetBufferSize())
                << "\nPacked:    " << HexDump(buffer2.GetBuffer(), buffer.GetBufferSize());

            CPPUNIT_FAIL(oss.str());
        }
    }
    
    void testUnpack2() {
        CRtcpAppTXFlowCtrlPacketV8 packet2;
        CPacketBufferData buffer;

        packet->SetBitrate(0x12345678);
        packet->SetH264LevelInteger(0x90AB);
        packet->SetH264LevelDecimal(0xCDEF);
        packet->SetH264MaxMbps(0xDEADBEEF);
        packet->SetH264MaxFs(0xFACEFACE);
        packet->SetH264MaxFps(0xA5A5A5A5);
        
        packet->Pack(buffer);
        CPPUNIT_ASSERT_EQUAL( packet2.Unpack(buffer), 0 );

        CPPUNIT_ASSERT_EQUAL( packet->GetOpcode(), packet2.GetOpcode() );
        CPPUNIT_ASSERT_EQUAL( packet->GetTarget(), packet2.GetTarget() );
    }

    void testUnpackFail() {
        CRtcpAppTXFlowCtrlPacketV8 packet2;
        CPacketBufferData buffer;

        packet2.SetType(CRtcpPacket::RR);
        packet2.Pack(buffer);
        
        CPPUNIT_ASSERT_EQUAL( packet->Unpack(buffer), -1 );
    }
        
    void testToStream() {
        ostringstream oss;
        packet->ToStream(oss);
    }
    
    CPPUNIT_TEST_SUITE( CRtcpAppTXFlowCtrlPacketV8Test );
    CPPUNIT_TEST( testDefaults );
    CPPUNIT_TEST( testBitrate );
    CPPUNIT_TEST( testH264LevelInteger );
    CPPUNIT_TEST( testH264LevelDecimal );
    CPPUNIT_TEST( testH264MaxMbps );
    CPPUNIT_TEST( testH264MaxFs );
    CPPUNIT_TEST( testH264MaxFps );
    CPPUNIT_TEST( testPack1 );
    CPPUNIT_TEST( testPack2 );
    CPPUNIT_TEST( testUnpack1 );
    CPPUNIT_TEST( testUnpack2 );
    CPPUNIT_TEST( testUnpackFail );
    CPPUNIT_TEST( testToStream );
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION( CRtcpAppFlowCtrlPacketTest );
CPPUNIT_TEST_SUITE_REGISTRATION( CRtcpAppTXFlowCtrlPacketV8Test );
