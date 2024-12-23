package Model.Exceptions;

public class DictionaryException extends Exception {
    private String msg;

    public DictionaryException(String msg) {
        super(msg);
        this.msg = msg;
    }

    public DictionaryException() {
        super("Dictionary operation failed. womp womp");
        this.msg = "Dictionary operation failed. womp womp";
    }

    @Override
    public String getMessage() {
        return this.msg;
    }

}
