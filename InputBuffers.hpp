#include <bitset>
#include "Window.h"
#include "CommandBuffer.hpp"


struct InputState
{
    std::bitset<512> keys;

    double mouseX = 0.0;
    double mouseY = 0.0;
    double mouseDeltaX = 0.0;
    double mouseDeltaY = 0.0;

    void Clear() noexcept
    {
        keys.reset();
        mouseDeltaX = 0.0;
        mouseDeltaY = 0.0;
    }
};

struct InputBuffers
{
    InputState read;
    InputState write;

    void BeginFrame() noexcept
    {
        write.Clear();
    }

    InputState& Write() noexcept { return write; }
    const InputState& Read() const noexcept { return read; }

    void Swap() noexcept { read = write; }
};