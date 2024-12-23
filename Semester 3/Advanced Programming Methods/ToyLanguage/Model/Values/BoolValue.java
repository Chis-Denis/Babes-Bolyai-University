package Model.Values;

import Model.Types.BoolType;
import Model.Types.IType;

public class BoolValue implements IValue {
    private boolean value;

    public BoolValue(boolean value) {
        this.value = value;
    }

    public boolean getValue() {
        return this.value;
    }

    @Override
    public IType getType() {
        return new BoolType();
    }

    @Override
    public IValue deepCopy() {
        return new BoolValue(this.value);
    }

    @Override
    public String toString() {
        return Boolean.toString(this.value);
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof BoolValue) {
            return this.value == ((BoolValue) other).getValue();
        }
        return false;
    }

}
