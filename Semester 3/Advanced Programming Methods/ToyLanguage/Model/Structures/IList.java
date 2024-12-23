package Model.Structures;

import java.util.ArrayList;

import Model.Exceptions.ListException;

public interface IList<T> {
    void add(T newElem);
    ArrayList<T> getElems();
    T getElemAtIndex(int index) throws ListException;
    int size();
}
