package Model.Statements;

import Model.Structures.MyDictionary;
import Model.Structures.MyStack;
import Model.ProgramState.ProgramState;
import Model.Values.IValue;
import Model.Types.IType;
import Model.Structures.IDictionary;


import Model.Exceptions.*;

import java.util.Map;
import java.util.stream.Collectors;


public class ForkStmt implements IStmt{
    private IStmt statement;

    public ForkStmt(IStmt statement) {
        this.statement = statement;
    }

    @Override
    public ProgramState execute(ProgramState currentState) throws StatementException {
        MyStack<IStmt> forkedExecutionStack  = new MyStack<>();

        Map<String, IValue> symbolTableContent = currentState.getSymbolTable().getContent();
        MyDictionary<String, IValue> forkedSymbolTable = new MyDictionary<>();
        forkedSymbolTable.setContent(symbolTableContent.entrySet().stream()
                .collect(Collectors.toMap(e -> e.getKey(), e -> e.getValue().deepCopy())));

        return new ProgramState(forkedExecutionStack, forkedSymbolTable, currentState.getOutput(), currentState.getFileTable(), currentState.getHeapTable(), this.statement);
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnvironment) throws StatementException, DictionaryException, ExpressionException {
        this.statement.typeCheck(typeEnvironment.shallowCopy());
        return typeEnvironment;
    }

    @Override
    public IStmt deepCopy() {
        return new ForkStmt(this.statement.deepCopy());
    }

    @Override
    public String toString() {
        return "fork(" + this.statement.toString() + ")";
    }

}
