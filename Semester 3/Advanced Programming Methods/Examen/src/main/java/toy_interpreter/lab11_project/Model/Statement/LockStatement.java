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

public class LockStatement implements IStatement {
    private final String var;
    private static final Lock lock = new ReentrantLock();

    public LockStatement(String var) {
        this.var = var;
    }

    @Override
    public ProgramState execute(ProgramState state) throws MyException {
        lock.lock();
        IDictionary<String, IValue> symTable = state.getSymbolTable();
        ILockTable lockTable = state.getLockTable();
        if (symTable.isDefined(var)) {
            if (symTable.lookUp(var).getType().equals(new IntType())) {
                IntValue fi = (IntValue) symTable.lookUp(var);
                int foundIndex = fi.getValue();
                if (lockTable.containsKey(foundIndex)) {
                    if (lockTable.get(foundIndex) == -1) {
                        lockTable.update(foundIndex, state.getId());
                        state.setLockTable(lockTable);
                    } else {
                        state.getExecutionStack().push(this);
                    }
                } else {
                    throw new MyException("Index is not in the lock table!");
                }
            } else {
                throw new MyException("Var is not of Types int!");
            }
        } else {
            throw new MyException("Variable not defined!");
        }
        lock.unlock();
        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws MyException {
        if (typeEnv.lookUp(var).equals(new IntType())) {
            return typeEnv;
        } else {
            throw new MyException("Var is not of int Types!");
        }
    }

    @Override
    public IStatement deepCopy() {
        return new LockStatement(var);
    }

    @Override
    public String toString() {
        return String.format("lock(%s)", var);
    }
}

