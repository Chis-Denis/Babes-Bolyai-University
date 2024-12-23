package Model.Statements;

import Model.Exceptions.DictionaryException;
import Model.Exceptions.ExpressionException;
import Model.Exceptions.StatementException;
import Model.Exceptions.FileException;
import Model.Exceptions.HeapException;
import Model.ProgramState.ProgramState;
import Model.Structures.IDictionary;
import Model.Types.IType;

public interface IStmt {
    String toString();
    ProgramState execute(ProgramState currentState) throws StatementException, ExpressionException, DictionaryException, HeapException ,FileException;
    IStmt deepCopy();
    IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnvironment) throws StatementException, DictionaryException, ExpressionException;
}
