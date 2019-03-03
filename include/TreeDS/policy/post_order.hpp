#pragma once

#include <functional> // std::mem_fn

#include <TreeDS/policy/basic_policy.hpp>
#include <TreeDS/utility.hpp>

namespace md::detail {

template <typename Node, typename Allocator = std::allocator<Node>>
class post_order_impl final : public basic_policy<post_order_impl<Node, Allocator>, Node, Allocator> {

    public:
    using basic_policy<post_order_impl, Node, Allocator>::basic_policy;

    const Node* increment_impl() {
        const Node* prev = this->current;
        const Node* next = this->current->get_parent_limit(*this->root);
        if (prev->is_root_limit(*this->root) || prev->is_last_child()) {
            return next;
        }
        next = prev->get_next_sibling();
        return next
            ? keep_calling(*next, std::mem_fn(&Node::get_first_child))
            : nullptr;
    }

    const Node* decrement_impl() {
        using namespace std::placeholders;
        const Node* result = this->current->get_last_child();
        if (result) {
            return result;
        }
        return keep_calling(
            // from
            *this->current,
            // keep calling
            [&](const Node& node) {
                return node.get_parent_limit(*this->root);
            },
            // until
            [](const Node& child, const Node&) {
                return !child.is_first_child();
            },
            [](const Node& child, const Node&) {
                return child.get_prev_sibling();
            });
    }

    const Node* go_first_impl() {
        return keep_calling(*this->root, std::mem_fn(&Node::get_first_child));
    }

    const Node* go_last_impl() {
        return this->root;
    }
};

} // namespace md::detail

namespace md::policy {
struct post_order : detail::tag<detail::post_order_impl> {
    // what needed is inherited
};

} // namespace md::policy
