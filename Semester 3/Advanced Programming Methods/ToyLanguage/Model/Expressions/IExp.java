package Model.Expressions;

import Model.Exceptions.ExpressionException;
import Model.Exceptions.HeapException;
import Model.Exceptions.DictionaryException;

import Model.Structures.IDictionary;
import Model.Structures.IHeapTable;
import Model.Values.IValue;
import Model.Types.IType;


public interface IExp {
    IValue eval(IDictionary<String, IValue> symbolTable, IHeapTable<IValue> heapTable) throws ExpressionException, DictionaryException, HeapException;
    IExp deepCopy();
    IType typeCheck(IDictionary<String, IType> typeEnvironment) throws ExpressionException, DictionaryException;
}
