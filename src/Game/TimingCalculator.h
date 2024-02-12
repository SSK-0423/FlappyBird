#pragma once

namespace FlappyBird
{
	/// <summary>
	/// �^�C�~���O���v�Z����N���X
	/// </summary>
	class TimingCalculator
	{
	public:
		TimingCalculator() = default;
		~TimingCalculator() = default;

		/// <summary>
		/// �^�C�~���O���~���b�P�ʂŌv�Z����
		/// </summary>
		/// <param name="judgeLineX">���胉�C����X���W</param>
		/// <param name="targetPosX">�^�C�~���O���Z�o����Ώۂ�X���W</param>
		/// <param name="viewportWidth">�r���[�|�[�g�̉���</param>
		/// <param name="currentTime">���݂̍Đ�����</param>
		/// <returns>targetPosX�ɂ�����^�C�~���O(�~���b)</returns>
		static float CalcTiming(float judgeLineX, float targetPosX, float viewportWidth, float currentTime);
	};
}