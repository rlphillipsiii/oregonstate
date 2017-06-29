import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class LinearDictionary extends Dictionary {
	private ArrayList<String> dictionary;

	public LinearDictionary(String path) throws FileNotFoundException{
		dictionary = readFile(path);

		super.time = 0;
		super.searches = 0;
	}
	
	@Override
	public boolean contains(String s) {
		super.searches ++;
		long start = System.currentTimeMillis();
		
		for(int i = 0; i < dictionary.size(); i++)
			if(s.equalsIgnoreCase(dictionary.get(i))){
				super.time += (System.currentTimeMillis() - start);
				
				return true;
			}
		
		
		super.time += (System.currentTimeMillis() - start);
		return false;
	}

	@Override
	public ArrayList<String> readFile(String path) throws FileNotFoundException {
		ArrayList<String> words = new ArrayList<String>();

		Scanner in = new Scanner(new File(path));
		while(in.hasNextLine())
			words.add(in.nextLine());

		in.close();
		return words;
	}
}
