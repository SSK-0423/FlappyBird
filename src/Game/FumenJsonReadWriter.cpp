#include "pch.h"
#include "FumenJsonReadWriter.h"
#include "Framework/Editor.h"

using namespace Framework;

namespace FlappyBird
{
	void FumenJsonReadWriter::Write(const std::string& filePath, FumenData& data)
	{
		picojson::object rootObject;

		// 譜面の基本情報
		picojson::object musicInfo;
		musicInfo.insert(std::make_pair("musicName", picojson::value(data.musicName)));
		musicInfo.insert(std::make_pair("musicFilePath", picojson::value(data.musicFilePath)));
		musicInfo.insert(std::make_pair("bpm", picojson::value(data.bpm)));
		musicInfo.insert(std::make_pair("beat", picojson::value(static_cast<double>(data.beat))));
		rootObject.insert(std::make_pair("MusicInfo", picojson::value(musicInfo)));

		// 障害物(土管)の配置を示すノーツの情報
		// 判定タイミングが早い順にソート
		std::sort(data.noteDatas.begin(), data.noteDatas.end(), 
			[](const NoteData& l, const NoteData& r) {return l.timing < r.timing; });
		picojson::array notesArray;
		for (auto& data : data.noteDatas)
		{
			picojson::object noteObject;
			noteObject.insert(std::make_pair("type", picojson::value(static_cast<double>(data.type))));	// ノーツの種類
			noteObject.insert(std::make_pair("time", picojson::value(data.timing)));	                // タイミング
			noteObject.insert(std::make_pair("posY", picojson::value(data.posY)));	                    // Y座標
			noteObject.insert(std::make_pair("spaceOffset", picojson::value(data.spaceOffset)));	    // 土管の上下間のスペースを基準からずらす量
			notesArray.push_back(picojson::value(noteObject));
		}
		rootObject.insert(std::make_pair("notes", picojson::value(notesArray)));

		// 隠しノーツの情報
		// 判定タイミングが早い順にソート
		std::sort(data.hiddenNoteDatas.begin(), data.hiddenNoteDatas.end(), 
			[](const HiddenNoteData& l, const HiddenNoteData& r) {return l.timing < r.timing; });
		picojson::array hiddenNotesArray;
		for (auto& data : data.hiddenNoteDatas)
		{
			picojson::object noteObject;
			noteObject.insert(std::make_pair("type", picojson::value(static_cast<double>(data.type))));	// ノーツの種類
			noteObject.insert(std::make_pair("time", picojson::value(data.timing)));	                // タイミング
			hiddenNotesArray.push_back(picojson::value(noteObject));
		}
		rootObject.insert(std::make_pair("hiddenNotes", picojson::value(hiddenNotesArray)));

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
		// 既存の障害物(土管)のデータをクリア
		data.noteDatas.clear();
		data.noteDatas.shrink_to_fit();

		// 障害物(土管)の配置を示すノーツの情報を読み込む
		picojson::array notesArray = fumen.get("notes").get<picojson::array>();
		for (auto& note : notesArray)
		{
			NoteData noteData;
			noteData.timing = note.get("time").get<double>();
			noteData.posY = static_cast<float>(note.get("posY").get<double>());
			noteData.spaceOffset = static_cast<float>(note.get("spaceOffset").get<double>());
			data.noteDatas.push_back(noteData);
		}

		// 既存の隠しノーツのデータをクリア
		data.hiddenNoteDatas.clear();
		data.hiddenNoteDatas.shrink_to_fit();

		// 隠しノーツの情報を読み込む
		picojson::array hiddenNotesArray = fumen.get("hiddenNotes").get<picojson::array>();
		for (auto& note : hiddenNotesArray)
		{
			HiddenNoteData hiddenNoteData;
			hiddenNoteData.timing = note.get("time").get<double>();
			data.hiddenNoteDatas.push_back(hiddenNoteData);
		}
	}
}