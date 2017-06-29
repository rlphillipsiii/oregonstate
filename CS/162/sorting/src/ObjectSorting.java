
public class ObjectSorting {
	
	public static void main(String[] args) {
		Person[] people = {
				new Person(42, "Joe", "Smith"),
				new Person(11, "Virgina", "Boone"),
				new Person(91, "Jose", "Gonzales"),
				new Person(38, "Cassandra", "Smith"),
				new Person(27, "Robert", "James"),
				new Person(75, "Keisha", "Williams"),
				new Person(66, "Michael", "Yang"),
				new Person(18, "Alex", "Smith")
		};
		
		
		Sorting.quicksort(people);
		
		for (Person p : people) {
			System.out.println(p);
		}
		
//		Sorting s = new Sorting();
//		s.sortTest();
	}
}
