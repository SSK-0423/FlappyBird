#include "pch.h"
#include "NotesManager.h"

namespace FlappyBird
{
	NotesManager::NotesManager(Framework::Object* owner) : IComponent(owner)
	{
	}
	NotesManager::~NotesManager()
	{
	}
	void NotesManager::Update(float deltaTime)
	{
	}
	void NotesManager::Draw()
	{
	}
	void NotesManager::CreateNotes(float x, float y, Note data)
	{
		// 指定位置にノーツを生成
	}
	void NotesManager::DeleteNotes(Note data)
	{
	}
}