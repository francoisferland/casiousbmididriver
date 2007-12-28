/*=============================================================================
CasioUSBMIDI.cpp
	
Copyright (c) 2008, Francois Ferland

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this 
list of conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution.

Neither the name of Francois Ferland nor the names of its contributors may be 
used to endorse or promote products derived from this software without specific 
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#include "CasioUSBMIDI.h"


// Driver UUID
// 70B13D96-B558-11DC-8314-0800200C9A66
#define kFactoryUUID CFUUIDGetConstantUUIDWithBytes(NULL, 0x70, 0xB1, 0x3D, 0x96, 0xB5, 0x58, 0x11, 0xDC, 0x83, 0x14, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x66)


// Device-specific constants
#define kTheInterfaceToUse	0		// First USB interface

#define kMyVendorID			0x07CF	// Casio
#define kMyProductID		0x6802
#define kMyNumPorts			1		// One in-out pair

// __________________________________________________________________________________________________


// Implementation of the factory function for this type.
extern "C" void *NewCasioUSBMIDIDriver(CFAllocatorRef allocator, CFUUIDRef typeID);
extern "C" void *NewCasioUSBMIDIDriver(CFAllocatorRef allocator, CFUUIDRef typeID) 
{
	// If correct type is being requested, allocate an
	// instance of TestType and return the IUnknown interface.
	if (CFEqual(typeID, kMIDIDriverTypeID)) {
		CasioUSBMIDIDriver *result = new CasioUSBMIDIDriver;
		return result->Self();
	} else {
		// If the requested type is incorrect, return NULL.
		return NULL;
	}
}

// __________________________________________________________________________________________________

CasioUSBMIDIDriver::CasioUSBMIDIDriver() :
	USBVendorMIDIDriver(kFactoryUUID)
{
}

CasioUSBMIDIDriver::~CasioUSBMIDIDriver()
{
}

// __________________________________________________________________________________________________

bool		CasioUSBMIDIDriver::MatchDevice(	USBDevice *		inUSBDevice)
{
	const IOUSBDeviceDescriptor * devDesc = inUSBDevice->GetDeviceDescriptor();
	if (USBToHostWord(devDesc->idVendor) == kMyVendorID) {
		UInt16 devProduct = USBToHostWord(devDesc->idProduct);
		if (devProduct == kMyProductID)
			return true;
	}
	return false;
}

MIDIDeviceRef	CasioUSBMIDIDriver::CreateDevice(	USBDevice *		inUSBDevice,
													USBInterface *	inUSBInterface)
{
	MIDIDeviceRef dev;
	MIDIEntityRef ent;

	CFStringRef boxName = CFSTR("Casio USB MIDI Interface");
	MIDIDeviceCreate(Self(),
		boxName,
		CFSTR("Casio"),	// manufacturer name
		boxName,
		&dev);
	
	// make entity for each port, with 1 source, 1 destination
	for (int port = 1; port <= kMyNumPorts; ++port) {
		char portname[64];
		if (kMyNumPorts > 1)
			sprintf(portname, "Port %d", port);
		else
			CFStringGetCString(boxName, portname, sizeof(portname), kCFStringEncodingMacRoman);

		CFStringRef str = CFStringCreateWithCString(NULL, portname, 0);
		MIDIDeviceAddEntity(dev, str, false, 1, 1, &ent);
		CFRelease(str);
	}

	return dev;
}

USBInterface *	CasioUSBMIDIDriver::CreateInterface(USBMIDIDevice *device)
{
	USBInterface *intf = device->mUSBDevice->FindInterface(kTheInterfaceToUse, 0);
	return intf;
}

void		CasioUSBMIDIDriver::StartInterface(USBMIDIDevice *usbmDev)
{
}

void		CasioUSBMIDIDriver::StopInterface(USBMIDIDevice *usbmDev)
{
}

void		CasioUSBMIDIDriver::HandleInput(USBMIDIDevice *usbmDev, MIDITimeStamp when, Byte *readBuf, ByteCount readBufSize)
{
	USBMIDIHandleInput(usbmDev, when, readBuf, readBufSize);
}

ByteCount	CasioUSBMIDIDriver::PrepareOutput(USBMIDIDevice *usbmDev, WriteQueue &writeQueue, 
				Byte *destBuf)
{
	int n = USBMIDIPrepareOutput(usbmDev, writeQueue, destBuf, usbmDev->mOutPipe.mMaxPacketSize);
	if (n < usbmDev->mOutPipe.mMaxPacketSize) {
		memset(destBuf + n, 0, usbmDev->mOutPipe.mMaxPacketSize - n);
	}
	return usbmDev->mOutPipe.mMaxPacketSize;
}
