package Model.Types;

import Model.Values.IValue;
import Model.Values.RefValue;

public class RefType implements IType {
    IType inner;

    public RefType(IType inner) {
        this.inner = inner;
    }

    public IType getInner() {
        return this.inner;
    }

    @Override
    public IValue getDefaultValue() {
        return new RefValue(0, this.inner);
    }

    @Override
    public IType deepCopy() {
        return new RefType(this.inner.deepCopy());
    }

    @Override
    public String toString() {
        return "Ref(" + this.inner.toString() + ")";
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof RefType) {
            return this.inner.equals(((RefType) other).getInner());
        }
        return false;
    }

}
