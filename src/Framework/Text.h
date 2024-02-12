#pragma once
#include "IComponent.h"
#include "DX12Wrapper/FontRenderer.h"

#include <DirectXMath.h>

namespace Framework
{
	/// <summary>
	/// �e�L�X�g��`�悷��R���|�[�l���g
	/// </summary>
	class Text : public IComponent
	{
	public:
		Text(std::shared_ptr<Object> owner);
		~Text();

		void Update(float deltaTime) override;
		void Draw() override;
		void DrawInspector() override;

		void SetText(const std::wstring& text);
		void SetColor(const DirectX::XMVECTORF32& color);
		void SetPosition(const DirectX::XMFLOAT2& position);

		/// <summary>
		/// �e�L�X�g�̑傫����ݒ肷��
		/// �s�N�Z���P�ʂł͂Ȃ��̂ŁA1.0f����ɑ傫����ݒ肷�邱��
		/// </summary>
		/// <param name="scale">�X�P�[��</param>
		void SetScale(const float& scale);

		void SetRotation(float angle);

	private:
		std::wstring m_text;
		DirectX::XMVECTORF32 m_color;
	};
}