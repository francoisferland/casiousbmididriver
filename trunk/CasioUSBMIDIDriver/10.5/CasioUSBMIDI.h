/*=============================================================================

CasioUSBMIDI.h
	
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

#ifndef __CasioUSBMIDI_h__
#define __CasioUSBMIDI_h__

#include "USBVendorMIDIDriver.h"

class CasioUSBMIDIDriver : public USBVendorMIDIDriver {
public:
	CasioUSBMIDIDriver();
	~CasioUSBMIDIDriver();
	
	// USBMIDIDriverBase overrides
	virtual bool			MatchDevice(		USBDevice *		inUSBDevice);

	virtual MIDIDeviceRef	CreateDevice(		USBDevice *		inUSBDevice,
												USBInterface *	inUSBInterface);

	virtual USBInterface *	CreateInterface(	USBMIDIDevice *	usbmDev);

	virtual void		StartInterface(			USBMIDIDevice *	usbmDev);
							// pipes are opened, do any extra initialization (send config msgs etc)
							
	virtual void		StopInterface(			USBMIDIDevice *	usbmDev);
							// pipes are about to be closed, do any preliminary cleanup
							
	virtual void		HandleInput(			USBMIDIDevice *	usbmDev,
												MIDITimeStamp	when,
												Byte *			readBuf,
												ByteCount 		readBufSize);
							// a USB message arrived, parse it into a MIDIPacketList and
							// call MIDIReceived

	virtual ByteCount	PrepareOutput(			USBMIDIDevice *	usbmDev,
												WriteQueue &	writeQueue,
												Byte *			destBuf);
							// dequeue from WriteQueue into a single USB message, return
};

#endif // __CasioUSBMIDI_h__
