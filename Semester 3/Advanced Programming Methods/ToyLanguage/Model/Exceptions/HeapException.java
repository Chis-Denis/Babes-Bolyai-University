package Model.Exceptions;

public class HeapException extends Exception{
    private String msg;

    public HeapException(String msg) {
        super(msg);
        this.msg = msg;
    }

    public HeapException() {
        super("Heap operation failed. womp womp");
        this.msg = "Heap operation failed. womp womp";
    }

    @Override
    public String getMessage() {
        return this.msg;
    }

}
