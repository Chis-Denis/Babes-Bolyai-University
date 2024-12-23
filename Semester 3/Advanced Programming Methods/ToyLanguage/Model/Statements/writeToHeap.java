package Model.Statements;

import Model.Expressions.IExp;
import Model.ProgramState.ProgramState;
import Model.Types.RefType;
import Model.Values.IValue;
import Model.Values.RefValue;
import Model.Structures.IDictionary;
import Model.Types.IType;

import Model.Exceptions.*;

public class writeToHeap implements IStmt {
    private String varName;
    private IExp exp;

    public writeToHeap(String varName, IExp exp) {
        this.varName = varName;
        this.exp = exp;
    }

    @Override
    public ProgramState execute(ProgramState state) throws StatementException, ExpressionException, DictionaryException, FileException, HeapException {
        if (!state.getSymbolTable().isDefined(this.varName)) {
            throw new StatementException("ERROR: The given variable(" + this.varName + ") is not defined in the symbol table.");
        }
        IValue varValue = state.getSymbolTable().lookUp(this.varName);
        if (!(varValue.getType().equals(new RefType(null)))) {
            throw new StatementException("ERROR: The given variable(" + this.varName + ") is not of type RefType.");
        }
        RefValue refValue = (RefValue) varValue;
        if (!state.getHeapTable().isDefined(refValue.getAddress())) {
            throw new StatementException("ERROR: The address associated with the given variable(" + this.varName + ") is no longer available in the heap.");
        }
        IValue expValue = this.exp.eval(state.getSymbolTable(), state.getHeapTable());
        if (!expValue.getType().equals(refValue.getLocationType())) {
            throw new StatementException("ERROR: The type of the given expression(" + this.exp.toString() + ") does not match with the location type.");
        }
        state.getHeapTable().updateHeapValue(refValue.getAddress(), expValue);
        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws StatementException, DictionaryException, ExpressionException {
        IType typeVar = typeEnv.lookUp(this.varName);
        IType typeExp = this.exp.typeCheck(typeEnv);

        if (typeVar.equals(new RefType(typeExp))) {
            return typeEnv;
        } 
        else {
            throw new StatementException("Assignment: right hand side and left hand side have different types.");
        }
    }

    @Override
    public IStmt deepCopy() {
        return new writeToHeap(this.varName, this.exp);
    }

    @Override
    public String toString() {
        return "WriteToHeap(" + this.varName + ", " + this.exp.toString() + ")";
    }

}
