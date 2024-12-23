package View;

import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.*;

public class TextMenu {
    private Map<String, Command> commands;

    public TextMenu() {
        this.commands = new HashMap<String, Command>();
    }

    public void addCommand(Command c) {
        this.commands.put(c.getKey(), c);
    }

    private void printMenu() {
        System.out.println("Available commands:");
        List<String> list = new ArrayList<String>(commands.keySet());
        list
            .stream()
            .sorted(Comparator.comparingInt(Integer::valueOf))
            .forEach(key -> {
                String line = String.format("%4s: %s", key, this.commands.get(key).getDescription());
                System.out.println(line);
            });
    }

    public void deleteExampleFiles() {
        String[] files = {"ex1.txt", "ex2.txt", "ex3.txt", "ex4.txt", "ex5.txt", "ex6.txt", "ex7.txt"};
        
        for (String fileName : files) {
            File file = new File(fileName);
            if (file.exists()) {
                if (file.delete()) {
                    System.out.println("Deleted file: " + fileName);
                } else {
                    System.out.println("Failed to delete file: " + fileName);
                }
            }
        }
    }

    public void show() {
        Scanner scanner = new Scanner(System.in);
        deleteExampleFiles();

        while (true) {
            this.printMenu();
            
            System.out.print("Input the option: ");
            String key = scanner.nextLine();
            Command command = this.commands.get(key);
            if (command == null) {
                System.out.println("Invalid option");
                continue;
            }
            command.execute();
        }
    }
}
