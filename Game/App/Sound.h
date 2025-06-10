#ifndef GE_SOUND_H
#define GE_SOUND_H

#include <dsound.h>
#include <map>

class CSound
{
private:
	struct WaveHeaderType
	{
		char			ChunkId[4];
		unsigned long	ChunkSize;
		char			Format[4];
		char			SubChunkId[4];
		unsigned long	SubChunkSize;
		unsigned short	AudioFormat;
		unsigned short	NumChannels;
		unsigned long	SampleRate;
		unsigned long	BytesPerSecond;
		unsigned short	BlockAlign;
		unsigned short	BitsPerSample;
		char			DataChunkId[4];
		unsigned long	DataSize;
	};

public:
	static CSound& GetInstance();
	CSound();	
	~CSound();

	bool Initialize(HWND Wnd);
	void Shutdown();
	
	bool PlaySound(const char* Filename, DWORD Flags = 0);
	bool StopSound(const char* Filename);
	bool IsPlaying(const char* Filename);
private:
	bool InitializeDirectSound(HWND Wnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(const char* Filename);
	
private:
	IDirectSound8* DirectSound;
	IDirectSoundBuffer* PrimaryBuffer;
	IDirectSoundBuffer8* SecondaryBuffer1;
	std::map<const char*, IDirectSoundBuffer8*> Sounds;
};

#endif