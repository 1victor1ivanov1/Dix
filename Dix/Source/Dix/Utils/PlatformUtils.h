#pragma once

namespace Dix
{
	class Time
	{
		public:
			Time() = delete;
			~Time() = delete;

			static f32 GetTime();
	};
}