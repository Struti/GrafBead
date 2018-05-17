#pragma once

namespace ti {

	class CalculateTime
	{
	public:
		static double getFrameDelta();
		static double getElapsedTime();

		static void init();
		static void calculateFrameTime();

	private:
		CalculateTime();
		~CalculateTime();
	};

}