import java.util.Iterator;


public class EvenIterator implements Iterator<Object>{
	private Integer[] array;
	private int location;
	private int size;
	
	public EvenIterator(Integer[] numbers){
		location = 0;
		array = numbers;
		size = numbers.length;
	}
	
	public boolean hasNext() {
		if(location < size){
			int count = location;
			for(int i = 0; i < size - location; i++){
				if(array[count] % 2 == 0){
					return true;
				}
				
				count++;
			}
		}
		
		return false;
	}

	public Object next() {
		int check = array[location];
		
		while(check%2 != 0){
			location++;
			if(location >= size){
				break;
			}
			
			check = array[location];
		}
		
		location++;
		return check;
	}

	public void remove() {
		throw new UnsupportedOperationException();
	}
	
}
