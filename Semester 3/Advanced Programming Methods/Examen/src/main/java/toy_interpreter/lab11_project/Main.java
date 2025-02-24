package toy_interpreter.lab11_project;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import toy_interpreter.lab11_project.Controller.ListOfProgramsController;
import toy_interpreter.lab11_project.Model.Expression.*;
import toy_interpreter.lab11_project.Model.Statement.*;
import toy_interpreter.lab11_project.Model.Type.BoolType;
import toy_interpreter.lab11_project.Model.Type.IntType;
import toy_interpreter.lab11_project.Model.Type.RefType;
import toy_interpreter.lab11_project.Model.Type.StringType;
import toy_interpreter.lab11_project.Model.Value.BoolValue;
import toy_interpreter.lab11_project.Model.Value.IntValue;
import toy_interpreter.lab11_project.Model.Value.StringValue;

import java.io.IOException;

public class Main extends Application {

    ListOfProgramsController listOfProgramsController;

    @Override
    public void start(Stage stage) {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(Main.class.getResource("list-of-programs.fxml"));
            Parent root = fxmlLoader.load();
            Scene scene = new Scene(root);
            //scene.getStylesheets().add(Objects.requireNonNull(getClass().getResource("style.css")).toExternalForm());
            this.listOfProgramsController = fxmlLoader.getController();
            this.addExamplesToController();
            this.listOfProgramsController.setMainStage(stage);
            stage.setTitle("Hello!");
            stage.setScene(scene);
            stage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        launch();
    }

    private void addExamplesToController() {
        this.listOfProgramsController.addStatement(getEx1());
        this.listOfProgramsController.addStatement(getEx2());
        this.listOfProgramsController.addStatement(getEx3());
        this.listOfProgramsController.addStatement(getEx4());
        this.listOfProgramsController.addStatement(getEx5());
        this.listOfProgramsController.addStatement(getEx6());
        this.listOfProgramsController.addStatement(getEx7());
        this.listOfProgramsController.addStatement(getEx8());
        this.listOfProgramsController.addStatement(getEx9());
        this.listOfProgramsController.addStatement(getEx10());
        this.listOfProgramsController.addStatement(getEx11());
        this.listOfProgramsController.addStatement(getEx12());
        this.listOfProgramsController.addStatement(getEx13());
        this.listOfProgramsController.addStatement(getEx14());
        this.listOfProgramsController.addStatement(getEx15());

    }

    private static IStatement getEx1() {
        return new CompoundStatement(new VariableDeclarationStatement("v", new IntType()),
                new CompoundStatement(new AssignStatement("v", new ValueExpression(new IntValue(2))),
                        new PrintStatement(new VariableExpression("v"))));
    }

    private static IStatement getEx2() {
        return new CompoundStatement(new VariableDeclarationStatement("a", new IntType()),
                new CompoundStatement(new VariableDeclarationStatement("b", new IntType()),
                        new CompoundStatement(new AssignStatement("a", new ArithmeticExpression(new ValueExpression(new IntValue(2)),
                                new ArithmeticExpression(new ValueExpression(new IntValue(3)),
                                        new ValueExpression(new IntValue(5)), "*"), "+")),
                                new CompoundStatement(new AssignStatement("b",
                                        new ArithmeticExpression(new VariableExpression("a"),
                                                new ValueExpression(new IntValue(1)), "+")),
                                        new PrintStatement(new VariableExpression("b"))))));
    }

    private static IStatement getEx3() {
        return new CompoundStatement(new VariableDeclarationStatement("a", new BoolType()),
                new CompoundStatement(new VariableDeclarationStatement("v", new IntType()),
                        new CompoundStatement(new AssignStatement("a", new ValueExpression(new BoolValue(true))),
                                new CompoundStatement(new IfStatement(new VariableExpression("a"), new AssignStatement("v", new ValueExpression(new IntValue(2))),
                                        new AssignStatement("v", new ValueExpression(new IntValue(3)))),
                                        new PrintStatement(new VariableExpression("v"))))));
    }

