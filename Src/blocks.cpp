#include "blocks.h"
#include "position.h"

// Local concrete types
namespace
{
    class LConcrete : public Block
    {
    public:
        LConcrete()
        {
            id = 1;
            cells[0] = {Position(0, 2), Position(1, 0), Position(1, 1), Position(1, 2)};
            cells[1] = {Position(0, 1), Position(1, 1), Position(2, 1), Position(2, 2)};
            cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 0)};
            cells[3] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(2, 1)};
            Move(0, 3);
        }
    };

    class JConcrete : public Block
    {
    public:
        JConcrete()
        {
            id = 2;
            cells[0] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(1, 2)};
            cells[1] = {Position(0, 1), Position(0, 2), Position(1, 1), Position(2, 1)};
            cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 2)};
            cells[3] = {Position(0, 1), Position(1, 1), Position(2, 0), Position(2, 1)};
            Move(0, 3);
        }
    };

    class IConcrete : public Block
    {
    public:
        IConcrete()
        {
            id = 3;
            cells[0] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(1, 3)};
            cells[1] = {Position(0, 2), Position(1, 2), Position(2, 2), Position(3, 2)};
            cells[2] = {Position(2, 0), Position(2, 1), Position(2, 2), Position(2, 3)};
            cells[3] = {Position(0, 1), Position(1, 1), Position(2, 1), Position(3, 1)};
            Move(-1, 3);
        }
    };

    class OConcrete : public Block
    {
    public:
        OConcrete()
        {
            id = 4;
            cells[0] = {Position(0, 0), Position(0, 1), Position(1, 0), Position(1, 1)};
            Move(0, 4);
        }
    };

    class SConcrete : public Block
    {
    public:
        SConcrete()
        {
            id = 5;
            cells[0] = {Position(0, 1), Position(0, 2), Position(1, 0), Position(1, 1)};
            cells[1] = {Position(0, 1), Position(1, 1), Position(1, 2), Position(2, 2)};
            cells[2] = {Position(1, 1), Position(1, 2), Position(2, 0), Position(2, 1)};
            cells[3] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(2, 1)};
            Move(0, 3);
        }
    };

    class TConcrete : public Block
    {
    public:
        TConcrete()
        {
            id = 6;
            cells[0] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(1, 2)};
            cells[1] = {Position(0, 1), Position(1, 1), Position(1, 2), Position(2, 1)};
            cells[2] = {Position(1, 0), Position(1, 1), Position(1, 2), Position(2, 1)};
            cells[3] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(2, 1)};
            Move(0, 3);
        }
    };

    class ZConcrete : public Block
    {
    public:
        ZConcrete()
        {
            id = 7;
            cells[0] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(1, 2)};
            cells[1] = {Position(0, 2), Position(1, 1), Position(1, 2), Position(2, 1)};
            cells[2] = {Position(1, 0), Position(1, 1), Position(2, 1), Position(2, 2)};
            cells[3] = {Position(0, 1), Position(1, 0), Position(1, 1), Position(2, 0)};
            Move(0, 3);
        }
    };
}

Block IBlock() { return IConcrete(); }
Block JBlock() { return JConcrete(); }
Block LBlock() { return LConcrete(); }
Block OBlock() { return OConcrete(); }
Block SBlock() { return SConcrete(); }
Block TBlock() { return TConcrete(); }
Block ZBlock() { return ZConcrete(); }

