#pragma once

#include <vector>
#include <functional>

namespace infinitashook
{
	template <typename callback_t> class base_hook
	{
		public:
			virtual void install() = 0;
			virtual void uninstall() {};

			void add_callback(callback_t* callback)
			{
				_callbacks.push_back(callback);
			};

			void remove_callback(callback_t* callback)
			{
				_callbacks.erase(std::remove(_callbacks.begin(), _callbacks.end(), callback), _callbacks.end());
			}

			template<typename... Args> void invoke_callbacks(Args... args)
			{
				for (callback_t* callback: _callbacks)
				{
					std::invoke(callback, args...);
				}
			}
		private:
			std::vector<callback_t*> _callbacks;
	};
};