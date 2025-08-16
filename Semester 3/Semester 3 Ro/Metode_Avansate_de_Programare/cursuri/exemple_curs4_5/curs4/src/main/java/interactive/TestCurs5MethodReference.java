package interactive;

import java.util.Arrays;
import java.util.List;

public class TestCurs5MethodReference {
    public static void main(String[] args) {
        List<String> l= Arrays.asList("abc","abcd","abd","","def","defg");

        l.removeIf(x->x.startsWith("a"));

        l.removeIf("abcd"::startsWith);

        l.removeIf(String::isEmpty);


    }
}
