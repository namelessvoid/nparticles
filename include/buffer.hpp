/*
 * Copyright (C) 2015 Simon Kerler
 *
 * This file is part of the "Nameless Particle Engine".
 * For conditions of distribution and use, see the copyright notice you should
 * have recievied with this software. I not, see:
 * http://opensource.org/licenses/Zlib
 */

#ifndef NP_BUFFER_HPP
#define NP_BUFFER_HPP

#include "glutils.hpp"
#include "logger.hpp"

namespace nparticles
{

/**
 * The BufferBase class is a virtual base class for Buffer classes.
 *
 * A buffer is a fixed size storage location residing in GPU memory. It can be used to store all kinds
 * of data (vertex positions, particle ids, etc.).
 *
 * This is a base class for Buffer%s which provides all type independant methods.
 */
class BufferBase
{
public:
    /**
     * The BufferBase constructor.
     *
     * Since buffers are fixed size storage containser, the @p itemCount must be specified.
     *
     * @param itemCount The number of items that are stored in the buffer.
     */
    BufferBase(int itemCount);

    /**
     * The virtual BufferBase destructor.
     */
    virtual ~BufferBase();

    /**
     * Bind the buffer to a target.
     *
     * This method binds a buffer to a given @p target. A buffer can only be bound to one target at a time.
     *
     * @note All previous bindings of this buffer are unbound before the new binding is performed!
     *
     * @param target The OpenGL target (e.g. GL_VERTEX_ARRAY_BUFFER) to which the buffer is bound. This must be
     *               a target which does not support multiple binding points!
     */
    void bind(GLenum target);

    /**
     * Bind the buffer to a target at specific index.
     *
     * This method binds a buffer to a given @p target at @p index. A buffer can only be bound to one
     * target at a time.
     *
     * @note All previous bindings of this buffer ar unbound before the new binding is performed!
     *
     * @param target The OpenGL buffer target (e.g. GL_UNIFORM_BUFFER) to which the buffer should be bound. This must be
     *               a target with multiple binding points!
     * @param index The index of the target on which the buffer should be bound.
     */
    void bindBase(GLenum target, GLuint index);

    /**
     * Unbind the buffer.
     *
     * If the buffer is currently bound to a target, it is unbound.
     */
    void unbind();

    /**
     * Get the number of items in the buffer.
     *
     * The number of items in the buffer is specified by Buffer::setData().
     *
     * @return The number of items stored in the buffer.
     */
    GLsizei getItemCount() const { return mItemCount; }

    /**
     * Get the current binding target.
     *
     * Get the target on which the buffer is currently bound.
     *
     * @return One of the available OpenGL buffer targets.
     */
    GLenum getCurrentBindingTarget() const { return mCurrentTarget; }

    /**
     * Get the current binding index.
     *
     * Get the index of a target on which the buffer is currently bound.
     *
     * @return The current binding index.
     */
    GLuint getCurrentBindingIndex() const { return mCurrentIndex; }

    /**
     * Get the GL type of the buffer.
     *
     * This returns the GL type of the buffer determined by a glutils::GlTypeInfo.
     *
     * @see glutils::GlTypeInfo
     *
     * @return Return the OpenGL type of the buffer (e.g. GL_FLOAT if floats are stored in the buffer).
     */
    GLenum getGlType() const { return mGlTypeInfo.mGlType; }

    /**
     * Get the base size of the Gl type of the buffer.
     *
     * This returns the size of one item in the buffer determined by a glutils::GlTypeInfo.
     *
     * @see glutils::GlTypeInfo
     *
     * @return The base size of one item in the buffer.
     */
    int getGlBaseSize() const { return mGlTypeInfo.mBaseSize; }

protected:
    /**
     * The OpenGL buffer handle.
     *
     * This is the OpenGL buffer handle created by glGenBuffers().
     */
    GLuint mBufferHandle;

    /**
     * The number of items in the buffer.
     *
     * This is the number of items stored in the buffer via Buffer::setData().
     */
    GLsizei mItemCount;

    /**
     * The glutils::GlTypeInfo for the buffer.
     *
     * This is set by Buffer() by either guessing the type via glutils::getGlTypeInfo() or provided
     * manually by the creator of the Buffer.
     */
    glutils::GlTypeInfo mGlTypeInfo;

private:

    /**
     * The current binding state.
     *
     * True, if the buffer is bound, false otherwise.
     */
    bool mCurrentlyBound;

    /**
     * The current binding target.
     *
     * This is the current binding target (if buffer is bound) as specified by bind().
     */
    GLenum mCurrentTarget;

    /**
     * The current binding index.
     *
     * This is the current binding index (if buffer is bound) as specified by bind().
     */
    GLuint mCurrentIndex;

