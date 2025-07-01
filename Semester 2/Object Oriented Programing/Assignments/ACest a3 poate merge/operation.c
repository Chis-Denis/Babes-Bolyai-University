#include "operation.h"

// Creates a new operation record with before and after states for undo/redo
Operation create_operation(OperationType type, Product before, Product after, int is_update) {
    Operation op;
    op.type = type;
    op.before = before;
    op.after = after;
    op.is_update = is_update;
    return op;
} 