#include "lexer.h"

#include <variant>
#include <memory>

struct Node;
struct DotNode;
struct LiteralNode;
struct ConcatNode;
struct UnionNode;
struct ClosureNode;
struct OneOrMoreNode;
struct ZeroOrOneNode;

struct SourceSpan {
    size_t start;
    size_t length;
};

using NodeHandler = std::unique_ptr<const Node>;

struct DotNode {};

struct LiteralNode {
    char literal;
};

struct ConcatNode {
    NodeHandler left;
    NodeHandler right;
};

struct UnionNode {
    NodeHandler left;
    NodeHandler right;
};

struct ClosureNode {
    NodeHandler child; 
};

struct OneOrMoreNode {
    NodeHandler child;
};

struct ZeroOrOneNode {
    NodeHandler child;
};

struct Node {
    SourceSpan span;
    std::variant<LiteralNode,
                 DotNode,
                 ConcatNode, 
                 UnionNode,
                 ClosureNode,
                 OneOrMoreNode,
                 ZeroOrOneNode> kind; 
};


