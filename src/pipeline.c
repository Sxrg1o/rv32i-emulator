#include "pipeline.h"
#include "opparser.h"

/******************* xd *******************/
void execute(InstructionStructure* decoded) {
    decoded->handler(decoded);
}