    private static IStatement getEx4() {
        return new CompoundStatement(new VariableDeclarationStatement("varf", new StringType()),
                new CompoundStatement(new AssignStatement("varf", new ValueExpression(new StringValue("test.in"))),
                        new CompoundStatement(new openRFile(new VariableExpression("varf")),
                                new CompoundStatement(new VariableDeclarationStatement("varc", new IntType()),
                                        new CompoundStatement(new readFile(new VariableExpression("varf"), "varc"),
                                                new CompoundStatement(new PrintStatement(new VariableExpression("varc")),
                                                        new CompoundStatement(new readFile(new VariableExpression("varf"), "varc"),
                                                                new CompoundStatement(new PrintStatement(new VariableExpression("varc")), new closeRFile(new VariableExpression("varf"))))))))));
    }

    private static IStatement getEx5() {
        return new CompoundStatement(new VariableDeclarationStatement("varf", new StringType()),
                new CompoundStatement(new AssignStatement("varf", new ValueExpression(new StringValue("test_empty.in"))),
                        new CompoundStatement(new openRFile(new VariableExpression("varf")),
                                new CompoundStatement(new VariableDeclarationStatement("varc", new IntType()),
                                        new CompoundStatement(new readFile(new VariableExpression("varf"), "varc"),
                                                new CompoundStatement(new PrintStatement(new VariableExpression("varc")),
                                                        new CompoundStatement(new readFile(new VariableExpression("varf"), "varc"),
                                                                new CompoundStatement(new PrintStatement(new VariableExpression("varc")), new closeRFile(new VariableExpression("varf"))))))))));
    }

    private static IStatement getEx6() {
        return new PrintStatement(new RelationalExpression(
                new ArithmeticExpression(new ValueExpression(new IntValue(2)), new ValueExpression(new IntValue(3)), "+"),
                new ValueExpression(new IntValue(1)), "<"));
    }

    private static IStatement getEx7() {
        return new PrintStatement(new RelationalExpression(new ArithmeticExpression(
                new ValueExpression(new IntValue(51)), new ValueExpression(new IntValue(3)), "/"),
                new ArithmeticExpression(new ValueExpression(new IntValue(15)), new ValueExpression(new IntValue(2)), "+"), "=="));
    }

    private static IStatement getEx8() {
        return new CompoundStatement(new VariableDeclarationStatement("v", new RefType(new IntType())),
                new CompoundStatement(new NewStmt("v", new ValueExpression(new IntValue(20))),
                        new CompoundStatement(new VariableDeclarationStatement("a", new RefType(new RefType(new IntType()))),
                                new CompoundStatement(new NewStmt("a", new VariableExpression("v")),
                                        new CompoundStatement(new PrintStatement(new VariableExpression("v")), new PrintStatement(new VariableExpression("a")))))));
    }

    private static IStatement getEx9() {
        return new CompoundStatement(new VariableDeclarationStatement("v", new RefType(new IntType())),
                new CompoundStatement(new NewStmt("v", new ValueExpression(new IntValue(20))),
                        new CompoundStatement(new VariableDeclarationStatement("a", new RefType(new RefType(new IntType()))),
                                new CompoundStatement(new NewStmt("a", new VariableExpression("v")),
                                        new CompoundStatement(new PrintStatement(new readFromHeap(new VariableExpression("v"))),
                                                new PrintStatement(new ArithmeticExpression(new readFromHeap(new readFromHeap(new VariableExpression("a"))), new ValueExpression(new IntValue(5)), "+")))))));
    }

    private static IStatement getEx10() {
        return new CompoundStatement(new VariableDeclarationStatement("v", new RefType(new IntType())),
                new CompoundStatement(new NewStmt("v", new ValueExpression(new IntValue(20))),
                        new CompoundStatement(new PrintStatement(new readFromHeap(new VariableExpression("v"))),
                                new CompoundStatement(new writeToHeap("v", new ValueExpression(new IntValue(30))),
                                        new PrintStatement(new ArithmeticExpression(new readFromHeap(new VariableExpression("v")), new ValueExpression(new IntValue(5)), "+"))))));
    }

