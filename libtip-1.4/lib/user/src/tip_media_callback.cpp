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

#include "tip_media_callback.h"
using namespace LibTip;

CTipMediaSinkCallback::CTipMediaSinkCallback() {}
CTipMediaSinkCallback::~CTipMediaSinkCallback() {}

void CTipMediaSinkCallback::Stop() {}
void CTipMediaSinkCallback::Start() {}

CTipMediaSourceCallback::CTipMediaSourceCallback() {}
CTipMediaSourceCallback::~CTipMediaSourceCallback() {}

void CTipMediaSourceCallback::Stop() {}
void CTipMediaSourceCallback::Start() {}
void CTipMediaSourceCallback::H264Configure(uint32_t bitrate, uint16_t level_integer,
                                            uint16_t level_decimal, uint32_t max_mbps,
                                            uint32_t max_fs, uint32_t max_fps)
{}

void CTipMediaSourceCallback::Refresh(bool idr) {}
void CTipMediaSourceCallback::ProcessNack(uint16_t seqno) {}
void CTipMediaSourceCallback::ProcessAck(uint16_t seqno) {}
