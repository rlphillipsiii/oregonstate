
public class IteratorRunner {

	public static void main(String[] args) {
		Integer [] numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 15, 22, 89, 9 };
		EvenIterator iterator = new EvenIterator(numbers);

		while ( iterator.hasNext() ) {
			Integer item = (Integer) iterator.next();
			System.out.println(item);
		}
	}
}