    private static IStatement getEx11() {
        return new CompoundStatement(new VariableDeclarationStatement("v", new RefType(new IntType())),
                new CompoundStatement(new NewStmt("v", new ValueExpression(new IntValue(20))),
                        new CompoundStatement(new VariableDeclarationStatement("a", new RefType(new RefType(new IntType()))),
                                new CompoundStatement(new NewStmt("a", new VariableExpression("v")),
                                        new CompoundStatement(new NewStmt("v", new ValueExpression(new IntValue(30))),
                                                new PrintStatement(new readFromHeap(new readFromHeap(new VariableExpression("a")))))))));
    }

    private static IStatement getEx12() {
        return new CompoundStatement(new VariableDeclarationStatement("v", new IntType()),
                new CompoundStatement(new AssignStatement("v", new ValueExpression(new IntValue(4))),
                        new CompoundStatement(new WhileStatement(new RelationalExpression(new VariableExpression("v"), new ValueExpression(new IntValue(0)), ">"),
                                new CompoundStatement(new PrintStatement(new VariableExpression("v")),
                                        new AssignStatement("v", new ArithmeticExpression(new VariableExpression("v"), new ValueExpression(new IntValue(1)), "-")))),
                                new PrintStatement(new VariableExpression("v")))));
    }

    private static IStatement getEx13() {
        return new CompoundStatement(new VariableDeclarationStatement("v", new IntType()),
                new CompoundStatement(new VariableDeclarationStatement("a", new RefType(new IntType())),
                        new CompoundStatement(new AssignStatement("v", new ValueExpression(new IntValue(10))),
                                new CompoundStatement(new NewStmt("a", new ValueExpression(new IntValue(22))),
                                        new CompoundStatement(new forkStatement(new CompoundStatement(new writeToHeap("a", new ValueExpression(new IntValue(30))),
                                                new CompoundStatement(new AssignStatement("v", new ValueExpression(new IntValue(32))),
                                                        new CompoundStatement(new PrintStatement(new VariableExpression("v")), new PrintStatement(new readFromHeap(new VariableExpression("a"))))))),
                                                new CompoundStatement(new PrintStatement(new VariableExpression("v")), new PrintStatement(new readFromHeap(new VariableExpression("a")))))))));
    }

    
    private static IStatement getEx14() {
        return new CompoundStatement(
            new VariableDeclarationStatement("a", new RefType(new IntType())), // Ref int a;
            new CompoundStatement(
                new NewStmt("a", new ValueExpression(new IntValue(20))), // new(a, 20);
                new CompoundStatement(
                    new VariableDeclarationStatement("v", new IntType()), // int v;
                    new CompoundStatement(
                        new AssignStatement("v", new ValueExpression(new IntValue(0))), // v = 0;
                        new CompoundStatement(
                            new WhileStatement(
                                new RelationalExpression(new VariableExpression("v"), new ValueExpression(new IntValue(3)), "<"), // while(v < 3)
                                new CompoundStatement(
                                    new forkStatement(
                                        new CompoundStatement(
                                            new PrintStatement(new VariableExpression("v")), // print(v);
                                            new AssignStatement("v", new ArithmeticExpression(new VariableExpression("v"), new readFromHeap(new VariableExpression("a")), "*")) // v = v * rH(a);
                                        )
                                    ),
                                    new AssignStatement("v", new ArithmeticExpression(new VariableExpression("v"), new ValueExpression(new IntValue(1)), "+")) // v = v + 1;
                                )
                            ),
                            new PrintStatement(new readFromHeap(new VariableExpression("a"))) // print(rH(a));
                        )
                    )
                )
            )
        );
    }

