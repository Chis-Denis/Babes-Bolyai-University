package Model.Structures;

import Model.Exceptions.HeapException;

import java.util.Map;
import java.util.HashMap;

public class MyHeapTable<V> implements IHeapTable<V>{

    private HashMap<Integer, V> heapTable;
    private int nextAddress;

    public MyHeapTable() {
        this.heapTable = new HashMap<Integer, V>();
        this.nextAddress = 1;
    }

    @Override
    public int addNewHeapEntry(V value) {
        this.heapTable.put(this.nextAddress, value);
        this.nextAddress++;
        return this.nextAddress - 1;
    }

    @Override
    public V getHeapValue(int address) throws HeapException {
        if (this.heapTable.containsKey(address)) {
            return this.heapTable.get(address);
        }
        throw new HeapException("Address " + address + " is not defined in the heap table.");
    }

    @Override
    public void updateHeapValue(int address, V newvalue) throws HeapException {
        if (this.heapTable.containsKey(address)) {
            this.heapTable.put(address, newvalue);
            return;
        }
        throw new HeapException("Address " + address + " is not defined in the heap table.");
    }

    @Override
    public boolean isDefined(int address) {
        return this.heapTable.containsKey(address);
    }

    @Override
    public void setContent(Map<Integer, V> newContent) {
        this.heapTable.clear();
        this.heapTable = (HashMap<Integer, V>) newContent;
    }

    @Override
    public Map<Integer, V> getContent() {
        return this.heapTable;
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        for (Map.Entry<Integer, V> entry : this.heapTable.entrySet()) {
            result.append(entry.getKey().toString()).append(" -> ").append(entry.getValue().toString()).append("\n");
        }
        return result.toString();
    }

}
