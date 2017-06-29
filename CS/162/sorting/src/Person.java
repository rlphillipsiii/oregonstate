import java.util.Comparator;


public class Person implements Comparator<Person>{
	private int id;
	private String firstName;
	private String lastName;

	public Person(int id, String firstName, String lastName) {
		this.id = id;
		this.firstName = firstName;
		this.lastName = lastName;
	}

	public int getId() {
		return id;
	}

	public String getFirstName() {
		return firstName;
	}

	public String getLastName() {
		return lastName;
	}

	public String toString() {
		return ""+ id +" "+ firstName +" "+ lastName;
	}

	public int compare(Person p1, Person p2) {		
		if(p1.getFirstName().compareToIgnoreCase(p2.getFirstName()) < 0){
			return -1;
		}
		
		if(p1.getFirstName().compareToIgnoreCase(p2.getFirstName()) > 0){
			return 1;
		}
		
		return 0;
	}
}
