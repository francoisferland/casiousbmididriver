/*	Copyright � 2007 Apple Inc. All Rights Reserved.
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by 
			Apple Inc. ("Apple") in consideration of your agreement to the
			following terms, and your use, installation, modification or
			redistribution of this Apple software constitutes acceptance of these
			terms.  If you do not agree with these terms, please do not use,
			install, modify or redistribute this Apple software.
			
			In consideration of your agreement to abide by the following terms, and
			subject to these terms, Apple grants you a personal, non-exclusive
			license, under Apple's copyrights in this original Apple software (the
			"Apple Software"), to use, reproduce, modify and redistribute the Apple
			Software, with or without modifications, in source and/or binary forms;
			provided that if you redistribute the Apple Software in its entirety and
			without modifications, you must retain this notice and the following
			text and disclaimers in all such redistributions of the Apple Software. 
			Neither the name, trademarks, service marks or logos of Apple Inc. 
			may be used to endorse or promote products derived from the Apple
			Software without specific prior written permission from Apple.  Except
			as expressly stated in this notice, no other rights or licenses, express
			or implied, are granted by Apple herein, including but not limited to
			any patent rights that may be infringed by your derivative works or by
			other works in which the Apple Software may be incorporated.
			
			The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
			MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
			THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
			FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
			OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
			
			IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
			OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
			SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
			INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
			MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
			AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
			STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
			POSSIBILITY OF SUCH DAMAGE.
*/
#include "CasioUSBMIDI.h"


// ^^^^^^^^^^^^^^^^^^ THINGS YOU MUST CUSTOMIZE ^^^^^^^^^^^^^^^^^^^^^^

// Generated UUID: 0AF61C91-A75F-4CDC-A91A-9A76B179F7DE
#define kFactoryUUID CFUUIDGetConstantUUIDWithBytes(NULL, 0x0A, 0xF6, 0x1C, 0x91, 0xA7, 0x5F, 0x4C, 0xDC, 0xA9, 0x1A, 0x9A, 0x76, 0xB1, 0x79, 0xF7, 0xDE)
// 46727133-BF59-11D5-927F-00039301A6E6

#define kTheInterfaceToUse	0		// First one

#define kMyVendorID			0x07CF	// Casio
#define kMyProductID		0x6802
#define kMyNumPorts			1		// One in-out pair.

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
	//UInt16 devProduct = USBToHostWord(inUSBDevice->GetDeviceDescriptor()->idProduct);
	
	CFStringRef boxName = CFSTR("Casio USB Keyboard");
	MIDIDeviceCreate(Self(),
		boxName,
		CFSTR("Acme"),	// manufacturer name
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
