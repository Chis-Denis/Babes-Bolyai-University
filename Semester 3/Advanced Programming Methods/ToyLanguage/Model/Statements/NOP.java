package Model.Statements;

import Model.ProgramState.ProgramState;
import Model.Structures.IDictionary;
import Model.Exceptions.StatementException;
import Model.Exceptions.ExpressionException;
import Model.Types.IType;

import java.security.Identity;

import Model.Exceptions.DictionaryException;

public class NOP implements IStmt {

    public NOP() {}

    @Override
    public ProgramState execute(ProgramState state) throws StatementException, ExpressionException, DictionaryException {
        return null;
    }

    @Override
    public IStmt deepCopy() {
        return new NOP();
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnvironment) throws StatementException, DictionaryException, ExpressionException {
        return typeEnvironment;
    }

    @Override
    public String toString() {
        return "No OPStatement";
    }
}