    // Hide copy and asignment operators
    BufferBase(const BufferBase&) = delete;
    void operator=(const BufferBase&) = delete;
};


/**
 * The Buffer class to store data of a specific type.
 *
 * The Buffer class is a subclass of BufferBase. Each Buffer is created with a specific
 * type and size.
 *
 * @tparam T the type of data stored in the Buffer.
 */
template<typename T>
class Buffer : public BufferBase
{
public:
    /**
     * Buffer constructor.
     *
     * The Buffer constructor creates a new OpenGL buffer. The OpenGL type info is either guessed
     * by the template type T or provided directly by the caller.
     *
     * @note If either @p glType or @p glBaseSize is set to GL_INVALID_VALUE and -1 respectively, *both* are guessed from
     *       the template type T!
     *
     * @param itemCount The number of items to store in this buffer.
     * @param glType The OpenGL type for this buffer (e.g. GL_FLOAT for a buffer of floats or glm::vec4).
     *               If set to GL_INVALID_VALUE (the default), the type AND base size is guessed from the given template
     *               type T using glutils::getGlTypeInfo().
     * @param glBaseSize The base size of one item in the buffer (e.g. 4 for glm::vec4). If set to -1 (the default)
     *                   base size AND type are guessed from the given template type T using glutils::getGlTypeInfo().
     * @param usage The OpenGL usage hint how the buffer is commonly used. Its good practice to set this according to the
     *              intended Buffer usage.
     * @param mainTarget The target the Buffer is most commonly bound to. This serves as a optimisation hint for the OpenGL
     *                   implementation, how the buffer is used most of the time. It is not necessary to specify the @p mainTarget,
     *                   but it is a good practice.
     */
    Buffer(int itemCount, GLenum glType = GL_INVALID_VALUE, int glBaseSize = -1, GLenum usage = GL_STATIC_DRAW, GLenum mainTarget = GL_SHADER_STORAGE_BUFFER);

    /**
     * Set the data inside the Buffer.
     *
     * This method is used to set the data of the Buffer. The @p data array must hold as many items as specified on buffer
     * creation, i.e. it must be of length Buffer::getItemCount().
     *
     * @param data Array holding the new data for the Buffer. Must be of length Buffer::getItemCount()!
     */
    void setData(T* data);

    /**
     * Map the buffer to application memory.
     *
     * This method maps the Buffer to application memory so it can be accessed via the returned pointer. The mapped
     * data may be accessed for read and write operations.
     *
     * @note *Always* unmap a buffer as soon as you are finish reading / writing data! A mapped Buffer cannot be used
     * by OpenGL which will lead to undefined behaviour!
     *
     * @return Pointer of template type T to access the buffer data.
     */
    T* map();

    /**
     * Unmap a mapped Buffer.
     *
     * This method unmaps the mapped Buffer
     */
    void unmap();

protected:
    /**
     * Pointer to mapped buffer data.
     *
     * This pointer is returned by the glMap() command to map the buffer data to client memory.
     */
    T* mMapPointer;

    /**
     * The main target this buffer is associated with.
     *
     * This is initialised by the constructor.
     */
    GLenum mMainTarget;
};


// Implementation
template<typename T>
Buffer<T>::Buffer(int itemCount, GLenum glType, int glBaseSize, GLenum usage, GLenum mainTarget)
    : BufferBase(itemCount),
      mMapPointer(nullptr),
      mMainTarget(mainTarget)
{
    // Determine GL type info
    // If not provided by user, try to guess type.
    if(glType == GL_INVALID_VALUE || glBaseSize == -1)
    {
        T testType;
        mGlTypeInfo = glutils::getGlTypeInfo(testType);
    }
    // Type info provided by user
    else
    {
        mGlTypeInfo.mGlType = glType;
        mGlTypeInfo.mBaseSize = glBaseSize;
    }

    // Safe OpenGL buffer binding
    GLint previouslyBoundBuffer = glutils::glGet(mainTarget);

    // Allocate the buffer
    glBindBuffer(mMainTarget, mBufferHandle);
    glBufferData(mMainTarget, itemCount * sizeof(T), nullptr, usage);

    // Restore OpenGL buffer binding
    glBindBuffer(mMainTarget, previouslyBoundBuffer);
}

template<typename T>
void Buffer<T>::setData(T* data)
{
    // Safe OpenGL buffer binding.
    GLuint previouslyBoundBuffer = glutils::glGet(GL_COPY_READ_BUFFER);

    glBindBuffer(GL_COPY_READ_BUFFER, mBufferHandle);
    glBufferSubData(GL_COPY_READ_BUFFER, 0, mItemCount * sizeof(T), data);
    glBindBuffer(GL_COPY_READ_BUFFER, 0);

    // Restore OpenGL buffer binding.
    glBindBuffer(GL_COPY_READ_BUFFER, previouslyBoundBuffer);
}

template<typename T>
T* Buffer<T>::map()
{
    if(mMapPointer)
        return mMapPointer;

    // Safe OpenGL buffer binding.
    GLuint previouslyBoundBuffer = glutils::glGet(GL_COPY_READ_BUFFER);

    glBindBuffer(GL_COPY_READ_BUFFER, mBufferHandle);
    mMapPointer = (T*)glMapBuffer(GL_COPY_READ_BUFFER, GL_READ_WRITE);

    // Restore OpenGL buffer binding.
    glBindBuffer(GL_COPY_READ_BUFFER, previouslyBoundBuffer);

    return mMapPointer;
}

template<typename T>
void Buffer<T>::unmap()
{
    if(!mMapPointer)
        return;

    // Safe OpenGL buffer binding.
    GLuint previouslyBoundBuffer = glutils::glGet(GL_COPY_READ_BUFFER);

    glBindBuffer(GL_COPY_READ_BUFFER, mBufferHandle);
    glUnmapBuffer(GL_COPY_READ_BUFFER);

    // Restore OpenGL buffer binding.
    glBindBuffer(GL_COPY_READ_BUFFER, previouslyBoundBuffer);

    mMapPointer = nullptr;
}

} // namespace nparticles

#endif // NP_BUFFER_HPP
