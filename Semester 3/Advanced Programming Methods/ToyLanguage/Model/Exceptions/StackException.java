package Model.Exceptions;

public class StackException extends Exception{
    private String msg;

    public StackException(String msg) {
        super(msg);
        this.msg = msg;
    }

    public StackException() {
        super("Stack operation failed womp womp.");
        this.msg = "Stack operation failed womp womp.";
    }

    @Override
    public String getMessage() {
        return this.msg;
    }

}
