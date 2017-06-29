import java.util.Iterator;

public class LinkedList <Type> {
	private Node head;
	private Node tail;
	private int size = 0;
	
	public int size(){
		return size;
	}
	
	public void add(Type x) {
		Node n = new Node();
		n.value = x;
		
		if(head == null){
			head = n;
		}else{
			tail.next = n;
		}
		
		tail = n;
		size++;
	}
	
	public Type get(int index){
		if(index >= size || index < 0){
			throw new IndexOutOfBoundsException();
		}
		
		Node pos = head;
		int count = 0;

		while(count < index){
			pos = pos.next;
			count++;
		}

		return pos.value;
	}

	public void remove(int index){
		if(index >= size || index < 0){
			throw new IndexOutOfBoundsException();
		}
		
		if(index == 0){
			head = head.next;
			size--;
			return;
		}
		
		Node pos = head;
		int count = 0;

		while(count < index - 1) {
			pos = pos.next;
			count ++;
		}

		pos.next = pos.next.next;
		size--;
	}

	public void set(int index, Type value){
		if(index >= size || index < 0){
			throw new IndexOutOfBoundsException();
		}
		
		Node pos = head;
		int count = 0;

		while(count < index){
			pos = pos.next;
			count++;
		}
		
		pos.value = value;
	}
	
	public void insert(int index, Type value){
		if(index >= size){
			if(size == 0){
				if(index != 0){
					throw new IndexOutOfBoundsException();
				}
			}else{
				throw new IndexOutOfBoundsException();
			}
		}
		
		if(index < 0){
			throw new IndexOutOfBoundsException();
		}
		
		Node n = new Node();
		n.value = value;
		
		Node pos = head;
		int count = 0;

		while(count < index - 1){
			pos = pos.next;
			count++;
		}
		
		n.next = pos.next;
		pos.next = n;
		
		size++;
	}
	
	public Iterator<Type> iterator() {
		return new LinkedListIterator <Type>();
	}
	
	private class Node {
		public Type value;
		public Node next;
	}
	
	private class LinkedListIterator <InnerType> implements Iterator <InnerType> {
		private int count = 0;
		
		public boolean hasNext() {
			if(count < size){
				return true;
			}
			
			return false;
		}

		@SuppressWarnings("unchecked")
		public InnerType next() {
			Node pos = head;
			
			for(int i = 0; i < count; i++){
				pos = pos.next;
			}
			
			count++;
			return (InnerType) pos.value;
		}

		public void remove() {
			throw new UnsupportedOperationException();			
		}
	}
}
