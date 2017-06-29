import java.util.Iterator;

public class ArrayIterator<Type> implements Iterator<Type> {
	private int location = 0;
	private int size;
	private Type[] array;
	
	ArrayIterator(Type [] list) {
		size = list.length;
		array = list;
	}

	public boolean hasNext() {
		if(location < size){
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
