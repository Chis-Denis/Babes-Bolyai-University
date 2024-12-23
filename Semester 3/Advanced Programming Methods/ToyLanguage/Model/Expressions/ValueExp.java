package Model.Expressions;

import Model.Structures.IDictionary;
import Model.Structures.IHeapTable;
import Model.Values.IValue;
import Model.Types.IType;

import Model.Exceptions.ExpressionException;
import Model.Exceptions.DictionaryException;

public class ValueExp implements IExp {
    private IValue val;

    public ValueExp(IValue v) {
        this.val = v;
    }

    public IValue getVal() {
        return this.val;
    }

    @Override
    public IValue eval(IDictionary<String, IValue> symbolTable, IHeapTable<IValue> heapTable) throws ExpressionException, DictionaryException {
        return this.val;
    }

    @Override
    public IType typeCheck(IDictionary<String, IType> typeEnvironment) throws ExpressionException, DictionaryException {
        return this.val.getType();
    }

    @Override
    public IExp deepCopy() {
        return new ValueExp(this.val.deepCopy());
    }

    @Override
    public String toString() {
        return this.val.toString();
    }
}
