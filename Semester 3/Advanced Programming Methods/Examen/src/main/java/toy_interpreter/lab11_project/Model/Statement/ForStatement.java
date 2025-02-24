package toy_interpreter.lab11_project.Model.Statement;

import toy_interpreter.lab11_project.Model.Type.*;
import toy_interpreter.lab11_project.Model.ProgramState.ProgramState;
import toy_interpreter.lab11_project.Model.Expression.*;
import toy_interpreter.lab11_project.Model.ADT.*;
import toy_interpreter.lab11_project.Model.Exceptions.MyException;


public class ForStatement implements IStatement{

    IStatement initialVal;
    IExpression comExpression;
    IStatement increment;
    IStatement body;

    public ForStatement(IStatement initialVal, IExpression comExpression, IStatement increment, IStatement body) {
        this.initialVal = initialVal;
        this.comExpression = comExpression;
        this.increment = increment;
        this.body = body;
    }

    @Override
    public ProgramState execute(ProgramState currentState) throws MyException {
        IStatement stmt1 = this.initialVal;
        WhileStatement stmt2 = new WhileStatement(this.comExpression, new CompoundStatement(this.body, this.increment));
        CompoundStatement newStmt = new CompoundStatement(stmt1, stmt2);
        currentState.getExecutionStack().push(newStmt);
        return null;
    }

    @Override
    public IStatement deepCopy() {
        return new ForStatement(this.initialVal.deepCopy(), this.comExpression.deepCopy(), this.increment.deepCopy(), this.body.deepCopy());
    }

    @Override
    public IDictionary<String, IType> typeCheck(IDictionary<String, IType> typeEnv) throws MyException {
        IType typeExp = this.comExpression.typeCheck(typeEnv);
        if (typeExp.equals(new BoolType())) {
            this.initialVal.typeCheck(typeEnv);
            this.increment.typeCheck(typeEnv);
            this.body.typeCheck(typeEnv);
            return typeEnv;
        } else {
            throw new MyException("TYPE CHECK ERROR: The condition of FOR is not of type bool.");
        }
    }

    @Override
    public String toString() {
        return "FOR (" + this.initialVal.toString() + "; " + this.comExpression.toString() + "; " + this.increment.toString() + ") " + this.body.toString();
    }

}
