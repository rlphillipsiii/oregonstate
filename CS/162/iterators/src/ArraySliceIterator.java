import java.util.Iterator;

public class ArraySliceIterator<Type> implements Iterator<Type> {
	private int location;
	private int ending;
	private Type[] array;
	
	ArraySliceIterator(Type [] list, int start, int end) {
		array = list;
		location = start;
		ending = end + 1;
	}

	public boolean hasNext() {
		if(location < ending){
			return true;
		}
		
		return false;
	}

	public Type next() {
		Type nextType = array[location];

		location++;

		return nextType;
	}

	public void remove() {
		throw new UnsupportedOperationException();
	}	
}