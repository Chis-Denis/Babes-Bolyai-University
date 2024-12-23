package Model.Structures;

import java.util.Map;

import Model.Exceptions.DictionaryException;

public interface IDictionary<K, V> {
    void addKeyValuePair(K newKey, V newValue);
    V lookUp(K key) throws DictionaryException;
    V removeKey(K key) throws DictionaryException;
    boolean isDefined(K key);
    Map<K, V> getContent();
    void setContent(Map<K, V> newContent);
    IDictionary<K, V> shallowCopy();
}
