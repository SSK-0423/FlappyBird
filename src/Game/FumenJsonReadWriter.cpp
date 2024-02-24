#include "pch.h"
#include "FumenJsonReadWriter.h"
#include "Framework/Editor.h"

using namespace Framework;

namespace FlappyBird
{
	void FumenJsonReadWriter::Write(const std::string& filePath, const FumenData& data)
	{
		picojson::object rootObject;

		// 譜面の基本情報
		picojson::object musicInfo;
		musicInfo.insert(std::make_pair("musicName", picojson::value(data.musicName)));
		musicInfo.insert(std::make_pair("musicFilePath", picojson::value(data.musicFilePath)));
		musicInfo.insert(std::make_pair("bpm", picojson::value(data.bpm)));
		musicInfo.insert(std::make_pair("beat", picojson::value(static_cast<double>(data.beat))));
		rootObject.insert(std::make_pair("MusicInfo", picojson::value(musicInfo)));

		// ノーツの情報
		picojson::array notesArray;
		for (auto& data : data.noteDatas)
		{
			picojson::object noteObject;
			noteObject.insert(std::make_pair("time", picojson::value(data.timing)));
			noteObject.insert(std::make_pair("posY", picojson::value(data.posY)));
			noteObject.insert(std::make_pair("spaceOffset", picojson::value(data.spaceOffset)));
			notesArray.push_back(picojson::value(noteObject));
		}
		rootObject.insert(std::make_pair("notes", picojson::value(notesArray)));

		// Jsonファイルに書き込む
		std::ofstream ofs(filePath);
		ofs << picojson::value(rootObject).serialize(true);
		ofs.close();
	}
	void FumenJsonReadWriter::Read(const std::string& filePath, FumenData& data)
	{
		std::ifstream ifs(filePath);

		if (ifs.fail())
		{
			Editor::DebugLog("Failed to open file: %s", filePath.c_str());
			return;
		}

		picojson::value fumen;

		ifs >> fumen;
		ifs.close();

		// 譜面の基本情報
		data.musicName = fumen.get("MusicInfo").get("musicName").get<std::string>();
		data.musicFilePath = fumen.get("MusicInfo").get("musicFilePath").get<std::string>();
		data.bpm = fumen.get("MusicInfo").get("bpm").get<double>();
		data.beat = static_cast<int>(fumen.get("MusicInfo").get("beat").get<double>());

		// ノーツの情報
		// 既存のノーツデータをクリア
		data.noteDatas.clear();
		data.noteDatas.shrink_to_fit();
		// ノーツデータを読み込み
		picojson::array notesArray = fumen.get("notes").get<picojson::array>();
		for (auto& note : notesArray)
		{
			NoteData noteData;
			noteData.timing = note.get("time").get<double>();
			noteData.posY = static_cast<float>(note.get("posY").get<double>());
			noteData.spaceOffset = static_cast<float>(note.get("spaceOffset").get<double>());
			data.noteDatas.push_back(noteData);
		}
	}
}