#pragma once

namespace Dix
{
	class Timestep
	{
		public:
			Timestep(float time = 0.0f)
				: m_Time(time)
			{}

			operator f32() const { return m_Time; }

			f32 GetSeconds() const { return m_Time; }
			f32 GetMilliseconds() const { return m_Time * 1000.f; }

		private:
			f32 m_Time;
	};
}