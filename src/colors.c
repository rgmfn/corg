#include "colors.h"
#include "node.h"

int getTypeColor(NodeType type) {
    switch (type) {
        case Todo:
        case Loop:
        case Idea:
        case Proj:
        case Unchecked:
            return GREEN;
        case Done:
        case Yes:
        case Okay:
        case Checked:
            return GRAY;
        case Wait:
        case Hold:
        case Waiting:
            return YELLOW;
        case Kill:
        case No:
            return RED;
        case Strt:
        case Started:
            return MAGENTA;
        default:
            return 0;
    }
}
