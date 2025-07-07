#pragma once

// Interface for objects that need per-frame updates
class IUpdateable
{
public:
    virtual ~IUpdateable() = default;
    
    virtual void Update(float deltaTime) = 0;
    virtual bool ShouldBeRemoved() const = 0;
};