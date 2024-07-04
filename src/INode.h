#pragma once

#include <vector>
#include <memory>

/**
 * Node interface class. Please do not change this interface.
 */ 
class INode {
public:
    virtual ~INode() = default;
    INode(const INode&) = delete;
    INode& operator=(const INode&) = delete;
    INode(INode&&) noexcept = default;
    INode& operator=(INode&&) noexcept = default;

    /** 
     * Returns the value of the node.
     */
    [[nodiscard]] virtual int value() const = 0;
    /** 
     * Returns a vector of all the children of the node.
     */
    [[nodiscard]] virtual const std::vector<std::unique_ptr<INode>>& children() const = 0;

protected:
    INode() = default;
};
