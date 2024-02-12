#pragma once
#include "IComponent.h"
#include <DirectXMath.h>

namespace Framework
{
	class SpriteRenderer;
	class Transform2D;

	/// <summary>
	/// �{�^���R���|�[�l���g
	/// </summary>
	class Button : public IComponent
	{
	public:
		Button(std::shared_ptr<Object> owner);
		~Button() = default;

	private:
		SpriteRenderer* m_spriteRenderer = nullptr;
		std::unique_ptr<class Text> m_text = nullptr;
		std::function<void()> m_onClick = nullptr;	// �N���b�N���̃R�[���o�b�N�֐�

		bool CheckClick();
	public:
		void Update(float deltaTime) override;
		void Draw() override;
		void SetPosition(const DirectX::XMFLOAT2& position);
		void SetScale(const DirectX::XMFLOAT2& scale);
		void SetTexture(const std::wstring& path);
		void SetText(const std::wstring& text);
		void SetLayer(int layer);
		
		/// <summary>
		/// �{�^�����N���b�N���ꂽ�Ƃ��̃R�[���o�b�N�֐���ݒ肷��
		/// </summary>
		/// <param name="onClick">�R�[���o�b�N�֐�</param>
		void SetOnClick(const std::function<void()>& onClick);
	};
}