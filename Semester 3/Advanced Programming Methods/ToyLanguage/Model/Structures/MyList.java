package Model.Structures;

import java.util.ArrayList;

import Model.Exceptions.ListException;

public class MyList<T> implements IList<T> {
    private ArrayList<T> elements;

    public MyList() {
        this.elements = new ArrayList<T>();
    }

    @Override
    public void add(T newElem) {
        this.elements.add(newElem);
    }

    @Override
    public int size() {
        return this.elements.size();
    }

    @Override
    public ArrayList<T> getElems() {
        return this.elements;
    }

    @Override
    public T getElemAtIndex(int index) throws ListException {
        if (index >= this.elements.size()) {
            throw new ListException("Failed to get element: the given index is too large.");
        }
        return this.elements.get(index);
    }

    @Override
    public String toString() {
        StringBuilder elemsInString = new StringBuilder();
        elemsInString.append("[");
        for (int i = 0; i < this.elements.size(); i++) {
            elemsInString.append(this.elements.get(i).toString());
            if (i < this.elements.size() - 1) {
                elemsInString.append(", ");
            }
        }
        elemsInString.append("]");
        return elemsInString.toString();
    }
}
