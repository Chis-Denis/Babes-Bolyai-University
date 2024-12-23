package Model.Structures;

import Model.Exceptions.DictionaryException;

import java.util.Map;

public class MyFileTable<K, V> extends MyDictionary<K, V> {

    @Override
    public synchronized void addKeyValuePair(K newKey, V newValue) {
        super.addKeyValuePair(newKey, newValue);
    }

    @Override
    public synchronized V lookUp(K key) throws DictionaryException {
        return super.lookUp(key);
    }

    @Override
    public synchronized V removeKey(K key) throws DictionaryException {
        return super.removeKey(key);
    }

    @Override
    public synchronized boolean isDefined(K key) {
        return super.isDefined(key);
    }

    @Override
    public synchronized Map<K, V> getContent() {
        return super.getContent();
    }

    @Override
    public synchronized void setContent(Map<K, V> newContent) {
        super.setContent(newContent);
    }

    @Override
    public synchronized String toString() {
        return super.toString();
    }

}
