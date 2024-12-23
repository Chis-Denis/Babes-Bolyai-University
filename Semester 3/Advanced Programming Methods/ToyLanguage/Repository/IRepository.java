package Repository;

import Model.ProgramState.ProgramState;

import Model.Exceptions.FileException;

import java.util.List;

public interface IRepository {
    void addProgramState(ProgramState newProgramState);
    void logPrgStateExec(ProgramState programState) throws FileException;
    void setPrgList(List<ProgramState> newProgramStates);
    List<ProgramState> getPrgList();
}
