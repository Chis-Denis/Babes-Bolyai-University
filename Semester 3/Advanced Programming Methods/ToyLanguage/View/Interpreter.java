package View;

import Controller.Controller;
import Model.Expressions.*;
import Model.Statements.*;
import Model.Structures.*;
import Model.Types.*;
import Model.Values.*;
import Repository.IRepository;
import Repository.Repository;
import Model.ProgramState.ProgramState;

import java.io.BufferedReader;

public class Interpreter {
    public static void main(String[] args) {

                // example 1: int v; v=2; Print(v)
        IStmt ex1 = new CompStmt(new VarDeclStmt("v", new IntType()),
                new CompStmt(new AssignStmt("v", new ValueExp(new IntValue(2))),
                        new PrintStmt(new VarExp("v"))));
        ProgramState prg1 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<>() ,ex1);
        IRepository repo1 = new Repository(prg1, "ex1.txt");
        Controller controller1 = new Controller(repo1);

        // example 2: int a;int b; a=2+3*5;b=a+1;Print(b)
        IStmt ex2 = new CompStmt(new VarDeclStmt("a", new IntType()),
                new CompStmt(new VarDeclStmt("b", new IntType()),
                        new CompStmt(new AssignStmt("a", new ArithExp(new ValueExp(new IntValue(2)),
                                new ArithExp(new ValueExp(new IntValue(3)),
                                        new ValueExp(new IntValue(5)), "*"), "+")),
                                new CompStmt(new AssignStmt("b",
                                        new ArithExp(new VarExp("a"),
                                                new ValueExp(new IntValue(1)), "+")),
                                        new PrintStmt(new VarExp("b"))))));
        ProgramState prg2 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<>() ,ex2);
        IRepository repo2 = new Repository(prg2, "ex2.txt");
        Controller controller2 = new Controller(repo2);

        // example 3: bool a; int v; a=true;(If a Then v=2 Else v=3);Print(v)
        IStmt ex3 = new CompStmt(new VarDeclStmt("a", new BoolType()),
                new CompStmt(new VarDeclStmt("v", new IntType()),
                        new CompStmt(new AssignStmt("a", new ValueExp(new BoolValue(true))),
                                new CompStmt(new IfStmt(new VarExp("a"), new AssignStmt("v", new ValueExp(new IntValue(2))),
                                        new AssignStmt("v", new ValueExp(new IntValue(3)))),
                                        new PrintStmt(new VarExp("v"))))));
        ProgramState prg3 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<>() ,ex3);
        IRepository repo3 = new Repository(prg3, "ex3.txt");
        Controller controller3 = new Controller(repo3);

// example 4: string fileName; fileName = "trial1.txt"; openRFile(fileName); int num1; int num2;
// readFile(fileName, num1); readFile(fileName, num2); int result; result = num1 + num2; print(result); closeRFile(fileName)

        IStmt ex4 = new CompStmt(new VarDeclStmt("fileName", new StringType()),
                new CompStmt(new AssignStmt("fileName", new ValueExp(new StringValue("trial1.txt"))),
                        new CompStmt(new openRFile(new VarExp("fileName")),
                                new CompStmt(new VarDeclStmt("num1", new IntType()),
                                        new CompStmt(new VarDeclStmt("num2", new IntType()),
                                                new CompStmt(new readFile(new VarExp("fileName"), "num1"),
                                                        new CompStmt(new readFile(new VarExp("fileName"), "num2"),
                                                                new CompStmt(new VarDeclStmt("result", new IntType()),
                                                                        new CompStmt(new AssignStmt("result", new ArithExp(new VarExp("num1"), new VarExp("num2"), "+")),
                                                                                new CompStmt(new PrintStmt(new VarExp("result")), new closeRFile(new VarExp("fileName"))))))))))));
        ProgramState prg4 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<>() ,ex4);
        IRepository repo4 = new Repository(prg4, "ex4.txt");
        Controller controller4 = new Controller(repo4);


