package Model.Structures;

import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

import Model.Exceptions.DictionaryException;

public class MyDictionary<K, V> implements IDictionary<K, V>{
    private HashMap<K, V> elems;

    public MyDictionary() {
        this.elems = new HashMap<K, V>();
    }

    @Override
    public void addKeyValuePair(K newKey, V newValue) {
        this.elems.put(newKey, newValue);   // returns the already existent V value if K was used, otherwise null
    }

    @Override
    public V lookUp(K key) throws DictionaryException {
        if (!this.elems.containsKey(key)) {
            throw new DictionaryException("Failed to get value: key not in dictionary.");
        }
        return this.elems.get(key);
    }

    @Override
    public V removeKey(K key) throws DictionaryException {
        V value = this.elems.remove(key);
        if (value != null) {
            return value;
        }
        else {
            throw new DictionaryException("Failed to remove key: key not in dictionary.");
        }
    }

    @Override
    public boolean isDefined(K key) {
        return this.elems.containsKey(key);
    }

    public Map<K, V> getContent() {
        return this.elems;
    }

    public void setContent(Map<K, V> newContent) {
        this.elems.clear();
        this.elems.putAll(newContent);
    }

    @Override
    public IDictionary<K, V> shallowCopy() {
        Map<K, V> dictionaryContent = this.getContent();
        IDictionary<K, V> newDictionary = new MyDictionary<>();
        newDictionary.setContent(dictionaryContent.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue)));
        return newDictionary;
    }

    @Override
    public String toString() {
        StringBuilder elemsInString = new StringBuilder();
        elemsInString.append("{");
        int i = 0;
        for (Map.Entry<K, V> entry: this.elems.entrySet()) {
            K key = entry.getKey();
            V value = entry.getValue();
            elemsInString.append("\"");
            elemsInString.append(key.toString());
            elemsInString.append("\"");
            elemsInString.append(": ");
            elemsInString.append(value.toString());
            if (i < this.elems.size() - 1) {
                elemsInString.append(", ");
            }
            i += 1;
        }
        elemsInString.append("}");
        return elemsInString.toString();
    }
}
