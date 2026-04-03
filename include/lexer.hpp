#pragma once

class Token {
    public:
        enum class Kind {
            RegExp,
            Union,
            Concatination,
            Closure,
            OneOrMore,
            ZeroOrOne,
            LeftParenthesis,
            RightParenthesis,
            Dot,
            EOF,
        };
};
