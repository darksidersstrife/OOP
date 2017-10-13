#include "wav_header.h"
#include <iostream>

using namespace std;



void WavFile::Open(const string& FileName)
{
	ifstream file(FileName);
	file.exceptions(ios::badbit);
	file.read((char *)&_WavHeader, sizeof(_WavHeader));

	file.seekg(0, ifstream::end);
	check_header(file.tellg());
	file.seekg(sizeof(_WavHeader));

	size_t chan_size = _WavHeader.subchunk2Size / _WavHeader.numChannels;
	_Channels.resize(_WavHeader.numChannels);

	for (int ch = 0; ch < _WavHeader.numChannels; ch++)
	{
		_Channels[ch].resize(chan_size);
		file.read(_Channels[ch].data(), chan_size);
	}
}



WavFile::WavFile(const std::string& inputFileName)
{
	Open(inputFileName);
}

WavFile::WavFile(const std::string& inputFileName, const std::string& outputFileName) : _OutputFileName(outputFileName)
{
	Open(inputFileName);
}

void WavFile::print_info()
{
	cout << "-------------------------\n";
	cout << " audioFormat   " << _WavHeader.audioFormat << endl;
	cout << " numChannels   " << _WavHeader.numChannels << endl;
	cout << " sampleRate    " << _WavHeader.sampleRate << endl;
	cout << " bitsPerSample " << _WavHeader.bitsPerSample << endl;
	cout << " byteRate      " << _WavHeader.byteRate << endl;
	cout << " blockAlign    " << _WavHeader.blockAlign << endl;
	cout << " chunkSize     " << _WavHeader.chunkSize << endl;
	cout << " subchunk1Size " << _WavHeader.subchunk1Size << endl;
	cout << " subchunk2Size " << _WavHeader.subchunk2Size << endl;
	cout << "-------------------------\n";
}





void WavFile::check_header(size_t file_size_bytes)
{
	if (_WavHeader.chunkId[0] != 'R' ||
		_WavHeader.chunkId[1] != 'I' ||
		_WavHeader.chunkId[2] != 'F' ||
		_WavHeader.chunkId[3] != 'F')
	{
		throw WavHeaderException("File format id is not RIFF");
	}

	if (_WavHeader.chunkSize != file_size_bytes - 8) 
	{
		throw WavHeaderException("Size of file doesn't match with header values");
	}

	if (_WavHeader.format[0] != 'w' ||
		_WavHeader.format[1] != 'a' ||
		_WavHeader.format[2] != 'v' ||
		_WavHeader.format[3] != 'e')
	{
		throw WavHeaderException("File format is not wave");
	}

	if (_WavHeader.subchunk1Id[0] != 'f' ||
		_WavHeader.subchunk1Id[1] != 'm' ||
		_WavHeader.subchunk1Id[2] != 't' ||
		_WavHeader.subchunk1Id[3] != ' ')
	{
		throw WavHeaderException("Chunk with format values id is not fmt");
	}

	if (_WavHeader.audioFormat != 1) 
	{
		throw WavHeaderException("Only pcm is supported");
	}

	if (_WavHeader.byteRate != _WavHeader.sampleRate * _WavHeader.numChannels * _WavHeader.bitsPerSample / 8)
	{
		throw WavHeaderException("Header data is invalid");
	}

	if (_WavHeader.blockAlign != _WavHeader.numChannels * _WavHeader.bitsPerSample / 8) 
	{
		throw WavHeaderException("Header data is invalid");
	}

	if (_WavHeader.subchunk2Id[0] != 'd' ||
		_WavHeader.subchunk2Id[1] != 'a' ||
		_WavHeader.subchunk2Id[2] != 't' ||
		_WavHeader.subchunk2Id[3] != 'a')
	{
		throw WavHeaderException("Data chunk id is not data");
	}

	if (_WavHeader.subchunk2Size != file_size_bytes - 44)
	{
		throw WavHeaderException("Size of file doesn't match with header values");
	}
}

void WavFile::prefill_header()
{
	_WavHeader.chunkId[0] = 0x52;
	_WavHeader.chunkId[1] = 0x49;
	_WavHeader.chunkId[2] = 0x46;
	_WavHeader.chunkId[3] = 0x46;

	_WavHeader.format[0] = 0x57;
	_WavHeader.format[1] = 0x41;
	_WavHeader.format[2] = 0x56;
	_WavHeader.format[3] = 0x45;

	_WavHeader.subchunk1Id[0] = 0x66;
	_WavHeader.subchunk1Id[1] = 0x6d;
	_WavHeader.subchunk1Id[2] = 0x74;
	_WavHeader.subchunk1Id[3] = 0x20;

	_WavHeader.subchunk2Id[0] = 0x64;
	_WavHeader.subchunk2Id[1] = 0x61;
	_WavHeader.subchunk2Id[2] = 0x74;
	_WavHeader.subchunk2Id[3] = 0x61;

	_WavHeader.audioFormat = 1;
	_WavHeader.subchunk1Size = 16;
	_WavHeader.bitsPerSample = 16;

}

