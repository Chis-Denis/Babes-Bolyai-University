package Model.Expressions;

import Model.Structures.IDictionary;
import Model.Structures.IHeapTable;
import Model.Values.IValue;
import Model.Types.IntType;
import Model.Values.IntValue;
import Model.Values.BoolValue;
import Model.Types.BoolType;
import Model.Types.IType;


import Model.Exceptions.DictionaryException;
import Model.Exceptions.ExpressionException;
import Model.Exceptions.HeapException;


public class RelaExp implements IExp {
    private IExp exp1;
    private IExp exp2;
    private String operation;

    public RelaExp(IExp e1, IExp e2, String op) {
        this.exp1 = e1;
        this.exp2 = e2;
        this.operation = op;
    }

    @Override
    public IValue eval(IDictionary<String, IValue> symbolTable, IHeapTable<IValue> heapTable) throws DictionaryException, ExpressionException, HeapException {
        IValue val1, val2;
        val1 = exp1.eval(symbolTable, heapTable);
        if (val1.getType().equals(new IntType())) {
            val2 = exp2.eval(symbolTable, heapTable);
            if (val2.getType().equals(new IntType())) {
                IntValue intValue1 = (IntValue) val1;
                IntValue intValue2 = (IntValue) val2;
                int i1, i2;
                i1 = intValue1.getValue();
                i2 = intValue2.getValue();

                switch (this.operation) {
                    case "<":
                        return new BoolValue(i1 < i2);
                    case "<=":
                        return new BoolValue(i1 <= i2);
                    case "==":
                        return new BoolValue(i1 == i2);
                    case "!=":
                        return new BoolValue(i1 != i2);
                    case ">":
                        return new BoolValue(i1 > i2);
                    case ">=":
                        return new BoolValue(i1 >= i2);
                    default:
                        throw new ExpressionException("Invalid relational operation given.");
                }
            }
            else {
                throw new ExpressionException("Second operand is not an integer.");
            }
        }
        else {
            throw new ExpressionException("First operand is not an integer.");
        }
    }

    @Override
    public IType typeCheck(IDictionary<String, IType> typeEnvironment) throws ExpressionException, DictionaryException {
        IType type1, type2;
        type1 = this.exp1.typeCheck(typeEnvironment);
        type2 = this.exp2.typeCheck(typeEnvironment);

        if (type1.equals(new IntType())) {
            if (type2.equals(new IntType())) {
                return new BoolType();
            }
            else {
                throw new ExpressionException("Second operand is not an integer.");
            }
        }
        else {
            throw new ExpressionException("First operand is not an integer.");
        }
    }

    @Override
    public IExp deepCopy() {
        return new RelaExp(this.exp1.deepCopy(), this.exp2.deepCopy(), this.operation);
    }

    @Override
    public String toString() {
        return this.exp1.toString() + " " + this.operation + " " + this.exp2.toString();
    }

}
