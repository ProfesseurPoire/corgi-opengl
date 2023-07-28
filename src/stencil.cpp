#include <corgi/opengl/stencil.h>
#include <glad/glad.h>

namespace corgi
{
    static GLenum convert_stencil_operation(stencil_operation v)
    {
        switch(v)
        {
            case stencil_operation::keep:
                return GL_KEEP;
            case stencil_operation::replace:
                return  GL_REPLACE;
        }
    }

	void stencil::apply(stencil previous)
	{
        if (enable_stencil != previous.enable_stencil)
            enable_stencil? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);

        if(stencil_mask_ != previous.stencil_mask_)
            glStencilMask(stencil_mask_);

        if(test != previous.test || stencil_value != previous.stencil_value)
        {
            switch(test)
            {
                case stencil_test::always:
                    glStencilFunc(
                        GL_ALWAYS, stencil_value,
                        0xFF);    // Means we always write in the stencil test
                    break;

                case stencil_test::not_equal:
                    glStencilFunc(GL_NOTEQUAL, stencil_value,
                                  0xFF);
                    break;

                case stencil_test::equal:
                    glStencilFunc(GL_EQUAL, stencil_value, 0xFF);
                    break;
            }
        }

        if(fail != previous.fail || success_depth_fail!= previous.success_depth_fail || success_depth_success!= previous.success_depth_success)
            glStencilOp(convert_stencil_operation(fail),
                    convert_stencil_operation(success_depth_fail),
                    convert_stencil_operation(success_depth_success));

        // I think it's supposed to limit what we can write on the stencil part of the buffer?
        //glStencilMask(0xFF);

	}
}