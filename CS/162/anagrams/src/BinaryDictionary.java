import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class BinaryDictionary extends Dictionary {
	private ArrayList<String> dictionary;
	
	public BinaryDictionary(String path) throws FileNotFoundException{
		dictionary = readFile(path);
		
		super.time = 0;
		super.searches = 0;
	}
	
	@Override
	public boolean contains(String s) {
		super.searches ++;
		long start = System.currentTimeMillis();
		
		boolean found = search(s, 0, dictionary.size() - 1);
		super.time += (System.currentTimeMillis() - start);
		
		return found;
	}

	private boolean search(String word, int lower, int upper){
		if(upper - lower < 2)
			return word.equalsIgnoreCase(dictionary.get(upper)) || word.equalsIgnoreCase(dictionary.get(lower));
		
		int mid = (upper + lower)/2;
		String temp = stripDashes(word);
		
		if(temp.compareTo(stripDashes(dictionary.get(mid))) < 0)
			return search(word, lower, mid);
		
		if(temp.compareTo(stripDashes(dictionary.get(mid))) > 0)
			return search(word, mid, upper);
		
		if(word.equalsIgnoreCase(dictionary.get(mid)))
			return true;
		
		boolean search = search(word, lower, mid);
		if(search)
			return true;
		
		return search(word, mid, upper);
	}
	
	private String stripDashes(String word){
		String temp = word;		
		while(temp.contains("-"))
			temp = temp.substring(0, temp.indexOf("-")) + temp.substring(temp.indexOf("-") + 1);
		
		return temp;
	}
	
	public ArrayList<String> readFile(String path) throws FileNotFoundException {
		ArrayList<String> words = new ArrayList<String>();

		Scanner in = new Scanner(new File(path));
		while(in.hasNextLine())
			words.add(in.nextLine());

		in.close();
		return words;
	}
}
