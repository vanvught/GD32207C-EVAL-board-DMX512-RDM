/**
 * @file storenode.h
 *
 */
/* Copyright (C) 2022-2023 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef STORENODE_H_
#define STORENODE_H_

#include <stdint.h>
#include <cassert>

#include "nodeparams.h"
#include "nodestore.h"

#include "configstore.h"

#include "debug.h"

class StoreNode final: public NodeParamsStore, public NodeStore {
public:
	StoreNode(uint32_t nPortIndexOffset = 0);

	void Update(const struct nodeparams::Params* pArtNetParams) override {
		DEBUG_ENTRY

		ConfigStore::Get()->Update(configstore::Store::NODE, pArtNetParams, sizeof(struct nodeparams::Params));

		DEBUG_EXIT
	}

	void Copy(struct nodeparams::Params* pArtNetParams) override {
		DEBUG_ENTRY

		ConfigStore::Get()->Copy(configstore::Store::NODE, pArtNetParams, sizeof(struct nodeparams::Params));

		DEBUG_EXIT
	}

	void SaveFailSafe(uint8_t nFailSafe) override {
		DEBUG_ENTRY

		ConfigStore::Get()->Update(configstore::Store::NODE, __builtin_offsetof(struct nodeparams::Params, nFailSafe), &nFailSafe, sizeof(uint8_t), nodeparams::Mask::FAILSAFE);

		DEBUG_EXIT
	}

	void SaveShortName(uint32_t nPortIndex, const char *pShortName) override {
		DEBUG_ENTRY

		ConfigStore::Get()->Update(configstore::Store::NODE, nPortIndex * 0 + __builtin_offsetof(struct nodeparams::Params, aLabel), pShortName, artnet::SHORT_NAME_LENGTH, nodeparams::Mask::SHORT_NAME);

		DEBUG_EXIT
	}

	void SaveLongName(const char *pLongName) override {
		DEBUG_ENTRY

		ConfigStore::Get()->Update(configstore::Store::NODE, __builtin_offsetof(struct nodeparams::Params, aLongName), pLongName, artnet::LONG_NAME_LENGTH, nodeparams::Mask::LONG_NAME);

		DEBUG_EXIT
	}

	void SaveMergeMode(uint32_t nPortIndex, lightset::MergeMode mergeMode) override {
		DEBUG_ENTRY
		DEBUG_PRINTF("%u, %u", nPortIndex, static_cast<uint32_t>(mergeMode));

		if (nPortIndex >= s_nPortIndexOffset) {
			nPortIndex -= s_nPortIndexOffset;
		} else {
			DEBUG_EXIT
			return;
		}

		DEBUG_PRINTF("nPortIndex=%u", nPortIndex);

		if (nPortIndex >= nodeparams::MAX_PORTS) {
			DEBUG_EXIT
			return;
		}

		uint16_t nMergeMode;
		ConfigStore::Get()->Copy(configstore::Store::NODE, &nMergeMode, sizeof(uint16_t), __builtin_offsetof(struct nodeparams::Params, nMergeMode));

		nMergeMode &= nodeparams::clear_mask(nPortIndex);

		if (mergeMode == lightset::MergeMode::LTP) {
			nMergeMode |= nodeparams::shift_left(static_cast<uint32_t>(lightset::MergeMode::LTP), nPortIndex);
			nMergeMode |= static_cast<uint16_t>(1U << (nPortIndex + 8));
		} else {
			nMergeMode |= nodeparams::shift_left(static_cast<uint32_t>(lightset::MergeMode::HTP), nPortIndex);
		}

		ConfigStore::Get()->Update(configstore::Store::NODE, __builtin_offsetof(struct nodeparams::Params, nMergeMode), &nMergeMode, sizeof(uint16_t));

		DEBUG_EXIT
	}

	void SavePortProtocol(uint32_t nPortIndex, artnet::PortProtocol portProtocol) override {
		DEBUG_ENTRY
		DEBUG_PRINTF("%u, %u", nPortIndex, static_cast<uint32_t>(portProtocol));

		if (nPortIndex >= s_nPortIndexOffset) {
			nPortIndex -= s_nPortIndexOffset;
		} else {
			DEBUG_EXIT
			return;
		}

		DEBUG_PRINTF("nPortIndex=%u", nPortIndex);

		if (nPortIndex >= nodeparams::MAX_PORTS) {
			DEBUG_EXIT
			return;
		}

		uint16_t nProtocol;
		ConfigStore::Get()->Copy(configstore::Store::NODE, &nProtocol, sizeof(uint16_t), __builtin_offsetof(struct nodeparams::Params, nProtocol));

		nProtocol &= nodeparams::clear_mask(nPortIndex);

		if (portProtocol == artnet::PortProtocol::SACN) {
			nProtocol |= nodeparams::shift_left(static_cast<uint32_t>(node::PortProtocol::SACN), nPortIndex);
			nProtocol |= static_cast<uint16_t>(1U << (nPortIndex + 8));
		} else {
			nProtocol |= nodeparams::shift_left(static_cast<uint32_t>(node::PortProtocol::ARTNET), nPortIndex);
		}

		ConfigStore::Get()->Update(configstore::Store::NODE, __builtin_offsetof(struct nodeparams::Params, nProtocol), &nProtocol, sizeof(uint16_t));

		DEBUG_EXIT
	}

	void SaveOutputStyle(uint32_t nPortIndex, const lightset::OutputStyle outputStyle) override {
		DEBUG_ENTRY
		DEBUG_PRINTF("s_nPortIndexOffset=%u, nPortIndex=%u, outputStyle=%u", s_nPortIndexOffset, nPortIndex, static_cast<uint32_t>(outputStyle));

		if (nPortIndex >= s_nPortIndexOffset) {
			nPortIndex -= s_nPortIndexOffset;
		} else {
			DEBUG_EXIT
			return;
		}

		DEBUG_PRINTF("nPortIndex=%u", nPortIndex);

		if (nPortIndex >= artnet::PORTS) {
			DEBUG_EXIT
			return;
		}

		uint8_t nOutputStyle;
		ConfigStore::Get()->Copy(configstore::Store::NODE, &nOutputStyle, sizeof(uint8_t), __builtin_offsetof(struct nodeparams::Params, nOutputStyle));

		if (outputStyle == lightset::OutputStyle::CONSTANT) {
			nOutputStyle |= static_cast<uint8_t>(1U << nPortIndex);
		} else {
			nOutputStyle &= static_cast<uint8_t>(~(1U << nPortIndex));
		}

		ConfigStore::Get()->Update(configstore::Store::NODE, __builtin_offsetof(struct nodeparams::Params, nOutputStyle), &nOutputStyle, sizeof(uint8_t));

		DEBUG_EXIT
	}

	void SaveRdmEnabled(uint32_t nPortIndex, bool isEnabled) override {
		DEBUG_ENTRY
		DEBUG_PRINTF("nPortIndex=%u, isEnabled=%d", nPortIndex, isEnabled);

		if (nPortIndex >= s_nPortIndexOffset) {
			nPortIndex -= s_nPortIndexOffset;
		} else {
			DEBUG_EXIT
			return;
		}

		DEBUG_PRINTF("nPortIndex=%u", nPortIndex);

		if (nPortIndex >= nodeparams::MAX_PORTS) {
			DEBUG_EXIT
			return;
		}

		uint16_t nRdm;
		ConfigStore::Get()->Copy(configstore::Store::NODE, &nRdm, sizeof(uint16_t), __builtin_offsetof(struct nodeparams::Params, nRdm));

		nRdm &= nodeparams::clear_mask(nPortIndex);

		if (isEnabled) {
			nRdm |= nodeparams::shift_left(1, nPortIndex);
			nRdm |= static_cast<uint16_t>(1U << (nPortIndex + 8));
		}

		ConfigStore::Get()->Update(configstore::Store::NODE, __builtin_offsetof(struct nodeparams::Params, nRdm), &nRdm, sizeof(uint16_t));

		DEBUG_EXIT
	}

	void SaveUniverseSwitch(uint32_t nPortIndex, __attribute__((unused)) uint8_t nAddress) override {
		DEBUG_ENTRY

		uint16_t nPortAddress;

		if (ArtNetNode::Get()->GetPortAddress(nPortIndex, nPortAddress)) {
			SaveUniverse(nPortIndex, nPortAddress);
		}

		DEBUG_EXIT
	}

	void SaveNetSwitch(uint32_t nPortIndex, __attribute__((unused)) uint8_t nAddress) override {
		DEBUG_ENTRY
		DEBUG_PRINTF("nPortIndex=%u, nAddress=%u", nPortIndex, static_cast<uint32_t>(nAddress));

		uint16_t nPortAddress;

		if (ArtNetNode::Get()->GetPortAddress(nPortIndex, nPortAddress)) {
			SaveUniverse(nPortIndex, nPortAddress);
		}

		DEBUG_EXIT
	}

	void SaveSubnetSwitch(uint32_t nPortIndex, __attribute__((unused)) uint8_t nAddress) override {
		DEBUG_ENTRY
		DEBUG_PRINTF("nPortIndex=%u, nAddress=%u", nPortIndex, static_cast<uint32_t>(nAddress));

		uint16_t nPortAddress;

		if (ArtNetNode::Get()->GetPortAddress(nPortIndex, nPortAddress)) {
			SaveUniverse(nPortIndex, nPortAddress);
		}

		DEBUG_EXIT
	}

	static StoreNode *Get() {
		return s_pThis;
	}

private:
	void SaveUniverse(uint32_t nPortIndex, uint16_t nUniverse) {
		DEBUG_ENTRY
		DEBUG_PRINTF("nPortIndex=%u, nUniverse=%u", nPortIndex, nUniverse);

		if (nPortIndex >= s_nPortIndexOffset) {
			nPortIndex -= s_nPortIndexOffset;
		} else {
			DEBUG_EXIT
			return;
		}

		DEBUG_PRINTF("nPortIndex=%u", nPortIndex);

		if (nPortIndex >= nodeparams::MAX_PORTS) {
			DEBUG_EXIT
			return;
		}

		ConfigStore::Get()->Update(configstore::Store::NODE, (sizeof(uint16_t) * nPortIndex) + __builtin_offsetof(struct nodeparams::Params, nUniverse), &nUniverse, sizeof(uint16_t), nodeparams::Mask::UNIVERSE_A << nPortIndex);

		DEBUG_EXIT
	}

private:
	static uint32_t s_nPortIndexOffset;
	static StoreNode *s_pThis;
};

#endif /* STORENODE_H_ */
