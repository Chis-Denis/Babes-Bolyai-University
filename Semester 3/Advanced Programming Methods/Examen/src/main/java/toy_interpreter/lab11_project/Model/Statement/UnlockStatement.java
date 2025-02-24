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

public class UnlockStatement implements IStatement {
    private final String var;
    private static final Lock lock = new ReentrantLock();

    public UnlockStatement(String var) {
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
                    if (lockTable.get(foundIndex) == state.getId())
                        lockTable.update(foundIndex, -1);
                } else {
                    throw new MyException("Index not in the lock table!");
                }
            } else {
                throw new MyException("Var is not of int Types!");
            }
        } else {
            throw new MyException("Variable is not defined!");
        }
        lock.unlock();
        return null;
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws MyException {
        if (typeEnv.lookUp(var).equals(new IntType()))
            return typeEnv;
        else
            throw new MyException("Var is not of Types int!");
    }

    @Override
    public IStatement deepCopy() {
        return new UnlockStatement(var);
    }

    @Override
    public String toString() {
        return String.format("unlock(%s)", var);
    }
}
