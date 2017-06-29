import java.util.Iterator;


public class LinkedListTestCase {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		LinkedList <String> list = new LinkedList <String>();
		list.add("one");
		list.add("two");
		list.add("three");
		list.add("four");
		list.add("five");
		list.insert(1, "insert 1");
		list.insert(3, "insert 3");
		list.remove(0);
		
//		for(int i = 0; i < list.size(); i++){
//			System.out.println(list.get(i));
//		}
		
		Iterator<String> iter = list.iterator();
		
		while(iter.hasNext()){
			System.out.println(iter.next());
		}
		
		System.out.println(list.size());
	}

}
