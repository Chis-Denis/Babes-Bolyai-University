package Model.Statements;

import Model.ProgramState.ProgramState;
import Model.Structures.IDictionary;
import Model.Types.IType;
import Model.Types.StringType;
import Model.Values.IValue;
import Model.Values.StringValue;
import Model.Expressions.IExp;

import Model.Exceptions.DictionaryException;
import Model.Exceptions.ExpressionException;
import Model.Exceptions.FileException;
import Model.Exceptions.HeapException;
import Model.Exceptions.StatementException;

import java.io.BufferedReader;
import java.io.IOException;

public class closeRFile implements IStmt {
    private IExp expression;

    public closeRFile(IExp expression) {
        this.expression = expression;
    }

    @Override
    public ProgramState execute(ProgramState state) throws StatementException, ExpressionException, DictionaryException, FileException, HeapException {
        IValue filePath = this.expression.eval(state.getSymbolTable(), state.getHeapTable());

        if (filePath.getType().equals(new StringType())) {
            StringValue stringFilePath = (StringValue) filePath;
            BufferedReader file = state.getFileTable().lookUp(stringFilePath);

            if (!state.getFileTable().isDefined(stringFilePath)) {
                throw new FileException("File path not in the file table.");
            }

            try {
                file.close();
            } 
            
            catch (IOException e) {
                throw new FileException("Error closing file.");
            }

            state.getFileTable().removeKey(stringFilePath);

        } 

        else {
            throw new StatementException("File path is not a string.");
        }

        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws StatementException, DictionaryException, ExpressionException {
        IType type = this.expression.typeCheck(typeEnv);
        if (type.equals(new StringType())) {
            return typeEnv;
        } 
        else {
            throw new StatementException("closeRFile: expression is not a string.");
        }
    }

    @Override
    public IStmt deepCopy() {
        return new closeRFile(this.expression.deepCopy());
    }

    @Override
    public String toString() {
        return "closeRFile(" + this.expression.toString() + ")";
    }

}
