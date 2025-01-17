#include "GameSound.h"

#include <windows.h>
#include <cassert>

#include <fstream>

#pragma comment(lib,"xaudio2.lib")

GameSound* GameSound::GetInstance()
{
	static GameSound instance;

	return &instance;
}

void GameSound::Initialize(const std::string& directoryPath)
{
	directoryPath_ = directoryPath;

	HRESULT result;

	IXAudio2MasteringVoice* masterVoice;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

void GameSound::Finalize()
{
	// XAudio2解放
	xAudio2_.Reset();
	// 音声データ解放
	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();

	for (; it != soundDatas_.end(); ++it) {
		Unload(&it->second);
	}
	soundDatas_.clear();
}

void GameSound::LoadWave(const std::string& filename)
{
	if (soundDatas_.find(filename) != soundDatas_.end()) {
		//重複読み込みは何もしないで
		return;
	}

	//ディレクトリパスとファイル名連結　フルパス得る
	std::string fullpath = directoryPath_ + filename;

	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(fullpath, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	// returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//サウンドデータを連想配列に格納
	soundDatas_.insert(std::make_pair(filename, soundData));
}

void GameSound::Unload(SoundData* soundData)
{
	// バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void GameSound::PlayWave(const std::string& filename, float volumecont, int loopCount)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	//未読み込み
	assert(it != soundDatas_.end());

	SoundData& soundData = it->second;

	//再生フラグ
	if (soundData.playWaveFlag == false)
	{
		soundData.playWaveFlag = true;
	}

	// 波形フォーマットを元にSourceVoiceの生成
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = loopCount;

	// 波形データの再生
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->SetVolume(volumecont);
	result = soundData.pSourceVoice->Start();

	//再生確認フラグの確認
	if (soundData.playWaveFlag == false)
	{
		soundData.playWaveFlag = true;
	}
}

void GameSound::SoundStop(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	//未読み込みの検出
	assert(it != soundDatas_.end());
	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	if (soundData.playWaveFlag == false)
	{
		return;
	}

	soundData.pSourceVoice->Stop(0);
	soundData.pSourceVoice->FlushSourceBuffers();
	soundData.pSourceVoice->SubmitSourceBuffer(&buf);

	soundData.playWaveFlag = false;
}