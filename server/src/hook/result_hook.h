#pragma once

#include "../core/bm2dx/bm2dx.h"
#include "base_hook.h"

namespace infinitashook
{
	class result_hook: public base_hook<void (bm2dx::StageResultDrawFrame*)>
	{
		public:
			void install();
	};
}