/**
 * @file main.cpp
 *
 */
/* Copyright (C) 2019-2024 by Arjan van Vught mailto:info@gd32-dmx.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <cstdint>
#include <cassert>

#include "hardware.h"
#include "network.h"
#include "networkconst.h"


#include "display.h"

#include "net/apps/mdns.h"

#if defined (ENABLE_NTP_CLIENT)
# include "net/apps/ntpclient.h"
#endif

#include "oscclient.h"
#include "oscclientparams.h"
#include "oscclientmsgconst.h"
#include "oscclientled.h"

#include "buttonsset.h"
#include "buttonsgpio.h"
#include "buttonsmcp.h"

#include "configstore.h"
#include "remoteconfig.h"
#include "remoteconfigparams.h"

#include "firmwareversion.h"
#include "software_version.h"

#include "displayhandler.h"

void Hardware::RebootHandler() {}

int main() {
	Hardware hw;
	Display display;
	ConfigStore configStore;
	display.TextStatus(NetworkConst::MSG_NETWORK_INIT, CONSOLE_YELLOW);
	Network nw;
	MDNS mDns;
	display.TextStatus(NetworkConst::MSG_NETWORK_STARTED, CONSOLE_GREEN);
	FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);

	fw.Print();
	

#if defined (ENABLE_NTP_CLIENT)
	NtpClient ntpClient;
	ntpClient.Start();
	ntpClient.Print();
#endif

	OscClientParams params;
	OscClient client;

	params.Load();
	params.Set(&client);

	mDns.ServiceRecordAdd(nullptr, mdns::Services::OSC, "type=client", client.GetPortIncoming());

	display.TextStatus(OscClientMsgConst::PARAMS, CONSOLE_YELLOW);

	client.Print();

	ButtonsSet *pButtonsSet;

	auto *pButtonsMcp = new ButtonsMcp(&client);
	assert(pButtonsMcp != nullptr);

	if (pButtonsMcp->Start()) {
		pButtonsSet = static_cast<ButtonsSet*>(pButtonsMcp);
		client.SetLedHandler(pButtonsMcp);
	} else {
		delete pButtonsMcp;

		auto *pButtonsGpio = new ButtonsGpio(&client);
		assert(pButtonsGpio != nullptr);

		pButtonsGpio->Start();

		pButtonsSet = static_cast<ButtonsSet*>(pButtonsGpio);
		client.SetLedHandler(pButtonsGpio);
	}

	RemoteConfig remoteConfig(remoteconfig::Node::OSC_CLIENT, remoteconfig::Output::OSC, pButtonsSet->GetButtonsCount());

	RemoteConfigParams remoteConfigParams;
	remoteConfigParams.Load();
	remoteConfigParams.Set(&remoteConfig);

	while (configStore.Flash())
		;

	for (auto i = 1; i < 7 ; i++) {
		display.ClearLine(i);
	}

	display.Write(1, "OSC Client");
	display.Printf(2, "%s.local", nw.GetHostName());
	display.Printf(3, "IP: " IPSTR " %c", IP2STR(Network::Get()->GetIp()), nw.IsDhcpKnown() ? (nw.IsDhcpUsed() ? 'D' : 'S') : ' ');
	display.Printf(4, "S : " IPSTR, IP2STR(client.GetServerIP()));
	display.Printf(5, "O : %d", client.GetPortOutgoing());
	display.Printf(6, "I : %d", client.GetPortIncoming());

	mDns.Print();

	display.TextStatus(OscClientMsgConst::START, CONSOLE_YELLOW);

	client.Start();

	display.TextStatus(OscClientMsgConst::STARTED, CONSOLE_GREEN);

	hw.WatchdogInit();

	for (;;) {
		hw.WatchdogFeed();
		nw.Run();
		client.Run();
		pButtonsSet->Run();
		remoteConfig.Run();
		configStore.Flash();
		mDns.Run();
#if defined (ENABLE_NTP_CLIENT)
		ntpClient.Run();
#endif
		hw.Run();
		display.Run();
	}
}
