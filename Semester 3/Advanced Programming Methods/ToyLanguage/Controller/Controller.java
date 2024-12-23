package Controller;

import Model.ProgramState.*;
import Model.Values.*;

import Model.Exceptions.*;

import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

import Repository.IRepository;

public class Controller {
    private IRepository repo;
    private ExecutorService executor;

    public Controller(IRepository repo) {
        this.repo = repo;
    }

    public void typeCheck() throws StatementException, ExpressionException, DictionaryException {
        this.repo.getPrgList().get(0).typeCheck();

    }

    public void addProgramState(ProgramState prg) {
        this.repo.addProgramState(prg);
    }

    List<ProgramState> removeCompletedPrograms(List<ProgramState> inProgramList) {
        return inProgramList.stream()
                .filter(ProgramState::isNotCompleted)
                .collect(Collectors.toList());
    }

    public void oneStepForAllPrg(List<ProgramState> programStatesList) throws InterruptedException {

        programStatesList.forEach(prg -> {
            try {
                this.repo.logPrgStateExec(prg);
            } catch (FileException e) {
                e.printStackTrace();
            }
        });

        List<Callable<ProgramState>> callList = programStatesList.stream()
                .map((ProgramState p) -> (Callable<ProgramState>) (p::oneStep))
                .collect(Collectors.toList());
        
        List<ProgramState> newProgramStatesList = this.executor.invokeAll(callList).stream()
                .map(future -> {
                    try {
                        return future.get();
                    } catch (InterruptedException | ExecutionException e) {
                        e.printStackTrace();
                        return null;
                    }
                })
                .filter(Objects::nonNull)
                .collect(Collectors.toList());

        programStatesList.addAll(newProgramStatesList);
        programStatesList.forEach(prg -> {
            try {
                this.repo.logPrgStateExec(prg);
            } catch (FileException e) {
                e.printStackTrace();
            }
        });

        this.repo.setPrgList(programStatesList);

    }

    public void allSteps() throws InterruptedException {
        this.executor = Executors.newFixedThreadPool(2);
        List<ProgramState> prgList = this.removeCompletedPrograms(this.repo.getPrgList());
        while (prgList.size() > 0) {
            this.conservativeGarbageCollector(prgList);
            this.oneStepForAllPrg(prgList);
            prgList = this.removeCompletedPrograms(this.repo.getPrgList());
        }
        this.executor.shutdownNow();
        this.repo.setPrgList(prgList);
    }

    private List<Integer> getAddressesFromSymTable(Collection<IValue> symbolTableValues) {
        return symbolTableValues.stream()
                .filter(value -> value instanceof RefValue)
                .map(v -> {RefValue v1 = (RefValue) v; return v1.getAddress();})
                .collect(Collectors.toList());
    }

    private List<Integer> addIndirectAddresses(List<Integer> symbolTableAddresses, Map<Integer, IValue> heapContent) {
        boolean change = true;
        List<Integer> newAddressList = new ArrayList<>(symbolTableAddresses);
        while (change) {
            List<Integer> appendingList;
            change = false;

            appendingList = heapContent.entrySet().stream()
                    .filter(e -> e.getValue() instanceof RefValue)
                    .filter(e -> newAddressList.contains(e.getKey()))
                    .map(e -> ((RefValue) e.getValue()).getAddress())
                    .filter(e -> !newAddressList.contains(e))
                    .collect(Collectors.toList());
            if (!appendingList.isEmpty()) {
                change = true;
                newAddressList.addAll(appendingList);
            }
        }
        return newAddressList;
    }

    private Map<Integer, IValue> garbageCollector(List<Integer> referencedAddresses, Map<Integer, IValue> heap) {
        return heap.entrySet().stream()
                .filter(e -> referencedAddresses.contains(e.getKey()))
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
    }

    private void conservativeGarbageCollector(List<ProgramState> prgList) {
        Map<Integer, IValue> heapContent = prgList.get(0).getHeapTable().getContent();
        List<IValue> symbolTableValues = prgList.stream().flatMap(prg -> prg.getSymbolTable().getContent().values().stream()).collect(Collectors.toList());
        List<Integer> symbolTableAddresses = this.getAddressesFromSymTable(symbolTableValues);
        List<Integer> allReferencedAddresses = this.addIndirectAddresses(symbolTableAddresses, heapContent);
        prgList.get(0).getHeapTable().setContent(this.garbageCollector(allReferencedAddresses, heapContent));
    }
}
