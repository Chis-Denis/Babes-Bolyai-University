package Model.Expressions;

import Model.Structures.*;
import Model.Values.IValue;
import Model.Values.RefValue;
import Model.Types.IType;
import Model.Types.RefType;

import Model.Exceptions.*;

public class readFromHeap implements IExp {
    private IExp exp;

    public readFromHeap(IExp exp) {
        this.exp = exp;
    }

    @Override
    public IValue eval(IDictionary<String, IValue> symbolTable, IHeapTable<IValue> heapTable) throws ExpressionException, DictionaryException, HeapException {
        IValue expressionValue = this.exp.eval(symbolTable, heapTable);
        if (!(expressionValue instanceof RefValue)) {
            throw new ExpressionException("ERROR: The given expression(" + this.exp.toString() + ") does not evaluate to a RefValue.");
        }
        RefValue refValue = (RefValue) expressionValue;
        int address = refValue.getAddress();
        if (!heapTable.isDefined(address)) {
            throw new ExpressionException("ERROR: The address of the given RefValue(" + this.exp.toString() + ") is not available in the heap.");
        }
        return heapTable.getHeapValue(address);
    }

    @Override
    public IType typeCheck(IDictionary<String, IType> typeEnvironment) throws ExpressionException, DictionaryException {
        IType expressionType = this.exp.typeCheck(typeEnvironment);
        if (expressionType instanceof RefType) {
            RefType refType = (RefType) expressionType;
            return refType.getInner();
        }
        throw new ExpressionException("ERROR: The given expression(" + this.exp.toString() + ") does not evaluate to a RefType.");
    }

    @Override
    public String toString() {
        return "readFromHeap(" + this.exp.toString() + ")";
    }

    @Override
    public IExp deepCopy() {
        return new readFromHeap(this.exp.deepCopy());
    }

}
