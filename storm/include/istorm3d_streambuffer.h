#ifndef INCLUDED_ISTORM3D_STREAM_BUFFER_H
#define INCLUDED_ISTORM3D_STREAM_BUFFER_H

#include <memory>
#include <stdint.h>

class IStorm3D_Stream
{
public:
	virtual ~IStorm3D_Stream() {}

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	// channels * bits
	virtual void addSample(const char *buffer, int length, uint64_t start, uint64_t duration) = 0;
	virtual uint64_t getCurrentTime() const = 0;
};

class IStorm3D_StreamBuilder
{
public:
	virtual ~IStorm3D_StreamBuilder() {}

	virtual void update() = 0;

	virtual void setStereo(bool stereo) = 0;
	virtual void setFrequency(int data) = 0;
	virtual void setBits(int bits) = 0;
	virtual std::shared_ptr<IStorm3D_Stream> getStream() = 0;
};


#endif