wav_errors_e WavFile::fill_header(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan)
{
	if (bits_per_sample != 16) {
		return UNSUPPORTED_FORMAT;
	}

	if (chan_count < 1) {
		return BAD_PARAMS;
	}
	prefill_header();

	int file_size_bytes = 44 + chan_count * (bits_per_sample / 8) * samples_count_per_chan;

	_WavHeader.sampleRate = sample_rate;
	_WavHeader.numChannels = chan_count;
	_WavHeader.bitsPerSample = 16;

	_WavHeader.chunkSize = file_size_bytes - 8;
	_WavHeader.subchunk2Size = file_size_bytes - 44;

	_WavHeader.byteRate = _WavHeader.sampleRate * _WavHeader.numChannels * _WavHeader.bitsPerSample / 8;
	_WavHeader.blockAlign = _WavHeader.numChannels * _WavHeader.bitsPerSample / 8;

	return WAV_OK;
}

wav_errors_e WavFile::make_wav_file(const char* filename, int sample_rate) const
{
	printf(">>>> make_wav_file( %s )\n", filename);
	wav_errors_e err;

	int chan_count = (int)_Ñhannels.size();

	if (chan_count < 1) {
		return BAD_PARAMS;
	}

	int samples_count_per_chan = (int)_Ñhannels[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (_Ñhannels[ch].size() != (size_t)samples_count_per_chan) {
			return BAD_PARAMS;
		}
	}

	err = fill_header(chan_count, 16, sample_rate, samples_count_per_chan);
	if (err != WAV_OK) {
		return err;
	}

	std::vector<short> all_channels;
	all_channels.resize(chan_count * samples_count_per_chan);

	for (int ch = 0; ch < chan_count; ch++) {
		const std::vector<short>& chdata = _Ñhannels[ch];
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			all_channels[chan_count * i + ch] = chdata[i];
		}
	}

	FILE* f = fopen(filename, "wb");
	fwrite(&_WavHeader, sizeof(_WavHeader), 1, f);
	fwrite(all_channels.data(), sizeof(short), all_channels.size(), f);
	if (!f) {
		return IO_ERROR;
	}

	fclose(f);

	return WAV_OK;
}

/*void WavFile::null_header()
{
	memset(this, 0, sizeof(_WavHeader));
}*/

void WavFile::MakeMono()
{
	int chan_count = (int)source.size();

	if (chan_count != 2) {
		return BAD_PARAMS;
	}

	int samples_count_per_chan = (int)source[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (source[ch].size() != (size_t)samples_count_per_chan) {
			return BAD_PARAMS;
		}
	}

	dest_mono.resize(1);
	std::vector<short>& mono = dest_mono[0];
	mono.resize(samples_count_per_chan);

	// Mono channel is an arithmetic mean of all (two) channels.
	for (size_t i = 0; i < samples_count_per_chan; i++) {
		mono[i] = (source[0][i] + source[1][i]) / 2;
	}

	return WAV_OK;
}

wav_errors_e WavFile::make_reverb(std::vector<std::vector<short> > &sounds, int sample_rate, double delay_seconds, float decay)
{
	int chan_count = (int)sounds.size();

	if (chan_count < 1) {
		return BAD_PARAMS;
	}

	int samples_count_per_chan = (int)sounds[0].size();

	// Verify that all channels have the same number of samples.
	for (size_t ch = 0; ch < chan_count; ch++) {
		if (sounds[ch].size() != (size_t)samples_count_per_chan) {
			return BAD_PARAMS;
		}
	}

	int delay_samples = (int)(delay_seconds * sample_rate);


	for (size_t ch = 0; ch < chan_count; ch++) {
		std::vector<float> tmp;
		tmp.resize(sounds[ch].size());

		// Convert signal from short to float
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			tmp[i] = sounds[ch][i];
		}

		// Add a reverb
		for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
			tmp[i + delay_samples] += decay * tmp[i];
		}

		// Find maximum signal's magnitude
		float max_magnitude = 0.0f;
		for (size_t i = 0; i < samples_count_per_chan - delay_samples; i++) {
			if (abs(tmp[i]) > max_magnitude) {
				max_magnitude = abs(tmp[i]);
			}
		}

		// Signed short can keep values from -32768 to +32767,
		// After reverb, usually there are values large 32000.
		// So we must scale all values back to [ -32768 ... 32768 ]
		float norm_coef = 30000.0f / max_magnitude;
		printf("max_magnitude = %.1f, coef = %.3f\n", max_magnitude, norm_coef);

		// Scale back and transform floats to shorts.
		for (size_t i = 0; i < samples_count_per_chan; i++) {
			sounds[ch][i] = (short)(norm_coef * tmp[i]);
		}
	}

	return WAV_OK;
}
