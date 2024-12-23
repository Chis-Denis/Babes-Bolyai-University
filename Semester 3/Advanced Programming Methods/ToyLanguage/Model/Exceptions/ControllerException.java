package Model.Exceptions;

public class ControllerException extends Exception{
    private String msg;

    public ControllerException(String msg) {
        super(msg);
        this.msg = msg;
    }

    public ControllerException() {
        super("Controller operation failed. womp womp");
        this.msg = "Controller operation failed. womp womp";
    }

    @Override
    public String getMessage() {
        return this.msg;
    }

}
