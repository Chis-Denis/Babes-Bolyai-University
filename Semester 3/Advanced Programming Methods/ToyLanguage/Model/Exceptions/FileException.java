package Model.Exceptions;

public class FileException extends Exception{
    private String msg;

    public FileException(String msg) {
        super(msg);
        this.msg = msg;
    }

    public FileException() {
        super("File operation failed. womp womp");
        this.msg = "File operation failed. womp womp";
    }

    @Override
    public String getMessage() {
        return this.msg;
    }

}
