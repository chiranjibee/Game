#include <windows.h>

#include "Sound.h"

CSound &CSound::GetInstance()
{
	static CSound Instance;
	return Instance;
}

CSound::CSound()
	:	DirectSound(nullptr),
		PrimaryBuffer(nullptr),
		SecondaryBuffer1(nullptr)
{
}

CSound::~CSound()
{
	Shutdown();
}

bool CSound::Initialize(HWND Wnd)
{
	// Initialize direct sound and the primary sound buffer
	return InitializeDirectSound(Wnd);
}

void CSound::Shutdown()
{
	// Release the secondary buffers
	for (auto S : Sounds) 
	{
		if(S.second )
		{
			S.second->Release();
		}		
	}

	// Shutdown the Direct Sound API
	ShutdownDirectSound();
	Sounds.clear();
	return;
}

bool CSound::InitializeDirectSound(HWND Wnd)
{
	HRESULT Res;
	DSBUFFERDESC BufferDesc;
	WAVEFORMATEX WaveFormat;

	// Initialize the direct sound interface pointer for the default sound device
	Res = DirectSoundCreate8(nullptr, &DirectSound, nullptr);
	if(FAILED(Res))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified
	Res = DirectSound->SetCooperativeLevel(Wnd, DSSCL_PRIORITY);
	if(FAILED(Res))
	{
		return false;
	}

	// Setup the primary buffer description
	BufferDesc.dwSize = sizeof(DSBUFFERDESC);
	BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	BufferDesc.dwBufferBytes = 0;
	BufferDesc.dwReserved = 0;
	BufferDesc.lpwfxFormat = nullptr;
	BufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device
	Res = DirectSound->CreateSoundBuffer(&BufferDesc, &PrimaryBuffer, nullptr);
	if(FAILED(Res))
	{
		return false;
	}

	// Setup the format of the primary sound bufffer
	WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	WaveFormat.nSamplesPerSec = 44100;
	WaveFormat.wBitsPerSample = 16;
	WaveFormat.nChannels = 2;
	WaveFormat.nBlockAlign = (WaveFormat.wBitsPerSample / 8) * WaveFormat.nChannels;
	WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
	WaveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified
	Res = PrimaryBuffer->SetFormat(&WaveFormat);
	if(FAILED(Res))
	{
		return false;
	}

	return true;
}


void CSound::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer
	if(PrimaryBuffer)
	{
		PrimaryBuffer->Release();
		PrimaryBuffer = 0;
	}

	// Release the direct sound interface pointer
	if(DirectSound)
	{
		DirectSound->Release();
		DirectSound = 0;
	}

	return;
}

bool CSound::PlaySound(const char* Filename, DWORD Flags)
{
	HRESULT Res;

	// Set position at the beginning of the sound buffer
	//result = m_secondaryBuffer1->SetCurrentPosition(0);
	if (Sounds[Filename] == nullptr)
	{
		Res = LoadWaveFile(Filename);
		if (FAILED(Res))
		{
			return false;
		}
	}

	if (Sounds[Filename])
	{
		Res = Sounds[Filename]->SetCurrentPosition(0);
		if (FAILED(Res))
		{
			return false;
		}

		// Set volume of the buffer to 100%
		Res = Sounds[Filename]->SetVolume(DSBVOLUME_MAX);
		if (FAILED(Res))
		{
			return false;
		}

		// Play the contents of the secondary sound buffer
		Res = Sounds[Filename]->Play(0, 0, Flags);
		if (FAILED(Res))
		{
			return false;
		}
		return true;
	}
	return false;
}


bool CSound::IsPlaying(const char* Filename)
{
	if (Sounds[Filename] != nullptr)
	{
		DWORD DwStatus;
		HRESULT Res = Sounds[Filename]->GetStatus(&DwStatus);
		if (!FAILED(Res))
		{
			return (DwStatus & DSBSTATUS_PLAYING) != 0;
		}			
	}
	return false;
}

bool CSound::StopSound(const char* Filename)
{
	if (IsPlaying(Filename) )
	{
		if (Sounds[Filename] != nullptr)
		{
			HRESULT Res = Sounds[Filename]->Stop();
			return (FAILED(Res));
		}
	}
	return false;
}

