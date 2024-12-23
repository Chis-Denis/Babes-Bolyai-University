package Model.Structures;

import Model.Exceptions.HeapException;

import java.util.Map;

public interface IHeapTable<V> {

    int addNewHeapEntry(V value);
    V getHeapValue(int address) throws HeapException;
    void updateHeapValue(int address, V newvalue) throws HeapException;
    boolean isDefined(int address);
    void setContent(Map<Integer, V> newContent);
    Map<Integer, V> getContent();
}
