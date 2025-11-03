#include "pipeline.h"
#include "parser.h"

/******************* xd *******************/
void execute(InstructionStructure* decoded) {
    decoded->handler(decoded);
}
