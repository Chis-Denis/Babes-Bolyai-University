package toy_interpreter.lab11_project.Model.Statement;

import toy_interpreter.lab11_project.Model.Exceptions.MyException;
import toy_interpreter.lab11_project.Model.ProgramState.ProgramState;
import toy_interpreter.lab11_project.Model.Type.IntType;
import toy_interpreter.lab11_project.Model.Type.IType;
import toy_interpreter.lab11_project.Model.ADT.IDictionary;
import toy_interpreter.lab11_project.Model.ADT.ILockTable;
import toy_interpreter.lab11_project.Model.Value.IntValue;
import toy_interpreter.lab11_project.Model.Value.IValue;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class NewLockStatement implements IStatement {
    private final String var;
    private static final Lock lock = new ReentrantLock();

    public NewLockStatement(String var) {
        this.var = var;
    }

    @Override
    public ProgramState execute(ProgramState state) throws MyException {
        lock.lock();
        ILockTable lockTable = state.getLockTable();
        IDictionary<String, IValue> symTable = state.getSymbolTable();
        int freeAddress = lockTable.getFreeValue();
        lockTable.put(freeAddress, -1);
        if (symTable.isDefined(var) && symTable.lookUp(var).getType().equals(new IntType())) {
            symTable.update(var, new IntValue(freeAddress));
        }
        else
            throw new MyException("Variable not declared!");
        lock.unlock();
        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws MyException {
        if (typeEnv.lookUp(var).equals(new IntType()))
            return typeEnv;
        else
            throw new MyException("Var is not of int Types!");
    }

    @Override
    public IStatement deepCopy() {
        return new NewLockStatement(var);
    }

    @Override
    public String toString() {
        return String.format("newLock(%s)", var);
    }
}
