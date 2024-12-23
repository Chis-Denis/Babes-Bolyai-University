package Model.Statements;

import Model.Structures.IDictionary;
import Model.Types.IType;
import Model.Values.IValue;
import Model.ProgramState.ProgramState;

import Model.Exceptions.StatementException;

public class VarDeclStmt implements IStmt{
    private String name;
    private IType type;

    public VarDeclStmt(String name, IType type) {
        this.name = name;
        this.type = type;
    }

    @Override
    public ProgramState execute(ProgramState currentState) throws StatementException {
        IDictionary<String, IValue> symbolTable = currentState.getSymbolTable();
        
        if (symbolTable.isDefined(this.name)) {
            throw new StatementException("Variable " + this.name + " is already declared.");
        } 
        
        else {
            symbolTable.addKeyValuePair(this.name, this.type.getDefaultValue());
        }
        
        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws StatementException {
        typeEnv.addKeyValuePair(this.name, this.type);
        return typeEnv;
    }

    @Override
    public IStmt deepCopy() {
        return new VarDeclStmt(this.name, this.type.deepCopy());
    }

    @Override
    public String toString() {
        return this.type.toString() + " " + this.name;
    }

}
