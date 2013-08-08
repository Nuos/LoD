#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include <string>
#include <GL/glew.h>

#include "error.hpp"
#include "enums.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace oglwrap {

// -------======{[ UniformObject ]}======-------

template<class GLtype>
/// An object implementing the base features for the Uniform and
/// LazyUniform classes. You shouldn't use this class directly.
class UniformObject {
protected:
    GLuint location; ///< The C handle for the uniform's location

    /// The handle defaults to INVALID_LOCATION
    UniformObject()
        : location(INVALID_LOCATION) { }

    /// Sets the location handle.
    UniformObject(GLuint location)
        : location(location) { }

    /// Sets the uniform to a GLtype variable's value. It finds the appropriate glUniform*
    /// using template specialization. If it is called with not an OpenGL type, it throws
    /// std::invalid argument.
    /// @see glUniform*
    void Set(const GLtype& value) { // See the specializations at the end of this file.
        throw std::invalid_argument("Trying to set a uniform to a value that is not an OpenGL type.");
    }

    /// Sets the uniform to 'value', via the Set() function.
    /// @see Set
    void operator=(const GLtype& value) {
        Set(value);
    }

public:
    /// Returns the C OpenGL handle for the uniform's location.
    GLuint Expose() const {
        return location;
    }
};

// -------======{[ Uniform ]}======-------

template<typename GLtype>
/// Uniform is used to set a uniform variable's value in a specified program.
/// It queries the location of the uniform in the constructor and also notifies on the
/// stderr, if setting the variable didn't work.
class Uniform : public UniformObject<GLtype> {
public:
    /// Queries a variable named 'identifier' in the 'program', and stores it's location,
    /// or writes to stderr if it didn't work. Also changes the currently active program to
    /// the program given as a parameter.
    /// @param program - The program to seek the uniform in. Will call program.Use().
    /// @param identifier - The name of the uniform that is to be set.
    /// @see glGetUniformLocation
    Uniform(Program& program, const std::string& identifier) {
        program.Use();
        UniformObject<GLtype>::location = glGetUniformLocation(program.Expose(), identifier.c_str());

        if(UniformObject<GLtype>::location == INVALID_LOCATION) {
            std::cerr << "Error getting the location of uniform '" << identifier << "'" << std::endl;
        }
        oglwrap_CheckError();
    }

    /// Sets the uniform to value if it is an OpenGL type or a glm vector or matrix.
    /// It throws std::invalid_argument if it is an unrecognized type.
    /// @param value - Specifies the new value to be used for the uniform variable.
    /// @see glUniform*
    void Set(const GLtype& value) {
        UniformObject<GLtype>::Set(value);
        oglwrap_CheckError();
    }

    /// Sets the uniform to value if it is an OpenGL type or a glm vector or matrix.
    /// It throws std::invalid_argument if it is an unrecognized type.
    /// @param value - Specifies the new value to be used for the uniform variable.
    /// @see glUniform*
    void operator=(const GLtype& value) {
        Set(value);
    }
};

typedef Uniform<GLint> UniformSampler;


// -------======{[ LazyUniform ]}======-------

template<typename GLtype>
/// LazyUniform is used to set uniform variables in shaders. It takes
/// a program reference and the uniform's name as a constructor, but
/// but the program doesn't have to be valid at that time. The variable's
/// locations will only be queried at the first Set call (or op=), so
/// it have to query the location all the time, like how the normal
/// Uniform class works. It also has the advantage, that you only have
/// to write program's and the uniform's name once, not matter how many
/// times you set it.
class LazyUniform : public UniformObject<GLtype> {
    Program& program; ///< The program in which the uniform should be set.
    const std::string identifier; ///< The uniform's name.
public:
    /// Stores the uniform's information. It will only be used at the first
    /// Set or op= call, so the program doesn't have to be valid at the time
    /// this constructor is called.
    /// @param program - The program in which the uniform is to be set.
    /// @param identifier - The uniform's name.
    LazyUniform(Program& program, const std::string& identifier)
        : UniformObject<GLtype>(INVALID_LOCATION)
        , program(program)
        , identifier(identifier)
    { }

