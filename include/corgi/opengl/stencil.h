#pragma once

namespace corgi
{

enum class stencil_test : char
{
    always,
    not_equal,
    equal
};

enum class stencil_operation : char
{
    replace = 0b00000000,
    keep    = 0b00000001
};

    // Overall the idea is that we write in a different buffer
    // If the stencil buffer is on, we don't write pixels but in the stencil
    // And we can later use this do determine if we want to draw a pixel or not
    // Try to make an exemple of stencil usage so it's less weird/ POC 
	class stencil
	{
        public:

			bool enable_stencil {false};
            stencil_test      test {stencil_test::always};

            stencil_operation success_depth_success {stencil_operation::replace};
            stencil_operation success_depth_fail {stencil_operation::replace};
            stencil_operation fail {stencil_operation::replace};

            int               stencil_value {0};

            // If 0, we don't write in the stencil buffer at all
            // If 1 we write in the stencil buffer
            int stencil_mask_ = 0xFF;

			void apply(stencil previous_stencil);

		private:
	};
}