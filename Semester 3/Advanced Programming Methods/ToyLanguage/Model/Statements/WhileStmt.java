package Model.Statements;

import Model.Expressions.IExp;
import Model.ProgramState.ProgramState;
import Model.Types.BoolType;
import Model.Values.BoolValue;
import Model.Values.IValue;
import Model.Types.IType;
import Model.Structures.IDictionary;

import Model.Exceptions.*;

public class WhileStmt implements IStmt {
    private IExp exp;
    private IStmt stmt;

    public WhileStmt(IExp exp, IStmt stmt) {
        this.exp = exp;
        this.stmt = stmt;
    }

    @Override
    public ProgramState execute(ProgramState state) throws StatementException, ExpressionException, DictionaryException, FileException, HeapException {
        IValue cond = this.exp.eval(state.getSymbolTable(), state.getHeapTable());
        if (cond.getType().equals(new BoolType())) {
            if (((BoolValue) cond).getValue()) {
                state.getExecutionStack().push(this);
                state.getExecutionStack().push(this.stmt);
            }
        } else {
            throw new StatementException("Condition is not a boolean value.");
        }
        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws StatementException, DictionaryException, ExpressionException {
        IType typeCond = this.exp.typeCheck(typeEnv);
        if (typeCond.equals(new BoolType())) {
            this.stmt.typeCheck(typeEnv.shallowCopy());
            return typeEnv;
        } else {
            throw new StatementException("Condition is not a boolean.");
        }
    }

    @Override
    public IStmt deepCopy() {
        return new WhileStmt(this.exp.deepCopy(), this.stmt.deepCopy());
    }

    @Override
    public String toString() {
        return "while (" + this.exp.toString() + ") { " + this.stmt.toString() + " }";
    }

}
