package Model.Exceptions;

public class ExpressionException extends Exception{
    private String msg;

    public ExpressionException(String msg) {
        super(msg);
        this.msg = msg;
    }

    public ExpressionException() {
        super("Expression operation failed. womp womp");
        this.msg = "Expression operation failed. womp womp";
    }

    @Override
    public String getMessage() {
        return this.msg;
    }

}
