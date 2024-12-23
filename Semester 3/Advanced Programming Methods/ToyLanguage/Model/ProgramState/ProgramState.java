package Model.ProgramState;

import Model.Structures.*;
import Model.Exceptions.ControllerException;
import Model.Exceptions.DictionaryException;
import Model.Exceptions.ExpressionException;
import Model.Exceptions.FileException;
import Model.Exceptions.HeapException;
import Model.Exceptions.StackException;
import Model.Exceptions.StatementException;
import Model.Statements.IStmt;
import Model.Values.IValue;
import Model.Values.StringValue;
import Model.Types.IType;

import java.io.BufferedReader;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;


public class ProgramState {
    private IStack<IStmt> executionStack;
    private IDictionary<String, IValue> symbolTable;
    private IDictionary<StringValue, BufferedReader> fileTable;
    private IList<IValue> output;
    private IHeapTable<IValue> heapTable;
    private IStmt originalProgram;
    private int id;
    private static int baseId = 0;
    private IDictionary<String, IType> typeEnvironment;

    public ProgramState(IStack<IStmt> exeStack, IDictionary<String, IValue> symTable, IList<IValue> out, IDictionary<StringValue, BufferedReader> fileTable,                          
                        IHeapTable<IValue> heapTable ,IStmt origPrg) {
        this.executionStack = exeStack;
        this.symbolTable = symTable;
        this.fileTable = fileTable;
        this.output = out;
        this.heapTable = heapTable;
        this.originalProgram = origPrg.deepCopy();
        this.executionStack.push(this.originalProgram);
        this.incrementBaseId();
        this.id = this.getBaseId();
        this.typeEnvironment = new MyDictionary<>();
    }

    public IStack<IStmt> getExecutionStack() {
        return this.executionStack;
    }

    public IDictionary<String, IValue> getSymbolTable() {
        return this.symbolTable;
    }

    public IDictionary<StringValue, BufferedReader> getFileTable() {
        return this.fileTable;
    }

    public IList<IValue> getOutput() {
        return this.output;
    }

    public IHeapTable<IValue> getHeapTable() {
        return this.heapTable;
    }

    public IStmt getOriginalProgram() {
        return this.originalProgram;
    }

    public synchronized int getBaseId() {
        return baseId;
    }

    public synchronized void incrementBaseId() {
        baseId = baseId + 1;    
    }

    public boolean isNotCompleted() {
        return !this.executionStack.isEmpty();
    }

    public ProgramState oneStep() throws ControllerException, StackException, StatementException, HeapException, ExpressionException, DictionaryException, FileException {
        IStack<IStmt> executionStack = this.executionStack;
        if (executionStack.isEmpty()) {
            throw new RuntimeException("Execution stack is empty.");
        }

        IStmt currentStatement = executionStack.pop();
        return currentStatement.execute(this);
    }

    public void typeCheck() throws StatementException, DictionaryException, ExpressionException {
        this.originalProgram.typeCheck(this.typeEnvironment);
    }

    @Override
    public String toString() {
        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
        LocalDateTime now = LocalDateTime.now();
        return  "ID: " + this.id + 
                "\n" +
                dtf.format(now) +
                "\n" +
                "Execution Stack: " + this.executionStack.toString() +
                "\n" +
                "Symbol Table: " + this.symbolTable.toString() +
                "\n" +
                "Heap Table: " + this.heapTable.toString() +
                "\n" +
                "File Table: " + this.fileTable.toString() +
                "\n" +
                "Output: " + this.output.toString() +
                "\n" +
                "~".repeat(50) +
                "\n";

    }
}
