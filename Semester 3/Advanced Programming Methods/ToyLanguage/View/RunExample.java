package View;

import Controller.Controller;
import Model.Exceptions.DictionaryException;
import Model.Exceptions.ExpressionException;
import Model.Exceptions.StatementException;


public class RunExample extends Command {
    private Controller controller;

    public RunExample(String key, String description, Controller controller) {
        super(key, description);
        this.controller = controller;
    }

    @Override
    public void execute() {
        try {
            this.controller.typeCheck();
            this.controller.allSteps();
        } 
        catch (InterruptedException | StatementException | DictionaryException | ExpressionException e) {
            System.out.println(e.getMessage());
        }
    }

}
