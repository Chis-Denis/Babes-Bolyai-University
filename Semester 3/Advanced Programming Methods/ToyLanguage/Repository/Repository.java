package Repository;

import Model.Exceptions.FileException;

import Model.ProgramState.ProgramState;

import java.util.ArrayList;
import java.io.*;
import java.util.List;

public class Repository implements IRepository{
    private ArrayList<ProgramState> elements;
    private String logFilePath;
    boolean firstTimeWrite;

    public Repository(ProgramState prgState, String logFilePath) {
        this.elements = new ArrayList<ProgramState>();
        this.elements.add(prgState);
        this.logFilePath = logFilePath;
        this.firstTimeWrite = true;
    }

    @Override
    public void addProgramState(ProgramState newProgramState) {
        this.elements.set(0, newProgramState);
    }

    @Override
    public void logPrgStateExec(ProgramState programState) throws FileException {
        PrintWriter logFile;

        try {
            if (this.firstTimeWrite) {
                logFile = new PrintWriter(new BufferedWriter(new FileWriter(this.logFilePath, false)));
                this.firstTimeWrite = false;
            }

            else {
                logFile = new PrintWriter(new BufferedWriter(new FileWriter(this.logFilePath, true)));
            }
        }

        catch (IOException e) {
            throw new FileException("Could not open/create/exist log file.");
        }

        logFile.println(programState.toString());
        logFile.flush();
        logFile.close();

        System.out.println(programState.toString());
    }

    @Override
    public void setPrgList(List<ProgramState> newProgramStates) {
        this.elements.clear();
        this.elements.addAll(newProgramStates);
    }

    @Override
    public List<ProgramState> getPrgList() {
        return this.elements;
    }

}
