package Model.Statements;

import Model.Structures.IDictionary;
import Model.Structures.IStack;
import Model.ProgramState.ProgramState;
import Model.Types.IType;

import Model.Exceptions.StatementException;

public class CompStmt implements IStmt{
    private IStmt first;
    private IStmt second;

    public CompStmt(IStmt first, IStmt second) {
        this.first = first;
        this.second = second;
    }

    @Override
    public ProgramState execute(ProgramState state) throws StatementException {
        IStack<IStmt> stack = state.getExecutionStack();
        stack.push(this.second);
        stack.push(this.first);
        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnvironment) throws StatementException, Model.Exceptions.DictionaryException, Model.Exceptions.ExpressionException {
        return this.second.typeCheck(this.first.typeCheck(typeEnvironment));
    }

    @Override
    public IStmt deepCopy() {
        return new CompStmt(this.first.deepCopy(), this.second.deepCopy());
    }

    @Override
    public String toString() {
        return "(" + this.first.toString() + "; " + this.second.toString() + ")";
    }

}