    /// At the first call, queries the uniform's location. It writes to stderr if it was unable to get it.
    /// At every call it sets the uniform to the specified value. It also changes the active program
    /// to the one specified in the constructor.
    /// @param Specifies the new value to be used for the uniform variable.
    void Set(const GLtype& value) {
        program.Use();

        // Get the uniform's location only at the first Set call.
        static bool firstCall = true;
        if(firstCall) {
            UniformObject<GLtype>::location = glGetUniformLocation(program.Expose(), identifier.c_str());
            firstCall = true;
        }

        // Check if it worked.
        if(UniformObject<GLtype>::location == INVALID_LOCATION) {
            std::cerr << "Error getting the location of uniform '" << identifier << "'" << std::endl;
        }

        UniformObject<GLtype>::Set(value);
        oglwrap_CheckError();
    }

    /// At the first call, queries the uniform's location. It writes to stderr if it was unable to get it.
    /// At every call it sets the uniform to the specified value. It also changes the active program
    /// to the one specified in the constructor.
    /// @param Specifies the new value to be used for the uniform variable.
    void operator=(const GLtype& value) {
        Set(value);
    }
};

typedef LazyUniform<GLint> LazyUniformSampler;


// -------======{[ UniformObject::Set specializations ]}======-------

template<>
inline void UniformObject<GLfloat>::Set(const GLfloat& value) {
    glUniform1f(location, value);
};

template<>
inline void UniformObject<GLdouble>::Set(const GLdouble& value) {
    glUniform1d(location, value);
};

template<>
inline void UniformObject<GLint>::Set(const GLint& value) {
    glUniform1i(location, value);
};

template<>
inline void UniformObject<GLuint>::Set(const GLuint& value) {
    glUniform1ui(location, value);
};

template<>
inline void UniformObject<glm::vec2>::Set(const glm::vec2& vec) {
    glUniform2fv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::dvec2>::Set(const glm::dvec2& vec) {
    glUniform2dv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::ivec2>::Set(const glm::ivec2& vec) {
    glUniform2iv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::uvec2>::Set(const glm::uvec2& vec) {
    glUniform2uiv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::vec3>::Set(const glm::vec3& vec) {
    glUniform3fv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::dvec3>::Set(const glm::dvec3& vec) {
    glUniform3dv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::ivec3>::Set(const glm::ivec3& vec) {
    glUniform3iv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::uvec3>::Set(const glm::uvec3& vec) {
    glUniform3uiv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::vec4>::Set(const glm::vec4& vec) {
    glUniform4fv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::dvec4>::Set(const glm::dvec4& vec) {
    glUniform4dv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::ivec4>::Set(const glm::ivec4& vec) {
    glUniform4iv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::uvec4>::Set(const glm::uvec4& vec) {
    glUniform4uiv(location, 1, glm::value_ptr(vec));
};

template<>
inline void UniformObject<glm::mat2>::Set(const glm::mat2& mat) {
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(mat));
};

template<>
inline void UniformObject<glm::dmat2>::Set(const glm::dmat2& mat) {
    glUniformMatrix2dv(location, 1, GL_FALSE, glm::value_ptr(mat));
};

template<>
inline void UniformObject<glm::mat3>::Set(const glm::mat3& mat) {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
};

template<>
inline void UniformObject<glm::dmat3>::Set(const glm::dmat3& mat) {
    glUniformMatrix3dv(location, 1, GL_FALSE, glm::value_ptr(mat));
};

template<>
inline void UniformObject<glm::mat4>::Set(const glm::mat4& mat) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
};

template<>
inline void UniformObject<glm::dmat4>::Set(const glm::dmat4& mat) {
    glUniformMatrix4dv(location, 1, GL_FALSE, glm::value_ptr(mat));
};

} // namespace oglwrap

#endif // UNIFORM_HPP