bool CSound::LoadWaveFile(const char* Filename)
{	
	int Err;
	FILE* FilePtr = nullptr;
	unsigned int Count;
	WaveHeaderType WaveFileHeader;
	WAVEFORMATEX WaveFormat;
	DSBUFFERDESC BufferDesc;
	HRESULT Res;
	IDirectSoundBuffer* TempBuffer;
	unsigned char* WaveData;
	unsigned char* BufferPtr;
	unsigned long BufferSize;

	// Find entry in map
	IDirectSoundBuffer8** SecondaryBuffer = &Sounds[Filename];

	// Open the wave file in binary
	Err = fopen_s(&FilePtr, Filename, "rb");
	if (Err != 0)
	{
		return false;
	}

	// Read in the wave file header
	Count = static_cast<unsigned int>(fread(&WaveFileHeader, sizeof(WaveFileHeader), 1, FilePtr));
	if (Count != 1)
	{
		return false;
	}

	// Check that the chunk ID is the RIFF format
	if ((WaveFileHeader.ChunkId[0] != 'R') || (WaveFileHeader.ChunkId[1] != 'I') ||
		(WaveFileHeader.ChunkId[2] != 'F') || (WaveFileHeader.ChunkId[3] != 'F'))
	{
		return false;
	}

	// Check that the file format is the WAVE format
	if ((WaveFileHeader.Format[0] != 'W') || (WaveFileHeader.Format[1] != 'A') ||
		(WaveFileHeader.Format[2] != 'V') || (WaveFileHeader.Format[3] != 'E'))
	{
		return false;
	}

	// Check that the sub chunk ID is the fmt format
	if ((WaveFileHeader.SubChunkId[0] != 'f') || (WaveFileHeader.SubChunkId[1] != 'm') ||
		(WaveFileHeader.SubChunkId[2] != 't') || (WaveFileHeader.SubChunkId[3] != ' '))
	{
		return false;
	}

	// Check for the data chunk header
	if ((WaveFileHeader.DataChunkId[0] != 'd') || (WaveFileHeader.DataChunkId[1] != 'a') ||
		(WaveFileHeader.DataChunkId[2] != 't') || (WaveFileHeader.DataChunkId[3] != 'a'))
	{
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto
	WaveFormat.wFormatTag = WaveFileHeader.AudioFormat;
	WaveFormat.nSamplesPerSec = WaveFileHeader.SampleRate;
	WaveFormat.wBitsPerSample = WaveFileHeader.BitsPerSample;
	WaveFormat.nChannels = WaveFileHeader.NumChannels;
	WaveFormat.nBlockAlign = (WaveFormat.wBitsPerSample / 8) * WaveFormat.nChannels;
	WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
	WaveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto
	BufferDesc.dwSize = sizeof(DSBUFFERDESC);
	BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	BufferDesc.dwBufferBytes = WaveFileHeader.DataSize;
	BufferDesc.dwReserved = 0;
	BufferDesc.lpwfxFormat = &WaveFormat;
	BufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings
	Res = DirectSound->CreateSoundBuffer(&BufferDesc, &TempBuffer, nullptr);
	if (FAILED(Res))
	{
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer
	Res = TempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*SecondaryBuffer);
	if (FAILED(Res))
	{
		return false;
	}

	// Release the temporary buffer
	TempBuffer->Release();
	TempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header
	fseek(FilePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data
	WaveData = new unsigned char[WaveFileHeader.DataSize];
	if (!WaveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer
	Count = static_cast<unsigned int>(fread(WaveData, 1, WaveFileHeader.DataSize, FilePtr));
	if (Count != WaveFileHeader.DataSize)
	{
		return false;
	}

	// Close the file once done reading
	Err = fclose(FilePtr);
	if (Err != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write wave data into it
	Res = (*SecondaryBuffer)->Lock(0, WaveFileHeader.DataSize, (void**)&BufferPtr, (DWORD*)&BufferSize, nullptr, nullptr, 0);
	if (FAILED(Res))
	{
		return false;
	}

	// Copy the wave data into the buffer
	memcpy(BufferPtr, WaveData, WaveFileHeader.DataSize);

	// Unlock the secondary buffer after the data has been written to it
	Res = (*SecondaryBuffer)->Unlock((void*)BufferPtr, BufferSize, nullptr, 0);
	if (FAILED(Res))
	{
		return false;
	}

	// Release the wave data since it was copied into the secondary buffer
	delete[] WaveData;
	WaveData = nullptr;

	return true;
}