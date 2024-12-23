package Model.Structures;

import java.util.Stack;

import Model.Exceptions.StackException;

public class MyStack<T> implements IStack<T>{
    private Stack<T> elems;

    public MyStack() {
        this.elems = new Stack<T>();
    }

    @Override
    public void push(T newElem) {
        this.elems.push(newElem);
    }

    @Override
    public T pop() throws StackException {
        if (this.elems.empty()) {
            throw new StackException("Failed to pop element: Full stack.");
        }
        return elems.pop();
    }

    @Override
    public boolean isEmpty() {
        return this.elems.empty();
    }

    public Stack<T> getElems() {
        return this.elems;
    }

    @Override
    public String toString() {
        
        StringBuilder elemsInString = new StringBuilder();
        elemsInString.append("|");
        Stack<T> copyElementsReverse = new Stack<T>();
        T topOfStack;

        while (!this.elems.empty()) {
            topOfStack = this.elems.pop();
            copyElementsReverse.push(topOfStack);
            elemsInString.append(topOfStack.toString());
            elemsInString.append(", ");
        }

        String elemsInStringAsString = elemsInString.toString();
        if (elemsInStringAsString.length() > 2) {
            elemsInStringAsString = elemsInStringAsString.substring(0, elemsInStringAsString.length() - 2);
        }
        elemsInStringAsString += "|";

        // restore the stack
        while (!copyElementsReverse.empty()) {
            topOfStack = copyElementsReverse.pop();
            this.elems.push(topOfStack);
        }
        return elemsInStringAsString;
    }
}