    private static IStatement getEx15() {
        return new CompoundStatement(
            new VariableDeclarationStatement("v1", new RefType(new IntType())), // Ref int v1;
            new CompoundStatement(
                new VariableDeclarationStatement("v2", new RefType(new IntType())), // Ref int v2;
                new CompoundStatement(
                    new VariableDeclarationStatement("x", new IntType()), // int x;
                    new CompoundStatement(
                        new VariableDeclarationStatement("q", new IntType()), // int q;
                        new CompoundStatement(
                            new NewStmt("v1", new ValueExpression(new IntValue(20))), // new(v1, 20);
                            new CompoundStatement(
                                new NewStmt("v2", new ValueExpression(new IntValue(30))), // new(v2, 30);
                                new CompoundStatement(
                                    new NewLockStatement("x"), // newLock(x);
                                    new CompoundStatement(
                                        new forkStatement( // Outer fork (Thread 1)
                                            new CompoundStatement(
                                                new forkStatement( // Inner fork (Thread 1.1)
                                                    new CompoundStatement(
                                                        new LockStatement("x"), // lock(x);
                                                        new CompoundStatement(
                                                            new writeToHeap("v1",
                                                                new ArithmeticExpression(
                                                                    new readFromHeap(new VariableExpression("v1")),
                                                                    new ValueExpression(new IntValue(1)),
                                                                    "-"
                                                                )
                                                            ), // wh(v1, rh(v1) - 1);
                                                            new UnlockStatement("x") // unlock(x);
                                                        )
                                                    )
                                                ),
                                                new CompoundStatement(
                                                    new LockStatement("x"), // lock(x);
                                                    new CompoundStatement(
                                                        new writeToHeap("v1",
                                                            new ArithmeticExpression(
                                                                new readFromHeap(new VariableExpression("v1")),
                                                                new ValueExpression(new IntValue(10)),
                                                                "*"
                                                            )
                                                        ), // wh(v1, rh(v1) * 10);
                                                        new UnlockStatement("x") // unlock(x);
                                                    )
                                                )
                                            )
                                        ),
                                        new CompoundStatement(
                                            new NewLockStatement("q"), // newLock(q);
                                            new CompoundStatement(
                                                new forkStatement( // Outer fork (Thread 2)
                                                    new CompoundStatement(
                                                        new forkStatement( // Inner fork (Thread 2.1)
                                                            new CompoundStatement(
                                                                new LockStatement("q"), // lock(q);
                                                                new CompoundStatement(
                                                                    new writeToHeap("v2",
                                                                        new ArithmeticExpression(
                                                                            new readFromHeap(new VariableExpression("v2")),
                                                                            new ValueExpression(new IntValue(5)),
                                                                            "+"
                                                                        )
                                                                    ), // wh(v2, rh(v2) + 5);
                                                                    new UnlockStatement("q") // unlock(q);
                                                                )
                                                            )
                                                        ),
                                                        new CompoundStatement(
                                                            new LockStatement("q"), // lock(q);
                                                            new CompoundStatement(
                                                                new writeToHeap("v2",
                                                                    new ArithmeticExpression(
                                                                        new readFromHeap(new VariableExpression("v2")),
                                                                        new ValueExpression(new IntValue(10)),
                                                                        "*"
                                                                    )
                                                                ), // wh(v2, rh(v2) * 10);
                                                                new UnlockStatement("q") // unlock(q);
                                                            )
                                                        )
                                                    )
                                                ),
                                                new CompoundStatement(
                                                    new NoOperationStatement(),
                                                    new CompoundStatement(
                                                        new NoOperationStatement(),
                                                        new CompoundStatement(
                                                            new NoOperationStatement(),
                                                            new CompoundStatement(
                                                                new NoOperationStatement(),
                                                                new CompoundStatement(
                                                                    new LockStatement("x"), // lock(x);
                                                                    new CompoundStatement(
                                                                        new PrintStatement(new readFromHeap(new VariableExpression("v1"))), // print(rh(v1));
                                                                        new CompoundStatement(
                                                                            new UnlockStatement("x"), // unlock(x);
                                                                            new CompoundStatement(
                                                                                new LockStatement("q"), // lock(q);
                                                                                new CompoundStatement(
                                                                                    new PrintStatement(new readFromHeap(new VariableExpression("v2"))), // print(rh(v2));
                                                                                    new UnlockStatement("q") // unlock(q);
                                                                                )
                                                                            )
                                                                        )
                                                                    )
                                                                )
                                                            )
                                                        )
                                                    )
                                                )
                                            )
                                        )
                                    )
                                )
                            )
                        )
                    )
                )
            )
        );
    }

}