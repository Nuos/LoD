/** @file texture_1D.hpp
    @brief Implements one-dimensional textures.
*/

#ifndef TEXUTRE_1D_HPP
#define TEXTURE_1D_HPP

#include "texture_base.hpp"

namespace oglwrap {

// -------======{[ Texture1D declaration ]}======-------

/// @brief A one dimensional texture class
/// @see GL_TEXTURE_1D
class Texture1D : public TextureBase<TexType::Tex1D> {
public:

    /// Generates an empty texture.
    /// @see glGenTextures
    Texture1D() {}

    template <TexType texture_t>
    /// Copies a 1D texture or changes the type of a texture to Texture1D
    /// @param src - The texture to copy or cast to Texture1D
    Texture1D(const TextureBase<texture_t>& src)
        :TextureBase<TexType::Tex1D>(src) {}

    /// Uploads the base image.
    /// @param internalFormat - Specifies the number, order, and size of the color components in the texture.
    /// @param width - Specifies the width of the texture image. All implementations support texture images that are at least 1024 texels wide.
    /// @param format - Specifies the format of the pixel data.
    /// @param type - Specifies the data type of the pixel data.
    /// @param data - Specifies a pointer to the image data in memory.
    /// @see glTexImage1D
    static void Upload(
        PixelDataInternalFormat internalFormat,
        GLsizei width,
        PixelDataFormat format,
        PixelDataType type,
        const void *data
    ) {
        gl( TexImage1D(
            TexType::Tex1D, 0, internalFormat, width, 0, format, type, data
        ));
    }

    /// Uploads a mipmap of the image.
    /// @param level - Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    /// @param internalFormat - Specifies the number, order, and size of the color components in the texture.
    /// @param width - Specifies the width of the texture image. All implementations support texture images that are at least 1024 texels wide.
    /// @param format - Specifies the format of the pixel data.
    /// @param type - Specifies the data type of the pixel data.
    /// @param data - Specifies a pointer to the image data in memory.
    /// @see glTexImage1D
    static void Upload_Mipmap(
        GLint level,
        PixelDataInternalFormat internalFormat,
        GLsizei width,
        PixelDataFormat format,
        PixelDataType type,
        const void *data
    ) {
        gl( TexImage1D(
            TexType::Tex1D, level, internalFormat, width, 0, format, type, data
        ));
    }

    /// Updates a part of the base image.
    /// @param xOffset - Specifies a texel offset in the x direction within the texture array.
    /// @param width - Specifies the width of the texture subimage.
    /// @param format - Specifies the format of the pixel data.
    /// @param type - Specifies the data type of the pixel data.
    /// @param data - Specifies a pointer to the image data in memory.
    /// @see glTexSubImage1D
    static void SubUpload(
        GLint xOffset,
        GLsizei width,
        PixelDataFormat format,
        PixelDataType type,
        const void *data
    ) {
        gl( TexSubImage1D(
            TexType::Tex1D, 0, xOffset, width, format, type, data
        ));
    }

    /// Updates a part of a mipmap image.
    /// @param level - Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    /// @param xOffset - Specifies a texel offset in the x direction within the texture array.
    /// @param width - Specifies the width of the texture subimage.
    /// @param format - Specifies the format of the pixel data.
    /// @param type - Specifies the data type of the pixel data.
    /// @param data - Specifies a pointer to the image data in memory.
    /// @see glTexSubImage1D
    static void SubUpload_Mipmap(
        GLint level,
        GLint xOffset,
        GLsizei width,
        PixelDataFormat format,
        PixelDataType type,
        const void *data
    ) {
        gl( TexSubImage1D(
            TexType::Tex1D, level, xOffset, width, format, type, data
        ));
    }

    /// Copies pixels from the current GL_READ_BUFFER into the base mipmap of this texture.
    /// @param internalFormat - Specifies the internal format of the texture.
    /// @param x, y - Specify the window coordinates of the left corner of the row of pixels to be copied.
    /// @param width - Specifies the number of texels to copy.
    /// @see glCopyTexImage1D
    static void Copy(
        PixelDataInternalFormat internalFormat,
        GLint x,
        GLint y,
        GLsizei width
    ) {
        gl( CopyTexImage1D(
            TexType::Tex1D, 0, internalFormat, x, y, width, 0
        ));
    }

    /// Copies pixels from the current GL_READ_BUFFER into a mipmap of this texture.
    /// @param level - Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    /// @param internalFormat - Specifies the internal format of the texture.
    /// @param x, y - Specify the window coordinates of the left corner of the row of pixels to be copied.
    /// @param width - Specifies the number of texels to copy.
    /// @see glCopyTexImage1D
    static void Copy_Mipmap(
        GLint level,
        PixelDataInternalFormat internalFormat,
        GLint x,
        GLint y,
        GLsizei width
    ) {
        gl( CopyTexImage1D(
            TexType::Tex1D, level, internalFormat, x, y, width, 0
        ));
    }

    /// Copies pixels from the current GL_READ_BUFFER and updates part of the base mipmap of this texture with them.
    /// @param xOffset - Specifies the texel offset within the destination texture array.
    /// @param x, y - Specify the window coordinates of the left corner of the row of pixels to be copied.
    /// @param width - Specifies the number of texels to copy.
    /// @see glCopyTexSubImage1D
    static void CopySub(
        GLint xOffset,
        GLint x,
        GLint y,
        GLsizei width
    ) {
        gl( CopyTexSubImage1D(
            TexType::Tex1D, 0, xOffset, x, y, width
        ));
    }

    /// Copies pixels from the current GL_READ_BUFFER and updates part of a mipmap of this texture with them.
    /// @param level - Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    /// @param xOffset - Specifies the texel offset within the destination texture array.
    /// @param x, y - Specify the window coordinates of the left corner of the row of pixels to be copied.
    /// @param width - Specifies the number of texels to copy.
    /// @see glCopyTexSubImage1D
    static void CopySub_Mipmap(
        GLint level,
        GLint xOffset,
        GLint x,
        GLint y,
        GLsizei width
    ) {
        gl( CopyTexSubImage1D(
            TexType::Tex1D, level, xOffset, x, y, width
        ));
    }

    /// Returns the width of a mipmap of the currently bound texture of this class.
    /// @param level - Specifies the mipmap whose size should be queried.
    /// @see glGetTexLevelParameteriv, GL_TEXTURE_WIDTH
    static GLsizei Width(GLint level = 0)  {
        GLsizei w;
        gl( GetTexLevelParameteriv(TexType::Tex1D, level, GL_TEXTURE_WIDTH, &w) );
        return w;
    }

    /// Return a compressed texture image
    /// @param level - Specifies the level-of-detail number of the desired image. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    /// @param img - Returns the compressed texture image.
    /// @see glGetCompressedTexImage
    static void GetCompressedImage(GLint level, GLvoid* img) {
        gl( GetCompressedTexImage(TexType::Tex1D, level, img) );
    }
};

} // namespace oglwrap

#endif // TEXTURE_1D
