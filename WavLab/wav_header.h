#ifndef WAV_HEADER_H
#define WAV_HEADER_H

// Got from
// https://audiocoding.ru/article/2008/05/22/wav-file-structure.html
//
// See also:
// http://soundfile.sapp.org/doc/WaveFormat/
//

#include <vector>
#include <fstream>
#include "WavExceptions.h"

enum wav_errors_e {
	WAV_OK = 0,
	IO_ERROR,
	BAD_FORMAT,
	UNSUPPORTED_FORMAT,
	BAD_PARAMS,
	DATA_SIZE_ERROR
};

// Possible header's errors
enum wav_headers_errors_e {
	HEADER_OK = 0,
	HEADER_RIFF_ERROR,
	HEADER_FILE_SIZE_ERROR,
	HEADER_WAVE_ERROR,
	HEADER_FMT_ERROR,
	HEADER_NOT_PCM,
	HEADER_SUBCHUNK1_ERROR,
	HEADER_BYTES_RATE_ERROR,
	HEADER_BLOCK_ALIGN_ERROR,
	HEADER_SUBCHUNK2_SIZE_ERROR
};

class WavFile
{
public:
	WavFile();
	WavFile(const std::string& inputFileName);
	WavFile(const std::string& inputFileName, const std::string& ouptutFileName);
	void Open(const std::string& fileName);
	wav_errors_e MakeMono();
	void print_info();
















	
	
	wav_errors_e extract_data();
	wav_errors_e make_wav_file();
	
	wav_errors_e make_reverb(int sample_rate, double delay_seconds, float decay);


	
	
	
	
	void prefill_header();

private:
	void check_header(size_t file_size_bytes);
	void null_header();
	wav_errors_e fill_header(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan);

	std::string _OutputFileName;
	struct
	{
		// WAV-формат начинается с RIFF-заголовка:

		// Содержит символы "RIFF" в ASCII кодировке
		// (0x52494646 в big-endian представлении)
		char chunkId[4];

		// 36 + subchunk2Size, или более точно:
		// 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
		// Это оставшийся размер цепочки, начиная с этой позиции.
		// Иначе говоря, это размер файла - 8, то есть,
		// исключены поля chunkId и chunkSize.
		unsigned long chunkSize;

		// Содержит символы "WAVE"
		// (0x57415645 в big-endian представлении)
		char format[4];

		// Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
		// Подцепочка "fmt " описывает формат звуковых данных:

		// Содержит символы "fmt "
		// (0x666d7420 в big-endian представлении)
		char subchunk1Id[4];

		// 16 для формата PCM.
		// Это оставшийся размер подцепочки, начиная с этой позиции.
		unsigned long subchunk1Size;

		// Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
		// Для PCM = 1 (то есть, Линейное квантование).
		// Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
		unsigned short audioFormat;

		// Количество каналов. Моно = 1, Стерео = 2 и т.д.
		unsigned short numChannels;

		// Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
		unsigned long sampleRate;

		// sampleRate * numChannels * bitsPerSample/8
		unsigned long byteRate;

		// numChannels * bitsPerSample/8
		// Количество байт для одного сэмпла, включая все каналы.
		unsigned short blockAlign;

		// Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
		unsigned short bitsPerSample;

		// Подцепочка "data" содержит аудио-данные и их размер.

		// Содержит символы "data"
		// (0x64617461 в big-endian представлении)
		char subchunk2Id[4];

		// numSamples * numChannels * bitsPerSample/8
		// Количество байт в области данных.
		unsigned long subchunk2Size;

		// Далее следуют непосредственно Wav данные.


	} _WavHeader;
	std::vector<std::vector<char>> _Channels;
};

#endif 