// example 5: string fileName; fileName = "trial2.txt"; openRFile(fileName); int value;
// readFile(fileName, value); IF (value % 2 == 0) THEN print("Even") ELSE print("Odd"); closeRFile(fileName)

        IStmt ex5 = new CompStmt(new VarDeclStmt("fileName", new StringType()),
                new CompStmt(new AssignStmt("fileName", new ValueExp(new StringValue("trial2.txt"))),
                        new CompStmt(new openRFile(new VarExp("fileName")),
                                new CompStmt(new VarDeclStmt("value", new IntType()),
                                        new CompStmt(new readFile(new VarExp("fileName"), "value"),
                                                new CompStmt(new IfStmt(new RelaExp(new ArithExp(new VarExp("value"), new ValueExp(new IntValue(2)), "%"),
                                                        new ValueExp(new IntValue(0)), "=="),
                                                        new PrintStmt(new ValueExp(new StringValue("Even"))),
                                                        new PrintStmt(new ValueExp(new StringValue("Odd")))),
                                                        new closeRFile(new VarExp("fileName"))))))));
        ProgramState prg5 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<>() ,ex5);
        IRepository repo5 = new Repository(prg5, "ex5.txt");
        Controller controller5 = new Controller(repo5);


        // example 6: print(2 + 3 < 1); (false)
        IStmt ex6 = new PrintStmt(new RelaExp(
                new ArithExp(new ValueExp(new IntValue(17)), new ValueExp(new IntValue(100)), "+"),
                new ValueExp(new IntValue(1)), "<"));
        ProgramState prg6 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<>() ,ex6);
        IRepository repo6 = new Repository(prg6, "ex6.txt");
        Controller controller6 = new Controller(repo6);

        // example 7: print(51 / 3 == 15 + 2); (true)
        IStmt ex7 = new PrintStmt(new RelaExp(new ArithExp(
                new ValueExp(new IntValue(300)), new ValueExp(new IntValue(100)), "/"),
                new ArithExp(new ValueExp(new IntValue(1)), new ValueExp(new IntValue(2)), "+"),
                "=="));
        ProgramState prg7 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<>() ,ex7);
        IRepository repo7 = new Repository(prg7, "ex7.txt");
        Controller controller7 = new Controller(repo7);

                // example 8: Ref int v;new(v, 20); Ref Ref int a; new(a, v); print(v); print(a)
                IStmt ex8 = new CompStmt(new VarDeclStmt("v", new RefType(new IntType())),
                new CompStmt(new neW("v", new ValueExp(new IntValue(20))),
                new CompStmt(new VarDeclStmt("a", new RefType(new RefType(new IntType()))),
                new CompStmt(new neW("a", new VarExp("v")),
                new CompStmt(new PrintStmt(new VarExp("v")), new PrintStmt(new VarExp("a")))))));
        ProgramState prg8 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex8);
        IRepository repo8 = new Repository(prg8, "ex8.txt");
        Controller controller8 = new Controller(repo8);

        // example 9: Ref int v; new(v, 20); Ref Ref int a; new(a, v); print(rH(v)); print(rH(rH(a)) + 5)
        IStmt ex9 = new CompStmt(new VarDeclStmt("v", new RefType(new IntType())),
                new CompStmt(new neW("v", new ValueExp(new IntValue(20))),
                new CompStmt(new VarDeclStmt("a", new RefType(new RefType(new IntType()))),
                new CompStmt(new neW("a", new VarExp("v")),
                new CompStmt(new PrintStmt(new readFromHeap(new VarExp("v"))),
                        new PrintStmt(new ArithExp(new readFromHeap(new readFromHeap(new VarExp("a"))), new ValueExp(new IntValue(5)), "+")))))));
        ProgramState prg9 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex9);
        IRepository repo9 = new Repository(prg9, "ex9.txt");
        Controller controller9 = new Controller(repo9);

        // example 10: Ref int v; new(v, 20); print(rH(v)); wH(v, 30); print(rH(v) + 5);
        IStmt ex10 = new CompStmt(new VarDeclStmt("v", new RefType(new IntType())),
                new CompStmt(new neW("v", new ValueExp(new IntValue(20))),
                new CompStmt(new PrintStmt(new readFromHeap(new VarExp("v"))),
                new CompStmt(new writeToHeap("v", new ValueExp(new IntValue(30))),
                        new PrintStmt(new ArithExp(new readFromHeap(new VarExp("v")), new ValueExp(new IntValue(5)), "+"))))));
        ProgramState prg10 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex10);
        IRepository repo10 = new Repository(prg10, "ex10.txt");
        Controller controller10 = new Controller(repo10);

        // example 11: Ref int v; new(v, 20); Ref Ref int a; new(a, v); new(v, 30); print(rH(rH(a)))
        IStmt ex11 = new CompStmt(new VarDeclStmt("v", new RefType(new IntType())),
                new CompStmt(new neW("v", new ValueExp(new IntValue(20))),
                new CompStmt(new VarDeclStmt("a", new RefType(new RefType(new IntType()))),
                new CompStmt(new neW("a", new VarExp("v")),
                new CompStmt(new neW("v", new ValueExp(new IntValue(30))),
                        new PrintStmt(new readFromHeap(new readFromHeap(new VarExp("a")))))))));
        ProgramState prg11 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex11);
        IRepository repo11 = new Repository(prg11, "ex11.txt");
        Controller controller11 = new Controller(repo11);

        // example 12: int v; v=4; (while (v>0) print(v); v=v-1); print(v)
        IStmt ex12 = new CompStmt(new VarDeclStmt("v", new IntType()),
                new CompStmt(new AssignStmt("v", new ValueExp(new IntValue(4))),
                new CompStmt(new WhileStmt(new RelaExp(new VarExp("v"), new ValueExp(new IntValue(0)), ">"),
                                                        new CompStmt(new PrintStmt(new VarExp("v")),
                                                                new AssignStmt("v", new ArithExp(new VarExp("v"), new ValueExp(new IntValue(1)), "-")))),
                                      new PrintStmt(new VarExp("v")))));
        ProgramState prg12 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyDictionary<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex12);
        IRepository repo12 = new Repository(prg12, "ex12.txt");
        Controller controller12 = new Controller(repo12);

                // example 13: int v; Ref int a; v = 10; new(a,22);
        //             fork(wH(a,30); v = 32; print(v); print(rH(a)));
        //             print(v); print(rH(a))
        IStmt ex13 = new CompStmt(new VarDeclStmt("v", new IntType()),
                new CompStmt(new VarDeclStmt("a", new RefType(new IntType())),
                        new CompStmt(new AssignStmt("v", new ValueExp(new IntValue(10))),
                                new CompStmt(new neW("a", new ValueExp(new IntValue(22))),
                                        new CompStmt(new ForkStmt(new CompStmt(new writeToHeap("a", new ValueExp(new IntValue(30))),
                                                new CompStmt(new AssignStmt("v", new ValueExp(new IntValue(32))),
                                                        new CompStmt(new PrintStmt(new VarExp("v")), new PrintStmt(new readFromHeap(new VarExp("a"))))))),
                                                new CompStmt(new PrintStmt(new VarExp("v")), new PrintStmt(new readFromHeap(new VarExp("a")))))))));
        ProgramState prg13 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyFileTable<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex13);
        IRepository repo13 = new Repository(prg13, "ex13.txt");
        Controller controller13 = new Controller(repo13);
        // example 14:
        //Ref (int) a;
        //int v;
        //new(a, 10);
        //fork(
        //	v=20;
        //	fork(
        //		wH(a, 40);
        //		print(rH(a));
        //	);
        //	print(v);
        //	);
        //v=30;
        //print(v);
        //print(rH(a));
        IStmt ex14 = new CompStmt(
                new VarDeclStmt("a", new RefType(new IntType())), // Ref int a
                new CompStmt(
                        new VarDeclStmt("v", new IntType()), // int v
                        new CompStmt(
                                new neW("a", new ValueExp(new IntValue(10))), // new(a, 10)
                                new CompStmt(
                                        new ForkStmt( // Outer fork
                                                new CompStmt(
                                                        new AssignStmt("v", new ValueExp(new IntValue(20))), // v = 20
                                                        new CompStmt(
                                                                new ForkStmt( // Inner fork
                                                                        new CompStmt(
                                                                                new writeToHeap("a", new ValueExp(new IntValue(40))), // wH(a, 40)
                                                                                new PrintStmt(new readFromHeap(new VarExp("a"))) // print(rH(a))
                                                                        )
                                                                ),
                                                                new PrintStmt(new VarExp("v")) // print(v)
                                                        )
                                                )
                                        ),
                                        new CompStmt(
                                                new AssignStmt("v", new ValueExp(new IntValue(30))), // v = 30
                                                new CompStmt(
                                                        new PrintStmt(new VarExp("v")), // print(v)
                                                        new PrintStmt(new readFromHeap(new VarExp("a"))) // print(rH(a))
                                                )
                                        )
                                )
                        )
                )
        );
        // Create ProgramState, Repository, and Controller
        ProgramState prg14 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyFileTable<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex14);
        IRepository repo14 = new Repository(prg14, "ex14.txt");
        Controller controller14 = new Controller(repo14);




        // example 15: string varf; varf = "test.in"; open file varf; fork(int varc; read file(varf, varc); print(varc);); int varc; read file(varf, varc); print(varc); close file(varf);
        IStmt ex15 = new CompStmt(new VarDeclStmt("varf", new StringType()),
                new CompStmt(new AssignStmt("varf", new ValueExp(new StringValue("test.in"))),
                        new CompStmt(new openRFile(new VarExp("varf")),
                                new CompStmt(new ForkStmt(new CompStmt(new VarDeclStmt("varc", new IntType()),
                                        new CompStmt(new readFile(new VarExp("varf"), "varc"),
                                                new PrintStmt(new VarExp("varc"))))),
                                        new CompStmt(new VarDeclStmt("varc", new IntType()),
                                                new CompStmt(new readFile(new VarExp("varf"), "varc"),
                                                        new CompStmt(new PrintStmt(new VarExp("varc")),
                                                                new closeRFile(new VarExp("varf")))))))));
        ProgramState prg15 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyFileTable<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex15);
        IRepository repo15 = new Repository(prg15, "ex15.txt");
        Controller controller15 = new Controller(repo15);

        // Example 16: "bool x; x = 420; print(x);", 
        IStmt ex16 = new CompStmt(
                new VarDeclStmt("x", new BoolType()), // Declare x as a boolean
                new CompStmt(
                        new AssignStmt("x", new ValueExp(new IntValue(5))), // Error: Assigning an integer to a boolean
                        new PrintStmt(new VarExp("x"))
                )
        );


        ProgramState prg16 = new ProgramState(new MyStack<IStmt>(), new MyDictionary<String, IValue>(),
                new MyList<IValue>(), new MyFileTable<StringValue, BufferedReader>(), new MyHeapTable<IValue>(), ex16);
        IRepository repo16 = new Repository(prg16, "ex16.txt");
        Controller controller16 = new Controller(repo16);

        TextMenu menu = new TextMenu();
        menu.addCommand(new ExitCommand("0", "Exit"));
        menu.addCommand(new RunExample("1", "int v; v = 2; print(x)", controller1));
        menu.addCommand(new RunExample("2", "int a; int b; a = 2 + 3 * 5; b = a + 1; print(b)", controller2));
        menu.addCommand(new RunExample("3", "bool a; int v; a = true; IF (a) THEN (v = 2) ELSE (v = 3); print(v)", controller3));
        menu.addCommand(new RunExample("4", "readFile(fileName, num1); readFile(fileName, num2); int result; result = num1 + num2; print(result); closeRFile(fileName)", controller4));
        menu.addCommand(new RunExample("5", "readFile(fileName, value); IF (value % 2 == 0) THEN print(\"Even\") ELSE print(\"Odd\"); closeRFile(fileName)", controller5));
        menu.addCommand(new RunExample("6", "print(17 + 100 < 1);", controller6));
        menu.addCommand(new RunExample("7", "print(300 / 100 == 1 + 2);", controller7));
        menu.addCommand(new RunExample("8", "Ref int v;new(v, 20); Ref Ref int a; new(a, v); print(v); print(a)", controller8));
        menu.addCommand(new RunExample("9", "Ref int v; new(v, 20); Ref Ref int a; new(a, v); print(rH(v)); print(rH(rH(a)) + 5)", controller9));
        menu.addCommand(new RunExample("10", "Ref int v; new(v, 20); print(rH(v)); wH(v, 30); print(rH(v) + 5);", controller10));
        menu.addCommand(new RunExample("11", "Ref int v; new(v, 20); Ref Ref int a; new(a, v); new(v, 30); print(rH(rH(a)))", controller11));
        menu.addCommand(new RunExample("12", "int v; v=4; (while (v>0) print(v); v=v-1); print(v)", controller12));
        menu.addCommand(new RunExample("13", "int v; Ref int a; v = 10; new(a,22); fork(wH(a,30); v = 32; print(v); print(rH(a));); print(v); print(rH(a)", controller13));
        menu.addCommand(new RunExample("14", "Ref (int) a; int v; new(a, 10); fork(v=20; fork(wH(a, 40); print(rH(a));); print(v);); v=30; print(v); print(rH(a);", controller14));
        menu.addCommand(new RunExample("15", "string varf; varf = \"test.in\"; open file varf; fork(int varc; read file(varf, varc); print(varc);); int varc; read file(varf, varc); print(varc); close file(varf);", controller15));
        menu.addCommand(new RunExample("16", "bool x; x = 5; print(x);", controller16));
        

        menu.show();
    }

}
