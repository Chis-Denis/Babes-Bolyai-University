package Model.Statements;

import Model.Expressions.IExp;
import Model.ProgramState.ProgramState;
import Model.Types.RefType;
import Model.Values.IValue;
import Model.Values.RefValue;
import Model.Structures.IDictionary;
import Model.Types.IType;

import Model.Exceptions.*;

public class neW implements IStmt{
    private String var_name;
    private IExp exp;

    public neW(String var_name, IExp exp) {
        this.var_name = var_name;
        this.exp = exp;
    }

    @Override
    public ProgramState execute(ProgramState state) throws StatementException, ExpressionException, DictionaryException, FileException, HeapException {
        if(state.getSymbolTable().isDefined(var_name)) {
            IValue value = state.getSymbolTable().lookUp(var_name);

            if(value.getType() instanceof RefType) {
                IValue exp_value = exp.eval(state.getSymbolTable(), state.getHeapTable());
                RefValue ref_value = (RefValue)value;

                if (exp_value.getType().equals(ref_value.getLocationType())) {

                    int address = state.getHeapTable().addNewHeapEntry(exp_value);
                    state.getSymbolTable().addKeyValuePair(var_name, new RefValue(address, ((RefValue)value).getLocationType()));
                    return null;
                }
                throw new StatementException("The expression value is not of the same type as the location type.");
            }
            throw new StatementException("The variable is not of RefType.");
        }
        throw new StatementException("The variable is not defined in the symbol table.");
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws StatementException, DictionaryException, ExpressionException {
        IType typeVar = typeEnv.lookUp(this.var_name);
        IType typeExp = this.exp.typeCheck(typeEnv);

        if (typeVar.equals(new RefType(typeExp))) {
            return typeEnv;
        } 
        else {
            throw new StatementException("Assignment: right hand side and left hand side have different types.");
        }
    }

    @Override
    public String toString() {
        return "new(" + this.var_name + ", " + this.exp.toString() + ")";
    }

    @Override
    public IStmt deepCopy() {
        return new neW(this.var_name, this.exp.deepCopy());
    }

}
